@echo off
SETLOCAL

REM *************SETUP
REM SET MSYS_HOME=
REM *************SETUP END

SET PATH=%PATH%;%MSYS_HOME%\usr\bin

flex --nounistd bang.l
bison.exe -d -v -Wcounterexamples --output=bang.tab.cpp bang.y

ENDLOCAL
