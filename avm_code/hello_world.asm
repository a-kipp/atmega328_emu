	.include "m328Pdef.inc"


start:ldi r16,'h'
    sts	UDR0, r16
    ldi	r16,'e'
    sts	UDR0, r16
    ldi	r16,'l'
    sts	UDR0, r16
    ldi	r16,'l'
    sts	UDR0, r16
    ldi	r16,'o'
    sts	UDR0, r16
    ldi	r16, 0x20
    sts	UDR0, r16
    ldi	r16, 0x20
    sts	UDR0, r16
    ldi	r16,'w'
    sts	UDR0, r16
    ldi	r16,'o'
    sts	UDR0, r16
    ldi	r16,'r'
    sts	UDR0, r16
    ldi	r16,'l'
    sts	UDR0, r16
    ldi	r16,'d'
    sts	UDR0, r16
    ldi	r16, 0x0A
    sts	UDR0, r16
rjmp start

