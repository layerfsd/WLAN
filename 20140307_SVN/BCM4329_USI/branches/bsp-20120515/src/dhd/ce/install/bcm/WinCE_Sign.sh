#!/bin/sh
#
# Script to sign WinCE objects from certificate that keep changing
#
# $Id: WinCE_Sign.sh,v 1.1.4.1 2010/04/03 03:44:43 Exp $

signscript=$0
signobjects=$*
signhost=`hostname`
signtime=`date '+%Y/%m/%d %H:%M:%S'`

signtool=z:/projects/hnd/tools/win/msdev/WinCE-Cert/signtool.exe
signcert=z:/projects/hnd/tools/win/msdev/WinCE-Cert/SDKSamplePrivDeveloper.pfx
signpass=brcm
signscriptdir=`dirname $0`


if [ ! -s "$signtool" ]; then
	$signtool="$signscriptdir/signtool.exe"
fi

if [ ! -s "$signcert" ]; then
	$signcert="$signscriptdir/SDKSamplePrivDeveloper.pfx"
fi


echo "[$signhost $signtime] $signtool sign /p $signpass /v /f $signcert $signobjects"
$signtool sign /p $signpass /v /f $signcert $signobjects
