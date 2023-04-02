C_FILES = $(shell find . -type f -name '*.c')
ASM_FILES = $(shell find . -type f -name '*.s')
OBJECTS = ${C_FILES:.c=.o} ${ASM_FILES:.s=.o}

CC = gcc
# -m32: sets int, long, and pointer types to 32 bits, and generates code that runs on any i386 system.
# -nostdlib: do not use the standard system startup files or libraries when linking.
# -nostdinc: do not search the standard system directories for header files.
# -fno-builtin: don’t recognize built-in functions that do not begin with ‘__builtin_’ as prefix.
# -fno-stack-protector: disables the compiler's built-in stack protection mechanisms.
# -nostartfiles: do not use the standard system startup files when linking.
# -nodefaultlibs: do not use the standard system libraries when linking.
# -Wall: this enables all the warnings about constructions that some users consider questionable, \
#		 and that are easy to avoid (or modify to prevent the warning), even in conjunction with macros.
# -Wextra: this enables some extra warning flags that are not enabled by -Wall.
# -Werror: make all warnings into errors.
# -c: compile or assemble the source files, but do not link. The linking stage simply is not done. \
 	  The ultimate output is in the form of an object file for each source file.
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
		 -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c

# -melf_i386: indicates that the output file should be in the ELF format for the Intel 386 architecture.
# -T: specify the linker script to use for the link-edit process.
LDFLAGS = -T link.ld -melf_i386
AS = nasm
# -f: specify the output format of the assembled code.
ASFLAGS = -f elf

all: kernel.elf

kernel.elf: $(OBJECTS)
	ld $(LDFLAGS) $(OBJECTS) -o kernel.elf

# -R: Generate SUSP and RR records using the Rock Ridge protocol to further describe the files on the ISO9660 filesystem.
# -b: Specifies the path and filename of the boot image to be used when making an El Torito bootable CD for x86 PCs.
# -no-emul-boot: Specifies that the boot image used to create El Torito bootable CDs is a "no emulation" image.
#  				 The system will load and execute this image without performing any disk emulation.
# -boot-load-size: Specifies the number of "virtual" (512-byte) sectors to load in no-emulation mode.
# -A: Specifies a text string that will be written into the volume header. This should describe the application that will be on the disc.
# -input-charset: Input charset that defines the characters used in local filenames.
# -quiet: This makes genisoimage even less verbose. No progress output will be provided.
# -boot-info-table: Adds a special table to the ISO image that provides information about the boot loader.
# -o: specifies the name of the output file (the ISO image)
os.iso: kernel.elf
	cp kernel.elf iso/boot/
	genisoimage -R \
			  -b boot/grub/stage2_eltorito    \
			  -no-emul-boot                   \
			  -boot-load-size 4               \
			  -A SaturnOS                     \
			  -input-charset utf8             \
			  -quiet                          \
			  -boot-info-table                \
			  -o SaturnOS.iso                 \
			  iso

run: os.iso
	bochs -f bochsrc.txt -q

%.o: %.c
	$(CC) $(CFLAGS)  $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

clean:
	find . -type f -name '*.o' -delete
	rm -f kernel.elf iso/boot/kernel.elf SaturnOS.iso com1.out bochslog.txt
