#!/bin/sh

rm -f TheFastestMouseClickerClassic64.exe
/mingw64/bin/windres --input manifest.rc --output manifest.res --output-format=coff
x86_64-w64-mingw32-gcc -mwindows -I../NewInstallerTemplateMSI/ -static -O0 -Wl,--subsystem,windows main-1993.cpp manifest.res -lstdc++ -lgdi32 -o TheFastestMouseClickerClassic64.exe
/mingw64/bin/strip -s TheFastestMouseClickerClassic64.exe
