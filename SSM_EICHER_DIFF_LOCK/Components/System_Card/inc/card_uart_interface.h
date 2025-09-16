/*
 * uart_interface.h
 *
 *  Created on: Nov 18th, 2023
 *      Author: Das, Somen
 *
 * @brief: wrapps HAL Uart API's for GSM com usage
 */
#pragma once
#include "system_types.h"
#include "stm32g0xx_hal.h"

#define CARD_UART_ROUND_BUFF_MAXLEN 512

/* Global External Variables */
// extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

#define ACTIVE_UART_HANDLE huart2
#define pACTIVE_UART_HANDLE &huart2
#define ACITIVE_UART_DRIVER_INST USART2

/******************************************************************************
 * Global Function Prototypes
 *****************************************************************************/

void card_buff_rx_data(uint8_t data);


/**
 * @brief initialize uart driver
 */
void init_uart(void);

/**
 * @brief deinitialize uart driver
 */
void deinit_uart(void);

/**
 * @brief uart_send_frame
 * @note Have to rewrite this
 */
void uart_send_frame(uint8_t *send_command);

/**
 * @brief uart_send_frame
 * @note Have to rewrite this
 */
int uart_write_data(uint8_t *pData, uint16_t len);

/**
 * @brief Available data from UART rx Buffer
 */
uint16_t uart_data_available(void);

/**
 * @brief Read data from UART rx Buffer
 */
uint8_t uart_data_read(void);

/**
 * @brief Read data from UART rx Buffer
 */
uint16_t uart_data_read_buffer(uint8_t *dataBuff, uint16_t len);

/**
 * @brief Flush UART rx Buffer
 */
void uart_data_flush();
