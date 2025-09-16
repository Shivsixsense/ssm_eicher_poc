/*
 * system_logging.h
 *
 *  Created on: Apr 4, 2023
 *      Author: TaiNguyen
 */

#ifndef INC_SYSTEM_LOGGING_H_
#define INC_SYSTEM_LOGGING_H_
#include"main.h"
#define LOG_LEVEL 0

void init_logging(UART_HandleTypeDef *huart);
void logging_uart(const char *logMessage);
void print_log(const char *fmt,...);
void logging_uartln(const char *logMessage);
void print_logln(const char *fmt,...);

#endif /* INC_SYSTEM_LOGGING_H_ */
