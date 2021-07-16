; Defined in `idt.h`.
[extern int_handler]

; Exceptions
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

%macro intdef_noerr 1
	cli
	push	byte 0
	push	byte %1
	jmp	int_common_stub
%endmacro

%macro intdef_err 1
	cli
	push	byte 0
	push	byte %1
	jmp	int_common_stub
%endmacro

; Exceptions
ex0:	intdef_noerr	0  ; Division By Zero
ex1:	intdef_noerr	1  ; Debug
ex2:	intdef_noerr	2  ; Non Maskable Interrupt
ex3:	intdef_noerr	3  ; Breakpoint
ex4:	intdef_noerr	4  ; Into Detected Overflow
ex5:	intdef_noerr	5  ; Out of Bounds
ex6:	intdef_noerr	6  ; Invalid Opcode
ex7:	intdef_noerr	7  ; No Coprocessor
ex8:	intdef_err	8  ; Double Fault (with error code)
ex9:	intdef_noerr	9  ; Coprocessor Segment Overrrun
ex10:	intdef_err	10 ; Bad TSS (with error code)
ex11:	intdef_err	11 ; Segment Not Present (with error code)
ex12:	intdef_err	12 ; Stack Fault (with error code)
ex13:	intdef_err	13 ; General Protection Fault (with error code)
ex14:	intdef_err	14 ; Page Fault (with error code)
ex15:	intdef_noerr	15 ; Unkown Interrupt
ex16:	intdef_noerr	16 ; Coprocessor Fault
ex17:	intdef_noerr	17 ; Alignment Check (486+)
ex18:	intdef_noerr	18 ; Machine Check (Pentium/586+)
ex19:	intdef_noerr	19 ; Reserved
ex20:	intdef_noerr	20 ; Reserved
ex21:	intdef_noerr	21 ; Reserved
ex22:	intdef_noerr	22 ; Reserved
ex23:	intdef_noerr	23 ; Reserved
ex24:	intdef_noerr	24 ; Reserved
ex25:	intdef_noerr	25 ; Reserved
ex26:	intdef_noerr	26 ; Reserved
ex27:	intdef_noerr	27 ; Reserved
ex28:	intdef_noerr	28 ; Reserved
ex29:	intdef_noerr	29 ; Reserved
ex30:	intdef_noerr	30 ; Reserved
ex31:	intdef_noerr	31 ; Reserved
; IRQs
irq0:	intdef_noerr	32 ; Programmable Interrupt Timer
irq1:	intdef_noerr	33 ; Keyboard
irq2:	intdef_noerr	34 ; Cascade (used internally by the two PICs, never raised)
irq3:	intdef_noerr	35 ; COM2 (if enabled)
irq4:	intdef_noerr	36 ; COM1 (if enabled)
irq5:	intdef_noerr	37 ; LPT2 (if enabled)
irq6:	intdef_noerr	38 ; Floppy Disk
irq7:	intdef_noerr	39 ; LPT1
irq8:	intdef_noerr	40 ; CMOS real-time clock (if enabled)
irq9:	intdef_noerr	41 ; Peripherals / Legacy SCSI / NIC
irq10:	intdef_noerr	42 ; Peripherals / SCSI / NIC
irq11:	intdef_noerr	43 ; Peripherals / SCSI / NIC
irq12:	intdef_noerr	44 ; PS2 Mouse
irq13:	intdef_noerr	45 ; FPU / Coprocessor / Inter-processor
irq14:	intdef_noerr	46 ; Primary ATA Hard Disk
irq15:	intdef_noerr	47 ; Secondary ATA Hard Disk

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
