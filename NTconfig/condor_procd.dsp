# Microsoft Developer Studio Project File - Name="condor_procd" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=condor_procd - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "condor_procd.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "condor_procd.mak" CFG="condor_procd - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "condor_procd - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "condor_procd - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "condor_procd - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Release"
# PROP Intermediate_Dir "..\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /Z7 /O1 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /FD /TP $(CONDOR_INCLUDE) /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 ../Release/condor_common.obj ../Release/condor_common_c.obj $(CONDOR_LIB) $(CONDOR_LIBPATH) $(CONDOR_GSOAP_LIB) $(CONDOR_GSOAP_LIBPATH) $(CONDOR_KERB_LIB) $(CONDOR_KERB_LIBPATH) $(CONDOR_PCRE_LIB) $(CONDOR_PCRE_LIBPATH) $(CONDOR_GLOBUS_LIB) $(CONDOR_GLOBUS_LIBPATH) $(CONDOR_OPENSSL_LIB) $(CONDOR_POSTGRESQL_LIB) $(CONDOR_OPENSSL_LIBPATH) $(CONDOR_POSTGRESQL_LIBPATH) /nologo /subsystem:console /pdb:none /map /debug /machine:I386

!ELSEIF  "$(CFG)" == "condor_procd - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "condor_procd___Win32_Debug"
# PROP BASE Intermediate_Dir "condor_procd___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Debug"
# PROP Intermediate_Dir "..\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /Gi /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FD /GZ /TP $(CONDOR_INCLUDE) /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ../Debug/condor_common.obj ..\Debug\condor_common_c.obj $(CONDOR_LIB) $(CONDOR_LIBPATH) $(CONDOR_GSOAP_LIB) $(CONDOR_GSOAP_LIBPATH) $(CONDOR_KERB_LIB) $(CONDOR_KERB_LIBPATH) $(CONDOR_PCRE_LIB) $(CONDOR_PCRE_LIBPATH) $(CONDOR_GLOBUS_LIB) $(CONDOR_GLOBUS_LIBPATH) $(CONDOR_OPENSSL_LIB) $(CONDOR_POSTGRESQL_LIB) $(CONDOR_OPENSSL_LIBPATH) $(CONDOR_POSTGRESQL_LIBPATH) /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "condor_procd - Win32 Release"
# Name "condor_procd - Win32 Debug"
# Begin Source File

SOURCE="..\src\condor_c++_util\condor_open.C"
# End Source File
# Begin Source File

SOURCE=..\src\condor_util_lib\condor_pidenvid.c
# End Source File
# Begin Source File

SOURCE=..\src\condor_procd\dprintf_lite.C
# End Source File
# Begin Source File

SOURCE=..\src\condor_procd\environment_tracker.h
# End Source File
# Begin Source File

SOURCE="..\src\condor_c++_util\extArray.h"
# End Source File
# Begin Source File

SOURCE=..\src\condor_procd\local_server.h
# End Source File
# Begin Source File

SOURCE=..\src\condor_procd\local_server.WINDOWS.C
# End Source File
# Begin Source File

SOURCE=..\src\condor_procd\login_tracker.C
# End Source File
# Begin Source File

SOURCE=..\src\condor_procd\login_tracker.h
# End Source File
# Begin Source File

SOURCE="..\src\condor_c++_util\ntsysinfo.C"
# End Source File
# Begin Source File

SOURCE=..\src\condor_procd\parent_tracker.C
# End Source File
# Begin Source File

SOURCE=..\src\condor_procd\parent_tracker.h
# End Source File
# Begin Source File

SOURCE=..\src\condor_procd\pid_tracker.h
# End Source File
# Begin Source File

SOURCE=..\src\condor_procd\proc_family.C
# End Source File
# Begin Source File

SOURCE=..\src\condor_procd\proc_family.h
# End Source File
# Begin Source File

SOURCE=..\src\condor_procd\proc_family_io.h
# End Source File
# Begin Source File

SOURCE=..\src\condor_procd\proc_family_member.C
# End Source File
# Begin Source File

SOURCE=..\src\condor_procd\proc_family_member.h
# End Source File
# Begin Source File

SOURCE=..\src\condor_procd\proc_family_monitor.C
# End Source File
# Begin Source File

SOURCE=..\src\condor_procd\proc_family_monitor.h
# End Source File
# Begin Source File

SOURCE=..\src\condor_procd\proc_family_server.C
# End Source File
# Begin Source File

SOURCE=..\src\condor_procd\proc_family_server.h
# End Source File
# Begin Source File

SOURCE=..\src\condor_procd\proc_family_tracker.C
# End Source File
# Begin Source File

SOURCE=..\src\condor_procd\proc_family_tracker.h
# End Source File
# Begin Source File

SOURCE=..\src\condor_procd\procd_common.C
# End Source File
# Begin Source File

SOURCE=..\src\condor_procd\procd_common.h
# End Source File
# Begin Source File

SOURCE=..\src\condor_procd\procd_main.C
# End Source File
# Begin Source File

SOURCE="..\src\condor_c++_util\process_control.WINDOWS.C"
# End Source File
# Begin Source File

SOURCE="..\src\condor_c++_util\process_control.WINDOWS.h"
# End Source File
# Begin Source File

SOURCE=..\src\condor_procd\tracker_helper_list.h
# End Source File
# End Target
# End Project
