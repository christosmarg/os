; call in pm_mode
	call	lm_check

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
	jmp	$		; Safety hang.

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

; Activate Long Mode and Paging using the EFER.
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

