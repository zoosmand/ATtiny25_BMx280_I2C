;/*
; * vectors.inc
; *
; *  Created: 02.05.2018 0:35:57
; *   Author: zoosman
; */ 


rjmp _RESET        ; Reset
rjmp _INT0         ; IRQ0
rjmp _PCINT0       ; PCINT0
rjmp _TIMER1_COMPA ; Timer1 CompareA
rjmp _TIMER1_OVF   ; Timer1 Overflow
rjmp _TIMER0_OVF   ; Timer0 Overflow
rjmp _EE_RDY       ; EEPROM Ready
rjmp _ANA_COMP     ; Analog Comparator
rjmp _ADC          ; ADC Conversion Complete
rjmp _TIMER1_COMPB ; Timer1 CompareB
rjmp _TIMER0_COMPA ; Timer0 CompareA
rjmp _TIMER0_COMPB ; Timer0 CompareB
rjmp _WDT          ; Watchdog Timeout
rjmp _USI_START    ; USI Start
rjmp _USI_OVF      ; USI Overflow


;/*********************************** Interrupts **********************************/
_INT0:
_PCINT0:
_TIMER1_COMPA:
_TIMER1_OVF:
;_TIMER0_OVF:
_EE_RDY:
_ANA_COMP:
_ADC:
_TIMER1_COMPB:
_TIMER0_COMPA:
_TIMER0_COMPB:
;_WDT:
_USI_START:
_USI_OVF:
  rjmp 0
