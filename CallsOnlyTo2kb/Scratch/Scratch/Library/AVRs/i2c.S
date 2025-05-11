/*
 * i2c.S
 *
 * Created: 4/9/2025 4:56:55 PM
 *  Author: sunev
 */ 
 .global avr_i2c_init
 .global avr_i2c_start
 .global avr_i2c_stop
 .global avr_i2c_write

 .section .text

;=================================
; I2C INIT
.equ TWSR, 0xB9
.equ TWBR, 0xB8
.equ TWCR, 0xBC
.equ TWEN, 2

avr_i2c_init:
	ldi r16, 0x00 // Prescaler 1
	sts TWSR, r16

	ldi r16, 0x48 ; ((F_CPU / scl_freq) - 16) / 2
	sts TWBR, r16

	ldi r16, (1 << TWEN) // Enable TWI
	sts TWCR, r16

	ret

;=================================
; I2C START
.equ TWCR, 0xBC
.equ TWINT, 7
.equ TWSTA, 5
.equ TWEN, 2
.equ TWSR, 0xB9
.equ TW_START, 0x08
.equ TW_REP_START, 0x10
.equ TWDR, 0xBB
.equ TW_MT_SLA_ACK, 0x18
.equ TW_MR_SLA_ACK, 0x40

avr_i2c_start:
	ldi r16, (1 << TWINT) | (1 << TWSTA) | (1 << TWEN)
	sts TWCR, r16

	rcall avr_i2c_twint_wait

	lds r16, TWSR
	andi r16, 0xF8

	cpi r16, TW_START
	breq avr_i2c_start_continue

	cpi r16, TW_REP_START
	breq avr_i2c_start_continue

	rjmp avr_i2c_failure

avr_i2c_start_continue:
	sts TWDR, r24
	
	ldi r16, (1 << TWINT) | (1 << TWEN)
	sts TWCR, r16

	rcall avr_i2c_twint_wait 
	
	lds r16, TWSR
	andi r16, 0xF8

	cpi r16, TW_MT_SLA_ACK
	breq avr_i2c_success

	cpi r16, TW_MR_SLA_ACK
	breq avr_i2c_success

	; either jump or let it continue to failure
	rjmp avr_i2c_failure

;=================================
; I2C STOP
.equ TWCR, 0xBC
.equ TWINT, 7
.equ TWSTO, 4
.equ TWEN, 2

avr_i2c_stop:
	ldi r16, (1 << TWINT) | (1 << TWSTO) | (1 << TWEN)
	sts TWCR, r16

	rcall avr_i2c_twsto_wait_loop

	ret

;=================================
; I2C WRITE
.equ TWDR, 0xBB
.equ TWCR, 0xBC
.equ TWINT, 7
.equ TWEN, 2
.equ TWSR, 0xB9
.equ TW_MT_DATA_ACK, 0x28

avr_i2c_write:
	sts TWDR, r24 ; param

	ldi r16, (1 << TWINT) | (1 << TWEN)
	sts TWCR, r16

	rcall avr_i2c_twint_wait

	lds r16, TWSR
	andi r16, 0xF8
	cpi r16, TW_MT_DATA_ACK
	breq avr_i2c_success

	rjmp avr_i2c_failure

;=================================
; controllers

; Wait for TWINT to be set
avr_i2c_twint_wait:

avr_i2c_twint_wait_loop:
	lds r16, TWCR
	sbrs r16, TWINT
	rjmp avr_i2c_twint_wait_loop
	ret

; Wait for TWSTO to be cleared
avr_i2c_twsto_wait_loop:
	lds r16, TWCR
	sbrc r16, TWSTO
	rjmp avr_i2c_twsto_wait_loop
	ret

;=================================
; Success of Failures (Booleans)
avr_i2c_failure:
	clr r24
	ldi r24, 0
	ret

avr_i2c_success:
	clr r24
	ldi r24, 1
	ret