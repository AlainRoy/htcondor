# Microsoft Developer Studio Project File - Name="condor_dagman" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=condor_dagman - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "condor_dagman.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "condor_dagman.mak" CFG="condor_dagman - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "condor_dagman - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "condor_dagman - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "condor_dagman - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Debug"
# PROP Intermediate_Dir "..\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MTd /W3 /Gm /Gi /GX /ZI /Od $(CONDOR_INCLUDE) $(CONDOR_GSOAP_INCLUDE) $(CONDOR_GLOBUS_INCLUDE) $(CONDOR_KERB_INCLUDE) $(CONDOR_PCRE_INCLUDE) /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"..\Debug\condor_common.pch" /Yu"condor_common.h" /FD /TP /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32  ../Debug/condor_common.obj ..\Debug\condor_common_c.obj $(CONDOR_LIB) $(CONDOR_LIBPATH) $(CONDOR_GSOAP_LIB) $(CONDOR_GSOAP_LIBPATH) $(CONDOR_KERB_LIB) $(CONDOR_KERB_LIBPATH) $(CONDOR_PCRE_LIB) $(CONDOR_PCRE_LIBPATH) /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /SWAPRUN:NET
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "condor_dagman - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "condor_dagman___Win32_Release"
# PROP BASE Intermediate_Dir "condor_dagman___Win32_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Release"
# PROP Intermediate_Dir "..\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"..\src\condor_c++_util/condor_common.pch" /Yu"condor_common.h" /FD /TP /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /MT /W3 /GX /Z7 /O1 $(CONDOR_INCLUDE) $(CONDOR_GSOAP_INCLUDE) $(CONDOR_GLOBUS_INCLUDE) $(CONDOR_KERB_INCLUDE) $(CONDOR_PCRE_INCLUDE) /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"..\Release\condor_common.pch" /Yu"condor_common.h" /FD /TP /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /SWAPRUN:NET
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32  ..\Release\condor_common.obj ..\Release\condor_common_c.obj $(CONDOR_LIB) $(CONDOR_LIBPATH) $(CONDOR_GSOAP_LIB) $(CONDOR_GSOAP_LIBPATH) $(CONDOR_KERB_LIB) $(CONDOR_KERB_LIBPATH) $(CONDOR_PCRE_LIB) $(CONDOR_PCRE_LIBPATH) /nologo /subsystem:console /pdb:none /map /debug /machine:I386 /SWAPRUN:NET

!ENDIF 

# Begin Target

# Name "condor_dagman - Win32 Debug"
# Name "condor_dagman - Win32 Release"
# Begin Source File

SOURCE=..\src\condor_dagman\dag.C
# End Source File
# Begin Source File

SOURCE=..\src\condor_dagman\dag.h
# End Source File
# Begin Source File

SOURCE=..\src\condor_dagman\dagman_commands.C
# End Source File
# Begin Source File

SOURCE=..\src\condor_dagman\dagman_commands.h
# End Source File
# Begin Source File

SOURCE=..\src\condor_dagman\dagman_instantiate.C
# End Source File
# Begin Source File

SOURCE=..\src\condor_dagman\dagman_main.C
# End Source File
# Begin Source File

SOURCE=..\src\condor_dagman\dagman_submit.C
# End Source File
# Begin Source File

SOURCE=..\src\condor_dagman\dagman_util.C
# End Source File
# Begin Source File

SOURCE=..\src\condor_dagman\debug.C
# End Source File
# Begin Source File

SOURCE=..\src\condor_dagman\debug.h
# End Source File
# Begin Source File

SOURCE=..\src\condor_dagman\helper.C
# End Source File
# Begin Source File

SOURCE=..\src\condor_dagman\helper.h
# End Source File
# Begin Source File

SOURCE=..\src\condor_dagman\job.C
# End Source File
# Begin Source File

SOURCE=..\src\condor_dagman\job.h
# End Source File
# Begin Source File

SOURCE=..\src\condor_dagman\parse.C
# End Source File
# Begin Source File

SOURCE=..\src\condor_dagman\parse.h
# End Source File
# Begin Source File

SOURCE=..\src\condor_dagman\script.C
# End Source File
# Begin Source File

SOURCE=..\src\condor_dagman\script.h
# End Source File
# Begin Source File

SOURCE=..\src\condor_dagman\scriptQ.C
# End Source File
# Begin Source File

SOURCE=..\src\condor_dagman\scriptQ.h
# End Source File
# Begin Source File

SOURCE=..\src\condor_dagman\soap_dagmanC.C
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\src\condor_dagman\soap_dagmanH.h
# End Source File
# Begin Source File

SOURCE=..\src\condor_dagman\soap_dagmanServer.C
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\src\condor_dagman\soap_dagmanStub.C
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\src\condor_dagman\soap_dagmanStub.h

!IF  "$(CFG)" == "condor_dagman - Win32 Debug"

# Begin Custom Build
InputDir=\condor\workspaces\v67-externals\src\condor_dagman
InputPath=..\src\condor_dagman\soap_dagmanStub.h

"$(InputDir)\soap_dagmanC.C" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(InputDir) 
	soap_gen dagman 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "condor_dagman - Win32 Release"

# Begin Custom Build
InputDir=\condor\workspaces\v67-externals\src\condor_dagman
InputPath=..\src\condor_dagman\soap_dagmanStub.h

"$(InputDir)\soap_dagmanC.C" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd $(InputDir) 
	soap_gen dagman 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\src\condor_dagman\submit.h
# End Source File
# Begin Source File

SOURCE=..\src\condor_dagman\types.C
# End Source File
# Begin Source File

SOURCE=..\src\condor_dagman\types.h
# End Source File
# Begin Source File

SOURCE=..\src\condor_dagman\util.h
# End Source File
# End Target
# End Project
