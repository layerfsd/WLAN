
--------------
SoftAP Manager
--------------
Its a MFC-GUI application to configure the SoftAP features on Windows Mobile.

------------------------
Development Environment
------------------------
MS Visual Studio 2005/2008 with VC++ Smart Device programming enabled
Windows Mobile 5.0/6.0 Pocket PC SDK(ARMV4I)
Windows Mobile 6.5 OS running on SMDK 6400

------------
Build Steps
------------
1) Open the project "SoftAP.sln" by selecting "File->Open->Project/Solution" in visual studio.
2) Make sure Windows Mobile SDK is installed.
3) Add the "Olca" include directories by selecting
"Tools->Options->Projects and Solutions->VC++ Directories->" then
select the appropriate "Platform(i.e SDK)", select "Include files" from 
"show directories for" and add the following Olca directories

\olca\host\os\wince\include
\olca\host\include
\olca\include

4) Press F7 or Select "Build->Build Solution" to build the project.
5) After successful build, "SoftAP.exe" will be created at 
"\SoftAP\SoftAP\Windows Mobile 5.0 Pocket PC SDK (ARMV4I)\Release"
directory, copy it to the device and start using it.



