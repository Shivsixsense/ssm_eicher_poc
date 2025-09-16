/*
 * eicher.c
 *
 *  Created on: Jan 13, 2025
 *      Author: shiv
 */

#include <can_interface.h>
#include"system_types.h"
#include "system_util.h"
#include "string.h"
#include "stdio.h"
#include "dev_appl.h"
#include "eicher.h"
#include "sys_log.h"

/******************************************************************************
 * Marco Defs
 *****************************************************************************/

/* Debug Log tag */

#define TAG " Eicher "

/******************************************************************************
 * Type Defs
 *****************************************************************************/

/******************************************************************************
 * Global variables
 *****************************************************************************/


static xTaskHandle eicher_task_h = NULL;
static xTaskHandle eicher_tx_task_h = NULL;
extern uint8_t rx_can_flag,CanDetected;
/******************************************************************************
 * Local variables
 *****************************************************************************/

Ecu_appl_parameter_list_t appl_param_list;
uint8_t  NoResponseCnt = 0;

/******************************************************************************
 * LOCAL FUNCTION PROTOTYPES
 *****************************************************************************/

static void eicher_receive_message_task(void *args);
static void eicher_tx_message_task(void *args);
void update_eicher_application_parameter_list(CAN_User_msg *response);
void hexArrayToHexString( unsigned char *hex_array, char *result_str);
uint8_t extract_bits_val(uint64_t data, int startBit, int len);
static int CalCulateVehicleSpeed(uint8_t *data);
static float CalAccelPedalPos(uint8_t *data);
static float CalCulateRelRAxl2Spd_L(uint8_t *data);
static float CalCulateRelRAxl2Spd_R(uint8_t *data);
static float CalCulateRelRAxl1Spd_L(uint8_t *data);
static float CalCulateRelRAxl1Spd_R(uint8_t *data);
static float CalCulateSteeringAngle(uint8_t *data);
static float CalCulateHours(uint8_t *data);

uint8_t extract_bits_motorola(const uint8_t *data, uint8_t bit_position, uint8_t num_bits);
/******************************************************************************
 * GLOBAL FUNCTION DEFINITIONS
 *****************************************************************************/



/** Global Fucntion Definations */

int initialise_eicher_task(void)
{

    /* create isotp rx task to handle rx can frames */
    if (xTaskCreate(eicher_receive_message_task, "Bms_receive_message_task", 1024, NULL, CAN_SERVICE_TASK_PRIORITY, &eicher_task_h) != pdPASS)
    {
        /* something serious is wrong */
        return 1;
    }
    /* create isotp rx task to handle rx can frames */
	if (xTaskCreate(eicher_tx_message_task, "Bms_transmitt_message_task", 1024, NULL, CAN_SERVICE_TASK_PRIORITY, &eicher_tx_task_h) != pdPASS)
	{
		/* something serious is wrong */
		return 1;
	}


    /* everything is just fine as it should be */
    return 0;
}

/**
 * @brief Receive response data
 */
static void eicher_receive_message_task(void *args)
{
    /* Holder from isotp rx frame generic to can msg */
	CAN_User_msg rxFrame;

    for (;;)
    {
        /** Wait for ECU response direct from CAN */
        if (xQueueReceive(CanRxQueue, &rxFrame, pdMS_TO_TICKS(portMAX_DELAY)) == pdPASS)
        {
        	update_eicher_application_parameter_list(&rxFrame);
        }
        vTaskDelay(pdMS_TO_TICKS(1));
    }

    /* Unexpected exit happend */
    vTaskDelete(NULL);
}

/**
 * @brief Receive response data
 */
static void eicher_tx_message_task(void *args)
{
    /* Holder from isotp rx frame generic to can msg */
	//CAN_User_msg TxFrame;

    for (;;)
    {
    	vTaskDelay(pdMS_TO_TICKS(500));


    	if(rx_can_flag == 2)
		{
			NoResponseCnt++;
			if(NoResponseCnt > 20)
			{
				 /* Reset All can data Strings */
				 reset_application_parameter_list();
				 NoResponseCnt = 0;
			}
		}
    	rx_can_flag = 2;

    }

    /* Unexpected exit happend */
    vTaskDelete(NULL);
}

/**
 * @brief Generic CAN send Interface generic
 * @param canFrame : (void*) Generic Can frame interface
 * @param waitTimeout : (U32) time out time
 */
int rte_send_can_message_to_queue(void *canFrame, U32 waitTimeout)
{
	if(xQueueSendToBack(CanTxQueue, canFrame, pdMS_TO_TICKS(waitTimeout)) != pdPASS)
	{
		return HAL_ERROR;
	}
   return HAL_OK;
}


/******************************************************************************
 * LOCAL FUNCTION DEFINITIONS
 *****************************************************************************/



void update_eicher_application_parameter_list(CAN_User_msg *response)
{

	  switch (response->ExtId)
	  {
	       case 0x18FEBF0B:   // Axel Wheel speed
	       {
	    	   appl_param_list.RelRAxl2Spd_L = CalCulateRelRAxl2Spd_L(response->data);
	    	   appl_param_list.RelRAxl2Spd_R = CalCulateRelRAxl2Spd_R(response->data);
	    	   appl_param_list.RelRAxl1Spd_L = CalCulateRelRAxl1Spd_L(response->data);
	    	   appl_param_list.RelRAxl1Spd_R = CalCulateRelRAxl1Spd_R(response->data);
			   CanDetected = 1;
	      	  break;
	       }

	       case 0x18FEF111:  // Speed/Brake/Clutch Switch
		   {
			//   uint64_t data = 0;
			   //memcpy(&data, response->data, sizeof(data));
			   appl_param_list.breakSw = extract_bits_motorola(response->data,28,2);
			   appl_param_list.cluchsw = extract_bits_motorola(response->data,30,2);
			   appl_param_list.SPD = CalCulateVehicleSpeed(response->data);
			  break;
		   }

	       case 0x18F01DE4:   // Steering_angle
		   {
			   appl_param_list.Steering_angle = CalCulateSteeringAngle(response->data);
			  break;
		   }

	       case 0xCF00300:   // Accelerator Pedal Position 1
		   {
			   appl_param_list.accel_prdal_pos = CalAccelPedalPos(response->data);
			   break;
		   }

	       case 0x18FEE500:
		   {
			   appl_param_list.Hrs = CalCulateHours(response->data);
			  break;
		   }

		   case 0xCFF0211:
		   {
			   appl_param_list.key_position = extract_bits_motorola(response->data, 14, 2);
			  break;
		   }

		   default:
		   {
			  break;
		   }
	  }

}


void hexArrayToHexString( unsigned char *hex_array, char *result_str)
{
    for (int i = 0; i < 8; i++) {
        sprintf(result_str + 2 * i, "%02x", hex_array[i]);
    }
}


static float CalAccelPedalPos(uint8_t *data)
{
    float value;
    value = hexToDecimal(data, 1, 1);
    value = value * 0.4;
    return value;
}


static float CalCulateRelRAxl2Spd_L(uint8_t *data)
{
    float value;
    value = hexToDecimal(data, 1, 2);
    value = (value * 0.0625) - 7.81245;
    return value;
}

static float CalCulateRelRAxl2Spd_R(uint8_t *data)
{
    float value;
    value = hexToDecimal(data, 1, 3);
    value = (value * 0.0625) - 7.81245;
    return value;
}

static float CalCulateRelRAxl1Spd_L(uint8_t *data)
{
	float value;
	value = hexToDecimal(data, 1, 2);
	value = (value * 0.0625) - 7.81245;
	return value;
}

static float CalCulateRelRAxl1Spd_R(uint8_t *data)
{
	float value;
	value = hexToDecimal(data, 1, 3);
	value = (value * 0.0625) - 7.81245;
	return value;
}


static int CalCulateVehicleSpeed(uint8_t *data)
{
    unsigned char tmpr[2];
    memset(tmpr, 0, 2);
    int value;
    // swapBytesAndStore(Sonalika_Data.Hex18fee500,tmp,0,0);
    swapBytesHexArray(data, 1, 2, tmpr);
    value = hexToDecimal(tmpr, 2, 0);
    value = value * 0.00390625;
    return value;
}

static float CalCulateSteeringAngle(uint8_t *data)
{
    unsigned char tmpr[2];
    float value;
    memset(tmpr, 0, 2);
    // swapBytesAndStore(Sonalika_Data.Hex18fee500,tmp,0,0);
    swapBytesHexArray(data, 0, 1, tmpr);
    value = hexToDecimal(tmpr, 2, 0);
    value = (value *0.0009765625) * 57.6;
    return value;
}

static float CalCulateHours(uint8_t *data)
{
    unsigned char tmpr[4];
    memset(tmpr, 0, 4);
    float value;
    // swapBytesAndStore(Sonalika_Data.Hex18fee500,tmp,0,0);
    swapBytesHexArray(data, 0, 3, tmpr);
    value = hexToDecimal(tmpr, 4, 0);
    value = value * 0.05;
    return value;
}


uint8_t extract_bits_val(uint64_t data, int startBit, int len)
{
    uint64_t mask = (1ULL << (len + 1)) - 1;
    mask &= ~(1 << (len));
    mask <<= (64 - (startBit + len));
    return (data & mask) >> (64 - (startBit + len));
}

uint8_t extract_bits_motorola(const uint8_t *data, uint8_t bit_position, uint8_t num_bits) {
    if (bit_position + num_bits > 64 || num_bits > 8) {
        return 0;  // Ensure we don't access out of bounds and restrict to max 8 bits
    }

    uint8_t result = 0;
    uint8_t i = 0;
   // for (uint8_t i = 0; i < num_bits; i++) {
        uint8_t byte_index = 7 - ((bit_position) / 8) - 1;  // Reverse byte order
        uint8_t bit_offset = (bit_position ) % 8;  // Normal bit offset
        uint8_t bit_value = (data[byte_index] >> bit_offset) & 0x01;  // Extract bit

        result |= (bit_value << i);  // Store extracted bit at the correct position
   // }

    return result;
}


uint32_t GetHourValue(void)
{
	return appl_param_list.Hrs;
}

void reset_application_parameter_list(void)
{
    memset(&appl_param_list, 0, sizeof(appl_param_list));
}
