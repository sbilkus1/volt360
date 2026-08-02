#include "gpu_detect.h"
#include "../core/util.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

static int gpu_detect_dxgi(GPUInfo *gpus, int max_gpus) {
    int count = 0;
    HMODULE dxgi = LoadLibraryA("dxgi.dll");
    if (!dxgi) return 0;
    typedef HRESULT (WINAPI *CreateDXGIFactory1_t)(REFIID, void**);
    CreateDXGIFactory1_t pCreateDXGIFactory1 = (CreateDXGIFactory1_t)GetProcAddress(dxgi, "CreateDXGIFactory1");
    if (!pCreateDXGIFactory1) { FreeLibrary(dxgi); return 0; }
    void *factory = NULL;
    static const GUID IID_IDXGIFactory1 = {0x770aae78,0xf26f,0x4dba,{0xa8,0x29,0x25,0x3c,0x83,0xd1,0xb3,0x87}};
    if (pCreateDXGIFactory1(&IID_IDXGIFactory1, &factory) < 0 || !factory) { FreeLibrary(dxgi); return 0; }
    for (unsigned int i = 0; i < (unsigned int)max_gpus; i++) {
        void *adapter = NULL;
        void **vtable = *(void***)factory;
        typedef HRESULT (WINAPI *EnumAdapters1_t)(void*, unsigned int, void**);
        EnumAdapters1_t EnumAdapters1 = (EnumAdapters1_t)vtable[12];
        if (EnumAdapters1(factory, i, &adapter) < 0 || !adapter) break;
        void **avtable = *(void***)adapter;
        typedef HRESULT (WINAPI *GetDesc1_t)(void*, void*);
        GetDesc1_t GetDesc1 = (GetDesc1_t)avtable[10];
        struct { wchar_t Description[128]; unsigned int VendorId, DeviceId, SubSysId, Revision; unsigned long long DedicatedVideoMemory, DedicatedSystemMemory, SharedSystemMemory; } desc;
        if (GetDesc1(adapter, &desc) >= 0) {
            GPUInfo *g = &gpus[count];
            WideCharToMultiByte(CP_UTF8,0,desc.Description,-1,g->name,128,NULL,NULL);
            if (desc.VendorId == 0x10DE) snprintf(g->vendor,32,"NVIDIA");
            else if (desc.VendorId == 0x1002 || desc.VendorId == 0x1022) snprintf(g->vendor,32,"AMD");
            else if (desc.VendorId == 0x8086) snprintf(g->vendor,32,"Intel");
            else snprintf(g->vendor,32,"Unknown");
            g->vram_mb = desc.DedicatedVideoMemory / (1024*1024);
            g->is_discrete = (desc.DedicatedVideoMemory > 512*1024*1024) ? 1 : 0;
            count++;
        }
        typedef unsigned long (WINAPI *Release_t)(void*);
        ((Release_t)((void**)adapter)[2])(adapter);
    }
    ((void (WINAPI*)(void*))((void**)factory)[2])(factory);
    FreeLibrary(dxgi);
    return count;
}
#else
static int gpu_detect_dxgi(GPUInfo *g, int m) { (void)g; (void)m; return 0; }
#endif

int gpu_detect_all(GPUInfo *g, int m) { return gpu_detect_dxgi(g, m); }

GPUInfo *gpu_detect_best(void) {
    static GPUInfo gpus[4]; static int count = -1;
    if (count < 0) count = gpu_detect_all(gpus, 4);
    if (count == 0) return NULL;
    int best = 0;
    for (int i = 1; i < count; i++) if (gpus[i].vram_mb > gpus[best].vram_mb) best = i;
    return &gpus[best];
}

bool gpu_can_run_llm(GPUInfo *gpu) { return gpu && gpu->vram_mb >= 6000 && gpu->is_discrete; }

int ai_model_recommendations(unsigned long long vram, AIModel *out, int max) {
    static AIModel g_models[] = {
        {"llama3.2:3b","Meta Llama 3.2 3B",4000,6000,2.0f,0},{"mistral:7b","Mistral 7B",6000,8000,4.1f,0},
        {"llama3.1:8b","Meta Llama 3.1 8B",7000,10000,4.7f,0},{"phi4:14b","Microsoft Phi-4 14B",10000,14000,9.1f,0},
        {"qwen2.5:32b","Qwen 2.5 32B",20000,28000,19.0f,0},{"llama3.1:70b","Meta Llama 3.1 70B",38000,48000,40.0f,0},
        {"deepseek-r1:70b","DeepSeek R1 70B",40000,48000,42.0f,0},
    };
    int n = 7; if (n > max) n = max;
    for (int i = 0; i < n; i++) { out[i] = g_models[i]; out[i].is_recommended = (vram >= out[i].recommended_vram_mb) ? 1 : 0; }
    return n;
}

int cloud_ai_providers(CloudAI *p, int max) {
    static CloudAI def[] = {
        {"openai","OpenAI","","https://api.openai.com/v1","gpt-4o",0},
        {"anthropic","Anthropic","","https://api.anthropic.com/v1","claude-3-opus",0},
        {"google","Google Gemini","","https://generativelanguage.googleapis.com/v1beta","gemini-2.0-flash",0},
        {"deepseek","DeepSeek","","https://api.deepseek.com/v1","deepseek-chat",0},
        {"groq","Groq","","https://api.groq.com/openai/v1","llama-3.3-70b",0},
        {"together","Together AI","","https://api.together.xyz/v1","llama-3.3-70b",0},
    };
    int n = 6; if (n > max) n = max;
    for (int i = 0; i < n; i++) p[i] = def[i];
    return n;
}

bool cloud_ai_save(CloudAI *p, int n) {
    FILE *f = fopen("build\\cloud_ai.json","w"); if(!f)return false;
    fprintf(f,"{\"providers\":[");
    for(int i=0;i<n;i++) fprintf(f,"{\"provider\":\"%s\",\"enabled\":%d}%s",p[i].provider,p[i].enabled,i<n-1?",":"");
    fprintf(f,"]}"); fclose(f); return true;
}

bool cloud_ai_load(CloudAI *p, int *count, int max) {
    size_t len; char *d = file_read("build\\cloud_ai.json",&len);
    if(!d)return false;
    *count = cloud_ai_providers(p,max);
    const char *cur=d;int idx=0;
    while((cur=strstr(cur,"\"provider\""))&&idx<*count){cur=strchr(cur,':')+2;if(*cur=='"')cur++;
        const char*end=strchr(cur,'"');if(end){char pr[32];int pl=(int)(end-cur);if(pl>31)pl=31;memcpy(pr,cur,(size_t)pl);pr[pl]=0;
        for(int i=0;i<*count;i++)if(strcmp(p[i].provider,pr)==0){p[i].enabled=1;idx++;break;}}}
    free(d); return true;
}

// lightweight AI setup state (no raylib drawing - that's in installer.c)
AISetup *ai_setup_init(void) {
    AISetup *ai = (AISetup *)calloc(1,sizeof(AISetup));
    GPUInfo *gpu = gpu_detect_best();
    if (gpu) { ai->gpu = *gpu; ai->has_gpu = 1; }
    ai->n_cloud = cloud_ai_providers(ai->cloud_providers,8);
    cloud_ai_load(ai->cloud_providers,&ai->n_cloud,8);
    ai->use_local = ai->has_gpu && gpu_can_run_llm(gpu?gpu:NULL) ? 1 : 0;
    return ai;
}

void ai_setup_free(AISetup *ai) { if(ai)free(ai); }
