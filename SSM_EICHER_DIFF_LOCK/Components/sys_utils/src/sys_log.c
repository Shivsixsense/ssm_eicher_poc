
#include "sys_log.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#if LOG_LEVEL > 0
char pMess[LOG_BUFFER_LEN];
static UART_HandleTypeDef *huart_log;
#endif

void init_logging(UART_HandleTypeDef *huart)
{
#if LOG_LEVEL > 0
    huart_log = huart;
#endif
}

void logging_uart(const char *logMessage)
{
#if LOG_LEVEL > 0
    int messLength = 0;
    memset(pMess, 0, LOG_BUFFER_LEN);
    messLength = sprintf(pMess, logMessage);
    if (messLength > 0)
    {
        HAL_UART_Transmit(huart_log, (uint8_t *)pMess, messLength, 10);
    }
#endif
}

void print_log(const char *fmt, ...)
{
#if LOG_LEVEL > 0
    char msg[LOG_BUFFER_LEN];
    memset(msg, 0, LOG_BUFFER_LEN);
    va_list args;
    va_start(args, fmt);
    vsprintf(msg, fmt, args);
    va_end(args);
    logging_uart(msg);
#endif
}

void logging_uartln(const char *logMessage)
{
#if LOG_LEVEL > 0
    int messLength = 0;
memset(pMess,0,64);
    messLength = sprintf(pMess, logMessage);
    if (messLength > 0)
    {
        strcat(pMess, "\r\n");
       // HAL_UART_Transmit(huart_log, (uint8_t *)pMess, messLength + 2, 10);
   	    CDC_Transmit_FS((uint8_t *)pMess,messLength);
    }
#endif

}

void print_logln(const char *fmt, ...)
{
#if LOG_LEVEL > 0
    char msg[LOG_BUFFER_LEN];
    memset(msg, 0, LOG_BUFFER_LEN);
    va_list args;
    va_start(args, fmt);
    vsprintf(msg, fmt, args);
    va_end(args);
    logging_uartln(msg);
#endif
}


