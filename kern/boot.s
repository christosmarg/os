; Load code to 0x7c00 (Boot Sector address).
[org 0x7c00]
[bits 16]

MAGICOFF	equ (0x7c00 + 510)
KERNLOAD	equ 0x1000
KERNOFF		equ 0x1000
STACKSIZE	equ 8*1024*1024

section .text
	global _start

; Entry point.
_start:
	cli			; Disable interrupts.
	jmp	0x0000:zeroseg
zeroseg:
	xor	ax, ax		; Clear segment registers.
	mov	ds, ax
	mov	es, ax
	mov	fs, ax
	mov	gs, ax
	mov	ss, ax		; Stack starts at 0.
	mov	bp, _start
	mov	sp, bp
	cld			; Read strings from low to high.
	sti			; Enable interrupts back.

	call	a20_test
	call	a20_enable
	call	kernel_load	; Read kernel from disk to memory.
	jmp	pm_enter	; Enter 32-bit Protected Mode.
	jmp	$		; Safety hang.

; Test to see if the A20 line is enabled.
; As a reference we will use the magic number 0xaa55 since we know it's always
; going to have the same value and be located at address 0x7dfe (0x7c00 + 510).
; If the return code in `ax` is 1, the A20 is enabled, and if it's 0 it's disabled.
a20_test:
	pusha
	
	mov	ax, [MAGICOFF] ; The magic number is there.
	mov	dx, ax

; We'll try to advance 1MB in memory. If the end result hasn't wrapped up
; around zero, it means that our processor is able to access more than 1MB
; of addresses, so the A20 is enabled. Using 0x7dfe again, the expected
; address after the 1MB advance should be:
;
;	0x100000 + 0x7dfe = 0x107dfe
;
; The formula for memory segmentation is:
; 
;	address = segment * 16 + offset
;
; To find the offset, the segment will be 0xffff, so `0xffff * 16 = 0xffff0`.
; Applying the formula to calculate our offset, we have:
; 
;	0xffff0 + offset = 0x107dfe =>
;	offset = 0x107dfe - 0xffff0 =>
;	offset = 0x7e0e

	push	bx
	mov	bx, 0xffff
	mov	es, bx		; Assign value to the segment register.
	pop	bx
	
	mov	bx, 0x7e0e
	mov	dx, [es:bx]

	cmp	ax, dx
	je	cont		; If 0xaa55, the A20 line might be disabled.
	popa
	mov	ax, 0x01	; Success code 1.
	ret
cont:
	mov	ax, [0x7dff]
	mov	dx, ax

	push	bx
	mov	bx, 0xffff	; Make another segment.
	mov	es, bx
	pop	bx

	mov	bx, 0x7e0f
	mov	dx, [es:bx]

	cmp	ax, dx		; Now it really is disabled if ax == [es:bx].
	je	exit
	popa
	mov	ax, 0x01	; Success code 1.
	ret
exit:
	popa
	xor	ax, ax		; Error code 0.	
	ret

; Enable the A20 line. We'll try enabling it using the following ways:
;	- BIOS interrupt.
;	- Keyboard controller.
;	- FastA20.
a20_enable:
	pusha

	; BIOS interrupt.
	mov	ax, 0x2401	; A20-Gate Active.
	int	0x15

	call	a20_test
	cmp	ax, 0x01
	je	a20_done
	jmp	a20_fail

	; Keyboard controller.
	sti			; Enable interrupts.

	call	a20_waitc
	mov	al, 0xad	; Disable controller.
	out	0x64, al	; Send data to port 0x64.
	call	a20_waitc
	mov	al, 0xd0	; We want to read from the controller.
	out	0x64, al
	
	call	a20_waitd
	in	al, 0x60	; Read data from port 0x60.
	push	ax		; Save data.

	call	a20_waitc
	mov	al, 0xd1	; We want to send data.
	out	0x64, al
	
	call	a20_waitc
	pop	ax
	or	al, 0x02	; Write the second bit back.
	out	0x60, al	; Send data to the data port.

	call	a20_waitc
	mov	al, 0xae	; Enable controller.
	out	0x64, al
	
	call	a20_waitc
	sti			; Enable interrupts again.

	call	a20_test
	cmp	ax, 0x01
	je	a20_done
	jmp	a20_fail

; Wait for the keyboard controller to be available for receiving commands.
a20_waitc:
	in	al, 0x64	; Read from port 0x64.
	test	al, 0x02	; Test the second bit.
	jnz	a20_waitc	; If it's 1, it's busy.
	ret
; Wait for the keyboard controller to send data back.
a20_waitd:
	in	al, 0x64	; Read from port 0x64 again.
	test	al, 0x01	; Test the first bit.
	jz	a20_waitd	; If it's 1, the data is not ready to be sent.
	ret

; FastA20
	in	al, 0x92	; FastA20 uses port 0x92. 
	or	al, 0x02	; Mask second bit.
	out	0x92, al	; Send data back to port 0x92.

	call	a20_test
	cmp	ax, 0x01
	je	a20_done
	jmp	a20_fail

; The A20 is enabled. Go on.
a20_done:
	popa
	ret

; The A20 is disabled.
a20_fail:
	mov	si, str_a20_fail
	call	puts
	jmp	$

; Load kernel from disk to memory.
kernel_load:
	pusha
	mov	ah, 0x42	; BIOS read disk extended function.
	mov	dl, [BOOTDRV]	; Set boot drive.
	mov	si, disk_packet
	int	0x13		; BIOS read disk interrupt.

	jc	diskerr		; There's an error. Too bad.
	popa
	ret
diskerr:
	mov	si, str_diskerr
	call	puts
	jmp	$		; There's nothing we can do at this point.

disk_packet:
	size:	db 0x10
	zero:	db 0x00
	count:	dw 0x0030	; FIXME: i don't know why this works...
	off16:	dw KERNLOAD
	seg16:	dw 0x0000
	lba:	dq 1

; Set up the GDT (Global Descriptor Table).
gdt:
gdt_null:			; offset = 0x0
	dq	0x00000000

gdt_kernel_code:		; offset = 0x08
	dw	0xffff		; segment limit 0..15
	dw	0x0000		; segment base 0..15
	db	0x00		; segment base 16..23 set for 0K
	db	0x9f		; flags: type
	db	0xcf		; flags: limit
	db	0x00		; segment base 24..32

gdt_kernel_data:		; offset = 0x10
	dw	0xffff
	dw	0x0000
	db	0x00
	db	0x93
	db	0xcf
	db	0x00

gdt_userland_code:		; offset = 0x18
	dw	0xffff
	dw	0x0000
	db	0x00
	db	0x9e
	db	0xcf
	db	0x00

gdt_userland_data:		; offset = 0x20
	dw	0xffff
	dw	0x0000
	db	0x00
	db	0x92
	db	0xcf
	db	0x00

gdt_ptr:
	dw	$ - gdt - 1
	dd	gdt

GDT_CODESEG	equ gdt_kernel_code - gdt
GDT_DATASEG	equ gdt_kernel_data - gdt

; Set up the GDT and go into Protected Mode.
pm_enter:
	cli			; Disable BIOS interrupts.
	lgdt	[gdt_ptr]	; Load the GDT.
	mov	eax, cr0
	or	eax, 1		; Protected Mode Enable.
	mov	cr0, eax	; There's no going back now.
	jmp	GDT_CODESEG:pm_init

; We're in Protected Mode.
[bits 32]
pm_init:
	mov	ax, GDT_DATASEG
	mov	ds, ax
	mov	es, ax
	mov	fs, ax
	mov	gs, ax
	mov	ss, ax

	xor	eax, eax
	mov	ebx, eax
	mov	ecx, eax
	mov	edx, eax
	mov	esi, eax
	mov	edi, eax
	mov	ebp, kern_stack_bottom
	mov	esp, kern_stack_top

	; Hand control over to the C kernel. Godspeed! You Black Emperor.
	jmp	GDT_CODESEG:KERNOFF
	jmp	$

; Print a null-terminated string.
[bits 16]
puts:
	pusha
	mov	ah, 0x0e	; Tell BIOS to display a character.
loop:
	mov	al, [si]	; Load character.
	cmp	al, 0		; Check for \0.
	jne	putchar		; If it's not \0, print the character.
	popa			; We're done, pop everything.
	ret			; Return back to where we were.
putchar:
	int	0x10		; BIOS print interrupt.
	inc	si		; `str++`
	jmp	loop		; Go to the next character.

; String declarations.
str_diskerr:	db "Error loading disk.", 0x0a, 0x0d, 0x00
str_a20_fail:	db "The A20 Line is disabled", 0x0a, 0x0d, 0x00

; Hard disk.
BOOTDRV:	db 0x80

; Padding to 512 bytes. The last 2 bytes will come from the magic number.
times	510 - ($ - $$) db 0
; Magic number.
dw	0xaa55

section .bss
	align	4096
kern_stack_bottom:
	resb	STACKSIZE
kern_stack_top:
