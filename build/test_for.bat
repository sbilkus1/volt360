@echo off
setlocal enabledelayedexpansion
set ROOT=C:\Users\realc\Documents\Default Project
set SRCDIR=%ROOT%\src
echo SRCDIR=[%SRCDIR%]
for /r "%SRCDIR%" %%f in (*.c) do (
  echo FOUND: %%f  BASENAME: %%~nf
)
echo done
