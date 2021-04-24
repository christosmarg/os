call	a20_test
call	a20_enable

; Test to see if the A20 line is enabled.
; As a reference we will use the magic number 0xaa55 since we know it's always
; going to have the same value and be located at address 0x7dfe (0x7c00 + 510).
; If the return code in `ax` is 1, the A20 is enabled, and if it's 0 it's disabled.
a20_test:
	pusha
	
	mov	ax, [0x7dfe]	; 0x7c00 + 510. The magic number is there.
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
	mov	dx, [es:bx]	; If the A20 line is disabled we get 0xaa55.

	cmp	ax, dx
	je	cont		; If they're equal, the A20 line might be disabled.
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
	jnz	a20_waitc		; If it's 1, it's busy.
	ret
; Wait for the keyboard controller to send data back.
a20_waitd:
	in	al, 0x64	; Read from port 0x64 again.
	test	al, 0x01	; Test the first bit.
	jz	a20_waitd		; If it's 1, the data is not ready to be sent.
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


str_a20_fail:	db "The A20 Line is disabled", 0x0a, 0x0d, 0x00
