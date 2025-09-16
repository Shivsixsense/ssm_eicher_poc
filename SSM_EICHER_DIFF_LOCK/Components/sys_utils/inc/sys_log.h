
#ifndef _SYS_LOG__H_
#define _SYS_LOG__H_

#include "main.h"

#define LOG_LEVEL 1
#define LOG_BUFFER_LEN 64

void init_logging(UART_HandleTypeDef *huart);
void logging_uart(const char *logMessage);
void print_log(const char *fmt, ...);
void logging_uartln(const char *logMessage);
void print_logln(const char *fmt, ...);
//void Print_logln(uint8_t *send_command);

#endif
