do_spm:
	; check previous SPM complete
wait_spm:
	in		r16, SPMCSR
	sbrc	r16, SELFPREGEN
	rjmp	wait_spm

	; save status register
	in		r17, SREG
	; disable interrupts
	cli

	; r20 determines the SPM action
	out		SPMCSR, r20
	spm

	; restore status register
	out		SREG, r17
	ret