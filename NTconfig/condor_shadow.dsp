# Microsoft Developer Studio Project File - Name="condor_shadow" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=condor_shadow - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "condor_shadow.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "condor_shadow.mak" CFG="condor_shadow - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "condor_shadow - Win32 Release" (based on\
 "Win32 (x86) Console Application")
!MESSAGE "condor_shadow - Win32 Debug" (based on\
 "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "condor_shadow - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\src\condor_shadow.V6.1"
# PROP Intermediate_Dir "..\src\condor_shadow.V6.1"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\src\h" /I "..\src\condor_includes" /I "..\src\condor_c++_util" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"..\src\condor_c++_util/condor_common.pch" /Yu"condor_common.h" /FD /TP /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib pdh.lib ws2_32.lib condor_c++_util\condor_common.obj /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "condor_shadow - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\src\condor_shadow.V6.1"
# PROP Intermediate_Dir "..\src\condor_shadow.V6.1"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MTd /W3 /GX /Z7 /Od /I "..\src\h" /I "..\src\condor_includes" /I "..\src\condor_c++_util" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"..\src\condor_c++_util/condor_common.pch" /Yu"condor_common.h" /FD /TP /c
# SUBTRACT CPP /Fr
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib pdh.lib ws2_32.lib mswsock.lib netapi32.lib ../src/condor_c++_util/condor_common.obj ../src/condor_util_lib/condor_common.obj /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /incremental:no

!ENDIF 

# Begin Target

# Name "condor_shadow - Win32 Release"
# Name "condor_shadow - Win32 Debug"
# Begin Source File

SOURCE=..\src\condor_shadow.V6.1\baseshadow.C
# End Source File
# Begin Source File

SOURCE=..\src\condor_shadow.V6.1\baseshadow.h
# End Source File
# Begin Source File

SOURCE=..\src\condor_shadow.V6.1\main.C
# End Source File
# Begin Source File

SOURCE=..\src\condor_shadow.V6.1\mpiresource.C
# End Source File
# Begin Source File

SOURCE=..\src\condor_shadow.V6.1\mpiresource.h
# End Source File
# Begin Source File

SOURCE=..\src\condor_shadow.V6.1\mpishadow.C
# End Source File
# Begin Source File

SOURCE=..\src\condor_shadow.V6.1\mpishadow.h
# End Source File
# Begin Source File

SOURCE=..\src\condor_shadow.V6.1\NTreceivers.C
# End Source File
# Begin Source File

SOURCE=..\src\condor_shadow.V6.1\pseudo_ops.C
# End Source File
# Begin Source File

SOURCE=..\src\condor_shadow.V6.1\pseudo_ops.h
# End Source File
# Begin Source File

SOURCE=..\src\condor_shadow.V6.1\remoteresource.C
# End Source File
# Begin Source File

SOURCE=..\src\condor_shadow.V6.1\remoteresource.h
# End Source File
# Begin Source File

SOURCE=..\src\condor_shadow.V6.1\shadow.C
# End Source File
# Begin Source File

SOURCE=..\src\condor_shadow.V6.1\shadow.h
# End Source File
# End Target
# End Project
