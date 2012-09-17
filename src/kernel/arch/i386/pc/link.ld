
ENTRY (start)

SECTIONS
{
	. = 0x00100000;

	.text ALIGN (0x1000) :
	{
		code = .; _code = .; __code = .;
		*(.text)
		*(.gnu.linkonce.t*)
     . = ALIGN(4096);
	}

	.rodata :
	{
		start_ctors = .;
		*(SORT(.ctors*))  /* Note the "SORT" */
		end_ctors = .;

		start_dtors = .;
		*(SORT(.dtors*))
		end_dtors = .;

		*(.rodata*)
		*(.gnu.linkonce.r*)
     . = ALIGN(4096);
	}

	.data :
	{
		*(.data)
		*(.gnu.linkonce.d*)
     . = ALIGN(4096);
	}

	.bss :
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

/*	/DISCARD/ :
	{
		*(.comment)
		*(.eh_frame) /* discard this, unless you are implementing runtime support for C++ exceptions. * /
     . = ALIGN(4096);
	}*/
}

/*
ENTRY(start)
SECTIONS
{
  .text 0x100000 :
  {
    code = .; _code = .; __code = .;
    *(.text)
    . = ALIGN(4096);
  }

  .data :
  {
     data = .; _data = .; __data = .;
     *(.data)
     *(.rodata)
     . = ALIGN(4096);
  }

  .bss :
  {
    bss = .; _bss = .; __bss = .;
    *(.bss)
    . = ALIGN(4096);
  }

  end = .; _end = .; __end = .;
}
*/

