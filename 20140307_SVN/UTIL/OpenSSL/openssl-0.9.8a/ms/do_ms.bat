
perl util\mkfiles.pl >MINFO
perl util\mk1mf.pl no-asm VC-WIN32 >ms\nt.mak
perl util\mk1mf.pl dll no-asm VC-WIN32 >ms\ntdll.mak
perl util\mk1mf.pl no-asm VC-CE >ms\ce.mak
perl util\mk1mf.pl dll no-asm VC-CE >ms\cedll.mak
perl util\mk1mf.pl no-asm VC-CE_PDA_CONSOLE >ms\cepdaconsole.mak
perl util\mk1mf.pl dll no-asm VC-CE_PDA_CONSOLE >ms\cepdaconsoledll.mak

perl util\mkdef.pl 32 libeay > ms\libeay32.def
perl util\mkdef.pl 32 ssleay > ms\ssleay32.def
