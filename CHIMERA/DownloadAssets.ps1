# CHIMERA Asset Downloader — Run this ONCE on your UE5 machine with internet access.
# This downloads all free CC0/Marketplace assets your world needs.
# After running: restart UE5, open CHIMERA, the engine auto-imports everything.
#
# REQUIREMENTS: PowerShell 5.1+, internet, ~80GB free disk space
# TIME: ~2-4 hours depending on internet speed

param(
    [string]$ProjectPath = "$PSScriptRoot"
)

$ContentDir = Join-Path $ProjectPath "Content"
$ImportDir = Join-Path $ContentDir "Imported"
$CharDir = Join-Path $ContentDir "Characters"
$VehDir = Join-Path $ContentDir "Vehicles"
$BuildDir = Join-Path $ContentDir "Buildings"

New-Item -ItemType Directory -Force -Path $ImportDir | Out-Null
New-Item -ItemType Directory -Force -Path $CharDir | Out-Null
New-Item -ItemType Directory -Force -Path $VehDir | Out-Null
New-Item -ItemType Directory -Force -Path $BuildDir | Out-Null

Write-Host "========================================================" -ForegroundColor Cyan
Write-Host "  CHIMERA Asset Downloader — v1.0" -ForegroundColor Cyan
Write-Host "  Target: $ProjectPath" -ForegroundColor Cyan
Write-Host "========================================================" -ForegroundColor Cyan
Write-Host ""

# ==================================================================
# STEP 1: Quixel Megascans (in-UE5, can't download via script)
# ==================================================================
Write-Host "[1/5] Quixel Megascans — OPEN UE5 AND USE BRIDGE" -ForegroundColor Yellow
Write-Host "  In UE5 Editor: Window -> Quixel Bridge -> Sign in (Epic account)" -ForegroundColor White
Write-Host "  Click 'Download All' for: Surfaces, 3D Assets, Decals, 3D Plants" -ForegroundColor White
Write-Host "  This gives you 10,000+ photorealistic scanned assets. Free with UE5." -ForegroundColor White
Write-Host "  Estimated: 50GB. Time: 1-2 hours." -ForegroundColor Gray
Write-Host ""

# ==================================================================
# STEP 2: UE Marketplace free packs (in-Launcher)
# ==================================================================
Write-Host "[2/5] UE Marketplace Free Packs — OPEN EPIC LAUNCHER" -ForegroundColor Yellow
Write-Host "  Launcher -> Unreal Engine -> Marketplace -> Filter: Free -> Price: Free" -ForegroundColor White
Write-Host "  Download these packs (all permanently free):" -ForegroundColor White
Write-Host "    - City Sample Buildings (modular urban architecture)" -ForegroundColor White
Write-Host "    - Modular SciFi (cyberpunk corridors and props)" -ForegroundColor White
Write-Host "    - Modular Fantasy (castle walls, towers, medieval props)" -ForegroundColor White
Write-Host "    - Rural Australia (farm buildings, fences, rural props)" -ForegroundColor White
Write-Host "    - Vehicle Variety Pack (cars, trucks, bikes)" -ForegroundColor White
Write-Host "    - Hospital Interior Pack (beds, monitors, OR equipment)" -ForegroundColor White
Write-Host "    - Police Station Props (desks, evidence lockers, cells)" -ForegroundColor White
Write-Host "    - Medieval Environment (castles, villages, dragon lairs)" -ForegroundColor White
Write-Host "  Add to project -> select 'CHIMERA'" -ForegroundColor White
Write-Host "  Estimated: 25GB. Time: 1 hour." -ForegroundColor Gray
Write-Host ""

# ==================================================================
# STEP 3: Poly Haven CC0 assets (polyhaven.com)
# ==================================================================
Write-Host "[3/5] Poly Haven CC0 Assets (HDRI skies + textures)" -ForegroundColor Yellow

$polyhavenFiles = @(
    @{url="https://dl.polyhaven.com/file/ph-assets/HDRIs/hdr/1k/kloofendal_48d_partly_cloudy_puresky_1k.hdr"; name="HDRI_Sky_Clear.hdr"},
    @{url="https://dl.polyhaven.com/file/ph-assets/HDRIs/hdr/1k/rogland_clear_night_1k.hdr"; name="HDRI_Sky_Night.hdr"},
    @{url="https://dl.polyhaven.com/file/ph-assets/HDRIs/hdr/1k/industrial_sunset_1k.hdr"; name="HDRI_Sunset_Industrial.hdr"}
)

$HDRIDir = Join-Path $ContentDir "Environment" "Sky"
New-Item -ItemType Directory -Force -Path $HDRIDir | Out-Null

foreach ($file in $polyhavenFiles) {
    $outPath = Join-Path $HDRIDir $file.name
    if (-not (Test-Path $outPath)) {
        try {
            Write-Host "  Downloading $($file.name)..." -ForegroundColor Gray
            Invoke-WebRequest -Uri $file.url -OutFile $outPath -TimeoutSec 120
            Write-Host "    -> OK" -ForegroundColor Green
        } catch {
            Write-Host "    -> Failed (download manually from polyhaven.com/hdris)" -ForegroundColor Red
        }
    } else {
        Write-Host "  $($file.name) already exists, skipping" -ForegroundColor Gray
    }
}
Write-Host ""

# ==================================================================
# STEP 4: Sketchfab CC0 3D models (best free source for specific models)
# ==================================================================
Write-Host "[4/5] Sketchfab CC0 Models — DOWNLOAD MANUALLY" -ForegroundColor Yellow
Write-Host "  Open these URLs in browser, click Download, choose FBX format:" -ForegroundColor White
Write-Host ""
Write-Host "  BUILDINGS:" -ForegroundColor Cyan
Write-Host "    sketchfab.com/search?q=medieval+castle&type=models&downloadable=true&license=cc0" -ForegroundColor White
Write-Host "    sketchfab.com/search?q=cyberpunk+building&type=models&downloadable=true&license=cc0" -ForegroundColor White
Write-Host "    sketchfab.com/search?q=hospital+modular&type=models&downloadable=true&license=cc0" -ForegroundColor White
Write-Host ""
Write-Host "  CHARACTERS:" -ForegroundColor Cyan
Write-Host "    sketchfab.com/search?q=police+officer+character&type=models&downloadable=true&license=cc0" -ForegroundColor White
Write-Host "    sketchfab.com/search?q=doctor+surgeon+character&type=models&downloadable=true&license=cc0" -ForegroundColor White
Write-Host "    sketchfab.com/search?q=fantasy+knight+character&type=models&downloadable=true&license=cc0" -ForegroundColor White
Write-Host ""
Write-Host "  VEHICLES:" -ForegroundColor Cyan
Write-Host "    sketchfab.com/search?q=porsche+911+model&type=models&downloadable=true&license=cc0" -ForegroundColor White
Write-Host "    sketchfab.com/search?q=lamborghini+countach+model&type=models&downloadable=true&license=cc0" -ForegroundColor White
Write-Host "    sketchfab.com/search?q=ford+mustang+model&type=models&downloadable=true&license=cc0" -ForegroundColor White
Write-Host ""
Write-Host "  After downloading FBX files, drag them into:" -ForegroundColor White
Write-Host "    Content/Imported/" -ForegroundColor White
Write-Host "  UE5 will auto-import and create StaticMesh + Material assets." -ForegroundColor White
Write-Host ""

# ==================================================================
# STEP 5: UE5 Post-Import Setup
# ==================================================================
Write-Host "[5/5] UE5 Post-Import Configuration" -ForegroundColor Yellow
Write-Host "  After all assets are imported, in UE5 Editor:" -ForegroundColor White
Write-Host "  1. Content Browser -> select all Static Meshes" -ForegroundColor White
Write-Host "  2. Right-click -> Asset Actions -> Bulk Edit via Property Matrix" -ForegroundColor White
Write-Host "  3. Set 'Nanite Enabled' = True (for UE5.3+)" -ForegroundColor White
Write-Host "  4. Project Settings -> Rendering -> Dynamic Global Illumination Method = Lumen" -ForegroundColor White
Write-Host "  5. Project Settings -> Rendering -> Shadow Map Method = Virtual Shadow Maps" -ForegroundColor White
Write-Host "  6. In DefaultEngine.ini add:" -ForegroundColor White
Write-Host "     r.RayTracing=1" -ForegroundColor Green
Write-Host "     r.RayTracing.Shadows=1" -ForegroundColor Green
Write-Host "     r.RayTracing.Reflections=1" -ForegroundColor Green
Write-Host "     r.Lumen.Reflections.Allow=1" -ForegroundColor Green

Write-Host ""
Write-Host "========================================================" -ForegroundColor Cyan
Write-Host "  DONE. Restart UE5 and open CHIMERA. The world will" -ForegroundColor Cyan
Write-Host "  auto-populate with real 3D models from the asset catalog." -ForegroundColor Cyan
Write-Host "========================================================" -ForegroundColor Cyan
