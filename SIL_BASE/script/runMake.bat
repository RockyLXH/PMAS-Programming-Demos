setlocal
cd /d %~dp0

set PATH=%PATH%;%MDS_HOME%cygwin\bin;%MDS_HOME%crosstool\gcc-4.1.0-glibc-2.3.6\powerpc-603e-linux-gnu\bin;%MDS_HOME%crosstool\gcc-linaro-arm-linux-gnu\bin;%MDS_HOME%crosstool\gcc-linaro-arm-linux-gnu\arm-linux-gnueabihf\libc\usr\include;%MDS_HOME%crosstool\gcc-linaro-arm-linux-gnu\arm-linux-gnueabihf\include\c++\4.7.3;%SIL_WORKSPACE%;%SIL_WORKSPACE%ONC_Server;%MDS_HOME%Maestro\includes;%MDS_HOME%Maestro\includes\CPP;../;%MDS_HOME%Maestro\includes\embl;%MDS_HOME%cygwin\tmp

make clean

make

 ::if copmilation failed exit
 IF %ERRORLEVEL% NEQ 0 (
	pause
	EXIT
)
 

powershell.exe  -ExecutionPolicy Bypass -File  "download_file.ps1"

exit