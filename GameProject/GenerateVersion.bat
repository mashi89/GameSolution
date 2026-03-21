@echo off
setlocal
for /f %%i in ('git -C "%~dp0.." rev-list --count HEAD 2^>nul') do set BUILD=%%i
if not defined BUILD set BUILD=0
(
echo #pragma once
echo #define VERSION_BUILD %BUILD%
) > "%~dp0VersionGenerated.h"
endlocal
