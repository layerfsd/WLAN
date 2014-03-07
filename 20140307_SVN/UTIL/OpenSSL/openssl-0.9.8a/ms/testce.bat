@echo off

pmkdir \OpenSSL

set test=ms
set opath=%PATH%
PATH=..\ms;%PATH%
pput -f apps\openssl.cnf \OpenSSL
set OPENSSL_CONF=\OpenSSL\openssl.cnf
set HOME=\OpenSSL
set prun_PASS_ENV=OPENSSL_CONF HOME

rem run this from inside the bin directory

rem Copy the DLL's (though they'll only exist if we're in out32dll)
REM if exist out32dll\libeay32.dll pput -f out32dll\libeay32.dll \OpenSSL
REM if exist out32dll\ssleay32.dll pput -f out32dll\ssleay32.dll \OpenSSL


echo testenc
call %test%\testencce openssl.exe
if errorlevel 1 goto done

goto end

echo rsa_test
call %test%\testce2 rsa_test
if errorlevel 1 goto done

echo destest
call %test%\testce2 destest
if errorlevel 1 goto done

echo ideatest
call %test%\testce2 ideatest
if errorlevel 1 goto done

echo bftest
call %test%\testce2 bftest
if errorlevel 1 goto done

echo shatest
call %test%\testce2 shatest
if errorlevel 1 goto done

echo sha1test
call %test%\testce2 sha1test
if errorlevel 1 goto done

echo md5test
call %test%\testce2 md5test
if errorlevel 1 goto done

echo md2test
call %test%\testce2 md2test
if errorlevel 1 goto done

REM echo mdc2test
REM call %test%\testce2 mdc2test
REM if errorlevel 1 goto done

echo rc2test
call %test%\testce2 rc2test
if errorlevel 1 goto done

echo rc4test
call %test%\testce2 rc4test
if errorlevel 1 goto done

echo randtest
call %test%\testce2 randtest
if errorlevel 1 goto done

echo dhtest
call %test%\testce2 dhtest
if errorlevel 1 goto done

echo exptest
call %test%\testce2 exptest
if errorlevel 1 goto done

echo dsatest
call %test%\testce2 dsatest
if errorlevel 1 goto done

echo testenc
call %test%\testencce openssl.exe
if errorlevel 1 goto done

echo testpem
call %test%\testpemce openssl.exe
if errorlevel 1 goto done

pput -f openssl.exe \OpenSSL

echo verify
copy ..\certs\*.pem cert.tmp >nul
pput -f cert.tmp \OpenSSL
pmkdir \OpenSSL\certs
rem pput -f ..\certs\*.pem \OpenSSL\certs
pput -f ..\certs\ca-cert.pem \OpenSSL\certs
pput -f ..\certs\dsa-ca.pem \OpenSSL\certs
pput -f ..\certs\dsa-pca.pem \OpenSSL\certs
pput -f ..\certs\factory.pem \OpenSSL\certs
pput -f ..\certs\ICE-CA.pem \OpenSSL\certs
pput -f ..\certs\ICE-root.pem \OpenSSL\certs
pput -f ..\certs\ICE-user.pem \OpenSSL\certs
pput -f ..\certs\nortelCA.pem \OpenSSL\certs
pput -f ..\certs\pca-cert.pem \OpenSSL\certs
pput -f ..\certs\RegTP-4R.pem \OpenSSL\certs
pput -f ..\certs\RegTP-5R.pem \OpenSSL\certs
pput -f ..\certs\RegTP-6R.pem \OpenSSL\certs
pput -f ..\certs\rsa-cca.pem \OpenSSL\certs
pput -f ..\certs\thawteCb.pem \OpenSSL\certs
pput -f ..\certs\thawteCp.pem \OpenSSL\certs
pput -f ..\certs\timCA.pem \OpenSSL\certs
pput -f ..\certs\tjhCA.pem \OpenSSL\certs
pput -f ..\certs\vsign1.pem \OpenSSL\certs
pput -f ..\certs\vsign2.pem \OpenSSL\certs
pput -f ..\certs\vsign3.pem \OpenSSL\certs
pput -f ..\certs\vsignss.pem \OpenSSL\certs
pput -f ..\certs\vsigntca.pem \OpenSSL\certs
prun \OpenSSL\openssl verify -CAfile \OpenSSL\cert.tmp \OpenSSL\certs\*.pem

echo testss
call %test%\testssce openssl.exe
if errorlevel 1 goto done

pput -f ssltest.exe \OpenSSL
pput -f ..\apps\server.pem \OpenSSL
pput -f ..\apps\client.pem \OpenSSL

echo test sslv2
prun \OpenSSL\ssltest -ssl2
if errorlevel 1 goto done

echo test sslv2 with server authentication
prun \OpenSSL\ssltest -ssl2 -server_auth -CAfile \OpenSSL\cert.tmp
if errorlevel 1 goto done

echo test sslv2 with client authentication
prun \OpenSSL\ssltest -ssl2 -client_auth -CAfile \OpenSSL\cert.tmp
if errorlevel 1 goto done

echo test sslv2 with both client and server authentication
prun \OpenSSL\ssltest -ssl2 -server_auth -client_auth -CAfile \OpenSSL\cert.tmp
if errorlevel 1 goto done

echo test sslv3
prun \OpenSSL\ssltest -ssl3
if errorlevel 1 goto done

echo test sslv3 with server authentication
prun \OpenSSL\ssltest -ssl3 -server_auth -CAfile \OpenSSL\cert.tmp
if errorlevel 1 goto done

echo test sslv3 with client authentication
prun \OpenSSL\ssltest -ssl3 -client_auth -CAfile \OpenSSL\cert.tmp
if errorlevel 1 goto done

echo test sslv3 with both client and server authentication
prun \OpenSSL\ssltest -ssl3 -server_auth -client_auth -CAfile \OpenSSL\cert.tmp
if errorlevel 1 goto done

echo test sslv2/sslv3
prun \OpenSSL\ssltest
if errorlevel 1 goto done

echo test sslv2/sslv3 with server authentication
prun \OpenSSL\ssltest -server_auth -CAfile \OpenSSL\cert.tmp
if errorlevel 1 goto done

echo test sslv2/sslv3 with client authentication
prun \OpenSSL\ssltest -client_auth -CAfile \OpenSSL\cert.tmp
if errorlevel 1 goto done

echo test sslv2/sslv3 with both client and server authentication
prun \OpenSSL\ssltest -server_auth -client_auth -CAfile \OpenSSL\cert.tmp
if errorlevel 1 goto done

echo test sslv2 via BIO pair
prun \OpenSSL\ssltest -bio_pair -ssl2
if errorlevel 1 goto done

echo test sslv2/sslv3 with 1024 bit DHE via BIO pair
prun \OpenSSL\ssltest -bio_pair -dhe1024dsa -v
if errorlevel 1 goto done

echo test sslv2 with server authentication via BIO pair
prun \OpenSSL\ssltest -bio_pair -ssl2 -server_auth -CAfile \OpenSSL\cert.tmp
if errorlevel 1 goto done

echo test sslv2 with client authentication via BIO pair
prun \OpenSSL\ssltest -bio_pair -ssl2 -client_auth -CAfile \OpenSSL\cert.tmp
if errorlevel 1 goto done

echo test sslv2 with both client and server authentication via BIO pair
prun \OpenSSL\ssltest -bio_pair -ssl2 -server_auth -client_auth -CAfile \OpenSSL\cert.tmp
if errorlevel 1 goto done

echo test sslv3 via BIO pair
prun \OpenSSL\ssltest -bio_pair -ssl3
if errorlevel 1 goto done

echo test sslv3 with server authentication via BIO pair
prun \OpenSSL\ssltest -bio_pair -ssl3 -server_auth -CAfile \OpenSSL\cert.tmp
if errorlevel 1 goto done

echo test sslv3 with client authentication  via BIO pair
prun \OpenSSL\ssltest -bio_pair -ssl3 -client_auth -CAfile \OpenSSL\cert.tmp
if errorlevel 1 goto done

echo test sslv3 with both client and server authentication via BIO pair
prun \OpenSSL\ssltest -bio_pair -ssl3 -server_auth -client_auth -CAfile \OpenSSL\cert.tmp
if errorlevel 1 goto done

echo test sslv2/sslv3 via BIO pair
prun \OpenSSL\ssltest
if errorlevel 1 goto done

echo test sslv2/sslv3 with server authentication
prun \OpenSSL\ssltest -bio_pair -server_auth -CAfile \OpenSSL\cert.tmp
if errorlevel 1 goto done

echo test sslv2/sslv3 with client authentication via BIO pair
prun \OpenSSL\ssltest -bio_pair -client_auth -CAfile \OpenSSL\cert.tmp
if errorlevel 1 goto done

echo test sslv2/sslv3 with both client and server authentication via BIO pair
prun \OpenSSL\ssltest -bio_pair -server_auth -client_auth -CAfile \OpenSSL\cert.tmp
if errorlevel 1 goto done

del cert.tmp

echo passed all tests
goto end
:done
echo problems.....
:end
PATH=%opath%

