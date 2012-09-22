# Set flags of include directory with hearders file for libc
set(LIBC_INCLUDE_DIRS
	"${PROJECT_SOURCE_DIR}/src/libc/PDClib/build/includes"
	"${PROJECT_SOURCE_DIR}/src/libc/PDClib/pdclib/internals"
	"${PROJECT_SOURCE_DIR}/src/libc/PDClib/pdclib/platform/example/internals/"
	)

# Now export our output variables
set(LIBC_STATIC "${PROJECT_SOURCE_DIR}/src/libc/PDClib/libc.a")

