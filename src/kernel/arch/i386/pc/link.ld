
ENTRY (start)

SECTIONS
{
	. = 0x00100000;

	.text :
	{
		code = .; _code = .; __code = .;
		*(.text)
/*		*(.gnu.linkonce.t*)*/
		. = ALIGN(4096);
	}

	.rodata :
	{
		rodata = .; _rodata = .; __rodata = .;

		start_ctors = .;
		*(SORT(.ctors*))
		end_ctors = .;

		start_dtors = .;
		*(SORT(.dtors*))
		end_dtors = .;
		data = .; _data = .; __data = .;

		*(.rodata*)
/*		*(.gnu.linkonce.r*)*/
		. = ALIGN(4096);
	}

	.data :
	{
		*(.data)
/*		*(.gnu.linkonce.d*)*/
		. = ALIGN(4096);
	}

	.bss :
	{
		bss = .; _bss = .; __bss = .;
		sbss = .;
/*		*(COMMON)*/
		*(.bss)
/*		*(.gnu.linkonce.b*)*/
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

