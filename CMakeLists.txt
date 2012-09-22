cmake_minimum_required(VERSION 2.6)
PROJECT(miniOS)

#The version number.
set(miniOS_VERSION_MAJOR 0)
set(miniOS_VERSION_MINOR 0)
set(miniOS_VERSION_MINOR_FIX 1)

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

add_subdirectory(src/libc/)

LOAD_COMPILER_PROFILE("gcc")

LOAD_ISA_PROFILE("i386" "pc")

LOAD_LIBC_PROFILE("PDClib")

set(CMAKE_C_FLAGS
	"${ISA_C_FLAGS} ${PLATFORM_C_FLAGS} ${COMPILER_C_FLAGS}"
)
set(CMAKE_CXX_FLAGS
	"${ISA_CXX_FLAGS} ${PLATFORM_CXX_FLAGS} ${COMPILER_CXX_FLAGS}"
)

if( DEFINED PLATFORM_LINK )
	set(CMAKE_EXE_LINKER_FLAGS
			  "-T${PLATFORM_LINK} ${ISA_LINKER_FLAGS} ${PLATFORM_LINKER_FLAGS} ${COMPILER_LINKER_FLAGS}"
	)
elseif( NOT DEFINED PLATFORM_LINK )
	set(CMAKE_EXE_LINKER_FLAGS
		"${ISA_LINKER_FLAGS} ${PLATFORM_LINKER_FLAGS} ${COMPILER_LINKER_FLAGS}"
	)
endif( DEFINED PLATFORM_LINK )

add_library(libc_static STATIC IMPORTED)
set_property(TARGET libc_static PROPERTY IMPORTED_LOCATION "${LIBC_STATIC}")

FILE(GLOB GENERIC_KERNEL_SOURCES 
	"src/kernel/*.asm"
	"src/kernel/*.cpp"
	"src/kernel/*.c"
)

ADD_EXECUTABLE(miniOS
	${ISA_SOURCES}
	${PLATFORM_SOURCES}
	${COMPILER_SOURCES}
	${GENERIC_KERNEL_SOURCES}
)
target_link_libraries(miniOS libc_static)

if(UNIX)
	add_custom_target(distclean @echo cleaning for source distribution)
	add_custom_command(
		COMMENT	"distribution clean"
		DEPENDS	clean
		COMMAND	sh
		ARGS		-c '(for x in cmake.depends cmake_install.cmake cmake.check_depends CMakeCache.txt cmake.check_cache Makefile gmon.out '*~' CMakeTmp .backups CMakeFiles config.h miniOS \; do find . -iname \"$$x\" \; done ) | xargs -n1 rm -rf'
		TARGET  distclean
	)
ENDIF(UNIX)

