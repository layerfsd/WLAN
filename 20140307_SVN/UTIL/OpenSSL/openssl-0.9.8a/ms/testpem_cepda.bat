@echo off
set ssleay=%1
set tmp1=\OpenSSL\pem.out
set cmp=\Openssl\OpenSSL\cmp.exe

call tpem_cepda.bat crl \OpenSSL\test\testcrl.pem
if errorlevel 1 goto err

call tpem_cepda.bat pkcs7 \OpenSSL\test\testp7.pem
if errorlevel 1 goto err

call tpem_cepda.bat req \OpenSSL\test\testreq2.pem
if errorlevel 1 goto err

call tpem_cepda.bat rsa \OpenSSL\test\testrsa.pem
if errorlevel 1 goto err

call tpem_cepda.bat x509 \OpenSSL\test\testx509.pem
if errorlevel 1 goto err

call tpem_cepda.bat x509 \OpenSSL\test\v3-cert1.pem
if errorlevel 1 goto err

call tpem_cepda.bat x509 \OpenSSL\test\v3-cert1.pem
if errorlevel 1 goto err

call tpem_cepda.bat sess_id \OpenSSL\test\testsid.pem
if errorlevel 1 goto err

echo OK
del %tmp1%
:err
