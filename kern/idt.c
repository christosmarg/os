#include <sys/libk.h>

#include "idt.h"

#define NINT 256
#define NISR 16
#define KERN_CODESEG 0x08

static void idt_set_entry(uint32_t);

/* IA-32 */
struct idt_ent {
	uint16_t	off_lo;
	uint16_t	sel;
	uint8_t		zero;
	uint8_t		flags;
	uint16_t	off_hi;
} __attribute__((packed)) idt[NINT];

static void *isr[NISR] = {NULL};
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
idt_set_entry(uint32_t ptr)
{
	static int i = 0;

	idt[i].off_lo = ptr & 0xffff;
	idt[i].sel = KERN_CODESEG;
	idt[i].zero = 0;
	/* The gate is present and is running in kernel mode. */
	idt[i].flags = 0x8e;
	idt[i].off_hi = (ptr >> 16) & 0xffff;
	i++;
}

void
idt_init(void)
{
	struct idt_reg {
		uint16_t limit;	/* IDT size in bytes. */
		uint32_t base;	/* Points at `idt[0]`. */
	} __attribute__((packed)) idtr;

	(void)memset(&idt, 0, sizeof(idt));

	/* Set up exception interrupts. */
	idt_set_entry((uint32_t)ex0);
	idt_set_entry((uint32_t)ex1);
	idt_set_entry((uint32_t)ex2);
	idt_set_entry((uint32_t)ex3);
	idt_set_entry((uint32_t)ex4);
	idt_set_entry((uint32_t)ex5);
	idt_set_entry((uint32_t)ex6);
	idt_set_entry((uint32_t)ex7);
	idt_set_entry((uint32_t)ex8);
	idt_set_entry((uint32_t)ex9);
	idt_set_entry((uint32_t)ex10);
	idt_set_entry((uint32_t)ex11);
	idt_set_entry((uint32_t)ex12);
	idt_set_entry((uint32_t)ex13);
	idt_set_entry((uint32_t)ex14);
	idt_set_entry((uint32_t)ex15);
	idt_set_entry((uint32_t)ex16);
	idt_set_entry((uint32_t)ex17);
	idt_set_entry((uint32_t)ex18);
	idt_set_entry((uint32_t)ex19);
	idt_set_entry((uint32_t)ex20);
	idt_set_entry((uint32_t)ex21);
	idt_set_entry((uint32_t)ex22);
	idt_set_entry((uint32_t)ex23);
	idt_set_entry((uint32_t)ex24);
	idt_set_entry((uint32_t)ex25);
	idt_set_entry((uint32_t)ex26);
	idt_set_entry((uint32_t)ex27);
	idt_set_entry((uint32_t)ex28);
	idt_set_entry((uint32_t)ex29);
	idt_set_entry((uint32_t)ex30);
	idt_set_entry((uint32_t)ex31);

	/* Remap the PIC */
	/* TODO: explain.*/
	outb(IO_PIC1_CMD, 0x11);
	outb(IO_PIC2_CMD, 0x11);
	outb(IO_PIC1_DATA, 0x20);
	outb(IO_PIC2_DATA, 0x28);
	outb(IO_PIC1_DATA, 0x04);
	outb(IO_PIC2_DATA, 0x02);
	outb(IO_PIC1_DATA, 0x01);
	outb(IO_PIC2_DATA, 0x01);
	outb(IO_PIC1_DATA, 0x00);
	outb(IO_PIC1_DATA, 0x00);

	/* Set up IRQs */
	idt_set_entry((uint32_t)irq0);
	idt_set_entry((uint32_t)irq1);
	idt_set_entry((uint32_t)irq2);
	idt_set_entry((uint32_t)irq3);
	idt_set_entry((uint32_t)irq4);
	idt_set_entry((uint32_t)irq5);
	idt_set_entry((uint32_t)irq6);
	idt_set_entry((uint32_t)irq7);
	idt_set_entry((uint32_t)irq8);
	idt_set_entry((uint32_t)irq9);
	idt_set_entry((uint32_t)irq10);
	idt_set_entry((uint32_t)irq11);
	idt_set_entry((uint32_t)irq12);
	idt_set_entry((uint32_t)irq13);
	idt_set_entry((uint32_t)irq14);
	idt_set_entry((uint32_t)irq15);

	/* https://wiki.osdev.org/Interrupt_Descriptor_Table#Location_and_Size */
	idtr.base = (uint32_t)&idt;
	idtr.limit = NINT * sizeof(struct idt_ent) - 1;
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
	if (r->intno >= 32 && (handler = isr[r->intno - 32]) != NULL)
		handler(r);
	/* Entries below index 32 in the IDT are exceptions, we need to hang. */
	else if (r->intno < 32)
		panic("%s: System halted...\n", except[r->intno]);
	if (r->intno >= 40)
		outb(IO_PIC2_CMD, 0x20);
	outb(IO_PIC1_CMD, 0x20);
}

void
int_add_handler(uint8_t intno, void (*handler)(struct reg *r))
{
	isr[intno] = handler;
}
