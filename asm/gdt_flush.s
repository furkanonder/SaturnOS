global gdt_flush    ; make the label gdt_flush visible outside this file

; gdt_flush - Flush the global descriptor table (GDT).
; stack: [esp + 4] the address of the first entry in the GDT
;        [esp    ] the return address
gdt_flush:
    cli                 ; clear the interrupt flag
    mov eax, [esp + 4]  ; load the address of the GDT into register eax
    lgdt [eax]          ; load the GDT

    ; 0x10 is our data segment register in the GDT.
    mov ax, 0x10
    ; The processor has six 16-bit segment registers: cs, ss, ds, es, gs and fs. The register cs is the code segment
    ; register and specifies the segment to use when fetching instructions. The register ss is used whenever accessing
    ; the stack (through the stack pointer esp), and ds is used for other data accesses. The OS is free to use the
    ; registers es, gs and fs however it want. Load all data segment registers.
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov gs, ax
    mov fs, ax

    ; To load cs we have to do a "far jump". A far jump is a jump where we explicitly specify the full 48-bit logical
    ; address: the segment selector to use and the absolute address to jump to. It will first set cs to 0x08 (0x08 is
    ; the code segment register in the GDT) and then jump to flush_cs using its absolute address.
    jmp 0x08:flush_cs

flush_cs:
    ; now we've changed cs to 0x08
    ret     ; return to the calling function
