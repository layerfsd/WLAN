// ----------------------------------------------------------------------------------
// Windows Mobile (WEHH)에서 openssl 을 컴파일 하기 위한 사용 설명서
//
// 작성자 : 함동호
// 작성일 : 2013-02-14
// ----------------------------------------------------------------------------------

1. OpenSSL source 다운로드
     http://www.openssl.org/source

2. Perl 다운로드
     http://downloads.activestate.com/ActivePerl/Windows/ (최신버전 알아서 선택)

3. wcecompat 다운로드
     http://github.com/mauricek/wcecompat
    
4. Building & Path
    ** 환경설정 
	 - WEHH Base OS 빌드 스크립터 실행.
	 - OpenSSL\bat\SetOpenSSLenv.bat 을 실행.

    1) VS 와 Active Perl 설치
    2) OpenSSL 압축 풀기
    3) 시작 -> 모든 프로그램 -> Microsoft Visual Studio 2005 -> Visual Studio Tools -> Visual Studio 2005 명령 프롬프트 선택
    4) wcecompat 폴더로 이동
    5) 설정 값 입력 (Program file 패스 지정, Win CE 일경우 해당 SDK 폴더 지정, 현재는 Win Mobile 6 기준임)

	-------------------------------------------------------------------------------------------------------------
	| MS 사이트에서 나온 예시.  (우리는 OpenSSL\bat\SetOpenSSLenv.bat 을 실행하면 설정 됨)
	-------------------------------------------------------------------------------------------------------------
 	set wcecompat=해당 디렉토리
	set OSVERSION=WCE502
	set TARGETCPU=ARMV4I
	set PLATFORM=VC-CE
	set PATH=D:\Program Files\Microsoft Visual Studio 8\VC\ce\bin\x86_arm;D:\Program Files\Microsoft Visual Studio 8\VC\bin;D:\Program Files\Microsoft Visual Studio 8\VC\PlatformSDK\bin;D:\Program Files\Microsoft Visual Studio 8\Common7\Tools;D:\Program Files\Microsoft Visual Studio 8\Common7\IDE;D:\Program Files\Microsoft Visual Studio 8\Common7\Tools;D:\Program Files\Microsoft Visual Studio 8\Common7\IDE;D:\Program Files\Microsoft Visual Studio 8\;$(FrameworkSDKDir)Bin;%PATH%
	set INCLUDE=D:\Program Files\Microsoft Visual Studio 8\VC\ce\include;D:\Program Files\Windows Mobile 6 SDK\PocketPC\include\ARMV4I;D:\Program Files\Microsoft Visual Studio 8\VC\ce\atlmfc\include;D:\Program Files\Microsoft Visual Studio 8\SmartDevices\SDK\SQL Server\Mobile\v3.0
	set LIB=D:\Program Files\Windows Mobile 6 SDK\PocketPC\Lib\Armv4i;D:\Program Files\Microsoft Visual Studio 8\VC\ce\atlmfc\lib\ARMV4I;D:\Program Files\Microsoft Visual Studio 8\VC\ce\lib\ARMV4I\;
	-------------------------------------------------------------------------------------------------------------

    6) perl config.pl 실행
    7) nmake 실행 (빌드가 되지 않으면 .vcproj 를 이용해서 빌드 할 수 있음)
    8) OpelSSL 폴더로 이동 후 perl configure VC-CE 입력

	9) ms\do_ms.bat 실행
	  ** 미리 WEHH 에서 빌드 검증이 완료된 파일을 백업해둠.
		openssl-1.0.1e\ms\20130214 - ce.mak
		openssl-1.0.1e\ms\20130214 - cedll.mak

	  A) ce.mak, cedll.mak 파일내에 Define을 추가로 선언 및 삭제해 줄 것.

		/WX 옵션은 제거, -DOPENSSL_USE_IPV6=0 <-- IPv6 을 사용하지 않음
		
		/* DOPENSSL_USE_IPV6=0 으로 설정하지 않을 경우, 아래와 같이 에러 발생 */
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

	  B) windows.h 에서 winsock.h 와 winsock2.h 내에 함수가 중복으로 선언되어 에러가 발생이 되는 것을 막기위해 선언.
		(1.0.1e 이하 버젼 모두)

		** 이 방법으로 해결하라.
		   windows.h 헤더 앞에서 winsdock2.h 을 선언해주어야, 중복 선언에 의해 에러가 발생되지 않는다.
		   서버 통신 프로그래밍을 하는 사람들은 잘 알더만...
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

		** 이 방법은 그닥 좋지 않은 방법
			D:\WM653WEHH\PUBLIC\WPC\cesysgen\sdk\inc\windows.h
			// eastsky 2013-02-13
			#ifdef OPENSSL_SYSNAME_WINCE
				#ifndef _WINSOCK2API_
					 #define _WINSOCK2API_
				#endif
			#endif
		
	  C) _access 함수 참조 에러 (1.0.1 버전 에서 발생)

		A. openssl-1.0.1e\apps\ca.c (Line 871)
		   아래와 같이 WinCE 에서는 io.h 에서 _access 함수를 지원하지 않으므로, wcecompat.lib 에서 생성한 함수를 사용하도록 수정.
			// eastsky 2013-02-14
			if (_wceaccess(outdir,R_OK|W_OK|X_OK) != 0)
			//if (_access(outdir,R_OK|W_OK|X_OK) != 0)
		
		B. openssl-1.0.1e\e_os.h (Line 317)
		   콘솔프로그램 빌드시 포함된 __iob_func 함수의 정의된 lib 가 없어서 에러가 발생하므로, 주석처리 및 기타 나머지는 CE 용으로 참조.
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

	10) nmake ms\ce.mak 실행    (static library) 결과는 out32_ARMV4I 에 생김
		- OpenSSL\bat\buildCE.bat 실행하면 됨.

	11) nmake ms\cedll.mak 실행 (static library) 결과는 out32_ARMV4I 에 생김
		- OpenSSL\bat\buildCEdll.bat 실행하면 됨.


