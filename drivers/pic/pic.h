#ifndef __PIC_H__
#define __PIC_H__

#define ICW1_ICW4	    0x01		/* Indicates that ICW4 will be present */
#define ICW1_INIT	    0x10		/* Initialization - required! */
#define ICW4_8086	    0x01		/* 8086/88 (MCS-80/85) mode */

#define PIC1		    0x20		/* IO base address for master PIC */
#define PIC2	    	0xA0		/* IO base address for slave PIC */
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	    (PIC1 + 1)
#define PIC2_COMMAND    PIC2
#define PIC2_DATA       (PIC2 + 1)

// PIC has two parts, PIC1 and PIC2. We remap them to the following interrupts.
#define PIC1_START_INTERRUPT    0x20
#define PIC2_START_INTERRUPT    0x28
#define PIC2_END_INTERRUPT      PIC2_START_INTERRUPT + 7
#define PIC_EOI		            0x20		/* End-of-interrupt command code */

void pic_remap(unsigned char offset1, unsigned char offset2);
void pic_acknowledge(unsigned int interrupt);

#endif