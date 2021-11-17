.include "m328Pdef.inc"


.org 0x000
rjmp setup
rjmp int0_interrupt
rjmp int1_interrupt

.org INT_VECTORS_SIZE
setup:	clr R16
	ori R16, 0b00000101 
	sts EICRA, R16    ; set interrupt sense control to "rising edge"
	clr R16

	ldi R16, LOW(RAMEND) ; set up stack pointer
    out SPL, R16
    ldi R16, HIGH(RAMEND)
    out SPH, R16

	ori R16, 0b10000000
	out SREG, R16     ; enable global interrupt flag
	in R17, SREG
	clr R16

	ori R16, 0b00000011 
	out EIMSK, R16    ; enable esternal interrupt INT0
	clr R16


	ori R16, 0b00000001 
	out	DDRB, R16     ; set pin 14 as output
	clr R16

mainloop:	rjmp mainloop

int0_interrupt: cbi PORTB, 0
	reti
int1_interrupt: sbi PORTB, 0
	reti



