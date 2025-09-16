/*
 * can_interface.c
 *
 *  Created on: June 14th, 2024
 *      Author: Shiv
 *
 * @brief:  wrapps CAN hal library with tp layer interface
 */

#include "can_interface.h"
#include "system_logging.h"
#include "main.h"
/******************************************************************************
 * Marco Defs
 *****************************************************************************/

#define FDCAN2_can_handle hfdcan2
#define CMSIS_OS_PRIOR_NORMAL 24
#define DATALEN_TO_FD_DLC(x) (x << 16)
#define FD_DLC_TO_DATALEN(x) (x >> 16)

/******************************************************************************
 * Type Defs
 *****************************************************************************/

/******************************************************************************
 * Global variables
 *****************************************************************************/
extern FDCAN_HandleTypeDef hfdcan2;
/* queue to hold all the receive can messages */
xQueueHandle CanRxQueue;
uint8_t rx_can_flag = 0;

/******************************************************************************
 * Local variables
 *****************************************************************************/
static xTaskHandle can_Rx_task_h = NULL;
static xTaskHandle can_tx_task_h = NULL;
extern xQueueHandle CanTxQueue;
/** receive can msg header */
FDCAN_RxHeaderTypeDef RxHeader;
static FDCAN_TxHeaderTypeDef TxHeader;
FDCAN_ProtocolStatusTypeDef drv_state;
/******************************************************************************
 * LOCAL FUNCTION PROTOTYPES
 *****************************************************************************/
static void can_Rx_task(void *args);
static void FDCan_Config(void);
static void can_tx_task(void *args);
HAL_StatusTypeDef canbus_set_tx_header(void);
static void Can_driver_status_and_recovery_scheduler(void);
/******************************************************************************
 * GLOBAL FUNCTION DEFINATIONS
 ******************************************************************************/


/******************************************************************************
 * LOCAL FUNCTION DEFINATIONS
 ******************************************************************************/



int initialise_can_interface(void)
{

    /* configure and start the can driver */
	FDCan_Config( );


    /** Create can ITL rx queue*/
    CanRxQueue = xQueueCreate(CAN_TX_QUEUE_LEN, sizeof(CAN_User_msg));

    CanTxQueue = xQueueCreate(CAN_TX_QUEUE_LEN, sizeof(CAN_User_msg));

    /** check if the queue has been created properly */
    if ((CanRxQueue == NULL) || (CanRxQueue == NULL))
    {
        /* something serious happend */
        print_logln("ERROR: Can TxRx Queue not created");
        return 1;
    }

    if (xTaskCreate(can_Rx_task, "can_Rx_task", 1024, NULL, CAN_RX_TASK_PRIORITY, &can_Rx_task_h) != pdPASS)
    {
        return 1;
    }

    if (xTaskCreate(can_tx_task, "can_tx_task", 1024, NULL, PULSE_RUNTIME_TASK_PRIORITY, &can_tx_task_h) != pdPASS)
	{
		return 1;
	}

    return 0;
}



/** Task for receive can messages polling
 * @bug ISR create scheduler issues
*/
static void can_Rx_task(void *args)
{
    /** holder for rx msg compatible with TP interface */
    CAN_User_msg RxItlMsgData;

    for (;;)
    {
        /* Check if any can msg is received by the FIFO0 */
        if(HAL_FDCAN_GetRxFifoFillLevel(&FDCAN2_can_handle, FDCAN_RX_FIFO0) > 0)
        {
            /* Get the can msg from HAL driver */
            HAL_FDCAN_GetRxMessage(&FDCAN2_can_handle, FDCAN_RX_FIFO0, &RxHeader, RxItlMsgData.data);

            /* Update the Id */
            RxItlMsgData.ExtId = (RxHeader.Identifier);
            rx_can_flag = 1;
            /* Send to ISO TP queue for processing */
            if(xQueueSendToBack(CanRxQueue, &RxItlMsgData, pdMS_TO_TICKS(10)) != pdTRUE)
            {
                /** erro queue may be full, fast process required */
               // print_logln("ERROR: CANRx : xQueueSendFromISR");
            }
            vTaskDelay(pdMS_TO_TICKS(1));
        }
        else
        {
            vTaskDelay(pdMS_TO_TICKS(2));
        }
    }
}


/** Task for transmit can messages */
static void can_tx_task(void *args)
{
    CAN_User_msg txMsgData;
    HAL_StatusTypeDef result;

    canbus_set_tx_header();
    for (;;)
    {
        /** Wait for any application request to send can msg */
        if (xQueueReceive(CanTxQueue, &txMsgData, pdMS_TO_TICKS(100)) == pdPASS)
        {
        	TxHeader.Identifier = txMsgData.ExtId;
            do
            {
                /** Send can msg to the driver */
                result = HAL_FDCAN_AddMessageToTxFifoQ(&FDCAN2_can_handle, &TxHeader, txMsgData.data);

                /** retry if the driver is busy */
            } while (result == HAL_BUSY);

        }
        vTaskDelay(pdMS_TO_TICKS(1));

        /** Check any CAN bus error or bus off status */
        Can_driver_status_and_recovery_scheduler();
        vTaskDelay(pdMS_TO_TICKS(10));
    }

    /* Unexpected exit of task */
    vTaskDelete(can_tx_task_h);
}

static void FDCan_Config(void)
{
    FDCAN_FilterTypeDef sFilterConfig;

    sFilterConfig.IdType = FDCAN_EXTENDED_ID;
	sFilterConfig.FilterIndex = 0;
	sFilterConfig.FilterType = FDCAN_FILTER_MASK;
	sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;

    if (HAL_FDCAN_ConfigFilter(&FDCAN2_can_handle, &sFilterConfig) != HAL_OK)
    {
        /* Filter configuration Error */
    	Error_Handler();
    }

    /* Start the FDCAN module */
    if (HAL_FDCAN_Start(&FDCAN2_can_handle) != HAL_OK)
    {
    	Error_Handler();
    }
}


/** Function to set can transmit id by user command */
HAL_StatusTypeDef canbus_set_tx_header(void)
{
    TxHeader.DataLength = FDCAN_DLC_BYTES_8;
    TxHeader.IdType = FDCAN_EXTENDED_ID;
    TxHeader.TxFrameType = FDCAN_DATA_FRAME;
    TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    TxHeader.BitRateSwitch = FDCAN_BRS_OFF;
    TxHeader.FDFormat = FDCAN_CLASSIC_CAN;
    TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
    TxHeader.MessageMarker = 0;

    return HAL_OK;
}

static void Can_driver_status_and_recovery_scheduler(void)
{
    if(HAL_FDCAN_GetProtocolStatus(&FDCAN2_can_handle, &drv_state) == HAL_OK)
    {
    	switch(drv_state.Activity)
		{
			case FDCAN_COM_STATE_SYNC:
				print_logln("INFO: CAN BUS Synchronizing");
				break;

			case FDCAN_COM_STATE_IDLE:
				print_logln("INFO: CAN BUS Idle");
				break;

			case FDCAN_COM_STATE_RX:
				print_logln("INFO: CAN BUS Receiving");
				break;

			case FDCAN_COM_STATE_TX:
				print_logln("INFO: CAN BUS Transmitting");
				break;

			default:
				print_logln("INFO: CAN BUS Unknown state");
				break;
		}

        if(drv_state.ErrorPassive == 1)
        {
            print_logln("ERROR: CAN BUS ErrorPassive");
        }

        if(drv_state.BusOff != 0)
        {
            print_logln("ERROR: CAN BUS BusOff");
            HAL_FDCAN_Stop(&FDCAN2_can_handle);
            vTaskDelay(pdMS_TO_TICKS(500));
            MX_FDCAN2_Init();
            vTaskDelay(pdMS_TO_TICKS(500));
            FDCan_Config();
            canbus_set_tx_header();
        }
    }
}

