
/*
 * isr.s
 *
 * Created: 10/04/2025 09.04.15
 *  Author: quist
 */ 
.global timer1_ctc_ocrnx
.global __vector_11

.section .vectors, "ax", @progbits
.org 0x0016        ; = 0x0000 + (11 * 2)
rjmp __vector_11

.global interrupted
.section .data
interrupted:
	.byte 1

.section .text

.equ TCCR1B, 0x81
.equ WGM12, 3
.equ CS11, 2
.equ CS10, 0
.equ OCR1AL, 0x88
.equ OCR1AH, 0x89
.equ TIMSK1, 0x6F
.equ OCIE1A, 1

timer1_ctc_ocrnx:
	; Set CTC mode
	lds r16, TCCR1B 
	; Set b011 for 64 prescaler (CTC) load immedaite
	ori r16, (1 << WGM12) |  (1 << CS11) | (1 << CS10)
	sts TCCR1B, r16
	
	; Set 100ms
	ldi r16, lo8(24999)
	ldi r17, hi8(24999)
	sts OCR1AL, r16
	sts OCR1AH, r17

	lds r16, TIMSK1
	ori r16, (1 << OCIE1A)
	sts TIMSK1, r16

	; Set global interrupts
	sei
	
	ret

__vector_11:
	ldi r16, 1
	sts interrupted, r16
	reti