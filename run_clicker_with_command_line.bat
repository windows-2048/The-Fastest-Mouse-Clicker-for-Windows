rem In the command line below, the arguments signify as follows:
rem '-c 100' = clicks per second is 100
rem '-t 32' = trigger key code is 32 (corresponds to 'Space' key)
rem '-s 300' = stop after 300 clicks,
rem '-m t' = clicking mode is toggle: the program starts to emit mouse events when you press 'Space' for the every first time,
rem and finishes to emit evens when you press 'Space' for the every second time. Thus you can repeat the clicking while the app runs.
rem '-b r' = mouse button to click is right

TheFastestMouseClicker.exe -c 100 -t 32 -s 300 -m t -b r

pause
