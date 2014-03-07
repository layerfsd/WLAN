@echo off

echo Creating Directory structure...

mkdir ..\toPDA >2
mkdir ..\toPDA\OpenSSL  >2
mkdir ..\toPDA\OpenSSL\OpenSSL  >2
mkdir ..\toPDA\OpenSSL\ms  >null
mkdir ..\toPDA\OpenSSL\test  >2
mkdir ..\toPDA\OpenSSL\certs  >2

echo Copying binaries

copy ..\out32dll_%TARGETCPU%_pda_console\*.exe ..\toPDA\OpenSSL\OpenSSL 
copy ..\out32dll_%TARGETCPU%_pda_console\*.dll ..\toPDA\OpenSSL\OpenSSL >2

echo Copying test batch files

copy ..\ms\test_cepda.bat ..\toPDA\OpenSSL\ms\ >2
copy ..\ms\testenc_cepda.bat ..\toPDA\OpenSSL\ms\ >2
copy ..\ms\tenc_cepda.bat ..\toPDA\OpenSSL\ms\ >2
copy ..\ms\testpem_cepda.bat ..\toPDA\OpenSSL\ms\  >2
copy ..\ms\tpem_cepda.bat ..\toPDA\OpenSSL\ms\ >2
copy ..\ms\testss_cepda.bat ..\toPDA\OpenSSL\ms\ >2

echo Copying Configuration and certificate files

copy ..\test\CAss.cnf ..\toPDA\OpenSSL\test\ >2
copy ..\test\testp7.pem ..\toPDA\OpenSSL\test\ >2
copy ..\test\testreq2.pem ..\toPDA\OpenSSL\test\ >2
copy ..\test\testrsa.pem ..\toPDA\OpenSSL\test\ >2
copy ..\test\testsid.pem ..\toPDA\OpenSSL\test\ >2
copy ..\test\v3-cert1.pem ..\toPDA\OpenSSL\test\ >2
copy ..\test\testcrl.pem ..\toPDA\OpenSSL\test\ >2
copy ..\test\evptests.txt ..\toPDA\OpenSSL\test\ >2
copy ..\test\testx509.pem ..\toPDA\OpenSSL\test\ >2
copy ..\test\Uss.cnf ..\toPDA\OpenSSL\test\ >2

copy ..\apps\server.pem ..\toPDA\OpenSSL\ >2
copy ..\apps\client.pem ..\toPDA\OpenSSL\ >2

copy ..\certs\*.pem ..\toPDA\OpenSSL\certs\ >2

copy ..\apps\openssl.cnf ..\toPDA\OpenSSL\OpenSSL\ >2

copy %WCE_LITE_COMPAT%\exe\cmp.exe ..\toPDA\OpenSSL\OpenSSL >2

echo To test copy "the content" of toPDA directory directly to your
echo PDA home directory "\". Open a console in your PDA, move to 
echo \OpenSSL\OpenSSL\ and run ..\ms\test_cepda.bat 
echo thats all!
