@echo off
REM ======================================================================
REM First set all the variables that contain common bits for our build
REM environment.
REM * * * * * * * * * * * * * PLEASE READ* * * * * * * * * * * * * * * *
REM Microsoft Visual Studio will choke on variables that contain strings 
REM exceeding 255 chars, so be careful when editing this file! It's 
REM totally lame but there's nothing we can do about it.
REM ======================================================================

REM Where do the completed externals live?
if A%EXTERN_DIR%==A  set EXTERN_DIR=%cd%\..\externals
set EXT_INSTALL=%EXTERN_DIR%\install
set EXT_TRIGGERS=%EXTERN_DIR%\triggers

REM Specify which versions of the externals we're using. To add a 
REM new external, just add its version here, and add that to the 
REM EXTERNALS_NEEDED variable defined below.
set EXT_GSOAP_VERSION=gsoap-2.6
set EXT_KERBEROS_VERSION=
set EXT_GLOBUS_VERSION=

REM Now tell the build system what externals we need built.
set EXTERNALS_NEEDED=%EXT_GSOAP_VERSION% %EXT_KERBEROS_VERSION% %EXT_GLOBUS_VERSION%

REM Put NTConfig in the PATH, since it's got lots of stuff we need
REM like awk, gunzip, tar, bison, yacc...
set PATH=%cd%;%SystemRoot%;%SystemRoot%\system32;C:\Perl\bin;"C:\Program Files\Microsoft Visual Studio\VC98\bin";"C:\Program Files\Microsoft SDK";
call vcvars32.bat
if not defined INCLUDE ( echo . && echo *** Failed to run VCVARS32.BAT! Is Microsoft Visual Studio 6.0 installed? && exit /B 1 )
call setenv.bat /2000 /RETAIL
if not defined MSSDK ( echo . && echo *** Failed to run SETENV.BAT! Is Microsoft Platform SDK installed? && exit /B 1 )

REM Set up some stuff for BISON
set BISON_SIMPLE=%cd%\bison.simple
set BISON_HAIRY=%cd%\bison.hairy

REM Tell the build system where we can find soapcpp2
set SOAPCPP2=%EXT_INSTALL%\%EXT_GSOAP_VERSION%\soapcpp2.exe

set CONDOR_INCLUDE=/I "..\src\h" /I "..\src\condor_includes" /I "..\src\condor_c++_util" /I "..\src\condor_daemon_client" /I "..\src\condor_daemon_core.V6" 
set CONDOR_LIB=Crypt32.lib mpr.lib psapi.lib mswsock.lib netapi32.lib imagehlp.lib advapi32.lib ws2_32.lib user32.lib
set CONDOR_LIBPATH=

REM ======================================================================
REM Now set the individual variables specific to each external package.
REM Some have been defined, but are not in use yet.
REM ======================================================================

REM ** GSOAP
set CONDOR_GSOAP_INCLUDE=
set CONDOR_GSOAP_LIB=
set CONDOR_GSOAP_LIBPATH=

REM ** GLOBUS
set CONDOR_GLOBUS_INCLUDE=
set CONDOR_GLOBUS_LIB=
set CONDOR_GLOBUS_LIBPATH=

REM ** KERBEROS
set CONDOR_KERB_INCLUDE=
set CONDOR_KERB_LIB=
set CONDOR_KERB_LIBPATH=

REM ** PCRE
set CONDOR_PCRE_INCLUDE=
set CONDOR_PCRE_LIB=
set CONDOR_PCRE_LIBPATH=

