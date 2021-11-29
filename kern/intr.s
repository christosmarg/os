; Defined in `idt.h`.
[extern intr_handler]

global ex_div
global ex_dbg
global ex_nmsk
global ex_bpt
global ex_ofl
global ex_bnd
global ex_ill
global ex_dna
global ex_dbl
global ex_fpusegm
global ex_tss
global ex_missing
global ex_stk
global ex_prot
global ex_page
global ex_rsvd
global ex_fpu
global ex_align
global ex_mchk
global ex_simd

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

%macro intr_noerr 1
	cli
	push	byte 0
	push	byte %1
	jmp	intr_common_stub
%endmacro

%macro intr_err 1
	cli
	push	byte %1
	jmp	intr_common_stub
%endmacro

; Exceptions
ex_div:		intr_noerr	0  ; Division By Zero
ex_dbg:		intr_noerr	1  ; Debug
ex_nmsk:	intr_noerr	2  ; Non Maskable Interrupt
ex_bpt:		intr_noerr	3  ; Breakpoint
ex_ofl:		intr_noerr	4  ; Into Detected Overflow
ex_bnd:		intr_noerr	5  ; Out of Bounds
ex_ill:		intr_noerr	6  ; Invalid Opcode
ex_dna:		intr_noerr	7  ; No Coprocessor
ex_dbl:		intr_err	8  ; Double Fault (with error code)
ex_fpusegm:	intr_noerr	9  ; Coprocessor Segment Overrrun
ex_tss:		intr_err	10 ; Bad TSS (with error code)
ex_missing:	intr_err	11 ; Segment Not Present (with error code)
ex_stk:		intr_err	12 ; Stack Fault (with error code)
ex_prot:	intr_err	13 ; General Protection Fault (with error code)
ex_page:	intr_err	14 ; Page Fault (with error code)
ex_rsvd:	intr_noerr	15 ; Unkown Interrupt
ex_fpu:		intr_noerr	16 ; Coprocessor Fault
ex_align:	intr_noerr	17 ; Alignment Check (486+)
ex_mchk:	intr_noerr	18 ; Machine Check (Pentium/586+)
ex_simd:	intr_noerr	19 ; Reserved ; FIXME: ??

; IRQs
irq0:		intr_noerr	32 ; Programmable Interrupt Timer
irq1:		intr_noerr	33 ; Keyboard
irq2:		intr_noerr	34 ; Cascade (used internally by the two PICs, never raised)
irq3:		intr_noerr	35 ; COM2 (if enabled)
irq4:		intr_noerr	36 ; COM1 (if enabled)
irq5:		intr_noerr	37 ; LPT2 (if enabled)
irq6:		intr_noerr	38 ; Floppy Disk
irq7:		intr_noerr	39 ; LPT1
irq8:		intr_noerr	40 ; CMOS real-time clock (if enabled)
irq9:		intr_noerr	41 ; Peripherals / Legacy SCSI / NIC
irq10:		intr_noerr	42 ; Peripherals / SCSI / NIC
irq11:		intr_noerr	43 ; Peripherals / SCSI / NIC
irq12:		intr_noerr	44 ; PS2 Mouse
irq13:		intr_noerr	45 ; FPU / Coprocessor / Inter-processor
irq14:		intr_noerr	46 ; Primary ATA Hard Disk
irq15:		intr_noerr	47 ; Secondary ATA Hard Disk

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
