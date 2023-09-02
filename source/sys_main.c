/** @file sys_main.c
 *   @brief Application main file
 *   @date 11-Dec-2018
 *   @version 04.07.01
 *
 *   This file contains an empty main function,
 *   which can be used for the application.
 */

/*
 * Copyright (C) 2009-2018 Texas Instruments Incorporated - www.ti.com
 *
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/* USER CODE BEGIN (0) */
/* USER CODE END */

/* Include Files */

#include "sys_common.h"

/* USER CODE BEGIN (1) */
/* USER CODE END */

/** @fn void main(void)
 *   @brief Application main function
 *   @note This function is empty by default.
 *
 *   This function is called after startup.
 *   The user can use this function to implement the application.
 */

/* USER CODE BEGIN (2) */
#include "gio.h"
#include "het.h"
#include "rti_delay.h"
#include "sci.h"
#include "stdio.h"
#include "string.h"
uint8_t DHT11_Init();
uint8_t DHT11_Read_Data(uint8_t *temph, uint8_t *templ, uint8_t *humi);
/* USER CODE END */

int main(void) {
  /* USER CODE BEGIN (3) */
  sciInit();
  rtiInit();
  gioInit();

  uint8_t th;
  uint8_t tl;
  uint8_t h;
  rtiStartCounter(rtiCOUNTER_BLOCK0);

  printf("run\r\n");
  while (1) {
    rtiDelay_ms(2000);
    gioSetDirection(hetPORT1, 0xFFFFFFFF);
    gioSetBit(hetPORT1, 17, 0);
    rtiDelay_ms(20);
    gioSetDirection(hetPORT1, 0x00000000);
    while (gioGetBit(hetPORT1, 17) != 0);
    rtiDelay_us(83);
    while (gioGetBit(hetPORT1, 17) != 1);
    rtiDelay_us(87);
    DHT11_Read_Data(&th, &tl, &h);
    printf("t:%d.%d, s:%d\n\r", th, tl, h);
  }

  /* USER CODE END */

  return 0;
}

/* USER CODE BEGIN (4) */

uint8_t DHT11_Read_Bit(void) {
  uint8_t retry = 0;
  while (gioGetBit(hetPORT1, 17) != 0);
  rtiDelay_us(15);
  while (!gioGetBit(hetPORT1, 17));
  rtiDelay_us(40);
  if (gioGetBit(hetPORT1, 17))
    return 1;
  else
    return 0;
}

uint8_t DHT11_Read_Byte(void) {
  uint8_t i, dat;
  dat = 0;
  for (i = 0; i < 8; i++) {
    dat <<= 1;
    dat |= DHT11_Read_Bit();
  }
  return dat;
}

uint8_t DHT11_Read_Data(uint8_t *temph, uint8_t *templ, uint8_t *humi) {
  uint8_t buf[5];
  uint8_t i;
  for (i = 0; i < 5; i++) {
    buf[i] = DHT11_Read_Byte();
  }
  if ((buf[0] + buf[1] + buf[2] + buf[3]) == buf[4]) {
    *humi = buf[0];
    *temph = buf[2];
    *templ = buf[3];
  }
  return 0;
}

int fputs(const char *_ptr, FILE *_fp) {
  unsigned int i, len;
  len = strlen(_ptr);
  for (i = 0; i < len; i++) {
    while ((scilinREG->FLR & (uint32)SCI_TX_INT) == 0U)
      ;
    scilinREG->TD = (unsigned char)_ptr[i];
  }
  return len;
}

int fputc(int ch, FILE *f) {
  while ((scilinREG->FLR & (uint32)SCI_TX_INT) == 0U)
    ; // 循环发送,直到发送完毕
  scilinREG->TD = (uint8)ch;
  return ch;
}
/* USER CODE END */
