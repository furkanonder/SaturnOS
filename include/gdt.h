#ifndef __GDT_H__
#define __GDT_H__

#include "../include/stdint.h"

#define GDT_ENTRY_COUNT     3

/* Segmentation provides a mechanism for dividing the processor’s addressable memory space (called the linear address
 * space) into smaller protected address spaces called segments. Segments can be used to hold the code, data, and stack
 * for a program or to hold system data structures (such as a TSS or LDT).
 *
 * If more than one program (or task) is running on a processor, each program can be assigned its own set of segments.
 * The processor then enforces the boundaries between these segments and insures that one program does not interfere
 * with the execution of another program by writing into the other program’s segments. The segmentation mechanism also
 * allows typing of segments so that the operations that may be performed on a particular type of segment can be
 * restricted.
 *
 * To enable segmentation we need to set up a table that describes each segment - a segment descriptor table. In x86,
 * there are two types of descriptor tables: the Global Descriptor Table (GDT) and Local Descriptor Tables (LDT).
 * An LDT is set up and managed by user-space processes, and all processes have their own LDT. LDTs can be used if
 * a more complex segmentation model is desired - we won’t use it. The GDT is shared by everyone - it’s global. */

/* GDT Entry
 * Bit:     | 31 30 29 28 27 26 25 24 | 23 22 21 20 |  19   18   17   16   | 15 14 13 12 11 10 9 8 |  7  6  5  4  3  2  1  0  |
 * Content: |    Base Address(31:24)  |    Flags    | Segment Limit(19:16) |      Access Bytes     |   Base Address(23:16)    |
 *
 * Bit:     | 31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 | 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0 |
 * Content: |              Base Address(15:00)                |          Segment Limit(15:00)         |
 *
 * Flags
 * Bit:     | 23 | 22 |  21 |     20     |
 * Content: | G |  DB |  L  |  reserved  |
 *
 * Access Bytes
 * Bit:     |  15  |  14  13 |  12  | 11  10  9  8   |
 * Content: |  P   |   DPL   |  S   |      Type     |
 *
 * Type
 * Bit:     |  11 |  10  |   9  |  8  |
 * Content: |  E  |  DC  |  RW  |  A  |
 *
 * E: Executable bit. If clear (0) the descriptor defines a data segment. If set (1) it defines a code segment which
 * can be executed from.
 *
 * DC: Direction bit/Conforming bit. For data selectors: Direction bit. If clear (0) the segment grows up. If set (1)
 * the segment grows down, ie. the Offset has to be greater than the Limit.
 * For code selectors: Conforming bit.
 *      If clear (0) code in this segment can only be executed from the ring set in DPL.
 *      If set (1) code in this segment can be executed from an equal or lower privilege level.
 *
 * RW: Readable bit/Writable bit.
 * For code segments: Readable bit. If clear (0), read access for this segment is not allowed. If set (1) read access
 * is allowed. Write access is never allowed for code segments.
 * For data segments: Writeable bit. If clear (0), write access for this segment is not allowed. If set (1) write access
 * is allowed. Read access is always allowed for data segments.
 *
 * A: Accessed bit. Best left clear (0), the CPU will set it when the segment is accessed.
 *
 * Segment limit field:
 * Specifies the size of the segment. The processor puts together the two segment limit fields to form a 20-bit value.
 * The processor interprets the segment limit in one of two ways, depending on the setting of the G (granularity) flag:
 *      • If the granularity flag is clear, the segment size can range from 1 byte to 1 MByte, in byte increments.
 *      • If the granularity flag is set, the segment size can range from 4 KBytes to 4 GBytes, in 4-KByte increments.
 *
 * Base address fields:
 * Defines the location of byte 0 of the segment within the 4-GByte linear address space. The processor puts together
 * the three base address fields to form a single 32-bit value. Segment base addresses should be aligned to 16-byte
 * boundaries. Although 16-byte alignment is not required, this alignment allows programs to maximize performance by
 * aligning code and data on 16-byte boundaries.
 *
 * Type field:
 * Indicates the segment or gate type and specifies the kinds of access that can be made to the segment and the
 * direction of growth. The interpretation of this field depends on whether the descriptor type flag specifies an
 * application (code or data) descriptor or a system descriptor.
 *
 * S (descriptor type) flag:
 * Specifies whether the segment descriptor is for a system segment (S flag is clear) or a code or data
 * segment (S flag is set).
 *
 * DPL (descriptor privilege level) field:
 * Specifies the privilege level of the segment. The privilege level can range from 0 to 3, with 0 being
 * the most privileged level. The DPL is used to control access to the segment.
 *
 * P (segment-present) flag:
 * Indicates whether the segment is present in memory (set) or not present (clear).
 *
 * D/B (default operation size/default stack pointer size and/or upper bound) flag:
 * Performs different functions depending on whether the segment descriptor is an executable code
 * segment, an expand-down data segment, or a stack segment. (This flag should always be set to 1
 * for 32-bit code and data segments and to 0 for 16-bit code and data segments.)
 *
 * G (granularity) flag:
 * Determines the scaling of the segment limit field. When the granularity flag is clear, the segment
 * limit is interpreted in byte units; when flag is set, the segment limit is interpreted in 4-KByte units.
 *
 * L (64-bit code segment) flag:
 * In IA-32e mode, bit 21 of the second doubleword of the segment descriptor indicates whether a code segment contains
 * native 64-bit code. A value of 1 indicates instructions in this code segment are executed in 64-bit mode. A value of
 * 0 indicates the instructions in this code segment are executed in compatibility mode. If L-bit is set, then D-bit
 * must be cleared. When not in IA-32e mode or for non-code segments, bit 21 is reserved and should always be set to 0.*/

struct gdt_entry {
    uint16_t segment_limit_low;
    uint16_t base_addr_low;
    uint8_t  base_addr_middle;
    uint8_t  access_byte;
    unsigned segment_limit_high: 4;
    unsigned flags: 4;
    uint8_t  base_addr_high;
} __attribute__((packed));

struct gdt {
    uint16_t address;
    uint32_t size;
} __attribute__((packed));

extern void setup_gdt();

#endif