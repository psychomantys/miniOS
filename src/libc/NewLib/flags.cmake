# Set flags of include directory with hearders file for libc
set(LIBC_INCLUDE_DIRS
	"${PROJECT_SOURCE_DIR}/src/libc/NewLib/build/newlib/libc/include"
	"${PROJECT_SOURCE_DIR}/src/libc/NewLib/build/targ-include"
	"${PROJECT_SOURCE_DIR}/src/libc/NewLib/extra_include"
)

# Now export our output variables
set(LIBC_STATIC "${PROJECT_SOURCE_DIR}/src/libc/NewLib/libc.a")

