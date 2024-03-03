@echo off
SETLOCAL

REM *************SETUP
REM SET MSYS_HOME=
REM *************SETUP END

SET PATH=%PATH%;%MSYS_HOME%\usr\bin

flex --nounistd beng.l
bison.exe -d -v -Wcounterexamples --output=beng.tab.cpp beng.y

ENDLOCAL
