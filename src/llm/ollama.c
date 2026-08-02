#include "ollama.h"
#include "../core/json.h"
#include <windows.h>
#include <winhttp.h>

// ================= low-level HTTP =================

// POST/GET helper. Returns malloc'd body on success, NULL on failure. *out_status receives HTTP status.
static char *http_req(const char *url, const char *method, const char *body, size_t body_len, long *out_status) {
    if (out_status) *out_status = 0;
    if (!url) return NULL;
    WINHTTP_CURRENT_USER_IE_PROXY_CONFIG pc = { 0 };
    WINHTTP_PROXY_INFO pi = { 0 };
    HINTERNET hSession = NULL, hConnect = NULL, hReq = NULL;
    char *result = NULL;
    URL_COMPONENTS uc;
    memset(&uc, 0, sizeof(uc));
    uc.dwStructSize = sizeof(uc);
    wchar_t wurl[1024];
    if (MultiByteToWideChar(CP_UTF8, 0, url, -1, wurl, 1024) == 0) return NULL;
    if (!WinHttpCrackUrl(wurl, 0, 0, &uc)) return NULL;

    wchar_t host[512], path[2048];
    int hostlen = uc.dwHostNameLength < 510 ? uc.dwHostNameLength : 510;
    wcsncpy_s(host, 512, uc.lpszHostName, hostlen); host[hostlen] = 0;
    int pathlen = uc.dwUrlPathLength + uc.dwExtraInfoLength;
    if (pathlen > 2040) pathlen = 2040;
    if (pathlen > 0) wcsncpy_s(path, 2048, uc.lpszUrlPath, pathlen);
    path[pathlen] = 0;
    if (path[0] == 0) wcscpy_s(path, 2048, L"/");
    INTERNET_PORT port = uc.nPort;

    hSession = WinHttpOpen(L"Volt360/0.1", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (!hSession) goto done;
    hConnect = WinHttpConnect(hSession, host, port, 0);
    if (!hConnect) goto done;
    hReq = WinHttpOpenRequest(hConnect, method, path, NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, uc.nScheme == INTERNET_SCHEME_HTTPS ? WINHTTP_FLAG_SECURE : 0);
    if (!hReq) goto done;
    if (body) {
        char hdrs[] = "Content-Type: application/json\r\n";
        if (!WinHttpSendRequest(hReq, hdrs, (DWORD)strlen(hdrs), (LPVOID)body, (DWORD)body_len, (DWORD)body_len, 0)) goto done;
    } else {
        if (!WinHttpSendRequest(hReq, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0)) goto done;
    }
    if (!WinHttpReceiveResponse(hReq, NULL)) goto done;

    DWORD status = 0, sz = sizeof(status);
    if (out_status) {
        if (WinHttpQueryHeaders(hReq, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER, WINHTTP_HEADER_NAME_BY_INDEX, &status, &sz, WINHTTP_NO_HEADER_INDEX)) *out_status = (long)status;
    }
    Buf b; buf_init(&b);
    DWORD available = 0, read = 0;
    do {
        if (!WinHttpQueryDataAvailable(hReq, &available)) break;
        if (available == 0) break;
        char tmp[16384];
        DWORD n = available > sizeof(tmp) ? (DWORD)sizeof(tmp) : available;
        if (!WinHttpReadData(hReq, tmp, n, &read) || read == 0) break;
        buf_append(&b, tmp, read);
    } while (read > 0);
    result = str_dup(buf_cstr(&b));
    buf_free(&b);
done:
    if (hReq) WinHttpCloseHandle(hReq);
    if (hConnect) WinHttpCloseHandle(hConnect);
    if (hSession) WinHttpCloseHandle(hSession);
    (void)pc; (void)pi;
    return result;
}

// ================= public API =================

bool ollama_ping(const char *url) {
    char *models = ollama_models(url);
    bool ok = models != NULL && models[0] != 0;
    free(models);
    return ok;
}

char *ollama_models(const char *url) {
    if (!url) return NULL;
    long status = 0;
    char *body = http_req(url, "GET", NULL, 0, &status);
    if (!body || status != 200) { free(body); return NULL; }
    JsonValue *root = json_parse(body, strlen(body));
    free(body);
    if (!root || root->type != J_OBJ) { json_free(root); return NULL; }
    JsonValue *models = json_get_arr(root, "models");
    Buf b; buf_init(&b);
    if (models) {
        for (int i = 0; i < json_arr_len(models); i++) {
            JsonValue *m = json_arr_get(models, i);
            const char *name = json_get_str(m, "name");
            if (name) { if (b.len) buf_append_str(&b, "\n"); buf_append_str(&b, name); }
        }
    }
    char *out = str_dup(buf_cstr(&b));
    buf_free(&b);
    json_free(root);
    return out;
}

// Appends a quoted JSON string into buf (reuses esc-style escaping inline).
static void jstr_into(Buf *b, const char *s) {
    buf_append_str(b, "\"");
    for (const unsigned char *p = (const unsigned char *)s; *p; p++) {
        switch (*p) {
            case '"': buf_append_str(b, "\\\""); break;
            case '\\': buf_append_str(b, "\\\\"); break;
            case '\n': buf_append_str(b, "\\n"); break;
            case '\r': buf_append_str(b, "\\r"); break;
            case '\t': buf_append_str(b, "\\t"); break;
            default:
                if (*p < 0x20) buf_appendf(b, "\\u%04x", *p);
                else buf_append(b, p, 1);
        }
    }
    buf_append_str(b, "\"");
}

static const char *g_extract_prompt =
    "You are an electronics datasheet parser. Extract a component from the datasheet text.\n"
    "Respond with ONLY a JSON object (no markdown, no commentary) with these fields:\n"
    "{ \"name\": string, \"description\": string, \"manufacturer\": string, \"package\": string, "
    "\"footprint\": string, \"size_mm\": { \"x\": number, \"y\": number, \"z\": number }, "
    "\"height_mm\": number, \"specs\": [ { \"key\": string, \"value\": string } ], "
    "\"pins\": [ { \"name\": string, \"number\": string, \"function\": string } ] }\n"
    "Use null for unknown fields. Datasheet text follows:\n\n";

bool ollama_extract_component(const char *url, const char *model, const char *text, const char *doc_id, Component *out) {
    if (!out) return false;
    memset(out, 0, sizeof(*out));
    if (!url || !text) return false;

    Buf req; buf_init(&req);
    buf_append_str(&req, "{\"model\":");
    jstr_into(&req, model ? model : "llama3");
    buf_append_str(&req, ",\"prompt\":");
    jstr_into(&req, g_extract_prompt);
    jstr_into(&req, text);
    buf_append_str(&req, ",\"stream\":false,\"format\":\"json\"}");
    long status = 0;
    char *body = http_req(url, "POST", buf_cstr(&req), buf_cstr(&req) ? strlen(buf_cstr(&req)) : 0, &status);
    buf_free(&req);
    if (!body) return false;

    JsonValue *root = json_parse(body, strlen(body));
    free(body);
    if (!root || root->type != J_OBJ) { json_free(root); return false; }
    const char *resp = json_get_str(root, "response");
    bool ok = false;
    if (resp && resp[0]) {
        JsonValue *comp = json_parse(resp, strlen(resp));
        if (comp && comp->type == J_OBJ) {
            const char *name = json_get_str(comp, "name");
            const char *desc = json_get_str(comp, "description");
            const char *mfr = json_get_str(comp, "manufacturer");
            const char *pkg = json_get_str(comp, "package");
            const char *fp = json_get_str(comp, "footprint");
            out->id = str_dup(make_id());
            out->name = str_dup(name && name[0] ? name : "component");
            out->description = str_dup(desc ? desc : "");
            out->manufacturer = str_dup(mfr ? mfr : "");
            out->package = str_dup(pkg ? pkg : "");
            out->footprint = str_dup(fp ? fp : "");
            out->doc_id = str_dup(doc_id ? doc_id : "");
            out->doc_text = str_dup(text ? text : "");
            if (out->doc_text && strlen(out->doc_text) > 4000) out->doc_text[4000] = 0;

            JsonValue *sz = json_get_obj(comp, "size_mm");
            if (sz) {
                out->size_mm = v3((float)json_get_num(sz, "x"), (float)json_get_num(sz, "y"), (float)json_get_num(sz, "z"));
            }
            out->height_mm = (float)json_get_num(comp, "height_mm");

            JsonValue *specs = json_get_arr(comp, "specs");
            if (specs) {
                int n = json_arr_len(specs);
                out->specs = (Spec *)calloc((size_t)(n ? n : 1), sizeof(Spec));
                out->nspecs = n;
                for (int i = 0; i < n; i++) {
                    JsonValue *s = json_arr_get(specs, i);
                    const char *k = json_get_str(s, "key");
                    const char *v = json_get_str(s, "value");
                    out->specs[i].key = str_dup(k ? k : "");
                    out->specs[i].value = str_dup(v ? v : "");
                }
            }
            JsonValue *pins = json_get_arr(comp, "pins");
            if (pins) {
                int n = json_arr_len(pins);
                out->pins = (PinMap *)calloc((size_t)(n ? n : 1), sizeof(PinMap));
                out->npins = n;
                for (int i = 0; i < n; i++) {
                    JsonValue *pn = json_arr_get(pins, i);
                    const char *nm = json_get_str(pn, "name");
                    const char *num = json_get_str(pn, "number");
                    const char *fn = json_get_str(pn, "function");
                    out->pins[i].name = str_dup(nm ? nm : "");
                    out->pins[i].number = str_dup(num ? num : "");
                    out->pins[i].function = str_dup(fn ? fn : "");
                }
            }
            json_free(comp);
            ok = true;
        } else json_free(comp);
    }
    json_free(root);
    if (!ok) { component_free_contents(out); return false; }
    return true;
}

char *ollama_chat(const char *url, const char *model, const char *user_msg) {
    if (!url || !user_msg) return NULL;
    Buf req; buf_init(&req);
    buf_append_str(&req, "{\"model\":");
    jstr_into(&req, model ? model : "llama3");
    buf_append_str(&req, ",\"messages\":[{\"role\":\"user\",\"content\":");
    jstr_into(&req, user_msg);
    buf_append_str(&req, "}],\"stream\":false}");
    long status = 0;
    char *body = http_req(url, "POST", buf_cstr(&req), strlen(buf_cstr(&req)), &status);
    buf_free(&req);
    if (!body) return NULL;
    JsonValue *root = json_parse(body, strlen(body));
    free(body);
    if (!root || root->type != J_OBJ) { json_free(root); return NULL; }
    const char *resp = json_get_str(root, "response");
    char *out = str_dup(resp ? resp : "");
    json_free(root);
    return out;
}
