;/*
; * init.inc
; *
; *  Created: 02.05.2018 0:42:48
; *   Author: zoosman
; */ 

;/********************** START AFTER POWER UP OR RESET FROM **********************/
_RESET:
  clr R1
  out SREG, R1
  ldi tmp, low(RAMEND)
  out SPL, tmp

;/*********************************** INIT LEDs **********************************/
INIT_LED:
  sbi LEDDDR, LED0PIN

;/******************** INIT EVENT AND MEASURE REGISTRY FLAGS  ********************/
INIT_FLAGS:
  clr _EREG_
  sbr _EREG_, (1<<_MIF_)

;/******************************** INIT VARIABLES ********************************/
INIT_VARIABLES:
  set_tcnt

;/************************************* Timer ************************************/
rcall INIT_TIMER

cli
;/********************************* Reduce Power *********************************/
INITPOWERMODE:
  in tmp, PRR
  sbr tmp, (1<<PRTIM1)|(1<<PRADC)
  out PRR, tmp
  in tmp, ACSR
  sbr tmp, (1<<ACD)
  out ACSR, tmp

;/******************************** WatchDog Timer ********************************/
INITWATCHDOG:
  in tmp, WDTCR
  sbr tmp, (1<<WDCE)|(1<<WDE)
  out WDTCR, tmp
  sbr tmp, (1<<WDP3)|(1<<WDP0)|(1<<WDIE) ; 8s
  out WDTCR, tmp

;/*********************************** I2C INIT ***********************************/
INIT_I2C_MASTER:
  sbi I2CPORT, SDA
  sbi I2CPORT, SCL
  sbi I2CDDR, SDA
  sbi I2CDDR, SCL
  ser tmp
  out USIDR, tmp
  clr tmp
  sbr tmp, (1<<USIWM1)|(1<<USICS1)|(1<<USICLK)
  out USICR, tmp
  clr tmp
  sbr tmp, (1<<USISIF)|(1<<USIOIF)|(1<<USIPF)|(1<<USIDC)
  out USISR, tmp

sei

;/********************************** BMx280 INIT *********************************/
rcall INIT_BMx280
