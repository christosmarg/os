#ifndef _KERNEL_IDT_H_
#define _KERNEL_IDT_H_

/* IA-32 */
struct gate_desc {
	uint16_t	gd_off_lo;
	uint16_t	gd_sel;
	uint8_t		gd_rsvd;
	uint8_t		gd_flags; /* type, dpl, p */
	uint16_t	gd_off_hi;
} __packed;

struct region_desc {
	uint16_t	rd_limit;
	uint32_t	rd_base;
} __packed;

/* TODO: keep counter? */

/* This will be populated by `intr_common_stub` in `intr.asm`. */
/* FIXME: move to i386 dir? */
struct reg {
	/* Will be popped last. */
	uint32_t	r_gs;
	uint32_t	r_fs;
	uint32_t	r_es;
	uint32_t	r_ds;
	/* Pushed by `pusha`. */
	uint32_t	r_edi;
	uint32_t	r_esi;
	uint32_t	r_ebp;
	uint32_t	r_esp;
	uint32_t	r_ebx;
	uint32_t	r_edx;
	uint32_t	r_ecx;
	uint32_t	r_eax;
	/* Interrupt info. Pushed by `push byte`. */
	uint32_t	r_intrno;
	uint32_t	r_err;
	/* Pushed by the CPU. */
	uint32_t	r_eip;
	uint32_t	r_cs;
	uint32_t	r_eflags;
	uint32_t	r_uesp;
	uint32_t	r_ss;
};

typedef void (*intrhand_t)(struct reg *);

/* Called by `kern_main`. */
void idt_init(void);

/* Called by drivers. */
void intr_handler(struct reg *);
void intr_register_handler(uint8_t, intrhand_t); 

#endif /* _KERNEL_IDT_H_ */
