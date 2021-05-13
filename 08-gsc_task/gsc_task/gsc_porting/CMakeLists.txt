cmake_minimum_required (VERSION 3.5.1)

#####################################################################################################
#Decide MRV_PDK_SDK_HOME
#If not set the environment, show warnning and GUESS one
#####################################################################################################
if(DEFINED ENV{MRV_PDK_SDK_HOME})
	set(MRV_PDK_SDK_HOME $ENV{MRV_PDK_SDK_HOME} CACHE FILEPATH "MRV SDK path" FORCE)
	message("[info] MRV_PDK_SDK_HOME is defined! Use as SDK path.")
else()
	set(MRV_PDK_SDK_HOME ${CMAKE_CURRENT_SOURCE_DIR}/../../../ CACHE PATH "MRV SDK path" FORCE) #Assume SDK_HOME/Demo/reference/<this_proj>
	message("****************************************************************************************")
	message("* MRV_PDK_SDK_HOME is NOT defined! Use GUESSED path instead.                           *")
	message("* You can use MRV_PDK_SDK_HOME environment variable to assign SDK path.                *")
	message("****************************************************************************************")
endif()
message("[info] MRV_PDK_SDK_HOME = ${MRV_PDK_SDK_HOME}")


#####################################################################################################
#Utility CMake Macro
#####################################################################################################
include(${MRV_PDK_SDK_HOME}/cmake/utility.cmake)
#####################################################################################################
#CrossCompiling Toolchain
#####################################################################################################
include(${MRV_PDK_SDK_HOME}/cmake/toolchain.cmake)

#####################################################################################################
#Project language
#####################################################################################################
project (mrv C ASM)

set(CMAKE_BUILD_TYPE Release CACHE STRING
	"Choose the type of build, options are: None Debug Release MinSizeRel.")

#set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
#	"Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel.")
#####################################################################################################
#Regression tests
#####################################################################################################
include(${MRV_PDK_SDK_HOME}/cmake/tests.cmake)

#####################################################################################################
#Options
#####################################################################################################
set(CONFIG_MRV_ARCH "MRV55" CACHE STRING "MRV33/MRV55")
set_property(CACHE CONFIG_MRV_ARCH PROPERTY STRINGS ${MRV_ARCH_LIST})
set(CONFIG_FFT_LOG2_SIZE "2" CACHE STRING "FFT log2 size")
set_property(CACHE CONFIG_FFT_LOG2_SIZE PROPERTY STRINGS "2;5;8;12")

#####################################################################################################
#Dependent directories & libraries
#####################################################################################################
#Dependency 1: import mtkpdk, libmrv.a and librtos (from SDK)
include( ${MRV_PDK_SDK_HOME}/lib/cmake/mtkpdk-config.cmake)
include( ${MRV_PDK_SDK_HOME}/lib/cmake/librtos/libmrv-config.cmake)
#Dependency 2: linker script (from SDK)
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../../../src/librtos/CMakeLists.txt)
	add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/../../../src/librtos ${CMAKE_CURRENT_BINARY_DIR}/librtos )
	set(LINKER_SCRIPT_DIR ${CMAKE_CURRENT_BINARY_DIR}/librtos)
else()
	include(${MRV_PDK_SDK_HOME}/lib/cmake/librtos/librtos-config.cmake)
	set(LINKER_SCRIPT_DIR ${MRV_PDK_SDK_HOME}/lib/rtos)
endif()

add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/liboppo ${CMAKE_CURRENT_BINARY_DIR}/liboppo )

######################################################################################################
#Source code & include paths
#		1. Build project with all source files under ${SRC_DIR}
#		2. Add all sub-directories under ${SRC_DIR} into include path
#		2. Run "cmake .." to RE-generate Makefiles each time you add/remove source files
######################################################################################################
set(SRC_DIR	${CMAKE_CURRENT_SOURCE_DIR}/src)
#1:source files
file(GLOB_RECURSE CUSTOMER_SRC
		FOLLOW_SYMLINKS
		LIST_DIRECTORIES false
		${SRC_DIR}/*.c  ${SRC_DIR}/*.C ${SRC_DIR}/*.h ${SRC_DIR}/*.H ${SRC_DIR}/*.s ${SRC_DIR}/*.S
	)

#2:include paths
LIST_SUB_DIRECTORIES( CUSTOMER_INC ${SRC_DIR} )
include_directories( ${SRC_DIR} ${CUSTOMER_INC} )
SHOW_VAR(CUSTOMER_INC)

message("****************************************************************************************")
message("*[NOTE]:                                                                               *")
message("* Build project with all source files under:                                           *")
message("*    ${SRC_DIR}")
message("* Please run \"cmake ..\" again to re-generate Makefiles each time you add/remove source *")
message("* files.                                                                               *")
message("****************************************************************************************")
message("[info] Source Files  = ${CUSTOMER_SRC}")
message("[info] Include Paths = ${SRC_DIR};${CUSTOMER_INC}")



#####################################################################################################
#Common Compiler Definitions/Options
#	(Default Nothing, add your local flags here)
#####################################################################################################
#Cat.1: Compile Definitions : reference below sample to add your local directory definitions
#(add_compile_definitions() start support at 3.12.4)
add_definitions(
	#			-DMRV_DEBUG
	#		-DMRV_PROFILING
	#		-DFFT_LOG2_SIZE=${CONFIG_FFT_LOG2_SIZE}
			)

#Cat.2: Compile Options : reference below sample to add your local directory options
add_compile_options(
			-fsigned-char
			)


#####################################################################################################
#Targets
#####################################################################################################
#Target: mrv.elf
foreach(mrv_arch IN LISTS MRV_ARCH_LIST)
    	add_library(dsp_${mrv_arch} STATIC IMPORTED)
    	set_target_properties(dsp_${mrv_arch} PROPERTIES
    	  IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/liboppo/lib/libdsp_${mrv_arch}_v0_1.a"
   	   INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/liboppo/lib"
  	  )

	set(EXCLUDE_FROM_ALL )
	if(mrv_arch STREQUAL CONFIG_MRV_ARCH)
		set(MRV_ELF mrv.elf)
	else()
		set(MRV_ELF ${mrv_arch}.elf)
		if(NOT BUILD_TESTING)
			set(EXCLUDE_FROM_ALL EXCLUDE_FROM_ALL)
		endif(NOT BUILD_TESTING)
	endif(mrv_arch STREQUAL CONFIG_MRV_ARCH)

	add_executable			( ${MRV_ELF}
									${EXCLUDE_FROM_ALL}
									${CUSTOMER_SRC}
							)
	target_link_libraries	( ${MRV_ELF}
							oppo_${mrv_arch}
							dsp_${mrv_arch}
							mtkpdk
							rtos_${mrv_arch}
							mrv_${mrv_arch}
							-Wl,--script=${LINKER_SCRIPT_DIR}/link_${mrv_arch}.ld
							-Wl,-Map=${mrv_arch}.map
							)

	# Do bit-true check
	if(BUILD_TESTING)
		mrv_pdk_test_sim(oppo_${mrv_arch} ${MRV_ELF})
		set_tests_properties (oppo_${mrv_arch}
			PROPERTIES PASS_REGULAR_EXPRESSION "Test passed")
	endif(BUILD_TESTING)
endforeach(mrv_arch)
