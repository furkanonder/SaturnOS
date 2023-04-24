#ifndef __IDT_H__
#define __IDT_H__

#include "../include/stdint.h"

/* IDT Entry
 * Bit:     | 31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 | 15 | 14 13 | 12 | 11 10 9 8 |  7  6  5  4  3  2  1  0 |
 * Content: |                   Offset 31..16                 | P |  DPL   | 0  | Gate Type |           zero          |
 *
 * Bit:     | 31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 | 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0 |
 * Content: |                 Segment Selector                |              Offset 15..0             |
 *
 * Offset: A 32-bit value, split in two parts. It represents the address of the entry point of the Interrupt Service
 *         Routine.
 *
 * Selector: A Segment Selector with multiple fields which must point to a valid code segment in your GDT.
 *
 * Gate Type: A 4-bit value which defines the type of gate this Interrupt Descriptor represents. There are five valid
 * type values:
 *      0b0101 or 0x5: Task Gate, note that in this case, the Offset value is unused and should be set to zero.
 *      0b0110 or 0x6: 16-bit Interrupt Gate
 *      0b0111 or 0x7: 16-bit Trap Gate
 *      0b1110 or 0xE: 32-bit Interrupt Gate
 *      0b1111 or 0xF: 32-bit Trap Gate
 *
 * DPL: A 2-bit value which defines the CPU Privilege Levels which are allowed to access this interrupt via the INT
 * instruction. Hardware interrupts ignore this mechanism.
 *
 * P: Present bit. Must be set (1) for the descriptor to be valid. */
struct idt_entry {
    uint16_t offset_low;
    uint16_t segment_selector;
    uint8_t zero;
    unsigned gate_type:4;
    unsigned zero2:1;
    unsigned dpl:2;
    unsigned p:1;
    uint16_t offset_high;
} __attribute__((packed));

struct idt {
    uint16_t size;
    uint32_t address;
} __attribute__((packed));

void init_idt();

#endif