#include "extern.h"
#include "idt.h"

static void idt_set_gate(uint8_t, uint32_t);

static struct idt_gate idt[N_INT];
static struct idt_reg idtr;
static void *irq_routines[16] = {NULL};

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
	idt[n].attr = 0x8e; /* The gate is present and is running in kernel mode. */
	idt[n].off_hi = (ptr >> 16) & 0xffff;
}

void
idt_init(void)
{
	/* Set up the ISRs */
	idt_set_gate(0, (uint32_t)isr0);
	idt_set_gate(1, (uint32_t)isr1);
	idt_set_gate(2, (uint32_t)isr2);
	idt_set_gate(3, (uint32_t)isr3);
	idt_set_gate(4, (uint32_t)isr4);
	idt_set_gate(5, (uint32_t)isr5);
	idt_set_gate(6, (uint32_t)isr6);
	idt_set_gate(7, (uint32_t)isr7);
	idt_set_gate(8, (uint32_t)isr8);
	idt_set_gate(9, (uint32_t)isr9);
	idt_set_gate(10, (uint32_t)isr10);
	idt_set_gate(11, (uint32_t)isr11);
	idt_set_gate(12, (uint32_t)isr12);
	idt_set_gate(13, (uint32_t)isr13);
	idt_set_gate(14, (uint32_t)isr14);
	idt_set_gate(15, (uint32_t)isr15);
	idt_set_gate(16, (uint32_t)isr16);
	idt_set_gate(17, (uint32_t)isr17);
	idt_set_gate(18, (uint32_t)isr18);
	idt_set_gate(19, (uint32_t)isr19);
	idt_set_gate(20, (uint32_t)isr20);
	idt_set_gate(21, (uint32_t)isr21);
	idt_set_gate(22, (uint32_t)isr22);
	idt_set_gate(23, (uint32_t)isr23);
	idt_set_gate(24, (uint32_t)isr24);
	idt_set_gate(25, (uint32_t)isr25);
	idt_set_gate(26, (uint32_t)isr26);
	idt_set_gate(27, (uint32_t)isr27);
	idt_set_gate(28, (uint32_t)isr28);
	idt_set_gate(29, (uint32_t)isr29);
	idt_set_gate(30, (uint32_t)isr30);
	idt_set_gate(31, (uint32_t)isr31);

	/* Remap the PIC */
	/* TODO: explain.*/
	outb(0x11, 0x20);
	outb(0x11, 0xa0);
	outb(0x20, 0x21);
	outb(0x28, 0xa1);
	outb(0x04, 0x21);
	outb(0x02, 0xa1);
	outb(0x01, 0x21);
	outb(0x01, 0xa1);
	outb(0x00, 0x21);
	outb(0x00, 0xa1);

	/* Set up the IRQs */
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

/* TODO: add comments */
void
isr_handler(struct reg *r)
{
	/* XXX: 31? */
	if (r->intno < 32) {
		tty_write(except[r->intno]);
		tty_write(". System halted...\n");
		__asm__ __volatile__ ("hlt");
	}
}

/* TODO: add comments */
void
irq_handler(struct reg *r)
{
	void (*handler)(struct reg *);

	if ((handler = irq_routines[r->intno - 32]) != 0)
		handler(r);
	if (r->intno >= 40)
		outb(0x20, 0xa0);
	outb(0x20, 0x20);
}

/* TODO: add comments */
void
irq_add_handler(uint8_t irqno, void (*handler)(struct reg *r))
{
	irq_routines[irqno] = handler;
}

/* TODO: add comments */
void
irq_rm_handler(uint8_t irqno)
{
	irq_routines[irqno] = NULL;
}
