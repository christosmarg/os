#ifndef _KERNEL_IDT_H
#define _KERNEL_IDT_H

#define N_INT 256 /* There are 256 interrupts in total. */
#define KERN_CODESEG 0x08 /* Kernel code segment. */

struct idt_reg {
	uint16_t limit;	/* Points at &IDT. */
	uint32_t base;	/* Points at the end of the IDT. */
} __attribute__((packed));

/* IA-32 */
struct idt_gate {
	uint16_t off_lo;
	uint16_t sel;
	uint8_t zero;
	uint8_t attr;
	uint16_t off_hi;
} __attribute__((packed));


/* This will be populated by isr_common_stub. */
struct reg {
	/* Segment registers. Will be poped last. */
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
	/* ISR info. Pushed by `push byte`. */
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

extern void isr_handler(struct reg *);
extern void irq_handler(struct reg *);
extern void irq_add_handler(uint8_t, void (*)(struct reg *));
extern void irq_rm_handler(uint8_t);

/* The first 32 ISRs are reserved for exceptions. */
extern void isr0(void);
extern void isr1(void);
extern void isr2(void);
extern void isr3(void);
extern void isr4(void);
extern void isr5(void);
extern void isr6(void);
extern void isr7(void);
extern void isr8(void);
extern void isr9(void);
extern void isr10(void);
extern void isr11(void);
extern void isr12(void);
extern void isr13(void);
extern void isr14(void);
extern void isr15(void);
extern void isr16(void);
extern void isr17(void);
extern void isr18(void);
extern void isr19(void);
extern void isr20(void);
extern void isr21(void);
extern void isr22(void);
extern void isr23(void);
extern void isr24(void);
extern void isr25(void);
extern void isr26(void);
extern void isr27(void);
extern void isr28(void);
extern void isr29(void);
extern void isr30(void);
extern void isr31(void);

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

#endif /* _KERNEL_IDT_H */
