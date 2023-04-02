global loader                   ; the entry symbol for ELF
extern os_main

; GRUB will transfer control to the operating system by jumping to a position in memory. Before the jump,
; GRUB will look for a magic number to ensure that it is actually jumping to an OS and not some random code.
; This magic number is part of the multiboot specification which GRUB adheres to.
MAGIC_NUMBER equ 0x1BADB002     ; define the magic number constant
FLAGS        equ 0x0            ; multiboot flags(which indicates that no special features are enabled for the bootloader.)
CHECKSUM     equ -MAGIC_NUMBER  ; calculate the checksum (magic number + checksum + flags should equal 0)
KERNEL_STACK_SIZE equ 4096      ; size of stack in bytes

section .text                   ; start of the text (code) section
; The align 4 directive is used to ensure that the code is aligned on a 4-byte boundary.
; This is a common requirement for x86 and x86-64 architectures, and ensures that the code will work correctly
; on all platforms. By aligning the code to a 4-byte boundary, the code can be accessed more efficiently by the CPU,
; which can improve performance.
align 4
    dd MAGIC_NUMBER             ; write the magic number to the machine code,
    dd FLAGS                    ; the flags,
    dd CHECKSUM                 ; and the checksum

loader:                         ; the loader label (defined as entry point in linker script)
  ; We could point esp to a random area in memory since, so far, the only thing in the memory is GRUB, BIOS,
  ; the OS kernel and some memory-mapped I/O. This is not a good idea - we don’t know how much memory is available or
  ; if the area esp would point to is used by something else. A better idea is to reserve a piece of uninitialized
  ; memory in the bss section in the ELF file of the kernel. It is better to use the bss section instead of the data
  ; section to reduce the size of the OS executable. Since GRUB understands ELF, GRUB will allocate any memory
  ; reserved in the bss section when loading the OS.
  mov esp, kernel_stack + KERNEL_STACK_SIZE   ; point esp to the start of the stack (end of memory area)
  call os_main
.loop:
    jmp .loop                   ; loop forever


section .bss
align 4
kernel_stack:                   ; label points to beginning of memory
  ; There is no need to worry about the use of uninitialized memory for the stack, since it is not possible to read
  ; a stack location that has not been written (without manual pointer fiddling). A (correct) program can not pop an
  ; element from the stack without having pushed an element onto the stack first. Therefore, the memory locations of
  ; the stack will always be written to before they are being read.
  resb KERNEL_STACK_SIZE        ; reserve stack for the kernel