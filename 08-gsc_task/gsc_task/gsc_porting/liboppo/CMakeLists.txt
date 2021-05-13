cmake_minimum_required (VERSION 3.5.1)

#####################################################################################################
#Decide MRV_PDK_SDK_HOME
#If not set the environment, show warnning and GUESS one
#####################################################################################################
if(DEFINED ENV{MRV_PDK_SDK_HOME})
	set(MRV_PDK_SDK_HOME $ENV{MRV_PDK_SDK_HOME} CACHE FILEPATH "MRV SDK path" FORCE)
	message("[info] MRV_PDK_SDK_HOME is defined! Use as SDK path.")
else()
	set(MRV_PDK_SDK_HOME ${CMAKE_CURRENT_SOURCE_DIR}/../../../../ CACHE PATH "MRV SDK path" FORCE) #Assume SDK_HOME/Demo/reference/sample_fft/<this_proj>
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
#set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
#	"Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel.")

set(CMAKE_BUILD_TYPE Release CACHE STRING
	"Choose the type of build, options are: None Debug Release MinSizeRel.")

project (liboppo C ASM)
#####################################################################################################
#Options
#####################################################################################################
option(CONFIG_LOOKUP_TABLE
	"Use table lookup." OFF)


set(COMMON_COMPILE_OPTIONS
			--target=${CMAKE_SYSTEM_PROCESSOR}
			-ffunction-sections
			-fdata-sections
			-fsigned-char
			)

set(COMMON_LINK_OPTIONS
			--target=${CMAKE_SYSTEM_PROCESSOR}
			-Wl,--gc-sections
			-nostartfiles
			)

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

if(CONFIG_LOOKUP_TABLE)
	set(LUT_SRC ${CMAKE_CURRENT_SOURCE_DIR}/fft_table.c)
endif(CONFIG_LOOKUP_TABLE)

#2:include paths
LIST_SUB_DIRECTORIES( CUSTOMER_INC ${SRC_DIR} )
include_directories( ${SRC_DIR} ${CUSTOMER_INC} ${CMAKE_CURRENT_SOURCE_DIR}/lib)
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
if(CONFIG_LOOKUP_TABLE)
add_definitions(
			-DFFT_TABLE
			-DFIXED_POINT
			)
endif(CONFIG_LOOKUP_TABLE)

#Cat.2: Compile Options : reference below sample to add your local directory options
add_compile_options(
			)


#####################################################################################################
#Targets
#####################################################################################################
#Target: liboppo_MRVxx.a
foreach(mrv_arch IN LISTS MRV_ARCH_LIST)
add_library			( oppo_${mrv_arch}
			STATIC
			${CUSTOMER_SRC}
			${LUT_SRC}
			)

target_include_directories( oppo_${mrv_arch}
			INTERFACE
			$<BUILD_INTERFACE:${SRC_DIR}>
	)
add_library(dsp_${mrv_arch} STATIC IMPORTED)
set_target_properties(dsp_${mrv_arch} PROPERTIES
	IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/liboppo/lib/libdsp_${mrv_arch}_v0_1.a"
	INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/liboppo/lib"
	)
if(CONFIG_LOOKUP_TABLE)
target_compile_definitions( oppo_${mrv_arch}
			INTERFACE
			-DFFT_TABLE
			-DFIXED_POINT
)
endif(CONFIG_LOOKUP_TABLE)
target_compile_definitions( oppo_${mrv_arch}
			PUBLIC
			-D${mrv_arch}
)
target_compile_options( oppo_${mrv_arch}
			PRIVATE
			${COMMON_COMPILE_OPTIONS}
			-DMRV
			-mcpu=${mrv_arch}${${mrv_arch}REV}
			)
target_link_libraries( dsp_${mrv_arch}
			INTERFACE
			oppo_${mrv_arch}
			${COMMON_LINK_OPTIONS}
			-mcpu=${mrv_arch}${${mrv_arch}REV}
			)
endforeach(mrv_arch)

if(CONFIG_LOOKUP_TABLE)
#Target: gentable
	add_executable		( gentable EXCLUDE_FROM_ALL gentable.c )
	target_link_libraries	( gentable
							-std=c99
							-DFIXED_POINT
							-lm
							)

	add_custom_target (
		table
		COMMAND ./gentable
		DEPENDS gentable
		)
endif(CONFIG_LOOKUP_TABLE)
