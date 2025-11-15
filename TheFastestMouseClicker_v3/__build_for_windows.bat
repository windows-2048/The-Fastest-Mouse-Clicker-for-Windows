@echo off
pushd "%~dp0"
cd /D "%~dp0"

echo CD=%CD%



set PATHMSVC2019=C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\Common7\IDE\\Extensions\Microsoft\IntelliCode\CLI;C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Tools\MSVC\14.29.30133\bin\HostX64\x64;C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\Common7\IDE\VC\VCPackages;C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\Common7\IDE\CommonExtensions\Microsoft\TestWindow;C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\Common7\IDE\CommonExtensions\Microsoft\TeamFoundation\Team Explorer;C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\MSBuild\Current\bin\Roslyn;C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\Team Tools\Performance Tools\x64;C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\Team Tools\Performance Tools;C:\Program Files (x86)\Microsoft Visual Studio\Shared\Common\VSPerfCollectionTools\vs2019\\x64;C:\Program Files (x86)\Microsoft Visual Studio\Shared\Common\VSPerfCollectionTools\vs2019\;C:\Program Files (x86)\Microsoft SDKs\Windows\v10.0A\bin\NETFX 4.8 Tools\x64\;C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\Common7\IDE\CommonExtensions\Microsoft\FSharp\Tools;C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\Common7\Tools\devinit;C:\Program Files (x86)\Windows Kits\10\bin\10.0.18362.0\x64;C:\Program Files (x86)\Windows Kits\10\bin\x64;C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\\MSBuild\Current\Bin;C:\Windows\Microsoft.NET\Framework64\v4.0.30319;C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\Common7\IDE\;C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\Common7\Tools
set PATHMSVCEXTR=C:\Users\test\AppData\Local\Programs\Python\Python311;C:\Program Files\CMake\bin

set PATH=.;%PATHMSVC2019%;%PATHMSVCEXTR%;C:\Program Files\WinRAR;C:\OpenSSL-Win32\bin;%systemroot%\System32;%systemroot%;%systemroot%\System32\Wbem
echo PATH=%PATH%


mkdir build
cd build\

cmake -DQt5_DIR=C:\qt-5.15.5-static\lib\cmake\Qt5 -G "Visual Studio 16 2019" ..

MSBuild.exe TheFastestMouseClickerv3.sln ^
    /p:Configuration=Release ^
    /p:Platform=x64 ^
    /t:TheFastestMouseClickerv3:Rebuild ^
    /m:4 ^
    /verbosity:normal

if %ERRORLEVEL% NEQ 0 (
    echo Build failed with error code %ERRORLEVEL%
    pause
    exit /b %ERRORLEVEL%
) else (
    echo Build succeeded
)



cd ..



popd

pause
