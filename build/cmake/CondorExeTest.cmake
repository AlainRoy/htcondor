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

MACRO (CONDOR_EXE_TEST _CNDR_TARGET _SRCS _LINK_LIBS )

	if (BUILD_TESTS)

		set ( LOCAL_${_CNDR_TARGET} ${_CNDR_TARGET} )

		if ( WINDOWS )
			string (REPLACE ".exe" "" ${LOCAL_${_CNDR_TARGET}} ${LOCAL_${_CNDR_TARGET}})
		endif( WINDOWS )

		add_executable( ${LOCAL_${_CNDR_TARGET}} EXCLUDE_FROM_ALL ${_SRCS})

		set_target_properties( ${LOCAL_${_CNDR_TARGET}} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_TARGET_DIR} )

		condor_set_link_libs( ${LOCAL_${_CNDR_TARGET}} "${_LINK_LIBS}" )

		# will tack onto a global var which will be *all test targets.
		if ( CONDOR_TESTS )
			set ( CONDOR_TESTS "${CONDOR_TESTS};${_CNDR_TARGET}" )
		else( CONDOR_TESTS )
			set ( CONDOR_TESTS ${_CNDR_TARGET} )
		endif( CONDOR_TESTS )

		set ( CONDOR_TESTS ${CONDOR_TESTS} PARENT_SCOPE )

	endif(BUILD_TESTS)

ENDMACRO(CONDOR_EXE_TEST)
