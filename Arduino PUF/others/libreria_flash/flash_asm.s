.global	flash_read_byte

; r25:r24	(in)	page address
; r24 		(out)	return value
flash_read_byte:
    mov ZH, r25
    mov ZL, r24
    clr r24
    lpm	r24, Z
    ret
