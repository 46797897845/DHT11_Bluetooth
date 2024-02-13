#ifndef __SERIAL_H
#define __SERIAL_H

#include "stm32f10x.h" 
#include "stdio.h"
#include "stdarg.h"

#define RX_GPIO_Pin					GPIO_Pin_10
#define TX_GPIO_Pin					GPIO_Pin_9
#define RX_TX_GPIO 					GPIOA

extern uint8_t Serial_TxPacket[];
extern uint8_t Serial_RxPacket[];

void Serial_SendByte(uint8_t Byte);
void Serial_Init(void);
void Serial_SendArray(uint8_t *Array, uint16_t Length);
void Serial_SendString(char *String);
void Serial_SendNumber(uint32_t Number, uint8_t Length);
void Serial_Printf(char *format, ...);

void Serial_SendPacket(void);

uint8_t Serial_GetRxFlag(void);

#endif
