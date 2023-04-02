global loader                   ; the entry symbol for ELF

; GRUB will transfer control to the operating system by jumping to a position in memory. Before the jump,
; GRUB will look for a magic number to ensure that it is actually jumping to an OS and not some random code.
; This magic number is part of the multiboot specification which GRUB adheres to.
MAGIC_NUMBER equ 0x1BADB002     ; define the magic number constant
FLAGS        equ 0x0            ; multiboot flags(which indicates that no special features are enabled for the bootloader.)
CHECKSUM     equ -MAGIC_NUMBER  ; calculate the checksum (magic number + checksum + flags should equal 0)

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
    mov eax, 0xCAFEBABE         ; place the number 0xCAFEBABE in the register eax
.loop:
    jmp .loop                   ; loop forever