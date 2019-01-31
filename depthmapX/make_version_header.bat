@echo off
del version_defs.h
echo // Copyright (C) 2018 Christian Sailer >> version_defs.h

echo // This program is free software: you can redistribute it and/or modify >> version_defs.h
echo // it under the terms of the GNU General Public License as published by >> version_defs.h
echo // the Free Software Foundation, either version 3 of the License, or >> version_defs.h
echo // (at your option) any later version. >> version_defs.h
echo.  >> version_defs.h
echo // This program is distributed in the hope that it will be useful, >> version_defs.h
echo // but WITHOUT ANY WARRANTY; without even the implied warranty of >> version_defs.h
echo // MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the >> version_defs.h
echo // GNU General Public License for more details. >> version_defs.h
echo. >> version_defs.h
echo // You should have received a copy of the GNU General Public License >> version_defs.h
echo|set /p="// along with this program.  If not, see <http://www.gnu.org/licenses/>." >> version_defs.h
echo. >> version_defs.h
echo. >> version_defs.h
echo // This file is autogenerated - do not modify it directly! >> version_defs.h
echo. >> version_defs.h
echo #pragma once >> version_defs.h
echo. >> version_defs.h

:: Get date in correct form (YYYY/MM/DD).
for /f "skip=1" %%x in ('wmic os get localdatetime') do if not defined MyDate set MyDate=%%x
for /f %%x in ('wmic path win32_localtime get /format:list ^| findstr "="') do set %%x
set fmonth=00%Month%
set fday=00%Day%
set today=%Year%/%fmonth:~-2%/%fday:~-2%

echo #ifndef APP_DATE >> version_defs.h
echo #define APP_DATE "%today%" >> version_defs.h
echo #endif >> version_defs.h
echo. >> version_defs.h
for /f "delims=" %%A in ('git rev-parse --abbrev-ref HEAD') do set "mybranch=%%A"
echo #ifndef APP_GIT_BRANCH >> version_defs.h
echo #define APP_GIT_BRANCH "%mybranch%" >> version_defs.h
echo #endif >> version_defs.h
echo. >> version_defs.h
for /f "delims=" %%A in ('git log "--pretty=format:%%h" -n 1') do set "mycommit=%%A"
echo #ifndef APP_GIT_COMMIT >> version_defs.h
echo #define APP_GIT_COMMIT "%mycommit%" >> version_defs.h
echo #endif >> version_defs.h