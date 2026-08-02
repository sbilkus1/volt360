#pragma once
// cloudsave.h - Multi-backend cloud save system: local, our server, OneDrive, Google Drive, custom.
#include <stdbool.h>

typedef enum { SAVE_LOCAL, SAVE_SERVER, SAVE_ONEDRIVE, SAVE_GOOGLE_DRIVE, SAVE_CUSTOM } CloudBackend;

// Cloud save configuration.
typedef struct {
    CloudBackend backend;
    char server_url[256];      // our server or custom server URL
    char auth_token[256];      // OAuth/API token
    char folder_path[256];     // remote folder path
    int auto_sync;             // 1 = auto-sync on save
    int sync_interval_min;     // minutes between auto-syncs
} CloudSaveConfig;

// Initialize with defaults.
void cloudsave_defaults(CloudSaveConfig *cfg);

// Save project to configured backend.
bool cloudsave_sync(CloudSaveConfig *cfg, const char *project_path, const char *project_name);

// Download/restore project from cloud.
bool cloudsave_restore(CloudSaveConfig *cfg, const char *project_name, const char *local_path);

// List projects on the cloud backend.
char *cloudsave_list(CloudSaveConfig *cfg);

// Check if backend is accessible (auth valid).
bool cloudsave_ping(CloudSaveConfig *cfg);

// OneDrive auth (device code flow stub).
char *onedrive_auth_url(void);
bool onedrive_auth_complete(const char *code);

// Google Drive auth (OAuth stub).
char *googledrive_auth_url(void);
bool googledrive_auth_complete(const char *code);

// Our server registration.
bool server_register(const char *email, const char *password, char *token, int token_size);
bool server_login(const char *email, const char *password, char *token, int token_size);

// Custom server setup.
bool custom_server_configure(CloudSaveConfig *cfg, const char *url, const char *token);

// Generate installer download page (index.html for GitHub Pages).
bool website_generate(const char *output_dir);
