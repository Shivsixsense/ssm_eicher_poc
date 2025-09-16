/*
 * uart_interface.c
 *
 *  Created on: Nov 18th, 2023
 *      Author: Das, Somen
 *
 * @brief: wrapps HAL Uart API's for GSM com usage
 */

#include <lora_uart_interface.h>
#include "system_logging.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stm32g0xx_ll_gpio.h"

#include "system_types.h"
#include "stdio.h"
#include "string.h"
/******************************************************************************
 * Type Defs
 *****************************************************************************/
extern UART_HandleTypeDef huart1;

/* Circuler Buffer to hold Uart rx data*/
typedef struct
{
    void *buffer;      // data buffer
    void *buffer_end;  // end of data buffer
    uint16_t capacity; // maximum number of items in the buffer
    uint16_t count;    // number of items in the buffer
    uint16_t sz;       // size of each item in the buffer
    void *head;        // pointer to head
    void *tail;        // pointer to tail
} uart_rx_buff_t;

/******************************************************************************
 * Global variables
 *****************************************************************************/

/******************************************************************************
 * Local variablses
 *****************************************************************************/
static uint8_t command[500];

extern uint8_t rawByte;
static uart_rx_buff_t rx_buffer;

static uint8_t init_flag = 0;

/******************************************************************************
 * Local Function Prototypes
 *****************************************************************************/

/**
 * @brief Init uart rx round buffer
 * @param uart_rx_buff_t* Buffer
 */
static int lora_rx_buf_init(uart_rx_buff_t *buff);

/**
 * @brief Deinit uart rx round buffer
 * @param uart_rx_buff_t* Buffer
 */
static int lora_rx_buf_deinit(uart_rx_buff_t *buff);

/**
 * @brief push 1 byte data into the round buffer
 * @param uart_rx_buff_t* ptr of the buffer
 * @param uint8_t data
 * @retval OK(0) , N_OK(-1)
 */
static inline int lora_rx_buf_push_data(uart_rx_buff_t *buff, uint8_t data);

/**
 * @brief pop 1 byte data from the round buffer
 * @param uart_rx_buff_t* ptr of the buffer
 * @param uint8_t* out data 1 byte
 * @retval OK(0) , N_OK(-1)
 */
static inline int lora_rx_buf_pop_data(uart_rx_buff_t *buff, uint8_t *data);

/******************************************************************************
 * Global Function Definations
 *****************************************************************************/

/**
 * @brief initialize uart driver
 */
void lora_init_uart(void)
{
    if(init_flag == 0)
    {
        lora_rx_buf_init(&rx_buffer);
        HAL_UART_Receive_IT(&huart1, (uint8_t *)&rawByte, 1);
    }
    init_flag = 1;
}

/**
 * @brief deinitialize uart driver
 */
void lora_deinit_uart(void)
{
    if(init_flag == 1)
    {
        lora_rx_buf_deinit(&rx_buffer);
    }
}



void uart_push_data(uint8_t data)
{
	lora_rx_buf_push_data(&rx_buffer, data);
}
/**
 * @brief uart_send_frame
 * @note Have to rewrite this
 */
void lora_uart_send_frame(const char *send_command)
{
    int TX_message_length = 0;
    TX_message_length = sprintf((char *)&command, send_command);
    strcat((char *)command, "\r\n");
    HAL_UART_Transmit(&huart1, command, TX_message_length + 2, 100);
}

/**
 * @brief Available data from UART rx Buffer
*/
uint16_t lora_uart_data_available(void)
{
    return (rx_buffer.count);
}

/**
 * @brief Read data from UART rx Buffer
*/
uint8_t lora_uart_data_read(void)
{
    uint8_t retval = 0;
    lora_rx_buf_pop_data(&rx_buffer, &retval);
    return retval;
}

/**
 * @brief Flush UART rx Buffer
*/
void lora_uart_data_flush()
{
	rx_buffer.count = 0;
	rx_buffer.head = rx_buffer.buffer;
	rx_buffer.tail = rx_buffer.buffer;
}

/******************************************************************************
 * Local Function Definations
 *****************************************************************************/

/**
 * @brief Init uart rx round buffer
 * @param uart_rx_buff_t* Buffer
 */
static int lora_rx_buf_init(uart_rx_buff_t *buff)
{
    buff->buffer = pvPortMalloc((sizeof(uint8_t) * LORA_UART_ROUND_BUFF_MAXLEN));
    if (buff->buffer == NULL)
    {
        /*something went wrong */
        return -1;
    }
    buff->buffer_end = (uint8_t *)buff->buffer + LORA_UART_ROUND_BUFF_MAXLEN * sizeof(uint8_t);
    buff->capacity = LORA_UART_ROUND_BUFF_MAXLEN;
    buff->count = 0;
    buff->sz = sizeof(uint8_t);
    buff->head = buff->buffer;
    buff->tail = buff->buffer;
    return 0;
}

/**
 * @brief Deinit uart rx round buffer
 * @param uart_rx_buff_t* Buffer
 */
static int lora_rx_buf_deinit(uart_rx_buff_t *buff)
{
    if (buff->buffer != NULL)
    {
        vPortFree(buff->buffer);
    }
    buff->capacity = 0;
    buff->count = 0;
    return 0;
}

/**
 * @brief push 1 byte data into the round buffer
 * @param uart_rx_buff_t* ptr of the buffer
 * @param uint8_t data
 * @retval OK(0) , N_OK(-1)
 */
static inline int lora_rx_buf_push_data(uart_rx_buff_t *buff, uint8_t data)
{
    if (buff->count == buff->capacity)
    {
        /* Buffer is full */
        return -1;
    }
    memcpy((void*)buff->head, (void*)&data, buff->sz);
    buff->head = (uint8_t *)buff->head + buff->sz;
    if (buff->head == buff->buffer_end)
    {
        buff->head = buff->buffer;
    }
    buff->count++;
    return 0;
}

/**
 * @brief pop 1 byte data from the round buffer
 * @param uart_rx_buff_t* ptr of the buffer
 * @param uint8_t* out data 1 byte
 * @retval OK(0) , N_OK(-1)
 */
static inline int lora_rx_buf_pop_data(uart_rx_buff_t *buff, uint8_t *data)
{
    if (buff->count == 0)
    {
        /* No data to read */
        return -1;
    }
    memcpy(data, buff->tail, buff->sz);
    buff->tail = (uint8_t *)buff->tail + buff->sz;
    if (buff->tail == buff->buffer_end)
    {
        buff->tail = buff->buffer;
    }
    buff->count--;
    return 0;
}
