[extern isr_handler]
[extern irq_handler]

; ISRs
global isr0
global isr1
global isr2
global isr3
global isr4
global isr5
global isr6
global isr7
global isr8
global isr9
global isr10
global isr11
global isr12
global isr13
global isr14
global isr15
global isr16
global isr17
global isr18
global isr19
global isr20
global isr21
global isr22
global isr23
global isr24
global isr25
global isr26
global isr27
global isr28
global isr29
global isr30
global isr31

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

; ================== ISR ======================

; 0 - Division By Zero
isr0:
	cli
	push	byte 0
	push	byte 0
	jmp	isr_common_stub

; 1 - Debug
isr1:
	cli
	push	byte 0
	push	byte 1
	jmp	isr_common_stub

; 2 - Non Maskable Interrupt
isr2:
	cli
	push	byte 0
	push	byte 2
	jmp	isr_common_stub

; 3 - Breakpoint
isr3:
	cli
	push	byte 0
	push	byte 3
	jmp	isr_common_stub

; 4 - Into Detected Overflow
isr4:
	cli
	push	byte 0
	push	byte 4
	jmp	isr_common_stub

; 5 - Out of Bounds
isr5:
	cli
	push	byte 0
	push	byte 5
	jmp	isr_common_stub

; 6 - Invalid Opcode
isr6:
	cli
	push	byte 0
	push	byte 6
	jmp	isr_common_stub

; 7 - No Coprocessor
isr7:
	cli
	push	byte 0
	push	byte 7
	jmp	isr_common_stub

; 8  - Double Fault (with error code)
isr8:
	cli
	push	byte 8
	jmp	isr_common_stub

; 9 - Coprocessor Segment Overrrun
isr9:
	cli
	push	byte 0
	push	byte 9
	jmp	isr_common_stub

; 10 - Bad TSS (with error code)
isr10:
	cli
	push	byte 10
	jmp	isr_common_stub

; 11 - Segment Not Present (with error code)
isr11:
	cli
	push	byte 11
	jmp	isr_common_stub

; 12 - Stack Fault (with error code)
isr12:
	cli
	push	byte 12
	jmp	isr_common_stub

; 13 - General Protection Fault (with error code)
isr13:
	cli
	push	byte 13
	jmp	isr_common_stub

; 14 - Page Fault (with error code)
isr14:
	cli
	push	byte 14
	jmp	isr_common_stub

; 15 - Unkown Interrupt
isr15:
	cli
	push	byte 0
	push	byte 15
	jmp	isr_common_stub

; 16 - Coprocessor Fault
isr16:
	cli
	push	byte 0
	push	byte 16
	jmp	isr_common_stub

; 17 - Alignment Check (486+)
isr17:
	cli
	push	byte 0
	push	byte 17
	jmp	isr_common_stub

; 18 - Machine Check (Pentium/586+)
isr18:
	cli
	push	byte 0
	push	byte 18
	jmp	isr_common_stub

; 19 - Reserved
isr19:
	cli
	push	byte 0
	push	byte 19
	jmp	isr_common_stub

; 20 - Reserved
isr20:
	cli
	push	byte 0
	push	byte 20
	jmp	isr_common_stub

; 21 - Reserved
isr21:
	cli
	push	byte 0
	push	byte 21
	jmp	isr_common_stub

; 22 - Reserved
isr22:
	cli
	push	byte 0
	push	byte 22
	jmp	isr_common_stub

; 23 - Reserved
isr23:
	cli
	push	byte 0
	push	byte 23
	jmp	isr_common_stub

; 24 - Reserved
isr24:
	cli
	push	byte 0
	push	byte 24
	jmp	isr_common_stub

; 25 - Reserved
isr25:
	cli
	push	byte 0
	push	byte 25
	jmp	isr_common_stub

; 26 - Reserved
isr26:
	cli
	push	byte 0
	push	byte 26
	jmp	isr_common_stub

; 27 - Reserved
isr27:
	cli
	push	byte 0
	push	byte 27
	jmp	isr_common_stub

; 28 - Reserved
isr28:
	cli
	push	byte 0
	push	byte 28
	jmp	isr_common_stub

; 29 - Reserved
isr29:
	cli
	push	byte 0
	push	byte 29
	jmp	isr_common_stub

; 30 - Reserved
isr30:
	cli
	push	byte 0
	push	byte 30
	jmp	isr_common_stub

; 31 - Reserved
isr31:
	cli
	push	byte 0
	push	byte 31
	jmp	isr_common_stub

; ================== IRQ ======================

; 32 - Programmable Interrupt Timer
irq0:
	cli
	push	byte 0
	push	byte 32
	jmp	irq_common_stub

; 33 - Keyboard
irq1:
	cli
	push	byte 0
	push	byte 33
	jmp	irq_common_stub

; 34  Cascade (used internally by the two PICs, never raised)
irq2:
	cli
	push	byte 0
	push	byte 34
	jmp	irq_common_stub

; 35 - COM2 (if enabled)
irq3:
	cli
	push	byte 0
	push	byte 35
	jmp	irq_common_stub

; 36 - COM1 (if enabled)
irq4:
	cli
	push	byte 0
	push	byte 36
	jmp	irq_common_stub

; 37 - LPT2 (if enabled)
irq5:
	cli
	push	byte 0
	push	byte 37
	jmp	irq_common_stub

; 38 - Floppy Disk
irq6:
	cli
	push	byte 0
	push	byte 38
	jmp	irq_common_stub

; 39 - LPT1
irq7:
	cli
	push	byte 0
	push	byte 39
	jmp	irq_common_stub

; 40 - CMOS real-time clock (if enabled)
irq8:
	cli
	push	byte 0
	push	byte 40
	jmp	irq_common_stub

; 41 - Peripherals / Legacy SCSI / NIC
irq9:
	cli
	push	byte 0
	push	byte 41
	jmp	irq_common_stub

; 42 - Peripherals / SCSI / NIC
irq10:
	cli
	push	byte 0
	push	byte 42
	jmp	irq_common_stub

; 43 - Peripherals / SCSI / NIC
irq11:
	cli
	push	byte 0
	push	byte 43
	jmp	irq_common_stub

; 44 - PS2 Mouse
irq12:
	cli
	push	byte 0
	push	byte 44
	jmp	irq_common_stub

; 45 - FPU / Coprocessor / Inter-processor
irq13:
	cli
	push	byte 0
	push	byte 45
	jmp	irq_common_stub

; 46 - Primary ATA Hard Disk
irq14:
	cli
	push	byte 0
	push	byte 46
	jmp	irq_common_stub

; 47 - Secondary ATA Hard Disk
irq15:
	cli
	push	byte 0
	push	byte 47
	jmp	irq_common_stub

; Save the processor state, call the C fault handler and restore the stack frame.
isr_common_stub:
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
	mov	eax, isr_handler
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

; Call the C handler and restore the stack frame.
irq_common_stub:
	pusha
	push	ds
	push	es
	push	fs
	push	gs
	mov	ax, 0x10
	mov	ds, ax
	mov	es, ax
	mov	fs, ax
	mov	gs, ax
	mov	eax, esp
	push	eax
	mov	eax, irq_handler
	call	eax
	pop	eax
	pop	gs
	pop	fs
	pop	es
	pop	ds
	popa
	add	esp, 8
	sti
	iret
