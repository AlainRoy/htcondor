# Microsoft Developer Studio Generated NMAKE File, Based on condor_collector.dsp
!IF "$(CFG)" == ""
CFG=condor_collector - Win32 Release
!MESSAGE No configuration specified. Defaulting to condor_collector - Win32 Release.
!ENDIF 

!IF "$(CFG)" != "condor_collector - Win32 Debug" && "$(CFG)" != "condor_collector - Win32 Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "condor_collector.mak" CFG="condor_collector - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "condor_collector - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "condor_collector - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "condor_collector - Win32 Debug"

OUTDIR=.\..\Debug
INTDIR=.\..\Debug
# Begin Custom Macros
OutDir=.\..\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\condor_collector.exe"

!ELSE 

ALL : "condor_privsep_client - Win32 Debug" "condor_procd_client - Win32 Debug" "condor_procapi - Win32 Debug" "condor_sysapi - Win32 Debug" "condor_io - Win32 Debug" "condor_daemon_core - Win32 Debug" "condor_classad - Win32 Debug" "condor_cpp_util - Win32 Debug" "condor_util_lib - Win32 Debug" "$(OUTDIR)\condor_collector.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"condor_util_lib - Win32 DebugCLEAN" "condor_cpp_util - Win32 DebugCLEAN" "condor_classad - Win32 DebugCLEAN" "condor_daemon_core - Win32 DebugCLEAN" "condor_io - Win32 DebugCLEAN" "condor_sysapi - Win32 DebugCLEAN" "condor_procapi - Win32 DebugCLEAN" "condor_procd_client - Win32 DebugCLEAN" "condor_privsep_client - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\collector.obj"
	-@erase "$(INTDIR)\collector_engine.obj"
	-@erase "$(INTDIR)\collector_main.obj"
	-@erase "$(INTDIR)\collector_stats.obj"
	-@erase "$(INTDIR)\hashkey.obj"
	-@erase "$(INTDIR)\soap_collectorC.obj"
	-@erase "$(INTDIR)\soap_collectorServer.obj"
	-@erase "$(INTDIR)\soap_collectorStub.obj"
	-@erase "$(INTDIR)\totals.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\view_server.obj"
	-@erase "$(OUTDIR)\condor_collector.exe"
	-@erase "$(OUTDIR)\condor_collector.ilk"
	-@erase "$(OUTDIR)\condor_collector.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /Gm /Gi /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\condor_common.pch" /Yu"condor_common.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /TP $(CONDOR_INCLUDE) $(CONDOR_GSOAP_INCLUDE) $(CONDOR_GLOBUS_INCLUDE) $(CONDOR_KERB_INCLUDE) $(CONDOR_PCRE_INCLUDE) $(CONDOR_OPENSSL_INCLUDE) $(CONDOR_POSTGRESQL_INCLUDE) /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\condor_collector.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=../Debug/condor_common.obj ..\Debug\condor_common_c.obj $(CONDOR_LIB) $(CONDOR_LIBPATH) $(CONDOR_GSOAP_LIB) $(CONDOR_GSOAP_LIBPATH) $(CONDOR_KERB_LIB) $(CONDOR_KERB_LIBPATH) $(CONDOR_PCRE_LIB) $(CONDOR_PCRE_LIBPATH) $(CONDOR_GLOBUS_LIB) $(CONDOR_GLOBUS_LIBPATH) $(CONDOR_OPENSSL_LIB) $(CONDOR_POSTGRESQL_LIB) $(CONDOR_OPENSSL_LIBPATH) $(CONDOR_POSTGRESQL_LIBPATH) /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\condor_collector.pdb" /debug /machine:I386 /out:"$(OUTDIR)\condor_collector.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\collector.obj" \
	"$(INTDIR)\collector_engine.obj" \
	"$(INTDIR)\collector_main.obj" \
	"$(INTDIR)\collector_stats.obj" \
	"$(INTDIR)\hashkey.obj" \
	"$(INTDIR)\soap_collectorC.obj" \
	"$(INTDIR)\soap_collectorServer.obj" \
	"$(INTDIR)\soap_collectorStub.obj" \
	"$(INTDIR)\totals.obj" \
	"$(INTDIR)\view_server.obj" \
	"..\src\condor_util_lib\condor_util.lib" \
	"$(OUTDIR)\condor_cpp_util.lib" \
	"$(OUTDIR)\condor_classad.lib" \
	"$(OUTDIR)\condor_daemon_core.lib" \
	"$(OUTDIR)\condor_io.lib" \
	"$(OUTDIR)\condor_sysapi.lib" \
	"$(OUTDIR)\condor_procapi.lib" \
	"$(OUTDIR)\condor_procd_client.lib" \
	"$(OUTDIR)\condor_privsep_client.lib"

"$(OUTDIR)\condor_collector.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "condor_collector - Win32 Release"

OUTDIR=.\../Release
INTDIR=.\../Release
# Begin Custom Macros
OutDir=.\../Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\condor_collector.exe"

!ELSE 

ALL : "condor_privsep_client - Win32 Release" "condor_procd_client - Win32 Release" "condor_procapi - Win32 Release" "condor_sysapi - Win32 Release" "condor_io - Win32 Release" "condor_daemon_core - Win32 Release" "condor_classad - Win32 Release" "condor_cpp_util - Win32 Release" "condor_util_lib - Win32 Release" "$(OUTDIR)\condor_collector.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"condor_util_lib - Win32 ReleaseCLEAN" "condor_cpp_util - Win32 ReleaseCLEAN" "condor_classad - Win32 ReleaseCLEAN" "condor_daemon_core - Win32 ReleaseCLEAN" "condor_io - Win32 ReleaseCLEAN" "condor_sysapi - Win32 ReleaseCLEAN" "condor_procapi - Win32 ReleaseCLEAN" "condor_procd_client - Win32 ReleaseCLEAN" "condor_privsep_client - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\collector.obj"
	-@erase "$(INTDIR)\collector_engine.obj"
	-@erase "$(INTDIR)\collector_main.obj"
	-@erase "$(INTDIR)\collector_stats.obj"
	-@erase "$(INTDIR)\hashkey.obj"
	-@erase "$(INTDIR)\soap_collectorC.obj"
	-@erase "$(INTDIR)\soap_collectorServer.obj"
	-@erase "$(INTDIR)\soap_collectorStub.obj"
	-@erase "$(INTDIR)\totals.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\view_server.obj"
	-@erase "$(OUTDIR)\condor_collector.exe"
	-@erase "$(OUTDIR)\condor_collector.map"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /Z7 /O1 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\condor_common.pch" /Yu"condor_common.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /TP $(CONDOR_INCLUDE) $(CONDOR_GSOAP_INCLUDE) $(CONDOR_GLOBUS_INCLUDE) $(CONDOR_KERB_INCLUDE) $(CONDOR_PCRE_INCLUDE) $(CONDOR_OPENSSL_INCLUDE) $(CONDOR_POSTGRESQL_INCLUDE) /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\condor_collector.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=../Release/condor_common.obj ../Release/condor_common_c.obj $(CONDOR_LIB) $(CONDOR_LIBPATH) $(CONDOR_GSOAP_LIB) $(CONDOR_GSOAP_LIBPATH) $(CONDOR_KERB_LIB) $(CONDOR_KERB_LIBPATH) $(CONDOR_PCRE_LIB) $(CONDOR_PCRE_LIBPATH) $(CONDOR_GLOBUS_LIB) $(CONDOR_GLOBUS_LIBPATH) $(CONDOR_OPENSSL_LIB) $(CONDOR_POSTGRESQL_LIB) $(CONDOR_OPENSSL_LIBPATH) $(CONDOR_POSTGRESQL_LIBPATH) /nologo /subsystem:console /pdb:none /map:"$(INTDIR)\condor_collector.map" /debug /machine:I386 /out:"$(OUTDIR)\condor_collector.exe" 
LINK32_OBJS= \
	"$(INTDIR)\collector.obj" \
	"$(INTDIR)\collector_engine.obj" \
	"$(INTDIR)\collector_main.obj" \
	"$(INTDIR)\collector_stats.obj" \
	"$(INTDIR)\hashkey.obj" \
	"$(INTDIR)\soap_collectorC.obj" \
	"$(INTDIR)\soap_collectorServer.obj" \
	"$(INTDIR)\soap_collectorStub.obj" \
	"$(INTDIR)\totals.obj" \
	"$(INTDIR)\view_server.obj" \
	"..\src\condor_util_lib\condor_util.lib" \
	"$(OUTDIR)\condor_cpp_util.lib" \
	"$(OUTDIR)\condor_classad.lib" \
	"$(OUTDIR)\condor_daemon_core.lib" \
	"$(OUTDIR)\condor_io.lib" \
	"$(OUTDIR)\condor_sysapi.lib" \
	"$(OUTDIR)\condor_procapi.lib" \
	"$(OUTDIR)\condor_procd_client.lib" \
	"$(OUTDIR)\condor_privsep_client.lib"

"$(OUTDIR)\condor_collector.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("condor_collector.dep")
!INCLUDE "condor_collector.dep"
!ELSE 
!MESSAGE Warning: cannot find "condor_collector.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "condor_collector - Win32 Debug" || "$(CFG)" == "condor_collector - Win32 Release"

!IF  "$(CFG)" == "condor_collector - Win32 Debug"

"condor_util_lib - Win32 Debug" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\condor_util_lib.mak" CFG="condor_util_lib - Win32 Debug" 
   cd "."

"condor_util_lib - Win32 DebugCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\condor_util_lib.mak" CFG="condor_util_lib - Win32 Debug" RECURSE=1 CLEAN 
   cd "."

!ELSEIF  "$(CFG)" == "condor_collector - Win32 Release"

"condor_util_lib - Win32 Release" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\condor_util_lib.mak" CFG="condor_util_lib - Win32 Release" 
   cd "."

"condor_util_lib - Win32 ReleaseCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\condor_util_lib.mak" CFG="condor_util_lib - Win32 Release" RECURSE=1 CLEAN 
   cd "."

!ENDIF 

!IF  "$(CFG)" == "condor_collector - Win32 Debug"

"condor_cpp_util - Win32 Debug" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\condor_cpp_util.mak" CFG="condor_cpp_util - Win32 Debug" 
   cd "."

"condor_cpp_util - Win32 DebugCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\condor_cpp_util.mak" CFG="condor_cpp_util - Win32 Debug" RECURSE=1 CLEAN 
   cd "."

!ELSEIF  "$(CFG)" == "condor_collector - Win32 Release"

"condor_cpp_util - Win32 Release" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\condor_cpp_util.mak" CFG="condor_cpp_util - Win32 Release" 
   cd "."

"condor_cpp_util - Win32 ReleaseCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\condor_cpp_util.mak" CFG="condor_cpp_util - Win32 Release" RECURSE=1 CLEAN 
   cd "."

!ENDIF 

!IF  "$(CFG)" == "condor_collector - Win32 Debug"

"condor_classad - Win32 Debug" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\condor_classad.mak" CFG="condor_classad - Win32 Debug" 
   cd "."

"condor_classad - Win32 DebugCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\condor_classad.mak" CFG="condor_classad - Win32 Debug" RECURSE=1 CLEAN 
   cd "."

!ELSEIF  "$(CFG)" == "condor_collector - Win32 Release"

"condor_classad - Win32 Release" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\condor_classad.mak" CFG="condor_classad - Win32 Release" 
   cd "."

"condor_classad - Win32 ReleaseCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\condor_classad.mak" CFG="condor_classad - Win32 Release" RECURSE=1 CLEAN 
   cd "."

!ENDIF 

!IF  "$(CFG)" == "condor_collector - Win32 Debug"

"condor_daemon_core - Win32 Debug" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\condor_daemon_core.mak" CFG="condor_daemon_core - Win32 Debug" 
   cd "."

"condor_daemon_core - Win32 DebugCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\condor_daemon_core.mak" CFG="condor_daemon_core - Win32 Debug" RECURSE=1 CLEAN 
   cd "."

!ELSEIF  "$(CFG)" == "condor_collector - Win32 Release"

"condor_daemon_core - Win32 Release" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\condor_daemon_core.mak" CFG="condor_daemon_core - Win32 Release" 
   cd "."

"condor_daemon_core - Win32 ReleaseCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\condor_daemon_core.mak" CFG="condor_daemon_core - Win32 Release" RECURSE=1 CLEAN 
   cd "."

!ENDIF 

!IF  "$(CFG)" == "condor_collector - Win32 Debug"

"condor_io - Win32 Debug" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\condor_io.mak" CFG="condor_io - Win32 Debug" 
   cd "."

"condor_io - Win32 DebugCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\condor_io.mak" CFG="condor_io - Win32 Debug" RECURSE=1 CLEAN 
   cd "."

!ELSEIF  "$(CFG)" == "condor_collector - Win32 Release"

"condor_io - Win32 Release" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\condor_io.mak" CFG="condor_io - Win32 Release" 
   cd "."

"condor_io - Win32 ReleaseCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\condor_io.mak" CFG="condor_io - Win32 Release" RECURSE=1 CLEAN 
   cd "."

!ENDIF 

!IF  "$(CFG)" == "condor_collector - Win32 Debug"

"condor_sysapi - Win32 Debug" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\condor_sysapi.mak" CFG="condor_sysapi - Win32 Debug" 
   cd "."

"condor_sysapi - Win32 DebugCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\condor_sysapi.mak" CFG="condor_sysapi - Win32 Debug" RECURSE=1 CLEAN 
   cd "."

!ELSEIF  "$(CFG)" == "condor_collector - Win32 Release"

"condor_sysapi - Win32 Release" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\condor_sysapi.mak" CFG="condor_sysapi - Win32 Release" 
   cd "."

"condor_sysapi - Win32 ReleaseCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\condor_sysapi.mak" CFG="condor_sysapi - Win32 Release" RECURSE=1 CLEAN 
   cd "."

!ENDIF 

!IF  "$(CFG)" == "condor_collector - Win32 Debug"

"condor_procapi - Win32 Debug" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\condor_procapi.mak" CFG="condor_procapi - Win32 Debug" 
   cd "."

"condor_procapi - Win32 DebugCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\condor_procapi.mak" CFG="condor_procapi - Win32 Debug" RECURSE=1 CLEAN 
   cd "."

!ELSEIF  "$(CFG)" == "condor_collector - Win32 Release"

"condor_procapi - Win32 Release" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\condor_procapi.mak" CFG="condor_procapi - Win32 Release" 
   cd "."

"condor_procapi - Win32 ReleaseCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\condor_procapi.mak" CFG="condor_procapi - Win32 Release" RECURSE=1 CLEAN 
   cd "."

!ENDIF 

!IF  "$(CFG)" == "condor_collector - Win32 Debug"

"condor_procd_client - Win32 Debug" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\condor_procd_client.mak" CFG="condor_procd_client - Win32 Debug" 
   cd "."

"condor_procd_client - Win32 DebugCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\condor_procd_client.mak" CFG="condor_procd_client - Win32 Debug" RECURSE=1 CLEAN 
   cd "."

!ELSEIF  "$(CFG)" == "condor_collector - Win32 Release"

"condor_procd_client - Win32 Release" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\condor_procd_client.mak" CFG="condor_procd_client - Win32 Release" 
   cd "."

"condor_procd_client - Win32 ReleaseCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\condor_procd_client.mak" CFG="condor_procd_client - Win32 Release" RECURSE=1 CLEAN 
   cd "."

!ENDIF 

!IF  "$(CFG)" == "condor_collector - Win32 Debug"

"condor_privsep_client - Win32 Debug" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\condor_privsep_client.mak" CFG="condor_privsep_client - Win32 Debug" 
   cd "."

"condor_privsep_client - Win32 DebugCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\condor_privsep_client.mak" CFG="condor_privsep_client - Win32 Debug" RECURSE=1 CLEAN 
   cd "."

!ELSEIF  "$(CFG)" == "condor_collector - Win32 Release"

"condor_privsep_client - Win32 Release" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\condor_privsep_client.mak" CFG="condor_privsep_client - Win32 Release" 
   cd "."

"condor_privsep_client - Win32 ReleaseCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\condor_privsep_client.mak" CFG="condor_privsep_client - Win32 Release" RECURSE=1 CLEAN 
   cd "."

!ENDIF 

SOURCE=..\src\condor_collector.V6\collector.C

"$(INTDIR)\collector.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\condor_common.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\condor_collector.V6\collector_engine.C

"$(INTDIR)\collector_engine.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\condor_common.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\condor_collector.V6\collector_main.C

"$(INTDIR)\collector_main.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\condor_common.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\condor_collector.V6\collector_stats.C

"$(INTDIR)\collector_stats.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\condor_common.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\condor_collector.V6\hashkey.C

"$(INTDIR)\hashkey.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\condor_common.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\condor_collector.V6\soap_collectorC.C

!IF  "$(CFG)" == "condor_collector - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /Gi /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /TP $(CONDOR_INCLUDE) $(CONDOR_GSOAP_INCLUDE) $(CONDOR_GLOBUS_INCLUDE) $(CONDOR_KERB_INCLUDE) $(CONDOR_PCRE_INCLUDE) $(CONDOR_OPENSSL_INCLUDE) $(CONDOR_POSTGRESQL_INCLUDE) /c 

"$(INTDIR)\soap_collectorC.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "condor_collector - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /Z7 /O1 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /TP $(CONDOR_INCLUDE) $(CONDOR_GSOAP_INCLUDE) $(CONDOR_GLOBUS_INCLUDE) $(CONDOR_KERB_INCLUDE) $(CONDOR_PCRE_INCLUDE) $(CONDOR_OPENSSL_INCLUDE) $(CONDOR_POSTGRESQL_INCLUDE) /c 

"$(INTDIR)\soap_collectorC.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\src\condor_collector.V6\soap_collectorServer.C

!IF  "$(CFG)" == "condor_collector - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /Gi /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /TP $(CONDOR_INCLUDE) $(CONDOR_GSOAP_INCLUDE) $(CONDOR_GLOBUS_INCLUDE) $(CONDOR_KERB_INCLUDE) $(CONDOR_PCRE_INCLUDE) $(CONDOR_OPENSSL_INCLUDE) $(CONDOR_POSTGRESQL_INCLUDE) /c 

"$(INTDIR)\soap_collectorServer.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "condor_collector - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /Z7 /O1 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /TP $(CONDOR_INCLUDE) $(CONDOR_GSOAP_INCLUDE) $(CONDOR_GLOBUS_INCLUDE) $(CONDOR_KERB_INCLUDE) $(CONDOR_PCRE_INCLUDE) $(CONDOR_OPENSSL_INCLUDE) $(CONDOR_POSTGRESQL_INCLUDE) /c 

"$(INTDIR)\soap_collectorServer.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\src\condor_collector.V6\soap_collectorStub.C

!IF  "$(CFG)" == "condor_collector - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /Gi /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\condor_common.pch" /Yu"condor_common.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /TP $(CONDOR_INCLUDE) $(CONDOR_GSOAP_INCLUDE) $(CONDOR_GLOBUS_INCLUDE) $(CONDOR_KERB_INCLUDE) $(CONDOR_PCRE_INCLUDE) $(CONDOR_OPENSSL_INCLUDE) $(CONDOR_POSTGRESQL_INCLUDE) /c 

"$(INTDIR)\soap_collectorStub.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\condor_common.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "condor_collector - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /Z7 /O1 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\condor_common.pch" /Yu"condor_common.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /TP $(CONDOR_INCLUDE) $(CONDOR_GSOAP_INCLUDE) $(CONDOR_GLOBUS_INCLUDE) $(CONDOR_KERB_INCLUDE) $(CONDOR_PCRE_INCLUDE) $(CONDOR_OPENSSL_INCLUDE) $(CONDOR_POSTGRESQL_INCLUDE) /c 

"$(INTDIR)\soap_collectorStub.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\condor_common.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\src\condor_status.V6\totals.C

"$(INTDIR)\totals.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\condor_common.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\condor_collector.V6\view_server.C

"$(INTDIR)\view_server.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\condor_common.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

