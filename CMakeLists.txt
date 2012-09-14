cmake_minimum_required (VERSION 2.8)
PROJECT (miniOS)

add_subdirectory(src/kernel/)

FUNCTION(LOAD_PROFILE ISA PLATFORM)
	# Obtain sources for the ISA
	FILE(GLOB ISA_SRCS "${ISA}/*.c" "${ISA}/*.cpp" "src/kernel/${ISA}/*.asm")
	FILE(GLOB PLATFORM_SRCS "${ISA}/${PLATFORM}/*.cpp" "${ISA}/${PLATFORM}/*.c" "${ISA}/${PLATFORM}/*.asm")
  
	# Load flags associated with ISA and Profile
	INCLUDE("src/kernel/${ISA}/flags.cmake")
	INCLUDE("src/kernel/${ISA}/${PLATFORM}/flags.cmake")
	# From the future
	#INCLUDE("${ISA}/flags.cmake")
	#INCLUDE("${ISA}/${PLATFORM}/flags.cmake")

	# Now export our output variables
	if( EXISTS "${ISA}/${PLATFORM}/link.ld" )
		SET(PLATFORM_LINK "${ISA}/${PLATFORM}/link.ld")
	elseif( EXISTS "${ISA}/link.ld" )
		SET(PLATFORM_LINK "${ISA}/link.ld")
	elseif( NOT DEFINED PLATFORM_LINK )
		message(STATUS "WARNING: no link(maybe link.ld) defenide for ISA ${ISA} or plataform ${PLATFORM}")
	endif( EXISTS "${ISA}/${PLATFORM}/link.ld" )

	SET(ISA_SRCS ${ISA_SRCS} PARENT_SCOPE)
	SET(PLATFORM_SRCS ${PLATFORM_SRCS} PARENT_SCOPE)

	# And specific flags
	SET(ISA_C_FLAGS ${ISA_C_FLAGS} PARENT_SCOPE)
	SET(ISA_CPP_FLAGS ${ISA_C_FLAGS} PARENT_SCOPE)
	SET(ISA_ASM_FLAGS ${ISA_ASM_FLAGS} PARENT_SCOPE)
ENDFUNCTION(LOAD_PROFILE)

#INCLUDE_DIRECTORIES (src)

#SET (CMAKE_CXX_FLAGS "-nostdlib -nostdinc -fno-builtin -fno-stack-protector")
#SET (CMAKE_C_FLAGS "-nostdlib -nostdinc -fno-builtin -fno-stack-protector")
#SET (CMAKE_EXE_LINKER_FLAGS "-T${CMAKE_CURRENT_LIST_DIR}/src/link.ld")
#SET (CMAKE_ASM_NASM_OBJECT_FORMAT "elf")

#LOAD_PROFILE("i386" "pc")

SET (CMAKE_C_FLAGS "${ISA_C_FLAGS} ${PLATFORM_C_FLAGS}")
SET (CMAKE_CXX_FLAGS "${ISA_CXX_FLAGS} ${PLATFORM_CXX_FLAGS}")
if( DEFINED PLATFORM_LINK )
	SET(CMAKE_EXE_LINKER_FLAGS "-T${PLATFORM_LINK} -N ${ISA_LINKER_FLAGS} ${PLATFORM_LINKER_FLAGS}")
else( DEFINED PLATFORM_LINK )
	SET(CMAKE_EXE_LINKER_FLAGS "-N ${ISA_LINKER_FLAGS} ${PLATFORM_LINKER_FLAGS}")
endif( DEFINED PLATFORM_LINK )

#SET (CMAKE_ASM_NASM_OBJECT_FORMAT "elf")

ENABLE_LANGUAGE (ASM_NASM)

#FILE (GLOB_RECURSE miniOS_SOURCES "src/kernel/*.c" "src/*.cpp" "src/*.asm")
FILE (GLOB_RECURSE miniOS_SOURCES ${ISA_SOURCE} ${PLATFORM_SOUCER})

ADD_EXECUTABLE(miniOS
	${miniOS_SOURCES})

