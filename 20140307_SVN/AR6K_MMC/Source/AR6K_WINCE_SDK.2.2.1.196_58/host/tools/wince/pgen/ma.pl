#!perl
#   June 2005 - wmm@kiwin.com

####################################################################
#
# Copyright (C) 2005 Stephen [kiwin] PALM
#
# License is granted to Wi-Fi Alliance members and designated
# contractors for exclusive use in testing of Wi-Fi equipment.
# This license is not transferable and does not extend to non-Wi-Fi
# applications.
#
# Derivative works are authorized and are limited by the
# same restrictions.
#
# Derivatives in binary form must reproduce the
# above copyright notice, the name of the author "Stephen [kiwin] Palm",
# this list of conditions and the following disclaimer in the documentation
# and/or other materials provided with the distribution.
#
# The name of the author may not be used to endorse or promote
# products derived from this software without specific prior
# written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
# OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
# GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
# IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
# OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
# IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
####################################################################

require "ctime.pl";

$mac_sta = 'zz';
$mac_con  = 'zz';
$seqtest = 0;
$arpup = 0;
$arp = 0;
$beacon_found= 0;
$tsf_start = 0;

$trig_vo=0; 
$trig_vi=0; 
$trig_be=0; 
$trig_bk=0; 
$triggers_known = 0;
$finished = 0;


$seqFile = $ARGV[1];

# set the input file name...
$filename_in  = $ARGV[0];
if( (length $seqFile) > 1)
 {
     $filename_out = $filename_in.$seqFile.".html";
 }
else
 {
     $filename_out = $filename_in.".html"; 
 }


open( OUT, ">$filename_out" ) || die "Can't open file: $filename_out\n";


printf OUT "<HTML>\n";
printf OUT "<HEAD><TITLE>$filename_out</title></head>\n";
printf OUT "<BODY bgcolor=white>\n";
printf OUT "<H1>$filename_out</H1><P><HR><P>\n";

printf OUT "This is an experimental page. ";
printf OUT "Questions and comments about this page/filter should be sent to: ";
printf OUT "<a href=\"mailto: wmm\@kiwin.com\">Steve Palm (wmm\@kiwin.com)</a> <P> \n";
printf OUT "This page was generated at %s <P>\n", &ctime(time);


printf OUT "<P><HR width = 50%%><P>\n";

printf OUT "<P>\n";


printf OUT "<Table border=1>\n";
printf OUT "<TR><TH>#<TH>tsf<TH>Type<TH>Retry<TH>PM<TH>More<TH>Seq<TH>SA<TH>DA<TH>UP sp<TH>text<TH>CMD<TH>#</TR>\n";


open( IN_FILE, $filename_in ) || die "Can't open file: $filename_in\n";

#Read the doc list
while (<IN_FILE>) {

  # Truncate the 8th bits
  tr/\200-\377/\000-\177/;

  chop;
  
  $line = $_;
  if ( $line =~ /fcs/ )                           { &get_header_fcs; }
  if ( $line =~ /raw/ )                           { &get_header_raw; }
  if ( $line =~ /moreFrag/ )                      { &get_header_mrf; }
  if ( $line =~ /Seq Num/ )                       { &get_header_dur; }
  if ( $line =~ /Qos AckPolicy/ )                 { &get_header_qos; }
  if ( $line =~ /SA: / )                          { &get_header_add; }
  if ( $line =~ /BeaconInt/ )                     { &get_header_bea; }
  if ( $line =~ /dtimCount/ )                     { &get_header_dtim; }
  if ( $line =~ /Partial/ )                       { &get_header_pvb; }
  if ( $line =~ /Param Set Count/ )               { &get_header_psc; }
  if ( $line =~ /Data Length/ )                   { &get_header_len; }
  if ( $line =~ /Load/ )                          { &get_apts_cmd; }
  if ( $line =~ /APTS/ )                          { &get_apts_cmd; }
  if ( $line =~ /[ABLM]\.[0-9a-zA-Z]/ )           { &get_apts_cmd; }
  if ( $line =~ /[0-9a-f]{4} [0-9a-f]{4}/ )       { &get_raw; }
  if ( $line =~ /L.1:/ )                          { &get_l1; }

  if ( $finished ) { last; }

} # while

if ( !$finished ) {
        if( $test =~ /L_1/ )
	    {
		print "\nL.1 Test: no checking on sequence file, nor STOP command.\n";
	    }
	else
	{
            printf OUT "<font size=+4>FAILURE ( Incomplete )</font>";
            print "\nFAILURE!!! (Incomplete)\n";
	}
}

printf OUT "</Table>\n";
close( IN_FILE );

# Trailer

printf OUT "<HR width=50%><P>\n";

printf OUT "<P>\n";
printf OUT "This page is automatically generated by a <A href=http://www.perl.com>PERL</A> \n";
printf OUT "<A href=\"t1e1.pl\">script</a>.<BR> ";

printf OUT "</body></html>\n";

exit;


sub get_header_fcs
{
    #printf OUT $mac_ap." ".$bssid." ".$beacon_found;
      if ( $type =~ /Probe/ || $type =~ /Ack/ || $type =~ /CTS/ || ( ($type =~ /Beacon/) && ($mac_ap ne $bssid ) && $beacon_found ) ) {
         #printf OUT "<tr><td>SKIP".$beacon_found."\n";
         #printf OUT $fcsbuffer;
         goto SKIPLINE;
      }
      else {
         printf OUT $fcsbuffer;
      }

      # Check if (previous) frame is in the sequence

       if ( $seqtest ) {
          ($snum, $stype, $ssa, $scmd, $seosp, $smgno) = split( ',', $seq[$seqno] );

          if (    ($ssa =~ /AP/ )  && ($bssid   eq $sa) ){ $seqmatcha=1;  }
          elsif ( ($ssa =~ /STA/ ) && ($mac_sta eq $sa) ){ $seqmatcha=1;  }
          elsif ( ($ssa =~ /Con/ ) && ($mac_con eq $sa) ){ $seqmatcha=1;  }
          else { $seqmatcha = 0;  }

          if ( ($type =~ /Beacon/) && ($type eq $stype) && $seqmatcha  ) {
             printf OUT "<TD> <TD> <TD> \n";
             $seqmatch = 1;
             #printf OUT " SEQ-A ";
          }
          elsif ( ($type =~ /PS Poll/) && ($type eq $stype) && $seqmatcha  ) {
             printf OUT "<TD> <TD> <TD> \n";
             $seqmatch = 1;
             #printf OUT " SEQ-B ";
          }
          elsif ( ($type =~ /QoS Null/) && ($type eq $stype) && ($eosp == ($seosp%2)) && $seqmatcha  ) {
             printf OUT "<TD> <TD> <TD> \n";
             $seqmatch = 1;
             #printf OUT " SEQ-C ";
          }
          elsif ( ($type =~ /Data/) && ($type eq $stype) && ($cmd eq $scmd) && $seqmatcha && !$arp ) {
             $seqmatch = 1;
             #printf OUT " SEQ-D ";
             $eosppending = 0;
          }
          elsif ( ($type =~ /QoS Data/) && ($type eq $stype) && ($eosp == ($seosp%2)) && ($cmd eq $scmd) && $seqmatcha && !$arp ) {
             $seqmatch = 1;
             #printf OUT " SEQ-E ";
             $eosppending = 0;
          }
          elsif ( ($type =~ /QoS Data/) && ($type eq $stype) && ($cmd eq $scmd) && $seqmatcha && !$arpup) {
             $seqmatch = 1;
             #printf OUT " SEQ-F ";
             if ( ( $up==7 && $trig_vo ) ||    # Really should be looking at "Delivery" but trig & delv are same for now
                  ( $up==6 && $trig_vo ) ||
                  ( $up==5 && $trig_vi ) ||
                  ( $up==4 && $trig_vi ) ||
                  ( $up==3 && $trig_be ) ||
                  ( $up==2 && $trig_bk ) ||
                  ( $up==1 && $trig_bk ) ||
                  ( $up==0 && $trig_be )    ) {
               $eosppending = 1; 
               printf OUT " eospPending ";
             }
	     else {
               $eosppending = 0; 
	     }
          }
          elsif ( ($type =~ /PS Poll/)  && ($type eq $stype) && $seqmatcha && !$arpup) {
             $seqmatch = 1;
             #printf OUT " SEQ-G ";
             $eosppending = 0;
             printf OUT "<TD> <TD> <TD> \n";
          }
          elsif ( $eosp && $eosppending  &&  ($type =~ /QoS Null/) ) {
             printf OUT " <TD> <TD> <TD><font color=green> EndOfSP2</font>\n";
             $seqmatch = 0;
          }
          elsif ( ($type =~ /QoS Data/) && ($type eq $stype)  && $seqmatcha  ) {
             printf OUT "<TD> <TD> <TD> \n";
          }
          else {
             printf OUT "<TD> <TD> <TD> \n";
          }

          if ( $eosp && $eosppending  && $arp ) {
             printf OUT "<TD><font color=green>$snum 1$scmd $ssa s#=$smgno EndOfSP1</font>\n";
             if ( $retry ) { 
                printf OUT " <font color=orange>Retry</font>";
             }
             $seqmatch = 0;
             $eosppending = 0;
          }
          elsif ( $seqmatch && ($cmd =~ /APTS/)  && $eosppending ) {
             printf OUT "<TD><font color=orange>$snum $stype $scmd $ssa s#=$smgno EOSP pending1</font>\n";
             if ( $retry ) { 
                printf OUT " <font color=orange>Retry</font>";
             }
             if ( ($cmd =~ /STOP/) && ($scmd =~ /STOP/ ) ) {
                printf OUT "<font size=+4>SUCCESS</font>";
                print "\nSUCCESS!!! \n";
                $finished = 1;
             }   
             $seqno++;  printf OUT " sq++1 "; 
             $seqmatch = 0;
             $eosp = 0;
          }
          elsif ( $seqmatch && ($cmd =~ /APTS/) && !$arpup ) {
             printf OUT "<TD><font color=green>$snum $stype $scmd $ssa s#=$smgno</font>\n";
             if ( $retry ) { 
                printf OUT " <font color=orange>Retry</font>";
             }
             if ( ($cmd =~ /STOP/) && ($scmd =~ /STOP/ ) ) {
                printf OUT "<font size=+4>SUCCESS</font>";
                print "\nSUCCESS!!! \n";
                $finished = 1;
             }   
             if ( ($cmd =~ /APTS_CK_BK/) && ($scmd =~ /APTS_CK_BK/ ) ) { # Determine which AC are trigger enabled
                # Hardcode by test number for now
                if ( $test =~ /A_J/ ) { $trig_vo=1; $trig_vi=1; $trig_be=1; $trig_bk=1; $triggers_known=1} #
                if ( $test =~ /A_U/ ) { $trig_vo=0; $trig_vi=0; $trig_be=0; $trig_bk=0; $triggers_known=1} #
                if ( $test =~ /A_Y/ ) { $trig_vo=1; $trig_vi=1; $trig_be=0; $trig_bk=0; $triggers_known=1} #
                if ( $test =~ /B_B/ ) { $trig_vo=1; $trig_vi=1; $trig_be=1; $trig_bk=1; $triggers_known=1} #
                if ( $test =~ /B_D/ ) { $trig_vo=1; $trig_vi=1; $trig_be=1; $trig_bk=1; $triggers_known=1} #
                if ( $test =~ /B_G/ ) { $trig_vo=1; $trig_vi=1; $trig_be=1; $trig_bk=1; $triggers_known=1} #
                if ( $test =~ /B_M/ ) { $trig_vo=1; $trig_vi=1; $trig_be=1; $trig_bk=1; $triggers_known=1} #
                if ( $test =~ /B_Z/ ) { $trig_vo=1; $trig_vi=1; $trig_be=1; $trig_bk=1; $triggers_known=1} #
                if ( $test =~ /L_1/ ) { $trig_vo=1; $trig_vi=1; $trig_be=1; $trig_bk=1; $triggers_known=1} #
                if ( $test =~ /M_B/ ) { $trig_vo=1; $trig_vi=1; $trig_be=0; $trig_bk=0; $triggers_known=1} #
                if ( $test =~ /M_D/ ) { $trig_vo=0; $trig_vi=0; $trig_be=0; $trig_bk=0; $triggers_known=1} #
                if ( $test =~ /M_G/ ) { $trig_vo=0; $trig_vi=1; $trig_be=0; $trig_bk=0; $triggers_known=1} #
                if ( $test =~ /M_I/ ) { $trig_vo=0; $trig_vi=1; $trig_be=0; $trig_bk=0; $triggers_known=1} #
                if ( $test =~ /M_K/ ) { $trig_vo=1; $trig_vi=1; $trig_be=0; $trig_bk=0; $triggers_known=1} #
                if ( $test =~ /M_L/ ) { $trig_vo=0; $trig_vi=0; $trig_be=0; $trig_bk=0; $triggers_known=1} #
                if ( $test =~ /M_U/ ) { $trig_vo=0; $trig_vi=0; $trig_be=0; $trig_bk=0; $triggers_known=1} #
                if ( $test =~ /M_V/ ) { $trig_vo=1; $trig_vi=1; $trig_be=0; $trig_bk=0; $triggers_known=1} #
                if ( $test =~ /M_W/ ) { $trig_vo=1; $trig_vi=1; $trig_be=0; $trig_bk=0; $triggers_known=1} #
                if ( $test =~ /M_Y/ ) { $trig_vo=1; $trig_vi=1; $trig_be=0; $trig_bk=0; $triggers_known=1} #

		printf OUT "<BR> trig_vo=$trig_vo trig_vi=$trig_vi trig_be=$trig_be trig_bk=$trig_bk \n";
	     }
             $seqno++;   printf OUT " sq++2 "; 
             $seqmatch = 0;
             $eosp = 0;
          }
          #elsif ( ($type =~ /QoS Data/) && $arpup  && $seqmatcha  ) {
          #   #$seqno--;
          #   ($snum, $stype, $ssa, $scmd, $seosp, $smgno) = split( ',', $seq[$seqno] );
          #   printf OUT "<TD> <TD> <TD> <font color=green>$snum 3$scmd $sss s#=$smgno</font>\n";;
          #   $arpup = 0;
          #}
          elsif ( $seqmatch ) {  # Qos Null & Exchange command
             printf OUT "<TD><font color=green>$snum $stype $scmd $ssa s#=$smgno</font>\n";
             if ( $retry ) { 
                printf OUT " <font color=orange>Retry</font>";
             }
             $seqno++;    printf OUT " sq++3 "; 
             $seqmatch = 0;
             $eosp = 0;
          }
          elsif ( $arpup ) {
            printf OUT "<TD><font color=orange>Upstream ARP</font>\n";
            $arpup = 0;
          }             
          elsif ( $eosppending ) {
             printf OUT "<TD><font color=orange>$snum $stype $scmd $ssa s#=$smgno EOSP pending2</font>\n";
             if ( $retry ) { 
                printf OUT " <font color=orange>Retry</font>";
             }
             $seqmatch = 0;
             if ( ($cmd =~ /STOP/) && !($scmd =~ /STOP/ ) ) {
                printf OUT "<font size=+4>FAILURE</font>";
                print "\nFAILURE!!! \n";
                $finished = 1;
             }   
          }
          elsif ( $eosp ) {
             printf OUT "<TD><font color=orange>$snum $stype $scmd $ssa s#=$smgno EOSP</font>\n";
             if ( $retry ) { 
                printf OUT " <font color=orange>Retry</font>";
             }
             $seqmatch = 0;
             if ( ($cmd =~ /STOP/) && !($scmd =~ /STOP/ ) ) {
                printf OUT "<font size=+4>FAILURE</font>";
                print "\nFAILURE!!! \n";
                $finished = 1;
             }   
          }
          #Add the following to check in AY and AU test, fail test if a PS Poll is sent
          elsif ( ($type =~ /PS Poll/)  && ($sa eq $mac_sta)) {
            printf OUT "<TD><font color=red>$snum $stype $scmd $ssa s#=$smgno</font>\n";
             if ( ($test =~ /A_Y/) || ($test =~ /A_U/ ) ) {
                printf OUT "<font size=+4>PS Poll, STA misconfigured! FAILURE</font>";
                print "\nNo PS Poll in A.Y/A.U test! STA was misconfigured! FAILURE!!! \n";
                $finished = 1;
	    }
          }
          else {
            printf OUT "<TD><font color=red>$snum $stype $scmd $ssa s#=$smgno</font>\n";
             if ( ($cmd =~ /STOP/) && !($scmd =~ /STOP/ ) ) {
                printf OUT "<font size=+4>FAILURE</font>";
                print "\nFAILURE!!! \n";
                $finished = 1;
             }   

          }
       } # seqtest

SKIPLINE:

       # cleanup some status
       $cmd = "";

       # Start printing next frame
       ($p[0],$p[1],$p[2],$p[3],$p[4],$tsf,$p[6],$p[7],$p[8],$p[9]) = split('[/(/)]', $line );
       $tsf /= 1000000;
       if ( $tsf_start == 0 ) {
          $tsf_start = $tsf;
       }
       $tsf -= $tsf_start;
       $tsff = sprintf( "%9.6f", $tsf);
       $fcsbuffer="<TR>\n<TD>".$p[3]."<TD>".$tsff."\n";
}    

sub get_header_raw
{
       ($p[0],$p[1],$p[2],$p[3],$p[4],$p[5],$p[6],$p[7],$p[8],$p[9]) = split('[/(/)]', $line );
       #printf OUT "<TD>$p[1]\n"; # raw
       #printf OUT "<TD>$p[3]\n"; # Vers
       if ( $p[5] =~ /QoS Data/ ) {
         $fcsbuffer = $fcsbuffer." <TD><font color=purple>$p[5]</font>\n"; # Type
       }
       elsif ( $p[5] =~ /QoS Null/ ) {
         $fcsbuffer = $fcsbuffer." <TD><font color=blue>".$p[5]."</font>\n"; # Type
       }
       elsif ( $p[5] =~ /Data/ ) {
         $fcsbuffer = $fcsbuffer." <TD><font color=red>".$p[5]."</font>\n"; # Type
       }
       else {
         $fcsbuffer = $fcsbuffer." <TD>".$p[5]."\n"; # Type
       }
       $type =  $p[5];
}

sub get_header_mrf
{
       ($p[0],$p[1],$p[2],$p[3],$p[4],$p[5],$p[6],$p[7],$p[8],$p[9]) = split('[/(/)]', $line );
       #printf OUT "<TD>$p[1]\n"; #moreFrag
       if ( $p[3] == 1 ) {
         $retry = 1;
         $fcsbuffer = $fcsbuffer." <td><font color=orange>Retry</font>\n"; #retry
       }
       else {
         $retry = 0;
         $fcsbuffer = $fcsbuffer." <TD>".$p[3]."\n"; #retry
       }
       if ( $p[5] == 1 ) {
         $pm = 1;
         $fcsbuffer = $fcsbuffer." <TD><font color=blue>PM</font>\n"; #PM
       }
       else {
         $pm = 0;
         $fcsbuffer = $fcsbuffer." <TD>".$p[5]."\n"; #PM
       }
       if ( $p[7] == 1 ) {
         $more = 1;
         $fcsbuffer = $fcsbuffer." <TD><font color=blue>More</font>\n"; #PM
       }
       else {
         $more = 0;
         $fcsbuffer = $fcsbuffer." <TD>".$p[7]."\n"; #More
       }

       if ( $type =~ /PS Poll/ ) {
         $fcsbuffer = $fcsbuffer." <TD> ps\n"; # Extra box for PS Poll since no sequence number
      }
}

sub get_header_dur
{
       ($p[0],$p[1],$p[2],$p[3],$p[4],$p[5],$p[6],$p[7],$p[8],$p[9]) = split('[/(/)]', $line );
       #$fcsbuffer = $fcsbuffer." <TD>".$p[1]."\n"; # Dur
       $fcsbuffer = $fcsbuffer." <TD>".$p[3]."\n"; # Seq
       $pseq = $p[3];
       if ( $type =~ /QoS / && ($pseq eq $pseql) && $retry ) { $seqno--; }
       #$fcsbuffer = $fcsbuffer."<TD>".$p[5]."\n"; # Frag num#

}

sub get_header_bea
{
       ($p[0],$p[1],$p[2],$p[3],$p[4],$p[5],$p[6],$p[7],$p[8],$p[9]) = split('[/(/)]', $line );
       $fcsbuffer = $fcsbuffer." <TD> <font color=blue>".$p[7]."</font>\n"; # SSID
}

sub get_header_len
{
       ($p[0],$p[1],$p[2],$p[3],$p[4],$p[5],$p[6],$p[7],$p[8],$p[9]) = split('[/(/)]', $line );
       $length = $p[1];
       #$fcsbuffer = $fcsbuffer." <TD> ".$p[1]."\n"; # Data Length
}

sub get_header_dtim
{
       ($p[0],$p[1],$p[2],$p[3],$p[4],$p[5],$p[6],$p[7],$p[8],$p[9]) = split('[/(/)]', $line );
       $dtim  = $p[1];
       $dtimp = $p[3];
       $bmc   = $p[7];  #/ bitmapControl (broadcast only)
       if ( $dtim ) {
          $fcsbuffer = $fcsbuffer." DTIM=".$dtim."/".$dtimp."\n"; # DTIMcount
       }
       else {
          $fcsbuffer = $fcsbuffer."<font color=green>DTIM=".$dtim."/".$dtimp."</font>\n"; # DTIMcount
       }
}

sub get_header_pvb
{
       ($p[0],$p[1],$p[2],$p[3],$p[4],$p[5],$p[6],$p[7],$p[8],$p[9]) = split('[/(/)]', $line );
       $pvb = $p[1];
       $fcsbuffer = $fcsbuffer." PVB=".$pvb."\n"; # PVB - Partial Virtual Bitmap
       $fcsbuffer = $fcsbuffer." BMC=".$bmc."\n"; # BMC - bitmapControl
}

sub get_l1
{
       ($p[0],$p[1],$p[2],$p[3],$p[4],$p[5],$p[6],$p[7],$p[8],$p[9]) = split('[/(/)]', $line );
       $l1time  = $p[1];
       $l1nerr  = $p[3];
       $l1nrecv = $p[5];
       $ipaddress = $p[7];
       printf OUT "<TR><TD><TD><TD><TD><TD><TD><TD>IpAddress=$ipaddress<TD>time=$l1time<TD>nerr=$l1nerr<TD>nrecv=$l1nrecv\n";
       if ( ($l1time < 300.0) && ($l1nerr < 300) && ($l1nrecv > 2999) ) {
           printf OUT "<TD><font size=+4>SUCCESS</font>\n";
	   printf OUT "<BR>";
                print "\nFor Device using IP Address: $ipaddress SUCCESS!!! \n";
           $test = "L_1";
       }
       else {
           printf OUT "<TD><font size=+4>FAILURE</font>\n";
                print "\nFor Device using IP Address: $ipaddress FAILURE!!! \n";
       }
}

sub get_header_psc
{
       ($p[0],$p[1],$p[2],$p[3],$p[4],$p[5],$p[6],$p[7],$p[8],$p[9]) = split('[/(/)]', $line );
       #$fcsbuffer = $fcsbuffer." Ver=".$p[1]."\n"; # WME ver
       #$fcsbuffer = $fcsbuffer." PSC=".$p[5]."\n"; # Parameter Set Count
       $fcsbuffer = $fcsbuffer." U-APSD=".$p[7]."\n"; # UAPSD
}

sub get_header_qos
{
       ($p[0],$p[1],$p[2],$p[3],$p[4],$p[5],$p[6],$p[7],$p[8],$p[9]) = split('[/(/)]', $line );
       #printf OUT "<TD>eosp$p[3]\n";
       #printf OUT "<TD>UP$p[5]\n";
       $ack = $p[1];
       $eosp = $p[3];
       $up = $p[5];
}

sub get_apts_cmd
{
       ($p[0],$p[1],$p[2],$p[3],$p[4],$p[5],$p[6],$p[7],$p[8],$p[9]) = split('    ', $line );
       $fcsbuffer = $fcsbuffer."<TD>".$p[1]."\n";

       ($p[0],$p[1],$msgno_raw,$p[3],$dscp,$p[5],$cookie,$p[7],$p[8],$p[9]) = split(' ', $line3 );
       ($p[0],$p[1],$p[2],$p[3],$p[4],$p[5],$nend,$p[7],$p[8],$p[9]) = split(' ', $line2 );
       ($nup,$p[1],$ndown,$p[3],$p[4],$p[5],$cmd_raw,$p[7],$p[8],$p[9]) = split(' ', $line1 );
       $nend  =~ s/(..)(..)/$2$1/;
       $nup   =~ s/(..)(..)/$2$1/;
       $ndown =~ s/(..)(..)/$2$1/;
       $msgno_raw =~ s/(..)(..)/$1/;
       if ($msgno_raw eq "00") { $msgno=0x00; }
       elsif ($msgno_raw eq "01") { $msgno=0x01; }
       elsif ($msgno_raw eq "02") { $msgno=0x02; }
       elsif ($msgno_raw eq "03") { $msgno=0x03; }
       elsif ($msgno_raw eq "04") { $msgno=0x04; }
       elsif ($msgno_raw eq "05") { $msgno=0x05; }
       elsif ($msgno_raw eq "06") { $msgno=0x06; }
       elsif ($msgno_raw eq "07") { $msgno=0x07; }
       elsif ($msgno_raw eq "08") { $msgno=0x08; }
       elsif ($msgno_raw eq "09") { $msgno=0x09; }
       elsif ($msgno_raw eq "0a") { $msgno=0x0a; }
       elsif ($msgno_raw eq "0b") { $msgno=0x0b; }
       elsif ($msgno_raw eq "0c") { $msgno=0x0c; }
       elsif ($msgno_raw eq "0d") { $msgno=0x0d; }
       elsif ($msgno_raw eq "0e") { $msgno=0x0e; }
       elsif ($msgno_raw eq "0f") { $msgno=0x0f; }
       elsif ($msgno_raw eq "10") { $msgno=0x10; }
       elsif ($msgno_raw eq "11") { $msgno=0x11; }
       elsif ($msgno_raw eq "12") { $msgno=0x12; }
       elsif ($msgno_raw eq "13") { $msgno=0x13; }
       elsif ($msgno_raw eq "14") { $msgno=0x14; }
       elsif ($msgno_raw eq "15") { $msgno=0x15; }
       elsif ($msgno_raw eq "16") { $msgno=0x16; }
       elsif ($msgno_raw eq "17") { $msgno=0x17; }
       elsif ($msgno_raw eq "18") { $msgno=0x18; }
       elsif ($msgno_raw eq "19") { $msgno=0x19; }
       elsif ($msgno_raw eq "1a") { $msgno=0x1a; }
       elsif ($msgno_raw eq "1b") { $msgno=0x1b; }
       elsif ($msgno_raw eq "1c") { $msgno=0x1c; }
       elsif ($msgno_raw eq "1d") { $msgno=0x1d; }
       elsif ($msgno_raw eq "1e") { $msgno=0x1e; }
       elsif ($msgno_raw eq "1f") { $msgno=0x1f; }
       elsif ($msgno_raw eq "20") { $msgno=0x20; }
       elsif ($msgno_raw eq "21") { $msgno=0x21; }
       elsif ($msgno_raw eq "22") { $msgno=0x22; }
       elsif ($msgno_raw eq "23") { $msgno=0x23; }
       elsif ($msgno_raw eq "24") { $msgno=0x24; }
       elsif ($msgno_raw eq "25") { $msgno=0x25; }
       elsif ($msgno_raw eq "26") { $msgno=0x26; }
       elsif ($msgno_raw eq "27") { $msgno=0x27; }
       elsif ($msgno_raw eq "28") { $msgno=0x28; }
       elsif ($msgno_raw eq "29") { $msgno=0x29; }
       elsif ($msgno_raw eq "2a") { $msgno=0x2a; }
       elsif ($msgno_raw eq "2b") { $msgno=0x2b; }
       elsif ($msgno_raw eq "2c") { $msgno=0x2c; }
       elsif ($msgno_raw eq "2d") { $msgno=0x2d; }
       elsif ($msgno_raw eq "2e") { $msgno=0x2e; }
       elsif ($msgno_raw eq "2f") { $msgno=0x2f; }
       elsif ($msgno_raw eq "30") { $msgno=0x30; }
       elsif ($msgno_raw eq "63") { $msgno=0x63; }
       else { $msgno= -1; }
       $cmd_raw  =~ s/(..)(..)/$1/;
       if ($cmd_raw eq "00") { $cmd="NONE0"; }
       if ($cmd_raw eq "01") { $cmd="APTS_TX"; }
       if ($cmd_raw eq "02") { $cmd="APTS_HELLO"; }
       if ($cmd_raw eq "03") { $cmd="APTS_HELLO_RESP"; }
       if ($cmd_raw eq "04") { $cmd="APTS_CONFIRM"; }
       if ($cmd_raw eq "05") { $cmd="APTS_STOP"; }
       if ($cmd_raw eq "06") { $cmd="APTS_CK_BE"; }
       if ($cmd_raw eq "07") { $cmd="APTS_CK_BK"; }
       if ($cmd_raw eq "08") { $cmd="APTS_CK_VI"; }
       if ($cmd_raw eq "09") { $cmd="APTS_CK_VO"; }
       if ($cmd_raw eq "0a") { $cmd="APTS_TESTS"; }
       if ($cmd_raw eq "0b") { $cmd="B_D"; }
       if ($cmd_raw eq "0c") { $cmd="B_2"; }
       if ($cmd_raw eq "0d") { $cmd="B_H"; }
       if ($cmd_raw eq "0e") { $cmd="B_4"; }
       if ($cmd_raw eq "0f") { $cmd="B_5"; }
       if ($cmd_raw eq "10") { $cmd="B_6"; }
       if ($cmd_raw eq "11") { $cmd="B_B"; }
       if ($cmd_raw eq "12") { $cmd="B_E"; }
       if ($cmd_raw eq "13") { $cmd="B_G"; }
       if ($cmd_raw eq "14") { $cmd="B_M"; }
       if ($cmd_raw eq "15") { $cmd="M_D"; }
       if ($cmd_raw eq "16") { $cmd="M_G"; }
       if ($cmd_raw eq "17") { $cmd="M_I"; }
       if ($cmd_raw eq "18") { $cmd="B_Z"; }
       if ($cmd_raw eq "19") { $cmd="M_Y"; }
       if ($cmd_raw eq "1a") { $cmd="L_1"; }
       if ($cmd_raw eq "1b") { $cmd="DLOAD"; }
       if ($cmd_raw eq "1c") { $cmd="uLOAD"; }
       if ($cmd_raw eq "1d") { $cmd="APTS_PASS"; }
       if ($cmd_raw eq "1e") { $cmd="APTS_FAIL"; }
       if ($cmd_raw eq "1f") { $cmd="A_Y"; }
       if ($cmd_raw eq "20") { $cmd="B_W"; }
       if ($cmd_raw eq "21") { $cmd="A_J"; }
       if ($cmd_raw eq "22") { $cmd="M_V"; }
       if ($cmd_raw eq "23") { $cmd="M_U"; }
       if ($cmd_raw eq "24") { $cmd="A_U"; }
       if ($cmd_raw eq "25") { $cmd="M_L"; }
       if ($cmd_raw eq "26") { $cmd="B_K"; }
       if ($cmd_raw eq "27") { $cmd="M_B"; }
       if ($cmd_raw eq "28") { $cmd="M_K"; }
       if ($cmd_raw eq "29") { $cmd="M_W"; }
       if ($cmd_raw eq "30") { $cmd="APTS_LAST"; }
       if ($cmd_raw eq "63") { $cmd="APTS_BCST"; }
       $fcsbuffer = $fcsbuffer."<TD>".$cmd."<TD>  msgno=".$msgno." len=".$length." cookie=".$cookie."\n";
       $fcsbuffer = $fcsbuffer." dscp=".$dscp."\n";

       if (  $cmd =~ /HELLO/ )  {
          $mac_sta = $sa;
          $mac_con = $da; 
          $mac_ap  = $bssid;
          $beacon_found = 1;
          &get_text_oui;
          $fcsbuffer = $fcsbuffer."<TD>AP=".$bssid_text." STA=".$mac_sta_text." Con=".$mac_con_text."\n";
       }

       if ( $cmd =~/B_/ || $cmd =~/M_/ || $cmd =~/L_/ || $cmd =~/A_/ ) {
          $fcsbuffer = $fcsbuffer." nend=".$nend." nup=".$nup." ndown=".$ndown." \n";
          $test = $cmd;
	  #print "Test is using ".$test;

          #open sequence file
	if((length $seqFile) > 1)
	{
	$filename_seq = $seqFile;
	}
	else
        {
	$filename_seq = "apts_seq_".$cmd.".txt"; 

	printf OUT "sequence file looking at is:".$filename_seq;
	}
          open( SEQ, "$filename_seq" ) || die "Can't open file: $filename_seq\n";
          $seqtest = 1;
          $seqno = 1;
          while( <SEQ> ) {
            $seq[$seqno++] = $_;
          }
          close ( SEQ );
          $seqno = 1;
          $arpup = 0;
       }


}


sub get_raw
{
       if ( $line =~ /aaaa 0300 0000 0806/ ) { 
         $fcsbuffer = $fcsbuffer." <b><font color=red>ARP</font></b> "; 
         if    ( $sa eq $mac_con ) {
            $arp++; 
         }
         elsif ( $sa eq $mac_sta ) { 
            if( $arp ) { 
               $arp--; 
            }
            if( !$arp ) {
               if ( $test =~ /B_D/ ) { $seqno++; } # An upstream ARP is an extra trigger if arp stack is empty and test B_D
            }
            $arpup = 1;
            $fcsbuffer = $fcsbuffer."ARP-UP";

            # If B.D swap current and current+1
            # If B.I swap current and current+2  then current+1 and current+3
            if ( ($test eq "B_D" ) && ($arp == 1) ) {
               ($tnum, $ttype, $tsa, $tcmd, $teosp, $tmgno) = split( ',', $seq[$seqno+0] );
               $seq[$seqno+0] = $seq[$seqno+1];
               $seq[$seqno+1] = join (",", $tnum, $ttype, $tsa, $tcmd, $teosp, $tmgno );
               $fcsbuffer = $fcsbuffer." ARP-SWAP ";
            }
            if ( ($test eq "B_I" || $test eq "M_I") && ($arp == 1) ) {
               ($tnum, $ttype, $tsa, $tcmd, $teosp, $tmgno) = split( ',', $seq[$seqno+0] );
               $seq[$seqno+0] = $seq[$seqno+2];
               $seq[$seqno+2] = join (",", $tnum, $ttype, $tsa, $tcmd, $teosp, $tmgno );
               ($tnum, $ttype, $tsa, $tcmd, $teosp, $tmgno) = split( ',', $seq[$seqno+1] );
               $seq[$seqno+1] = $seq[$seqno+3];
               $seq[$seqno+3] = join (",", $tnum, $ttype, $tsa, $tcmd, $teosp, $tmgno );
               $fcsbuffer = $fcsbuffer." ARP-SWAP ";
            }

         }


       }
       $line3 = $line2;
       $line2 = $line1;
       $line1 = $line;


}

sub get_header_add
{
       ($p[0],$p[1],$p[2],$p[3],$p[4],$p[5],$p[6],$p[7],$p[8],$p[9]) = split('[/(/)]', $line );
       if ( $p[2] =~ /SA/ ) {
           $sa = $p[3];
       }
       elsif (  $p[4] =~ /SA/ ) {
           $sa = $p[5];
       }
       if ( $p[0] =~ /DA/ ) {
           $da = $p[1];
       }
       elsif (  $p[4] =~ /DA/ ) {
           $da = $p[5];
       }
       if ( $p[0] =~ /BSSID/ ) {
           $bssid = $p[1];
       }
       elsif (  $p[2] =~ /BSSID/ ) {
           $bssid = $p[3];
       }
       elsif (  $p[4] =~ /BSSID/ ) {
           $bssid = $p[5];
       }

       #if ( ($type =~ /Auth/) && ($da eq $bssid) ) { $mac_sta = $sa; }

       if ( $sa =~ /$mac_ap/ ) {
          $fcsbuffer = $fcsbuffer."<TD><font color=purple> &nbsp AP</font>\n";
       }
       elsif ( $sa =~ /$mac_sta/ ) {
          $fcsbuffer = $fcsbuffer."<TD><font color=blue> &nbsp STA</font>\n";
       }
       elsif ( $sa =~ /$mac_con/ ) {
          $fcsbuffer = $fcsbuffer."<TD><font color=darkgreen> &nbsp Con</font>\n";
       }
       else {
          $fcsbuffer = $fcsbuffer."<TD>".$sa."\n";
       }
       if ( $da =~ /$mac_ap/ ) {
          $fcsbuffer = $fcsbuffer."<TD><font color=purple> &nbsp AP</font>\n";
       }
       elsif ( $da =~ /$mac_sta/ ) {
          $fcsbuffer = $fcsbuffer."<TD><font color=blue> &nbsp STA</font>\n";
       }
       elsif ( $da =~ /$mac_con/ ) {
          $fcsbuffer = $fcsbuffer."<TD><font color=darkgreen> &nbsp Con </font>\n";
       }
       elsif (( $da =~ /ff:ff:ff:ff:ff:ff/) && ($sa eq $bssid )) {   # Beacon
          # do not print out
       }
       else {
          $fcsbuffer = $fcsbuffer."<TD>".$da."\n";
       }


       if ( $type =~/QoS/ ) {
          $fcsbuffer = $fcsbuffer."<TD> UP=".$up." \n";
          if ( $eosp ) {
             $fcsbuffer = $fcsbuffer."<font color=green>EOSP</font>\n";
          }
          if ( ($sa eq $mac_sta ) && $pm && $triggers_known ) {
             if ( ( $up==7 && $trig_vo ) ||
                  ( $up==6 && $trig_vo ) ||
                  ( $up==5 && $trig_vi ) ||
                  ( $up==4 && $trig_vi ) ||
                  ( $up==3 && $trig_be ) ||
                  ( $up==2 && $trig_bk ) ||
                  ( $up==1 && $trig_bk ) ||
                  ( $up==0 && $trig_be )    ) {
               $fcsbuffer = $fcsbuffer."<font color=orange>trig</font>\n";
             }
	     else {
               $fcsbuffer = $fcsbuffer."<font color=orange>legacy</font>\n";
	     }
          }
       }
       else {
          $fcsbuffer = $fcsbuffer."<TD><font color=lightgray>".$type."</font>\n";
       }

}

sub get_text_oui {

  ($oui1, $oui2, $oui3, $oui4, $oui5, $oui6) = split( ":", $mac_sta );  
  $man = $oui1."-".$oui2."-".$oui3;
  $man =~ s#0-A-F7#BRCM#i;            
  $man =~ s#0-10-18#BRCM#i;            
  $man =~ s#0-90-4C#BRCMe#i;            
  $man =~ s#0-3-7F#ATHR#i;            
  $man =~ s#0-9-6B#IBM#i;            
  $man =~ s#0-d-60#IBM#i;            
  $man =~ s#0-9-E1#Gemtek#i;            
  $man =~ s#0-14-A5#Gemtek#i;            
  $man =~ s#0-90-4B#Gemtek#i;            
  $man =~ s#0-80-45#MEI#i;            
  $man =~ s#0-50-43#MRVL#i;       
  $man =~ s#0-b-DB#DELL#i;       
  $man =~ s#0-3-47#Intel#i;       
  $mac_sta_text = $man.":".$oui4.":".$oui5.":".$oui6;

  ($oui1, $oui2, $oui3, $oui4, $oui5, $oui6) = split( ":", $mac_con );  
  $man = $oui1."-".$oui2."-".$oui3;
  $man =~ s#0-A-F7#BRCM#i;            
  $man =~ s#0-10-18#BRCM#i;            
  $man =~ s#0-90-4C#BRCMe#i;            
  $man =~ s#0-3-7F#ATHR#i;            
  $man =~ s#0-9-6B#IBM#i;            
  $man =~ s#0-d-60#IBM#i;            
  $man =~ s#0-9-E1#Gemtek#i;            
  $man =~ s#0-14-A5#Gemtek#i;            
  $man =~ s#0-90-4B#Gemtek#i;            
  $man =~ s#0-80-45#MEI#i;            
  $man =~ s#0-50-43#MRVL#i;       
  $man =~ s#0-b-DB#DELL#i;       
  $man =~ s#0-3-47#Intel#i;       
  $mac_con_text = $man.":".$oui4.":".$oui5.":".$oui6;

  ($oui1, $oui2, $oui3, $oui4, $oui5, $oui6) = split( ":", $bssid );  
  $man = $oui1."-".$oui2."-".$oui3;
  $man =~ s#0-A-F7#BRCM#i;            
  $man =~ s#0-10-18#BRCM#i;            
  $man =~ s#0-3-7F#ATHR#i;            
  $man =~ s#0-9-6B#IBM#i;            
  $man =~ s#0-d-60#IBM#i;            
  $man =~ s#0-9-E1#Gemtek#i;            
  $man =~ s#0-14-A5#Gemtek#i;            
  $man =~ s#0-90-4B#Gemtek#i;            
  $man =~ s#0-80-45#MEI#i;            
  $man =~ s#0-50-43#MRVL#i;       
  $man =~ s#0-b-DB#DELL#i;       
  $man =~ s#0-3-47#Intel#i;       
  $bssid_text = $man.":".$oui4.":".$oui5.":".$oui6;

}
