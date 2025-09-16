#pragma once


#include "stm32g0xx_hal.h"
#include "system_types.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"


/** CAN BUS DEVICE CONFIG */
#define CAN_TX_QUEUE_LEN 5
#define CAN_RX_QUEUE_LEN 5

/** user application data packet for can queues */
typedef struct CAN_User_msg_t
{
    uint8_t data[8];
    uint32_t ExtId;
}CAN_User_msg;

/* initialise_can_interface */
int initialise_can_interface(void);

/** Global CAN msg send interface */

/* Global Externs */
extern xQueueHandle CanRxQueue;
