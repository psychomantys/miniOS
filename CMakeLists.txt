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

if(UNIX)
	add_custom_target(distclean @echo cleaning for source distribution)
	ADD_CUSTOM_COMMAND(
		COMMENT	"distribution clean"
		DEPENDS	clean
		COMMAND	sh
		ARGS		-c '(for x in cmake.depends cmake_install.cmake cmake.check_depends CMakeCache.txt cmake.check_cache Makefile gmon.out '*~' CMakeTmp .backups CMakeFiles config.h miniOS \; do find . -iname \"$$x\" \; done ) | xargs -n1 rm -rf'
		TARGET  distclean
)
ENDIF(UNIX)

