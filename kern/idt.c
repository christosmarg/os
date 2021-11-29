#include "libk.h"
#include "idt.h"

#define NINT		256
#define NRSVINT		32
#define NISR		16
#define PL_KERN		0	/* Kernel privilege level */
#define PL_DRV1		1	/* Device driver privilege level 1 */
#define PL_DRV2		2	/* Device driver privilege level 2 */
#define PL_USER		3	/* User privilege level */
#define SEL_KCODE	0x08	/* Kernel code descriptor */
#define TP_386IGT	0xe	/* System 386 interrupt gate */
/* Type 0..3, DPL 4..6, P 7..8 */
#define GT_FLAGS(r, t)	((1<<7) | (r<<5) | t)

#define PIC_MASTER_CMD	0x20
#define PIC_MASTER_DATA	(PIC_MASTER_CMD + 1)
#define PIC_SLAVE_CMD	0xa0
#define PIC_SLAVE_DATA	(PIC_SLAVE_CMD + 1)

static void print_regs(struct reg *);
static void idt_set_gate(struct gate_desc *, void *, uint16_t, uint8_t);
static void pic_remap(void);

static struct gate_desc idt[NINT];
static intrhand_t isr[NISR] = { NULL };

static void
print_regs(struct reg *r)
{
	printf("eax=%#08x\tebx=%#08x\tecx=%#08x\tedx=%#08x\n",
	    r->r_eax, r->r_ebx, r->r_ecx, r->r_edx);
	printf("esp=%#08x\tebp=%#08x\tesi=%#08x\tedi=%#08x\n",
	    r->r_esp, r->r_ebp, r->r_esi, r->r_edi);
	printf("ds=%#08x \tes=%#08x \tfs=%#08x \tgs=%#08x\n",
	    r->r_ds, r->r_es, r->r_fs, r->r_gs);
	printf("eip=%#08x\tcs=%#08x \tss=%#08x \teflags=%08x\n",
	    r->r_eip, r->r_cs, r->r_ss, r->r_eflags);
	printf("int=%#08x\terr=%#08x\tuesp=%#08x\n",
	    r->r_intrno, r->r_err, r->r_uesp);
}

static void
idt_set_gate(struct gate_desc *gd, void *func, uint16_t sel, uint8_t flags)
{
	gd->gd_off_lo = (uint32_t)func & 0xffff;
	gd->gd_sel = sel;
	gd->gd_rsvd = 0;
	gd->gd_flags = flags;
	gd->gd_off_hi = ((uint32_t)func >> 16) & 0xffff;
}

/* TODO: explain. */
static void
pic_remap(void)
{
	outb(PIC_MASTER_CMD, 0x11);
	outb(PIC_SLAVE_CMD, 0x11);
	outb(PIC_MASTER_DATA, 0x20);
	outb(PIC_SLAVE_DATA, 0x28);
	outb(PIC_MASTER_DATA, 0x04);
	outb(PIC_SLAVE_DATA, 0x02);
	outb(PIC_MASTER_DATA, 0x01);
	outb(PIC_SLAVE_DATA, 0x01);
	outb(PIC_MASTER_DATA, 0x00);
	outb(PIC_MASTER_DATA, 0x00);
}

extern void ex_div, ex_dbg, ex_nmsk, ex_bpt, ex_ofl, ex_bnd, ex_ill, ex_dna,
    ex_dbl, ex_fpusegm, ex_tss, ex_missing, ex_stk, ex_prot, ex_page, ex_rsvd,
    ex_fpu, ex_align, ex_mchk, ex_simd;

extern void irq0, irq1, irq2, irq3, irq4, irq5, irq6, irq7, irq8, irq9, irq10,
    irq11, irq12, irq13, irq14, irq15;

void
idt_init(void)
{
	struct region_desc r_idt;
	int i;

	for (i = 0; i < NINT; i++)
		idt_set_gate(&idt[i], &ex_rsvd, SEL_KCODE,
		    GT_FLAGS(PL_KERN, TP_386IGT));
	idt_set_gate(&idt[ 0], &ex_div, SEL_KCODE, GT_FLAGS(PL_KERN, TP_386IGT));
	idt_set_gate(&idt[ 1], &ex_dbg,	SEL_KCODE, GT_FLAGS(PL_KERN, TP_386IGT));
	idt_set_gate(&idt[ 2], &ex_nmsk, SEL_KCODE, GT_FLAGS(PL_KERN, TP_386IGT));
	idt_set_gate(&idt[ 3], &ex_bpt,	SEL_KCODE, GT_FLAGS(PL_KERN, TP_386IGT));
	idt_set_gate(&idt[ 4], &ex_ofl,	SEL_KCODE, GT_FLAGS(PL_KERN, TP_386IGT));
	idt_set_gate(&idt[ 5], &ex_bnd,	SEL_KCODE, GT_FLAGS(PL_KERN, TP_386IGT));
	idt_set_gate(&idt[ 6], &ex_ill,	SEL_KCODE, GT_FLAGS(PL_KERN, TP_386IGT));
	idt_set_gate(&idt[ 7], &ex_dna,	SEL_KCODE, GT_FLAGS(PL_KERN, TP_386IGT));
	idt_set_gate(&idt[ 8], &ex_dbl,	SEL_KCODE, GT_FLAGS(PL_KERN, TP_386IGT));
	idt_set_gate(&idt[ 9], &ex_fpusegm, SEL_KCODE, GT_FLAGS(PL_KERN, TP_386IGT));
	idt_set_gate(&idt[10], &ex_tss,	SEL_KCODE, GT_FLAGS(PL_KERN, TP_386IGT));
	idt_set_gate(&idt[11], &ex_missing, SEL_KCODE, GT_FLAGS(PL_KERN, TP_386IGT));
	idt_set_gate(&idt[12], &ex_stk,	SEL_KCODE, GT_FLAGS(PL_KERN, TP_386IGT));
	idt_set_gate(&idt[13], &ex_prot, SEL_KCODE, GT_FLAGS(PL_KERN, TP_386IGT));
	idt_set_gate(&idt[14], &ex_page, SEL_KCODE, GT_FLAGS(PL_KERN, TP_386IGT));
	idt_set_gate(&idt[15], &ex_rsvd, SEL_KCODE, GT_FLAGS(PL_KERN, TP_386IGT));
	idt_set_gate(&idt[16], &ex_fpu,	SEL_KCODE, GT_FLAGS(PL_KERN, TP_386IGT));
	idt_set_gate(&idt[17], &ex_align, SEL_KCODE, GT_FLAGS(PL_KERN, TP_386IGT));
	idt_set_gate(&idt[18], &ex_mchk, SEL_KCODE, GT_FLAGS(PL_KERN, TP_386IGT));
	idt_set_gate(&idt[19], &ex_simd, SEL_KCODE, GT_FLAGS(PL_KERN, TP_386IGT));
	/* 20 - 31 are reserved. */

	pic_remap();

	/* FIXME: how is this related to irq 0 -> timer etc? */
	idt_set_gate(&idt[32], &irq0, SEL_KCODE, GT_FLAGS(PL_KERN, TP_386IGT));
	idt_set_gate(&idt[33], &irq1, SEL_KCODE, GT_FLAGS(PL_KERN, TP_386IGT));
	idt_set_gate(&idt[34], &irq2, SEL_KCODE, GT_FLAGS(PL_KERN, TP_386IGT));
	idt_set_gate(&idt[35], &irq3, SEL_KCODE, GT_FLAGS(PL_KERN, TP_386IGT));
	idt_set_gate(&idt[36], &irq4, SEL_KCODE, GT_FLAGS(PL_KERN, TP_386IGT));
	idt_set_gate(&idt[37], &irq5, SEL_KCODE, GT_FLAGS(PL_KERN, TP_386IGT));
	idt_set_gate(&idt[38], &irq6, SEL_KCODE, GT_FLAGS(PL_KERN, TP_386IGT));
	idt_set_gate(&idt[39], &irq7, SEL_KCODE, GT_FLAGS(PL_KERN, TP_386IGT));
	idt_set_gate(&idt[40], &irq8, SEL_KCODE, GT_FLAGS(PL_KERN, TP_386IGT));
	idt_set_gate(&idt[41], &irq9, SEL_KCODE, GT_FLAGS(PL_KERN, TP_386IGT));
	idt_set_gate(&idt[42], &irq10, SEL_KCODE, GT_FLAGS(PL_KERN, TP_386IGT));
	idt_set_gate(&idt[43], &irq11, SEL_KCODE, GT_FLAGS(PL_KERN, TP_386IGT));
	idt_set_gate(&idt[44], &irq12, SEL_KCODE, GT_FLAGS(PL_KERN, TP_386IGT));
	idt_set_gate(&idt[45], &irq13, SEL_KCODE, GT_FLAGS(PL_KERN, TP_386IGT));
	idt_set_gate(&idt[46], &irq14, SEL_KCODE, GT_FLAGS(PL_KERN, TP_386IGT));
	idt_set_gate(&idt[47], &irq15, SEL_KCODE, GT_FLAGS(PL_KERN, TP_386IGT));

	/*idt_set_gate(&idt[127], &syscall, SEL_KCODE, GT_FLAGS(PL_KERN, TP_386IGT));*/

	r_idt.rd_base = (uint32_t)&idt;
	r_idt.rd_limit = NINT * sizeof(struct gate_desc) - 1;
	lidt(&r_idt);
}

static const char *exceptmsg[] = {
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

void
intr_handler(struct reg *r)
{
	intrhand_t handler;

	/*
	 * We'll call the handler only if the interrupt number is > 32,
	 * which means that we're dealing with an IRQ and not an exception.
	 */
	if (r->r_intrno >= 32 && (handler = isr[r->r_intrno - 32]) != NULL)
		handler(r);
	else if (r->r_intrno < 32) {
		print_regs(r);
		panic("%s: system halted...\n", exceptmsg[r->r_intrno]);
	}
	if (r->r_intrno >= 40)
		outb(PIC_SLAVE_CMD, 0x20);
	outb(PIC_MASTER_CMD, 0x20);
}

void
intr_register_handler(uint8_t intrno, intrhand_t handler)
{
	isr[intrno] = handler;
}
