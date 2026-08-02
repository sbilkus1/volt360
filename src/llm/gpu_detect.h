#pragma once
// gpu_detect.h - GPU detection, VRAM query, AI model recommendations.
#include <stdbool.h>

// GPU info.
typedef struct {
    char name[128];         // e.g. "NVIDIA GeForce RTX 4090"
    char vendor[32];        // "NVIDIA", "AMD", "Intel", "Unknown"
    unsigned long long vram_mb; // dedicated video memory in MB
    int is_discrete;        // 1 = dedicated GPU, 0 = integrated
} GPUInfo;

// Detect all GPUs. Returns count, fills array.
int gpu_detect_all(GPUInfo *gpus, int max_gpus);

// Get the best GPU (most VRAM).
GPUInfo *gpu_detect_best(void);

// Check if GPU is suitable for local LLM (8GB+ VRAM recommended).
bool gpu_can_run_llm(GPUInfo *gpu);

// ===== AI MODEL RECOMMENDATIONS =====
typedef struct {
    char name[64];          // "llama3.2:3b", "mistral:7b", "llama3.1:70b"
    char description[128];
    unsigned long long min_vram_mb;
    unsigned long long recommended_vram_mb;
    float size_gb;
    int is_recommended;     // 1 if recommended for detected GPU
} AIModel;

// Get recommended models for the detected VRAM.
int ai_model_recommendations(unsigned long long vram_mb, AIModel *models, int max_models);

// ===== CLOUD AI PROVIDERS =====
typedef struct {
    char provider[32];      // "openai", "anthropic", "google", "deepseek", "groq", "together"
    char display_name[64];
    char api_key[256];
    char endpoint[256];
    char model[64];         // e.g. "gpt-4o", "claude-3-opus", "gemini-2.0-flash"
    int enabled;
} CloudAI;

// Get available cloud providers.
int cloud_ai_providers(CloudAI *providers, int max);

// Save/load cloud AI config.
bool cloud_ai_save(CloudAI *providers, int count);
bool cloud_ai_load(CloudAI *providers, int *count, int max);

// ===== AI SETUP STATE =====
typedef struct {
    GPUInfo gpu;
    int has_gpu;
    int use_local;          // 0=cloud, 1=local
    AIModel local_model;
    int local_model_idx;
    CloudAI cloud_providers[8];
    int n_cloud;
    int step;               // 0=detect, 1=choose, 2=cloud_keys, 3=done
    char status[256];
} AISetup;

AISetup *ai_setup_init(void);
void ai_setup_free(AISetup *ai);
