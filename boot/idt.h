#ifndef _KERNEL_IDT_H_
#define _KERNEL_IDT_H_

/* IA-32 */
struct idt_gate {
	uint16_t off_lo;
	uint16_t sel;
	uint8_t zero;
	uint8_t flags;
	uint16_t off_hi;
} __attribute__((packed));

/* This will be populated by int_common_stub. */
struct reg {
	/* Segment registers. Will be popped last. */
	uint32_t gs;
	uint32_t fs;
	uint32_t es;
	uint32_t ds;
	/* General purpose registers. Pushed by `pusha`. */
	uint32_t edi;
	uint32_t esi;
	uint32_t ebp;
	uint32_t esp;
	uint32_t ebx;
	uint32_t edx;
	uint32_t ecx;
	uint32_t eax;
	/* Interrupt info. Pushed by `push byte`. */
	uint32_t intno;
	uint32_t err;
	/* Pushed by the processor. */
	uint32_t eip;
	uint32_t cs;
	uint32_t eflags;
	uint32_t uesp;
	uint32_t ss;
};

/* `kernel_main` calls this. */
extern void idt_init(void);

/* Called by drivers */
extern void int_handler(struct reg *);
extern void int_add_handler(uint8_t, void (*)(struct reg *));

/* The first 32 interrupts are reserved for exceptions. */
extern void ex0(void);
extern void ex1(void);
extern void ex2(void);
extern void ex3(void);
extern void ex4(void);
extern void ex5(void);
extern void ex6(void);
extern void ex7(void);
extern void ex8(void);
extern void ex9(void);
extern void ex10(void);
extern void ex11(void);
extern void ex12(void);
extern void ex13(void);
extern void ex14(void);
extern void ex15(void);
extern void ex16(void);
extern void ex17(void);
extern void ex18(void);
extern void ex19(void);
extern void ex20(void);
extern void ex21(void);
extern void ex22(void);
extern void ex23(void);
extern void ex24(void);
extern void ex25(void);
extern void ex26(void);
extern void ex27(void);
extern void ex28(void);
extern void ex29(void);
extern void ex30(void);
extern void ex31(void);

/* IRQs */
extern void irq0(void);
extern void irq1(void);
extern void irq2(void);
extern void irq3(void);
extern void irq4(void);
extern void irq5(void);
extern void irq6(void);
extern void irq7(void);
extern void irq8(void);
extern void irq9(void);
extern void irq10(void);
extern void irq11(void);
extern void irq12(void);
extern void irq13(void);
extern void irq14(void);
extern void irq15(void);

#endif /* _KERNEL_IDT_H_ */
