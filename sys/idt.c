#include "../include/idt.h"
#include "../include/string.h"

/* To aid in handling exceptions and interrupts, each architecturally defined exception and each interrupt condition
 * requiring special handling by the processor is assigned a unique identification number, called a vector number. The
 * processor uses the vector number assigned to an exception or interrupt as an index into the interrupt descriptor
 * table (IDT). The allowable range for vector numbers is 0 to 255. Vector numbers in the range 0 through 31 are
 * reserved by the Intel 64 and IA-32 architectures for architecture-defined exceptions and interrupts. Vector numbers
 * in the range 32 to 255 are designated as user-defined interrupts and are not reserved by the Intel 64 and IA-32
 * architecture. */
struct idt_entry idt_entries[256];
struct idt idt_ptr;

// Defined in asm/idt_loader.s, and is used to load the IDT.
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

    // Points the processor's internal register to the new IDT.
    load_idt(&idt_ptr);
}