write_page:
	; page erase
	ldi		r20, (1 << PGERS) | (1 << SELFPRGEN)
	rcall	do_spm

	; re-enable RWW section to clear the temp buffer
	ldi		r20, (1 << RWWSRE) | (1 << SELFPRGEN)
	rcall	do_spm

	; transfer data from RAM to temp buffer
	ldi		r24, low(PAGESIZEB)
	ldi		r25, high(PAGESIZEB)

wrloop:
	; load data from RAM (Y pointer)
	ld		r0,	Y+
	ld		r1, Y+

	; load data to temp buffer
	ldi		r20, (1 << SELFPRGEN)
	rcall	do_spm

	; increment Z (PCWORD counter)
	adiw	ZH:ZL, 2

	; decrement loop counter
	sbiw	r25:r24, 2
	brne	wrloop

	;restore pointer
	subi	ZL, low(PAGESIZEB)         
	sbci	ZH, high(PAGESIZEB)

	; execute page write
	ldi		r20, (1<<PGWRT) | (1<<SELFPRGEN)
	rcall	do_spm

	; re-enable RWW section
	ldi		r20, (1 << RWWSRE) | (1 << SELFPRGEN)
	rcall	do_spm

	; read back and check
	; init loop counter
	ldi		r24, low(PAGESIZEB)
	ldi		r25, high(PAGESIZEB)

	; restore temp buffer pointer
	subi	YL, low(PAGESIZEB)
	sbci	YH, high(PAGESIZEB)

rdloop:
	lpm		r0, Z+
	ld		r1,	Y+
	cpse	r0, r1
	rjmp	error

	; decrement loop counter
	sbiw	r25:r24, 1
	brne	rdloop

	; return to RWW section
	; verify that RWW section is safe to read
return:
	in		r16, SPMCSR

	; if RWWSB is set then the RWW is not ready yet
	sbrs	r16, RWWSB

	; re-enable RWW section
	ldi		r20, (1 << RWWSRE) | (1 << SELFPRGEN)
	rcall	do_spm
	rjmp	return

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