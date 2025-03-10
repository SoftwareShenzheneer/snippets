@echo off

setlocal enabledelayedexpansion

set SRC_FILES=src
set INC_FILES=inc

set FILES=
for %%f in (%SRC_FILES%\*.c) do set FILES=!FILES! %%f

gcc -o app !FILES! -I%INC_FILES%

