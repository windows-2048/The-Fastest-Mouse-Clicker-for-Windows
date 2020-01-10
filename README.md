# The Fastest Mouse Clicker for Windows (formerly The Windows Intel Fastest Mouse Clicker)

The fastest hardware-limit mouse clicker for Windows with command line support and random clicking capabilities.

This is a very simple application that does just one task but does it
perfectly. The program emulates Windows mouse clicks once a triggered
key is pressed by end-user. The application performs this job extremely
quickly with theoretical speed limit reached. I have rewritten this
program inspired by a currently obsolete project "Fast Mouse Clicker" by
 xytor.

![The Fastest Mouse Clicker for Windows version 2.1.7.0 on Windows: what's new](https://github.com/windows-2048/The-Fastest-Mouse-Clicker-for-Windows/raw/master/screenshots_new/v2.1.7.0/TFMCfW_2_1_7_0w_s1_1322x986.png)

![The Fastest Mouse Clicker for Windows version 2.1.7.0 under Wine on Linux](https://github.com/windows-2048/The-Fastest-Mouse-Clicker-for-Windows/raw/master/screenshots_new/v2.1.7.0/TFMCfW_2_1_7_0w_s1w_1322x986.png)

![The Fastest Mouse Clicker for Windows version 2.1.7.0 on Windows: what's important](https://github.com/windows-2048/The-Fastest-Mouse-Clicker-for-Windows/raw/master/screenshots_new/v2.1.7.0/TFMCfW_2_1_7_0w_s2_1322x986.png)

My application utilizes the cutting-edge feature of the SendInput()
system call that aggregates some 100-1000 mouse events in one array and
fires this array to a computer hardware at once. Furthermore, my program
 calls Sleep() between SendInput() very precisely, based on the Windows
High Resolution Timer, to completely eliminate any idle moments that
could be spent otherwise to the mouse clicks.

* NEW Command line has been supported:
TheFastestMouseClicker.exe -c <clicks/s> -t <trigger key> -s <stop at> -m {p|t} -b {l|m|r},
where
'-m {p|t}' is the 'trigger key mode' {'press'|'toggle'},
'-b {l|m|r}' is the 'mouse button to click' {'left'|'middle'|'right'}.
One may specify any part of arguments; unspecified or unrecognized values will be treated
as defaults (see them by running the app without command line,
e.g. from Windows programs menu).

* NEW Auto-save all the clicking parameters has been added! This feature does not
interfere parameters from the command line.

* NEW Reset to default parameters button added.

* NEW Fractional values for clicks/s parameter are supported. It allows you to click once in two seconds for example.

* NEW Button "Batch folder" has been added to open a directory with *.bat files quickly; it simplifies command line usage a lot.

* NEW Random clicking has been implemented. Just click the "Batch folder" button and see remarks in the *.bat files reside there in order how to use command line arguments and to enable random clicking.

* NEW Different, INDEPENDENT trigger keys to begin/end the clicking in toggle mode.

# Download the latest Windows installer binaries

VERSION 2.1.7.0: THIS IS A GREAT BUG-FIX RELEASE. THE CLICKER HAS BEEN COMPILED BY GCC/MINGW32. UPDATE AS SOON AS POSSIBLE.
---------------------------------------------------------------------------------------------------------------------------

This is a second mirror, source-only. [The main site is here](https://sourceforge.net/projects/fast-mouse-clicker-pro/) and [first mirror is here](https://gitlab.com/mashanovedad/The-Fastest-Mouse-Clicker-for-Windows/). See them for Windows installer binaries to download.
