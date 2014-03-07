
REM Batch file to flash the target with os/wlan/dsets.
REM To load applications to the AR6K RAM and execute, follow the steps below.
REM Before loading the driver add a registry key 
REM 'HKLM\Comm\AR6K1\parms\defaultApp' for SDIO or  
REM 'HKLM\Comm\PCCARD\ATHR...\parms\defaultApp' for CF  
REM and set the registry value to 0

REM Target Setup
bmiloader.exe -i AR6K1 -w -a 0x80009000 -f ecos.ram.bin
REM Change Access time for 1.8V Flash. Uncomment the next line for 1.8V flash only.
REM bmiloader.exe -i AR6K1 -s -a 0xa0012a68 -p 0x920100d1
bmiloader.exe -i AR6K1 -w -a 0x80002000 -f flash.ram.bin
bmiloader.exe -i AR6K1 -s -a 0xac0140c0 -p 0x9
bmiloader.exe -i AR6K1 -b -a 0xa0009000
REM Update OS
sleep 10
flashloader.exe -i AR6K1 -w -a 0xa2000000 -f ecos.flash.bin
REM Update WLAN
flashloader.exe -i AR6K1 -w -a 0xa2010000 -f athwlan.flash.bin
REM Update Dsets
flashloader.exe -i AR6K1 -e -a 0xa2040000 -l 256
flashloader.exe -i AR6K1 -w -D 0 -a 0xa2040000
flashloader.exe -i AR6K1 -w -D 2 -a 0xa2041000 -f regulatoryData_AG.bin
flashloader.exe -i AR6K1 -w -D 7 -a 0xa2042000 -f AR6K40M.bin

REM After flashing AR6K, unload the driver, 
REM set the defaultApp registry key to 1 or remove the defaultApp registry key
REM and reload the driver to execute from the udpated flash.
