@echo off
set /p var= <%1
set /a var= %var%+1
echo %var% >%1
echo #define PROJECT_BUILDNO %var% >%1.h
