;/*
; * i2c.inc
; *
; *  Created: 02.05.2018 0:53:55
; *   Author: zoosman
; */ 


;/********************************* I2C Routines *********************************/
;/* /////////////////////////// I2C Start Condition //////////////////////////// */
I2CM_START:
  sbi I2CPORT, SCL
  nop
  nop
  nop
  nop
  nop
  cbi I2CPORT, SDA
  nop
  nop
  nop
  nop
  cbi I2CPORT, SCL
  sbi I2CPORT, SDA
  in tmp, USISR
  sbr tmp, (1<<USISIF)
  out USISR, tmp
  ret

;/* /////////////////////////// I2C Stop Condition ///////////////////////////// */
I2CM_STOP:
  ldi tmp, 0x80
  out USIDR, tmp
  cbi I2CPORT, SDA
  sbi I2CPORT, SCL
  _I2CM_WAIT_SCK_HIGH:
    sbis I2CPIN, SCL
    rjmp _I2CM_WAIT_SCK_HIGH
  nop
  nop
  nop
  nop
  sbi I2CPORT, SDA
  nop
  nop
  nop
  nop
  in tmp, USISR
  sbr tmp, (1<<USIPF)
  out USISR, tmp
  ret

;/* /////////////////////////////// I2C Transfer /////////////////////////////// */
I2CM_TRANSFER:
  in tmp, USICR
  sbr tmp, (1<<USITC)
  _I2CM_TRANSFER_START:
    nop
    nop
    nop
    nop
    nop
    out USICR, tmp
    _I2CM_TRANSFER_WAIT_SCL_HIGH:
      sbis I2CPIN, SCL
      rjmp _I2CM_TRANSFER_WAIT_SCL_HIGH
    nop
    nop  
    nop
    nop
    out USICR, tmp
    sbis USISR, USIOIF  
    rjmp _I2CM_TRANSFER_START
  nop
  nop
  nop
  nop
  nop
  ret

;/* //////////////////////////// I2C Transfer Buffer /////////////////////////// */
I2CM_TRANSFER_BUFFER:
  cbi I2CPORT, SCL
  in tmp, USISR
  sbr tmp, (1<<USIOIF)
  cbr tmp, (0xe<<USICNT0)
  out USISR, tmp
  rcall I2CM_TRANSFER
  ret

;/* ////////////////////////// I2C Transfer ACK/NACK /////////////////////////// */
I2CM_RECEIVE_ACK:
  cbi I2CDDR, SDA ; Input
  in tmp, USISR
  sbr tmp, (1<<USIOIF)|(0xe<<USICNT0)
  out USISR, tmp
  clr tmp
  out USIDR, tmp
  rcall I2CM_TRANSFER
  sbi I2CDDR, SDA ; Output
  ret

;/* ////////////////////////// I2C Transfer ACK/NACK /////////////////////////// */
I2CM_SEND_ACK:
  sbi I2CDDR, SDA ; Output
  in tmp, USISR
  sbr tmp, (1<<USIOIF)|(0xe<<USICNT0)
  out USISR, tmp
  
  sbrc _EREG_, _I2CANF_
  rjmp _PREPARE_NACK
  rjmp _PREPARE_ACK

  _PREPARE_NACK:
    ser tmp
    rjmp _TRANSFER

  _PREPARE_ACK:
    clr tmp

  _TRANSFER:
    out USIDR, tmp
    rcall I2CM_TRANSFER
  ret

;/* ////////////////////////// I2C Send Slave Address ////////////////////////// */
I2CM_SEND_ADDRESS:
  lsl txByte
  sbrs _EREG_, _I2CRWF_
  rjmp _I2CM_SEND_ADDRESS
  sbr txByte, (1<<0) ; 0 - write byte, 1 - read byte

  _I2CM_SEND_ADDRESS: 
    out USIDR, txByte	
    rcall I2CM_TRANSFER_BUFFER
    rcall I2CM_RECEIVE_ACK
    sbis USIDR, 0
    rjmp _I2CM_SEND_ADDRESS_EXIT
    sbr _EREG_, (1<<_I2CERF_)
    rcall I2CM_STOP
  _I2CM_SEND_ADDRESS_EXIT:
    ret

;/* ///////////////////////// I2C Send Packet to Slave ///////////////////////// */
I2CM_SEND_PACKET:
  out USIDR, txByte
  rcall I2CM_TRANSFER_BUFFER
  rcall I2CM_RECEIVE_ACK
  sbis USIDR, 0
  rjmp _I2CM_SEND_PACKET_EXIT
  sbr _EREG_, (1<<_I2CERF_)
  rcall I2CM_STOP
  _I2CM_SEND_PACKET_EXIT:
    ret

;/* /////////////////////// I2C Receive Packet from Slave ////////////////////// */
I2CM_RECEIVE_PACKET:
  cbi I2CDDR, SDA ; Input
  rcall I2CM_TRANSFER_BUFFER
  in rxByte, USIDR
  rcall I2CM_SEND_ACK
  ret
