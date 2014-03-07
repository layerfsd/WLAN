// ----------------------------------------------------------------------------------
// Windows Mobile (WEHH)���� openssl �� ������ �ϱ� ���� ��� ����
//
// �ۼ��� : �Ե�ȣ
// �ۼ��� : 2013-02-14
// ----------------------------------------------------------------------------------

1. OpenSSL source �ٿ�ε�
     http://www.openssl.org/source

2. Perl �ٿ�ε�
     http://downloads.activestate.com/ActivePerl/Windows/ (�ֽŹ��� �˾Ƽ� ����)

3. wcecompat �ٿ�ε�
     http://github.com/mauricek/wcecompat
    
4. Building & Path
    ** ȯ�漳�� 
	 - WEHH Base OS ���� ��ũ���� ����.
	 - OpenSSL\bat\SetOpenSSLenv.bat �� ����.

    1) VS �� Active Perl ��ġ
    2) OpenSSL ���� Ǯ��
    3) ���� -> ��� ���α׷� -> Microsoft Visual Studio 2005 -> Visual Studio Tools -> Visual Studio 2005 ��� ������Ʈ ����
    4) wcecompat ������ �̵�
    5) ���� �� �Է� (Program file �н� ����, Win CE �ϰ�� �ش� SDK ���� ����, ����� Win Mobile 6 ������)

	-------------------------------------------------------------------------------------------------------------
	| MS ����Ʈ���� ���� ����.  (�츮�� OpenSSL\bat\SetOpenSSLenv.bat �� �����ϸ� ���� ��)
	-------------------------------------------------------------------------------------------------------------
 	set wcecompat=�ش� ���丮
	set OSVERSION=WCE502
	set TARGETCPU=ARMV4I
	set PLATFORM=VC-CE
	set PATH=D:\Program Files\Microsoft Visual Studio 8\VC\ce\bin\x86_arm;D:\Program Files\Microsoft Visual Studio 8\VC\bin;D:\Program Files\Microsoft Visual Studio 8\VC\PlatformSDK\bin;D:\Program Files\Microsoft Visual Studio 8\Common7\Tools;D:\Program Files\Microsoft Visual Studio 8\Common7\IDE;D:\Program Files\Microsoft Visual Studio 8\Common7\Tools;D:\Program Files\Microsoft Visual Studio 8\Common7\IDE;D:\Program Files\Microsoft Visual Studio 8\;$(FrameworkSDKDir)Bin;%PATH%
	set INCLUDE=D:\Program Files\Microsoft Visual Studio 8\VC\ce\include;D:\Program Files\Windows Mobile 6 SDK\PocketPC\include\ARMV4I;D:\Program Files\Microsoft Visual Studio 8\VC\ce\atlmfc\include;D:\Program Files\Microsoft Visual Studio 8\SmartDevices\SDK\SQL Server\Mobile\v3.0
	set LIB=D:\Program Files\Windows Mobile 6 SDK\PocketPC\Lib\Armv4i;D:\Program Files\Microsoft Visual Studio 8\VC\ce\atlmfc\lib\ARMV4I;D:\Program Files\Microsoft Visual Studio 8\VC\ce\lib\ARMV4I\;
	-------------------------------------------------------------------------------------------------------------

    6) perl config.pl ����
    7) nmake ���� (���尡 ���� ������ .vcproj �� �̿��ؼ� ���� �� �� ����)
    8) OpelSSL ������ �̵� �� perl configure VC-CE �Է�

	9) ms\do_ms.bat ����
	  ** �̸� WEHH ���� ���� ������ �Ϸ�� ������ ����ص�.
		openssl-1.0.1e\ms\20130214 - ce.mak
		openssl-1.0.1e\ms\20130214 - cedll.mak

	  A) ce.mak, cedll.mak ���ϳ��� Define�� �߰��� ���� �� ������ �� ��.

		/WX �ɼ��� ����, -DOPENSSL_USE_IPV6=0 <-- IPv6 �� ������� ����
		
		/* DOPENSSL_USE_IPV6=0 ���� �������� ���� ���, �Ʒ��� ���� ���� �߻� */
		d:\WM653WEHH\PLATFORM\MVV210\SRC\OpenSSL\openssl-1.0.1e\crypto\bio\b_sock.c(195)
			 : warning C4013: 'getservbyname' undefined; assuming extern returning int
			d:\WM653WEHH\PLATFORM\MVV210\SRC\OpenSSL\openssl-1.0.1e\crypto\bio\b_sock.c(195)
			 : warning C4047: '=' : 'servent *' differs in levels of indirection from 'int'
			d:\WM653WEHH\PLATFORM\MVV210\SRC\OpenSSL\openssl-1.0.1e\crypto\bio\b_sock.c(629)
			 : error C2079: 'sa_in6' uses undefined struct 'sockaddr_in6'
			d:\WM653WEHH\PLATFORM\MVV210\SRC\OpenSSL\openssl-1.0.1e\crypto\bio\b_sock.c(776)
			 : error C2224: left of '.sin6_addr' must have struct/union type
			d:\WM653WEHH\PLATFORM\MVV210\SRC\OpenSSL\openssl-1.0.1e\crypto\bio\b_sock.c(776)
			 : error C2224: left of '.sin6_addr' must have struct/union type
			d:\WM653WEHH\PLATFORM\MVV210\SRC\OpenSSL\openssl-1.0.1e\crypto\bio\b_sock.c(776)
			 : error C2168: 'memset' : too few actual parameters for intrinsic function
			d:\WM653WEHH\PLATFORM\MVV210\SRC\OpenSSL\openssl-1.0.1e\crypto\bio\b_sock.c(777)
			 : error C2224: left of '.sin6_addr' must have struct/union type
			d:\WM653WEHH\PLATFORM\MVV210\SRC\OpenSSL\openssl-1.0.1e\crypto\bio\b_sock.c(861)
			 : error C2079: 'sa_in6' uses undefined struct 'sockaddr_in6'
			NMAKE : fatal error U1077: 'D:\WM653WEHH\sdk\bin\i386\ARM\cl.EXE' : return code
			'0x2'
			Stop.

	  B) windows.h ���� winsock.h �� winsock2.h ���� �Լ��� �ߺ����� ����Ǿ� ������ �߻��� �Ǵ� ���� �������� ����.
		(1.0.1e ���� ���� ���)

		** �� ������� �ذ��϶�.
		   windows.h ��� �տ��� winsdock2.h �� �������־��, �ߺ� ���� ���� ������ �߻����� �ʴ´�.
		   ���� ��� ���α׷����� �ϴ� ������� �� �˴���...
			// ----------------------------------------------------------------------------------
			// eastsky 2013-02-14
			// ----------------------------------------------------------------------------------
			#include <winsock2.h>
			// ----------------------------------------------------------------------------------
			#include <windows.h>

			"D:\DEV_BOX\Backup\Wireless\Wlan\UTIL\OpenSSL\openssl-1.0.1e\e_os.h"(285,6):#    include <windows.h>
			"D:\DEV_BOX\Backup\Wireless\Wlan\UTIL\OpenSSL\openssl-1.0.1e\crypto\rand\rand.h"(67,2):#include <windows.h>
			"D:\DEV_BOX\Backup\Wireless\Wlan\UTIL\OpenSSL\openssl-1.0.1e\engines\vendor_defns\cswift.h"(160,6):    #include <windows.h>
			"D:\DEV_BOX\Backup\Wireless\Wlan\UTIL\OpenSSL\openssl-1.0.1e\inc32\openssl\rand.h"(67,2):#include <windows.h>
			"D:\DEV_BOX\Backup\Wireless\Wlan\UTIL\OpenSSL\openssl-1.0.1e\tmp32dll_ARMV4I\e_os.h"(285,6):#    include <windows.h>

		** �� ����� �״� ���� ���� ���
			D:\WM653WEHH\PUBLIC\WPC\cesysgen\sdk\inc\windows.h
			// eastsky 2013-02-13
			#ifdef OPENSSL_SYSNAME_WINCE
				#ifndef _WINSOCK2API_
					 #define _WINSOCK2API_
				#endif
			#endif
		
	  C) _access �Լ� ���� ���� (1.0.1 ���� ���� �߻�)

		A. openssl-1.0.1e\apps\ca.c (Line 871)
		   �Ʒ��� ���� WinCE ������ io.h ���� _access �Լ��� �������� �����Ƿ�, wcecompat.lib ���� ������ �Լ��� ����ϵ��� ����.
			// eastsky 2013-02-14
			if (_wceaccess(outdir,R_OK|W_OK|X_OK) != 0)
			//if (_access(outdir,R_OK|W_OK|X_OK) != 0)
		
		B. openssl-1.0.1e\e_os.h (Line 317)
		   �ܼ����α׷� ����� ���Ե� __iob_func �Լ��� ���ǵ� lib �� ��� ������ �߻��ϹǷ�, �ּ�ó�� �� ��Ÿ �������� CE ������ ����.
		    /*
			#        undef stdin
			#        undef stdout
			#        undef stderr
					 FILE *__iob_func();
			#        define stdin  (&__iob_func()[0])
			#        define stdout (&__iob_func()[1])
			#        define stderr (&__iob_func()[2])
			*/
			#     include <stdlib.h>	

	10) nmake ms\ce.mak ����    (static library) ����� out32_ARMV4I �� ����
		- OpenSSL\bat\buildCE.bat �����ϸ� ��.

	11) nmake ms\cedll.mak ���� (static library) ����� out32_ARMV4I �� ����
		- OpenSSL\bat\buildCEdll.bat �����ϸ� ��.


