;/*
; * macroses.inc
; *
; *  Created: 02.05.2018 0:40:46
; *   Author: zoosman
; */ 

;/******************************* Long Time Counter ******************************/
.MACRO set_tcnt
  ldi tcntH, 0x00
  ldi tcntL, 0x7f
.ENDMACRO

;/************************************* Delay ************************************/
.MACRO DELAY_MACRO
  ldi tmp, @0
  mov dClock, tmp
  ldi tmp, @1
  mov dRate, tmp
  rcall DELAY
.ENDMACRO
