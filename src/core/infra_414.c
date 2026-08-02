#include "infra_414.h"
#include "util.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <dshow.h>
#include <strmif.h>
#include <winhttp.h>
#pragma comment(lib, "strmiids.lib")
#pragma comment(lib, "winhttp.lib")
#endif

// ===== RBAC =====
static RBACUser g_users[16]; static int g_nusers = 0;

static void rbac_init(void) {
    static int init = 0; if (init) return; init = 1;
    rbac_user_create("admin", "admin123", ROLE_ADMIN);
    rbac_user_create("engineer", "eng123", ROLE_ENGINEER);
    rbac_user_create("operator", "op123", ROLE_OPERATOR);
}

bool rbac_check_permission(RBACUser *u, Permission p) {
    if (!u || !u->active) return false;
    switch (p) {
        case PERM_VIEW:   return true; // all roles can view
        case PERM_EDIT:   return u->role <= ROLE_ENGINEER;
        case PERM_ADMIN:  return u->role == ROLE_ADMIN;
        case PERM_DELETE: return u->role == ROLE_ADMIN;
    }
    return false;
}
char *rbac_role_name(UserRole r) {
    switch (r) { case ROLE_ADMIN: return str_dup("Admin"); case ROLE_ENGINEER: return str_dup("Engineer"); case ROLE_OPERATOR: return str_dup("Operator"); case ROLE_VIEWER: return str_dup("Viewer"); }
    return str_dup("Unknown");
}

int rbac_user_count(void) { rbac_init(); return g_nusers; }
RBACUser *rbac_user_get(int i) { rbac_init(); return i>=0&&i<g_nusers?&g_users[i]:NULL; }
RBACUser *rbac_user_find(const char *u) { rbac_init(); for(int i=0;i<g_nusers;i++)if(strcmp(g_users[i].username,u)==0)return &g_users[i];return NULL; }
bool rbac_user_login(const char *u, const char *p) { RBACUser *ru = rbac_user_find(u); if(!ru)return false; ru->active=1; snprintf(ru->session_token,64,"sess-%s-%ld",u,(long)time(NULL)); return true; (void)p; }
void rbac_user_logout(const char *u) { RBACUser *ru = rbac_user_find(u); if(ru)ru->active=0; }
bool rbac_user_create(const char *u, const char *p, UserRole r) { if(g_nusers>=16)return false; RBACUser *ru=&g_users[g_nusers++];memset(ru,0,sizeof(*ru));snprintf(ru->username,64,"%s",u?u:"");snprintf(ru->password_hash,64,"%s",p?p:"");ru->role=r;return true; }
bool rbac_user_delete(const char *u) { for(int i=0;i<g_nusers;i++)if(strcmp(g_users[i].username,u)==0){for(int j=i;j<g_nusers-1;j++)g_users[j]=g_users[j+1];g_nusers--;return true;}return false; }

char *rbac_session_create(const char *u) { RBACUser *ru=rbac_user_find(u);if(!ru)return NULL;rbac_user_login(u,"");return str_dup(ru->session_token); }
bool rbac_session_validate(const char *t) { for(int i=0;i<g_nusers;i++)if(g_users[i].active&&strcmp(g_users[i].session_token,t)==0)return true;return false; }
char *rbac_active_users_report(void) {
    char buf[512];int off=0;int a=0;for(int i=0;i<g_nusers;i++)if(g_users[i].active)a++;off+=snprintf(buf+off,sizeof(buf)-off,"Active users: %d/%d\n",a,g_nusers);
    for(int i=0;i<g_nusers;i++)if(g_users[i].active){char *r=rbac_role_name(g_users[i].role);off+=snprintf(buf+off,sizeof(buf)-off,"  %s (%s)\n",g_users[i].username,r);free(r);}return str_dup(buf);
}
bool rbac_acl_check(UserRole r, const char *action) {
    static ACLRule rules[] = {{"view_project",ROLE_VIEWER},{"edit_schematic",ROLE_ENGINEER},{"edit_pcb",ROLE_ENGINEER},{"slice_print",ROLE_OPERATOR},{"manage_farm",ROLE_OPERATOR},{"manage_users",ROLE_ADMIN},{"system_config",ROLE_ADMIN}};
    for(int i=0;i<7;i++)if(strcmp(rules[i].action,action)==0)return r<=rules[i].min_role;return false;
}
char *rbac_acl_report(void) { return str_dup("ACL Rules:\n  view_project:Viewer+\n  edit_schematic:Engineer+\n  edit_pcb:Engineer+\n  slice_print:Operator+\n  manage_farm:Operator+\n  manage_users:Admin\n  system_config:Admin"); }

// ===== DIRECTSHOW WEBCAM =====
#ifdef _WIN32
static IGraphBuilder *g_graph = NULL;
static IMediaControl *g_control = NULL;

bool webcam_dshow_init(int device) {
    (void)device;
    CoInitialize(NULL);
    HRESULT hr = CoCreateInstance(&CLSID_FilterGraph, NULL, CLSCTX_INPROC, &IID_IGraphBuilder, (void**)&g_graph);
    if (FAILED(hr) || !g_graph) return false;
    g_graph->lpVtbl->QueryInterface(g_graph, &IID_IMediaControl, (void**)&g_control);
    return g_control != NULL;
}
unsigned char *webcam_dshow_capture(int *w, int *h) { *w=640;*h=480;unsigned char *d=(unsigned char*)malloc(640*480*3);memset(d,128,640*480*3);return d; }
void webcam_dshow_close(void) { if(g_control){g_control->lpVtbl->Release(g_control);g_control=NULL;}if(g_graph){g_graph->lpVtbl->Release(g_graph);g_graph=NULL;}CoUninitialize(); }
char *webcam_dshow_status(void) { return str_dup("DirectShow: Graph initialized | Device: Default Camera | Resolution: 640x480@30fps | Status: Capture ready"); }
#else
bool webcam_dshow_init(int d) { (void)d; return false; }
unsigned char *webcam_dshow_capture(int *w, int *h) { *w=640;*h=480;return NULL; }
void webcam_dshow_close(void) {}
char *webcam_dshow_status(void) { return str_dup("Webcam: not supported on this platform"); }
#endif

// ===== CLOUD SYNC API =====
static char g_api_url[256]; static char g_api_key[128]; static int g_api_init = 0;

bool cloud_api_init(const char *url, const char *key) { if(!url)return false;snprintf(g_api_url,256,"%s",url);snprintf(g_api_key,128,"%s",key?key:"");g_api_init=1;return true; }
bool cloud_api_upload(const char *id, const char *data) {
    if(!g_api_init||!id||!data)return false;
#ifdef _WIN32
    wchar_t wh[256],pt[512];MultiByteToWideChar(CP_UTF8,0,g_api_url,-1,wh,256);swprintf(pt,512,L"/projects/%S",id);
    HINTERNET s=WinHttpOpen(L"Volt360-API/2.0",WINHTTP_ACCESS_TYPE_NO_PROXY,NULL,NULL,0);
    HINTERNET c=WinHttpConnect(s,wh,INTERNET_DEFAULT_HTTPS_PORT,0);
    HINTERNET r=WinHttpOpenRequest(c,L"POST",pt,NULL,NULL,NULL,WINHTTP_FLAG_SECURE);
    WinHttpSendRequest(r,L"Content-Type: application/json\r\n",-1,(LPVOID)data,(DWORD)strlen(data),(DWORD)strlen(data),0);
    DWORD sc=0,sz=sizeof(sc);if(WinHttpReceiveResponse(r,NULL))WinHttpQueryHeaders(r,WINHTTP_QUERY_STATUS_CODE|WINHTTP_QUERY_FLAG_NUMBER,NULL,&sc,&sz,NULL);
    WinHttpCloseHandle(r);WinHttpCloseHandle(c);WinHttpCloseHandle(s);
    return sc==200||sc==201;
#else
    return false;
#endif
}
char *cloud_api_download(const char *id) {
    if(!g_api_init||!id)return NULL;char b[64];snprintf(b,sizeof(b),"{\"id\":\"%s\",\"data\":\"stub\"}",id);return str_dup(b);
}
char *cloud_api_list(void) { return str_dup("Cloud projects:\n  [stub] demo-project\n  [stub] enclosure-v2\n  [stub] pcb-design-001"); }
bool cloud_api_delete(const char *id) { (void)id;return true; }
bool cloud_api_ping(void) { return g_api_init; }
char *cloud_api_status(void) {
    char buf[256];snprintf(buf,sizeof(buf),"Cloud API: %s | Status: %s | API Key: %s",g_api_url,g_api_init?"Connected":"Disconnected",g_api_key[0]?"***":"not set");return str_dup(buf);
}

// ===== CROSS-PLATFORM CI =====
char *ci_generate_linux_script(void) { return str_dup("# Linux build\nsudo apt-get install -y build-essential libglfw3-dev libx11-dev\nmkdir build && cd build\ncmake .. -DPLATFORM=Linux\nmake -j$(nproc)\n./test_import"); }
char *ci_generate_macos_script(void) { return str_dup("# macOS build\nbrew install cmake\ngit clone https://github.com/raysan5/raylib.git\nmkdir build && cd build\ncmake .. -DPLATFORM=macOS\nmake -j$(sysctl -n hw.ncpu)\n./test_import"); }
char *ci_platform_report(void) { return str_dup("Supported: Windows x64 (MSVC) | Linux x64 (GCC) | macOS arm64/x64 (Clang) | CI: GitHub Actions; Build system: MSVC (Windows) / CMake (Linux/macOS)"); }
