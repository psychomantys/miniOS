
FUNCTION(LOAD_PROFILE ISA PLATFORM)
	# Obtain sources for the ISA
	FILE(GLOB ISA_SRCS "${ISA}/*.c" "${ISA}/*.cpp" "src/kernel/${ISA}/*.asm")
	FILE(GLOB PLATFORM_SRCS "${ISA}/${PLATFORM}/*.cpp" "${ISA}/${PLATFORM}/*.c" "${ISA}/${PLATFORM}/*.asm")

	# Load flags associated with ISA and Profile
	INCLUDE("${ISA}/flags.cmake")
	INCLUDE("${ISA}/${PLATFORM}/flags.cmake")

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

LOAD_PROFILE("i386" "pc")

