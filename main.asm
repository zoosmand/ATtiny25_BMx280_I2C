;
; ATtiny25_AS__I2C_Master_BMx280_.asm
;
; Created: 21.06.2017 18:40:16
; Author : setup
;

.include "tn25def.inc"

.def BMxAddr  = R10
.def dClock   = R12
.def dRate    = R13
.def cntHL    = R14
.def cntd     = R15
.def tmp      = R16
.def _EREG_   = R17
.def txByte   = R18
.def rxByte   = R19
.def tcntL    = R20
.def tcntH    = R21
.def tmpL     = R22
.def tmpH     = R23
.def cntLL    = R24
.def cntLH    = R25

.equ LEDDDR   = DDRB
.equ LEDPORT  = PORTB
.equ LEDPIN   = PINB
.equ LED0PIN  = PB3

.equ I2CDDR   = DDRB
.equ I2CPORT  = PORTB
.equ I2CPIN   = PINB
.equ SDA      = PB0
.equ SCL      = PB2

;/********************************* Event REGistry *******************************/
.equ _MIF_    = 0 ; Timer Measure Interval Flag
.equ _I2CRWF_ = 1 ; I2C Read/Write Flag, 0 - write (m->s), 1 - read (m<-s)
.equ _I2CANF_ = 2 ; I2C Ack/Nack Flag, 0 - ACK, 1 - NACK
.equ _I2CERF_ = 3 ; I2C ERror Flag

.cseg
.org 0


.include "./inc/vectors.inc"
.include "./inc/macroses.inc"
.include "./inc/init.inc"


;/*********************************** MAIN LOOP **********************************/
MAIN:
  sbrs _EREG_, _MIF_
  rjmp _GO_TO_SLEEP
  cbr _EREG_, (1<<_MIF_)
  rcall MAKE_MEASURE

  _RESTORE_TIMER:
    rcall CLEAR_TIMER
    rcall INIT_TIMER

  _GO_TO_SLEEP:
    in tmp, MCUCR
    sbr tmp, (1<<SE)
    out MCUCR, tmp
    sei
    sleep

  rjmp MAIN
  rjmp THE_END


.include "./inc/i2c.inc"
.include "./inc/bmx280.inc"
.include "./inc/interrupts.inc"
.include "./inc/utils.inc"



;/*********************************** MEASURMENT *********************************/
MAKE_MEASURE:
  sbrc _EREG_, _I2CERF_
  rcall INIT_BMx280
  BMx280_WRITE_MACRO 0x25, 0xf4 ; Datasheet 3.3.3 Forced mode. For a next measurement, forced mode needs to be selected again.
  BMx280_READ_MACRO BMx280_data, 0xf7, 0x08 ; Read data registers, count 0x08 for BME280 or 0x06 for BMP280
  ret


;/************************************ THE END ***********************************/
THE_END:
  cli
  rjmp 0

.include "./inc/var.inc"
