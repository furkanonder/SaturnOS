#include "../include/isr.h"
#include "../include/framebuffer.h"

// Array of function pointers to store 256 function pointers
void (*interrupt_handlers[256]) ();

/** interrupt_handler:
 *  Calls the function pointed to by the function pointer stored in the interrupt_handlers array at the index
 *  corresponding to the given interrupt number. If the interrupt number is less than 32, it writes the interrupt
 *  number, cpu_state and stack_state and enters an endless loop.
 *
 * @param cpu  Popped registers from the stack by common_interrupt_handler(defined in asm/interrupt_handler.s)
 * @param interrupt Occured Interrupt number
 * @param stack Stack at the time of the interrupt
 */
void interrupt_handler(struct cpu_state cpu, int interrupt, struct stack_state stack) {
    if (interrupt < 32) {
        os_printf("Exception! System Halted!\n");
        os_printf("Interrupt No: %d\n", interrupt);
        os_printf("EIP: 0x%x\n", stack.eip);
        os_printf("CS: 0x%x\n", stack.cs);
        os_printf("EFLAGS: 0x%x\n", stack.eflags);
        os_printf("EAX: 0x%x\n", cpu.eax);
        os_printf("EBX: 0x%x\n", cpu.ebx);
        os_printf("ECX: 0x%x\n", cpu.ecx);
        os_printf("EDX: 0x%x\n", cpu.edx);
        os_printf("ESP: 0x%x\n", cpu.esp);
        os_printf("EBP: 0x%x\n", cpu.ebp);
        os_printf("ESI: 0x%x\n", cpu.esi);
        os_printf("EDI: 0x%x\n", cpu.edi);
        asm volatile ("hlt");
    }
    // Call the interrupt handler
    interrupt_handlers[interrupt](interrupt);
}

/** register_handler:
 * Stores the function pointer in the interrupt_handlers array at the index corresponding to the given interrupt number.
 *
 * @param interrupt  An interrupt number stored in the Interrupt Descriptor Table
 * @param handler   function to handle the given interrupt.
 */
void register_handler(int interrupt, void (*handler)()) {
    interrupt_handlers[interrupt] = handler;
}