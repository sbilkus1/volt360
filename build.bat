@echo off
setlocal enabledelayedexpansion

set ROOT=%~dp0
if "%ROOT:~-1%"=="\" set ROOT=%ROOT:~0,-1%
set BUILD=%ROOT%\build
set RL=%ROOT%\third_party\raylib\src
set RLINC1=%RL%
set RLINC2=%RL%\external\glfw\include
set SRCDIR=%ROOT%\src
set MINIZDIR=%ROOT%\third_party\miniz
set APPOUT=%BUILD%\app.exe
set TESTOUT=%BUILD%\test_import.exe
set TEST414OUT=%BUILD%\test_414.exe
set MODE=app
if /i "%~1"=="test" set MODE=test
if /i "%~1"=="test414" set MODE=test414
if /i "%~1"=="alltests" set MODE=alltests
if /i "%~1"=="vita" set MODE=vita

if not exist "%BUILD%" mkdir "%BUILD%"

call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvarsall.bat" x64 >nul
if errorlevel 1 ( echo [build] VCVARSALL FAILED & exit /b 1 )

set CFLAGS=/nologo /O2 /W3 /wd4996 /wd4100 /wd4189 /utf-8 /DPLATFORM_DESKTOP /DGRAPHICS_API_OPENGL_33 /D_CRT_SECURE_NO_WARNINGS
set LIBS=user32.lib gdi32.lib shell32.lib winmm.lib opengl32.lib ole32.lib comdlg32.lib uuid.lib advapi32.lib winhttp.lib

REM ---------- raylib ----------
if not exist "%BUILD%\raylib.lib" (
  echo [build] compiling raylib...
  pushd "%BUILD%"
  cl %CFLAGS% /I"%RLINC1%" /I"%RLINC2%" /c "%RL%\rcore.c" "%RL%\rshapes.c" "%RL%\rtextures.c" "%RL%\rtext.c" "%RL%\rmodels.c" "%RL%\raudio.c" "%RL%\utils.c" "%RL%\rglfw.c" || exit /b 1
  lib /nologo /out:raylib.lib rcore.obj rshapes.obj rtextures.obj rtext.obj rmodels.obj raudio.obj utils.obj rglfw.obj || exit /b 1
  popd
  echo [build] raylib OK
)

REM ---------- core library (all of src except main.c) ----------
echo [build] compiling core...
pushd "%BUILD%"
set CORE=
for /r "%SRCDIR%" %%f in (*.c) do (
  if /i not "%%~nf"=="main" if /i not "%%~nf"=="test_import" (
    cl %CFLAGS% /I"%RLINC1%" /I"%RLINC2%" /I"%MINIZDIR%" /I"%ROOT%" /I"%SRCDIR%" /c "%%f" /Fo"%%~nf.obj" || exit /b 1
    set CORE=!CORE! "%%~nf.obj"
  )
)
cl %CFLAGS% /I"%MINIZDIR%" /c "%MINIZDIR%\miniz.c" /Fominiz.obj || exit /b 1
cl %CFLAGS% /I"%MINIZDIR%" /c "%MINIZDIR%\miniz_tdef.c" /Fominiz_tdef.obj || exit /b 1
cl %CFLAGS% /I"%MINIZDIR%" /c "%MINIZDIR%\miniz_tinfl.c" /Fominiz_tinfl.obj || exit /b 1
cl %CFLAGS% /I"%MINIZDIR%" /c "%MINIZDIR%\miniz_zip.c" /Fominiz_zip.obj || exit /b 1
set CORE=!CORE! "miniz.obj" "miniz_tdef.obj" "miniz_tinfl.obj" "miniz_zip.obj"

if /i "%MODE%"=="test" (
  echo [build] compiling test_import...
  cl %CFLAGS% /I"%RLINC1%" /I"%RLINC2%" /I"%MINIZDIR%" /I"%ROOT%" /I"%SRCDIR%" /c "%ROOT%\tests\test_import.c" /Fotest_import.obj || exit /b 1
  link /nologo /OUT:"%TESTOUT%" test_import.obj %CORE% raylib.lib %LIBS% || exit /b 1
  popd
  echo [build] BUILD OK: %TESTOUT%
  exit /b 0
)

if /i "%MODE%"=="test414" (
  echo [build] compiling test_414...
  cl %CFLAGS% /GS- /I"%RLINC1%" /I"%RLINC2%" /I"%MINIZDIR%" /I"%ROOT%" /I"%SRCDIR%" /c "%ROOT%\tests\test_414.c" /Fotest_414.obj || exit /b 1
  link /nologo /STACK:8388608 /OUT:"%TEST414OUT%" test_414.obj %CORE% raylib.lib %LIBS% || exit /b 1
  popd
  echo [build] BUILD OK: %TEST414OUT%
  exit /b 0
)

if /i "%MODE%"=="alltests" (
  echo [build] compiling test_import...
  cl %CFLAGS% /I"%RLINC1%" /I"%RLINC2%" /I"%MINIZDIR%" /I"%ROOT%" /I"%SRCDIR%" /c "%ROOT%\tests\test_import.c" /Fotest_import.obj || exit /b 1
  link /nologo /OUT:"%TESTOUT%" test_import.obj %CORE% raylib.lib %LIBS% || exit /b 1
  echo [build] compiling test_414...
  cl %CFLAGS% /GS- /I"%RLINC1%" /I"%RLINC2%" /I"%MINIZDIR%" /I"%ROOT%" /I"%SRCDIR%" /c "%ROOT%\tests\test_414.c" /Fotest_414.obj || exit /b 1
  link /nologo /STACK:8388608 /OUT:"%TEST414OUT%" test_414.obj %CORE% raylib.lib %LIBS% || exit /b 1
  popd
  echo [build] BUILD OK: %TESTOUT% + %TEST414OUT%
  exit /b 0
)

if /i "%MODE%"=="vita" (
  echo [build] compiling vitachrona test...
  cl %CFLAGS% /GS- /I"%RLINC1%" /I"%RLINC2%" /I"%MINIZDIR%" /I"%ROOT%" /I"%SRCDIR%" /c "%ROOT%\tests\test_vitachrona.c" /Fotest_vitachrona.obj || exit /b 1
  link /nologo /STACK:8388608 /OUT:"%BUILD%\test_vitachrona.exe" test_vitachrona.obj %CORE% raylib.lib %LIBS% || exit /b 1
  popd
  echo [build] BUILD OK: %BUILD%\test_vitachrona.exe
  exit /b 0
)

REM ---------- app ----------
echo [build] compiling app...
cl %CFLAGS% /I"%RLINC1%" /I"%RLINC2%" /I"%MINIZDIR%" /I"%ROOT%" /I"%SRCDIR%" /c "%SRCDIR%\main.c" /Fomain.obj || exit /b 1
link /nologo /OUT:"%APPOUT%" main.obj %CORE% raylib.lib %LIBS% || exit /b 1
popd
echo [build] BUILD OK: %APPOUT%
