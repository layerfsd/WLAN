REM @echo off
echo start testenc

path=..\ms;%path%
set ssleay=%1%
copy ms\testenc.bat >nul
set input=testenc.bat
set tmp1=cipher.out
set out1=clear.out
set cmp=perl ms\cmp.pl

pput -f out32dll\%ssleay% \OpenSSL

call ms\tencce.bat enc
if errorlevel 1 goto err

call ms\tencce.bat rc4
if errorlevel 1 goto err

call ms\tencce.bat des-cfb
if errorlevel 1 goto err

call ms\tencce.bat des-ede-cfb
if errorlevel 1 goto err

call ms\tencce.bat des-ede3-cfb
if errorlevel 1 goto err

call ms\tencce.bat des-ofb
if errorlevel 1 goto err

call ms\tencce.bat des-ede-ofb
if errorlevel 1 goto err

call ms\tencce.bat des-ede3-ofb
if errorlevel 1 goto err

call ms\tencce.bat des-ecb
if errorlevel 1 goto err

call ms\tencce.bat des-ede
if errorlevel 1 goto err

call ms\tencce.bat des-ede3
if errorlevel 1 goto err

call ms\tencce.bat des-cbc
if errorlevel 1 goto err

call ms\tencce.bat des-ede-cbc
if errorlevel 1 goto err

call ms\tencce.bat des-ede3-cbc
if errorlevel 1 goto err

call ms\tencce.bat idea-ecb
if errorlevel 1 goto err

call ms\tencce.bat idea-cfb
if errorlevel 1 goto err

call ms\tencce.bat idea-ofb
if errorlevel 1 goto err

call ms\tencce.bat idea-cbc
if errorlevel 1 goto err

call ms\tencce.bat rc2-ecb
if errorlevel 1 goto err

call ms\tencce.bat rc2-cfb
if errorlevel 1 goto err

call ms\tencce.bat rc2-ofb
if errorlevel 1 goto err

call ms\tencce.bat rc2-cbc
if errorlevel 1 goto err

call ms\tencce.bat bf-ecb
if errorlevel 1 goto err

call ms\tencce.bat bf-cfb
if errorlevel 1 goto err

call ms\tencce.bat bf-ofb
if errorlevel 1 goto err

call ms\tencce.bat bf-cbc
if errorlevel 1 goto err

echo OK
del %out1% >nul 2>&1
del %tmp1% >nul 2>&1
:err

