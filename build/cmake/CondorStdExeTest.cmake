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

MACRO (CONDOR_STD_EXE_TEST _CNDR_TARGET _COMPILER _SRCS _LINK_FLAGS )

	if (STD_UNIVERSE AND BUILD_TESTS)

		# so normally you should be able to pass the archive directly to 
		foreach(_file ${_SRCS})
			if(objs_${_CNDR_TARGET})
				set(objs_${_CNDR_TARGET} ${_file}.o)
			else()
				 set(objs_${_CNDR_TARGET} ${objs_${_CNDR_TARGET}};${_file}.o)
			endif()
		endforeach()

		#dprint("SRCS=${_SRCS}  objs_${_CNDR_TARGET}=${objs_${_CNDR_TARGET}}")
		# putput from condor_compile
		add_library( ${_CNDR_TARGET} STATIC EXCLUDE_FROM_ALL ${_SRCS})

		command_target( arx_${_CNDR_TARGET} ar "-x;lib${_CNDR_TARGET}.a" "${objs_${_CNDR_TARGET}}")
		add_dependencies( arx_${_CNDR_TARGET} ${_CNDR_TARGET})

		command_target( cc_${_CNDR_TARGET} ${CONDOR_COMPILE} "-condor_lib;${STDU_LIB_LOC};-condor_ld_dir;${STDU_LIB_LOC};${_COMPILER};-o;${_CNDR_TARGET}.cndr.exe;${objs_${_CNDR_TARGET}};${_LINK_FLAGS}" "${_CNDR_TARGET}.cndr.exe" )
		add_dependencies( cc_${_CNDR_TARGET} arx_${_CNDR_TARGET})

		command_target( ca_${_CNDR_TARGET} ${CONDOR_ARCH_LINK} "${_CNDR_TARGET}.cndr.exe" "${_CNDR_TARGET}.cndr.exe.Linux.${SYS_ARCH}")
		add_dependencies( ca_${_CNDR_TARGET} cc_${_CNDR_TARGET})

		append_var(CONDOR_TESTS ca_${_CNDR_TARGET})
		set_target_properties( cc_${_CNDR_TARGET} ca_${_CNDR_TARGET} arx_${_CNDR_TARGET} PROPERTIES EXCLUDE_FROM_ALL TRUE)

	endif()

ENDMACRO(CONDOR_STD_EXE_TEST)
