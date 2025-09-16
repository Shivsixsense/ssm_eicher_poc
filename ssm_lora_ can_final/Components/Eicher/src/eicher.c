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


xQueueHandle CanTxQueue;
static xTaskHandle eicher_task_h = NULL;
static xTaskHandle eicher_tx_task_h = NULL;
extern uint8_t rx_can_flag;
/******************************************************************************
 * Local variables
 *****************************************************************************/
uint8_t NoResponseCnt = 0;
Ecu_appl_parameter_list_t appl_param_list;
char json_data[255];
char UID[] = "SSM01";
extern char utcString[30];
/******************************************************************************
 * LOCAL FUNCTION PROTOTYPES
 *****************************************************************************/

static void eicher_receive_message_task(void *args);
static void eicher_tx_message_task(void *args);
void update_eicher_application_parameter_list(CAN_User_msg *response);
void hexArrayToHexString( unsigned char *hex_array, char *result_str);


static int CalculateRTCSecValue(uint8_t* data);
static int CalculateRTCMinValue(uint8_t* data);
static int CalculateRTCHourValue(uint8_t* data);
static int CalculateRTCMonthValue(uint8_t* data);
static int CalculateRTCYearValue(uint8_t* data);
static int CalculateRTCDayValue(uint8_t* data);
static int CalculateRTCMinOffValue(uint8_t* data);
static int CalculateRTCHourOffValue(uint8_t* data);
static float CalCulateRpm(uint8_t *data);
static int CalCulateCTM(uint8_t *data);
static float CalCulateHours(uint8_t *data);
static float CalCulateVehicleSpeed(uint8_t *data);
static float CalCulateOdo(uint8_t *data);
static int CalculateFuelLevValue(uint8_t* data);
static int CalculateOilPressure(uint8_t *data);
static float CalCulateBattVolt(uint8_t *data);
static int CalculateEngCoolantLevValue(uint8_t* data);
static float CalculateUreaQuality(uint8_t* data);
static float CalEngTotalIdelFuelUsed(uint8_t *data);
static float CalEngTotalIdelHoursValue(uint8_t *data);
char * CalculateHex18FD7C00(uint8_t* data);
static int CalAfterTreatmentTemperature(uint8_t* data);
static float CalAfterTreatmentFluidLev(uint8_t* data);
static float Catalystupstreamtemperature(uint8_t* data);
static int CalIntakeAirtempretaure(uint8_t* data);
static float Calculate_HRLFC_value(uint8_t *data);
static int Calculate_Fan_Speed(uint8_t* data);
static int CalAccelPedalPos(uint8_t *data);
static uint8_t ActualEnginePercentTorque(uint8_t* data);
static uint8_t EngineDemandPercentTorque(uint8_t* data);
/******************************************************************************
 * GLOBAL FUNCTION DEFINITIONS
 *****************************************************************************/



/** Global Fucntion Definations */

int initialise_eicher_task(void)
{

	CanTxQueue = xQueueCreate(CAN_RX_QUEUE_LEN, sizeof(CAN_User_msg));
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
        if (xQueueReceive(CanRxQueue, &rxFrame, pdMS_TO_TICKS(2000)) == pdPASS)
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
	       case 0x18FD0900:
	       {
	    	   Calculate_HRLFC_value(response->data);
				  break;
	       }
	       case 0x18FEE617:
	       {
	    	  CalculateRTCSecValue(response->data);
	    	  CalculateRTCMinValue(response->data);
			  CalculateRTCHourValue(response->data);
			  CalculateRTCMonthValue(response->data);
			  CalculateRTCYearValue(response->data);
			  CalculateRTCDayValue(response->data);
			  CalculateRTCMinOffValue(response->data);
			  CalculateRTCHourOffValue(response->data);
			  break;
	       }

	       case 0x18FEC117:
		   {
			   CalCulateOdo(response->data);
			  break;
		   }

	       case 0x18FEFC17:
	       {
	    	   CalculateFuelLevValue(response->data);
	    	   break;
	       }

	       case 0x0CF00400:
	       {
	    	   CalCulateRpm(response->data);
	    	   ActualEnginePercentTorque(response->data);
	    	   EngineDemandPercentTorque(response->data);
		      break;
	       }

	       case 0x18FEE500:
	       {
	    	   CalCulateHours(response->data);
	      	  break;
	       }

	       case 0x18FEF111:
		   {
			   CalCulateVehicleSpeed(response->data);
			  break;
		   }

	       case 0x18FEEF00:
		   {
			   CalculateOilPressure(response->data);
			   CalculateEngCoolantLevValue(response->data);
			  break;
		   }

	       case 0x18FEEE00:
		   {
			   CalCulateCTM(response->data);
			  break;
		   }

	       case 0x18FEF700:
		   {
			   CalCulateBattVolt(response->data);
			  break;
		   }

		   case 0x18FD9B3D:
		   {
			   CalculateUreaQuality(response->data);
			  break;
		   }

		   case 0x18FD7C00:
		   {
			   CalculateHex18FD7C00(response->data);
			  break;
		   }

		   case 0x18FE5600:
		   {
			   CalAfterTreatmentTemperature(response->data);
			   CalAfterTreatmentFluidLev(response->data);
			  break;
		   }

	       case 0x18FD2000:
		   {
			   Catalystupstreamtemperature(response->data);
			  break;
		   }

		   case 0x18FEFF00:
		   {

			  break;
		   }

		   case 0x18FEF500:   //CAN in Doubt
		   {
			  CalIntakeAirtempretaure(response->data);
			  break;
		   }

		   case 0X18FEDC00:
		   {
			   CalEngTotalIdelFuelUsed(response->data);
			   CalEngTotalIdelHoursValue(response->data);
			  break;
		   }

		   case 0x18FEBD00:
		   {
			   Calculate_Fan_Speed(response->data);
			   break;
		   }

		   case 0xCF00300:   // Accelerator Pedal Position 1
		   {
			   appl_param_list.accel_prdal_pos = CalAccelPedalPos(response->data);
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

static int CalculateRTCSecValue(uint8_t* data)
{
	appl_param_list.Sec = hexToDecimal(data, 1, 0);
    appl_param_list.Sec = appl_param_list.Sec * 0.25;
    return appl_param_list.Sec;
}

static int CalculateRTCMinValue(uint8_t* data)
{
	appl_param_list.Min = hexToDecimal(data, 1, 1);
    return appl_param_list.Min;
}

static int CalculateRTCHourValue(uint8_t* data)
{
	appl_param_list.Hour = hexToDecimal(data, 1, 2);
    return appl_param_list.Hour;
}

static int CalculateRTCMonthValue(uint8_t* data)
{
	appl_param_list.Month = hexToDecimal(data, 1, 3);
    return appl_param_list.Month;
}

static int CalculateRTCYearValue(uint8_t* data)
{
	appl_param_list.Year = hexToDecimal(data, 1, 5);
    return appl_param_list.Year;
}

static int CalculateRTCDayValue(uint8_t* data)
{
	appl_param_list.Day = hexToDecimal(data, 1, 4);
    appl_param_list.Day = appl_param_list.Day * 0.25;
    return appl_param_list.Day;
}

static int CalculateRTCMinOffValue(uint8_t* data)
{
	appl_param_list.MinOffset = hexToDecimal(data, 1, 6);
    return appl_param_list.MinOffset;
}

static int CalculateRTCHourOffValue(uint8_t* data)
{
	appl_param_list.HourOffset = hexToDecimal(data, 1, 7);
    return appl_param_list.HourOffset;
}

static float CalCulateRpm(uint8_t *data)
{
    unsigned char tmpr[2];
    swapBytesHexArray(data, 3, 4, tmpr);
    appl_param_list.RPM = hexToDecimal(tmpr, 2, 0);
    appl_param_list.RPM = appl_param_list.RPM * 0.125;
    return appl_param_list.RPM;
}


static int CalCulateCTM(uint8_t *data)
{
    appl_param_list.CTM = hexToDecimal(data, 1, 0);
    if (rx_can_flag == 1)
    {
        appl_param_list.CTM = appl_param_list.CTM - 40;
    }
    return appl_param_list.CTM;
}

static float CalCulateHours(uint8_t *data)
{
    unsigned char tmpr[4];
    memset(tmpr, 0, 4);
    // swapBytesAndStore(Sonalika_Data.Hex18fee500,tmp,0,0);
    swapBytesHexArray(data, 0, 3, tmpr);
    appl_param_list.Hours = hexToDecimal(tmpr, 4, 0);
    appl_param_list.Hours = appl_param_list.Hours * 0.05;
    return appl_param_list.Hours;
}

static float CalCulateVehicleSpeed(uint8_t *data)
{
    unsigned char tmpr[2];
    memset(tmpr, 0, 2);
    // swapBytesAndStore(Sonalika_Data.Hex18fee500,tmp,0,0);
    swapBytesHexArray(data, 1, 2, tmpr);
    appl_param_list.SPD = hexToDecimal(tmpr, 2, 0);
    appl_param_list.SPD = appl_param_list.SPD * 0.00390625;
    return appl_param_list.SPD;
}

static float CalCulateOdo(uint8_t *data)
{
    unsigned char tmpr[4];
    memset(tmpr, 0, 4);
    // swapBytesAndStore(Sonalika_Data.Hex18fee500,tmp,0,0);
    swapBytesHexArray(data, 0, 3, tmpr);
    appl_param_list.TotolVehicleDistane = hexToDecimal(tmpr, 4, 0);
    appl_param_list.TotolVehicleDistane = (appl_param_list.TotolVehicleDistane * 5)/1000;
    return appl_param_list.TotolVehicleDistane;
}

static int CalculateFuelLevValue(uint8_t* data)
{
	appl_param_list.FLV = hexToDecimal(data, 1, 1);
	appl_param_list.FLV = appl_param_list.FLV * 0.4;
    return appl_param_list.FLV;
}

static int CalculateOilPressure(uint8_t *data)
{
	appl_param_list.EngineOilPressure = hexToDecimal(data, 1, 3);
	appl_param_list.EngineOilPressure = appl_param_list.EngineOilPressure * 4;
	return appl_param_list.EngineOilPressure;
}

static float CalCulateBattVolt(uint8_t *data)
{
    unsigned char tmpr[2];
    memset(tmpr, 0, 2);
    // swapBytesAndStore(Sonalika_Data.Hex18fee500,tmp,0,0);
    swapBytesHexArray(data, 6, 7, tmpr);
    appl_param_list.BVL = hexToDecimal(tmpr, 2, 0);
    appl_param_list.BVL = (appl_param_list.BVL * 0.05);
    return appl_param_list.BVL;
}

static int CalculateEngCoolantLevValue(uint8_t* data)
{
	appl_param_list.ECL = hexToDecimal(data, 1, 6);
    appl_param_list.ECL = appl_param_list.ECL * 0.4;
    return appl_param_list.ECL;
}

static float CalculateUreaQuality(uint8_t* data)
{
	appl_param_list.UQL = hexToDecimal(data, 1, 1);
    appl_param_list.UQL = appl_param_list.UQL * 0.25;
    return appl_param_list.UQL;
}

static float CalEngTotalIdelFuelUsed(uint8_t *data)
{
    unsigned char tmpr[4];
    memset(tmpr, 0, 4);
    // swapBytesAndStore(Sonalika_Data.Hex18fee500,tmp,0,0);
    swapBytesHexArray(data, 0, 3, tmpr);
    appl_param_list.Engine_total_idle_fuel_used = hexToDecimal(tmpr, 4, 0);
    appl_param_list.Engine_total_idle_fuel_used = appl_param_list.Engine_total_idle_fuel_used * 0.5;
    return appl_param_list.Engine_total_idle_fuel_used;
}

static float CalEngTotalIdelHoursValue(uint8_t *data)
{
    unsigned char tmpr[4];
    memset(tmpr, 0, 4);
    // swapBytesAndStore(Sonalika_Data.Hex18fee500,tmp,0,0);
    swapBytesHexArray(data, 4, 7, tmpr);
    appl_param_list.Engine_total_time_idle_hours = hexToDecimal(tmpr, 4, 0);
    appl_param_list.Engine_total_time_idle_hours = appl_param_list.Engine_total_time_idle_hours * 0.05;
    return appl_param_list.Engine_total_time_idle_hours;
}

char * CalculateHex18FD7C00(uint8_t* data)
{
	hexArrayToHexString(data,appl_param_list.Hex18FD7C00);
	return appl_param_list.Hex18FD7C00;
}

static int CalAfterTreatmentTemperature(uint8_t* data)
{
	appl_param_list.AftertreatmentTempreature = hexToDecimal(data, 1, 1);
	if(rx_can_flag == 1)
	{
       appl_param_list.AftertreatmentTempreature = appl_param_list.AftertreatmentTempreature - 40;
	}
    return appl_param_list.AftertreatmentTempreature;
}

static float CalAfterTreatmentFluidLev(uint8_t* data)
{
	unsigned char tmpr[2];
    memset(tmpr, 0, 2);
	// swapBytesAndStore(Sonalika_Data.Hex18fee500,tmp,0,0);
	swapBytesHexArray(data, 2, 3, tmpr);
	appl_param_list.AftertreatmentFluidLevel = hexToDecimal(tmpr, 2, 0);
    appl_param_list.AftertreatmentFluidLevel = appl_param_list.AftertreatmentFluidLevel * 0.1;
    return appl_param_list.AftertreatmentFluidLevel;
}

static float Catalystupstreamtemperature(uint8_t* data)
{
	unsigned char tmpr[2];
    memset(tmpr, 0, 2);
	// swapBytesAndStore(Sonalika_Data.Hex18fee500,tmp,0,0);
	swapBytesHexArray(data, 4, 5, tmpr);
	appl_param_list.Catalyst_upstreamtemperature = hexToDecimal(tmpr, 2, 0);
    appl_param_list.Catalyst_upstreamtemperature = (appl_param_list.Catalyst_upstreamtemperature * 0.1) - 40;
    return appl_param_list.Catalyst_upstreamtemperature;
}

static int CalIntakeAirtempretaure(uint8_t* data)
{
	appl_param_list.IAT = hexToDecimal(data, 1, 5);
	if(rx_can_flag == 1)
	{
	   appl_param_list.IAT = appl_param_list.IAT - 40;
	}
    return appl_param_list.IAT;
}

static float Calculate_HRLFC_value(uint8_t *data)
{
	unsigned char tmpr[4];
	memset(tmpr, 0, 4);
	// swapBytesAndStore(Sonalika_Data.Hex18fee500,tmp,0,0);
	swapBytesHexArray(data, 4, 7, tmpr);
	appl_param_list.HRLFC = hexToDecimal(tmpr, 4, 0);
	appl_param_list.HRLFC = appl_param_list.HRLFC * 0.001;
	return appl_param_list.HRLFC;
}

static int Calculate_Fan_Speed(uint8_t* data)
{
	unsigned char tmpr[2];
	memset(tmpr, 0, 2);
	// swapBytesAndStore(Sonalika_Data.Hex18fee500,tmp,0,0);
	swapBytesHexArray(data, 2, 3, tmpr);
	appl_param_list.Fan_speed = hexToDecimal(tmpr, 2, 0);
	appl_param_list.Fan_speed = (appl_param_list.Fan_speed * 0.125);
	return appl_param_list.Fan_speed;

}

static int Calculate_pedal_position(uint8_t* data)
{
	unsigned char tmpr[2];
	memset(tmpr, 0, 2);
	// swapBytesAndStore(Sonalika_Data.Hex18fee500,tmp,0,0);
	swapBytesHexArray(data, 2, 3, tmpr);
	appl_param_list.Fan_speed = hexToDecimal(tmpr, 2, 0);
	appl_param_list.Fan_speed = (appl_param_list.Fan_speed * 0.125);
	return appl_param_list.Fan_speed;

}

static int CalAccelPedalPos(uint8_t *data)
{
    float value;
    value = hexToDecimal(data, 1, 1);
    value = value * 0.4;
    return value;
}

static uint8_t ActualEnginePercentTorque(uint8_t* data)
{
	appl_param_list.ActualEnginePercentTorque = hexToDecimal(data, 1, 2);
    appl_param_list.ActualEnginePercentTorque = appl_param_list.ActualEnginePercentTorque - 125;
    return appl_param_list.ActualEnginePercentTorque;
}

static uint8_t EngineDemandPercentTorque(uint8_t* data)
{
	appl_param_list.EngineDemandPercentTorque = hexToDecimal(data, 1, 7);
    appl_param_list.EngineDemandPercentTorque = appl_param_list.EngineDemandPercentTorque - 125;
    return appl_param_list.EngineDemandPercentTorque;
}

char * initRTC(void)
{
	memset(utcString, 0, 30);
    sprintf(utcString, "%04u-%02u-%02uT%02u:%02u:%02u", appl_param_list.Year, appl_param_list.Month,
		   appl_param_list.Day, appl_param_list.Hour,appl_param_list.Min, appl_param_list.Sec);
    return utcString;
}

char *get_vehicle_data_JSON_string(void)
{
    char tmp[50];

    /* make empty string */
    json_data[0] = '\0';
    memset(json_data, 0, 255);
    /* check if herder present */

    strcat(json_data, "{");

        ADD_STRING(json_data,UID)
		ADD_MORE(json_data,ADD_JSON_ELEMENT_U8(json_data, appl_param_list.ELD))
        ADD_MORE(json_data, ADD_JSON_ELEMENT_FLOAT(json_data, appl_param_list.RPM))
        ADD_MORE(json_data, ADD_JSON_ELEMENT_U8(json_data, appl_param_list.SPD))
        ADD_MORE(json_data, ADD_JSON_ELEMENT_U8(json_data, appl_param_list.CTM))
        ADD_MORE(json_data, ADD_JSON_ELEMENT_U8(json_data, appl_param_list.EngineOilPressure))

    strcat(json_data, "}");
    return json_data;
}

int GetEngineStatus(void)
{
	if(appl_param_list.RPM > 100)
	{
		return ON;
	}
  return OFF;
}

void reset_application_parameter_list(void)
{
    memset(&appl_param_list, 0, sizeof(appl_param_list));
}
