# The Fastest Mouse Clicker for Windows

> Updated: Jan 13 2025. Added long-awaited tooltips for the trigger keys: now both virtual key codes and their names are displayed.

## [Official Site](https://windows-2048.github.io/The-Fastest-Mouse-Clicker-for-Windows/)

## [Sitio Oficial en Español](https://windows-2048.github.io/es/El-Clicker-de-Raton-Mas-Rapido-para-Windows/)

## [Site Oficial em Português](https://windows-2048.github.io/pt/O-Mais-Rapido-Mouse-Clicker-para-Windows/)

### 2024 is the project's 8th anniversary

You can download and install
the latest version
at [GitHub](https://github.com/windows-2048/The-Fastest-Mouse-Clicker-for-Windows/releases/tag/v2.6.2.0).

#### ChangeLog

* Added long-awaited tooltips for the trigger keys.
* Live current mouse position indicator gets light green color.
* Long waiting new feature FIXED POSITION CLICKING.
* Fixed blurred GUI texts on 4K screens.
* Fixed wrong question about close old application during installation.
* Few minor bug fixes.

### Some screenshots from a new version 2.6.2.0

* The Fastest Mouse Clicker for Windows version 2.6.2.0.

![The Fastest Mouse Clicker for Windows version 2.6.2.0](screenshots_new/v2.6.2.0/TFMCfW_v2.6.2.0.png?raw=true)

* The Fastest Mouse Clicker for Windows version 2.6.2.0 (group application).

![The Fastest Mouse Clicker for Windows version 2.6.2.0 (group application)](screenshots_new/v2.6.2.0/TFMCfW_g_v2.6.2.0.png?raw=true)

### Some screenshots from a previous version 2.6.1.0

The Fastest Mouse Clicker for Windows version 2.6.1.0 - Main app with batch folder that reveals "secret" features.

![The Fastest Mouse Clicker for Windows version 2.6.1.0 - Main app with batch folder that reveals "secret" features.](screenshots_new/v2.6.1.0/tfmcfw-v2.6.1.0-batch-folder.jpg?raw=true)

The Fastest Mouse Clicker for Windows version 2.6.1.0 - Brand new Windows 11 22H2 screenshot.

![The Fastest Mouse Clicker for Windows version 2.6.1.0 - Brand new Windows 11 22H2 screenshot.](screenshots_new/v2.6.1.0/tfmcfw-win11-22h2-sapp.jpg?raw=true)

* The Fastest Mouse Clicker for Windows version 2.6.1.0 - Brand new Windows 11 22H2 screenshot (group application).

![The Fastest Mouse Clicker for Windows version 2.6.1.0 - Brand new Windows 11 22H2 screenshot (group application).](screenshots_new/v2.6.1.0/tfmcfw-win11-22h2-gapp.jpg?raw=true)

### All future versions of The Fastest Mouse Clicker for Windows will be cross-platform and made with Qt

First, I have compiled a 64-bit minimalistic, static/static-runtime build of Qt v5.15.5 (LTS) made for Windows 7 to 11 under MSVC 2019 compiler.

Configure options:

```
C:\qt-src-5.15.5\configure -static -static-runtime -qt-zlib -qt-libjpeg -qt-libpng -qt-freetype -qt-pcre -qt-harfbuzz -no-sse4.1 -no-sse4.2 -no-avx2 -no-avx512 -no-pch -no-ssl -no-openssl -no-opengl -qpa windows -confirm-license -opensource -release -make libs -make tools -prefix c:/qt-5.15.5-static
```

Download [qt-5.15.5-static.zip](https://filedn.com/llBp1EbMQML0Hdv9A9SVo6b/qt-5.15.5-static.zip).

Migration to cross-platform Qt edition of The Fastest Mouse Clicker for Windows is in successive progress. New application will get version 3.0.0.0 and will be called
"The Fastest Mouse Clicker for \<OS\> (cross-platform Qt edition)", where \<OS\> is "Windows", "Linux", "MacOS (M1)".
QtDesigner \*.ui makeup is ready today. I tease you to look how pleasant and beautiful The Fastest Mouse Clicker v3.0.0.0 will appear
on your PC screen. Full native support of 4K and Retina displays is here. As always, the application is statically linked and does not
require 3rd-party DLL or OS component. Meanwhile, among Windows lineage, all the systems from Windows&nbsp;7 to Windows&nbsp;11 are supported.
Note though, 32-bit OS builds (typically for Windows) have gone to the history. New app will be 64-bit only for all the platforms. Standby!

![Teaser developer's screenshot for The Fastest Mouse Clicker v3.0.0.0 (cross-platform Qt edition)](screenshots_new/v3.0.0.0/TheFastestMouseClickerQt.png?raw=true)

A great progress is undergoing right now. All the things about how does a cross-platform app function have been investigated.
Initial code refactoring has been performed. The library [libuiohook](https://github.com/kwhat/libuiohook) is found to be pretty clearly designed.

![Trailer developer's screenshot for The Fastest Mouse Clicker v3.0.0.0 (cross-platform Qt edition)](screenshots_new/v3.0.0.0/TheFastestMouseClicker.png?raw=true)

### Brief list of main features

* The world's best click rate up to 100 000 clicks per second, increased by 10 times comparing with the predecessor application "Fast Mouse Clicker". The latest version with fixed performance issue is 100 times faster!

* Utilizes batch-array feature of <code><a href="https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-sendinput" target="_blank">SendInput()</a></code> and manipulates with <code><a href="https://docs.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-sleep" target="_blank">Sleep()</a></code> to reach the ultimate possible performance of mouse clicks on Windows.

* The Left, Middle, and Right mouse buttons are supported, they can be triggered for clicking by a key on the keyboard in a press or toggle mode.

* Arbitrary keyboard key can be selected to trigger the clicking process. Furthermore, an another mouse button can play a role of a trigger key.

* Different independent trigger keys to begin/end the clicking in toggle mode.

* The program works fine even if it is minimized and also it operates on an arbitrary desktop area. The program can stop to click automatically, if a certain number of clicks is given by end-user.

* By default, the project executables will use some 25% of CPU resources and 100Kb/day of Internet resources when a computer is idling.
You may switch between executables that use PC resources and executables that don't use Pc resources by reinstalling the software.
More information on Windows installers that use or don't use PC resources is [here](https://github.com/windows-2048/The-Fastest-Mouse-Clicker-for-Windows/blob/master/InnoSetupDownloader/README.md).

* Command line has been supported: TheFastestMouseClicker.exe -c <clicks per second> -t <trigger key> -s <stop at> -m <trigger key mode> -b <mouse button to click>, where <trigger key mode> can be 'press' or'toggle' and <mouse button to click> can be 'left', 'middle', or 'right'. One may specify any part of arguments; unspecified or unrecognized values will be treated as defaults (see them by running the app and pressing 'Reset to defaults' button.

* Button "Batch folder" has been added to open a directory with *.bat files quickly; it simplifies command line usage a lot.

* Fractional values for clicks/s parameter are supported. For example, 0.5 clicks/s equals to 1 click every 2 seconds.

* Random clicking has been implemented. Just click the "Batch folder" button and see remarks in the *.bat files reside there in order how to use command line arguments and to enable random clicking.

* This is free, open source application without ads, viruses, trojans, malware, etc. forever.

### Watch the Youtube video about how to install and use The Fastest Mouse Clicker for Windows

https://www.youtube.com/watch?v=gCpALY1WqmE

#### The Fastest Mouse Clicker for Windows - how to download and install video

https://youtu.be/BwB65SpH3-I

This video as well as many others recorded by our partners you can explore at the official sites above.

#### Copyright (c) 2016-2025 by [Open Source Developer Masha Novedad](https://windows-2048.github.io/)

[See at indeed.com...](https://profile.indeed.com/p/mashan-hc2ql7c)
