/*
 * uart_interface.h
 *
 *  Created on: Nov 18th, 2023
 *      Author: Das, Somen
 *
 * @brief: wrapps HAL Uart API's for GSM com usage
 */
#pragma once
#include "stm32g0xx_hal.h"
#include <string.h>
#include <stdio.h>

#define LORA_UART_ROUND_BUFF_MAXLEN 255

/* Global External Variables */


/******************************************************************************
 * Global Function Prototypes
 *****************************************************************************/

void uart_push_data(uint8_t data);

/**
 * @brief initialize uart driver
 */
void lora_init_uart(void);

/**
 * @brief deinitialize uart driver
 */
void lora_deinit_uart(void);

/**
 * @brief uart_send_frame
 * @note Have to rewrite this
 */
void lora_uart_send_frame(const char *send_command);

/**
 * @brief Available data from UART rx Buffer
*/
uint16_t lora_uart_data_available(void);

/**
 * @brief Read data from UART rx Buffer
*/
uint8_t lora_uart_data_read(void);

/**
 * @brief Flush UART rx Buffer
*/
void lora_uart_data_flush();
