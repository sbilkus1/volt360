#include "autoupdate.h"
#include "util.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winhttp.h>
#pragma comment(lib, "winhttp.lib")
#endif

int autoupdate_compare_versions(const char *a, const char *b) {
    int va[3]={0}, vb[3]={0};
    sscanf(a,"%d.%d.%d",&va[0],&va[1],&va[2]);
    sscanf(b,"%d.%d.%d",&vb[0],&vb[1],&vb[2]);
    for(int i=0;i<3;i++){if(va[i]>vb[i])return 1;if(va[i]<vb[i])return -1;}
    return 0;
}

char *autoupdate_fetch_latest_version(const char *owner, const char *repo) {
#ifdef _WIN32
    wchar_t wh[256], url[512];
    MultiByteToWideChar(CP_UTF8,0,"api.github.com",-1,wh,256);
    swprintf(url,512,L"/repos/%S/%S/releases/latest",owner,repo);

    HINTERNET s=WinHttpOpen(L"Volt360/2.0",WINHTTP_ACCESS_TYPE_NO_PROXY,NULL,NULL,0);
    HINTERNET c=WinHttpConnect(s,wh,INTERNET_DEFAULT_HTTPS_PORT,0);
    HINTERNET r=WinHttpOpenRequest(c,L"GET",url,NULL,NULL,NULL,WINHTTP_FLAG_SECURE);
    // GitHub requires User-Agent
    LPCWSTR hdr = L"User-Agent: Volt360-Updater\r\nAccept: application/vnd.github+json\r\n";
    WinHttpSendRequest(r,hdr,(DWORD)wcslen(hdr),NULL,0,0,0);
    if(!WinHttpReceiveResponse(r,NULL)){WinHttpCloseHandle(r);WinHttpCloseHandle(c);WinHttpCloseHandle(s);return NULL;}
    char *buf=(char*)malloc(8192);int total=0;DWORD rd;
    while(WinHttpReadData(r,buf+total,8191-total,&rd)&&rd>0){total+=(int)rd;if(total>=8190)break;}buf[total]=0;
    WinHttpCloseHandle(r);WinHttpCloseHandle(c);WinHttpCloseHandle(s);
    // extract "tag_name":"v2.0.0"
    const char *t=strstr(buf,"\"tag_name\"");if(!t){free(buf);return NULL;}
    t=strchr(t,':');if(!t){free(buf);return NULL;}t+=2;if(*t=='\"')t++;
    const char *end=strchr(t,'\"');if(!end){free(buf);return NULL;}
    int len=(int)(end-t);char *ver=(char*)malloc((size_t)(len+1));
    memcpy(ver,t,(size_t)len);ver[len]=0;
    // strip leading 'v' if present
    if(ver[0]=='v'||ver[0]=='V') memmove(ver,ver+1,strlen(ver));
    free(buf);return ver;
#else
    (void)owner;(void)repo;return NULL;
#endif
}

int autoupdate_check(const char *owner, const char *repo, char *latest, int lsize) {
    char *ver=autoupdate_fetch_latest_version(owner,repo);
    if(!ver)return -1;
    snprintf(latest,lsize,"%s",ver);
    int cmp=autoupdate_compare_versions(ver,VOLT360_VERSION);
    int result=(cmp>0)?1:0;
    free(ver);
    return result;
}

bool autoupdate_download(const char *owner, const char *repo, const char *asset, const char *save) {
    (void)owner;(void)repo;(void)asset;(void)save;
    return false; // stub: would download release asset
}

char *autoupdate_run(const char *owner, const char *repo) {
    char latest[32]={0};
    int status=autoupdate_check(owner,repo,latest,32);
    char buf[256];
    if(status>0) snprintf(buf,sizeof(buf),"Update available: v%s (current v" VOLT360_VERSION "). Visit github.com/%s/%s/releases/latest",latest,owner,repo);
    else if(status==0) snprintf(buf,sizeof(buf),"Volt360 v" VOLT360_VERSION " is up to date");
    else snprintf(buf,sizeof(buf),"Could not check for updates (offline?)");
    return str_dup(buf);
}
