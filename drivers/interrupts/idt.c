#include "idt.h"
#include "isr.h"
#include "../../include/string.h"

/* To aid in handling exceptions and interrupts, each architecturally defined exception and each interrupt condition
 * requiring special handling by the processor is assigned a unique identification number, called a vector number. The
 * processor uses the vector number assigned to an exception or interrupt as an index into the interrupt descriptor
 * table (IDT). The allowable range for vector numbers is 0 to 255. Vector numbers in the range 0 through 31 are
 * reserved by the Intel 64 and IA-32 architectures for architecture-defined exceptions and interrupts. Vector numbers
 * in the range 32 to 255 are designated as user-defined interrupts and are not reserved by the Intel 64 and IA-32
 * architecture. */
struct idt_entry idt_entries[256];
struct idt idt_ptr;

// Defined in idt_loader.s, and is used to load the IDT.
extern int load_idt(void*);

/** idt_set_gate:
 * Creates a IDT entry in the specified index.
 *
 * @param index The index for the idt_entry.
 * @param base The base address of the Interrupt Service Routine.
 * @param selector The segment selector for the kernel code segment.
 * @param g_type Gate Type
 */
void idt_set_gate(int32_t num, void(*base), uint16_t selector, unsigned g_type) {
    idt_entries[num].offset_low = (uint32_t)base & 0xFFFF;
    idt_entries[num].segment_selector = selector;
    // Always set to 0.
    idt_entries[num].zero = 0;
    // Set the gate type
    idt_entries[num].gate_type = g_type;
    idt_entries[num].zero2 = 0;
    // Descriptor privilege level. 0 is the most privileged level of the segment.
    idt_entries[num].dpl = 0;
    // Must be set (1) for the descriptor to be valid.
    idt_entries[num].p = 1;
    idt_entries[num].offset_high = ((uint32_t)base >> 16) & 0xFFFF;
}

/** init_idt:
 * This function will set up the special IDT pointer, set up the entries in our IDT, and then finally call
 * load_idt() to load IDT.
 */
void init_idt() {
    // Set up the IDT pointer
    idt_ptr.address = (uint32_t) &idt_entries;
    idt_ptr.size = sizeof(struct idt_entry) * 256 - 1;

    // Clear out the entire IDT, initializing it to zeros.
    memset(&idt_entries, 0, sizeof(struct idt_entry) * 256);

    /* Add the interrupts to the Interrupt Descriptor Table.
     *
     * base = The base address of the Interrupt Service Routine.
     * selector = 0x08 is points to code segment in our GDT.
     * gate type = 1110 (32-bit Interrupt Gate) */
    idt_set_gate(0, interrupt_handler_0, 0x08, 0b1110);
    idt_set_gate(1, interrupt_handler_1, 0x08, 0b1110);
    idt_set_gate(2, interrupt_handler_2, 0x08, 0b1110);
    idt_set_gate(3, interrupt_handler_3, 0x08, 0b1110);
    idt_set_gate(4, interrupt_handler_4, 0x08, 0b1110);
    idt_set_gate(5, interrupt_handler_5, 0x08, 0b1110);
    idt_set_gate(6, interrupt_handler_6, 0x08, 0b1110);
    idt_set_gate(7, interrupt_handler_7, 0x08, 0b1110);
    idt_set_gate(8, interrupt_handler_8, 0x08, 0b1110);
    idt_set_gate(9, interrupt_handler_9, 0x08, 0b1110);
    idt_set_gate(10, interrupt_handler_10, 0x08, 0b1110);
    idt_set_gate(11, interrupt_handler_11, 0x08, 0b1110);
    idt_set_gate(12, interrupt_handler_12, 0x08, 0b1110);
    idt_set_gate(13, interrupt_handler_13, 0x08, 0b1110);
    idt_set_gate(14, interrupt_handler_14, 0x08, 0b1110);
    idt_set_gate(15, interrupt_handler_15, 0x08, 0b1110);
    idt_set_gate(16, interrupt_handler_16, 0x08, 0b1110);
    idt_set_gate(17, interrupt_handler_17, 0x08, 0b1110);
    idt_set_gate(18, interrupt_handler_18, 0x08, 0b1110);
    idt_set_gate(19, interrupt_handler_19, 0x08, 0b1110);
    idt_set_gate(20, interrupt_handler_20, 0x08, 0b1110);
    idt_set_gate(21, interrupt_handler_21, 0x08, 0b1110);
    idt_set_gate(22, interrupt_handler_22, 0x08, 0b1110);
    idt_set_gate(23, interrupt_handler_23, 0x08, 0b1110);
    idt_set_gate(24, interrupt_handler_24, 0x08, 0b1110);
    idt_set_gate(25, interrupt_handler_25, 0x08, 0b1110);
    idt_set_gate(26, interrupt_handler_26, 0x08, 0b1110);
    idt_set_gate(27, interrupt_handler_27, 0x08, 0b1110);
    idt_set_gate(28, interrupt_handler_28, 0x08, 0b1110);
    idt_set_gate(29, interrupt_handler_29, 0x08, 0b1110);
    idt_set_gate(30, interrupt_handler_30, 0x08, 0b1110);
    idt_set_gate(31, interrupt_handler_31, 0x08, 0b1110);

    idt_set_gate(32, interrupt_handler_32, 0x08, 0b1110);
    idt_set_gate(33, interrupt_handler_33, 0x08, 0b1110);
    idt_set_gate(34, interrupt_handler_34, 0x08, 0b1110);
    idt_set_gate(35, interrupt_handler_35, 0x08, 0b1110);
    idt_set_gate(36, interrupt_handler_36, 0x08, 0b1110);
    idt_set_gate(37, interrupt_handler_37, 0x08, 0b1110);
    idt_set_gate(38, interrupt_handler_38, 0x08, 0b1110);
    idt_set_gate(39, interrupt_handler_39, 0x08, 0b1110);
    idt_set_gate(40, interrupt_handler_40, 0x08, 0b1110);
    idt_set_gate(41, interrupt_handler_41, 0x08, 0b1110);
    idt_set_gate(42, interrupt_handler_42, 0x08, 0b1110);
    idt_set_gate(43, interrupt_handler_43, 0x08, 0b1110);
    idt_set_gate(44, interrupt_handler_44, 0x08, 0b1110);
    idt_set_gate(45, interrupt_handler_45, 0x08, 0b1110);
    idt_set_gate(46, interrupt_handler_46, 0x08, 0b1110);
    idt_set_gate(47, interrupt_handler_47, 0x08, 0b1110);

    // Points the processor's internal register to the new IDT.
    load_idt(&idt_ptr);
}