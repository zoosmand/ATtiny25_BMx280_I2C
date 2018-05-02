;/*
; * bmx280.inc
; *
; *  Created: 02.05.2018 0:50:17
; *   Author: zoosman
; */ 

;/******************************** BMx280 macroses *******************************/
.MACRO BMx280_READ_MACRO
  ldi YH, high(@0)
  ldi YL, low(@0)
  ldi txByte, @1
  ldi tmp, @2
  mov R0, tmp
  rcall BMx280_READ
.ENDMACRO

.MACRO BMx280_READ_ONE_REG_MACRO
  ldi txByte, @0
  rcall BMx280_READ_ONE_REG
.ENDMACRO

.MACRO BMx280_Write_MACRO
  ldi tmp, @0
  mov R2, tmp
  ldi tmp, @1
  mov R3, tmp
  rcall BMx280_WRITE
.ENDMACRO


;/******************************** BMx280 routines *******************************/
INIT_BMx280:
  cbr _EREG_, (1<<_I2CERF_) ; clear I2C error flag
  ldi ZH, high(2*BMx280_addr)
  ldi ZL, low(2*BMx280_addr)
  lpm BMxAddr, Z
  BMx280_READ_ONE_REG_MACRO 0xd0 ; Read BMx280 ID, BMP280 - 0x58, BME280 - 0x60
  cpi rxByte, 0x60
  brne _INIT_BMx280_ID_FAILED
  ; Datasheet 5.4 Register description
  BMx280_WRITE_MACRO 0x01, 0xf2 ; f2 - ctrl_hum register, osrh_t 001 - oversampling x1, comment this string on BMP280
  BMx280_WRITE_MACRO 0x25, 0xf4 ; f4 - ctrl_meas register, osrs_t 001 - oversampling x1, osrs_p 001 - oversampling x1, mode 01 - Forced Mode. Datasheet 3.6 Power modes
  BMx280_READ_MACRO BMx280_calib1, 0x88, 0x1a ; Read calibration registers calib00-calib25
  BMx280_READ_MACRO BMx280_calib2, 0xe1, 0x10 ; Read calibration registers calib26-calib41, comment this string on BMP280
  rjmp _INIT_BMx280_EXIT

  _INIT_BMx280_ID_FAILED:
    sbr _EREG_, (1<<_I2CERF_)
  _INIT_BMx280_EXIT:
    ret



BMx280_READ_ONE_REG:
  sbrc _EREG_, _I2CERF_
  ret
  push txByte
  rcall I2CM_START
  mov txByte, BMxAddr
  rcall I2CM_SEND_ADDRESS
  pop txByte
  rcall I2CM_SEND_PACKET
  rcall I2CM_STOP

  rcall I2CM_START
  sbr _EREG_, (1<<_I2CRWF_)
  mov txByte, BMxAddr
  rcall I2CM_SEND_ADDRESS
  sbr _EREG_, (1<<_I2CANF_)
  rcall I2CM_RECEIVE_PACKET
  rcall I2CM_STOP

  cbr _EREG_, (1<<_I2CRWF_)|(1<<_I2CANF_)
  ret



BMx280_READ:
  sbrc _EREG_, _I2CERF_
  ret
  push txByte
  mov R0, tmp
  ldi tmp, 0x01
  mov R1, tmp

  rcall I2CM_START
  mov txByte, BMxAddr
  rcall I2CM_SEND_ADDRESS
  pop txByte
  rcall I2CM_SEND_PACKET
  rcall I2CM_STOP

  rcall I2CM_START
  sbr _EREG_, (1<<_I2CRWF_)
  mov txByte, BMxAddr
  rcall I2CM_SEND_ADDRESS

  _READ_DATA:
    cp R0, R1
    brne _READ_DATA_PACKET
    sbr _EREG_, (1<<_I2CANF_)
    _READ_DATA_PACKET:
      rcall I2CM_RECEIVE_PACKET   
      st Y+, rxByte 
    dec R0
    brne _READ_DATA
  rcall I2CM_STOP

  cbr _EREG_, (1<<_I2CRWF_)|(1<<_I2CANF_)
  ret



BMx280_WRITE:
  sbrc _EREG_, _I2CERF_
  ret
  push R2
  push R3 
  rcall I2CM_START
  mov txByte, BMxAddr
  rcall I2CM_SEND_ADDRESS
  pop txByte
  rcall I2CM_SEND_PACKET
  pop txByte
  rcall I2CM_SEND_PACKET
  rcall I2CM_STOP
  ret

