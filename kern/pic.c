#include "pic.h"
#include "idt.h"
#include "io.h"

/* 
 * BIOS reserves IRQ 0 to 15 for the PIC. We need to remap it to non-reserved
 * vectors so that there are no conflicts when setting our IRQs.
 */
void
pic_remap(void)
{
	u_int8_t m1, m2;

	/* Save masks */
	m1 = inb(PIC_MASTER_DATA);
	m2 = inb(PIC_SLAVE_DATA);

	/* Reinit both PICs */
	outb(PIC_MASTER_CMD, ICW1_INIT | ICW1_ICW4);
	/* Older machines need some time... */
	io_wait();
	outb(PIC_SLAVE_CMD, ICW1_INIT | ICW1_ICW4);
	io_wait();
	/* Change offsets */
	outb(PIC_MASTER_DATA, PIC_MASTER_OFFT);
	io_wait();
	outb(PIC_SLAVE_DATA, PIC_SLAVE_OFFT);
	io_wait();
	/* Slave PIC at IRQ2 */
	outb(PIC_MASTER_DATA, 0x04);
	io_wait();
	/* Tell Slave PIC its cascade identity */
	outb(PIC_SLAVE_DATA, ICW1_SINGLE);
	io_wait();
	/* 8086 mode */
	outb(PIC_MASTER_DATA, ICW4_8086);
	io_wait();
	outb(PIC_SLAVE_DATA, ICW4_8086);
	io_wait();
	/* Restore masks */
	outb(PIC_MASTER_DATA, m1);
	outb(PIC_SLAVE_DATA, m2);
}

void
pic_eoi(u_int32_t intrno)
{
	outb(PIC_MASTER_CMD, PIC_EOI);
	if (intrno >= IRQ8)
		outb(PIC_SLAVE_CMD, PIC_EOI);
}

void
pic_mask(u_int8_t irq, int flag)
{
	u_int16_t port;
	u_int8_t v;

	if (irq < 8)
		port = PIC_MASTER_DATA;
	else {
		port = PIC_SLAVE_DATA;
		irq -= 8;
	}
	if (flag == PIC_SET_MASK)
		v = inb(port) | (1 << irq);
	else if (flag == PIC_CLEAR_MASK)
		v = inb(port) & ~(1 << irq);
	else
		return;
	outb(port, v);
}

void
pic_on(void)
{
}

void
pic_off(void)
{
	outb(PIC_MASTER_DATA, 0xff);
	outb(PIC_SLAVE_DATA, 0xff);
}
