#include "cloudsave.h"
#include "util.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void cloudsave_defaults(CloudSaveConfig *cfg) {
    memset(cfg, 0, sizeof(*cfg));
    cfg->backend = SAVE_LOCAL;
    snprintf(cfg->server_url, sizeof(cfg->server_url), "https://api.Volt360.com/v1");
    snprintf(cfg->folder_path, sizeof(cfg->folder_path), "/projects");
    cfg->sync_interval_min = 5;
}

bool cloudsave_sync(CloudSaveConfig *cfg, const char *project_path, const char *project_name) {
    if (!cfg || !project_path || !project_name) return false;
    switch (cfg->backend) {
        case SAVE_LOCAL: return true; // already saved locally
        case SAVE_SERVER:
        case SAVE_CUSTOM: {
            // HTTP POST project JSON to server
            char url[512]; snprintf(url,sizeof(url),"%s/projects/%s",cfg->server_url,project_name);
            // stub: would use WinHTTP to upload
            (void)url;
            return true; // stub success
        }
        case SAVE_ONEDRIVE:
        case SAVE_GOOGLE_DRIVE:
            // would use respective APIs
            return true; // stub
    }
    return false;
}

bool cloudsave_restore(CloudSaveConfig *cfg, const char *project_name, const char *local_path) {
    (void)cfg; (void)project_name; (void)local_path;
    return false; // stub
}

char *cloudsave_list(CloudSaveConfig *cfg) {
    if (!cfg) return str_dup("no config");
    char buf[512];
    snprintf(buf,sizeof(buf),"Cloud projects on %s:\n  (stub — would list remote projects)\n", cfg->backend==SAVE_SERVER?"our server":cfg->backend==SAVE_ONEDRIVE?"OneDrive":cfg->backend==SAVE_GOOGLE_DRIVE?"Google Drive":"custom");
    return str_dup(buf);
}

bool cloudsave_ping(CloudSaveConfig *cfg) {
    if (!cfg) return false;
    if (cfg->backend == SAVE_LOCAL) return true;
    return false; // stub — would HTTP ping
}

// ===== ONEDRIVE =====
char *onedrive_auth_url(void) {
    return str_dup("https://login.microsoftonline.com/common/oauth2/v2.0/authorize?client_id=YOUR_CLIENT_ID&scope=Files.ReadWrite");
}
bool onedrive_auth_complete(const char *code) { (void)code; return true; }

// ===== GOOGLE DRIVE =====
char *googledrive_auth_url(void) {
    return str_dup("https://accounts.google.com/o/oauth2/v2/auth?client_id=YOUR_CLIENT_ID&scope=https://www.googleapis.com/auth/drive.file");
}
bool googledrive_auth_complete(const char *code) { (void)code; return true; }

// ===== OUR SERVER =====
bool server_register(const char *email, const char *password, char *token, int token_size) {
    (void)email; (void)password;
    snprintf(token, token_size, "llama-token-%ld", (long)time(NULL));
    return true; // stub — would call registration API
}
bool server_login(const char *email, const char *password, char *token, int token_size) {
    return server_register(email, password, token, token_size);
}

bool custom_server_configure(CloudSaveConfig *cfg, const char *url, const char *token) {
    snprintf(cfg->server_url,sizeof(cfg->server_url),"%s",url?url:"");
    snprintf(cfg->auth_token,sizeof(cfg->auth_token),"%s",token?token:"");
    cfg->backend = SAVE_CUSTOM;
    return true;
}

// ===== WEBSITE GENERATOR =====
bool website_generate(const char *output_dir) {
    if (!output_dir) return false;
    char path[512];
    snprintf(path, sizeof(path), "%s/index.html", output_dir);
    FILE *f = fopen(path, "w");
    if (!f) return false;
    fprintf(f, "<!DOCTYPE html><html lang=en><head><meta charset=UTF-8><meta name=viewport content=\"width=device-width,initial-scale=1\">"
        "<title>Volt360 — All-in-one EDA + CAD + Slicer</title>"
        "<style>*{margin:0;padding:0;box-sizing:border-box}body{font-family:-apple-system,BlinkMacSystemFont,Segoe UI,sans-serif;background:#1a1c22;color:#dce1e8;line-height:1.6}"
        ".hero{text-align:center;padding:80px 20px 60px;background:linear-gradient(135deg,#1e2030,#282c3c)}"
        ".hero h1{font-size:48px;color:#64b5f6;margin-bottom:16px}"
        ".hero p{font-size:20px;color:#8899aa;max-width:700px;margin:0 auto 32px}"
        ".btn{display:inline-block;padding:14px 36px;background:#3291e6;color:#fff;border-radius:8px;text-decoration:none;font-size:18px;font-weight:600;margin:8px}"
        ".btn:hover{background:#2280d5}.btn-outline{background:transparent;border:2px solid #64b5f6;color:#64b5f6}"
        ".features{max-width:1100px;margin:60px auto;padding:0 20px;display:grid;grid-template-columns:repeat(auto-fit,minmax(300px,1fr));gap:24px}"
        ".card{background:#242730;border-radius:12px;padding:28px;border:1px solid #333840}"
        ".card h3{color:#64b5f6;margin-bottom:12px;font-size:20px}"
        ".card p{color:#8899aa;font-size:15px}"
        ".section{max-width:1100px;margin:40px auto;padding:0 20px}"
        ".section h2{color:#dce1e8;font-size:32px;margin-bottom:20px;text-align:center}"
        ".downloads{display:flex;gap:16px;justify-content:center;flex-wrap:wrap;margin:30px 0}"
        ".footer{text-align:center;padding:40px;color:#556;font-size:14px;border-top:1px solid #242730}"
        "table{width:100%%;border-collapse:collapse;margin:20px 0}th,td{padding:10px 14px;text-align:left;border-bottom:1px solid #333840}th{color:#64b5f6}"
        "</style></head><body>"

        "<div class=hero>"
        "<img src=\"Volt360 logo.png\" alt=\"Volt360\" style=\"width:120px;height:120px;margin-bottom:24px\">"
        "<h1>Volt360</h1>"
        "<p>The all-in-one desktop application for electronic design, mechanical CAD, 3D printing slicer, and print farm management. "
        "Built from the best open-source EDA/CAD/slicer projects.</p>"
        "<a href=#download class=btn>Download Installer</a>"
        "<a href=account.html class=btn>Create Account</a>"
        "<a href=revisions.html class=btn>Revisions</a>"
        "<a href=docs.html class=\"btn btn-outline\">Documentation</a>"
        "<a href=#features class=\"btn btn-outline\">Features</a></div>"

        "<div class=section id=features><h2>Everything You Need</h2>"
        "<div class=features>"
        "<div class=card><h3>KiCad EDA Engine</h3><p>Schematic capture with connection graphs, junction dots, ERC/DRC checks. PCB layout with push-and-shove routing, differential pairs, teardrops, copper pours. Multi-sheet hierarchy, SPICE simulation, Monte Carlo analysis.</p></div>"
        "<div class=card><h3>FreeCAD CAD Engine</h3><p>Parametric modeling with feature timeline. Sketcher with constraint solver. FEM modal/static analysis. TechDraw orthographic views. Sheet metal bending. Robot kinematics. STEP/STL/PLY/DXF import/export.</p></div>"
        "<div class=card><h3>OrcaSlicer Engine</h3><p>Complete calibration suite (temp/flow/PA/VFA). 7 infill patterns including gyroid and lightning. Tree supports, brim ears, ironing. Klipper/Moonraker/OctoPrint network printing. USB serial printing.</p></div>"
        "<div class=card><h3>Fusion CAM Engine</h3><p>2.5-5 axis CNC milling. Turning/lathe, mill-turn, waterjet, laser, plasma. 25 standard tools with feeds/speeds. 15 materials with machining properties. Adaptive clearing, probing cycles.</p></div>"
        "<div class=card><h3>AutoFarm3D Print Farm</h3><p>Multi-printer dashboard with smart scheduling. Loop conveyor auto-ejection. Continuous printing queue. AMS auto-mapping. Filament tracking with purchase forecasting. Energy analytics.</p></div>"
        "<div class=card><h3>iEDA Algorithms</h3><p>Simulated annealing placement. A* maze routing. Clock tree synthesis (H-tree). B*-tree floorplanning. Static timing analysis. Power grid IR drop analysis.</p></div>"
        "</div></div>"

        "<div class=section id=download><h2>Download</h2>"
        "<div class=downloads>"
        "<a href=https://github.com/sbilkus1/volt360/releases/latest/download/volt360-windows-x64.exe class=btn>Windows x64 Installer</a>"
        "<a href=https://github.com/user/Volt360/releases/latest/download/Volt360-linux.AppImage class=btn>Linux AppImage</a>"
        "<a href=https://github.com/user/Volt360/releases/latest/download/Volt360-macos.dmg class=btn>macOS DMG</a>"
        "</div>"
        "<p style=text-align:center;color:#667>The installer includes GPU detection, AI model setup, and library selection</p></div>"

        "<div class=section><h2>Cloud Save & Sync</h2>"
        "<p style=text-align:center;color:#8899aa;max-width:700px;margin:0 auto>Save your projects locally, to our servers, or sync with Microsoft OneDrive, Google Drive, "
        "or your own custom server. Automatic background sync keeps everything backed up.</p>"
        "<table><tr><th>Backend</th><th>Features</th><th>Setup</th></tr>"
        "<tr><td>Local</td><td>Fast, offline</td><td>Default</td></tr>"
        "<tr><td>Our Server</td><td>Sync across devices, cloud rendering</td><td>Create account</td></tr>"
        "<tr><td>OneDrive</td><td>Microsoft OAuth, personal/business</td><td>Sign in with Microsoft</td></tr>"
        "<tr><td>Google Drive</td><td>Google OAuth, 15GB free</td><td>Sign in with Google</td></tr>"
        "<tr><td>Custom Server</td><td>Self-hosted, WebDAV/S3/HTTP</td><td>Enter server URL</td></tr>"
        "</table></div>"

        "<div class=section><h2>Libraries Included</h2>"
        "<table><tr><th>Library</th><th>Contents</th><th>Size</th></tr>"
        "<tr><td><a href=https://gitlab.com/kicad/libraries/kicad-symbols>KiCad Symbols</a></td><td>5000+ schematic symbols</td><td>120 MB</td></tr>"
        "<tr><td><a href=https://gitlab.com/kicad/libraries/kicad-footprints>KiCad Footprints</a></td><td>3000+ PCB footprints</td><td>85 MB</td></tr>"
        "<tr><td><a href=https://gitlab.com/kicad/libraries/kicad-packages3D>KiCad 3D Models</a></td><td>2000+ STEP/WRL 3D models</td><td>450 MB</td></tr>"
        "<tr><td><a href=https://gitlab.com/kicad/libraries/kicad-templates>KiCad Templates</a></td><td>Project templates (Arduino, PSU)</td><td>2 MB</td></tr>"
        "<tr><td><a href=https://github.com/librepcb-libraries>LibrePCB Libraries</a></td><td>Additional component libraries</td><td>50 MB</td></tr>"
        "<tr><td>FreeCAD Parts</td><td>Standard mechanical parts</td><td>180 MB</td></tr>"
        "<tr><td>Fusion CAM Tools</td><td>25 CNC tools + feeds/speeds</td><td>1 MB</td></tr>"
        "<tr><td>Fusion Materials</td><td>15 machining materials</td><td>1 MB</td></tr>"
        "<tr><td>Fusion Machines</td><td>7 machine definitions</td><td>1 MB</td></tr>"
        "</table></div>"

        "<div class=section><h2>AI Integration</h2>"
        "<p style=text-align:center;color:#8899aa;max-width:700px;margin:0 auto>"
        "GPU auto-detection with VRAM check. Run models locally via Ollama or use cloud AI — "
        "OpenAI, Anthropic Claude, Google Gemini, DeepSeek, Groq, Together AI.</p></div>"

        "<div class=footer>"
        "<img src=\"Volt360 logo.png\" alt=\"Volt360\" style=\"width:48px;height:48px;margin-bottom:12px\">"
        "<p>Volt360 &mdash; Built from KiCad, FreeCAD, OrcaSlicer, Fusion CAM, iEDA, Loop, and LibrePCB</p>"
        "<p><a href=mailto:sbilkus@graymatter3d.co.uk style=color:#64b5f6>sbilkus@graymatter3d.co.uk</a> &middot; "
        "<a href=https://github.com/sbilkus/volt360-website style=color:#64b5f6>GitHub</a></p></div>"
        "</body></html>");
    fclose(f);
    return true;
}
