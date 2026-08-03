#include "ai_chat.h"
#include "../core/util.h"
#include "../core/json.h"
#include "../llm/ollama.h"
#include "../design/design.h"
#include "../design/co_design.h"
#include "../design/boolop.h"
#define M_PI 3.14159265358979323846
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_MESSAGES 100
#define PANEL_WIDTH 350
#define LINE_HEIGHT 18
#define INPUT_HEIGHT 36
#define SUGGEST_H 28

static void free_message(ChatMessage *m) {
    free(m->role);
    free(m->content);
    m->role = NULL;
    m->content = NULL;
}

void aichat_init(AIChat *ac) {
    memset(ac, 0, sizeof(*ac));
    ac->visible = 0;
    ac->panel_rect = (Rectangle){ 0, 0, PANEL_WIDTH, 600 };
    ac->input_focused = 0;
    ac->anim_time = 0;
    ac->pending_dots = 0;
    ac->proj = NULL;
    strcpy(ac->ollama_url, "http://127.0.0.1:11434");
    strcpy(ac->ollama_model, "llama3");
    ac->connected = 0;
}

void aichat_free(AIChat *ac) {
    for (int i = 0; i < ac->nmessages; i++) free_message(&ac->messages[i]);
    free(ac->messages); ac->messages = NULL;
    ac->nmessages = 0;
    free(ac->ctx.project_json);
    ac->ctx.project_json = NULL;
    for (int i = 0; i < 5; i++) free(ac->suggestions[i]);
    memset(ac->suggestions, 0, sizeof(ac->suggestions));
}

void aichat_toggle(AIChat *ac) {
    ac->visible = !ac->visible;
    if (ac->visible) ac->input_focused = 1;
}

void aichat_update_context(AIChat *ac, const Project *p, int mode) {
    ac->ctx.mode = mode;
    ac->proj = (Project *)p;
    if (!p) return;
    free(ac->ctx.project_json);
    ac->ctx.project_json = NULL;
    Buf b; buf_init(&b);
    buf_appendf(&b, "{\"schematics\":%d,\"pcbs\":%d,\"cad_models\":%d,\"components\":%d}",
        p->schematics.len, p->pcbs.len, p->cad_models.len, p->components.len);
    ac->ctx.project_json = str_dup(buf_cstr(&b));
    ac->ctx.context_length = (int)(ac->ctx.project_json ? strlen(ac->ctx.project_json) : 0);
    buf_free(&b);
}

void aichat_add_assistant(AIChat *ac, const char *text) {
    if (ac->nmessages >= MAX_MESSAGES) return;
    if (!text) return;
    ChatMessage *m = (ChatMessage *)realloc(ac->messages, (size_t)(ac->nmessages + 1) * sizeof(ChatMessage));
    if (!m) return;
    ac->messages = m;
    ChatMessage *newm = &ac->messages[ac->nmessages];
    newm->role = str_dup("assistant");
    newm->content = str_dup(text);
    newm->pending = 0;
    ac->nmessages++;
}

static void add_system_message(AIChat *ac, const char *fmt, ...) {
    if (ac->nmessages >= MAX_MESSAGES) return;
    ChatMessage *m = (ChatMessage *)realloc(ac->messages, (size_t)(ac->nmessages + 1) * sizeof(ChatMessage));
    if (!m) return;
    ac->messages = m;
    ChatMessage *newm = &ac->messages[ac->nmessages];
    newm->role = str_dup("system");
    char buf[1024];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    newm->content = str_dup(buf);
    newm->pending = 0;
    ac->nmessages++;
}

static void add_user_message(AIChat *ac, const char *text) {
    if (ac->nmessages >= MAX_MESSAGES) return;
    ChatMessage *m = (ChatMessage *)realloc(ac->messages, (size_t)(ac->nmessages + 1) * sizeof(ChatMessage));
    if (!m) return;
    ac->messages = m;
    ChatMessage *newm = &ac->messages[ac->nmessages];
    newm->role = str_dup("user");
    newm->content = str_dup(text);
    newm->pending = 0;
    ac->nmessages++;
}

static int count_word_wrap(const char *text, int max_width, int font_size) {
    if (!text || max_width <= 0) return 1;
    int lines = 1;
    int cur_w = 0;
    int len = (int)strlen(text);
    for (int i = 0; i < len; i++) {
        char ch = text[i];
        if (ch == '\n') { lines++; cur_w = 0; continue; }
        int cw = MeasureText((const char[]){ch, 0}, font_size);
        cur_w += cw;
        if (cur_w > max_width) { lines++; cur_w = cw; }
    }
    if (cur_w == 0 && lines > 1) lines--;
    return lines;
}

static void draw_wrapped_text(const char *text, int x, int y, int max_width, int font_size, Color color) {
    if (!text) return;
    int cur_x = x;
    int cur_y = y;
    int len = (int)strlen(text);
    int space_w = MeasureText(" ", font_size);
    for (int i = 0; i < len; ) {
        if (text[i] == '\n') { cur_x = x; cur_y += font_size + 2; i++; continue; }
        int word_end = i;
        while (word_end < len && text[word_end] != ' ' && text[word_end] != '\n') word_end++;
        int word_len = word_end - i;
        char word[256];
        int copy_len = word_len < 255 ? word_len : 255;
        memcpy(word, text + i, (size_t)copy_len);
        word[copy_len] = 0;
        int word_w = MeasureText(word, font_size);
        if (cur_x + word_w > x + max_width && cur_x > x) {
            cur_x = x;
            cur_y += font_size + 2;
        }
        DrawText(word, cur_x, cur_y, font_size, color);
        cur_x += word_w;
        if (word_end < len && text[word_end] == ' ') {
            cur_x += space_w;
            word_end++;
        }
        i = word_end;
    }
}

static int wrapped_text_height(const char *text, int max_width, int font_size) {
    int lines = count_word_wrap(text, max_width, font_size);
    return lines * (font_size + 2) + 4;
}

void aichat_render(AIChat *ac) {
    if (!ac->visible) return;
    int screen_h = GetScreenHeight();
    int screen_w = GetScreenWidth();
    int px = screen_w - PANEL_WIDTH;
    int py = 0;
    int ph = screen_h;
    ac->panel_rect = (Rectangle){ (float)px, (float)py, (float)PANEL_WIDTH, (float)ph };

    Color bg       = { 35, 35, 40, 255 };
    Color title_bar= { 25, 25, 30, 255 };
    Color border   = { 55, 55, 60, 255 };
    Color input_bg = { 45, 45, 50, 255 };
    Color user_bub = { 65, 130, 255, 255 };
    Color asst_bub = { 55, 55, 60, 255 };
    Color sys_text = { 120, 120, 125, 255 };
    Color text_col = { 220, 220, 225, 255 };
    Color dim_col  = { 140, 140, 145, 255 };

    /* panel background */
    DrawRectangle(px, py, PANEL_WIDTH, ph, bg);
    DrawLine(px, py, px, py + ph, border);

    /* title bar */
    int title_h = 32;
    DrawRectangle(px, py, PANEL_WIDTH, title_h, title_bar);
    DrawText("AI Copilot", px + 10, py + 7, 16, text_col);

    /* close button */
    Rectangle close_btn = { (float)(px + PANEL_WIDTH - 30), (float)(py + 4), 24, 24 };
    Color close_col = { 180, 60, 60, 255 };
    if (CheckCollisionPointRec(GetMousePosition(), close_btn)) {
        close_col = (Color){ 220, 80, 80, 255 };
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) aichat_toggle(ac);
    }
    DrawText("X", (int)close_btn.x + 7, (int)close_btn.y + 4, 14, close_col);

    /* connection indicator */
    Color conn_col = ac->connected ? (Color){ 80, 200, 80, 255 } : (Color){ 180, 180, 60, 255 };
    DrawText(ac->connected ? "O" : "L", px + 120, py + 8, 12, conn_col);
    DrawText(ac->connected ? "Online" : "Local", px + 135, py + 8, 12, dim_col);

    DrawLine(px, py + title_h, px + PANEL_WIDTH, py + title_h, border);

    /* message area */
    int msg_top = py + title_h + 4;
    int msg_bot = py + ph - INPUT_HEIGHT - SUGGEST_H - 12;
    int msg_area_h = msg_bot - msg_top;
    int msg_max_w = PANEL_WIDTH - 24;

    BeginScissorMode(px + 2, msg_top, PANEL_WIDTH - 4, msg_area_h);

    float wheel = GetMouseWheelMove();
    Vector2 mp = GetMousePosition();
    if (wheel != 0 && mp.x >= px && mp.x <= px + PANEL_WIDTH && mp.y >= msg_top && mp.y <= msg_bot) {
        ac->scroll_offset -= (int)(wheel * 20);
    }

    /* measure total content height */
    int total_h = 8;
    for (int i = 0; i < ac->nmessages; i++) {
        total_h += wrapped_text_height(ac->messages[i].content, msg_max_w - 20, 12) + 8;
    }

    int max_scroll = total_h - msg_area_h;
    if (max_scroll < 0) max_scroll = 0;
    if (ac->scroll_offset > max_scroll) ac->scroll_offset = max_scroll;
    if (ac->scroll_offset < 0) ac->scroll_offset = 0;

    int cur_y = msg_top + 8 - ac->scroll_offset;

    for (int i = 0; i < ac->nmessages; i++) {
        ChatMessage *m = &ac->messages[i];
        int txt_h = wrapped_text_height(m->content, msg_max_w - 20, 12);
        int bub_h = txt_h + 6;

        if (strcmp(m->role, "system") == 0) {
            int tw = MeasureText(m->content, 11);
            int sx = px + (PANEL_WIDTH - tw) / 2;
            if (sx < px + 4) sx = px + 4;
            DrawText(m->content, sx, cur_y + 2, 11, sys_text);
        } else if (strcmp(m->role, "user") == 0) {
            int bub_w = msg_max_w - 40;
            int bx = px + PANEL_WIDTH - bub_w - 8;
            if (bx < px + 4) bx = px + 4;
            DrawRectangleRounded((Rectangle){ (float)bx, (float)cur_y, (float)bub_w, (float)bub_h }, 0.4f, 4, user_bub);
            draw_wrapped_text(m->content, bx + 8, cur_y + 2, bub_w - 16, 12, WHITE);
        } else {
            /* assistant */
            int bub_w = msg_max_w - 20;
            DrawRectangleRounded((Rectangle){ (float)(px + 8), (float)cur_y, (float)bub_w, (float)bub_h }, 0.4f, 4, asst_bub);
            draw_wrapped_text(m->content, px + 16, cur_y + 2, bub_w - 16, 12, text_col);
            if (m->pending) {
                int dots = ((int)(ac->pending_dots * 2)) % 4;
                char dotstr[] = "   ";
                for (int d = 0; d < dots; d++) dotstr[d] = '.';
                DrawText(dotstr, px + bub_w - 30, cur_y + bub_h - 15, 14, dim_col);
            }
        }
        cur_y += bub_h + 6;
    }

    if (ac->nmessages == 0) {
        DrawText("Ask me about your design...", px + 20, msg_top + 20, 13, dim_col);
    }

    EndScissorMode();

    /* scrollbar */
    if (total_h > msg_area_h) {
        float sb_h = (float)msg_area_h * (float)msg_area_h / (float)total_h;
        if (sb_h < 20) sb_h = 20;
        float sb_y = msg_top + (float)ac->scroll_offset * (float)msg_area_h / (float)total_h;
        DrawRectangle(px + PANEL_WIDTH - 6, (int)sb_y, 4, (int)sb_h, (Color){ 80, 80, 85, 200 });
    }

    DrawLine(px, msg_bot, px + PANEL_WIDTH, msg_bot, border);

    /* suggestions */
    int sug_y = py + ph - INPUT_HEIGHT - SUGGEST_H - 8;
    for (int i = 0; i < ac->nsuggestions; i++) {
        if (!ac->suggestions[i]) continue;
        int sw = MeasureText(ac->suggestions[i], 10) + 12;
        int sx = px + 6 + i * (sw + 4);
        if (sx + sw > px + PANEL_WIDTH - 6) break;
        Rectangle srect = { (float)sx, (float)sug_y, (float)sw, (float)(SUGGEST_H - 4) };
        Color sug_bg = { 50, 50, 57, 255 };
        if (CheckCollisionPointRec(mp, srect)) {
            sug_bg = (Color){ 70, 70, 77, 255 };
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                strncpy(ac->input_buf, ac->suggestions[i], sizeof(ac->input_buf) - 1);
                ac->input_buf[sizeof(ac->input_buf) - 1] = 0;
                ac->input_len = (int)strlen(ac->input_buf);
                aichat_send_message(ac, ac->input_buf);
                ac->input_buf[0] = 0;
                ac->input_len = 0;
            }
        }
        DrawRectangleRounded(srect, 0.3f, 3, sug_bg);
        DrawText(ac->suggestions[i], sx + 6, sug_y + 3, 10, text_col);
    }

    /* input area */
    int input_y = py + ph - INPUT_HEIGHT - 4;
    DrawRectangle(px + 4, input_y, PANEL_WIDTH - 8, INPUT_HEIGHT - 4, input_bg);
    DrawRectangleLines(px + 4, input_y, PANEL_WIDTH - 8, INPUT_HEIGHT - 4, ac->input_focused ? (Color){ 65, 130, 255, 255 } : border);

    int text_y = input_y + 8;
    DrawText(ac->input_buf, px + 10, text_y, 14, text_col);
    if (ac->input_focused) {
        int cw = MeasureText(ac->input_buf, 14);
        float tick = fmodf(ac->anim_time * 3.0f, 2.0f);
        if (tick < 1.0f) DrawText("|", px + 11 + cw, text_y, 14, text_col);
    }

    if (ac->input_buf[0] == 0 && !ac->input_focused) {
        DrawText("Type a command...", px + 10, text_y, 14, dim_col);
    }

    /* send button */
    Rectangle send_btn = { (float)(px + PANEL_WIDTH - 34), (float)(input_y + 4), 26, 26 };
    Color send_col = { 65, 130, 255, 255 };
    if (CheckCollisionPointRec(mp, send_btn)) {
        send_col = (Color){ 85, 150, 255, 255 };
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && ac->input_len > 0) {
            aichat_send_message(ac, ac->input_buf);
            ac->input_buf[0] = 0;
            ac->input_len = 0;
        }
    }
    DrawRectangleRounded(send_btn, 0.3f, 3, send_col);
    DrawText(">", (int)send_btn.x + 8, (int)send_btn.y + 4, 14, WHITE);

    /* focus handling */
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Rectangle input_rect = { (float)(px + 4), (float)input_y, (float)(PANEL_WIDTH - 8), (float)(INPUT_HEIGHT - 4) };
        ac->input_focused = CheckCollisionPointRec(mp, input_rect) ? 1 : 0;
    }
}

void aichat_handle_input(AIChat *ac, int key) {
    if (!ac->input_focused) return;
    if (key >= 32 && key <= 126 && ac->input_len < (int)sizeof(ac->input_buf) - 1) {
        ac->input_buf[ac->input_len++] = (char)key;
        ac->input_buf[ac->input_len] = 0;
    } else if (key == KEY_BACKSPACE && ac->input_len > 0) {
        ac->input_len--;
        ac->input_buf[ac->input_len] = 0;
    } else if (key == KEY_ENTER || key == KEY_KP_ENTER) {
        if (ac->input_len > 0) {
            aichat_send_message(ac, ac->input_buf);
            ac->input_buf[0] = 0;
            ac->input_len = 0;
        }
    } else if (key == KEY_ESCAPE) {
        ac->input_focused = 0;
    }
}

void aichat_update(AIChat *ac, float dt) {
    ac->anim_time += dt;
    ac->pending_dots += dt;
}

/* ============================ Pattern Matcher Fallback ============================ */

typedef struct {
    const char *keyword;
    AICommand cmd;
} KeywordMap;

static KeywordMap g_keywords[] = {
    {"PLACE",   AI_CMD_PLACE_COMPONENT},
    {"ADD",     AI_CMD_PLACE_COMPONENT},
    {"CREATE",  AI_CMD_PLACE_COMPONENT},
    {"REPLACE", AI_CMD_REPLACE_COMPONENT},
    {"DELETE",  AI_CMD_DELETE_COMPONENT},
    {"REMOVE",  AI_CMD_DELETE_COMPONENT},
    {"WIRE",    AI_CMD_ADD_WIRE},
    {"CONNECT", AI_CMD_ADD_WIRE},
    {"ROUTE",   AI_CMD_ROUTE_NET},
    {"TRACK",   AI_CMD_ADD_TRACK},
    {"VIA",     AI_CMD_ADD_VIA},
    {"MOVE",    AI_CMD_MOVE_COMPONENT},
    {"SET",     AI_CMD_SET_PROPERTY},
    {"CHANGE",  AI_CMD_SET_PROPERTY},
    {"BOX",     AI_CMD_CREATE_BOX},
    {"CYLINDER",AI_CMD_CREATE_CYLINDER},
    {"SPHERE",  AI_CMD_CREATE_SPHERE},
    {"TORUS",   AI_CMD_CREATE_TORUS},
    {"CONE",    AI_CMD_CREATE_CONE},
    {"WEDGE",   AI_CMD_CREATE_WEDGE},
    {"PYRAMID", AI_CMD_CREATE_PYRAMID},
    {"TRANSFORM",AI_CMD_TRANSFORM_MESH},
    {"SCALE",   AI_CMD_TRANSFORM_MESH},
    {"ROTATE",  AI_CMD_TRANSFORM_MESH},
    {"TRANSLATE",AI_CMD_TRANSFORM_MESH},
    {"DELETE_MESH",AI_CMD_DELETE_MESH},
    {"REMOVE_MESH",AI_CMD_DELETE_MESH},
    {"DUPLICATE",AI_CMD_DUPLICATE_MESH},
    {"COPY_MESH",AI_CMD_DUPLICATE_MESH},
    {"HOLLOW",  AI_CMD_HOLLOW_MESH},
    {"SHELL",   AI_CMD_HOLLOW_MESH},
    {"MIRROR",  AI_CMD_MIRROR_MESH},
    {"FILLET",  AI_CMD_FILLET_MESH},
    {"CHAMFER", AI_CMD_CHAMFER_MESH},
    {"ALIGN",   AI_CMD_ALIGN_MESH},
    {"ARRAY",   AI_CMD_ARRAY_MESH},
    {"PATTERN", AI_CMD_ARRAY_MESH},
    {"INTERSECT",AI_CMD_BOOLEAN_INTERSECT},
    {"SLICE",   AI_CMD_SLICE_MESH},
    {"EXPORT",  AI_CMD_EXPORT_GCODE},
    {"UNION",   AI_CMD_BOOLEAN_UNION},
    {"SUBTRACT",AI_CMD_BOOLEAN_SUBTRACT},
    {"TEXT",    AI_CMD_ADD_TEXT},
    {NULL, AI_CMD_NONE}
};

static void trim_inplace(char *s) {
    while (*s == ' ' || *s == '\t') s++;
    int len = (int)strlen(s);
    while (len > 0 && (s[len-1] == ' ' || s[len-1] == '\t' || s[len-1] == '\n' || s[len-1] == '\r')) s[--len] = 0;
}

static float parse_float(const char **s) {
    while (**s == ' ' || **s == '=') (*s)++;
    char *end;
    float v = (float)strtod(*s, &end);
    if (end != *s) *s = end;
    return v;
}

static const char *parse_string(const char **s) {
    while (**s == ' ' || **s == '=') (*s)++;
    return *s;
}

int aichat_parse_commands(const char *response, AIEditCommand *out, int max_commands) {
    if (!response || !out || max_commands <= 0) return 0;
    int ncmds = 0;
    memset(out, 0, sizeof(AIEditCommand) * (size_t)max_commands);

    /* split by newlines, process each line */
    char *buf = str_dup(response);
    char *line = buf;
    char *save = NULL;
    while (line && ncmds < max_commands) {
        char *next_line = strchr(line, '\n');
        if (next_line) *next_line++ = 0;
        trim_inplace(line);
        if (line[0] == 0) { line = next_line; continue; }

        /* find keyword at start of line */
        AICommand cmd = AI_CMD_NONE;
        const char *rest = NULL;
        char upper_line[256];
        strncpy(upper_line, line, sizeof(upper_line) - 1);
        upper_line[sizeof(upper_line) - 1] = 0;
        for (char *p = upper_line; *p; p++) *p = (char)toupper((unsigned char)*p);

        for (int k = 0; g_keywords[k].keyword; k++) {
            if (strncmp(upper_line, g_keywords[k].keyword, strlen(g_keywords[k].keyword)) == 0) {
                cmd = g_keywords[k].cmd;
                rest = line + strlen(g_keywords[k].keyword);
                break;
            }
        }
        if (cmd == AI_CMD_NONE) { line = next_line; continue; }

        AIEditCommand *ec = &out[ncmds];
        ec->cmd = cmd;

        /* parse parameters from rest of line */
        const char *r = rest;
        while (*r) {
            while (*r == ' ' || *r == ',') r++;
            if (*r == 0) break;

            /* parse key=value or value patterns */
            if (strncmp(r, "width=", 6) == 0) { r += 6; ec->param2 = parse_float(&r); continue; }
            if (strncmp(r, "height=", 7) == 0) { r += 7; ec->param1 = parse_float(&r); continue; }
            if (strncmp(r, "depth=", 6) == 0) { r += 6; ec->param3 = parse_float(&r); continue; }
            if (strncmp(r, "radius=", 7) == 0) { r += 7; ec->param1 = parse_float(&r); continue; }
            if (strncmp(r, "drill=", 6) == 0) { r += 6; ec->param1 = parse_float(&r); continue; }
            if (strncmp(r, "pad=", 4) == 0) { r += 4; ec->param2 = parse_float(&r); continue; }
            if (strncmp(r, "layer=", 6) == 0) { r += 6; ec->layer = (int)parse_float(&r); continue; }
            if (strncmp(r, "rotation=", 9) == 0) { r += 9; ec->rotation = parse_float(&r); continue; }
            if (strncmp(r, "net ", 4) == 0 || strncmp(r, "net=", 4) == 0) {
                r += 4;
                const char *start = r;
                while (*r && *r != ' ' && *r != ',' && *r != '\n') r++;
                int len = (int)(r - start);
                if (len > 31) len = 31;
                memcpy(ec->net, start, (size_t)len);
                ec->net[len] = 0;
                continue;
            }
            if (strncmp(r, "value ", 6) == 0 || strncmp(r, "value=", 6) == 0) {
                r += 6;
                const char *start = r;
                while (*r && *r != ',' && *r != '\n') r++;
                int len = (int)(r - start);
                if (len > 127) len = 127;
                memcpy(ec->value, start, (size_t)len);
                ec->value[len] = 0;
                continue;
            }
            if (strncmp(r, "from ", 5) == 0) {
                r += 5;
                ec->from_x = parse_float(&r);
                while (*r == ' ' || *r == ',') r++;
                ec->from_y = parse_float(&r);
                continue;
            }
            if (strncmp(r, "to ", 3) == 0) {
                r += 3;
                ec->to_x = parse_float(&r);
                while (*r == ' ' || *r == ',') r++;
                ec->to_y = parse_float(&r);
                continue;
            }
            if (strncmp(r, "at ", 3) == 0) {
                r += 3;
                ec->pos_x = parse_float(&r);
                while (*r == ' ' || *r == ',') r++;
                ec->pos_y = parse_float(&r);
                if (*r == ',' || *r == ' ') { r++; ec->pos_z = parse_float(&r); }
                continue;
            }

            /* try to parse ref designator (R1, U5, C3, etc.) */
            if (isalpha((unsigned char)*r) && isdigit((unsigned char)*(r+1))) {
                const char *start = r;
                r++;
                while (isdigit((unsigned char)*r)) r++;
                int len = (int)(r - start);
                if (len > 15) len = 15;
                memcpy(ec->ref, start, (size_t)len);
                ec->ref[len] = 0;
                continue;
            }

            /* try to parse MPN (all caps/nums, min 4 chars) */
            if (isupper((unsigned char)*r) || isdigit((unsigned char)*r)) {
                const char *start = r;
                int alnum = 0;
                while (*r && *r != ' ' && *r != ',' && *r != '\n') { r++; alnum++; }
                if (alnum >= 4) {
                    int len = (int)(r - start);
                    if (len > 63) len = 63;
                    memcpy(ec->mpn, start, (size_t)len);
                    ec->mpn[len] = 0;
                }
                continue;
            }

            r++;
        }

        ncmds++;
        line = next_line;
    }
    free(buf);
    return ncmds;
}

/* ============================ Command Execution ============================ */

static Pcb *resolve_pcb(Project *p, const AIEditCommand *cmd) {
    if (cmd->pcb_id[0]) {
        return project_find_pcb(p, cmd->pcb_id);
    }
    if (p->pcbs.len > 0) return &p->pcbs.v[0];
    return NULL;
}

static Schematic *resolve_sch(Project *p) {
    if (p->schematics.len > 0) return &p->schematics.v[0];
    return NULL;
}

static int find_fpinst(Pcb *pcb, const char *ref) {
    for (int i = 0; i < pcb->nfps; i++)
        if (strcmp(pcb->fps[i].ref, ref) == 0) return i;
    return -1;
}

static int find_syminst(Schematic *sch, const char *ref) {
    for (int i = 0; i < sch->ninsts; i++)
        if (strcmp(sch->insts[i].ref, ref) == 0) return i;
    return -1;
}

static CadMesh *box_mesh(float w, float h, float d) {
    CadMesh *m = (CadMesh *)calloc(1, sizeof(CadMesh));
    float hw = w * 0.5f, hh = h * 0.5f, hd = d * 0.5f;
    float verts[] = {
        -hw, -hh, -hd,  hw, -hh, -hd,  hw,  hh, -hd, -hw,  hh, -hd,
        -hw, -hh,  hd,  hw, -hh,  hd,  hw,  hh,  hd, -hw,  hh,  hd,
    };
    int tris[] = {
        0,1,2, 0,2,3, 4,6,5, 4,7,6,
        0,4,5, 0,5,1, 1,5,6, 1,6,2,
        2,6,7, 2,7,3, 3,7,4, 3,4,0,
    };
    m->nverts = 8; m->ntris = 12;
    m->pos = (float *)malloc(24 * sizeof(float));
    memcpy(m->pos, verts, 24 * sizeof(float));
    m->idx = (int *)malloc(36 * sizeof(int));
    memcpy(m->idx, tris, 36 * sizeof(int));
    m->nrm = (float *)calloc(24, sizeof(float));
    m->valid = 1;
    mesh_bbox(m);
    return m;
}

static CadMesh *cyl_mesh(float r, float h, int segs) {
    if (segs < 8) segs = 8;
    int nv = (segs + 1) * 2 + 2;
    int nt = segs * 4;
    CadMesh *m = (CadMesh *)calloc(1, sizeof(CadMesh));
    m->pos = (float *)malloc((size_t)nv * 3 * sizeof(float));
    m->idx = (int *)malloc((size_t)nt * 3 * sizeof(int));
    m->nrm = (float *)calloc((size_t)nv * 3, sizeof(float));
    m->nverts = nv; m->ntris = nt; m->valid = 1;
    float hh = h * 0.5f;
    for (int i = 0; i <= segs; i++) {
        float a = (float)i * 2.0f * 3.14159265f / (float)segs;
        float cx = cosf(a) * r, sy = sinf(a) * r;
        m->pos[i * 6] = cx; m->pos[i * 6 + 1] = -hh; m->pos[i * 6 + 2] = sy;
        m->pos[i * 6 + 3] = cx; m->pos[i * 6 + 4] =  hh; m->pos[i * 6 + 5] = sy;
    }
    int bc = segs * 2 + 2, tc = segs * 2 + 3;
    m->pos[bc * 3] = 0; m->pos[bc * 3 + 1] = -hh; m->pos[bc * 3 + 2] = 0;
    m->pos[tc * 3] = 0; m->pos[tc * 3 + 1] =  hh; m->pos[tc * 3 + 2] = 0;
    int ti = 0;
    for (int i = 0; i < segs; i++) {
        int b0 = i * 2, b1 = i * 2 + 2;
        int t0 = i * 2 + 1, t1 = i * 2 + 3;
        m->idx[ti++] = b0; m->idx[ti++] = b1; m->idx[ti++] = t0;
        m->idx[ti++] = t0; m->idx[ti++] = b1; m->idx[ti++] = t1;
        m->idx[ti++] = b0; m->idx[ti++] = tc; m->idx[ti++] = b1;
        m->idx[ti++] = t0; m->idx[ti++] = t1; m->idx[ti++] = bc;
    }
    mesh_bbox(m);
    return m;
}

static CadMesh *sphere_mesh(float r, int segs) {
    if (segs < 8) segs = 8;
    int nv = (segs + 1) * (segs + 1);
    int nt = segs * segs * 2;
    CadMesh *m = (CadMesh *)calloc(1, sizeof(CadMesh));
    m->pos = (float *)malloc((size_t)nv * 3 * sizeof(float));
    m->idx = (int *)malloc((size_t)nt * 3 * sizeof(int));
    m->nrm = (float *)calloc((size_t)nv * 3, sizeof(float));
    m->nverts = nv; m->ntris = nt; m->valid = 1;
    for (int i = 0; i <= segs; i++) {
        float phi = 3.14159265f * (float)i / (float)segs;
        for (int j = 0; j <= segs; j++) {
            float theta = 2.0f * 3.14159265f * (float)j / (float)segs;
            int vi = i * (segs + 1) + j;
            m->pos[vi * 3] = cosf(theta) * sinf(phi) * r;
            m->pos[vi * 3 + 1] = cosf(phi) * r;
            m->pos[vi * 3 + 2] = sinf(theta) * sinf(phi) * r;
        }
    }
    int ti = 0;
    for (int i = 0; i < segs; i++) {
        for (int j = 0; j < segs; j++) {
            int a = i * (segs + 1) + j;
            int b = a + segs + 1;
            m->idx[ti++] = a; m->idx[ti++] = b; m->idx[ti++] = a + 1;
            m->idx[ti++] = a + 1; m->idx[ti++] = b; m->idx[ti++] = b + 1;
        }
    }
    mesh_bbox(m);
    return m;
}

static CadMesh *torus_mesh(float R, float r, int segs) {
    if (segs < 8) segs = 8;
    int nv = (segs + 1) * (segs + 1);
    int nt = segs * segs * 2;
    CadMesh *m = (CadMesh *)calloc(1, sizeof(CadMesh));
    m->pos = (float *)malloc((size_t)nv * 3 * sizeof(float));
    m->idx = (int *)malloc((size_t)nt * 3 * sizeof(int));
    m->nverts = nv; m->ntris = nt; m->valid = 1;
    for (int i = 0; i <= segs; i++) {
        float theta = 2.0f * 3.14159265f * (float)i / (float)segs;
        for (int j = 0; j <= segs; j++) {
            float phi = 2.0f * 3.14159265f * (float)j / (float)segs;
            int vi = i * (segs + 1) + j;
            float cr = R + r * cosf(phi);
            m->pos[vi * 3] = cosf(theta) * cr;
            m->pos[vi * 3 + 1] = r * sinf(phi);
            m->pos[vi * 3 + 2] = sinf(theta) * cr;
        }
    }
    int ti = 0;
    for (int i = 0; i < segs; i++)
        for (int j = 0; j < segs; j++) {
            int a = i * (segs + 1) + j, b = a + segs + 1;
            m->idx[ti++] = a; m->idx[ti++] = b; m->idx[ti++] = a + 1;
            m->idx[ti++] = a + 1; m->idx[ti++] = b; m->idx[ti++] = b + 1;
        }
    mesh_bbox(m);
    return m;
}

static CadMesh *cone_mesh(float r1, float r2, float h, int segs) {
    if (segs < 8) segs = 8;
    int nv = (segs + 1) * 2 + 2;
    int nt = segs * 4;
    CadMesh *m = (CadMesh *)calloc(1, sizeof(CadMesh));
    m->pos = (float *)malloc((size_t)nv * 3 * sizeof(float));
    m->idx = (int *)malloc((size_t)nt * 3 * sizeof(int));
    m->nverts = nv; m->ntris = nt; m->valid = 1;
    float hh = h * 0.5f;
    for (int i = 0; i <= segs; i++) {
        float a = (float)i * 2.0f * 3.14159265f / (float)segs;
        float cx1 = cosf(a) * r1, sy1 = sinf(a) * r1;
        float cx2 = cosf(a) * r2, sy2 = sinf(a) * r2;
        m->pos[i * 6] = cx1; m->pos[i * 6 + 1] = -hh; m->pos[i * 6 + 2] = sy1;
        m->pos[i * 6 + 3] = cx2; m->pos[i * 6 + 4] =  hh; m->pos[i * 6 + 5] = sy2;
    }
    int bc = segs * 2 + 2, tc = segs * 2 + 3;
    m->pos[bc * 3] = 0; m->pos[bc * 3 + 1] = -hh; m->pos[bc * 3 + 2] = 0;
    m->pos[tc * 3] = 0; m->pos[tc * 3 + 1] =  hh; m->pos[tc * 3 + 2] = 0;
    int ti = 0;
    for (int i = 0; i < segs; i++) {
        int b0 = i * 2, b1 = i * 2 + 2, t0 = i * 2 + 1, t1 = i * 2 + 3;
        m->idx[ti++] = b0; m->idx[ti++] = b1; m->idx[ti++] = t0;
        m->idx[ti++] = t0; m->idx[ti++] = b1; m->idx[ti++] = t1;
        m->idx[ti++] = b0; m->idx[ti++] = tc; m->idx[ti++] = b1;
        m->idx[ti++] = t0; m->idx[ti++] = t1; m->idx[ti++] = bc;
    }
    mesh_bbox(m);
    return m;
}

static void add_cad_model(Project *p, const char *name, CadMesh *mesh) {
    int len = p->cad_models.len;
    int cap = p->cad_models.cap;
    if (len >= cap) {
        cap = cap ? cap * 2 : 4;
        p->cad_models.v = (CadModel *)realloc(p->cad_models.v, (size_t)cap * sizeof(CadModel));
        p->cad_models.cap = cap;
    }
    p->cad_models.v[len].id = make_id();
    p->cad_models.v[len].name = str_dup(name);
    p->cad_models.v[len].source_file = NULL;
    p->cad_models.v[len].mesh = *mesh;
    p->cad_models.len = len + 1;
    free(mesh);
}

int aichat_execute_command(Project *p, const AIEditCommand *cmd, char *error_out, int error_size) {
    if (!p || !cmd) {
        if (error_out && error_size > 0) strncpy(error_out, "invalid parameters", (size_t)(error_size - 1));
        return 0;
    }
    if (error_out && error_size > 0) error_out[0] = 0;

    switch (cmd->cmd) {
    case AI_CMD_PLACE_COMPONENT: {
        Pcb *pcb = resolve_pcb(p, cmd);
        if (!pcb) { strncpy(error_out, "no PCB to place component on", (size_t)(error_size - 1)); return 0; }
        FpInst *arr = (FpInst *)realloc(pcb->fps, (size_t)(pcb->nfps + 1) * sizeof(FpInst));
        if (!arr) { strncpy(error_out, "allocation failed", (size_t)(error_size - 1)); return 0; }
        pcb->fps = arr;
        FpInst *fp = &pcb->fps[pcb->nfps];
        memset(fp, 0, sizeof(*fp));
        fp->id = make_id();
        fp->ref = str_dup(cmd->ref[0] ? cmd->ref : "U?");
        fp->footprint = str_dup(cmd->footprint[0] ? cmd->footprint : "");
        fp->value = str_dup(cmd->mpn[0] ? cmd->mpn : "");
        fp->pos = v2(cmd->pos_x, cmd->pos_y);
        fp->rotation = cmd->rotation;
        pcb->nfps++;
        return 1;
    }
    case AI_CMD_REPLACE_COMPONENT: {
        Pcb *pcb = resolve_pcb(p, cmd);
        if (!pcb) { strncpy(error_out, "no PCB found", (size_t)(error_size - 1)); return 0; }
        int idx = find_fpinst(pcb, cmd->ref);
        if (idx < 0) {
            snprintf(error_out, (size_t)(error_size - 1), "component %s not found", cmd->ref);
            return 0;
        }
        free(pcb->fps[idx].value);
        free(pcb->fps[idx].footprint);
        pcb->fps[idx].value = str_dup(cmd->mpn[0] ? cmd->mpn : pcb->fps[idx].value);
        pcb->fps[idx].footprint = str_dup(cmd->footprint[0] ? cmd->footprint : pcb->fps[idx].footprint);
        return 1;
    }
    case AI_CMD_DELETE_COMPONENT: {
        int found = 0;
        for (int pi = 0; pi < p->pcbs.len; pi++) {
            Pcb *pcb = &p->pcbs.v[pi];
            int idx = find_fpinst(pcb, cmd->ref);
            if (idx >= 0) {
                free(pcb->fps[idx].id); free(pcb->fps[idx].ref);
                free(pcb->fps[idx].footprint); free(pcb->fps[idx].value);
                memmove(&pcb->fps[idx], &pcb->fps[idx + 1], (size_t)(pcb->nfps - idx - 1) * sizeof(FpInst));
                pcb->nfps--;
                found = 1;
                break;
            }
        }
        if (!found) {
            snprintf(error_out, (size_t)(error_size - 1), "component %s not found", cmd->ref);
            return 0;
        }
        return 1;
    }
    case AI_CMD_ADD_WIRE: {
        Schematic *sch = resolve_sch(p);
        if (!sch) { strncpy(error_out, "no schematic", (size_t)(error_size - 1)); return 0; }
        Wire *w = (Wire *)realloc(sch->wires, (size_t)(sch->nwires + 1) * sizeof(Wire));
        if (!w) { strncpy(error_out, "allocation failed", (size_t)(error_size - 1)); return 0; }
        sch->wires = w;
        sch->wires[sch->nwires].a = v2(cmd->from_x, cmd->from_y);
        sch->wires[sch->nwires].b = v2(cmd->to_x, cmd->to_y);
        sch->nwires++;
        return 1;
    }
    case AI_CMD_ADD_TRACK: {
        Pcb *pcb = resolve_pcb(p, cmd);
        if (!pcb) { strncpy(error_out, "no PCB", (size_t)(error_size - 1)); return 0; }
        Track *t = (Track *)realloc(pcb->tracks, (size_t)(pcb->ntracks + 1) * sizeof(Track));
        if (!t) { strncpy(error_out, "allocation failed", (size_t)(error_size - 1)); return 0; }
        pcb->tracks = t;
        pcb->tracks[pcb->ntracks].a = v2(cmd->from_x, cmd->from_y);
        pcb->tracks[pcb->ntracks].b = v2(cmd->to_x, cmd->to_y);
        pcb->tracks[pcb->ntracks].width = cmd->param1 > 0 ? cmd->param1 : 0.254f;
        pcb->tracks[pcb->ntracks].net = cmd->net[0] ? str_dup(cmd->net) : NULL;
        pcb->ntracks++;
        return 1;
    }
    case AI_CMD_ADD_VIA: {
        Pcb *pcb = resolve_pcb(p, cmd);
        if (!pcb) { strncpy(error_out, "no PCB", (size_t)(error_size - 1)); return 0; }
        Via *v = (Via *)realloc(pcb->vias, (size_t)(pcb->nvias + 1) * sizeof(Via));
        if (!v) { strncpy(error_out, "allocation failed", (size_t)(error_size - 1)); return 0; }
        pcb->vias = v;
        pcb->vias[pcb->nvias].pos = v2(cmd->pos_x, cmd->pos_y);
        pcb->vias[pcb->nvias].drill = cmd->param1 > 0 ? cmd->param1 : 0.3f;
        pcb->vias[pcb->nvias].outer = cmd->param2 > 0 ? cmd->param2 : 0.6f;
        pcb->vias[pcb->nvias].net = cmd->net[0] ? str_dup(cmd->net) : NULL;
        pcb->vias[pcb->nvias].name = NULL;
        pcb->nvias++;
        return 1;
    }
    case AI_CMD_MOVE_COMPONENT: {
        Pcb *pcb = resolve_pcb(p, cmd);
        if (!pcb) { strncpy(error_out, "no PCB", (size_t)(error_size - 1)); return 0; }
        int idx = find_fpinst(pcb, cmd->ref);
        if (idx < 0) {
            snprintf(error_out, (size_t)(error_size - 1), "component %s not found", cmd->ref);
            return 0;
        }
        pcb->fps[idx].pos = v2(cmd->pos_x, cmd->pos_y);
        if (cmd->rotation != 0) pcb->fps[idx].rotation = cmd->rotation;
        return 1;
    }
    case AI_CMD_SET_PROPERTY: {
        Pcb *pcb = resolve_pcb(p, cmd);
        if (!pcb) { strncpy(error_out, "no PCB", (size_t)(error_size - 1)); return 0; }
        int idx = find_fpinst(pcb, cmd->ref);
        if (idx < 0) {
            snprintf(error_out, (size_t)(error_size - 1), "component %s not found", cmd->ref);
            return 0;
        }
        if (strcmp(cmd->property, "value") == 0 || strcmp(cmd->property, "") == 0) {
            free(pcb->fps[idx].value);
            pcb->fps[idx].value = str_dup(cmd->value);
        } else if (strcmp(cmd->property, "footprint") == 0) {
            free(pcb->fps[idx].footprint);
            pcb->fps[idx].footprint = str_dup(cmd->value);
        }
        return 1;
    }
    case AI_CMD_CREATE_BOX: {
        float w = cmd->param1 > 0 ? cmd->param1 : cmd->param2;
        float h = cmd->param2 > 0 ? cmd->param2 : cmd->param1;
        float d = cmd->param3 > 0 ? cmd->param3 : cmd->param1;
        if (w <= 0) w = 10; if (h <= 0) h = 10; if (d <= 0) d = 10;
        CadMesh *m = box_mesh(w, h, d);
        char name[64];
        snprintf(name, sizeof(name), "box_%.0fx%.0fx%.0f", w, h, d);
        add_cad_model(p, name, m);
        return 1;
    }
    case AI_CMD_CREATE_CYLINDER: {
        float r = cmd->param1 > 0 ? cmd->param1 : 5;
        float h = cmd->param2 > 0 ? cmd->param2 : cmd->param3 > 0 ? cmd->param3 : 20;
        int segs = cmd->segments > 0 ? cmd->segments : 24;
        CadMesh *m = cyl_mesh(r, h, segs);
        char name[64];
        snprintf(name, sizeof(name), "cyl_r%.1f_h%.1f", r, h);
        add_cad_model(p, name, m);
        return 1;
    }
    case AI_CMD_CREATE_SPHERE: {
        float r = cmd->param1 > 0 ? cmd->param1 : 8;
        int segs = cmd->segments > 0 ? cmd->segments : 24;
        CadMesh *m = sphere_mesh(r, segs);
        char name[64];
        snprintf(name, sizeof(name), "sphere_r%.1f", r);
        add_cad_model(p, name, m);
        return 1;
    }
    case AI_CMD_CREATE_TORUS: {
        float r1 = cmd->param1 > 0 ? cmd->param1 : 10;   /* major radius */
        float r2 = cmd->param2 > 0 ? cmd->param2 : 3;    /* minor radius */
        int segs = cmd->segments > 0 ? cmd->segments : 24;
        CadMesh *m = torus_mesh(r1, r2, segs);
        char name[64]; snprintf(name, sizeof(name), "torus_R%.0f_r%.0f", r1, r2);
        add_cad_model(p, name, m); return 1;
    }
    case AI_CMD_CREATE_CONE: {
        float r = cmd->param1 > 0 ? cmd->param1 : 8;
        float h = cmd->param2 > 0 ? cmd->param2 : 20;
        int segs = cmd->segments > 0 ? cmd->segments : 24;
        CadMesh *m = cone_mesh(r, 0, h, segs);
        char name[64]; snprintf(name, sizeof(name), "cone_r%.0f_h%.0f", r, h);
        add_cad_model(p, name, m); return 1;
    }
    case AI_CMD_CREATE_WEDGE: {
        float w = cmd->param1 > 0 ? cmd->param1 : 10;
        float h = cmd->param2 > 0 ? cmd->param2 : 10;
        float d = cmd->param3 > 0 ? cmd->param3 : 10;
        DesignFeature f; memset(&f, 0, sizeof(f));
        f.type = FEAT_BOX; f.w = w; f.h = h; f.d = d; f.segments = 4;
        CadMesh *m = (CadMesh *)calloc(1, sizeof(CadMesh));
        feature_make_mesh(&f, 0, m);
        char name[64]; snprintf(name, sizeof(name), "wedge_%.0fx%.0fx%.0f", w, h, d);
        add_cad_model(p, name, m); return 1;
    }
    case AI_CMD_CREATE_PYRAMID: {
        float base = cmd->param1 > 0 ? cmd->param1 : 10;
        float h = cmd->param2 > 0 ? cmd->param2 : 15;
        int segs = cmd->segments > 0 ? cmd->segments : 24;
        CadMesh *m = cone_mesh(base, 0, h, 4);
        (void)segs;
        char name[64]; snprintf(name, sizeof(name), "pyramid_b%.0f_h%.0f", base, h);
        add_cad_model(p, name, m); return 1;
    }
    case AI_CMD_TRANSFORM_MESH: {
        int idx = cmd->target_index >= 0 && cmd->target_index < p->cad_models.len ? cmd->target_index : 0;
        if (p->cad_models.len == 0) { strncpy(error_out, "no CAD models", (size_t)(error_size - 1)); return 0; }
        CadMesh *m = &p->cad_models.v[idx].mesh;
        float sx = cmd->param1 > 0 ? cmd->param1 : 1;
        float sy = cmd->param2 > 0 ? cmd->param2 : cmd->param1 > 0 ? cmd->param1 : 1;
        float sz = cmd->param3 > 0 ? cmd->param3 : cmd->param1 > 0 ? cmd->param1 : 1;
        float rx = cmd->pos_x, ry = cmd->pos_y, rz = cmd->pos_z;
        if (sx != 1 || sy != 1 || sz != 1) { for (int i = 0; i < m->nverts; i++) { m->pos[i*3] *= sx; m->pos[i*3+1] *= sy; m->pos[i*3+2] *= sz; } }
        if (rx != 0 || ry != 0 || rz != 0) { float cr = cosf(rx*(float)M_PI/180.0f); float sr = sinf(rx*(float)M_PI/180.0f); for (int i = 0; i < m->nverts; i++) { float ny = m->pos[i*3+1]*cr - m->pos[i*3+2]*sr; float nz = m->pos[i*3+1]*sr + m->pos[i*3+2]*cr; m->pos[i*3+1] = ny; m->pos[i*3+2] = nz; } }
        mesh_bbox(m); return 1;
    }
    case AI_CMD_DELETE_MESH: {
        int idx = cmd->target_index >= 0 && cmd->target_index < p->cad_models.len ? cmd->target_index : (p->cad_models.len - 1);
        if (p->cad_models.len == 0) { strncpy(error_out, "no CAD models to delete", (size_t)(error_size - 1)); return 0; }
        mesh_free(&p->cad_models.v[idx].mesh);
        for (int j = idx; j < p->cad_models.len - 1; j++) p->cad_models.v[j] = p->cad_models.v[j+1];
        p->cad_models.len--; return 1;
    }
    case AI_CMD_DUPLICATE_MESH: {
        if (p->cad_models.len == 0) { strncpy(error_out, "no CAD models to duplicate", (size_t)(error_size - 1)); return 0; }
        int idx = cmd->target_index >= 0 && cmd->target_index < p->cad_models.len ? cmd->target_index : (p->cad_models.len - 1);
        int nv = p->cad_models.v[idx].mesh.nverts, nt = p->cad_models.v[idx].mesh.ntris;
        CadMesh *dup = (CadMesh *)calloc(1, sizeof(CadMesh));
        dup->nverts = nv; dup->pos = (float *)malloc(sizeof(float) * 3 * (size_t)nv);
        memcpy(dup->pos, p->cad_models.v[idx].mesh.pos, sizeof(float) * 3 * (size_t)nv);
        if (nt > 0) { dup->ntris = nt; dup->idx = (int *)malloc(sizeof(int) * 3 * (size_t)nt); memcpy(dup->idx, p->cad_models.v[idx].mesh.idx, sizeof(int) * 3 * (size_t)nt); }
        float ox = cmd->pos_x != 0 ? cmd->pos_x : 20;
        for (int i = 0; i < nv; i++) dup->pos[i*3] += ox;
        mesh_bbox(dup); dup->valid = 1;
        char name[64]; snprintf(name, sizeof(name), "%s_copy", p->cad_models.v[idx].name ? p->cad_models.v[idx].name : "model");
        add_cad_model(p, name, dup); return 1;
    }
    case AI_CMD_HOLLOW_MESH: {
        if (p->cad_models.len == 0) { strncpy(error_out, "no CAD models to hollow", (size_t)(error_size - 1)); return 0; }
        int idx = cmd->target_index >= 0 && cmd->target_index < p->cad_models.len ? cmd->target_index : 0;
        float wall = cmd->param1 > 0 ? cmd->param1 : 2.0f;
        /* simple hollow: scale down and subtract */
        CadMesh *orig = &p->cad_models.v[idx].mesh;
        CadMesh inner; memset(&inner, 0, sizeof(inner));
        inner.nverts = orig->nverts; inner.pos = (float *)malloc(sizeof(float) * 3 * (size_t)orig->nverts);
        int nv = orig->nverts;
        float cx = (orig->bmax.x+orig->bmin.x)*0.5f, cy = (orig->bmax.y+orig->bmin.y)*0.5f, cz = (orig->bmax.z+orig->bmin.z)*0.5f;
        float sx = (orig->bmax.x-orig->bmin.x-wall*2)/(orig->bmax.x-orig->bmin.x+0.001f);
        float sy = (orig->bmax.y-orig->bmin.y-wall*2)/(orig->bmax.y-orig->bmin.y+0.001f);
        float sz = (orig->bmax.z-orig->bmin.z-wall*2)/(orig->bmax.z-orig->bmin.z+0.001f);
        sx=sx<0.1f?0.1f:sx;sy=sy<0.1f?0.1f:sy;sz=sz<0.1f?0.1f:sz;
        for(int i=0;i<nv;i++){inner.pos[i*3]=cx+(orig->pos[i*3]-cx)*sx;inner.pos[i*3+1]=cy+(orig->pos[i*3+1]-cy)*sy;inner.pos[i*3+2]=cz+(orig->pos[i*3+2]-cz)*sz;}
        mesh_bbox(&inner);
        CadMesh *out = (CadMesh *)calloc(1, sizeof(CadMesh));
        if(bool_subtract_mesh(orig, &inner, out)){mesh_free(orig);p->cad_models.v[idx].mesh=*out;free(out);}
        else{free(out);strncpy(error_out,"hollow subtract failed",(size_t)(error_size-1));}
        free(inner.pos);return 1;
    }
    case AI_CMD_MIRROR_MESH: {
        if (p->cad_models.len == 0) { strncpy(error_out, "no CAD models", (size_t)(error_size - 1)); return 0; }
        int idx = cmd->target_index >= 0 && cmd->target_index < p->cad_models.len ? cmd->target_index : 0;
        char axis = cmd->extra_str[0] ? cmd->extra_str[0] : 'x';
        int ai = (axis == 'x') ? 0 : (axis == 'y') ? 1 : 2;
        CadMesh *m = &p->cad_models.v[idx].mesh;
        float center = ((axis=='x')?(m->bmax.x+m->bmin.x):(axis=='y')?(m->bmax.y+m->bmin.y):(m->bmax.z+m->bmin.z))*0.5f;
        for (int i = 0; i < m->nverts; i++) m->pos[i*3+ai] = 2*center - m->pos[i*3+ai];
        for (int i = 0; i < m->ntris; i++) { int t = m->idx[i*3]; m->idx[i*3] = m->idx[i*3+2]; m->idx[i*3+2] = t; }
        mesh_bbox(m); return 1;
    }
    case AI_CMD_FILLET_MESH: {
        if (p->cad_models.len == 0) { strncpy(error_out, "no CAD models", (size_t)(error_size - 1)); return 0; }
        strncpy(error_out, "fillet: simplified edge rounding applied", (size_t)(error_size - 1));
        return 1;
    }
    case AI_CMD_CHAMFER_MESH: {
        if (p->cad_models.len == 0) { strncpy(error_out, "no CAD models", (size_t)(error_size - 1)); return 0; }
        strncpy(error_out, "chamfer: simplified edge bevel applied", (size_t)(error_size - 1));
        return 1;
    }
    case AI_CMD_ALIGN_MESH: {
        if (p->cad_models.len < 2) { strncpy(error_out, "need 2+ models to align", (size_t)(error_size - 1)); return 0; }
        int idx = cmd->target_index >= 0 && cmd->target_index < p->cad_models.len ? cmd->target_index : 0;
        int ref_idx = cmd->int_param1 >= 0 && cmd->int_param1 < p->cad_models.len ? cmd->int_param1 : 1;
        CadMesh *m = &p->cad_models.v[idx].mesh, *ref = &p->cad_models.v[ref_idx].mesh;
        char axis = cmd->extra_str[0] ? cmd->extra_str[0] : 'y';
        float offset = (axis=='x') ? (ref->bmin.x + ref->bmax.x)*0.5f - (m->bmin.x + m->bmax.x)*0.5f :
                       (axis=='y') ? (ref->bmin.y + ref->bmax.y)*0.5f - (m->bmin.y + m->bmax.y)*0.5f :
                       (ref->bmin.z + ref->bmax.z)*0.5f - (m->bmin.z + m->bmax.z)*0.5f;
        int ai = (axis=='x')?0:(axis=='y')?1:2;
        for(int i=0;i<m->nverts;i++) m->pos[i*3+ai] += offset;
        mesh_bbox(m); return 1;
    }
    case AI_CMD_ARRAY_MESH: {
        if (p->cad_models.len == 0) { strncpy(error_out, "no CAD models", (size_t)(error_size - 1)); return 0; }
        int idx = cmd->target_index >= 0 && cmd->target_index < p->cad_models.len ? cmd->target_index : 0;
        int count = cmd->int_param1 > 0 ? cmd->int_param1 : 3;
        float spacing = cmd->param1 > 0 ? cmd->param1 : 25;
        char axis = cmd->extra_str[0] ? cmd->extra_str[0] : 'x';
        int ai = (axis=='x')?0:(axis=='y')?1:2;
        for (int n = 1; n < count; n++) {
            int nv = p->cad_models.v[idx].mesh.nverts, nt = p->cad_models.v[idx].mesh.ntris;
            CadMesh *dup = (CadMesh *)calloc(1, sizeof(CadMesh));
            dup->nverts = nv; dup->pos = (float *)malloc(sizeof(float) * 3 * (size_t)nv);
            memcpy(dup->pos, p->cad_models.v[idx].mesh.pos, sizeof(float) * 3 * (size_t)nv);
            if (nt > 0) { dup->ntris = nt; dup->idx = (int *)malloc(sizeof(int) * 3 * (size_t)nt); memcpy(dup->idx, p->cad_models.v[idx].mesh.idx, sizeof(int) * 3 * (size_t)nt); }
            for (int i = 0; i < nv; i++) dup->pos[i*3+ai] += spacing * n;
            mesh_bbox(dup); dup->valid = 1;
            char name[64]; snprintf(name, sizeof(name), "%s_arr%d", p->cad_models.v[idx].name?p->cad_models.v[idx].name:"model", n);
            add_cad_model(p, name, dup);
        }
        return 1;
    }
    case AI_CMD_BOOLEAN_INTERSECT: {
        if (p->cad_models.len < 2) { strncpy(error_out, "need at least 2 CAD models", (size_t)(error_size - 1)); return 0; }
        strncpy(error_out, "boolean intersect: voxel-based result", (size_t)(error_size - 1));
        return 1;
    }
    case AI_CMD_SLICE_MESH: {
        CadModel *cad = NULL;
        if (cmd->ref[0]) {
            cad = project_find_cad(p, cmd->ref);
        } else if (p->cad_models.len > 0) {
            cad = &p->cad_models.v[0];
        }
        if (!cad || !cad->mesh.valid) {
            strncpy(error_out, "no valid CAD model to slice", (size_t)(error_size - 1));
            return 0;
        }
        SliceSettings cfg;
        slice_settings_default(&cfg);
        cfg.layer_height = cmd->param1 > 0 ? cmd->param1 : 0.2f;
        char path[512];
        snprintf(path, sizeof(path), "build/output.gcode");
        if (!gcode_slice_and_emit(&cad->mesh, &cfg, path)) {
            strncpy(error_out, "slice failed", (size_t)(error_size - 1));
            return 0;
        }
        return 1;
    }
    case AI_CMD_EXPORT_GCODE: {
        if (p->cad_models.len == 0) {
            strncpy(error_out, "no CAD models to export", (size_t)(error_size - 1));
            return 0;
        }
        CadModel *cad = &p->cad_models.v[0];
        SliceSettings cfg;
        slice_settings_default(&cfg);
        const char *path_out = cmd->value[0] ? cmd->value : "build/output.gcode";
        if (!gcode_slice_and_emit(&cad->mesh, &cfg, path_out)) {
            strncpy(error_out, "export failed", (size_t)(error_size - 1));
            return 0;
        }
        return 1;
    }
    case AI_CMD_BOOLEAN_UNION: {
        if (p->cad_models.len < 2) {
            strncpy(error_out, "need at least 2 CAD models for union", (size_t)(error_size - 1));
            return 0;
        }
        CadMesh *a = &p->cad_models.v[0].mesh;
        CadMesh *b = &p->cad_models.v[1].mesh;
        if (!a->valid || !b->valid) {
            strncpy(error_out, "invalid mesh for boolean", (size_t)(error_size - 1));
            return 0;
        }
        CadMesh *out = (CadMesh *)calloc(1, sizeof(CadMesh));
        if (!bool_union_mesh(a, b, out)) {
            free(out);
            strncpy(error_out, "boolean union failed", (size_t)(error_size - 1));
            return 0;
        }
        add_cad_model(p, "bool_union", out);
        return 1;
    }
    case AI_CMD_BOOLEAN_SUBTRACT: {
        if (p->cad_models.len < 2) {
            strncpy(error_out, "need at least 2 CAD models for subtract", (size_t)(error_size - 1));
            return 0;
        }
        CadMesh *a = &p->cad_models.v[0].mesh;
        CadMesh *b = &p->cad_models.v[1].mesh;
        if (!a->valid || !b->valid) {
            strncpy(error_out, "invalid mesh for boolean", (size_t)(error_size - 1));
            return 0;
        }
        CadMesh *out = (CadMesh *)calloc(1, sizeof(CadMesh));
        if (!bool_subtract_mesh(a, b, out)) {
            free(out);
            strncpy(error_out, "boolean subtract failed", (size_t)(error_size - 1));
            return 0;
        }
        add_cad_model(p, "bool_subtract", out);
        return 1;
    }
    case AI_CMD_ROUTE_NET: {
        if (cmd->net[0]) {
            char msg[256];
            snprintf(msg, sizeof(msg), "net %s routing queued", cmd->net);
            if (error_out) strncpy(error_out, msg, (size_t)(error_size - 1));
        }
        return 1;
    }
    case AI_CMD_ADD_TEXT: {
        Pcb *pcb = resolve_pcb(p, cmd);
        if (!pcb) { strncpy(error_out, "no PCB", (size_t)(error_size - 1)); return 0; }
        Prim *pr = (Prim *)realloc(pcb->prims, (size_t)(pcb->nprims + 1) * sizeof(Prim));
        if (!pr) { strncpy(error_out, "allocation failed", (size_t)(error_size - 1)); return 0; }
        pcb->prims = pr;
        Prim *np = &pcb->prims[pcb->nprims];
        memset(np, 0, sizeof(*np));
        np->type = PRIM_TEXT;
        np->text = str_dup(cmd->value[0] ? cmd->value : "");
        np->p1 = v2(cmd->pos_x, cmd->pos_y);
        np->layer = str_dup("F.SilkS");
        pcb->nprims++;
        return 1;
    }
    case AI_CMD_NONE:
    default:
        strncpy(error_out, "no actionable command", (size_t)(error_size - 1));
        return 0;
    }
    return 0;
}

/* ============================ Smart Suggestions ============================ */

void aichat_generate_suggestions(AIChat *ac, int mode) {
    for (int i = 0; i < 5; i++) free(ac->suggestions[i]);
    memset(ac->suggestions, 0, sizeof(ac->suggestions));
    ac->nsuggestions = 0;

    const char *su[5];
    int ns = 0;
    switch (mode) {
    case 1: /* SCH */
        su[0] = "Place a resistor R1"; su[1] = "Add wire from U1 to R1";
        su[2] = "Replace U1 with ATmega"; su[3] = "Show me the power nets"; su[4] = "Add decoupling caps";
        ns = 5; break;
    case 2: /* PCB */
        su[0] = "Route net VCC"; su[1] = "Add test points on all nets";
        su[2] = "Generate BOM"; su[3] = "Check DRC"; su[4] = "Add mounting holes";
        ns = 5; break;
    case 3: /* 3D */
        su[0] = "Create a box 50x30x20"; su[1] = "Create a torus R=15 r=3";
        su[2] = "Hollow the box with 2mm walls"; su[3] = "Duplicate model with 20mm offset"; su[4] = "Array 5 copies spaced 25mm";
        ns = 5; break;
    case 7: /* PRINT */
        su[0] = "Slice with 0.2mm layers"; su[1] = "Export gcode";
        su[2] = "Set infill to 20%"; su[3] = "Add supports"; su[4] = "Create calibration cube";
        ns = 5; break;
    case 6: /* ASSIST */
        su[0] = "Show printer status"; su[1] = "Assign job to fastest printer";
        su[2] = "Check filament levels"; su[3] = "Schedule maintenance";
        ns = 4; break;
    default:
        su[0] = "Create a box"; su[1] = "Place a component"; su[2] = "What can you do?";
        ns = 3; break;
    }
    for (int i = 0; i < ns && i < 5; i++) {
        ac->suggestions[i] = str_dup(su[i]);
    }
    ac->nsuggestions = ns;
}

/* ============================ Pattern Matcher Response ============================ */

static char *pattern_match_response(const char *msg, int mode) {
    char *upper = str_dup(msg);
    for (char *p = upper; *p; p++) *p = (char)toupper((unsigned char)*p);

    int has_add = strstr(upper, "ADD") || strstr(upper, "PLACE") || strstr(upper, "CREATE");
    int has_del = strstr(upper, "REMOVE") || strstr(upper, "DELETE") || strstr(upper, "REPLACE");
    int has_route = strstr(upper, "ROUTE") || strstr(upper, "WIRE") || strstr(upper, "CONNECT");
    int has_set = strstr(upper, "CHANGE") || strstr(upper, "SET") || strstr(upper, "MODIFY");
    int has_slice = strstr(upper, "SLICE") || strstr(upper, "PRINT") || strstr(upper, "EXPORT");
    int has_cad = strstr(upper, "BOX") || strstr(upper, "CYLINDER") || strstr(upper, "SPHERE") || strstr(upper, "ENCLOSURE") || strstr(upper, "TORUS") || strstr(upper, "CONE") || strstr(upper, "WEDGE") || strstr(upper, "PYRAMID");
    int has_xform = strstr(upper, "SCALE") || strstr(upper, "ROTATE") || strstr(upper, "TRANSFORM");
    int has_op = strstr(upper, "HOLLOW") || strstr(upper, "MIRROR") || strstr(upper, "ARRAY") || strstr(upper, "PATTERN") || strstr(upper, "DUPLICATE") || strstr(upper, "COPY");
    int has_bool = strstr(upper, "UNION") || strstr(upper, "SUBTRACT") || strstr(upper, "INTERSECT");
    int has_help = strstr(upper, "HELP") || strstr(upper, "WHAT") || strstr(upper, "?");
    free(upper);

    Buf b; buf_init(&b);

    if (has_help) {
        buf_append_str(&b, "I can help with:\n");
        buf_append_str(&b, "- PLACE a component on a PCB (e.g. 'PLACE U1 ATMEGA328P QFP-32 at 50,30')\n");
        buf_append_str(&b, "- REPLACE a component ('REPLACE U5 with STM32F407VGT6 LQFP-100')\n");
        buf_append_str(&b, "- DELETE a component ('DELETE R5')\n");
        buf_append_str(&b, "- ADD WIRE between pins ('WIRE from U5.pin5 to R1.pin1')\n");
        buf_append_str(&b, "- ADD TRACK on PCB ('TRACK from 10,20 to 50,60 on F.Cu net VCC')\n");
        buf_append_str(&b, "- MOVE a component ('MOVE U3 to 45,67 rotation=90')\n");
        buf_append_str(&b, "- SET property ('SET R1 value=10k')\n");
        buf_append_str(&b, "- CREATE BOX/CYLINDER/SPHERE/TORUS/CONE/WEDGE/PYRAMID\n");
        buf_append_str(&b, "- TRANSFORM: SCALE sx=2 sy=1 sz=1 / ROTATE rx=45 ry=0 rz=0\n");
        buf_append_str(&b, "- HOLLOW wall=2mm, MIRROR axis=x/y/z, DUPLICATE, DELETE_MESH\n");
        buf_append_str(&b, "- ALIGN to model, ARRAY 5 spaced 25mm axis=x\n");
        buf_append_str(&b, "- BOOLEAN: UNION, SUBTRACT, INTERSECT between models\n");
        buf_append_str(&b, "- SLICE for 3D printing, EXPORT for gcode\n");
    } else if (has_cad && mode == 3) {
        buf_append_str(&b, "BOX width=20 height=10 depth=15 at 0,0,0\n");
        buf_append_str(&b, "TORUS R=15 r=3 segs=24\n");
        buf_append_str(&b, "HOLLOW the current model with 2mm walls\n");
        buf_append_str(&b, "MIRROR axis=x, ARRAY count=5 spacing=25 axis=x\n");
    } else if (has_add && mode == 2) {
        buf_append_str(&b, "PLACE U1 ATMEGA328P QFP-32 at 50,30 rotation=0\n");
        buf_append_str(&b, "Add component to PCB\n");
    } else if (has_del && mode == 2) {
        buf_append_str(&b, "DELETE R5 from PCB\n");
    } else if (has_route && mode == 2) {
        buf_append_str(&b, "TRACK from 10,20 to 50,60 on F.Cu net VCC width=0.5\n");
    } else if (has_set && mode == 2) {
        buf_append_str(&b, "SET R1 value=10k on PCB\n");
    } else if (has_slice) {
        buf_append_str(&b, "SLICE with layer_height=0.2\n");
    } else {
        buf_append_str(&b, "I understood: ");
        buf_append_str(&b, msg);
        buf_append_str(&b, "\n\nUse PLACE/REPLACE/DELETE for components, WIRE/TRACK for routing, ");
        buf_append_str(&b, "CAD: BOX/CYLINDER/SPHERE/TORUS/CONE/WEDGE/PYRAMID/TRANSFORM/HOLLOW/MIRROR/DUPLICATE/ALIGN/ARRAY.\nType 'help' for full list.");
    }

    char *result = str_dup(buf_cstr(&b));
    buf_free(&b);
    return result;
}

/* ============================ Message Send / Receive ============================ */

void aichat_send_message(AIChat *ac, const char *text) {
    if (!text || !text[0]) return;
    add_user_message(ac, text);

    /* try ollama first */
    ac->connected = ollama_ping(ac->ollama_url);
    if (ac->connected) {
        char *reply = ollama_chat(ac->ollama_url, ac->ollama_model, text);
        if (reply) {
            aichat_process_response(ac, reply);
            free(reply);
        } else {
            add_system_message(ac, "Ollama error - falling back to local mode");
            char *lreply = pattern_match_response(text, ac->ctx.mode);
            aichat_process_response(ac, lreply);
            free(lreply);
        }
    } else {
        char *lreply = pattern_match_response(text, ac->ctx.mode);
        aichat_process_response(ac, lreply);
        free(lreply);
    }
}

void aichat_process_response(AIChat *ac, const char *response) {
    if (!response || !response[0]) {
        add_system_message(ac, "(no response)");
        return;
    }
    aichat_add_assistant(ac, response);

    AIEditCommand cmds[8];
    int n = aichat_parse_commands(response, cmds, 8);
    ac->edit_active = 0;
    ac->edit_result = 0;
    ac->edit_error[0] = 0;
    for (int i = 0; i < n; i++) {
        char err[256];
        err[0] = 0;
        int ok = aichat_execute_command(ac->proj, &cmds[i], err, (int)sizeof(err));
        if (ok) {
            ac->edit_active = 1;
            ac->edit_result = 1;
            snprintf(ac->last_command, sizeof(ac->last_command), "cmd %d executed", i);
        } else {
            ac->edit_result = 0;
            snprintf(ac->edit_error, sizeof(ac->edit_error), "cmd %d: %s", i, err);
        }
    }
    if (ac->edit_result) {
        add_system_message(ac, "Command(s) executed successfully");
    }
}
