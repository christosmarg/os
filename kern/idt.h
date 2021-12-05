#ifndef _IDT_H_
#define _IDT_H_

#include <u.h>
#include <reg.h>

#define INTVEC(name)	__CONCAT(intr_, name)

struct gate_desc {
	u_int16_t	gd_off_lo;
	u_int16_t	gd_sel;
	u_int8_t	gd_rsvd;
	u_int8_t	gd_flags; /* type, dpl, p */
	u_int16_t	gd_off_hi;
} __packed;

struct region_desc {
	u_int16_t	rd_limit;
	u_int32_t	rd_base;
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

typedef void (*intrhand_t)(struct reg *);

void idt_init(void);
void intr_handler(struct reg *);
void intr_register_handler(u_int8_t, intrhand_t); 
void print_regs(struct reg *); /* FIXME: move elsewhere? */

#endif /* _IDT_H_ */
