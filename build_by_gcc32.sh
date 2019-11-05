rm -f TheFastestMouseClicker.exe
gcc -static -O2 -Wl,--subsystem,windows main.cpp manifest.res -lstdc++ -lgdi32 -o TheFastestMouseClicker.exe
strip -s TheFastestMouseClicker.exe
