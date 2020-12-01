	.arch msp430g2553
	.p2align 2
	.text

	.data
	.extern freq
	.extern state2_status

	.text
	.global buzzer_advance
	
buzzer_advance:	
	cmp #0, &state2_status	;lower freq if state2_status 0, not 0 raise freq
	jz else

if:
	add #225, &freq		;raise freq
	jmp exit
else:
	sub #450, &freq		;lower freq

exit:
	mov &freq, r12
	call #get_period	;call get_period 2000000/r12

	call #buzzer_set_period	;call buzzer_set_period with result
	pop r0			;return
