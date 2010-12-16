 ###############################################################
 # 
 # Copyright (C) 1990-2010, Redhat. 
 # 
 # Licensed under the Apache License, Version 2.0 (the "License"); you 
 # may not use this file except in compliance with the License.  You may 
 # obtain a copy of the License at 
 # 
 #    http://www.apache.org/licenses/LICENSE-2.0 
 # 
 # Unless required by applicable law or agreed to in writing, software 
 # distributed under the License is distributed on an "AS IS" BASIS, 
 # WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 # See the License for the specific language governing permissions and 
 # limitations under the License. 
 # 
 ############################################################### 

MACRO (CONDOR_EXE _CNDR_TARGET _SRCS _INSTALL_LOC _LINK_LIBS _COPY_PDBS)

	add_executable( ${_CNDR_TARGET} ${_SRCS})

	condor_set_link_libs( ${_CNDR_TARGET} "${_LINK_LIBS}")

	set(${_CNDR_TARGET}_loc ${_INSTALL_LOC})

	if (${_CNDR_TARGET}_loc)
		install (TARGETS ${_CNDR_TARGET} DESTINATION ${_INSTALL_LOC} )
		#dprint ("${_CNDR_TARGET} install destination (${CMAKE_INSTALL_PREFIX}/${_INSTALL_LOC})")
	endif()
	
	# the following will install the .pdb files, some hackery needs to occur because of build configuration is not known till runtime.
	if ( WINDOWS )		
		# Finally, get it installed.
		if ( ${_COPY_PDBS} )
			INSTALL(CODE "FILE(INSTALL DESTINATION \"\${CMAKE_INSTALL_PREFIX}/${_INSTALL_LOC}\" TYPE EXECUTABLE FILES \"${CMAKE_CURRENT_BINARY_DIR}/\${CMAKE_INSTALL_CONFIG_NAME}/${_CNDR_TARGET}.pdb\")")
		endif ()
		
		set_property( TARGET ${_CNDR_TARGET} PROPERTY FOLDER "executables" )
	endif( WINDOWS )

ENDMACRO (CONDOR_EXE)
