SET (ISA_C_FLAGS "-nostdlib -nostdinc -fno-builtin -fno-stack-protector")
SET (ISA_C_FLAGS "-nostdlib -nostdinc -fno-builtin -fno-stack-protector")
SET (ISA_EXE_LINKER_FLAGS "-T${CMAKE_CURRENT_LIST_DIR}/src/link.ld")

# Obtain sources for the ISA
FILE(GLOB ISA_SRCS "*.c" "*.cpp" "*.asm")

#SET(PLATFORM_LINK "${ISA}/link.ld")
FILE(GLOB ISA_SRCS "${ISA}/*.c" "${ISA}/*.cpp" "${ISA}/*.asm")

