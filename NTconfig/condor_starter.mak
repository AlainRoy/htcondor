# Microsoft Developer Studio Generated NMAKE File, Based on condor_starter.dsp
!IF "$(CFG)" == ""
CFG=condor_starter - Win32 Release
!MESSAGE No configuration specified. Defaulting to condor_starter - Win32 Release.
!ENDIF 

!IF "$(CFG)" != "condor_starter - Win32 Debug" && "$(CFG)" != "condor_starter - Win32 Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "condor_starter.mak" CFG="condor_starter - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "condor_starter - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "condor_starter - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "condor_starter - Win32 Debug"

OUTDIR=.\..\Debug
INTDIR=.\..\Debug
# Begin Custom Macros
OutDir=.\..\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\condor_starter.exe"

!ELSE 

ALL : "condor_privsep_client - Win32 Debug" "condor_procd_client - Win32 Debug" "condor_qmgmt - Win32 Debug" "condor_sysapi - Win32 Debug" "condor_procapi - Win32 Debug" "condor_util_lib - Win32 Debug" "condor_io - Win32 Debug" "condor_daemon_core - Win32 Debug" "condor_cpp_util - Win32 Debug" "condor_classad - Win32 Debug" "$(OUTDIR)\condor_starter.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"condor_classad - Win32 DebugCLEAN" "condor_cpp_util - Win32 DebugCLEAN" "condor_daemon_core - Win32 DebugCLEAN" "condor_io - Win32 DebugCLEAN" "condor_util_lib - Win32 DebugCLEAN" "condor_procapi - Win32 DebugCLEAN" "condor_sysapi - Win32 DebugCLEAN" "condor_qmgmt - Win32 DebugCLEAN" "condor_procd_client - Win32 DebugCLEAN" "condor_privsep_client - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\condor_privsep_helper.WINDOWS.obj"
	-@erase "$(INTDIR)\io_proxy.obj"
	-@erase "$(INTDIR)\io_proxy_handler.obj"
	-@erase "$(INTDIR)\java_detect.obj"
	-@erase "$(INTDIR)\java_proc.obj"
	-@erase "$(INTDIR)\jic_local.obj"
	-@erase "$(INTDIR)\jic_local_config.obj"
	-@erase "$(INTDIR)\jic_local_file.obj"
	-@erase "$(INTDIR)\jic_local_schedd.obj"
	-@erase "$(INTDIR)\jic_shadow.obj"
	-@erase "$(INTDIR)\job_info_communicator.obj"
	-@erase "$(INTDIR)\local_user_log.obj"
	-@erase "$(INTDIR)\mpi_comrade_proc.obj"
	-@erase "$(INTDIR)\mpi_master_proc.obj"
	-@erase "$(INTDIR)\NTsenders.obj"
	-@erase "$(INTDIR)\os_proc.obj"
	-@erase "$(INTDIR)\parallel_proc.obj"
	-@erase "$(INTDIR)\script_proc.obj"
	-@erase "$(INTDIR)\soap_starterC.obj"
	-@erase "$(INTDIR)\soap_starterServer.obj"
	-@erase "$(INTDIR)\soap_starterStub.obj"
	-@erase "$(INTDIR)\starter_class.obj"
	-@erase "$(INTDIR)\starter_user_policy.obj"
	-@erase "$(INTDIR)\starter_v61_main.obj"
	-@erase "$(INTDIR)\StarterHookMgr.obj"
	-@erase "$(INTDIR)\stream_handler.obj"
	-@erase "$(INTDIR)\tool_daemon_proc.obj"
	-@erase "$(INTDIR)\user_proc.obj"
	-@erase "$(INTDIR)\vanilla_proc.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\vm_gahp_request.obj"
	-@erase "$(INTDIR)\vm_gahp_server.obj"
	-@erase "$(INTDIR)\vm_proc.obj"
	-@erase "$(OUTDIR)\condor_starter.exe"
	-@erase "$(OUTDIR)\condor_starter.ilk"
	-@erase "$(OUTDIR)\condor_starter.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /Gm /Gi /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\condor_common.pch" /Yu"condor_common.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /TP $(CONDOR_INCLUDE) $(CONDOR_DEFINES) $(CONDOR_CPPARGS) $(CONDOR_GSOAP_INCLUDE) $(CONDOR_GLOBUS_INCLUDE) $(CONDOR_KERB_INCLUDE) $(CONDOR_PCRE_INCLUDE) $(CONDOR_OPENSSL_INCLUDE) $(CONDOR_POSTGRESQL_INCLUDE) /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\condor_starter.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=../Debug/condor_common.obj ..\Debug\condor_common_c.obj $(CONDOR_LIB) $(CONDOR_LIBPATH) $(CONDOR_GSOAP_LIB) $(CONDOR_GSOAP_LIBPATH) $(CONDOR_KERB_LIB) $(CONDOR_KERB_LIBPATH) $(CONDOR_PCRE_LIB) $(CONDOR_PCRE_LIBPATH) $(CONDOR_GLOBUS_LIB) $(CONDOR_GLOBUS_LIBPATH) $(CONDOR_OPENSSL_LIB) $(CONDOR_POSTGRESQL_LIB) $(CONDOR_OPENSSL_LIBPATH) $(CONDOR_POSTGRESQL_LIBPATH) /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\condor_starter.pdb" /debug /machine:I386 /out:"$(OUTDIR)\condor_starter.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\condor_privsep_helper.WINDOWS.obj" \
	"$(INTDIR)\io_proxy.obj" \
	"$(INTDIR)\io_proxy_handler.obj" \
	"$(INTDIR)\java_detect.obj" \
	"$(INTDIR)\java_proc.obj" \
	"$(INTDIR)\jic_local.obj" \
	"$(INTDIR)\jic_local_config.obj" \
	"$(INTDIR)\jic_local_file.obj" \
	"$(INTDIR)\jic_local_schedd.obj" \
	"$(INTDIR)\jic_shadow.obj" \
	"$(INTDIR)\job_info_communicator.obj" \
	"$(INTDIR)\local_user_log.obj" \
	"$(INTDIR)\mpi_comrade_proc.obj" \
	"$(INTDIR)\mpi_master_proc.obj" \
	"$(INTDIR)\NTsenders.obj" \
	"$(INTDIR)\os_proc.obj" \
	"$(INTDIR)\parallel_proc.obj" \
	"$(INTDIR)\script_proc.obj" \
	"$(INTDIR)\soap_starterC.obj" \
	"$(INTDIR)\soap_starterServer.obj" \
	"$(INTDIR)\soap_starterStub.obj" \
	"$(INTDIR)\starter_class.obj" \
	"$(INTDIR)\starter_user_policy.obj" \
	"$(INTDIR)\starter_v61_main.obj" \
	"$(INTDIR)\StarterHookMgr.obj" \
	"$(INTDIR)\stream_handler.obj" \
	"$(INTDIR)\tool_daemon_proc.obj" \
	"$(INTDIR)\user_proc.obj" \
	"$(INTDIR)\vanilla_proc.obj" \
	"$(INTDIR)\vm_gahp_request.obj" \
	"$(INTDIR)\vm_gahp_server.obj" \
	"$(INTDIR)\vm_proc.obj" \
	"$(OUTDIR)\condor_classad.lib" \
	"$(OUTDIR)\condor_cpp_util.lib" \
	"$(OUTDIR)\condor_daemon_core.lib" \
	"$(OUTDIR)\condor_io.lib" \
	"..\src\condor_util_lib\condor_util.lib" \
	"$(OUTDIR)\condor_procapi.lib" \
	"$(OUTDIR)\condor_sysapi.lib" \
	"$(OUTDIR)\condor_qmgmt.lib" \
	"$(OUTDIR)\condor_procd_client.lib" \
	"$(OUTDIR)\condor_privsep_client.lib"

"$(OUTDIR)\condor_starter.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "condor_starter - Win32 Release"

OUTDIR=.\..\Release
INTDIR=.\..\Release
# Begin Custom Macros
OutDir=.\..\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\condor_starter.exe"

!ELSE 

ALL : "condor_privsep_client - Win32 Release" "condor_procd_client - Win32 Release" "condor_qmgmt - Win32 Release" "condor_sysapi - Win32 Release" "condor_procapi - Win32 Release" "condor_util_lib - Win32 Release" "condor_io - Win32 Release" "condor_daemon_core - Win32 Release" "condor_cpp_util - Win32 Release" "condor_classad - Win32 Release" "$(OUTDIR)\condor_starter.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"condor_classad - Win32 ReleaseCLEAN" "condor_cpp_util - Win32 ReleaseCLEAN" "condor_daemon_core - Win32 ReleaseCLEAN" "condor_io - Win32 ReleaseCLEAN" "condor_util_lib - Win32 ReleaseCLEAN" "condor_procapi - Win32 ReleaseCLEAN" "condor_sysapi - Win32 ReleaseCLEAN" "condor_qmgmt - Win32 ReleaseCLEAN" "condor_procd_client - Win32 ReleaseCLEAN" "condor_privsep_client - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\condor_privsep_helper.WINDOWS.obj"
	-@erase "$(INTDIR)\io_proxy.obj"
	-@erase "$(INTDIR)\io_proxy_handler.obj"
	-@erase "$(INTDIR)\java_detect.obj"
	-@erase "$(INTDIR)\java_proc.obj"
	-@erase "$(INTDIR)\jic_local.obj"
	-@erase "$(INTDIR)\jic_local_config.obj"
	-@erase "$(INTDIR)\jic_local_file.obj"
	-@erase "$(INTDIR)\jic_local_schedd.obj"
	-@erase "$(INTDIR)\jic_shadow.obj"
	-@erase "$(INTDIR)\job_info_communicator.obj"
	-@erase "$(INTDIR)\local_user_log.obj"
	-@erase "$(INTDIR)\mpi_comrade_proc.obj"
	-@erase "$(INTDIR)\mpi_master_proc.obj"
	-@erase "$(INTDIR)\NTsenders.obj"
	-@erase "$(INTDIR)\os_proc.obj"
	-@erase "$(INTDIR)\parallel_proc.obj"
	-@erase "$(INTDIR)\script_proc.obj"
	-@erase "$(INTDIR)\soap_starterC.obj"
	-@erase "$(INTDIR)\soap_starterServer.obj"
	-@erase "$(INTDIR)\soap_starterStub.obj"
	-@erase "$(INTDIR)\starter_class.obj"
	-@erase "$(INTDIR)\starter_user_policy.obj"
	-@erase "$(INTDIR)\starter_v61_main.obj"
	-@erase "$(INTDIR)\StarterHookMgr.obj"
	-@erase "$(INTDIR)\stream_handler.obj"
	-@erase "$(INTDIR)\tool_daemon_proc.obj"
	-@erase "$(INTDIR)\user_proc.obj"
	-@erase "$(INTDIR)\vanilla_proc.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vm_gahp_request.obj"
	-@erase "$(INTDIR)\vm_gahp_server.obj"
	-@erase "$(INTDIR)\vm_proc.obj"
	-@erase "$(OUTDIR)\condor_starter.exe"
	-@erase "$(OUTDIR)\condor_starter.map"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /Z7 /O1 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\condor_common.pch" /Yu"condor_common.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /TP $(CONDOR_INCLUDE) $(CONDOR_DEFINES) $(CONDOR_CPPARGS) $(CONDOR_GSOAP_INCLUDE) $(CONDOR_GLOBUS_INCLUDE) $(CONDOR_KERB_INCLUDE) $(CONDOR_PCRE_INCLUDE) $(CONDOR_OPENSSL_INCLUDE) $(CONDOR_POSTGRESQL_INCLUDE) /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\condor_starter.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=../Release/condor_common.obj ../Release/condor_common_c.obj $(CONDOR_LIB) $(CONDOR_LIBPATH) $(CONDOR_GSOAP_LIB) $(CONDOR_GSOAP_LIBPATH) $(CONDOR_KERB_LIB) $(CONDOR_KERB_LIBPATH) $(CONDOR_PCRE_LIB) $(CONDOR_PCRE_LIBPATH) $(CONDOR_GLOBUS_LIB) $(CONDOR_GLOBUS_LIBPATH) $(CONDOR_OPENSSL_LIB) $(CONDOR_POSTGRESQL_LIB) $(CONDOR_OPENSSL_LIBPATH) $(CONDOR_POSTGRESQL_LIBPATH) /nologo /subsystem:console /pdb:none /map:"$(INTDIR)\condor_starter.map" /debug /machine:I386 /out:"$(OUTDIR)\condor_starter.exe" 
LINK32_OBJS= \
	"$(INTDIR)\condor_privsep_helper.WINDOWS.obj" \
	"$(INTDIR)\io_proxy.obj" \
	"$(INTDIR)\io_proxy_handler.obj" \
	"$(INTDIR)\java_detect.obj" \
	"$(INTDIR)\java_proc.obj" \
	"$(INTDIR)\jic_local.obj" \
	"$(INTDIR)\jic_local_config.obj" \
	"$(INTDIR)\jic_local_file.obj" \
	"$(INTDIR)\jic_local_schedd.obj" \
	"$(INTDIR)\jic_shadow.obj" \
	"$(INTDIR)\job_info_communicator.obj" \
	"$(INTDIR)\local_user_log.obj" \
	"$(INTDIR)\mpi_comrade_proc.obj" \
	"$(INTDIR)\mpi_master_proc.obj" \
	"$(INTDIR)\NTsenders.obj" \
	"$(INTDIR)\os_proc.obj" \
	"$(INTDIR)\parallel_proc.obj" \
	"$(INTDIR)\script_proc.obj" \
	"$(INTDIR)\soap_starterC.obj" \
	"$(INTDIR)\soap_starterServer.obj" \
	"$(INTDIR)\soap_starterStub.obj" \
	"$(INTDIR)\starter_class.obj" \
	"$(INTDIR)\starter_user_policy.obj" \
	"$(INTDIR)\starter_v61_main.obj" \
	"$(INTDIR)\StarterHookMgr.obj" \
	"$(INTDIR)\stream_handler.obj" \
	"$(INTDIR)\tool_daemon_proc.obj" \
	"$(INTDIR)\user_proc.obj" \
	"$(INTDIR)\vanilla_proc.obj" \
	"$(INTDIR)\vm_gahp_request.obj" \
	"$(INTDIR)\vm_gahp_server.obj" \
	"$(INTDIR)\vm_proc.obj" \
	"$(OUTDIR)\condor_classad.lib" \
	"$(OUTDIR)\condor_cpp_util.lib" \
	"$(OUTDIR)\condor_daemon_core.lib" \
	"$(OUTDIR)\condor_io.lib" \
	"..\src\condor_util_lib\condor_util.lib" \
	"$(OUTDIR)\condor_procapi.lib" \
	"$(OUTDIR)\condor_sysapi.lib" \
	"$(OUTDIR)\condor_qmgmt.lib" \
	"$(OUTDIR)\condor_procd_client.lib" \
	"$(OUTDIR)\condor_privsep_client.lib"

"$(OUTDIR)\condor_starter.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("condor_starter.dep")
!INCLUDE "condor_starter.dep"
!ELSE 
!MESSAGE Warning: cannot find "condor_starter.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "condor_starter - Win32 Debug" || "$(CFG)" == "condor_starter - Win32 Release"

!IF  "$(CFG)" == "condor_starter - Win32 Debug"

"condor_classad - Win32 Debug" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\condor_classad.mak CFG="condor_classad - Win32 Debug" 
   cd "."

"condor_classad - Win32 DebugCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\condor_classad.mak CFG="condor_classad - Win32 Debug" RECURSE=1 CLEAN 
   cd "."

!ELSEIF  "$(CFG)" == "condor_starter - Win32 Release"

"condor_classad - Win32 Release" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\condor_classad.mak CFG="condor_classad - Win32 Release" 
   cd "."

"condor_classad - Win32 ReleaseCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\condor_classad.mak CFG="condor_classad - Win32 Release" RECURSE=1 CLEAN 
   cd "."

!ENDIF 

!IF  "$(CFG)" == "condor_starter - Win32 Debug"

"condor_cpp_util - Win32 Debug" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\condor_cpp_util.mak CFG="condor_cpp_util - Win32 Debug" 
   cd "."

"condor_cpp_util - Win32 DebugCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\condor_cpp_util.mak CFG="condor_cpp_util - Win32 Debug" RECURSE=1 CLEAN 
   cd "."

!ELSEIF  "$(CFG)" == "condor_starter - Win32 Release"

"condor_cpp_util - Win32 Release" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\condor_cpp_util.mak CFG="condor_cpp_util - Win32 Release" 
   cd "."

"condor_cpp_util - Win32 ReleaseCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\condor_cpp_util.mak CFG="condor_cpp_util - Win32 Release" RECURSE=1 CLEAN 
   cd "."

!ENDIF 

!IF  "$(CFG)" == "condor_starter - Win32 Debug"

"condor_daemon_core - Win32 Debug" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\condor_daemon_core.mak CFG="condor_daemon_core - Win32 Debug" 
   cd "."

"condor_daemon_core - Win32 DebugCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\condor_daemon_core.mak CFG="condor_daemon_core - Win32 Debug" RECURSE=1 CLEAN 
   cd "."

!ELSEIF  "$(CFG)" == "condor_starter - Win32 Release"

"condor_daemon_core - Win32 Release" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\condor_daemon_core.mak CFG="condor_daemon_core - Win32 Release" 
   cd "."

"condor_daemon_core - Win32 ReleaseCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\condor_daemon_core.mak CFG="condor_daemon_core - Win32 Release" RECURSE=1 CLEAN 
   cd "."

!ENDIF 

!IF  "$(CFG)" == "condor_starter - Win32 Debug"

"condor_io - Win32 Debug" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\condor_io.mak CFG="condor_io - Win32 Debug" 
   cd "."

"condor_io - Win32 DebugCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\condor_io.mak CFG="condor_io - Win32 Debug" RECURSE=1 CLEAN 
   cd "."

!ELSEIF  "$(CFG)" == "condor_starter - Win32 Release"

"condor_io - Win32 Release" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\condor_io.mak CFG="condor_io - Win32 Release" 
   cd "."

"condor_io - Win32 ReleaseCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\condor_io.mak CFG="condor_io - Win32 Release" RECURSE=1 CLEAN 
   cd "."

!ENDIF 

!IF  "$(CFG)" == "condor_starter - Win32 Debug"

"condor_util_lib - Win32 Debug" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\condor_util_lib.mak CFG="condor_util_lib - Win32 Debug" 
   cd "."

"condor_util_lib - Win32 DebugCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\condor_util_lib.mak CFG="condor_util_lib - Win32 Debug" RECURSE=1 CLEAN 
   cd "."

!ELSEIF  "$(CFG)" == "condor_starter - Win32 Release"

"condor_util_lib - Win32 Release" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\condor_util_lib.mak CFG="condor_util_lib - Win32 Release" 
   cd "."

"condor_util_lib - Win32 ReleaseCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\condor_util_lib.mak CFG="condor_util_lib - Win32 Release" RECURSE=1 CLEAN 
   cd "."

!ENDIF 

!IF  "$(CFG)" == "condor_starter - Win32 Debug"

"condor_procapi - Win32 Debug" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\condor_procapi.mak CFG="condor_procapi - Win32 Debug" 
   cd "."

"condor_procapi - Win32 DebugCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\condor_procapi.mak CFG="condor_procapi - Win32 Debug" RECURSE=1 CLEAN 
   cd "."

!ELSEIF  "$(CFG)" == "condor_starter - Win32 Release"

"condor_procapi - Win32 Release" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\condor_procapi.mak CFG="condor_procapi - Win32 Release" 
   cd "."

"condor_procapi - Win32 ReleaseCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\condor_procapi.mak CFG="condor_procapi - Win32 Release" RECURSE=1 CLEAN 
   cd "."

!ENDIF 

!IF  "$(CFG)" == "condor_starter - Win32 Debug"

"condor_sysapi - Win32 Debug" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\condor_sysapi.mak CFG="condor_sysapi - Win32 Debug" 
   cd "."

"condor_sysapi - Win32 DebugCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\condor_sysapi.mak CFG="condor_sysapi - Win32 Debug" RECURSE=1 CLEAN 
   cd "."

!ELSEIF  "$(CFG)" == "condor_starter - Win32 Release"

"condor_sysapi - Win32 Release" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\condor_sysapi.mak CFG="condor_sysapi - Win32 Release" 
   cd "."

"condor_sysapi - Win32 ReleaseCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\condor_sysapi.mak CFG="condor_sysapi - Win32 Release" RECURSE=1 CLEAN 
   cd "."

!ENDIF 

!IF  "$(CFG)" == "condor_starter - Win32 Debug"

"condor_qmgmt - Win32 Debug" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\condor_qmgmt.mak CFG="condor_qmgmt - Win32 Debug" 
   cd "."

"condor_qmgmt - Win32 DebugCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\condor_qmgmt.mak CFG="condor_qmgmt - Win32 Debug" RECURSE=1 CLEAN 
   cd "."

!ELSEIF  "$(CFG)" == "condor_starter - Win32 Release"

"condor_qmgmt - Win32 Release" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\condor_qmgmt.mak CFG="condor_qmgmt - Win32 Release" 
   cd "."

"condor_qmgmt - Win32 ReleaseCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\condor_qmgmt.mak CFG="condor_qmgmt - Win32 Release" RECURSE=1 CLEAN 
   cd "."

!ENDIF 

!IF  "$(CFG)" == "condor_starter - Win32 Debug"

"condor_procd_client - Win32 Debug" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\condor_procd_client.mak CFG="condor_procd_client - Win32 Debug" 
   cd "."

"condor_procd_client - Win32 DebugCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\condor_procd_client.mak CFG="condor_procd_client - Win32 Debug" RECURSE=1 CLEAN 
   cd "."

!ELSEIF  "$(CFG)" == "condor_starter - Win32 Release"

"condor_procd_client - Win32 Release" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\condor_procd_client.mak CFG="condor_procd_client - Win32 Release" 
   cd "."

"condor_procd_client - Win32 ReleaseCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\condor_procd_client.mak CFG="condor_procd_client - Win32 Release" RECURSE=1 CLEAN 
   cd "."

!ENDIF 

!IF  "$(CFG)" == "condor_starter - Win32 Debug"

"condor_privsep_client - Win32 Debug" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\condor_privsep_client.mak CFG="condor_privsep_client - Win32 Debug" 
   cd "."

"condor_privsep_client - Win32 DebugCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\condor_privsep_client.mak CFG="condor_privsep_client - Win32 Debug" RECURSE=1 CLEAN 
   cd "."

!ELSEIF  "$(CFG)" == "condor_starter - Win32 Release"

"condor_privsep_client - Win32 Release" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\condor_privsep_client.mak CFG="condor_privsep_client - Win32 Release" 
   cd "."

"condor_privsep_client - Win32 ReleaseCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F .\condor_privsep_client.mak CFG="condor_privsep_client - Win32 Release" RECURSE=1 CLEAN 
   cd "."

!ENDIF 

SOURCE=..\src\condor_starter.V6.1\condor_privsep_helper.WINDOWS.cpp

"$(INTDIR)\condor_privsep_helper.WINDOWS.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\condor_common.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\condor_starter.V6.1\io_proxy.cpp

"$(INTDIR)\io_proxy.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\condor_common.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\condor_starter.V6.1\io_proxy_handler.cpp

"$(INTDIR)\io_proxy_handler.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\condor_common.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\condor_starter.V6.1\java_detect.cpp

"$(INTDIR)\java_detect.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\condor_common.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\condor_starter.V6.1\java_proc.cpp

"$(INTDIR)\java_proc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\condor_common.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\condor_starter.V6.1\jic_local.cpp

"$(INTDIR)\jic_local.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\condor_common.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\condor_starter.V6.1\jic_local_config.cpp

"$(INTDIR)\jic_local_config.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\condor_common.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\condor_starter.V6.1\jic_local_file.cpp

"$(INTDIR)\jic_local_file.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\condor_common.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\condor_starter.V6.1\jic_local_schedd.cpp

"$(INTDIR)\jic_local_schedd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\condor_common.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\condor_starter.V6.1\jic_shadow.cpp

"$(INTDIR)\jic_shadow.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\condor_common.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\condor_starter.V6.1\job_info_communicator.cpp

"$(INTDIR)\job_info_communicator.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\condor_common.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\condor_starter.V6.1\local_user_log.cpp

"$(INTDIR)\local_user_log.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\condor_common.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\condor_starter.V6.1\mpi_comrade_proc.cpp

"$(INTDIR)\mpi_comrade_proc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\condor_common.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\condor_starter.V6.1\mpi_master_proc.cpp

"$(INTDIR)\mpi_master_proc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\condor_common.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\condor_starter.V6.1\NTsenders.cpp

"$(INTDIR)\NTsenders.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\condor_common.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\condor_starter.V6.1\os_proc.cpp

"$(INTDIR)\os_proc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\condor_common.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\condor_starter.V6.1\parallel_proc.cpp

"$(INTDIR)\parallel_proc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\condor_common.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\condor_starter.V6.1\script_proc.cpp

"$(INTDIR)\script_proc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\condor_common.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\condor_starter.V6.1\soap_starterC.cpp

!IF  "$(CFG)" == "condor_starter - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /Gi /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /TP $(CONDOR_INCLUDE) $(CONDOR_DEFINES) $(CONDOR_CPPARGS) $(CONDOR_GSOAP_INCLUDE) $(CONDOR_GLOBUS_INCLUDE) $(CONDOR_KERB_INCLUDE) $(CONDOR_PCRE_INCLUDE) $(CONDOR_OPENSSL_INCLUDE) $(CONDOR_POSTGRESQL_INCLUDE) /c 

"$(INTDIR)\soap_starterC.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "condor_starter - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /Z7 /O1 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /TP $(CONDOR_INCLUDE) $(CONDOR_DEFINES) $(CONDOR_CPPARGS) $(CONDOR_GSOAP_INCLUDE) $(CONDOR_GLOBUS_INCLUDE) $(CONDOR_KERB_INCLUDE) $(CONDOR_PCRE_INCLUDE) $(CONDOR_OPENSSL_INCLUDE) $(CONDOR_POSTGRESQL_INCLUDE) /c 

"$(INTDIR)\soap_starterC.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\src\condor_starter.V6.1\soap_starterServer.cpp

!IF  "$(CFG)" == "condor_starter - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /Gi /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /TP $(CONDOR_INCLUDE) $(CONDOR_DEFINES) $(CONDOR_CPPARGS) $(CONDOR_GSOAP_INCLUDE) $(CONDOR_GLOBUS_INCLUDE) $(CONDOR_KERB_INCLUDE) $(CONDOR_PCRE_INCLUDE) $(CONDOR_OPENSSL_INCLUDE) $(CONDOR_POSTGRESQL_INCLUDE) /c 

"$(INTDIR)\soap_starterServer.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "condor_starter - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /Z7 /O1 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /TP $(CONDOR_INCLUDE) $(CONDOR_DEFINES) $(CONDOR_CPPARGS) $(CONDOR_GSOAP_INCLUDE) $(CONDOR_GLOBUS_INCLUDE) $(CONDOR_KERB_INCLUDE) $(CONDOR_PCRE_INCLUDE) $(CONDOR_OPENSSL_INCLUDE) $(CONDOR_POSTGRESQL_INCLUDE) /c 

"$(INTDIR)\soap_starterServer.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\src\condor_starter.V6.1\soap_starterStub.cpp

!IF  "$(CFG)" == "condor_starter - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /Gi /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /TP $(CONDOR_INCLUDE) $(CONDOR_DEFINES) $(CONDOR_CPPARGS) $(CONDOR_GSOAP_INCLUDE) $(CONDOR_GLOBUS_INCLUDE) $(CONDOR_KERB_INCLUDE) $(CONDOR_PCRE_INCLUDE) $(CONDOR_OPENSSL_INCLUDE) $(CONDOR_POSTGRESQL_INCLUDE) /c 

"$(INTDIR)\soap_starterStub.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "condor_starter - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /Z7 /O1 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /TP $(CONDOR_INCLUDE) $(CONDOR_DEFINES) $(CONDOR_CPPARGS) $(CONDOR_GSOAP_INCLUDE) $(CONDOR_GLOBUS_INCLUDE) $(CONDOR_KERB_INCLUDE) $(CONDOR_PCRE_INCLUDE) $(CONDOR_OPENSSL_INCLUDE) $(CONDOR_POSTGRESQL_INCLUDE) /c 

"$(INTDIR)\soap_starterStub.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\src\condor_starter.V6.1\starter_class.cpp

"$(INTDIR)\starter_class.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\condor_common.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\condor_starter.V6.1\starter_user_policy.cpp

"$(INTDIR)\starter_user_policy.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\condor_common.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\condor_starter.V6.1\starter_v61_main.cpp

"$(INTDIR)\starter_v61_main.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\condor_common.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\condor_starter.V6.1\StarterHookMgr.cpp

"$(INTDIR)\StarterHookMgr.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\condor_common.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\condor_starter.V6.1\stream_handler.cpp

!IF  "$(CFG)" == "condor_starter - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /Gi /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /TP $(CONDOR_INCLUDE) $(CONDOR_DEFINES) $(CONDOR_CPPARGS) $(CONDOR_GSOAP_INCLUDE) $(CONDOR_GLOBUS_INCLUDE) $(CONDOR_KERB_INCLUDE) $(CONDOR_PCRE_INCLUDE) $(CONDOR_OPENSSL_INCLUDE) $(CONDOR_POSTGRESQL_INCLUDE) /c 

"$(INTDIR)\stream_handler.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "condor_starter - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /Z7 /O1 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /TP $(CONDOR_INCLUDE) $(CONDOR_DEFINES) $(CONDOR_CPPARGS) $(CONDOR_GSOAP_INCLUDE) $(CONDOR_GLOBUS_INCLUDE) $(CONDOR_KERB_INCLUDE) $(CONDOR_PCRE_INCLUDE) $(CONDOR_OPENSSL_INCLUDE) $(CONDOR_POSTGRESQL_INCLUDE) /c 

"$(INTDIR)\stream_handler.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\src\condor_starter.V6.1\tool_daemon_proc.cpp

"$(INTDIR)\tool_daemon_proc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\condor_common.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\condor_starter.V6.1\user_proc.cpp

"$(INTDIR)\user_proc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\condor_common.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\condor_starter.V6.1\vanilla_proc.cpp

"$(INTDIR)\vanilla_proc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\condor_common.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\condor_starter.V6.1\vm_gahp_request.cpp

"$(INTDIR)\vm_gahp_request.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\condor_common.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\condor_starter.V6.1\vm_gahp_server.cpp

"$(INTDIR)\vm_gahp_server.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\condor_common.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\src\condor_starter.V6.1\vm_proc.cpp

"$(INTDIR)\vm_proc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\condor_common.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

