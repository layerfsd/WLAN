@ echo off
echo ********************************************
echo  Settings - OpenSSl Env
echo ********************************************

echo PERL Path
set _PERLBIN=C:\Perl\bin
set _PERLSITE=C:\Perl\site

echo OpenSSL Root Path
set _OPENSSSL_ROOT=D:\DEV_BOX\Backup\Wireless\Wlan\UTIL\OpenSSL

echo wecompat Path
set wcecompat=%_OPENSSSL_ROOT%\wcecompat

echo Platform Env
set OSVERSION=WCE502
set TARGETCPU=ARMV4I
set PLATFORM=VC-CE

REM echo Etc Env (VS 2008)
REM set PATH=C:\Program Files\Microsoft Visual Studio 9.0\VC\ce\bin\x86_arm;C:\Program Files\Microsoft Visual Studio 9.0\VC\bin;C:\Program Files\Microsoft Visual Studio 9.0\VC\PlatformSDK\bin;C:\Program Files\Microsoft Visual Studio 9.0\Common7\Tools;C:\Program Files\Microsoft Visual Studio 9.0\Common7\IDE;C:\Program Files\Microsoft Visual Studio 9.0\Common7\Tools;C:\Program Files\Microsoft Visual Studio 9.0\Common7\IDE;C:\Program Files\Microsoft Visual Studio 9.0\;$(FrameworkSDKDir)Bin;%PATH%
REM set PATH=%PATH%;%_PERLBIN%;%_PERLSITE%;%_OPENSSSL_ROOT%\bat;C:\Program Files\Microsoft Visual Studio 9.0\VC\lib
REM set INCLUDE=C:\Program Files\Microsoft Visual Studio 9.0\VC\ce\include;D:\Program Files\Windows Mobile 6 SDK\PocketPC\include\ARMV4I;C:\Program Files\Microsoft Visual Studio 9.0\VC\ce\atlmfc\include;C:\Program Files\Microsoft Visual Studio 9.0\SmartDevices\SDK\SQL Server\Mobile\v3.0
REM set LIB=D:\Program Files\Windows Mobile 6 SDK\PocketPC\Lib\Armv4i;C:\Program Files\Microsoft Visual Studio 9.0\VC\ce\atlmfc\lib\ARMV4I;C:\Program Files\Microsoft Visual Studio 9.0\VC\ce\lib\ARMV4I\;

echo Path, Include, Lib - Env (WEHH AKU)
set PATH=%PATH%;%_PERLBIN%;%_PERLSITE%;%_OPENSSSL_ROOT%\bat
set INCLUDE=%wcecompat%\include;%_PROJECTROOT%\cesysgen\sdk\inc
set LIB=%wcecompat%\lib;%_PROJECTROOT%\cesysgen\sdk\lib\ARMV4I\retail
