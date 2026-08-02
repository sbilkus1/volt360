#pragma once
// installer.h - App installer with login, library selection, custom import.
#include <stdbool.h>
#include "../model/project.h"

// ===== ACCOUNT SYSTEM =====
typedef struct {
    char provider[16];    // "google", "github", "apple", "email"
    char email[128];
    char display_name[64];
    char token[256];      // simulated auth token
    int logged_in;
} UserAccount;

// Check if user is logged in (account persisted to disk).
bool account_is_logged_in(void);
UserAccount *account_get(void);
bool account_login(const char *provider, const char *email, const char *name);
void account_logout(void);
bool account_save(void);
bool account_load(void);

// ===== INSTALLER FLOW =====
typedef enum {
    INSTALL_WELCOME,     // Welcome screen
    INSTALL_LOGIN,       // Login / create account
    INSTALL_AI_SETUP,    // GPU detect + AI model choice
    INSTALL_CLOUD_SAVE,  // Cloud save backend choice
    INSTALL_LIBS,        // Library selection
    INSTALL_IMPORT,      // Custom library import
    INSTALL_PROGRESS,    // Installation progress
    INSTALL_DONE,        // Complete!
    INSTALL_FINISHED     // Exit installer
} InstallStep;

// Library package definition.
typedef struct {
    char name[64];
    char description[128];
    int size_mb;
    int selected;       // checkbox state
    int installed;      // already installed
} LibraryPackage;

// Get available library packages.
LibraryPackage *installer_libraries(int *count);

// Installer flow state.
typedef struct {
    InstallStep step;
    float progress_pct;
    char status_msg[256];
    LibraryPackage *libs; int n_libs;
    char custom_path[512];
    int custom_import;
} Installer;

Installer *installer_init(void);
void installer_free(Installer *inst);

// Draw the current installer step. Returns INSTALL_FINISHED when done.
InstallStep installer_draw(Installer *inst, int sw, int sh);
InstallStep installer_handle_input(Installer *inst);

// Run selected library installations.
void installer_run(Installer *inst, Project *p);
