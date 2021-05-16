; C functions defined in `idt.h`.
[extern int_handler]

; ISRs
global ex0
global ex1
global ex2
global ex3
global ex4
global ex5
global ex6
global ex7
global ex8
global ex9
global ex10
global ex11
global ex12
global ex13
global ex14
global ex15
global ex16
global ex17
global ex18
global ex19
global ex20
global ex21
global ex22
global ex23
global ex24
global ex25
global ex26
global ex27
global ex28
global ex29
global ex30
global ex31

; IRQs
global irq0
global irq1
global irq2
global irq3
global irq4
global irq5
global irq6
global irq7
global irq8
global irq9
global irq10
global irq11
global irq12
global irq13
global irq14
global irq15

; ================== EXCEPTIONS ======================

; 0 - Division By Zero
ex0:
	cli
	push	byte 0
	push	byte 0
	jmp	int_common_stub

; 1 - Debug
ex1:
	cli
	push	byte 0
	push	byte 1
	jmp	int_common_stub

; 2 - Non Maskable Interrupt
ex2:
	cli
	push	byte 0
	push	byte 2
	jmp	int_common_stub

; 3 - Breakpoint
ex3:
	cli
	push	byte 0
	push	byte 3
	jmp	int_common_stub

; 4 - Into Detected Overflow
ex4:
	cli
	push	byte 0
	push	byte 4
	jmp	int_common_stub

; 5 - Out of Bounds
ex5:
	cli
	push	byte 0
	push	byte 5
	jmp	int_common_stub

; 6 - Invalid Opcode
ex6:
	cli
	push	byte 0
	push	byte 6
	jmp	int_common_stub

; 7 - No Coprocessor
ex7:
	cli
	push	byte 0
	push	byte 7
	jmp	int_common_stub

; 8  - Double Fault (with error code)
ex8:
	cli
	push	byte 8
	jmp	int_common_stub

; 9 - Coprocessor Segment Overrrun
ex9:
	cli
	push	byte 0
	push	byte 9
	jmp	int_common_stub

; 10 - Bad TSS (with error code)
ex10:
	cli
	push	byte 10
	jmp	int_common_stub

; 11 - Segment Not Present (with error code)
ex11:
	cli
	push	byte 11
	jmp	int_common_stub

; 12 - Stack Fault (with error code)
ex12:
	cli
	push	byte 12
	jmp	int_common_stub

; 13 - General Protection Fault (with error code)
ex13:
	cli
	push	byte 13
	jmp	int_common_stub

; 14 - Page Fault (with error code)
ex14:
	cli
	push	byte 14
	jmp	int_common_stub

; 15 - Unkown Interrupt
ex15:
	cli
	push	byte 0
	push	byte 15
	jmp	int_common_stub

; 16 - Coprocessor Fault
ex16:
	cli
	push	byte 0
	push	byte 16
	jmp	int_common_stub

; 17 - Alignment Check (486+)
ex17:
	cli
	push	byte 0
	push	byte 17
	jmp	int_common_stub

; 18 - Machine Check (Pentium/586+)
ex18:
	cli
	push	byte 0
	push	byte 18
	jmp	int_common_stub

; 19 - Reserved
ex19:
	cli
	push	byte 0
	push	byte 19
	jmp	int_common_stub

; 20 - Reserved
ex20:
	cli
	push	byte 0
	push	byte 20
	jmp	int_common_stub

; 21 - Reserved
ex21:
	cli
	push	byte 0
	push	byte 21
	jmp	int_common_stub

; 22 - Reserved
ex22:
	cli
	push	byte 0
	push	byte 22
	jmp	int_common_stub

; 23 - Reserved
ex23:
	cli
	push	byte 0
	push	byte 23
	jmp	int_common_stub

; 24 - Reserved
ex24:
	cli
	push	byte 0
	push	byte 24
	jmp	int_common_stub

; 25 - Reserved
ex25:
	cli
	push	byte 0
	push	byte 25
	jmp	int_common_stub

; 26 - Reserved
ex26:
	cli
	push	byte 0
	push	byte 26
	jmp	int_common_stub

; 27 - Reserved
ex27:
	cli
	push	byte 0
	push	byte 27
	jmp	int_common_stub

; 28 - Reserved
ex28:
	cli
	push	byte 0
	push	byte 28
	jmp	int_common_stub

; 29 - Reserved
ex29:
	cli
	push	byte 0
	push	byte 29
	jmp	int_common_stub

; 30 - Reserved
ex30:
	cli
	push	byte 0
	push	byte 30
	jmp	int_common_stub

; 31 - Reserved
ex31:
	cli
	push	byte 0
	push	byte 31
	jmp	int_common_stub

; ================== IRQ ======================

; 32 - Programmable Interrupt Timer
irq0:
	cli
	push	byte 0
	push	byte 32
	jmp	int_common_stub

; 33 - Keyboard
irq1:
	cli
	push	byte 0
	push	byte 33
	jmp	int_common_stub

; 34  Cascade (used internally by the two PICs, never raised)
irq2:
	cli
	push	byte 0
	push	byte 34
	jmp	int_common_stub

; 35 - COM2 (if enabled)
irq3:
	cli
	push	byte 0
	push	byte 35
	jmp	int_common_stub

; 36 - COM1 (if enabled)
irq4:
	cli
	push	byte 0
	push	byte 36
	jmp	int_common_stub

; 37 - LPT2 (if enabled)
irq5:
	cli
	push	byte 0
	push	byte 37
	jmp	int_common_stub

; 38 - Floppy Disk
irq6:
	cli
	push	byte 0
	push	byte 38
	jmp	int_common_stub

; 39 - LPT1
irq7:
	cli
	push	byte 0
	push	byte 39
	jmp	int_common_stub

; 40 - CMOS real-time clock (if enabled)
irq8:
	cli
	push	byte 0
	push	byte 40
	jmp	int_common_stub

; 41 - Peripherals / Legacy SCSI / NIC
irq9:
	cli
	push	byte 0
	push	byte 41
	jmp	int_common_stub

; 42 - Peripherals / SCSI / NIC
irq10:
	cli
	push	byte 0
	push	byte 42
	jmp	int_common_stub

; 43 - Peripherals / SCSI / NIC
irq11:
	cli
	push	byte 0
	push	byte 43
	jmp	int_common_stub

; 44 - PS2 Mouse
irq12:
	cli
	push	byte 0
	push	byte 44
	jmp	int_common_stub

; 45 - FPU / Coprocessor / Inter-processor
irq13:
	cli
	push	byte 0
	push	byte 45
	jmp	int_common_stub

; 46 - Primary ATA Hard Disk
irq14:
	cli
	push	byte 0
	push	byte 46
	jmp	int_common_stub

; 47 - Secondary ATA Hard Disk
irq15:
	cli
	push	byte 0
	push	byte 47
	jmp	int_common_stub

; Save the processor state, call the C interrupt handler and restore the
; stack frame.
int_common_stub:
	pusha
	push	ds
	push	es
	push	fs
	push	gs
	mov	ax, 0x10	; Kernel data segment descriptor.
	mov	ds, ax
	mov	es, ax
	mov	fs, ax
	mov	gs, ax
	mov	eax, esp	; Push the stack.
	push	eax
	mov	eax, int_handler 
	call	eax
	pop	eax
	pop	gs
	pop	fs
	pop	es
	pop	ds
	popa
	add	esp, 8		; Clean up ISR info.
	sti
	iret
