#ifndef _IDT_H_
#define _IDT_H_

#include <u.h>
#include <regs.h>

#define INTVEC(name)	CONCAT(intr_, name)

#define NINT		256		/* number of interrupts */

/* gdt segments */
#define NULLSEG		0		/* null segment */
#define KCSEG		1		/* kernel code segment */
#define KDSEG		2		/* kernel data segment */
#define UCSEG		3		/* user code segment */
#define UDSEG		4		/* kernel data segment */
/* TODO: 9front mem.h */

#define SELGDT		(0 << 2)	/* selector is in gdt */
#define SELLDT		(1 << 2)	/* selector is in ldt */

#define SEGIG		(0x0e << 8)	/* interrupt gate */
#define SEGPL(x)	((x) << 13)	/* priority level */
#define SEGP		(1 << 15)	/* segment present */

#define KPL		0		/* kernel priority level */
#define DPL1		1		/* device driver priority level 1 */
#define DPL2		2		/* device driver priority level 2 */
#define UPL		3		/* user priority level */

#define SELECTOR(i, t, p)	(((i) << 3) | (t) | (p))

/* selectors */
#define NULLSEL		SELECTOR(NULLSEG, SELGDT, KPL)
#define KDSEL		SELECTOR(KDSEG, SELGDT, KPL)
#define KCSEL		SELECTOR(KCSEG, SELGDT, KPL)
#define UCSEL		SELECTOR(UCSEG, SELGDT, UPL)
#define UDSEL		SELECTOR(UDSEG, SELGDT, UPL)
/* TODO: 9front mem.h */

struct gate_desc {
	u_int32_t gd_lo;
	u_int32_t gd_hi;
} __packed;

struct region_desc {
	u_int16_t rd_limit;
	u_int32_t rd_base;
} __packed;

enum {
	IRQ0 = 32,
	IRQ1,
	IRQ2,
	IRQ3,
	IRQ4,
	IRQ5,
	IRQ6,
	IRQ7,
	IRQ8,
	IRQ9,
	IRQ10,
	IRQ11,
	IRQ12,
	IRQ13,
	IRQ14,
	IRQ15,
};

typedef void (*intrhand_t)(struct regs *);

void idt_init(void);
void intr_handler(struct regs *);
void intr_register_handler(int, intrhand_t);
void dump_regs(struct regs *); /* FIXME: move elsewhere? */

#endif /* _IDT_H_ */
