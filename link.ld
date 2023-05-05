ENTRY(loader) /* the name of the entry label */

SECTIONS {
    /* We want GRUB to load the kernel at a memory address larger than or equal to 0x00100000 (1 megabyte (MB)),
     * because addresses lower than 1 MB are used by GRUB itself, BIOS and memory-mapped I/O. */
    .grub_sig 0x00100000 : AT(0x100000)
    {
      *(.grub_sig)
    }

    .text ALIGN (0x1000) :   /* align at 4 KB */
    {
        *(.text)             /* all text sections from all files */
    }

    .rodata ALIGN (0x1000) : /* align at 4 KB */
    {
        *(.rodata*)          /* all read-only data sections from all files */
    }

    .data ALIGN (0x1000) :   /* align at 4 KB */
    {
        *(.data)             /* all data sections from all files */
    }

    .bss ALIGN (0x1000) :    /* align at 4 KB */
    {
        *(COMMON)            /* all COMMON sections from all files */
        *(.bss)              /* all bss sections from all files */
    }
}