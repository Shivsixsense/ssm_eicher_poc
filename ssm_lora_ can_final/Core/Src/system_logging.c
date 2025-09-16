/*
 * system_logging.c
 *
 *  Created on: Apr 4, 2023
 *      Author: TaiNguyen
 */

#include "system_logging.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#if LOG_LEVEL > 0
static UART_HandleTypeDef *huart_log;
char pMess[2024];
#endif

void init_logging(UART_HandleTypeDef *huart){
#if LOG_LEVEL > 0
	huart_log = huart;
#endif
}

void logging_uart(const char *logMessage){
#if LOG_LEVEL > 0
	int messLength = 0;

	messLength = sprintf(pMess,logMessage);
	if(messLength > 0){
		HAL_UART_Transmit(huart_log,(uint8_t *)pMess,messLength,10);
	}
#endif
}

void print_log(const char *fmt,...){
#if LOG_LEVEL > 0
	char msg[2000];
	va_list args;
	va_start(args, fmt);
	vsprintf(msg, fmt, args);
	va_end(args);
	logging_uart(msg);
	memset(msg,0,2000);
	vTaskDelay(10);
#endif
}

void logging_uartln(const char *logMessage){
#if LOG_LEVEL > 0
	int messLength = 0;

	messLength = sprintf(pMess,logMessage);
	if(messLength > 0){
		strcat(pMess,"\r\n");
		HAL_UART_Transmit(huart_log,(uint8_t *)pMess,messLength+2,10);
	}
#endif
}

void print_logln(const char *fmt,...){
#if LOG_LEVEL > 0
	char msg[2000];
	va_list args;
	va_start(args, fmt);
	vsprintf(msg, fmt, args);
	va_end(args);
	logging_uartln(msg);
	memset(msg,0,2000);
	vTaskDelay(10);
#endif
}

