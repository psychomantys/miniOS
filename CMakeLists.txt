cmake_minimum_required (VERSION 2.8)
PROJECT(miniOS)

#The version number.
set(miniOS_VERSION_MAJOR 0)
set(miniOS_VERSION_MINOR 1)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file(
	"${PROJECT_SOURCE_DIR}/src/config.h.in"
	"${PROJECT_BINARY_DIR}/src/config.h"
)

ENABLE_LANGUAGE(ASM_NASM)

INCLUDE_DIRECTORIES(
	${PROJECT_BINARY_DIR}
	${PROJECT_BINARY_DIR}/src
	${PROJECT_BINARY_DIR}/src/kernel
)
add_subdirectory(src/kernel/)

LOAD_PROFILE("i386" "pc")

message(STATUS "${ISA_CXX_FLAGS} ${PLATFORM_CXX_FLAGS}")
SET (CMAKE_C_FLAGS "${ISA_C_FLAGS} ${PLATFORM_C_FLAGS}")
SET (CMAKE_CXX_FLAGS "${ISA_CXX_FLAGS} ${PLATFORM_CXX_FLAGS}")
if( DEFINED PLATFORM_LINK )
	SET(CMAKE_EXE_LINKER_FLAGS "-T${PLATFORM_LINK} ${ISA_LINKER_FLAGS} ${PLATFORM_LINKER_FLAGS}")
elseif( NOT DEFINED PLATFORM_LINK )
	SET(CMAKE_EXE_LINKER_FLAGS "${ISA_LINKER_FLAGS} ${PLATFORM_LINKER_FLAGS}")
endif( DEFINED PLATFORM_LINK )

FILE(GLOB GENERIC_KERNEL_SRCS 
		"src/kernel/*.cpp"
		"src/kernel/*.asm"
		"src/kernel/*.c")

set(miniOS_SOURCES ${ISA_SRCS} ${PLATFORM_SRCS} ${GENERIC_KERNEL_SRCS})

ADD_EXECUTABLE(miniOS
	${miniOS_SOURCES} )

