extern interrupt_handler            ; make the label interrupt_handler visible outside this file

%macro no_error_code_interrupt_handler 1
global interrupt_handler_%1
interrupt_handler_%1:
    push dword 0                    ; Since not all interrupts produce an error code the value 0 will be added as the
                                    ; "error code" for interrupts without an error code.
    push dword %1                   ; push the interrupt number
    jmp common_interrupt_handler    ; jump to the common handler
%endmacro

%macro error_code_handler 1
global interrupt_handler_%1
interrupt_handler_%1:
    push dword %1                   ; push the interrupt number
    jmp common_interrupt_handler    ; jump to the common handler
%endmacro

common_interrupt_handler:           ; the common parts of the generic interrupt handler
    ; save the registers
    push  eax
    push  ebx
    push  ecx
    push  edx
    push  esp
    push  ebp
    push  esi
    push  edi

    ; call the C function
    call  interrupt_handler

   ; restore the registers
    pop   edi
    pop   esi
    pop   ebp
    pop   esp
    pop   edx
    pop   ecx
    pop   ebx
    pop   eax

    ; Add 8 to esp (because of the error code and the interrupt number pushed earlier)
    add esp, 8

    ; The instruction iret expects the stack to be the same as at the time of the interrupt. Therefore, any values
    ; pushed onto the stack by the interrupt handler must be popped. Before returning, iret restores eflags by popping
    ; the value from the stack and then finally jumps to cs:eip as specified by the values on the stack.
    iret

no_error_code_interrupt_handler 0   ; Divide Error
no_error_code_interrupt_handler 1   ; Debug Exception
no_error_code_interrupt_handler 2   ; NMI Interrupt
no_error_code_interrupt_handler 3   ; Breakpoint
no_error_code_interrupt_handler 4   ; Overflow
no_error_code_interrupt_handler 5   ; BOUND Range Exceeded
no_error_code_interrupt_handler 6   ; Invalid Opcode (Undefined Opcode)
no_error_code_interrupt_handler 7   ; Device Not Available (No Math Coprocessor)
error_code_handler 8                ; Double fault
no_error_code_interrupt_handler 9   ; Coprocessor Segment Overrun (reserved)
error_code_handler 10               ; Invalid TSS
error_code_handler 11               ; Segment Not Present
error_code_handler 12               ; Stack-Segment Fault
error_code_handler 13               ; General Protection
error_code_handler 14               ; Page Fault
no_error_code_interrupt_handler 15  ; (Intel reserved. Do not use.)
no_error_code_interrupt_handler 16  ; x87 FPU Floating-Point Error (Math Fault)
error_code_handler 17               ; Alignment Check
no_error_code_interrupt_handler 18  ; Machine Check
no_error_code_interrupt_handler 19  ; SIMD Floating-Point Exception
no_error_code_interrupt_handler 20  ; Virtualization Exception
no_error_code_interrupt_handler 21  ; Intel reserved. Do not use.
no_error_code_interrupt_handler 22  ; Intel reserved. Do not use.
no_error_code_interrupt_handler 23  ; Intel reserved. Do not use.
no_error_code_interrupt_handler 24  ; Intel reserved. Do not use.
no_error_code_interrupt_handler 25  ; Intel reserved. Do not use.
no_error_code_interrupt_handler 26  ; Intel reserved. Do not use.
no_error_code_interrupt_handler 27  ; Intel reserved. Do not use.
no_error_code_interrupt_handler 28  ; Intel reserved. Do not use.
no_error_code_interrupt_handler 29  ; Intel reserved. Do not use.
no_error_code_interrupt_handler 30  ; Intel reserved. Do not use.
no_error_code_interrupt_handler 31  ; Intel reserved. Do not use.

; IRQs
no_error_code_interrupt_handler 32  ; Programmable Interrupt Timer Interrupt
no_error_code_interrupt_handler 33  ; Keyboard interrupt handler
no_error_code_interrupt_handler 34  ; Cascade (used internally by the two PICs. never raised)
no_error_code_interrupt_handler 35  ; COM2 (if enabled)
no_error_code_interrupt_handler 36  ; COM1 (if enabled)
no_error_code_interrupt_handler 37  ; LPT2 (if enabled)
no_error_code_interrupt_handler 38  ; Floppy Disk
no_error_code_interrupt_handler 39  ; LPT1 / Unreliable "spurious" interrupt (usually)
no_error_code_interrupt_handler 40  ; CMOS real-time clock (if enabled)
no_error_code_interrupt_handler 41  ; Free for peripherals / legacy SCSI / NIC
no_error_code_interrupt_handler 42  ; Free for peripherals / SCSI / NIC
no_error_code_interrupt_handler 43  ; Free for peripherals / SCSI / NIC
no_error_code_interrupt_handler 44  ; PS2 Mouse
no_error_code_interrupt_handler 45  ; FPU / Coprocessor / Inter-processor
no_error_code_interrupt_handler 46  ; Primary ATA Hard Disk
no_error_code_interrupt_handler 47  ; Secondary ATA Hard Disk