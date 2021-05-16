#include "extern.h"
#include "idt.h"
#include "port.h"

#define N_INT 256 /* There are 256 interrupts in total. */
#define KERN_CODESEG 0x08 /* Kernel code segment. */

static void idt_set_gate(uint8_t, uint32_t);

static struct idt_gate idt[N_INT];
static void *isr[16] = {NULL};
/* Exception messages for the first 32 ISRs. */
static const char *except[] = {
	"Division By Zero Exception",
	"Debug Exception",
	"Non Maskable Interrupt Exception",
	"Breakpoint Exception",
	"Into Detected Overflow Exception",
	"Out of Bounds Exception",
	"Invalid Opcode Exception",
	"No Coprocessor Exception",
	"Double Fault Exception",
	"Coprocessor Segment Overrun Exception",
	"Bad TSS Exception",
	"Segment Not Present Exception",
	"Stack Fault Exception",
	"General Protection Fault Exception",
	"Page Fault Exception",
	"Unknown Interrupt Exception",
	"Coprocessor Fault Exception",
	"Alignment Check Exception (486+)",
	"Machine Check Exception (Pentium/586+)",
	"Reserved Exception",
	"Reserved Exception",
	"Reserved Exception",
	"Reserved Exception",
	"Reserved Exception",
	"Reserved Exception",
	"Reserved Exception",
	"Reserved Exception",
	"Reserved Exception",
	"Reserved Exception",
	"Reserved Exception",
	"Reserved Exception",
	"Reserved Exception",
};

static void
idt_set_gate(uint8_t n, uint32_t ptr)
{
	idt[n].off_lo = ptr & 0xffff;
	idt[n].sel = KERN_CODESEG;
	idt[n].zero = 0;
	/* The gate is present and is running in kernel mode. */
	idt[n].flags = 0x8e;
	idt[n].off_hi = (ptr >> 16) & 0xffff;
}

void
idt_init(void)
{
	struct idt_reg {
		uint16_t limit;	/* Points at IDT[0]. */
		uint32_t base;	/* Points at the end of the IDT. */
	} __attribute__((packed)) idtr;

	(void)memset(&idt, 0, sizeof(idt));

	/* Set up exception interrupts. */
	idt_set_gate(0, (uint32_t)ex0);
	idt_set_gate(1, (uint32_t)ex1);
	idt_set_gate(2, (uint32_t)ex2);
	idt_set_gate(3, (uint32_t)ex3);
	idt_set_gate(4, (uint32_t)ex4);
	idt_set_gate(5, (uint32_t)ex5);
	idt_set_gate(6, (uint32_t)ex6);
	idt_set_gate(7, (uint32_t)ex7);
	idt_set_gate(8, (uint32_t)ex8);
	idt_set_gate(9, (uint32_t)ex9);
	idt_set_gate(10, (uint32_t)ex10);
	idt_set_gate(11, (uint32_t)ex11);
	idt_set_gate(12, (uint32_t)ex12);
	idt_set_gate(13, (uint32_t)ex13);
	idt_set_gate(14, (uint32_t)ex14);
	idt_set_gate(15, (uint32_t)ex15);
	idt_set_gate(16, (uint32_t)ex16);
	idt_set_gate(17, (uint32_t)ex17);
	idt_set_gate(18, (uint32_t)ex18);
	idt_set_gate(19, (uint32_t)ex19);
	idt_set_gate(20, (uint32_t)ex20);
	idt_set_gate(21, (uint32_t)ex21);
	idt_set_gate(22, (uint32_t)ex22);
	idt_set_gate(23, (uint32_t)ex23);
	idt_set_gate(24, (uint32_t)ex24);
	idt_set_gate(25, (uint32_t)ex25);
	idt_set_gate(26, (uint32_t)ex26);
	idt_set_gate(27, (uint32_t)ex27);
	idt_set_gate(28, (uint32_t)ex28);
	idt_set_gate(29, (uint32_t)ex29);
	idt_set_gate(30, (uint32_t)ex30);
	idt_set_gate(31, (uint32_t)ex31);

	/* Remap the PIC */
	/* TODO: explain.*/
	outb(P_PIC1_CMD, 0x11);
	outb(P_PIC2_CMD, 0x11);
	outb(P_PIC1_DATA, 0x20);
	outb(P_PIC2_DATA, 0x28);
	outb(P_PIC1_DATA, 0x04);
	outb(P_PIC2_DATA, 0x02);
	outb(P_PIC1_DATA, 0x01);
	outb(P_PIC2_DATA, 0x01);
	outb(P_PIC1_DATA, 0x00);
	outb(P_PIC2_DATA, 0x00);

	/* Set up IRQs */
	idt_set_gate(32, (uint32_t)irq0);
	idt_set_gate(33, (uint32_t)irq1);
	idt_set_gate(34, (uint32_t)irq2);
	idt_set_gate(35, (uint32_t)irq3);
	idt_set_gate(36, (uint32_t)irq4);
	idt_set_gate(37, (uint32_t)irq5);
	idt_set_gate(38, (uint32_t)irq6);
	idt_set_gate(39, (uint32_t)irq7);
	idt_set_gate(40, (uint32_t)irq8);
	idt_set_gate(41, (uint32_t)irq9);
	idt_set_gate(42, (uint32_t)irq10);
	idt_set_gate(43, (uint32_t)irq11);
	idt_set_gate(44, (uint32_t)irq12);
	idt_set_gate(45, (uint32_t)irq13);
	idt_set_gate(46, (uint32_t)irq14);
	idt_set_gate(47, (uint32_t)irq15);

	/* https://wiki.osdev.org/Interrupt_Descriptor_Table#Location_and_Size */
	idtr.base = (uint32_t)&idt;
	idtr.limit = N_INT * sizeof(struct idt_gate) - 1;
	/* Load the IDT */
	__asm__ __volatile__ ("lidtl (%0)" : : "r" (&idtr));
}

void
int_handler(struct reg *r)
{
	void (*handler)(struct reg *);

	/*
	 * We'll call the handler only if the interrupt number is > 32,
	 * which means that we're dealing with an IRQ and not an exception.
	 */
	if (r->intno >= 32 && (handler = isr[r->intno - 32]) != 0)
		handler(r);
	/* Entries below index 32 in the IDT are exceptions, we need to hang. */
	else if (r->intno < 32) {
		tty_write(except[r->intno]);
		tty_write(". System halted...\n");
		__asm__ __volatile__ ("hlt");
	}
	if (r->intno >= 40)
		outb(P_PIC2_CMD, 0x20);
	outb(P_PIC1_CMD, 0x20);
}

void
int_add_handler(uint8_t intno, void (*handler)(struct reg *r))
{
	isr[intno] = handler;
}
