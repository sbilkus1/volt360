#pragma once
// infra_414.h - Infrastructure: RBAC, Webcam, Cloud API, Cross-platform CI.
#include <stdbool.h>
#include "../model/project.h"

// ===== MULTI-USER RBAC =====
typedef enum { ROLE_ADMIN, ROLE_ENGINEER, ROLE_OPERATOR, ROLE_VIEWER } UserRole;

typedef struct {
    char username[64];
    char password_hash[64]; // simple SHA256 placeholder
    UserRole role;
    int active;           // 1 = logged in
    char session_token[64];
} RBACUser;

// Permission check.
typedef enum { PERM_VIEW, PERM_EDIT, PERM_ADMIN, PERM_DELETE } Permission;
bool rbac_check_permission(RBACUser *user, Permission p);
char *rbac_role_name(UserRole r);

// User management.
int rbac_user_count(void);
RBACUser *rbac_user_get(int idx);
RBACUser *rbac_user_find(const char *username);
bool rbac_user_login(const char *username, const char *password);
void rbac_user_logout(const char *username);
bool rbac_user_create(const char *username, const char *password, UserRole role);
bool rbac_user_delete(const char *username);

// Session management.
char *rbac_session_create(const char *username);
bool rbac_session_validate(const char *token);
char *rbac_active_users_report(void);

// Access control list for project operations.
typedef struct { char action[32]; UserRole min_role; } ACLRule;
bool rbac_acl_check(UserRole role, const char *action);
char *rbac_acl_report(void);

// ===== REAL WEBCAM (DirectShow) =====
// Initialize webcam capture.
bool webcam_dshow_init(int device_index);
// Capture a frame and return raw RGB data.
unsigned char *webcam_dshow_capture(int *w, int *h);
// Release webcam.
void webcam_dshow_close(void);
// Status.
char *webcam_dshow_status(void);

// ===== CLOUD SYNC API CLIENT =====
// Initialize cloud connection.
bool cloud_api_init(const char *server_url, const char *api_key);
// Upload project to cloud.
bool cloud_api_upload(const char *project_id, const char *json_data);
// Download project from cloud.
char *cloud_api_download(const char *project_id);
// List projects on cloud.
char *cloud_api_list(void);
// Delete project from cloud.
bool cloud_api_delete(const char *project_id);
// Ping server.
bool cloud_api_ping(void);
// Status.
char *cloud_api_status(void);

// ===== CROSS-PLATFORM CI =====
// Generate CI build script for platform.
char *ci_generate_linux_script(void);
char *ci_generate_macos_script(void);
// Report supported platforms.
char *ci_platform_report(void);
