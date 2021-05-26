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

/* This will be populated by `int_common_stub` in `int.asm`. */
struct reg {
	/* Will be popped last. */
	uint32_t gs;
	uint32_t fs;
	uint32_t es;
	uint32_t ds;
	/* Pushed by `pusha`. */
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
	/* Pushed by the CPU. */
	uint32_t eip;
	uint32_t cs;
	uint32_t eflags;
	uint32_t uesp;
	uint32_t ss;
};

/* Called by `kmain`. */
void idt_init(void);

/* Called by drivers. */
void int_handler(struct reg *);
void int_add_handler(uint8_t, void (*)(struct reg *));

/* The first 32 interrupts are reserved for exceptions. */
void ex0(void);
void ex1(void);
void ex2(void);
void ex3(void);
void ex4(void);
void ex5(void);
void ex6(void);
void ex7(void);
void ex8(void);
void ex9(void);
void ex10(void);
void ex11(void);
void ex12(void);
void ex13(void);
void ex14(void);
void ex15(void);
void ex16(void);
void ex17(void);
void ex18(void);
void ex19(void);
void ex20(void);
void ex21(void);
void ex22(void);
void ex23(void);
void ex24(void);
void ex25(void);
void ex26(void);
void ex27(void);
void ex28(void);
void ex29(void);
void ex30(void);
void ex31(void);

/* IRQs */
void irq0(void);
void irq1(void);
void irq2(void);
void irq3(void);
void irq4(void);
void irq5(void);
void irq6(void);
void irq7(void);
void irq8(void);
void irq9(void);
void irq10(void);
void irq11(void);
void irq12(void);
void irq13(void);
void irq14(void);
void irq15(void);

#endif /* _KERNEL_IDT_H_ */
