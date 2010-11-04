
##################################################################
## Begin the CPACK variable on-slaught.
##
## Start with the common section.
##################################################################
set (PACKAGE_REVISION "1")
set (CPACK_PACKAGE_NAME ${PACKAGE_NAME})
set (CPACK_PACKAGE_VENDOR "Condor Team - University of Wisconsin Madison")
set (CPACK_PACKAGE_VERSION ${PACKAGE_VERSION})
set (CPACK_PACKAGE_CONTACT "condor-users@cs.wisc.edu") 
set (URL "http://www.cs.wisc.edu/condor/")
set (CONDOR_VER "${PACKAGE_NAME}-${PACKAGE_VERSION}")

set (CPACK_PACKAGE_DESCRIPTION "Condor is a specialized workload management system for
compute-intensive jobs. Like other full-featured batch systems,
Condor provides a job queueing mechanism, scheduling policy,
priority scheme, resource monitoring, and resource management.
Users submit their serial or parallel jobs to Condor, Condor places
them into a queue, chooses when and where to run the jobs based
upon a policy, carefully monitors their progress, and ultimately
informs the user upon completion.")

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Condor: High Throughput Computing")

set(CPACK_DEBIAN_DESCRIPTION_SUMMARY "workload management system
 Condor is a specialized workload management system for
 compute-intensive jobs. Condor provides a job queueing mechanism, 
 scheduling policy, priority scheme, resource monitoring, 
 and resource management. Users submit their serial or parallel jobs
 to Condor, Condor places them into a queue, chooses when and where 
 to run the jobs based upon a policy, carefully monitors their progress,
 and ultimately informs the user upon completion.")

set(CPACK_RESOURCE_FILE_LICENSE "${CONDOR_SOURCE_DIR}/LICENSE-2.0.txt")
#set(CPACK_RESOURCE_FILE_README "${CONDOR_SOURCE_DIR}/build/backstage/release_notes/README")
#set(CPACK_RESOURCE_FILE_WELCOME "${CONDOR_SOURCE_DIR}/build/backstage/release_notes/DOC") # this should be more of a Hiya welcome.

set(CPACK_SYSTEM_NAME "${OS_NAME}-${SYS_ARCH}" )
set(CPACK_TOPLEVEL_TAG "${OS_NAME}-${SYS_ARCH}" )
set(CPACK_PACKAGE_ARCHITECTURE ${SYS_ARCH} ) 

# always strip the source files.
set(CPACK_SOURCE_STRIP_FILES TRUE)

# you can enable/disable file stripping.
option(CONDOR_STRIP_PACKAGES "Enables a package build" ON)
set(CPACK_STRIP_FILES ${CONDOR_STRIP_PACKAGES})

# here is where we can
set (CPACK_PACKAGE_FILE_NAME "${CONDOR_VER}-${OS_NAME}-${SYS_ARCH}" )

##################################################################
## Now onto platform specific package generation
## http://www.itk.org/Wiki/CMake:CPackPackageGenerators
##################################################################

#option used to enable/disable make package for rpm/deb with different install paths
option(CONDOR_PACKAGE_BUILD "Enables a package build" OFF)

# 1st set the location of the install targets.
set( C_BIN			bin)
set( C_LIB			lib)
set( C_LIBEXEC		libexec )
set( C_SBIN			sbin)

set( C_INCLUDE		include/condor)
set( C_MAN			man)
set( C_SRC			src)
set( C_SQL			usr/share/condor/sql)

set( C_INIT			etc/init.d )
set( C_ETC			etc/examples )
set( C_CONFIGD		etc/condor/config.d )
set( C_SYSCONFIG	etc/sysconfig )

set( C_ETC_EXAMPLES	. ) #etc/examples)
set( C_DOC			. )

set( C_LOCAL_DIR	var/lib/condor )
set( C_LOG_DIR		var/log/condor )
set( C_LOCK_DIR		var/lock/condor )
set( C_RUN_DIR		var/run/condor )
# NOTE: any RPATH should use these variables + PREFIX for location

# set a default generator
set ( CPACK_GENERATOR "TGZ" )

#this needs to be evaluated in order due to WIN collision.
if(${OS_NAME} STREQUAL "DARWIN")
	# enable if we desire native packaging.
	# set ( CPACK_GENERATOR "${CPACK_GENERATOR};PackageMaker" ) ;
	# set (CPACK_OSX_PACKAGE_VERSION)
elseif ( ${OS_NAME} MATCHES "WIN" )

	# override for windows.
	set( C_BIN bin )
	set( C_LIB bin )
	set( C_LIBEXEC bin )
	set( C_SBIN bin )
	set( C_ETC etc )

	set (CONDOR_WIX_LOC ${CONDOR_SOURCE_DIR}/msconfig/WiX)
	set (CPACK_PACKAGE_INSTALL_DIRECTORY "${CONDOR_VER}")
	set (CPACK_PACKAGE_FILE_NAME "${CONDOR_VER}")
	set (CPACK_PACKAGE_INSTALL_REGISTRY_KEY "${CONDOR_VER}")

	# branding and licensing
	set (CPACK_PACKAGE_ICON ${CONDOR_WIX_LOC}/Bitmaps/dlgbmp.bmp) 
	set (CPACK_RESOURCE_FILE_LICENSE "${CONDOR_SOURCE_DIR}/msconfig/license.rtf")

	set (CPACK_GENERATOR "WIX;ZIP")
	set (CPACK_WIX_PRODUCT_GUID "ea9608e1-9a9d-4678-800c-645df677094a")
	set (CPACK_WIX_UPGRADE_GUID "ef96d7c4-29df-403c-8fab-662386a089a4")
	set (CPACK_WIX_BITMAP_FOLDER ${CONDOR_WIX_LOC}/Bitmaps)
	set (CPACK_WIX_WXS_FILES ${CONDOR_WIX_LOC}/xml/CondorCfgDlg.wxs;${CONDOR_WIX_LOC}/xml/CondorPoolCfgDlg.wxs;${CONDOR_WIX_LOC}/xml/CondorExecCfgDlg.wxs;${CONDOR_WIX_LOC}/xml/CondorDomainCfgDlg.wxs;${CONDOR_WIX_LOC}/xml/CondorEmailCfgDlg.wxs;${CONDOR_WIX_LOC}/xml/CondorJavaCfgDlg.wxs;${CONDOR_WIX_LOC}/xml/CondorPermCfgDlg.wxs;${CONDOR_WIX_LOC}/xml/CondorVMCfgDlg.wxs;${CONDOR_WIX_LOC}/xml/CondorHDFSCfgDlg.wxs)

	# the configure file f(n) will replace @CMAKE_XYZ@ with their value
	configure_file(${CONDOR_WIX_LOC}/xml/win.xsl.in ${CONDOR_WIX_LOC}/xml/win.xsl @ONLY)
	set (CPACK_WIX_XSL ${CONDOR_WIX_LOC}/xml/win.xsl)

	option(WIN_EXEC_NODE_ONLY "Minimal Package Win exec node only" OFF)

	# below are options an overrides to enable packge generation for rpm & deb
elseif( ${OS_NAME} STREQUAL "LINUX" AND CONDOR_PACKAGE_BUILD )

	# it's a smaller subset easier to differentiate.
	# check the operating system name

	if ( ${LINUX_NAME} STREQUAL  "Debian" )

		message (STATUS "Configuring for Debian package on ${LINUX_NAME}-${LINUX_VER}-${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}")

		##############################################################
		# For details on DEB package generation see:
		# http://www.itk.org/Wiki/CMake:CPackPackageGenerators#DEB_.28UNIX_only.29
		##############################################################
		set ( CPACK_GENERATOR "DEB" )

		# Use dkpg-shlibdeps to generate dependency list
		set ( CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON )
		# Enable debug message
		set ( CPACK_DEBIAN_PACKAGE_DEBUG 1 )

		set (CPACK_PACKAGE_FILE_NAME "${PACKAGE_NAME}_${PACKAGE_VERSION}-${PACKAGE_REVISION}_${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}" )
		string( TOLOWER "${CPACK_PACKAGE_FILE_NAME}" CPACK_PACKAGE_FILE_NAME )

		set ( CPACK_DEBIAN_PACKAGE_SECTION "contrib/misc")
		set ( CPACK_DEBIAN_PACKAGE_PRIORITY "extra")
		set ( CPACK_DEBIAN_PACKAGE_DESCRIPTION "${CPACK_DEBIAN_DESCRIPTION_SUMMARY}")
		set ( CPACK_DEBIAN_PACKAGE_MAINTAINER "Condor Team <${CPACK_PACKAGE_CONTACT}>" )
		set ( CPACK_DEBIAN_PACKAGE_VERSION "${PACKAGE_VERSION}-${PACKAGE_REVISION}")
		set ( CPACK_DEBIAN_PACKAGE_HOMEPAGE "${URL}")
		set ( CPACK_DEBIAN_PACKAGE_DEPENDS "python, adduser")

		#Control files
		set( CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA
			"${CMAKE_CURRENT_SOURCE_DIR}/build/packaging/debian/postinst"
			"${CMAKE_CURRENT_SOURCE_DIR}/build/packaging/debian/postrm"
			"${CMAKE_CURRENT_SOURCE_DIR}/build/packaging/debian/preinst"
			"${CMAKE_CURRENT_SOURCE_DIR}/build/packaging/debian/prerm"
			"${CMAKE_CURRENT_SOURCE_DIR}/build/packaging/debian/conffiles")

		#Directory overrides
		#Same as RPM
		set( C_BIN			usr/bin )
		set( C_LIB			usr/lib/condor )
		set( C_SBIN			usr/sbin )
		set( C_INCLUDE		usr/include/condor )
		set( C_MAN			usr/share/man )
		set( C_SRC			usr/src)
		set( C_SQL			usr/share/condor/sql)
		set( C_INIT			etc/init.d )
		set( C_ETC			etc/condor )
		set( C_CONFIGD		etc/condor/config.d )

		#Debian specific
		set( C_ETC_EXAMPLES	usr/share/doc/condor/etc/examples )
		set( C_DOC			usr/share/doc/condor )
		set( C_LIBEXEC		usr/lib/condor/libexec )
		set( C_SYSCONFIG	etc/default )

		#Because CPACK_PACKAGE_DEFAULT_LOCATION is set to "/" somewhere, so we have to set prefix like this
		#This might break as we move to newer version of CMake
		set( CMAKE_INSTALL_PREFIX "")
		set( CPACK_SET_DESTDIR "ON")

		# Processing control files
		add_subdirectory(build/packaging/debian)

	elseif ( RPM_SYSTEM_NAME )
		# This variable will be defined if the platfrom support RPM
		message (STATUS "Configuring RPM package on ${LINUX_NAME}-${LINUX_VER}-${SYS_ARCH}")

		##############################################################
		# For details on RPM package generation see:
		# http://www.itk.org/Wiki/CMake:CPackPackageGenerators#RPM_.28Unix_Only.29
		##############################################################

		set ( CPACK_GENERATOR "RPM" )
		#set ( CPACK_SOURCE_GENERATOR "RPM" )

		#Enable trace during packaging
		set(CPACK_RPM_PACKAGE_DEBUG 1)

		set (CPACK_PACKAGE_FILE_NAME "${CONDOR_VER}-${PACKAGE_REVISION}-${RPM_SYSTEM_NAME}-${SYS_ARCH}" )
		string( TOLOWER "${CPACK_PACKAGE_FILE_NAME}" CPACK_PACKAGE_FILE_NAME )

		set ( CPACK_RPM_PACKAGE_RELEASE ${PACKAGE_REVISION})
		set ( CPACK_RPM_PACKAGE_GROUP "Applications/System")
		set ( CPACK_RPM_PACKAGE_LICENSE "Apache License, Version 2.0")
		set ( CPACK_RPM_PACKAGE_VENDOR ${CPACK_PACKAGE_VENDOR})
		set ( CPACK_RPM_PACKAGE_URL ${URL})
		set ( CPACK_RPM_PACKAGE_DESCRIPTION ${CPACK_PACKAGE_DESCRIPTION})
		set ( CPACK_RPM_SPEC_IGNORE_FILES
			"/etc/condor/condor_config"
			"/etc/condor/condor_config.local"
			"/etc/init.d/condor")

		PackageDate( RPM CPACK_RPM_DATE)

		#Specify SPEC file template
		set(CPACK_RPM_USER_BINARY_SPECFILE "${CMAKE_CURRENT_SOURCE_DIR}/build/packaging/rpm/condor.spec.in")

		#Directory overrides
		set( C_BIN			usr/bin )
		set( C_LIB			usr/lib/condor )
		set( C_LIBEXEC		usr/libexec/condor )
		set( C_SBIN			usr/sbin )
		set( C_INCLUDE		usr/include/condor )
		set( C_MAN			usr/share/man )
		set( C_SRC			usr/src)
		set( C_SQL			usr/share/condor/sql)
		set( C_INIT			etc/init.d )
		set( C_ETC			etc/condor )
		set( C_CONFIGD		etc/condor/config.d )
		set( C_ETC_EXAMPLES	usr/share/doc/${CONDOR_VER}/etc/examples )
		set( C_DOC			usr/share/doc/${CONDOR_VER} )

		#Because CPACK_PACKAGE_DEFAULT_LOCATION is set to "/" somewhere, so we have to set prefix like this
		#This might break as we move to newer version of CMake
		set(CMAKE_INSTALL_PREFIX "")
		set(CPACK_SET_DESTDIR "ON")

	endif()

	# Generate empty folder to ship with package
	#Local dir
	file (MAKE_DIRECTORY execute temp )
	add_custom_target(	change_execute_folder_permission
						ALL
						WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
						COMMAND chmod 1777 execute
						DEPENDS execute)
	install(DIRECTORY	execute
			DESTINATION	"${C_LOCAL_DIR}"
			DIRECTORY_PERMISSIONS	USE_SOURCE_PERMISSIONS)
	install(DIRECTORY	temp
			DESTINATION	"${C_LOCAL_DIR}/spool")
	install(DIRECTORY	temp
			DESTINATION	"${C_CONFIGD}")
	install(DIRECTORY	temp
			DESTINATION	"${C_LOCK_DIR}")
	install(DIRECTORY	temp
			DESTINATION	"${C_LOG_DIR}")
	install(DIRECTORY	temp
			DESTINATION	"${C_RUN_DIR}")
	#install(DIRECTORY	temp
	#	DESTINATION	"${C_MAN}")
	install(DIRECTORY	temp
			DESTINATION	"${C_INCLUDE}")
	install(DIRECTORY	temp
			DESTINATION	"${C_LIB}")

endif()

