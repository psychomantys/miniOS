cmake_minimum_required(VERSION 2.6)
PROJECT(miniOS)

#The version number.
set(miniOS_VERSION_MAJOR 0)
set(miniOS_VERSION_MINOR 0)
set(miniOS_VERSION_MINOR_FIX 1)
set(miniOS_VERSION_CUSTOM 0 CACHE STRING "Custom version string for kernel")

set(PROFILE_COMPILER "gcc" CACHE STRING "Select compiler profile for use in kernel. Options in directory: ${PROJECT_BINARY_DIR}/src/kernel/compiler")
set(PROFILE_LIBC "PDClib" CACHE STRING "Select libc profile for use in kernel to provide basic C functions. Options in directory: ${PROJECT_BINARY_DIR}/src/libc/")
set(PROFILE_LIBCPP "NewLib" CACHE STRING "Select libcxx profile for use in kernel to provide basic CPP functions. Options in directory: ${PROJECT_BINARY_DIR}/src/libcxx/")
set(PROFILE_ISA "i386" CACHE STRING "Select ISA profile for use in kernel. Options in directory: ${PROJECT_BINARY_DIR}/src/kernel/arch/")
set(PROFILE_PLATAFORM "pc" CACHE STRING "Select plataform profile for use in kernel. Options in directory: ${PROJECT_BINARY_DIR}/src/kernel/arch/$ISA_DIR")

# configure a header file to pass some of the CMake settings
# to the source code
configure_file(
	"${PROJECT_SOURCE_DIR}/src/config.hpp.in"
	"${PROJECT_BINARY_DIR}/src/config.hpp"
)

ENABLE_LANGUAGE(ASM_NASM)
ENABLE_LANGUAGE(ASM-ATT)

INCLUDE_DIRECTORIES(
	${PROJECT_BINARY_DIR}
	${PROJECT_BINARY_DIR}/src
#	${PROJECT_BINARY_DIR}/src/kernel
)

add_subdirectory(src/kernel/)
add_subdirectory(src/libc/)
add_subdirectory(src/libcxx/)

LOAD_COMPILER_PROFILE("${PROFILE_COMPILER}")

LOAD_ISA_PROFILE("${PROFILE_ISA}" "${PROFILE_PLATAFORM}")

LOAD_LIBC_PROFILE("${PROFILE_LIBC}")

#LOAD_LIBCXX_PROFILE("${PROFILE_LIBCPP}")

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

if( DEFINED LIBC_STATIC )
	add_library(libc_static STATIC IMPORTED)
	set_property(TARGET libc_static PROPERTY IMPORTED_LOCATION "${LIBC_STATIC}")
endif( DEFINED LIBC_STATIC )

if( DEFINED LIBCPP_STATIC )
	add_library(libcxx_static STATIC IMPORTED)
	set_property(TARGET libcxx_static PROPERTY IMPORTED_LOCATION "${LIBCXX_STATIC}")
endif( DEFINED LIBCPP_STATIC )

FILE(GLOB GENERIC_KERNEL_SOURCES 
	"src/kernel/*.asm"
	"src/kernel/*.s"
	"src/kernel/*.cpp"
	"src/kernel/kpp/*.cpp"
#	"src/kernel/*.c"
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
		ARGS		-c '(for x in cmake.depends cmake_install.cmake cmake.check_depends CMakeCache.txt cmake.check_cache Makefile gmon.out '*~' CMakeTmp .backups CMakeFiles config.hpp miniOS \; do find . -iname \"$$x\" \; done ) | xargs -n1 rm -rf'
		TARGET  distclean
	)
ENDIF(UNIX)

