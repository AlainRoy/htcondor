MACRO (CONDOR_EXE _CNDR_TARGET _SRCS _INSTALL_LOC _LINK_LIBS )

	add_executable( ${_CNDR_TARGET} ${_SRCS})

	condor_set_link_libs( ${_CNDR_TARGET} "${_LINK_LIBS}")

	install (TARGETS ${_CNDR_TARGET}
			 RUNTIME DESTINATION ${_INSTALL_LOC} )

	# the following will install the .pdb files, some hackery needs to occur because of build configuration is not known till runtime.
	if ( WINDOWS )		
		# Finally, get it installed.
		INSTALL(CODE "FILE(INSTALL DESTINATION \"\${CMAKE_INSTALL_PREFIX}/bin\" TYPE EXECUTABLE FILES \"${CMAKE_CURRENT_BINARY_DIR}/\${CMAKE_INSTALL_CONFIG_NAME}/${_CNDR_TARGET}.pdb\")")
	endif( WINDOWS )

ENDMACRO (CONDOR_EXE)