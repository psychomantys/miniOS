
FUNCTION(LOAD_LIBC_PROFILE LIBC)
	set(LIBC_BASE_DIR "${PROJECT_SOURCE_DIR}/src/libc")

	# Load flags associated with used libc Profile.
	INCLUDE("${LIBC_BASE_DIR}/${LIBC}/flags.cmake")

	# Inlcude diretory with includes of C lib
	INCLUDE_DIRECTORIES(${LIBC_INCLUDE_DIRS})

	# Now export our output variables
	set(LIBC_STATIC ${LIBC_STATIC} PARENT_SCOPE)
ENDFUNCTION(LOAD_LIBC_PROFILE)

