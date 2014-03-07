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

@REM echo Etc Env (Default)
@REM set PATH=D:\Program Files\Microsoft Visual Studio 8\VC\ce\bin\x86_arm;D:\Program Files\Microsoft Visual Studio 8\VC\bin;D:\Program Files\Microsoft Visual Studio 8\VC\PlatformSDK\bin;D:\Program Files\Microsoft Visual Studio 8\Common7\Tools;D:\Program Files\Microsoft Visual Studio 8\Common7\IDE;D:\Program Files\Microsoft Visual Studio 8\Common7\Tools;D:\Program Files\Microsoft Visual Studio 8\Common7\IDE;D:\Program Files\Microsoft Visual Studio 8\;$(FrameworkSDKDir)Bin;%PATH%
@REM set INCLUDE=D:\Program Files\Microsoft Visual Studio 8\VC\ce\include;D:\Program Files\Windows Mobile 6 SDK\PocketPC\include\ARMV4I;D:\Program Files\Microsoft Visual Studio 8\VC\ce\atlmfc\include;D:\Program Files\Microsoft Visual Studio 8\SmartDevices\SDK\SQL Server\Mobile\v3.0
@REM set LIB=D:\Program Files\Windows Mobile 6 SDK\PocketPC\Lib\Armv4i;D:\Program Files\Microsoft Visual Studio 8\VC\ce\atlmfc\lib\ARMV4I;D:\Program Files\Microsoft Visual Studio 8\VC\ce\lib\ARMV4I\;

echo Path, Include, Lib - Env
set PATH=%PATH%;%_PERLBIN%;%_PERLSITE%;%_OPENSSSL_ROOT%\bat
set INCLUDE=%wcecompat%\include;%_PROJECTROOT%\cesysgen\oak\inc;%_PROJECTROOT%\cesysgen\ddk\inc;%_PROJECTROOT%\cesysgen\sdk\inc;
set LIB=%wcecompat%\lib;%_PROJECTROOT%\cesysgen\oak\lib;%_PROJECTROOT%\cesysgen\ddk\lib;%_PROJECTROOT%\cesysgen\sdk\lib;