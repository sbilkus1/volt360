#include "installer.h"
#include "../core/util.h"
#include "../import/kicad_lib.h"
#include "../slicer/fusion_lib.h"
#include "../llm/gpu_detect.h"
#include <raylib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static UserAccount g_account = {0};
static int g_account_loaded = 0;

bool account_is_logged_in(void) {
    if (!g_account_loaded) account_load();
    return g_account.logged_in;
}

UserAccount *account_get(void) { return &g_account; }

bool account_login(const char *provider, const char *email, const char *name) {
    snprintf(g_account.provider, sizeof(g_account.provider), "%s", provider ? provider : "email");
    snprintf(g_account.email, sizeof(g_account.email), "%s", email ? email : "");
    snprintf(g_account.display_name, sizeof(g_account.display_name), "%s", name ? name : email ? email : "User");
    snprintf(g_account.token, sizeof(g_account.token), "sim_%s_%ld", provider, (long)GetTime());
    g_account.logged_in = 1;
    return account_save();
}

void account_logout(void) { memset(&g_account, 0, sizeof(g_account)); account_save(); }

bool account_save(void) {
    return file_write("build\\account.json", "{\"logged\":true}", 16);
}

bool account_load(void) {
    g_account_loaded = 1;
    size_t len; char *d = file_read("build\\account.json", &len);
    if (d && strstr(d, "logged")) { g_account.logged_in = 1; snprintf(g_account.provider,16,"email"); snprintf(g_account.email,128,"user@volt360.local"); snprintf(g_account.display_name,64,"User"); free(d); return true; }
    free(d); return false;
}

// ===== LIBRARY PACKAGES =====
static LibraryPackage g_packages[] = {
    {"KiCad Symbols","Standard KiCad symbol library (5000+ symbols)",120,0,0},
    {"KiCad Footprints","Standard KiCad footprint library (3000+ footprints)",85,0,0},
    {"KiCad 3D Models","STEP/WRL 3D models for footprints (2000+ models)",450,0,0},
    {"KiCad Templates","Project templates (Arduino, PSU, etc.)",2,0,0},
    {"FreeCAD Parts","Standard mechanical part library",180,0,0},
    {"Fusion CAM Tools","25 standard CNC tools + feeds/speeds",1,1,0},
    {"Fusion Materials","15 machining materials with properties",1,1,0},
    {"Fusion Machines","7 machine definitions with post-processors",1,1,0},
    {"OrcaSlicer Profiles","Printer profiles (Voron, Bambu, Prusa, etc.)",1,0,0},
    {"OrcaSlicer Calibration","Calibration test suite G-code generators",1,1,0},
    {"iEDA Algorithms","EDA algorithms: placement, routing, CTS",2,0,0},
    {"Loop Auto-Eject","Continuous printing + conveyance system",1,1,0},
};

LibraryPackage *installer_libraries(int *count) { *count = 12; return g_packages; }

// ===== INSTALLER =====
Installer *installer_init(void) {
    Installer *inst = (Installer *)calloc(1, sizeof(Installer));
    inst->step = INSTALL_WELCOME;
    inst->libs = installer_libraries(&inst->n_libs);
    // pre-select a sensible default set
    for (int i = 0; i < inst->n_libs; i++) inst->libs[i].selected = 1;
    inst->custom_path[0] = 0;
    return inst;
}

void installer_free(Installer *inst) { if (inst) free(inst); }

static int g_login_email_len = 0;
static char g_login_email[128] = "";
static char g_login_name[64] = "";
static int g_name_field = 0;

InstallStep installer_draw(Installer *inst, int sw, int sh) {
    if (!inst) return INSTALL_FINISHED;
    Color bg = {28,30,34,255}, panel = {35,38,45,255}, text = {220,225,235,255}, accent = {100,160,220,255};
    ClearBackground(bg);
    int cx = sw/2, cy = sh/2;

    switch (inst->step) {
        case INSTALL_WELCOME: {
            DrawText("Welcome to Volt360", cx - MeasureText("Welcome to Volt360", 28)/2, cy-120, 28, accent);
            DrawText("All-in-one EDA + CAD + Slicer + Print Farm", cx - MeasureText("All-in-one EDA + CAD + Slicer + Print Farm", 16)/2, cy-80, 16, GRAY);
            DrawText("Import KiCad, FreeCAD, OrcaSlicer, Fusion, iEDA libraries", cx-380, cy-50, 15, text);
            DrawText("Design schematics, PCBs, enclosures, and print them", cx-380, cy-28, 15, text);
            DrawText("Manage your print farm with Loop auto-ejection", cx-380, cy-6, 15, text);

            // Get Started button
            int bx = cx - 100, by = cy + 60, bw = 200, bh = 40;
            Vector2 mp = GetMousePosition();
            int hover = mp.x >= bx && mp.x <= bx+bw && mp.y >= by && mp.y <= by+bh;
            DrawRectangle(bx, by, bw, bh, hover ? (Color){70,90,130,255} : (Color){55,60,75,255});
            DrawRectangleLines(bx, by, bw, bh, accent);
            DrawText("Get Started", bx + 40, by + 10, 20, WHITE);
            if (hover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) inst->step = INSTALL_LOGIN;

            // Skip button
            DrawText("Skip login (offline mode)", cx - MeasureText("Skip login (offline mode)", 13)/2, cy+120, 13, DARKGRAY);
            if (mp.x >= cx-80 && mp.x <= cx+80 && mp.y >= cy+115 && mp.y <= cy+130 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                account_login("offline", "offline@local", "Offline User");
                inst->step = INSTALL_AI_SETUP;
            }
            break;
        }

        case INSTALL_LOGIN: {
            DrawText("Sign In / Create Account", cx - MeasureText("Sign In / Create Account", 22)/2, 60, 22, accent);

            // OAuth buttons
            const char *providers[] = {"Google","GitHub","Apple"};
            int px = cx - 180;
            for (int i = 0; i < 3; i++) {
                Vector2 mp = GetMousePosition();
                int hov = mp.x >= px+i*130 && mp.x <= px+i*130+120 && mp.y >= 110 && mp.y <= 150;
                DrawRectangle(px+i*130, 110, 120, 40, hov ? (Color){70,90,130,255} : (Color){50,55,68,255});
                DrawRectangleLines(px+i*130, 110, 120, 40, accent);
                DrawText(providers[i], px+i*130+25, 120, 18, WHITE);
                if (hov && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    account_login(providers[i], "user@provider.com", providers[i]);
                    inst->step = INSTALL_AI_SETUP;
                }
            }

            // Email login
            DrawText("Or sign up with email:", cx-100, 180, 15, GRAY);
            // Email field
            DrawRectangle(cx-150, 210, 300, 32, (Color){40,44,56,255});
            DrawRectangleLines(cx-150, 210, 300, 32, g_name_field==1?accent:(Color){60,64,76,255});
            DrawText(g_login_email[0] ? g_login_email : "Email address", cx-142, 216, 16, g_login_email[0]?WHITE:GRAY);
            // Name field
            DrawRectangle(cx-150, 252, 300, 32, (Color){40,44,56,255});
            DrawRectangleLines(cx-150, 252, 300, 32, g_name_field==2?accent:(Color){60,64,76,255});
            DrawText(g_login_name[0] ? g_login_name : "Display name", cx-142, 258, 16, g_login_name[0]?WHITE:GRAY);

            // handle typing - simplified via key capture
            int key = GetKeyPressed();
            if (key >= 32 && key <= 126) {
                if (g_name_field == 1 && g_login_email_len < 126) { g_login_email[g_login_email_len++] = (char)key; g_login_email[g_login_email_len]=0; }
                else if (g_name_field == 2 && (int)strlen(g_login_name) < 62) { int l=(int)strlen(g_login_name); g_login_name[l]=(char)key; g_login_name[l+1]=0; }
            }
            if (key == 259 && g_name_field == 1 && g_login_email_len > 0) g_login_email[--g_login_email_len] = 0;
            if (key == 259 && g_name_field == 2 && g_login_name[0]) g_login_name[strlen(g_login_name)-1] = 0;

            Vector2 mp = GetMousePosition();
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (mp.x >= cx-150 && mp.x <= cx+150 && mp.y >= 210 && mp.y <= 242) g_name_field = 1;
                else if (mp.x >= cx-150 && mp.x <= cx+150 && mp.y >= 252 && mp.y <= 284) g_name_field = 2;
            }

            // Continue button
            int bx = cx - 80, by = 310;
            int hov2 = mp.x >= bx && mp.x <= bx+160 && mp.y >= by && mp.y <= by+36;
            DrawRectangle(bx, by, 160, 36, hov2 ? (Color){70,90,130,255} : accent);
            DrawText("Continue", bx+35, by+8, 18, WHITE);
            if (hov2 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && g_login_email[0]) {
                account_login("email", g_login_email, g_login_name[0]?g_login_name:g_login_email);
                inst->step = INSTALL_AI_SETUP;
            }

            DrawText("Back", 20, sh-30, 16, GRAY);
            if (mp.x >= 20 && mp.x <= 80 && mp.y >= sh-35 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) inst->step = INSTALL_WELCOME;
            break;
        }

        case INSTALL_AI_SETUP: {
            static AISetup *g_ai = NULL; static int g_ai_step = 0;
            if (!g_ai) { g_ai = ai_setup_init(); g_ai_step = 0; }
            Color bg2={28,30,34,255}, panel2={35,38,45,255}, text2={220,225,235,255}, acc2={100,160,220,255}, gn2={50,200,50,255};
            int cx2=sw/2, cy2=sh/2; Vector2 mp2=GetMousePosition();
            switch(g_ai_step){
                case 0: DrawText("AI Setup - GPU Detection",cx2-MeasureText("AI Setup - GPU Detection",24)/2,60,24,acc2);
                    if(g_ai->has_gpu){char b[256];snprintf(b,sizeof(b),"Detected: %s (%s)",g_ai->gpu.name,g_ai->gpu.vendor);DrawText(b,cx2-MeasureText(b,16)/2,130,16,gn2);
                        snprintf(b,sizeof(b),"VRAM: %llu MB",g_ai->gpu.vram_mb);DrawText(b,cx2-MeasureText(b,15)/2,155,15,text2);}
                    else DrawText("No GPU - Cloud AI only",cx2-120,130,16,ORANGE);
                    DrawText("Continue",cx2-30,220,18,acc2);
                    if(mp2.x>=cx2-30&&mp2.y>=220&&mp2.y<=240&&IsMouseButtonPressed(MOUSE_BUTTON_LEFT))g_ai_step=1; break;
                case 1: DrawText("Choose AI Mode",cx2-MeasureText("Choose AI Mode",24)/2,60,24,acc2);
                    DrawRectangle(cx2-200,120,400,60,panel2);DrawRectangleLines(cx2-200,120,400,60,g_ai->use_local?gn2:(Color){60,64,76,255});
                    DrawText(g_ai->use_local?"> Local LLM (Ollama)":"  Local LLM (Ollama)",cx2-188,130,16,WHITE);
                    DrawRectangle(cx2-200,190,400,60,panel2);DrawRectangleLines(cx2-200,190,400,60,!g_ai->use_local?gn2:(Color){60,64,76,255});
                    DrawText(!g_ai->use_local?"> Cloud AI (API)":"  Cloud AI (API)",cx2-188,200,16,WHITE);
                    if(mp2.x>=cx2-200&&mp2.x<=cx2+200&&mp2.y>=120&&mp2.y<=180&&IsMouseButtonPressed(MOUSE_BUTTON_LEFT))g_ai->use_local=1;
                    if(mp2.x>=cx2-200&&mp2.x<=cx2+200&&mp2.y>=190&&mp2.y<=250&&IsMouseButtonPressed(MOUSE_BUTTON_LEFT))g_ai->use_local=0;
                    if(g_ai->use_local){AIModel ms[7];int nm=ai_model_recommendations(g_ai->gpu.vram_mb,ms,7);
                        for(int i=0;i<nm&&i<4;i++){int my=270+i*24;DrawText(ms[i].name,cx2-180,my,13,ms[i].is_recommended?gn2:GRAY);}}
                    DrawText("Continue",cx2-30,sh-50,18,acc2);if(mp2.x>=cx2-30&&mp2.y>=sh-50&&mp2.y<=sh-30&&IsMouseButtonPressed(MOUSE_BUTTON_LEFT))g_ai_step=g_ai->use_local?3:2; break;
                case 2: DrawText("Cloud AI API Keys",cx2-MeasureText("Cloud AI API Keys",24)/2,60,24,acc2);
                    for(int i=0;i<g_ai->n_cloud&&i<6;i++){int py=120+i*44;DrawText(g_ai->cloud_providers[i].display_name,cx2-180,py+4,14,WHITE);
                        DrawRectangle(cx2+80,py,160,22,(Color){40,44,56,255});DrawText(g_ai->cloud_providers[i].api_key[0]?"***":"Key...",cx2+86,py+4,12,g_ai->cloud_providers[i].api_key[0]?WHITE:GRAY);}
                    DrawText("Continue",cx2-30,sh-50,18,acc2);if(mp2.x>=cx2-30&&mp2.y>=sh-50&&mp2.y<=sh-30&&IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){cloud_ai_save(g_ai->cloud_providers,g_ai->n_cloud);g_ai_step=3;} break;
                case 3: DrawText("AI Ready!",cx2-60,cy2-20,24,gn2);DrawText("Continue",cx2-30,sh-50,18,acc2);
                    if(mp2.x>=cx2-30&&mp2.y>=sh-50&&mp2.y<=sh-30&&IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){ai_setup_free(g_ai);g_ai=NULL;inst->step=INSTALL_CLOUD_SAVE;} break;
            }
            break;
        }

        case INSTALL_CLOUD_SAVE: {
            static int cs_backend = 0;
            DrawText("Cloud Save Setup", cx-MeasureText("Cloud Save Setup",24)/2, 50, 24, accent);
            DrawText("Choose where to save your projects:", cx-200, 100, 15, text);

            const char *backs[] = {"Local Only (offline)","Our Server (Volt360.com)","Microsoft OneDrive","Google Drive","Custom Server (WebDAV/HTTP)"};
            for (int i = 0; i < 5; i++) {
                int py = 140 + i * 48;
                DrawRectangle(cx-280, py, 560, 40, panel);
                DrawRectangleLines(cx-280, py, 560, 40, cs_backend==i?accent:(Color){60,64,76,255});
                DrawText(cs_backend==i?"> ":"  ", cx-268, py+10, 15, cs_backend==i?WHITE:text);
                DrawText(backs[i], cx-240, py+10, 15, cs_backend==i?WHITE:text);
                Vector2 mp = GetMousePosition();
                if (mp.x>=cx-280&&mp.x<=cx+280&&mp.y>=py&&mp.y<=py+40&&IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) cs_backend=i;
            }

            // server registration fields
            if (cs_backend == 1) {
                static char reg_email[64]="", reg_pass[64]="";
                DrawText("Email:", cx-280, 400, 14, text); DrawRectangle(cx-220, 396, 400, 24, panel); DrawText(reg_email[0]?reg_email:"register@email.com", cx-214, 400, 13, reg_email[0]?WHITE:GRAY);
                DrawText("Password:", cx-280, 430, 14, text); DrawRectangle(cx-220, 426, 400, 24, panel); DrawText(reg_pass[0]?"********":"password", cx-214, 430, 13, reg_pass[0]?WHITE:GRAY);
            }

            DrawText("Continue ...", cx-40, sh-50, 18, accent);
            Vector2 mp2 = GetMousePosition();
            if (mp2.x>=cx-40&&mp2.x<=cx+60&&mp2.y>=sh-50&&mp2.y<=sh-30&&IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                inst->step = INSTALL_LIBS;
            DrawText("Skip", cx-160, sh-50, 16, GRAY);
            if (mp2.x>=cx-160&&mp2.x<=cx-120&&mp2.y>=sh-50&&mp2.y<=sh-30&&IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) inst->step = INSTALL_LIBS;
            break;
        }

        case INSTALL_LIBS: {
            DrawText("Choose Libraries to Install", cx - MeasureText("Choose Libraries to Install", 22)/2, 30, 22, accent);
            DrawText("Select the libraries you want. Pre-installed items are marked with [*].", cx-370, 60, 14, GRAY);

            int py = 90;
            Vector2 mp = GetMousePosition();
            for (int i = 0; i < inst->n_libs; i++) {
                LibraryPackage *lp = &inst->libs[i];
                int bx = 60, bw = sw - 120;
                if (py > sh - 160) break;
                Color row_bg = (i%2==0) ? (Color){32,35,42,255} : (Color){28,30,36,255};
                DrawRectangle(bx, py, bw, 36, row_bg);

                // checkbox
                int cbx = bx + 8, cby = py + 8;
                DrawRectangle(cbx, cby, 20, 20, lp->selected ? accent : (Color){50,53,62,255});
                DrawRectangleLines(cbx, cby, 20, 20, text);
                if (lp->selected) DrawText("x", cbx+5, cby+1, 16, WHITE);

                if (mp.x >= cbx && mp.x <= cbx+20 && mp.y >= cby && mp.y <= cby+20 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                    lp->selected = !lp->selected;

                DrawText(lp->name, bx+36, py+4, 15, WHITE);
                DrawText(lp->description, bx+36, py+20, 12, GRAY);
                char sz[16]; snprintf(sz,sizeof(sz),"%dMB",lp->size_mb);
                DrawText(sz, bx+bw-60, py+10, 13, lp->installed ? GREEN : GRAY);
                if (lp->installed) DrawText("[*]", bx+bw-100, py+10, 13, GREEN);

                py += 40;
            }
            // Select All / None
            if (mp.x >= 60 && mp.x <= 180 && mp.y >= py && mp.y <= py+24 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                for (int i = 0; i < inst->n_libs; i++) inst->libs[i].selected = 1;
            if (mp.x >= 200 && mp.x <= 340 && mp.y >= py && mp.y <= py+24 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                for (int i = 0; i < inst->n_libs; i++) inst->libs[i].selected = 0;
            DrawText("Select All", 60, py, 14, mp.x>=60&&mp.x<=180&&mp.y>=py&&mp.y<=py+24?WHITE:GRAY);
            DrawText("Deselect All", 200, py, 14, mp.x>=200&&mp.x<=340&&mp.y>=py&&mp.y<=py+24?WHITE:GRAY);

            // Custom import checkbox
            int ciy = py + 30;
            DrawRectangle(60, ciy, 20, 20, inst->custom_import ? accent : (Color){50,53,62,255});
            DrawRectangleLines(60, ciy, 20, 20, text);
            if (inst->custom_import) DrawText("x", 65, ciy+1, 16, WHITE);
            if (mp.x >= 60 && mp.x <= 80 && mp.y >= ciy && mp.y <= ciy+20 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                inst->custom_import = !inst->custom_import;
            DrawText("Import custom libraries from folder:", 88, ciy+2, 14, WHITE);
            if (inst->custom_import) {
                DrawRectangle(88, ciy+22, 400, 24, (Color){40,44,56,255});
                DrawText(inst->custom_path[0] ? inst->custom_path : "Click to browse...", 94, ciy+26, 13, inst->custom_path[0]?WHITE:GRAY);
                if (mp.x >= 88 && mp.x <= 488 && mp.y >= ciy+22 && mp.y <= ciy+46 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                    inst->step = INSTALL_IMPORT;
            }

            // Install button
            int bx = cx - 80, by = sh - 80;
            int hov2 = mp.x >= bx && mp.x <= bx+160 && mp.y >= by && mp.y <= by+40;
            DrawRectangle(bx, by, 160, 40, hov2 ? (Color){70,90,130,255} : accent);
            DrawText("Install Libraries", bx+10, by+10, 18, WHITE);
            if (hov2 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { inst->step = INSTALL_PROGRESS; inst->progress_pct = 0; }

            DrawText("Skip", 20, sh-30, 16, GRAY);
            if (mp.x >= 20 && mp.x <= 80 && mp.y >= sh-35 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) inst->step = INSTALL_DONE;
            break;
        }

        case INSTALL_IMPORT: {
            DrawText("Custom Library Import", cx-120, 60, 22, accent);
            DrawText("Enter the path to your custom library folder:", cx-200, 120, 15, text);
            DrawRectangle(cx-250, 160, 500, 36, (Color){40,44,56,255});
            DrawRectangleLines(cx-250, 160, 500, 36, accent);
            DrawText(inst->custom_path[0] ? inst->custom_path : "C:\\Users\\...\\my-libraries", cx-240, 168, 16, inst->custom_path[0]?WHITE:GRAY);

            int key = GetKeyPressed();
            if (key >= 32 && key <= 126) { int l=(int)strlen(inst->custom_path); if(l<510){inst->custom_path[l]=(char)key;inst->custom_path[l+1]=0;} }
            if (key == 259 && inst->custom_path[0]) inst->custom_path[strlen(inst->custom_path)-1]=0;

            Vector2 mp = GetMousePosition();
            DrawText("OK", cx-40, 220, 16, accent);
            int hov = mp.x >= cx-40 && mp.x <= cx+10 && mp.y >= 220 && mp.y <= 256;
            if (hov && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (inst->custom_path[0]) {
                    kicad_import_fp_lib(NULL, inst->custom_path);
                    kicad_link_3d_models(NULL, inst->custom_path);
                }
                inst->step = INSTALL_LIBS;
            }
            DrawText("Cancel", cx-160, 220, 16, GRAY);
            if (mp.x >= cx-160 && mp.x <= cx-110 && mp.y >= 220 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) inst->step = INSTALL_LIBS;
            break;
        }

        case INSTALL_PROGRESS: {
            inst->progress_pct += GetFrameTime() * 25.0f;
            if (inst->progress_pct > 100.0f) { inst->progress_pct = 100.0f; inst->step = INSTALL_DONE; }

            DrawText("Installing...", cx-60, cy-40, 22, accent);
            // progress bar
            DrawRectangle(cx-200, cy, 400, 24, (Color){50,53,62,255});
            DrawRectangle(cx-200, cy, (int)(400*inst->progress_pct/100), 24, accent);
            char pct[16]; snprintf(pct,sizeof(pct),"%.0f%%",inst->progress_pct);
            DrawText(pct, cx-15, cy+3, 16, WHITE);

            DrawText("Extracting libraries...", cx-80, cy+40, 14, GRAY);
            if (inst->progress_pct > 30) DrawText("Importing symbols...", cx-80, cy+58, 14, GRAY);
            if (inst->progress_pct > 60) DrawText("Linking 3D models...", cx-80, cy+76, 14, GRAY);
            break;
        }

        case INSTALL_DONE: {
            DrawText("Installation Complete!", cx - MeasureText("Installation Complete!", 28)/2, cy-40, 28, GREEN);
            DrawText("Your libraries are ready. Start creating!", cx - MeasureText("Your libraries are ready. Start creating!", 16)/2, cy, 16, text);

            // Launch button
            Vector2 mp = GetMousePosition();
            int bx = cx - 100, by = cy + 50;
            int hov = mp.x >= bx && mp.x <= bx+200 && mp.y >= by && mp.y <= by+44;
            DrawRectangle(bx, by, 200, 44, hov ? (Color){70,90,130,255} : accent);
            DrawText("Launch Application", bx+15, by+12, 18, WHITE);
            if (hov && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { inst->step = INSTALL_FINISHED; installer_free(inst); account_save(); }
            break;
        }

        default: break;
    }
    return inst->step;
}

InstallStep installer_handle_input(Installer *inst) { return inst ? inst->step : INSTALL_FINISHED; }

void installer_run(Installer *inst, Project *p) {
    if (!inst || !p) return;
    for (int i = 0; i < inst->n_libs; i++) {
        if (!inst->libs[i].selected) continue;
        if (strstr(inst->libs[i].name, "KiCad")) {
            // Import KiCad libraries
            if (strstr(inst->libs[i].name, "Symbols")) kicad_import_sym_lib(p, "libraries/kicad-symbols");
            if (strstr(inst->libs[i].name, "Footprints")) kicad_import_fp_lib(p, "libraries/kicad-footprints");
            if (strstr(inst->libs[i].name, "3D")) kicad_link_3d_models(p, "libraries/kicad-packages3D");
            if (strstr(inst->libs[i].name, "Template")) template_apply(p, TMPL_ARDUINO_UNO);
        }
    }
    if (inst->custom_import && inst->custom_path[0]) {
        kicad_import_fp_lib(p, inst->custom_path);
        kicad_link_3d_models(p, inst->custom_path);
    }
    fusion_library_save("C:\\Users\\realc\\AppData\\Roaming\\Autodesk\\CAM360\\libraries\\Local\\Library.json");
}
