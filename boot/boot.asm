; Load code to 0x7c00 (Boot Sector address).
[org 0x7c00]
[bits 16]

KERNOFF	equ 0x1000

section .text
global _start

; Entry point.
_start:
	cli			; Disable interrupts.
	xor	ax, ax		; Clear segment registers.
	mov	ds, ax
	mov	es, ax
	mov	fs, ax
	mov	gs, ax
	mov	ss, ax
	mov	bp, 0x9000	; Set the base and stack pointers.
	mov	sp, bp
	cld			; Read strings from low to high.
	sti			; Enable interrupts back.

	mov	[BOOTDRV], dl	; Set boot drive.
	call	kernel_load
	jmp	pm_enter
	jmp	$		; Safety hang.

; Load kernel from disk to memory.
kernel_load:
	pusha
	mov	ah, 0x42
	mov	dl, [BOOTDRV]
	mov	si, disk_packet
	int	0x13

	jc	diskerr		; There's an error. Too bad.
	popa
	ret
diskerr:
	mov	si, str_diskerr
	call	puts
	jmp	$		; There's nothing we can do at this point.

disk_packet:
	.size	db 0x10
	.zero	db 0x00
	.count	dw 0x0f
	.off16	dw KERNOFF
	.seg16	dw 0x0000
	.lba	dq 1

[bits 16]
; Set up the GDT.
gdt:
gdt_null:
	dd	0x0
	dd	0x0

gdt_kernel_code:
	dw	0xffff
	dw	0x0
	db	0x0
	db	10011010b
	db	11001111b
	db	0x0

gdt_kernel_data:
	dw	0xffff
	dw	0x0
	db	0x0
	db	10010010b
	db	11001111b
	db	0x0

gdt_userland_code:
	dw	0xffff
	dw	0x0
	db	0x0
	db	11111010b
	db	11001111b
	db	0x0

gdt_userland_data:
	dw	0xffff
	dw	0x0
	db	0x0
	db	11110010b
	db	11001111b
	db	0x0

gdt_ptr:
	dw	$ - gdt - 1
	dd	gdt

GDT_CODESEG	equ gdt_kernel_code - gdt
GDT_DATASEG	equ gdt_kernel_data - gdt

; Go into Protected Mode and set PAE Paging and the GDT.
pm_enter:
	cli			; Disable BIOS interrupts.
	lgdt	[gdt_ptr]	; Load the GDT.

	mov	eax, cr0
	or	eax, 1 << 0	; Protected Mode Enable.
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

	mov	ebp, 0x90000
	mov	esp, ebp

	call	lm_check
	call	kernel_exec	; If we cannot get into Long Mode, start in PE.
	jmp	$

; Check for Long Mode.
lm_check:
	pusha
	pushfd			; Push EFLAGS register to the stack.
	pop	eax		; Pop EFLAGS registers to `eax`.
	mov	ecx, eax	; Keep a backup in `ecx`.
	xor	eax, 1 << 21	; Flip the 21st bit if the it's not 1.
	push	eax
	popfd			; Pop `eax` to EFLAGS.

	pushfd
	pop	eax		; Copy EFLAGS back to `eax`.
	push	ecx
	popfd			; Restore the flipped version.
	xor	eax, ecx
	jz	lm_fail		; If EFLAGS' value hasn't changed, we have CPUID.

	mov	eax, 0x80000000	; CPUID argument.
	cpuid			; CPU identification. `eax` is now populated with info.
	cmp	eax, 0x80000001	; If it's less than 0x80000000 we cannot have Long Mode.
	jb	lm_fail

	mov	eax, 0x80000001	; Get extended processor information.
	cpuid
	test	edx, 1 << 29
	jz	lm_fail		; If the 29th bit is 0, we cannot have Long Mode.

	popa
	ret

; No long mode.
lm_fail:
	popa
	call	kernel_exec

; We'll check for (and switch to) Long Mode here.
lm_enter:
	cli
	mov	eax, cr0
	or	eax, 1 << 31	; Paging Enable.
	mov	cr0, eax
; Set up PAE paging.
	mov	edi, 0x1000	; Page table starts 4KiB in memory.
	mov	cr3, edi	; Hold the location of the highest page table.
	xor	eax, eax	; Clear memory space.
	mov	ecx, 4096
	rep	stosd		; Write 4096 dwords with the value 0.	
	mov	edi, 0x1000	; Go to the initial location.

; Page table locations:
;	Page Map Level 4 Table (PML4T):		0x1000
;	Page Directory Pointer Table (PDPT):	0x2000
;	Page Directory Table (PDT):		0x3000
;	Page Table (PT);			0x4000
;
; In order for each table to point to each other we'll use a size directive.
; Since tables are 4KiB away from each other we'll move dwords.
	mov	dword [edi], 0x2003	; PML4T -> PDPT
	add	edi, 0x1000		; Move 4KiB forward.
	mov	dword [edi], 0x3003	; PDPT -> PDT
	add	edi, 0x1000
	mov	dword [edi], 0x4003	; PDT -> PT
	add	edi, 0x1000

	mov	dword ebx, 3	; Map beginning of code.
	mov	ecx, 512	; Do it 512 times.
setentry:
	mov	dword [edi], ebx
	add	ebx, 0x1000
	add	edi, 8		; Go to the next entry in the page table.
	loop	setentry

; Enable the 6th bit in CR4 (Physical Address Extension Bit) to tell
; the CPU that we're using PAE paging.
	mov	eax, cr4
	or	eax, 1 << 5
	mov	cr4, eax

; Activate Protected Mode and Paging using the EFER.
	mov	ecx, 0xc0000080
	rdmsr			; Copy the contents of EFER to `eax`. 
	or	eax, 1 << 8	; Set LME bit to enable Long Mode.
	wrmsr			; Write `eax` back to EFER.	
	
	lgdt	[gdt_ptr]		; Load the GDT.
	jmp	GDT_CODESEG:lm_init	; Switch to Long Mode.

; We're in Long Mode now.
[bits 64]
lm_init:
	mov	ax, GDT_DATASEG
	mov	ds, ax
	mov	ss, ax
	mov	es, ax
	mov	fs, ax
	mov	gs, ax

	mov	rbp, 0x90000
	mov	rsp, rbp
	call	kernel_exec
	jmp	$	

; Hand control over to the C kernel. Godspeed.
kernel_exec:
	call	KERNOFF
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
BOOTDRV:	db 0x80

; Padding to 512 bytes. The last 2 bytes will come from the magic number.
times	510 - ($ - $$) db 0
; Magic number.
dw	0xaa55
