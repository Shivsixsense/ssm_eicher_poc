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

/* software timer helper macro */
#define IsTimerElapsed(x) ((x > 0) && (x <= (HAL_GetTick())))
#define IsTimerRunning(x) ((x > 0) && (x > (HAL_GetTick())))
#define IsTimerEnabled(x) ((x) > 0)
#define StartTimer(x, y)         \
    {                            \
        x = (HAL_GetTick()) + y; \
    }
#define ResetTimer(x, y)         \
    {                            \
        x = (HAL_GetTick()) + y; \
    }
#define StopTimer(x) \
    {                \
        x = 0;       \
    }

/**
 * @brief Format for adding U8 element in JSON
 */
#define ADD_JSON_ELEMENT_U8(DataType, Data)       \
    {                                             \
        sprintf(tmp, "%03d", Data);               \
        strcat(DataType, tmp);                    \
    }

/**
 * @brief Format for adding U16 element in JSON
 */
#define ADD_JSON_ELEMENT_U16(DataType, Data)       \
    {                                              \
        sprintf(tmp, "%05d", Data);                \
        strcat(DataType, tmp);                     \
    }
/**
 * @brief Format for adding U16 element in JSON
 */
#define ADD_JSON_ELEMENT_U32(DataType, Data) \
    {                                              \
        sprintf(tmp, "%ld", Data);                 \
        strcat(DataType, tmp);                     \
    }
/**
 * @brief Format for adding float element in JSON
 */
#define ADD_JSON_ELEMENT_FLOAT(DataType, Data) \
    {                                                \
        sprintf(tmp, "%04.1f", Data);                \
        strcat(DataType, tmp);                       \
        strcat(DataType, "\"");                      \
    }

#define ADD_JSON_ELEMENT_TWO_DEC_FLOAT(Name, DataType, Data) \
    {                                                \
        strcat(DataType, "\"" Name "\":\"");         \
        sprintf(tmp, "%04.2f", Data);                \
        strcat(DataType, tmp);                       \
        strcat(DataType, "\"");                      \
    }


#define ADD_GPS_ELEMENT_FLOAT(DataType, Data) \
    {                                                \
        sprintf(tmp, "%04.9f", Data);                \
        strcat(DataType, tmp);                       \
    }

/**
 * @brief Format for adding more element in JSON
 */
#define ADD_STRING(DataType, Data)     \
    {                                        \
        strcat(DataType, Data);              \
    }

#define ADD_DEC_TO_TEXT(DataType, Data)  \
    {                                    \
        S0 = Data / 100 + 0x1E;          \
        S1 = ((Data % 100) / 10) + 0x1e; \
        S2 = ((Data % 100) % 10) + 0x1e; \
        sprintf(tmp, "%d", S0);          \
        strcat(DataType, tmp);           \
        sprintf(tmp, "%d", S1);          \
        strcat(DataType, tmp);           \
        sprintf(tmp, "%d", S2);          \
        strcat(DataType, tmp);           \
        strcat(DataType, "\"");          \
    }

/**
 * @brief Format for adding more element in JSON
 */
#define ADD_JASON_ELEMENT_HEX(DataType, Data) \
    {                                         \
        sprintf(tmp, "%02X", Data);           \
        strcat(DataType, tmp);                \
    }

/**
 * @brief Format for adding more element in JSON
 */
#define ADD_MORE(DataType, JSON_MACRO) \
    {                                  \
        strcat(DataType, ",");         \
        JSON_MACRO                     \
    }
/**
 * @brief Declare variable needed for Other macro
 */
#define JSON_MACRO_INIT char tmp[50];

typedef union {
    struct {
        uint8_t lbyte;
        uint8_t hbyte;
    } bytes;
    uint16_t word;
} UWord;



void swapBytesHexArray(unsigned char *array, size_t start, size_t end, unsigned char *result);
unsigned int hexToDecimal(unsigned char hexValue[], int numBytes, int startByte);
