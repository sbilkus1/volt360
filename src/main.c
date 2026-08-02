#include "ui/ui.h"
#include "ui/installer.h"
#include "core/util.h"
#include "core/cloudsave.h"
#include "import/importer.h"
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    // --website: generate GitHub Pages site and exit (no window needed)
    extern int __argc; extern char **__argv;
    for (int i = 1; i < __argc; i++) {
        if (strcmp(__argv[i], "--website") == 0) {
            website_generate("website");
            printf("Website generated at website/index.html\n");
            return 0;
        }
    }

    int W = 1280, H = 800;
    InitWindow(W, H, "Volt360");
    // Set app icon from logo file
    Image logo = LoadImage("Volt360 logo.png");
    if (logo.data) { SetWindowIcon(logo); UnloadImage(logo); }
    SetTargetFPS(60);
    SetConfigFlags(FLAG_MSAA_4X_HINT);

    App app;
    app_init(&app);

    // Installer flow (first run or not logged in) — skip in SMOKE mode
    if (!account_is_logged_in() && !getenv("SMOKE")) {
        Installer *inst = installer_init();
        SetTargetFPS(60);
        while (!WindowShouldClose() && inst->step != INSTALL_FINISHED) {
            BeginDrawing();
            installer_draw(inst, W, H);
            EndDrawing();
        }
        // run selected library installations
        if (inst->step == INSTALL_FINISHED) installer_run(inst, &app.proj);
        installer_free(inst);
        // recheck login in case user logged in
        account_load();
        if (!account_is_logged_in()) {
            // continue to app anyway in offline mode
            account_login("offline", "offline@local", "Offline User");
        }
        app_init(&app); // re-init to pick up any imported data
    }

    // optional: auto-open a folder passed as argv[1]
    const char *arg = NULL;
    // first arg after program name
    // (raylib leaves args intact; use __argc/__argv on MSVC)
    extern int __argc; extern char **__argv;
    if (__argc > 1) arg = __argv[1];

    if (arg) {
        app_open_folder(&app, arg);
    }

    // --website: generate GitHub Pages site and exit
    extern int __argc; extern char **__argv;
    for (int i = 1; i < __argc; i++) {
        if (strcmp(__argv[i], "--website") == 0) {
            website_generate("website");
            printf("Website generated at website/index.html\n");
            return 0;
        }
    }

    if (getenv("SMOKE")) {
        // cycle through all modes to exercise render paths
        app.mode = UI_SCH;
        for (int i = 0; i < 5; i++) app_frame(&app);
        app.mode = UI_PCB;
        for (int i = 0; i < 5; i++) app_frame(&app);
        app.mode = UI_3D;
        app.yaw = 20; app.pitch = -30;
        for (int i = 0; i < 5; i++) app_frame(&app);
        app.mode = UI_FIT;
        for (int i = 0; i < 5; i++) app_frame(&app);
        app.mode = UI_DESIGN;
        for (int i = 0; i < 5; i++) app_frame(&app);
        app.mode = UI_ASSIST;
        for (int i = 0; i < 5; i++) app_frame(&app);
        app.mode = UI_PRINT;
        for (int i = 0; i < 5; i++) app_frame(&app);
        CloseWindow();
        printf("SMOKE_OK\n");
        app_free(&app);
        return 0;
    } else {
        app_run(&app);
    }

    app_free(&app);
    CloseWindow();
    return 0;
}
