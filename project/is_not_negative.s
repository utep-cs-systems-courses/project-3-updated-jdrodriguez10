	.text
	.global isNotNegative
isNotNegative:
	cmp #0, r12
	jge NotNegative

Negative:
	mov #0, r12
	jmp exit

NotNegative:
	mov #1, r12
exit:
	pop r0
