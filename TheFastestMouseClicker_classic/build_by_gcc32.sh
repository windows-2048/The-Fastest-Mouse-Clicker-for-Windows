#!/bin/sh

rm -f TheFastestMouseClickerClassic.exe
/mingw32/bin/windres --input manifest.rc --output manifest.res --output-format=coff
/mingw32/bin/gcc -static -O0 -Wl,--subsystem,windows main-1993.cpp manifest.res -lstdc++ -lgdi32 -o TheFastestMouseClickerClassic.exe
/mingw32/bin/strip -s TheFastestMouseClickerClassic.exe
