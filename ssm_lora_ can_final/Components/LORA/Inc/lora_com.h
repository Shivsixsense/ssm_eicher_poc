/*
 * gsm_com.h
 *
 *  Created on: Nov 18th, 2023
 *      Author: Das, Somen
 *
 * @brief: Handle GSM module communication through AT command sets
 */
#pragma once
#include <lora_uart_interface.h>
#include "stdbool.h"
#include "main.h"

#define ERROR_THRESHOLD 10

/* GSM runtime communication data */
typedef struct
{
    char _buffer[1500];
    char _readBuffer[5120];
    char _reqBuffer[1500];
    char LORA_Header[1024];
    char _command[32];
    char _get_buffer[500];
    char ATReq[255];
    char ATRsp[1024];
    char imei[16];
    char time_buff[50];
    char utcString[30];
    uint8_t time_set;
    char _gpsBuffer[90];
} LORA_Runtime_info;

typedef enum lora_status_type_e
{

    LORA_OK = 0x00,
    LORA_ERROR = 0x01,
    LORA_BUSY = 0x02,
} LoraStatusType;


int init_lora_module(void);
int lora_send_packet(uint8_t* packet , size_t len);
int lora_recieve_packet(char* packet);
