set(COMPILER_C_FLAGS "-Wall -Wno-main -nostdlib -nostdinc -fno-builtin -nostartfiles -nodefaultlibs -fno-exceptions -fno-stack-protector -ffreestanding")
set(COMPILER_CXX_FLAGS "-Wall -Wno-main -nostdlib -nostdinc -fno-builtin -nostartfiles -nodefaultlibs -fno-exceptions -fno-rtti -fno-stack-protector -ffreestanding")
#set(COMPILER_CXX_FLAGS "-nostdlib -nostdinc -fno-builtin -fno-stack-protector -Wno-main -nostartfiles")
#set(COMPILER_LINKER_FLAGS "")

FILE(GLOB GLOB_COMPILER_SOURCES
	"src/kernel/compiler/gcc/quad/*.c"
	"src/kernel/compiler/gcc/*.c"
	"src/kernel/compiler/gcc/*.asm"
	"src/kernel/compiler/gcc/*.cpp"
)
set(COMPILER_SOURCES "${GLOB_COMPILER_SOURCES}")

