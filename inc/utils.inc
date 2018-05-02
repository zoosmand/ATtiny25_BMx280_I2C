;/*
; * utils.inc
; *
; *  Created: 02.05.2018 0:47:03
; *   Author: zoosman
; */ 


;/*********************************** LED0 BLINK *********************************/
LED0_BLINK:
  sbis LEDPIN, LED0PIN
  rjmp _LED0PIN_H
  cbi LEDPORT, LED0PIN
  ret
  _LED0PIN_H:
    sbi LEDPORT, LED0PIN
    ret

;/************************************* Timer ************************************/
;/*     (1/8000000)*1024*256 = 32.768ms      */
INIT_TIMER:
  cli
  in tmp, TCCR0B
  sbr tmp, (1<<CS02)|(1<<CS00) ; 1024
  out TCCR0B, tmp
  in tmp, TIMSK
  sbr tmp, (1<<TOIE0)
  out TIMSK, tmp
  clr tmp ; 256
  out TCNT0, tmp
  reti

;/********************************** Clear Timer *********************************/
CLEAR_TIMER:
  cli
  clr tmp
  out TCCR0B, tmp
  out TIMSK, tmp
  reti

;/************************************* Delay ************************************/
DELAY:
  cli
  out TCCR0B, dClock
  clr tmp
  out TCNT0, tmp
  _DELAY_:
    in tmp, TCNT0
    cp tmp, dRate
    brne _DELAY_
  sei
  ret
