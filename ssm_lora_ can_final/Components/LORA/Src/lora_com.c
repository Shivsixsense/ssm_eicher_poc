/*
 * LORA_com.c
 *
 *  Created on: Nov 18th, 2023
 *      Author: Das, Somen
 *
 * @brief: Handle LORA module communication through AT command sets
 */
#include <lora_uart_interface.h>
#include "../Inc/lora_com.h"
#include "system_util.h"
#include "stm32g0xx_ll_gpio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include"math.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "system_util.h"
/* Debug tag */
#define TAG "LORA "
#define MAX_FIELDS 20

/* Helper Macro */
#define LORA_CHECK_ERR(x)               \
    if (x != LORA_OK)                   \
    {                                  \
        return LORA_ERROR;              \
    }
#define LORA_CHECK_ERR_RET(x, y)        \
    if (x != LORA_OK)                   \
    {                                  \
        return y;                      \
    }
#define LORA_CHECK_OK(x) \
    if (x == LORA_OK)    \
    {                   \
        return LORA_OK;  \
    }

#define _SEC(x) ((x) * 1000)

/******************************************************************************
 * Type Defs
 *****************************************************************************/

/******************************************************************************
 * Global variables
 *****************************************************************************/
extern IWDG_HandleTypeDef hiwdg;
/******************************************************************************
 * Local variablses
 *****************************************************************************/
static uint8_t lora_Error_cnt = 0;
LORA_Runtime_info LORA_run_info;
static SemaphoreHandle_t LORA_driver_sem = NULL;
char lora_msg[400] = {0};
/******************************************************************************
 * Local Function Prototypes
 *****************************************************************************/
void LORA_flush(void);
static int LORA_Check_Multi_Line_Reply(const char *command, const char *reply, uint16_t timeout);
static int LORA_Wait_For_Reply(const char *command, const char *reply, uint16_t timeout);
static int LORA_Wait_For_Lines(const char *command, uint16_t lines, uint16_t timeout);
static int LORA_read_Lines(uint16_t lines, uint16_t timeout);
static int LORA_read_Reply(const char *reply, uint16_t timeout);
static int LORA_read_bytes(uint16_t numBytes, uint16_t timeout);
/******************************************************************************
 * Global Function Definations
 *****************************************************************************/



/**
 * @brief initalize sim module
 * @param init_timeout wait till initiaze
 */
int init_lora_module(void)
{
    /* Set no error */
    lora_Error_cnt = 0;

    /* check if semaphore has been created already */
    if (LORA_driver_sem == NULL)
    {
        /* create semaphore */
    	LORA_driver_sem = xSemaphoreCreateBinary();
    }

    xSemaphoreGive(LORA_driver_sem);

    lora_init_uart();


    retry:
	HAL_IWDG_Refresh(&hiwdg);
    if(LORA_Check_Multi_Line_Reply("AT+QBAND=?", "QBAND:5", _SEC(2)) != LORA_OK)
    {
        if(LORA_Check_Multi_Line_Reply("AT+QBAND=5", "OK", _SEC(2)) != LORA_OK)
        {

        }
        goto retry;
    }

    if(LORA_Check_Multi_Line_Reply("AT+QCS", "STORED", _SEC(10)) != LORA_OK)
    {
        LORA_CHECK_ERR(LORA_Check_Multi_Line_Reply("AT+QRFS", "APPLICATION_VERSION", _SEC(10)))
         goto retry;
    }

    memset(LORA_run_info.ATRsp,0,1024);
    if(LORA_Wait_For_Lines("AT+QJOIN=1", 9, _SEC(15)) == LORA_OK)
    {
    	strcpy(LORA_run_info.LORA_Header,LORA_run_info.ATRsp);
    	if(strstr(LORA_run_info.LORA_Header,"JOINED"))
    	{
    	    return LORA_OK;
    	}
    	else
    	{
            goto retry;
    	}
    }
    else
    {
        goto retry;
    }

//    LORA_CHECK_ERR(LORA_Check_Multi_Line_Reply("AT+MODE=0", "OK", _SEC(5)))
//
//    LORA_CHECK_ERR(LORA_Check_Multi_Line_Reply("AT+PARAMETER=11,9,4,12", "OK", _SEC(2)))
//
//    LORA_CHECK_ERR(LORA_Check_Multi_Line_Reply("AT+BAND=915000000", "OK", _SEC(2)))
//
//    LORA_CHECK_ERR(LORA_Check_Multi_Line_Reply("AT+NETWORKID=18", "OK", _SEC(2)))
//
//    LORA_CHECK_ERR(LORA_Check_Multi_Line_Reply("AT+CRFOP=22", "OK", _SEC(2)))
//
//    LORA_CHECK_ERR(LORA_Check_Multi_Line_Reply("AT+ADDRESS=22", "OK", _SEC(2)))

    return LORA_OK;
}





int lora_send_packet(uint8_t* packet, size_t len) {

    char msg[300] = {0};

    for (size_t i = 0; i < len; i++) {
        sprintf(&msg[i * 2], "%02X", packet[i]);  // Corrected indexing
    }

    sprintf(lora_msg, "AT+QSEND=25:1:%s",msg);

    if (LORA_Wait_For_Reply(lora_msg, "txDone", _SEC(5)) == LORA_OK)
    {
        return LORA_OK;
    }
    else
    {
        return LORA_ERROR;
    }
}


int lora_recieve_packet(char* packet)
{

   //snprintf(lora_msg, sizeof(lora_msg), "AT+SEND=22,%d,%s", strlen(msg), msg);
   if(LORA_read_Lines(1 , _SEC(3)) == LORA_OK)
   {
	   strcpy(packet,LORA_run_info.ATRsp);
	   return LORA_OK;
   }
   else
   {
	    return LORA_ERROR;
   }
   return LORA_ERROR;
}


/**
 * @brief Send Command and Check for Multiline Response
 */
static int LORA_Check_Multi_Line_Reply(const char *command, const char *reply, uint16_t timeout)
{
    LORA_CHECK_OK(LORA_Wait_For_Reply(command, reply, timeout))
    return LORA_ERROR;
}

static int LORA_Wait_For_Reply(const char *command, const char *reply, uint16_t timeout)
{
    LORA_flush();
    lora_uart_send_frame(command);
    return LORA_read_Reply(reply, timeout);
}

static int LORA_Wait_For_Lines(const char *command, uint16_t lines, uint16_t timeout)
{
    LORA_flush();
    lora_uart_send_frame(command);
    return LORA_read_Lines(lines, timeout);
}

static int LORA_read_Reply(const char *reply, uint16_t timeout)
{
    uint32_t tmr = 0;
    uint16_t idx = 0;
    char data = 0;
    LORA_run_info.ATRsp[idx] = 0;

    if (LORA_driver_sem != NULL)
    {
        if (xSemaphoreTake(LORA_driver_sem, (TickType_t)_SEC(5)) != pdTRUE)
        {
            return LORA_BUSY;
        }

        StartTimer(tmr, timeout);
        do
        {
            vTaskDelay(pdMS_TO_TICKS(5));
            while (lora_uart_data_available() > 0)
            {
                data = (char)lora_uart_data_read();
                if ((data == '\r') || (data == '\n'))
                {
                    continue;
                }
                LORA_run_info.ATRsp[idx++] = data;
                LORA_run_info.ATRsp[idx] = 0;
            }
        } while (IsTimerRunning(tmr) && (strstr(LORA_run_info.ATRsp, reply) == NULL));

        /* No data received from LORA Module */
        if (idx == 0)
        {
            lora_Error_cnt++;
        }

        xSemaphoreGive(LORA_driver_sem);
        return ((IsTimerRunning(tmr) == 1) ? LORA_OK : LORA_ERROR);
    }
    return LORA_ERROR;
}

static int LORA_read_Lines(uint16_t lines, uint16_t timeout)
{
    uint32_t tmr = 0;
    uint16_t idx = 0;
    uint16_t linesFound = 0;
    char data = 0;
    LORA_run_info.ATRsp[idx] = 0;

    if (LORA_driver_sem != NULL)
    {
        if (xSemaphoreTake(LORA_driver_sem, (TickType_t)_SEC(5)) != pdTRUE)
        {
            return LORA_BUSY;
        }

        StartTimer(tmr, timeout);
        do
        {
            vTaskDelay(pdMS_TO_TICKS(5));
            while (lora_uart_data_available() > 0)
            {
                data = (char)lora_uart_data_read();
                if ((data == '\r') || (data == (char)0))
                {
                    continue;
                }

                if ((idx == 0) && (data == '\n'))
                {
                    continue;
                }

                if (data == '\n')
                {
                    linesFound++;
                }

                if (linesFound >= lines)
                {
                    break;
                }
                LORA_run_info.ATRsp[idx++] = data;
                LORA_run_info.ATRsp[idx] = 0;
            }
        } while (IsTimerRunning(tmr) && (linesFound < lines));

        /* No data received from LORA Module */
        if (idx == 0)
        {
            lora_Error_cnt++;
        }
        xSemaphoreGive(LORA_driver_sem);
        return ((IsTimerRunning(tmr) == 1) ? LORA_OK : LORA_ERROR);
    }
    return LORA_ERROR;
}


/**
 * @brief Read raw bytes from LORA Module
 * @param numBytes : Number of bytes to be ready
 * @param timeout : LORA Module Read timeout
 * @return LORA Status
*/
static int LORA_read_bytes(uint16_t numBytes, uint16_t timeout)
{
    uint32_t tmr = 0;
    uint16_t idx = 0;

    /* check if initialize properly */
    if (LORA_driver_sem != NULL)
    {
        /* take control of LORA driver */
        if (xSemaphoreTake(LORA_driver_sem, (TickType_t)_SEC(5)) != pdTRUE)
        {
            return LORA_BUSY;
        }

        /* start response wait timer */
        StartTimer(tmr, timeout);
        do
        {
            vTaskDelay(pdMS_TO_TICKS(5));
            while (lora_uart_data_available() > 0)
            {
                LORA_run_info._readBuffer[idx++] = (char)lora_uart_data_read();
            }
        } while (IsTimerRunning(tmr) && (idx < numBytes));

        /* No data received from LORA Module */
        if (idx == 0)
        {
            lora_Error_cnt++;
        }
        xSemaphoreGive(LORA_driver_sem);
        return ((IsTimerRunning(tmr) == 1) ? LORA_OK : LORA_ERROR);
    }
    return LORA_ERROR;
}


/**
 * @brief LORA Module flush unread data
 */
void LORA_flush(void)
{
    lora_uart_data_flush();
}

