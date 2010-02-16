# Based on the script with the same name by Eric Noulard 

# define a set of string with may-be useful readable name
# this file is meant to be included in a CMakeLists.txt
# not as a standalone CMake script
set(SPECIFIC_COMPILER_NAME "")
set(SPECIFIC_SYSTEM_VERSION_NAME "")

#For Linux, the foloing varaible will be set
#LINUX_NAME		= Fedora, CentOS, Redhat, Ubuntu, openSUSE
#LINUX_VER		= 12 (Fedora), 5.0 (Debian, CentOS)
#RPM_SYSTEM_NAME	= fc12, rhel5, ..
#BIT_MODE		= 32 | 64
#CPACK_DEBIAN_PACKAGE_ARCHITECTURE	= i386, amd64, ...  (value from dpkg utility) 

if(WIN32)
# information taken from
# http://www.codeguru.com/cpp/w-p/system/systeminformation/article.php/c8973/
	# Win9x series
	if(CMAKE_SYSTEM_VERSION MATCHES "4.0")
      set(SPECIFIC_SYSTEM_VERSION_NAME "Win95")
	endif(CMAKE_SYSTEM_VERSION MATCHES "4.0")
	if(CMAKE_SYSTEM_VERSION MATCHES "4.10")
      set(SPECIFIC_SYSTEM_VERSION_NAME "Win98")
	endif(CMAKE_SYSTEM_VERSION MATCHES "4.10")
	if(CMAKE_SYSTEM_VERSION MATCHES "4.90")
      set(SPECIFIC_SYSTEM_VERSION_NAME "WinME")
	endif(CMAKE_SYSTEM_VERSION MATCHES "4.90")

	# WinNTyyy series
	if(CMAKE_SYSTEM_VERSION MATCHES "3.0")
      set(SPECIFIC_SYSTEM_VERSION_NAME "WinNT351")
	endif(CMAKE_SYSTEM_VERSION MATCHES "3.0")
	if(CMAKE_SYSTEM_VERSION MATCHES "4.1")
      set(SPECIFIC_SYSTEM_VERSION_NAME "WinNT4")
	endif(CMAKE_SYSTEM_VERSION MATCHES "4.1")

    # Win2000/XP series
	if(CMAKE_SYSTEM_VERSION MATCHES "5.0")
      set(SPECIFIC_SYSTEM_VERSION_NAME "Win2000")
	endif(CMAKE_SYSTEM_VERSION MATCHES "5.0")
	if(CMAKE_SYSTEM_VERSION MATCHES "5.1")
      set(SPECIFIC_SYSTEM_VERSION_NAME "WinXP")
	endif(CMAKE_SYSTEM_VERSION MATCHES "5.1")
	if(CMAKE_SYSTEM_VERSION MATCHES "5.2")
      set(SPECIFIC_SYSTEM_VERSION_NAME "Win2003")
	endif(CMAKE_SYSTEM_VERSION MATCHES "5.2")

	# WinVista/7 series
	if(CMAKE_SYSTEM_VERSION MATCHES "6.0")
      set(SPECIFIC_SYSTEM_VERSION_NAME "WinVISTA")
	endif(CMAKE_SYSTEM_VERSION MATCHES "6.0")
	if(CMAKE_SYSTEM_VERSION MATCHES "6.1")
	  set(SPECIFIC_SYSTEM_VERSION_NAME "Win7")
	endif(CMAKE_SYSTEM_VERSION MATCHES "6.1")

    # Compilers
	# taken from http://predef.sourceforge.net/precomp.html#sec34
	IF (MSVC)
       if(MSVC_VERSION EQUAL 1200)
	     set(SPECIFIC_COMPILER_NAME "MSVC-6.0")
       endif(MSVC_VERSION EQUAL 1200)
       if(MSVC_VERSION EQUAL 1300)
	     set(SPECIFIC_COMPILER_NAME "MSVC-7.0")
       endif(MSVC_VERSION EQUAL 1300)
       if(MSVC_VERSION EQUAL 1310)
	     set(SPECIFIC_COMPILER_NAME "MSVC-7.1-2003") #Visual Studio 2003
       endif(MSVC_VERSION EQUAL 1310)
       if(MSVC_VERSION EQUAL 1400)
	     set(SPECIFIC_COMPILER_NAME "MSVC-8.0-2005") #Visual Studio 2005
       endif(MSVC_VERSION EQUAL 1400)
       if(MSVC_VERSION EQUAL 1500)
	     set(SPECIFIC_COMPILER_NAME "MSVC-9.0-2008") #Visual Studio 2008
       endif(MSVC_VERSION EQUAL 1500)
	endif(MSVC)
	IF (MINGW)
	   set(SPECIFIC_COMPILER_NAME "MinGW")
	endif(MINGW)
	IF (CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
       set(SPECIFIC_SYSTEM_VERSION_NAME "${SPECIFIC_SYSTEM_VERSION_NAME}-x86_64")
	endif(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
endif(WIN32)

if(UNIX)
  if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(SPECIFIC_SYSTEM_VERSION_NAME "${CMAKE_SYSTEM_NAME}")
    if(EXISTS "/etc/issue")
      set(LINUX_NAME "")
      file(READ "/etc/issue" LINUX_ISSUE)
      # Fedora case
      if(LINUX_ISSUE MATCHES "Fedora")
        string(REGEX MATCH "release ([0-9]+)" FEDORA "${LINUX_ISSUE}")
        set(LINUX_NAME "Fedora")
	set(LINUX_VER "${CMAKE_MATCH_1}")
	set(RPM_SYSTEM_NAME "fc${CMAKE_MATCH_1}")
      endif(LINUX_ISSUE MATCHES "Fedora")
      # Scientific Linux case
      if(LINUX_ISSUE MATCHES "Scientific Linux")
        string(REGEX MATCH "release ([0-9]+\\.[0-9]+)" CENTOS "${LINUX_ISSUE}")
        set(LINUX_NAME "ScientificLinux")        
	set(LINUX_VER "${CMAKE_MATCH_1}")
	set(RPM_SYSTEM_NAME "rhel${CMAKE_MATCH_1}")
      endif(LINUX_ISSUE MATCHES "Scientific Linux")
      # CentOS case
      if(LINUX_ISSUE MATCHES "CentOS")
        string(REGEX MATCH "release ([0-9]+\\.[0-9]+)" CENTOS "${LINUX_ISSUE}")
        set(LINUX_NAME "CentOS")        
	set(LINUX_VER "${CMAKE_MATCH_1}")
	set(RPM_SYSTEM_NAME "rhel${CMAKE_MATCH_1}")
      endif(LINUX_ISSUE MATCHES "CentOS")
      # Redhat case
      if(LINUX_ISSUE MATCHES "Red Hat")
        string(REGEX MATCH "release ([0-9]+\\.[0-9]+)" REDHAT "${LINUX_ISSUE}")
        set(LINUX_NAME "Redhat")        
	set(LINUX_VER "${CMAKE_MATCH_1}")
	set(RPM_SYSTEM_NAME "rhel${CMAKE_MATCH_1}")
      endif(LINUX_ISSUE MATCHES "Red Hat")
      # Ubuntu case
      if(LINUX_ISSUE MATCHES "Ubuntu")
        string(REGEX MATCH "buntu ([0-9]+\\.[0-9]+)" UBUNTU "${LINUX_ISSUE}")
        set(LINUX_NAME "Ubuntu")        
	set(LINUX_VER "${CMAKE_MATCH_1}")		  
      endif(LINUX_ISSUE MATCHES "Ubuntu")		
      # Debian case
      if(LINUX_ISSUE MATCHES "Debian")
        string(REGEX MATCH "Debian .*ux ([0-9]+\\.[0-9]+)" DEBIAN "${LINUX_ISSUE}")
        set(LINUX_NAME "Debian")
	set(LINUX_VER "${CMAKE_MATCH_1}")        
      endif(LINUX_ISSUE MATCHES "Debian")      
      # Open SuSE case
      if(LINUX_ISSUE MATCHES "SUSE")
        string(REGEX MATCH "SUSE  ([0-9]+\\.[0-9]+)" SUSE "${LINUX_ISSUE}")
        set(LINUX_NAME "openSUSE")        
	set(LINUX_VER "${CMAKE_MATCH_1}")        
	 string(REPLACE "/" "_" LINUX_VER ${LINUX_VER})        
      endif(LINUX_ISSUE MATCHES "SUSE")
      # Mandriva case
      # TODO      

	#Find CPU Arch
	if (${CMAKE_SYSTEM_PROCESSOR} MATCHES "i386|i586|i686")
		set ( BIT_MODE "32")
	elseif (${CMAKE_SYSTEM_PROCESSOR} MATCHES "x86_64")			 
		set ( BIT_MODE "64")
	endif ()
	
	#Find CPU Arch for Debian system 
	if ((LINUX_NAME STREQUAL "Debian") OR (LINUX_NAME STREQUAL "Ubuntu"))
		
	  # There is no such thing as i686 architecture on debian, you should use i386 instead
	  # $ dpkg --print-architecture
	  FIND_PROGRAM(DPKG_CMD dpkg)
	  IF(NOT DPKG_CMD)
		 # Cannot find dpkg in your path, default to i386
		 # Try best guess
		 if (BIT_MODE STREQUAL "32")
			SET(CPACK_DEBIAN_PACKAGE_ARCHITECTURE i386)
		 elseif (BIT_MODE STREQUAL "64")
			SET(CPACK_DEBIAN_PACKAGE_ARCHITECTURE amd64)
		 endif()
	  ENDIF(NOT DPKG_CMD)
	  EXECUTE_PROCESS(COMMAND "${DPKG_CMD}" --print-architecture
		 OUTPUT_VARIABLE CPACK_DEBIAN_PACKAGE_ARCHITECTURE
		 OUTPUT_STRIP_TRAILING_WHITESPACE
		 )
	endif ()

      if(LINUX_NAME) 
         set(SPECIFIC_SYSTEM_VERSION_NAME "${CMAKE_SYSTEM_NAME}-${LINUX_NAME}-${LINUX_VER}")
      endif(LINUX_NAME)

    endif(EXISTS "/etc/issue")      
  endif(CMAKE_SYSTEM_NAME MATCHES "Linux")
  set(SPECIFIC_SYSTEM_VERSION_NAME "${SPECIFIC_SYSTEM_VERSION_NAME}-${CMAKE_SYSTEM_PROCESSOR}")
  set(SPECIFIC_COMPILER_NAME "")
endif(UNIX)