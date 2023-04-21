#include "../include/gdt.h"

// Defined in asm/gdt_flush.s
extern void gdt_flush(uint32_t);

struct gdt_entry gdt_entries[GDT_ENTRY_COUNT];
struct gdt gdt_pointer;

/** gdt_set_gate:
 * Creates a GDT entry in the specified index.
 *
 * @param index The index for the gdt_entry.
 * @param base  The base address for the segment.
 * @param limit The top address the segment.
 * @param access_byte The access byte for the GDT to identify access.
 * @param flags Flags for setting the segment descriptor.
 */
static void gdt_set_gate(int32_t index, uint32_t base, uint32_t limit, uint8_t access_byte, uint8_t flags) {
    gdt_entries[index].base_addr_low        = (base  & 0xFFFF);
    gdt_entries[index].base_addr_middle     = (base >> 16) & 0xFF;
    gdt_entries[index].base_addr_high       = (base >> 24) & 0xFF;
    gdt_entries[index].segment_limit_low    = (limit & 0xFFFF);
    gdt_entries[index].segment_limit_high   = ((limit >> 16) & 0x0F);
    access_byte |= 0x10;
    gdt_entries[index].flags                = flags;
    gdt_entries[index].access_byte          = access_byte;
}

/* The simplest memory model for a system is the basic "flat model" in which the operating system and application
 * programs have access to a continuous, unsegmented address space. To the greatest extent possible, this basic flat
 * model hides the segmentation mechanism of the architecture from both the system designer and the application
 * programmer.
 *
 * To implement a basic flat memory model with the IA-32 architecture, at least two segment descriptors must be
 * created, one for referencing a code segment and one for referencing a data segment. Both of these segments, however,
 * are mapped to the entire linear address space: that is, both segment descriptors have the same base address value of
 * 0 and the same segment limit of 4 GBytes. By setting the segment limit to 4 GBytes, the segmentation mechanism is
 * kept from generating exceptions for out of limit memory references, even if no physical memory resides at a
 * particular address. ROM (EPROM) is generally located at the top of the physical address space, because the processor
 * begins execution at FFFF_FFF0H. RAM (DRAM) is placed at the bottom of the address space because the initial base
 * address for the DS data segment after reset initialization is 0. */
void setup_gdt() {
    gdt_pointer.address = (sizeof(struct gdt_entry) * GDT_ENTRY_COUNT) - 1;
    gdt_pointer.size = (unsigned int) &gdt_entries;

    /* Null Segment. The first descriptor in the GDT is always a null descriptor and can never be used to access memory.*/
    gdt_set_gate(0, 0, 0, 0, 0);

    /* Code Segment
     *
     * Access Bytes
     * Bit:     |  7  |  6  5 |  4  |  3  |  2  |  1  |  0  |
     * Content: |  P  |  DPL  |  S  |  E  |  DC |  RW |  A  |
     * Value:   |  1  |  0 0  |  1  |  1  |  0  |  1  |  0  | = 1001 1010 = 0x9A
     *
     * Flags
     * Bit:     |  3  |  2  |  1  |     0      |
     * Content: |  G  |  DB |  L  |  reserved  |
     * Value:   |  1  |  1  |  0  |     0      | = 1100 1111 = 0xCF */
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

    /* Data Segment
     *
     * Access Bytes
     * Bit:     |  7  |  6  5 |  4  |  3  |  2  |  1  |  0  |
     * Content: |  P  |  DPL  |  S  |  E  |  DC |  RW |  A  |
     * Value:   |  1  |  0 0  |  1  |  0  |  0  |  1  |  0  | = 1001 0010 = 0x92
     *
     * Flags
     * Bit:     |  3  |  2  |  1  |     0      |
     * Content: |  G  |  DB |  L  |  reserved  |
     * Value:   |  1  |  1  |  0  |     0      | = 1100 1111 = 0xCF */
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    gdt_flush((uint32_t) &gdt_pointer);
}