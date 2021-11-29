; Defined in `idt.h`.
[extern intr_handler]

%macro intr_noerr 2
global intr_%1
intr_%1:
	cli
	push	byte 0
	push	byte %2
	jmp	intr_common_stub
%endmacro

%macro intr_err 2
global intr_%1
intr_%1:
	cli
	push	byte %2
	jmp	intr_common_stub
%endmacro

; Exceptions
intr_noerr	div,	0  ; Division By Zero
intr_noerr	dbg,	1  ; Debug
intr_noerr	nmsk,	2  ; Non Maskable Interrupt
intr_noerr	bpt,	3  ; Breakpoint
intr_noerr	ofl,	4  ; Into Detected Overflow
intr_noerr	bnd,	5  ; Out of Bounds
intr_noerr	ill,	6  ; Invalid Opcode
intr_noerr	dna,	7  ; No Coprocessor
intr_err	dbl,	8  ; Double Fault (with error code)
intr_noerr	fpusegm,9  ; Coprocessor Segment Overrrun
intr_err	tss,	10 ; Bad TSS (with error code)
intr_err	missing,11 ; Segment Not Present (with error code)
intr_err	stk,	12 ; Stack Fault (with error code)
intr_err	prot,	13 ; General Protection Fault (with error code)
intr_err	page,	14 ; Page Fault (with error code)
intr_noerr	rsvd,	15 ; Unkown Interrupt
intr_noerr	fpu,	16 ; Coprocessor Fault
intr_noerr	align,	17 ; Alignment Check (486+)
intr_noerr	mchk,	18 ; Machine Check (Pentium/586+)
intr_noerr	simd,	19 ; Reserved ; FIXME: ??

; IRQs
intr_noerr	irq0,	32 ; Programmable Interrupt Timer
intr_noerr	irq1,	33 ; Keyboard
intr_noerr	irq2,	34 ; Cascade (used internally by the two PICs, never raised)
intr_noerr	irq3,	35 ; COM2 (if enabled)
intr_noerr	irq4,	36 ; COM1 (if enabled)
intr_noerr	irq5,	37 ; LPT2 (if enabled)
intr_noerr	irq6,	38 ; Floppy Disk
intr_noerr	irq7,	39 ; LPT1
intr_noerr	irq8,	40 ; CMOS real-time clock (if enabled)
intr_noerr	irq9,	41 ; Peripherals / Legacy SCSI / NIC
intr_noerr	irq10,	42 ; Peripherals / SCSI / NIC
intr_noerr	irq11,	43 ; Peripherals / SCSI / NIC
intr_noerr	irq12,	44 ; PS2 Mouse
intr_noerr	irq13,	45 ; FPU / Coprocessor / Inter-processor
intr_noerr	irq14,	46 ; Primary ATA Hard Disk
intr_noerr	irq15,	47 ; Secondary ATA Hard Disk

; Save the processor state, call the C interrupt handler and restore the
; stack frame.
intr_common_stub:
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
	mov	eax, intr_handler 
	call	eax
	pop	eax
	pop	gs
	pop	fs
	pop	es
	pop	ds
	popa
	add	esp, 8		; Clean up ISR info.
	sti
	iret			; I spent many hours debugging this...

; TODO: irq_common_stub bx?
