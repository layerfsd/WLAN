@ echo off
echo ********************************************
echo Configure CE
echo ********************************************

perl configure VC-CE -DOPENSSL_USE_IPV6=0
ms\do_ms.bat
