#pragma once
// autoupdate.h - Auto-updater: check GitHub releases for new versions.
#include <stdbool.h>

// Current app version.
#define VOLT360_VERSION "2.0.0"

// Check for updates by querying the GitHub releases API.
// Returns: 0=up-to-date, 1=update available, -1=error
int autoupdate_check(const char *repo_owner, const char *repo_name, char *latest_version, int version_size);

// Query GitHub API and return the latest release tag.
char *autoupdate_fetch_latest_version(const char *repo_owner, const char *repo_name);

// Compare two version strings (e.g. "2.0.0" > "1.9.0").
// Returns: 1 if a > b, -1 if a < b, 0 if equal
int autoupdate_compare_versions(const char *a, const char *b);

// Download the latest release asset to a local path.
bool autoupdate_download(const char *repo_owner, const char *repo_name, const char *asset_name, const char *save_path);

// Full update flow: check, download, prompt to install.
// Returns status text.
char *autoupdate_run(const char *repo_owner, const char *repo_name);
