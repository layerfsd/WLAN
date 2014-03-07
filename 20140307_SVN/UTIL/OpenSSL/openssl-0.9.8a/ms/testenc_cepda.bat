@echo off
echo start testenc_cepda

set PATH=..\ms;
set ssleay=%1
set input=\Openssl\ms\testenc_cepda.bat
set tmp1=\Openssl\ms\cipher.out
set out1=\Openssl\ms\clear.out
rem set cmp=perl \Openssl\ms\cmp.pl
set cmp=\openssl\openssl\cmp.exe

cd
call tenc_cepda.bat enc
if errorlevel 1 goto err

call tenc_cepda.bat rc4
if errorlevel 1 goto err

call tenc_cepda.bat des-cfb
if errorlevel 1 goto err

call tenc_cepda.bat des-ede-cfb
if errorlevel 1 goto err

call tenc_cepda.bat des-ede3-cfb
if errorlevel 1 goto err

call tenc_cepda.bat des-ofb
if errorlevel 1 goto err

call tenc_cepda.bat des-ede-ofb
if errorlevel 1 goto err

call tenc_cepda.bat des-ede3-ofb
if errorlevel 1 goto err

call tenc_cepda.bat des-ecb
if errorlevel 1 goto err

call tenc_cepda.bat des-ede
if errorlevel 1 goto err

call tenc_cepda.bat des-ede3
if errorlevel 1 goto err

call tenc_cepda.bat des-cbc
if errorlevel 1 goto err

call tenc_cepda.bat des-ede-cbc
if errorlevel 1 goto err

call tenc_cepda.bat des-ede3-cbc
if errorlevel 1 goto err

call tenc_cepda.bat idea-ecb
if errorlevel 1 goto err

call tenc_cepda.bat idea-cfb
if errorlevel 1 goto err

call tenc_cepda.bat idea-ofb
if errorlevel 1 goto err

call tenc_cepda.bat idea-cbc
if errorlevel 1 goto err

call tenc_cepda.bat rc2-ecb
if errorlevel 1 goto err

call tenc_cepda.bat rc2-cfb
if errorlevel 1 goto err

call tenc_cepda.bat rc2-ofb
if errorlevel 1 goto err

call tenc_cepda.bat rc2-cbc
if errorlevel 1 goto err

call tenc_cepda.bat bf-ecb
if errorlevel 1 goto err

call tenc_cepda.bat bf-cfb
if errorlevel 1 goto err

call tenc_cepda.bat bf-ofb
if errorlevel 1 goto err

call tenc_cepda.bat bf-cbc
if errorlevel 1 goto err

echo OK
del %out1%
del %tmp1%
:err

