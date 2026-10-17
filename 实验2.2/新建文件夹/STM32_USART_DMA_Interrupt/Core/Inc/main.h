#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f1xx_hal.h"

extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_tx;

void Error_Handler(void);

#endif
