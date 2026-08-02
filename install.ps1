# Volt360 Installer Script
# Run as Administrator: powershell -ExecutionPolicy Bypass -File install.ps1
param([switch]$Silent, [string]$Path = "$env:ProgramFiles\Volt360")

$ErrorActionPreference = "Stop"
$INSTALL_DIR = $Path
$DESKTOP = [Environment]::GetFolderPath("Desktop")
$START_MENU = [Environment]::GetFolderPath("StartMenu")
$VERSION = "2.0.0"

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  Volt360 v$VERSION Installer" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# Create directories
New-Item -ItemType Directory -Force -Path $INSTALL_DIR | Out-Null
New-Item -ItemType Directory -Force -Path "$INSTALL_DIR\lib" | Out-Null
New-Item -ItemType Directory -Force -Path "$INSTALL_DIR\examples" | Out-Null

# Copy files
Write-Host "[*] Installing to $INSTALL_DIR..." -ForegroundColor Green
Copy-Item "app.exe" "$INSTALL_DIR\" -Force
Copy-Item "raylib.lib" "$INSTALL_DIR\" -Force 2>$null
Copy-Item "..\examples\*" "$INSTALL_DIR\examples\" -Force -ErrorAction SilentlyContinue
Copy-Item "..\website\*" "$INSTALL_DIR\lib\" -Force -ErrorAction SilentlyContinue

# Create shortcut
Write-Host "[*] Creating shortcuts..." -ForegroundColor Green
$WScriptShell = New-Object -ComObject WScript.Shell
$Shortcut = $WScriptShell.CreateShortcut("$DESKTOP\Volt360.lnk")
$Shortcut.TargetPath = "$INSTALL_DIR\app.exe"
$Shortcut.WorkingDirectory = $INSTALL_DIR
$Shortcut.Description = "Volt360 - All-in-one EDA + CAD + Slicer"
$Shortcut.Save()

# Start Menu shortcuts
New-Item -ItemType Directory -Force -Path "$START_MENU\Programs\Volt360" | Out-Null
$SM = $WScriptShell.CreateShortcut("$START_MENU\Programs\Volt360\Volt360.lnk")
$SM.TargetPath = "$INSTALL_DIR\app.exe"
$SM.WorkingDirectory = $INSTALL_DIR
$SM.Save()

$Uninstall = $WScriptShell.CreateShortcut("$START_MENU\Programs\Volt360\Uninstall.lnk")
$Uninstall.TargetPath = "powershell.exe"
$Uninstall.Arguments = "-ExecutionPolicy Bypass -File `"$INSTALL_DIR\uninstall.ps1`""
$Uninstall.Save()

# Create uninstaller
Write-Host "[*] Creating uninstaller..." -ForegroundColor Green
@'
param([switch]$Force)
Write-Host "Uninstalling Volt360..." -ForegroundColor Yellow
Remove-Item "$env:ProgramFiles\Volt360" -Recurse -Force
Remove-Item "$env:Desktop\Volt360.lnk" -Force -ErrorAction SilentlyContinue
Remove-Item "$env:APPDATA\Microsoft\Windows\Start Menu\Programs\Volt360" -Recurse -Force -ErrorAction SilentlyContinue
Write-Host "Volt360 uninstalled." -ForegroundColor Green
'@ | Out-File -FilePath "$INSTALL_DIR\uninstall.ps1" -Encoding ASCII

# Register with Add/Remove Programs
$regPath = "HKLM:\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Volt360"
New-Item -Path $regPath -Force | Out-Null
Set-ItemProperty -Path $regPath -Name "DisplayName" -Value "Volt360"
Set-ItemProperty -Path $regPath -Name "DisplayVersion" -Value $VERSION
Set-ItemProperty -Path $regPath -Name "Publisher" -Value "GrayMatter3D"
Set-ItemProperty -Path $regPath -Name "UninstallString" -Value "powershell -ExecutionPolicy Bypass -File `"$INSTALL_DIR\uninstall.ps1`""
Set-ItemProperty -Path $regPath -Name "InstallLocation" -Value $INSTALL_DIR
Set-ItemProperty -Path $regPath -Name "NoModify" -Value 1
Set-ItemProperty -Path $regPath -Name "NoRepair" -Value 1

Write-Host ""
Write-Host "========================================" -ForegroundColor Green
Write-Host "  Volt360 v$VERSION installed!" -ForegroundColor Green
Write-Host "  Location: $INSTALL_DIR" -ForegroundColor White
Write-Host "  Desktop shortcut created" -ForegroundColor White
Write-Host "========================================" -ForegroundColor Green

if (-not $Silent) { Read-Host "Press Enter to exit" }
