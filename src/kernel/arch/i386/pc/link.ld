/*OUTPUT_FORMAT("binary")*/
ENTRY (start)
/*
begin_phys_addres = 0x00100000;

_MH_MAGIC = 0x1BADB002;
_MH_FLAGS = 0x00000003;
kernel_pma = 1M;
*/
SECTIONS
{
	. = 0x00100000 ;
	.text :
	{
		code = .; _code = .; __code = .;
		/* Multiboot Header Gostaria que isso desse certo
		LONG(_MH_MAGIC)
		LONG(_MH_FLAGS)
		LONG(-(_MH_MAGIC + _MH_FLAGS))

		LONG(0)
		LONG(code)
		LONG(bss)
		LONG(end)
		LONG(rodata)
		LONG(start)
*/
		*(.header*) ;
		*(.text*) ;

		*(.gnu.linkonce.t*) ;
		. = ALIGN(4096);
	} = 0x00100000

	.rodata : ALIGN(4096)
/*	AT(begin_phys_addres+(rodata-code)) */
	{
		rodata = .; _rodata = .; __rodata = .;

		start_ctors = .;
		*(SORT(.ctors*))
		end_ctors = .;

		start_dtors = .;
		*(SORT(.dtors*))
		end_dtors = .;

		*(.rodata*)
		*(.gnu.linkonce.r*)
		. = ALIGN(4096);
	}

	.data : ALIGN(4096)
	/* : AT(begin_phys_add+(data-code)) */
	{
		data = .; _data = .; __data = .;

		*(.data)
		*(.gnu.linkonce.d*)
		. = ALIGN(4096);
	}

	.bss : ALIGN(4096)
	/* : AT( begin_phys_add+(bss-code) )*/
	{
		bss = .; _bss = .; __bss = .;
		sbss = .;
		*(COMMON)
		*(.bss)
		*(.gnu.linkonce.b*)
		ebss = .;
		. = ALIGN(4096);
	}

	end = .; _end = .; __end = .;

	/*
	/DISCARD/ :
	{
		*(.comment)
		*(.eh_frame) /* discard this, unless you are implementing runtime support for C++ exceptions. * /
		. = ALIGN(4096);
	}*/
}

