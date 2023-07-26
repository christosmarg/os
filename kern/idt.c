#include <libc.h>
#include "libk.h"
#include "idt.h"
#include "pic.h"
#include "cpufunc.h"

static void idt_set_gate(struct gate_desc *, void *, u_int, u_int, u_int);

static struct gate_desc idt[NINT];
static intrhand_t isr[NINT] = { NULL };

static void
idt_set_gate(struct gate_desc *gd, void *func, u_int sel, u_int dpl, u_int type)
{
	u_int32_t addr;

	addr = (u_int32_t)func;
	gd->gd_hi = (addr & 0xffff0000) | SEGP | dpl | type;
	gd->gd_lo = (sel << 16) | (addr & 0xffff);
}

/*
 * u_char so `-pedantic` won't complain about taking the address of `void`.
 */
extern u_char INTVEC(div), INTVEC(dbg), INTVEC(nmsk), INTVEC(bpt), INTVEC(ofl),
    INTVEC(bnd), INTVEC(ill), INTVEC(dna), INTVEC(dbl), INTVEC(fpusegm),
    INTVEC(tss), INTVEC(missing), INTVEC(stk), INTVEC(prot), INTVEC(page),
    INTVEC(rsvd), INTVEC(fpu), INTVEC(align), INTVEC(mchk), INTVEC(simd);

extern u_char INTVEC(irq0), INTVEC(irq1), INTVEC(irq2), INTVEC(irq3),
    INTVEC(irq4), INTVEC(irq5), INTVEC(irq6), INTVEC(irq7), INTVEC(irq8),
    INTVEC(irq9), INTVEC(irq10), INTVEC(irq11), INTVEC(irq12), INTVEC(irq13),
    INTVEC(irq14), INTVEC(irq15);

void
idt_init(void)
{
	struct region_desc r_idt;
	int i;

	for (i = 0; i < NINT; i++)
		idt_set_gate(&idt[i], &INTVEC(rsvd), KCSEL, SEGPL(KPL), SEGIG);
	idt_set_gate(&idt[ 0], &INTVEC(div), KCSEL, SEGPL(KPL), SEGIG);
	idt_set_gate(&idt[ 1], &INTVEC(dbg), KCSEL, SEGPL(KPL), SEGIG);
	idt_set_gate(&idt[ 2], &INTVEC(nmsk), KCSEL, SEGPL(KPL), SEGIG);
	idt_set_gate(&idt[ 3], &INTVEC(bpt), KCSEL, SEGPL(KPL), SEGIG);
	idt_set_gate(&idt[ 4], &INTVEC(ofl), KCSEL, SEGPL(KPL), SEGIG);
	idt_set_gate(&idt[ 5], &INTVEC(bnd), KCSEL, SEGPL(KPL), SEGIG);
	idt_set_gate(&idt[ 6], &INTVEC(ill), KCSEL, SEGPL(KPL), SEGIG);
	idt_set_gate(&idt[ 7], &INTVEC(dna), KCSEL, SEGPL(KPL), SEGIG);
	idt_set_gate(&idt[ 8], &INTVEC(dbl), KCSEL, SEGPL(KPL), SEGIG);
	idt_set_gate(&idt[ 9], &INTVEC(fpusegm), KCSEL, SEGPL(KPL), SEGIG);
	idt_set_gate(&idt[10], &INTVEC(tss), KCSEL, SEGPL(KPL), SEGIG);
	idt_set_gate(&idt[11], &INTVEC(missing), KCSEL, SEGPL(KPL), SEGIG);
	idt_set_gate(&idt[12], &INTVEC(stk), KCSEL, SEGPL(KPL), SEGIG);
	idt_set_gate(&idt[13], &INTVEC(prot), KCSEL, SEGPL(KPL), SEGIG);
	idt_set_gate(&idt[14], &INTVEC(page), KCSEL, SEGPL(KPL), SEGIG);
	idt_set_gate(&idt[15], &INTVEC(rsvd), KCSEL, SEGPL(KPL), SEGIG);
	idt_set_gate(&idt[16], &INTVEC(fpu), KCSEL, SEGPL(KPL), SEGIG);
	idt_set_gate(&idt[17], &INTVEC(align), KCSEL, SEGPL(KPL), SEGIG);
	idt_set_gate(&idt[18], &INTVEC(mchk), KCSEL, SEGPL(KPL), SEGIG);
	idt_set_gate(&idt[19], &INTVEC(simd), KCSEL, SEGPL(KPL), SEGIG);
	/* 20 - 31 are reserved. */

	pic_remap();

	idt_set_gate(&idt[32], &INTVEC(irq0), KCSEL, SEGPL(KPL), SEGIG);
	idt_set_gate(&idt[33], &INTVEC(irq1), KCSEL, SEGPL(KPL), SEGIG);
	idt_set_gate(&idt[34], &INTVEC(irq2), KCSEL, SEGPL(KPL), SEGIG);
	idt_set_gate(&idt[35], &INTVEC(irq3), KCSEL, SEGPL(KPL), SEGIG);
	idt_set_gate(&idt[36], &INTVEC(irq4), KCSEL, SEGPL(KPL), SEGIG);
	idt_set_gate(&idt[37], &INTVEC(irq5), KCSEL, SEGPL(KPL), SEGIG);
	idt_set_gate(&idt[38], &INTVEC(irq6), KCSEL, SEGPL(KPL), SEGIG);
	idt_set_gate(&idt[39], &INTVEC(irq7), KCSEL, SEGPL(KPL), SEGIG);
	idt_set_gate(&idt[40], &INTVEC(irq8), KCSEL, SEGPL(KPL), SEGIG);
	idt_set_gate(&idt[41], &INTVEC(irq9), KCSEL, SEGPL(KPL), SEGIG);
	idt_set_gate(&idt[42], &INTVEC(irq10), KCSEL, SEGPL(KPL), SEGIG);
	idt_set_gate(&idt[43], &INTVEC(irq11), KCSEL, SEGPL(KPL), SEGIG);
	idt_set_gate(&idt[44], &INTVEC(irq12), KCSEL, SEGPL(KPL), SEGIG);
	idt_set_gate(&idt[45], &INTVEC(irq13), KCSEL, SEGPL(KPL), SEGIG);
	idt_set_gate(&idt[46], &INTVEC(irq14), KCSEL, SEGPL(KPL), SEGIG);
	idt_set_gate(&idt[47], &INTVEC(irq15), KCSEL, SEGPL(KPL), SEGIG);

	/*idt_set_gate(&idt[127], &syscall, KCSEL, SEGPL(UPL), SEGIG);*/

	r_idt.rd_base = (u_int32_t)&idt;
	r_idt.rd_limit = NINT * sizeof(struct gate_desc) - 1;
	__asm__ __volatile("lidt (%0)" : : "r" (&r_idt));
}

static const char *exceptmsg[32] = {
	"division by zero",
	"debug exception",
	"non maskable interrupt",
	"breakpoint",
	"overflow",
	"out of bounds",
	"invalid opcode",
	"no coprocessor",
	"double fault",
	"coprocessor segment overrun",
	"bad tss",
	"segment not present",
	"stack fault",
	"general protection fault",
	"page fault",
	"15 (reserved)",
	"coprocessor fault",
	"alignment check exception (486+)",
	"machine check exception (pentium/586+)",
	"simd error",
	"20 (reserved)",
	"21 (reserved)",
	"22 (reserved)",
	"23 (reserved)",
	"24 (reserved)",
	"25 (reserved)",
	"26 (reserved)",
	"27 (reserved)",
	"28 (reserved)",
	"29 (reserved)",
	"30 (reserved)",
	"31 (reserved)",
};

void
intr_handler(struct regs *r)
{
	intrhand_t handler;

	if ((handler = isr[r->r_intrno]) != NULL)
		handler(r);
	else if (r->r_intrno < ARRLEN(exceptmsg)) {
		dump_regs(r);
		panic("%s: system halted\n", exceptmsg[r->r_intrno]);
	}
	pic_eoi(r->r_intrno);
}

void
intr_register_handler(int intrno, intrhand_t handler)
{
	if (intrno < 0 || intrno >= NINT)
		panic("invalid interrupt number: %d\n", intrno);
	isr[intrno] = handler;
	pic_mask(intrno, PIC_CLEAR_MASK);
}

void
dump_regs(struct regs *r)
{
	printf("eax=%#08x\tebx=%#08x\tecx=%#08x\tedx=%#08x\n",
	    r->r_eax, r->r_ebx, r->r_ecx, r->r_edx);
	printf("esi=%#08x\tedi=%#08x\tebp=%#08x\tesp=%#08x\n",
	    r->r_esi, r->r_edi, r->r_ebp, r->r_esp);
	printf("ds=%#04x \t\tes=%#04x \t\tfs=%#04x \t\tgs=%#04x\n",
	    (u_short)r->r_ds, (u_short)r->r_es, (u_short)r->r_fs, (u_short)r->r_gs);
	printf("cs=%#04x \t\tss=%#04x \t\teip=%#08x\teflags=%#08x\n",
	    (u_short)r->r_cs, (u_short)r->r_ss, r->r_eip, r->r_eflags);
	printf("int=%#08x\terr=%#08x\tuesp=%#08x\n",
	    r->r_intrno, r->r_err, r->r_uesp);
}
