rem In the command line below, the arguments signify as follows:
rem '-t 32' = trigger key code is 32 (corresponds to 'Space' key),
rem other parameters are default or auto-saved from the previous app run-time.
rem '-r 10 20 100 200' = bounding rectangle for random clicking,
rem originated at the left-top point with desktop coordinates (10, 20)
rem and lasts till right-bottom point (10 + 100, 20 + 200) = (110, 220).
rem Note. Currently it is up to you how to obtain that values,
rem it is recommended to make a screenshot (press the 'Print Screen/Sys rq' key)
rem and then to open it in any picture/photo editor to get screen coordinates
rem of a desired rectangle for random clicking.

TheFastestMouseClicker.exe -t 32 -r 10 20 100 200

pause
