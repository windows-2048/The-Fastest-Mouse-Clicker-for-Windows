## The Fastest Mouse Clicker for Windows

### [Official Download](https://gitlab.com/mashanovedad/The-Fastest-Mouse-Clicker-for-Windows/-/raw/master/WindowsInstaller/Install_TheFastestMouseClicker_2.5.4.0.exe?inline=false) - The latest version 2.5.4.0 - Aug 01 2020

### Free and fast, open source, full-featured, statically-linked mouse auto clicker for Windows written in vanilla C++. Uses hardware-limited arrayed Win32 <code><a href="https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-sendinput" target="_blank">SendInput()</a></code> calls to reach up to 100000 clicks/s. Supports command line, random clicks and record-play the sequences of clicks.

Please update your URLs:
* <span style="color:DarkOrange;">https://sourceforge.net/projects/fast-mouse-clicker-pro/</span>
* <span style="color:DarkOrange;">https://sourceforge.net/projects/fastclicker/</span>

— are obsolete and points to the wrong locations.

<span style="color:OliveDrab;"><b>Official site is here</b></span>.

### What's New - The latest version 2.5.4.0 - Aug 01 2020

* BRAND NEW Group App to record/play sequences of clicks!
* Fixed Help text to reflect brand new 'Run group app' button.
* Fixed 'Can't stop the clicking' bug.
* Few tiny bug fixes
* Few fixes in Windows Installer
* **IMPORTANT! Fixed clicking speed degradation at high clicking rates**

"Single" application | "Group" application
----- | -----
![The Fastest Mouse Clicker for Windows version 2.5.4.0: "Single" application](screenshots_new/v2.5.4.0/mw_v2.5.4.0.jpg?raw=true) | ![The Fastest Mouse Clicker for Windows version 2.5.4.0: "Group" application](screenshots_new/v2.5.4.0/mw_groupapp_v2.5.4.0.jpg?raw=true)

## Introduction

This is the official site to download various versions of The Fastest Mouse Clicker for Windows. Also there is [official mirror](https://gitlab.com/mashanovedad/The-Fastest-Mouse-Clicker-for-Windows/){:target="_blank"}.
All the versions are shipped with SHA256 fingerprints to ensure you do download the genuine software.

Industry standard free open source mouse auto clicker emulates Windows clicks EXTREMELY QUICKLY via arrays of 1-1000 mouse events in Win32 <code><a href="https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-sendinput" target="_blank">SendInput()</a></code>, making up to 100 000 clicks per second. Brand new group clicking support, Command line for batch files, Auto-save on exit, Random clicking, App's window always topmost are supported. This compact standalone program is compiled and statically linked by gnu/gcc compiler and supports all the Windows 7 to 10 and Linux via Wine emulator. This clicker is the best for incremental games: Cooking, Soda, Minecraft etc.

The program is written in vanilla C++ with native Win32 API and linked statically thus it becomes a super-compact executable without external dependencies and can run on a bare Windows installation.

<p>
Here is a short intro video that tells how to download and install The Fastest Mouse Clicker for Windows.
Also it shows basic quick-start guide to use automated clicks.
 <video style="outline:none; width:100%; height:100%;" controls preload="none" poster="/videos/TFMCfW_intro.jpg">
  <source src="videos/TFMCfW_intro.mp4" type="video/mp4"/>
  Your browser does not support the video tag.
</video>
<a href="https://www.youtube.com/watch?v=gCpALY1WqmE" target="_blank">Watch intro to The Fastest Mouse Clicker for Windows in Youtube.</a>
</p>

There are a lot of use-cases of The Fastest Mouse Clicker for Windows.
Amateurs can use it for cheating in various web sites or video games such as Counter-Strike: Global Offensive (CS:GO), Candy Crush Saga, Roblox games, etc.
Professionals can use it for quality assistant and testing purposes because full support of command line in batch files, PowerShell scripts, etc.

## Features

* The world's best click rate up to 100 000 clicks per second, increased by 10 times comparing with the predecessor application "Fast Mouse Clicker". The latest version with fixed performance issue is 100 times faster!

* Utilizes batch-array feature of <code><a href="https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-sendinput" target="_blank">SendInput()</a></code> and manipulates with <code><a href="https://docs.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-sleep" target="_blank">Sleep()</a></code> to reach the ultimate possible performance of mouse clicks on Windows.

* The Left, Middle, and Right mouse buttons are supported, they can be triggered for clicking by a key on the keyboard in a press or toggle mode.

* Arbitrary keyboard key can be selected to trigger the clicking process. Furthermore, an another mouse button can play a role of a trigger key.

* Different independent trigger keys to begin/end the clicking in toggle mode.

* The program works fine even if it is minimized and also it operates on an arbitrary desktop area. The program can stop to ckick automatically, if a certain number of clicks is given by end-user.

* This is free, open source application without ads, viruses, trojans, malware, etc. forever.

* The program has built-in updater service under construction that may perform additional scientific tasks when your CPU is idle with very tiny CPU and Internet usage. See source code of the installer. The application uninstalls clearly and is NOT a virus or malware.

* The application can be used on a bare system, it does not depend on .NET Framework or any other external library as "Speed AutoClicker", "Fast Clicker", etc.

* Command line has been supported: TheFastestMouseClicker.exe -c <clicks per second> -t <trigger key> -s <stop at> -m <trigger key mode> -b <mouse button to click>, where <trigger key mode> can be 'press' or'toggle' and <mouse button to click> can be 'left', 'middle', or 'right'. One may specify any part of arguments; unspecified or unrecognized values will be treated as defaults (see them by running the app and pressing 'Reset to defaults' button.

* Button "Batch folder" has been added to open a directory with *.bat files quickly; it simplifies command line usage a lot.

* Fractional values for clicks/s parameter are supported. For example, 0.5 clicks/s equals to 1 click every 2 seconds.

* Random clicking has been implemented. Just click the "Batch folder" button and see remarks in the *.bat files reside there in order how to use command line arguments and to enable random clicking.

* Group clicking (record/play the sequences of clicks) supported via additional application since v.2.5.3.2. You can quickly switch between the applications by clicking the "Run group app"/"Run single app" button.

* Window Always Top checkbox to keep the app's window topmost.

## Comparison

What about other auto-clickers and their features?
Here is the table that summarizes all the key features of 3 most downloaded programs.

Feature | [Fast Mouse Clicker](https://sourceforge.net/projects/fastclicker/){:target="_blank"} | [Auto<wbr/>Clicker](https://sourceforge.net/projects/orphamielautoclicker/){:target="_blank"} | The Fastest Mouse Clicker for Windows
------- | ------- | -------
Open source project | No | **Yes** | **Yes**
Regular updates and bug fixes | No | No | **Yes**
Arbitrary trigger key for clicking | **Yes** | **Yes** | **Yes**
Mouse button as trigger key for clicking | **Yes** | No | **Yes**
Independent trigger keys in toggle mode | No | **Yes** | **Yes**
All the clicking parameters auto-save | No | **Yes** | **Yes**
Group clicking (record-play the sequences of clicks) | No | **Yes** | **Yes**
Command line support in batch files | No | No | **Yes**
Button to open a folder with all the batch files | No | No | **Yes**
Button to reset all the clicking parameters to default values | No | No | **Yes**
Random clicking in a specified rectangle | No | No | **Yes**
Hardware-limited fastest clicking via arrayed <code><a href="https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-sendinput" target="_blank">SendInput()</a></code> | No | No | **Yes**
Side DLL/runtime independent (runs on bare Windows) | No | No | **Yes**
Checkbox to keep the app window always Top | No | No | **Yes**

The Fastest Mouse Clicker for Windows wins this competition because its code is a further developing of the rest 2 popular apps.

## Technology

Unlike other auto-clickers that use obsolete <code><a href="https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-mouse_event" target="_blank">mouse_event()</a></code>
system call from C/C++ source or un-arrayed <code><a href="https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-sendinput" target="_blank">SendInput()</a></code> from C#/.Net source, The Fastest Mouse Clicker for Windows uses
<i>arrayed</i> <code><a href="https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-sendinput" target="_blank">SendInput()</a></code> with specially prepared <i>arrays</i> of mouse events:

<pre><code title="Arrayed SendInput() example">
UINT nCntExtra = (nCnt - 1) * 2; // reserved index for DOWN, UP

for (UINT iExtra = 0; iExtra < nCntExtra; iExtra += 2)
{
    input[1 + iExtra].type = INPUT_MOUSE;

    input[1 + iExtra].mi.dx = dx;
    input[1 + iExtra].mi.dy = dy;

    input[1 + iExtra].mi.mouseData = dwData;
    input[1 + iExtra].mi.time = 0;
    input[1 + iExtra].mi.dwExtraInfo = dwExtraInfo;

    ...
}

...

UINT ret = SendInput(1 + nCntExtra, input, sizeof(INPUT));
</code></pre>

The size of the <i>arrays</i> is carefully computed based on the click rate given by end-user. To avoid system event buffer
overflow, the time in <code><a href="https://docs.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-sleep" target="_blank">Sleep()</a></code> is selected properly according the size of the <i>array</i>.

The GUI of the application seems archaic, but it is made by very base Win32 system calls
to avoid performance degradation caused by
high-level third-side libraries such as [Qt](https://www.qt.io/){:target="_blank"} or slow managed code in frameworks like C#/.Net.
For example, <code><a href="https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getasynckeystate" target="_blank">GetAsyncKeyState()</a></code> is used to detect the trigger keys pressed by end-user:

<pre><code title="Base GetAsyncKeyState() example">
if (!doToggle)
{
    if (toggleState == 0 && GetAsyncKeyState(atoi(triggerText)))
        toggleState = 1;
    ...
}
else
{
    if (toggleState == 0 && GetAsyncKeyState(atoi(triggerText)))
        toggleState = 1;
    ...
}
</code></pre>

Another benefit of such an approach is compact, statically-linked executable without any external dependencies.

When end-user selects low click rates, actual size of the <i>array</i> of mouse events in <code><a href="https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-sendinput" target="_blank">SendInput()</a></code>
is set to 1 and number of clicks per second is regulated by <code><a href="https://docs.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-sleep" target="_blank">Sleep()</a></code> only.
But when end-user selects high click rates, the size of the <i>array</i> becomes significant. In rare circumstances, it may lead to freeze the whole Windows GUI.
To avoid that, the helper thread is created to scan <code><a href="https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getasynckeystate" target="_blank">GetAsyncKeyState()</a></code> independently in order end-user has requested to stop the clicking
and force <code><a href="https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-blockinput" target="_blank">BlockInput()</a></code> because mouse event buffer may be full:

<pre><code title="Helper thread with BlockInput() example">
DWORD WINAPI MyThreadFunction(LPVOID lpParam)
{
    while (true)
    {
        if (GetAsyncKeyState(atoi(triggerText2)))
        {
            ...
            BlockInput(TRUE);
            Sleep(100);
            BlockInput(FALSE);
            ...
            SetMsgStatus(hWnd, GetDlgCtrlID(statusText), "idle");
        }

        Sleep(10);
    }

    return 0;
}
</code></pre>

Complete source code with comments is shipped with Windows installer or can be watched on
[Github](https://github.com/windows-2048/The-Fastest-Mouse-Clicker-for-Windows){:target="_blank"}
and [Gitlab](https://gitlab.com/mashanovedad/The-Fastest-Mouse-Clicker-for-Windows){:target="_blank"}.

## Screenshots

### For the latest version 2.5.4.0

* The Fastest Mouse Clicker for Windows version 2.5.4.0: introduce the "Run group app" button.

![The Fastest Mouse Clicker for Windows version 2.5.4.0: introduce the "Run group app" button](screenshots_new/v2.5.4.0/TFMCfW_2_5_4_0w_s1_1322x986.jpg?raw=true)

* The Fastest Mouse Clicker for Windows version 2.5.4.0: the brand new Group App in details.

![The Fastest Mouse Clicker for Windows version 2.5.4.0: the brand new Group App in details](screenshots_new/v2.5.4.0/TFMCfW_2_5_4_0w_s1h_1322x986.jpg?raw=true)

* The Fastest Mouse Clicker for Windows version 2.5.4.0: running under Wine 4.0.2/Linux(CentOS 6).

![The Fastest Mouse Clicker for Windows version 2.5.4.0: running under Wine 4.0.2/Linux(CentOS 6)](screenshots_new/v2.5.4.0/TFMCfW_2_5_4_0w_s1w_1322x986.jpg?raw=true)

* The Fastest Mouse Clicker for Windows version pre-2.5.x.x family: what's old but important.

![The Fastest Mouse Clicker for Windows version pre-2.5.x.x family: what's old but important](screenshots_new/v2.5.4.0/TFMCfW_2_5_4_0w_s2_1322x986.jpg?raw=true)

* The Fastest Mouse Clicker for Windows version 2.5.4.0: completely hand-made art by the clicker application.

![The Fastest Mouse Clicker for Windows version 2.5.4.0: completely hand-made art by the clicker application](screenshots_new/v2.5.4.0/TFMCfW_2_5_4_0w_a1_1322x986.jpg?raw=true)

## Partners

* [Bytesin](https://www.bytesin.com/software/The-Fastest-Mouse-Clicker-for-Windows/){:target="_blank"}, Your Daily Dose of Bytes

* [Chocolatey](https://chocolatey.org/packages/fastest-mouse-clicker){:target="_blank"}, The Package Manager for Windows

* [Github](https://github.com/windows-2048/The-Fastest-Mouse-Clicker-for-Windows){:target="_blank"}, Built for Developers

* [Gitlab](https://gitlab.com/mashanovedad/The-Fastest-Mouse-Clicker-for-Windows){:target="_blank"}, Simplify Your Workflows

* [Lamerkomp](https://lamerkomp.ru/load/sistemnye_utility/avtoklikery/the_fastest_mouse_clicker_for_windows/56-1-0-6328){:target="_blank"}, Download Freeware without Registration

* [Majorgeeks](http://m.majorgeeks.com/files/details/the_fastest_mouse_clicker_for_windows.html){:target="_blank"}, It's Geekalicious

* [OSDN](https://osdn.net/projects/fastest-clicker/){:target="_blank"}, Develop and Download Open Source Software

* [Softpedia](https://www.softpedia.com/get/System/OS-Enhancements/The-Fastest-Mouse-Clicker-for-Windows.shtml){:target="_blank"}, Free Downloads Encyclopedia

* [Uptodown](https://the-fastest-mouse-clicker-for-windows.en.uptodown.com/windows/download){:target="_blank"}, Download Discover Share

## Download all the official releases

* Aug 01 2020 - Windows installer (32/64-bit): [Install_TheFastestMouseClicker_2.5.4.0.exe](https://gitlab.com/mashanovedad/The-Fastest-Mouse-Clicker-for-Windows/-/raw/master/WindowsInstaller/Install_TheFastestMouseClicker_2.5.4.0.exe?inline=false)

SHA256(Install_TheFastestMouseClicker_2.5.4.0.exe)= 738058b7dc1e95b963860e5797bab5761a8801bda90feb0311c038e98477cc31

* Jul 15 2020 - Windows installer (32/64-bit): [Install_TheFastestMouseClicker_2.5.3.3.exe](https://gitlab.com/mashanovedad/The-Fastest-Mouse-Clicker-for-Windows/-/raw/master/WindowsInstaller/Install_TheFastestMouseClicker_2.5.3.3.exe?inline=false)

SHA256(Install_TheFastestMouseClicker_2.5.3.3.exe)= 55bde08c90989d4dbeb9602d93b3c7bcb3645135281e1b64c32d59521799836b

* Jun 17 2020 - Windows installer (32/64-bit): [Install_TheFastestMouseClicker_2.5.3.2.exe](https://gitlab.com/mashanovedad/The-Fastest-Mouse-Clicker-for-Windows/-/raw/master/WindowsInstaller/Install_TheFastestMouseClicker_2.5.3.2.exe?inline=false)

SHA256(Install_TheFastestMouseClicker_2.5.3.2.exe)= 58854af05b2024ce39078d828228d512548212fc3283c511c1a16c19c844bf06

* May 06 2020 - Windows installer (32/64-bit): [Install_TheFastestMouseClicker_2.5.1.0.exe](https://gitlab.com/mashanovedad/The-Fastest-Mouse-Clicker-for-Windows/-/raw/master/WindowsInstaller/Install_TheFastestMouseClicker_2.5.1.0.exe?inline=false)

SHA256(Install_TheFastestMouseClicker_2.5.1.0.exe)= cb13c125212feb8241f4a4258919781d546084f0f19862ad11f07a3e95004577

* Apr 22 2019 - Windows installer (32/64-bit): [Install_TheFastestMouseClicker_2.0.0.0.exe](https://gitlab.com/mashanovedad/The-Fastest-Mouse-Clicker-for-Windows/-/raw/master/WindowsInstaller/Install_TheFastestMouseClicker_2.0.0.0.exe?inline=false)

SHA256(Install_TheFastestMouseClicker_2.0.0.0.exe)= c12fbeee1a12ce598bcd1f6b39872abcbcfc89d2b21d235882ca479fd26a324a

## Contacts

#### Copyright 2016-2020 by [Open Source Developer Masha Novedad](https://windows-2048.github.io){:target="_blank"}
