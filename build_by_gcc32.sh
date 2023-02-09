# Copyright (c) 2016-2023 by [Open Source Developer Masha Novedad](https://windows-2048.github.io)

rm -f TheFastestMouseClicker.exe
gcc -static -O2 -Wl,--subsystem,windows main.cpp manifest.res -lstdc++ -lgdi32 -o TheFastestMouseClicker.exe
strip -s TheFastestMouseClicker.exe
