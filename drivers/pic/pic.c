#include "../pic/pic.h"
#include "../io/io.h"

/** pic_remap:
 *  Remaps the PIC's.
 *
 *  In protected mode, the IRQs 0 to 7 conflict with the CPU exception which are reserved by Intel up until 0x1F.
 *  (It was an IBM design mistake.) In that reason, we are changing the PIC's offsets (also known as remapping the PIC)
 *  so that IRQs use non-reserved vectors.
 *
 * Based on https://wiki.osdev.org/8259_PIC
 * @param offset1   vector offset for master PIC vectors on the master become offset1..offset1 + 7
 * @param offset2   vector offset for slave PIC vectors on the master become offset2..offset2 + 7
 */
void pic_remap(unsigned char offset1, unsigned char offset2) {
    unsigned char a1, a2;

    // save masks
    a1 = inb(PIC1_DATA);
    a2 = inb(PIC2_DATA);

    // Starts the initialization sequence (in cascade mode)
    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);

    // ICW2: Master PIC vector offset
    outb(PIC1_DATA, offset1);
    // ICW2: Slave PIC vector offset
    outb(PIC2_DATA, offset2);

    // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
    outb(PIC1_DATA, 4);
    // ICW3: tell Slave PIC its cascade identity (0000 0010)
    outb(PIC2_DATA, 2);

    // ICW4: have the PICs use 8086 mode (and not 8080 mode)
    outb(PIC1_DATA, ICW4_8086);
    outb(PIC2_DATA, ICW4_8086);

    // restore saved masks
    outb(PIC1_DATA, a1);
    outb(PIC2_DATA, a2);

    /* The 8259A has the ability to mask off interrupts you tell it to. This is done by writing an 8 bit value
     * corresponding to the mask to PIC Data port while the PIC is in normal operation mode. The PIC will only
     * listen to interrupt which have a 0 in the corresponding bit in its mask. */

    /* The hardware interrupts are shown in the table below:
     * 0 	Timer 	        | 8 	Real Time Clock
     * 1 	Keyboard 	    | 9 	General I/O
     * 2 	PIC 2 	        | 10 	General I/O
     * 3 	COM 2 	        | 11 	General I/O
     * 4 	COM 1 	        | 12 	General I/O
     * 5 	LPT 2 	        | 13 	Coprocessor
     * 6 	Floppy disk 	| 14 	IDE Bus
     * 7 	LPT 1 	        | 15 	IDE Bus
     *
     * 0xFD = 1111 1101     Only listen to IRQ 1(to enable keyboard)
     * 0xFF = 1111 1111     Don't listen to any interrupts from PIC2 */
    outb(PIC1_DATA, 0xFD);
    outb(PIC2_DATA, 0xFF);

    // Enable the interrupts
    asm volatile("sti");
}

/** pic_acknowledge:
 *  Acknowledges an interrupt from either PIC 1 or PIC 2.
 *
 *  Every interrupt from the PIC has to be acknowledged - that is, sending a message to the PIC confirming that the
 *  interrupt has been handled. If this isn’t done the PIC won’t generate any more interrupts.
 *
 *  @param interrupt    The number of the interrupt
 */
void pic_acknowledge(unsigned int interrupt) {
    // Verify the interrupt number
    if (interrupt < PIC1_START_INTERRUPT || interrupt > PIC2_END_INTERRUPT) {
        return;
    }

    if (interrupt >= PIC2_START_INTERRUPT) {
        outb(PIC2_COMMAND, PIC_EOI);
    }
    else {
        outb(PIC1_COMMAND, PIC_EOI);
    }

}