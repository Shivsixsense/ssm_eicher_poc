/*
 * card.c
 *
 *  Created on: Jul 8, 2024
 * 1     Author: apple
 */
#include "card.h"
#include "stdio.h"
#include "sys_log.h"
#include "cmsis_os.h"
#include <card_uart_interface.h>
#include <string.h>
#include "stdbool.h"
#include "com_interface.h"
#include"system_util.h"
#include "flash.h"
/* Marco Defs ****************************************************************
 *****************************************************************************/


#define BYTE_CHECK_SOF(x) ((x & 0x01U) == 0U)
#define GET_FRAME_SIZE_FROM_RAW(x) ((x >> 1U) & 0x7FU)

#define START_BYTE 0x7E
#define END_BYTE   0x7F
#define MAX_DATA_SIZE 256

/* Private Types **************************************************************
 *****************************************************************************/

typedef struct card_uart_data_hadler_info_t
{
    U8 start_of_frame;
    U8 data_index;
    U8 data[150];
    U8 incoming_frame_len;
} card_uart_data_hadler_info;


/*  Local Variables ***********************************************************
 *****************************************************************************/
card_uart_data_hadler_info card_raw_data_info;
Frame_t Frame;
uint32_t card_com_timer = 0;
U8 runtime_src_id = 0;

extern uint64_t IAD_state, IWD_state, Cur_IAD , Cur_IWD;

extern uint64_t Speed1, SteerAngle1;


extern uint64_t Kmph1,EngTor1, PadelPos1, Break1, TimeTor1, TimeIAWD1;

extern uint64_t  Kmph2, EngTor2, PadelPos2, Break2, TimeTor2, TimeIAWD2;
extern uint64_t  Kmph3, EngTor3, PadelPos3, Break3, TimeTor3, TimeIAWD3;
extern uint64_t ConditionValueOfHours, ThreeHourFlag, TimeOfAction, RestTimeOfRelay, FilterClearafterMins;

extern double FilterClearafterHours;
/* Local Functions Prototypes ************************************************
 *****************************************************************************/
static void card_idle_monitor_task(void *args);
int parse_frame(const uint8_t *frame_buffer,  Frame_t *frame);
void handle_variable_by_address(uint8_t address, uint16_t data);
/* Exported Functions ********************************************************
 *****************************************************************************/

iot_card_status iotCard_init_module(void)
{
    if (xTaskCreate(card_idle_monitor_task, "card_idle_monitor_task",
                    1024, NULL, 24, NULL) != pdPASS)
    {
        return IOT_CARD_STATUS_NOK;
    }

    return IOT_CARD_STATUS_OK;
}

/** @brief Parser for RAW Bytes received from UART
 * @return iot_card_status
 */
iot_card_status iotCard_parse_raw_data(uint8_t* data)
{
    iot_card_status ret = IOT_CARD_STATUS_OK;

	/* parse the received frame from UART */
	if(parse_frame(data , &Frame) == IOT_CARD_STATUS_OK)
	{
		handle_variable_by_address(Frame.address,Frame.data);
	}
//	memset(card_raw_data_info.data, 0, 150);

    return ret;
}


/* Local Tasks**** ***********************************************************
 *****************************************************************************/

/** @brief card_idle_monitor_task
 * @param args : * void (NULL)
 */
static void card_idle_monitor_task(void *args)
{

#ifdef RTOS_ANALYZE_STACK_USAGE
    /* Inspect high water mark on entering the task. */
    UBaseType_t uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
    print_logln("STACK1: <%s> [%lu]", __func__, uxHighWaterMark);
#endif

    for (;;)
    {
        if(card_raw_data_info.start_of_frame == true)
        {

          if (IsTimerElapsed(card_com_timer))
          {
             /* send nac for previous frame */

             /* wait for next SOF */
             card_raw_data_info.start_of_frame = false;
          }

          vTaskDelay(pdMS_TO_TICKS(100));
        }
    }

    vTaskDelete(NULL);
}

/* Local Functions ***********************************************************
 *****************************************************************************/

int parse_frame(const uint8_t *frame_buffer,  Frame_t *frame) {

    // Parse fixed fields

	uint16_t RawData = 0;
    frame->start_byte = frame_buffer[0];
    frame->dest_id = frame_buffer[1];
    frame->src_id = frame_buffer[2];
    frame->address = frame_buffer[3];
    frame->data_length = frame_buffer[4];


    memcpy(&RawData, frame_buffer + 5, frame->data_length);

    frame->data = swap_bytes(RawData);
    frame->checksum = frame_buffer[7];
    frame->end_byte = frame_buffer[8];

    // Validate start and end bytes
    if (frame->start_byte != START_BYTE || frame->end_byte != END_BYTE)
	{
        print_logln("Error: invalid start/end byte");
	    return IOT_CARD_STATUS_NOK; // Error: invalid start/end byte
	}


    if(frame->checksum != 8)
    {
        print_logln("Error: checksum mismatch");
    	return IOT_CARD_STATUS_NOK; // Error: checksum mismatch
    }

    return IOT_CARD_STATUS_OK; // Success
}


void handle_variable_by_address(uint8_t address, uint16_t data)
{
    switch (address)
    {
        case 0x00:
        {
             IAD_state = data;
 			 Flash_Write_Data(FLASH_USER_START_ADDR + 2048 * 5, &IAD_state, 1);
             print_logln("Address: 0x00, Variable: IAD_state, Value: %d\n", data);
             break;
        }
        case 0x01:
        {
            IWD_state = data;
			 Flash_Write_Data(FLASH_USER_START_ADDR + 2048 * 6, &IWD_state, 1);
            print_logln("Address: 0x01, Variable: IWD_state, Value: %d\n", data);
            break;
        }
        case 0x02:
        {
            Cur_IAD = data;
			 Flash_Write_Data(FLASH_USER_START_ADDR + 2048 * 7, &Cur_IAD, 1);

            print_logln("Address: 0x02, Variable: Cur_IAD, Value: %d\n", data);
            break;
        }
        case 0x03:
        {
           Cur_IWD = data;
			 Flash_Write_Data(FLASH_USER_START_ADDR + 2048 * 8, &Cur_IWD, 1);

           print_logln("Address: 0x03, Variable: Cur_IWD, Value: %d\n", data);
           break;
        }
        case 0x04:
        {
             Speed1 = data;
 			 Flash_Write_Data(FLASH_USER_START_ADDR + 2048 * 9, &Speed1, 1);

             print_logln("Address: 0x04, Variable: Speed1, Value: %d\n", data);
             break;
        }
        case 0x05:
        {
            SteerAngle1 = data;
			 Flash_Write_Data(FLASH_USER_START_ADDR + 2048 * 10, &SteerAngle1, 1);

            print_logln("Address: 0x05, Variable: SteerAngle1, Value: %d\n", data);
            break;
        }
        case 0x06:
        {
            Kmph1 = data;
		    Flash_Write_Data(FLASH_USER_START_ADDR + 2048 * 11, &Kmph1, 1);

            print_logln("Address: 0x06, Variable: Kmph1, Value: %d\n", data);
            break;
        }
        case 0x07:
        {
            EngTor1 = data;
			 Flash_Write_Data(FLASH_USER_START_ADDR + 2048 * 12, &EngTor1, 1);

            print_logln("Address: 0x07, Variable: EngTor1, Value: %d\n", data);
            break;
        }
        case 0x08:
        {
            PadelPos1 = data;
			 Flash_Write_Data(FLASH_USER_START_ADDR + 2048 * 13, &PadelPos1, 1);

            print_logln("Address: 0x08, Variable: PadelPos1, Value: %d\n", data);
            break;
        }
        case 0x09:
        {
            Break1 = data;
			Flash_Write_Data(FLASH_USER_START_ADDR + 2048 * 14, &Break1, 1);

            print_logln("Address: 0x09, Variable: Break1, Value: %d\n", data);
            break;
        }
        case 0x0A:
        {
            TimeTor1 = data;
			 Flash_Write_Data(FLASH_USER_START_ADDR + 2048 * 15, &TimeTor1, 1);

            print_logln("Address: 0x0A, Variable: TimeTor1, Value: %d\n", data);
            break;
        }
        case 0x0B:
        {
            TimeIAWD1 = data;
			 Flash_Write_Data(FLASH_USER_START_ADDR + 2048 * 16, &TimeIAWD1, 1);

            print_logln("Address: 0x0B, Variable: TimeIAWD1, Value: %d\n", data);
            break;
        }
        case 0x0C:
        {
            Kmph2 = data;
			 Flash_Write_Data(FLASH_USER_START_ADDR + 2048 * 17, &Kmph2, 1);

            print_logln("Address: 0x0C, Variable: Kmph2, Value: %d\n", data);
            break;
        }
        case 0x0D:
        {
            EngTor2 = data;
			 Flash_Write_Data(FLASH_USER_START_ADDR + 2048 * 18, &EngTor2, 1);

            print_logln("Address: 0x0D, Variable: EngTor2, Value: %d\n", data);
            break;
        }
        case 0x0E:
        {
            PadelPos2 = data;
			 Flash_Write_Data(FLASH_USER_START_ADDR + 2048 * 19, &PadelPos2, 1);

            print_logln("Address: 0x0E, Variable: PadelPos2, Value: %d\n", data);
            break;
        }
        case 0x0F:
        {
            Break2 = data;
			 Flash_Write_Data(FLASH_USER_START_ADDR + 2048 * 20, &Break2, 1);

            print_logln("Address: 0x0F, Variable: Break2, Value: %d\n", data);
            break;
        }
        case 0x10:
        {
            TimeTor2 = data;
			 Flash_Write_Data(FLASH_USER_START_ADDR + 2048 * 21, &TimeTor2, 1);

            print_logln("Address: 0x10, Variable: TimeTor2, Value: %d\n", data);
            break;
        }
        case 0x11:
        {
            TimeIAWD2 = data;
			 Flash_Write_Data(FLASH_USER_START_ADDR + 2048 * 22, &TimeIAWD2, 1);

            print_logln("Address: 0x11, Variable: TimeIAWD2, Value: %d\n", data);
            break;
        }
        case 0x12:
        {
            Kmph3 = data;
			 Flash_Write_Data(FLASH_USER_START_ADDR + 2048 * 23, &Kmph3, 1);

            print_logln("Address: 0x12, Variable: Kmph3, Value: %d\n", data);
            break;
        }
        case 0x13:
        {
            EngTor3 = data;
			 Flash_Write_Data(FLASH_USER_START_ADDR + 2048 * 24, &EngTor3, 1);

            print_logln("Address: 0x13, Variable: EngTor3, Value: %d\n", data);
            break;
        }
        case 0x14:
        {
            PadelPos3 = data;
			 Flash_Write_Data(FLASH_USER_START_ADDR + 2048 * 25, &PadelPos3, 1);

            print_logln("Address: 0x14, Variable: PadelPos3, Value: %d\n", data);
            break;
        }
        case 0x15:
        {
            Break3 = data;
			 Flash_Write_Data(FLASH_USER_START_ADDR + 2048 * 26, &Break3, 1);

            print_logln("Address: 0x15, Variable: Break3, Value: %d\n", data);
            break;
        }
        case 0x16:
        {
            TimeTor3 = data;
			 Flash_Write_Data(FLASH_USER_START_ADDR + 2048 * 27, &TimeTor3, 1);

            print_logln("Address: 0x16, Variable: TimeTor3, Value: %d\n", data);
            break;
        }
        case 0x17:
        {
            TimeIAWD3 = data;
			 Flash_Write_Data(FLASH_USER_START_ADDR + 2048 * 28, &TimeIAWD3, 1);

            print_logln("Address: 0x17, Variable: TimeIAWD3, Value: %d\n", data);
            break;
        }

        case 0x18:
		{
			ThreeHourFlag = data;
			Flash_Write_Data(FLASH_USER_START_ADDR,&ThreeHourFlag, 1);
            print_logln("Address: 0x18, Variable: ThreeHourFlag, Value: %d\n", data);


		   break;
		}

		case 0x19:
		{
			TimeOfAction = data;
			Flash_Write_Data(FLASH_USER_START_ADDR + 2048,&TimeOfAction, 1);
            print_logln("Address: 0x19, Variable: TimeOfAction, Value: %d\n", data);

			break;
		}

		case 0x1A:
		{
			RestTimeOfRelay = data;
			Flash_Write_Data(FLASH_USER_START_ADDR + 2048 * 2,&RestTimeOfRelay, 1);
            print_logln("Address: 0x1A, Variable: RestTimeOfRelay, Value: %d\n", data);

			break;
		}

		case 0x1B:
		{
			FilterClearafterMins = data ;
			FilterClearafterHours = FilterClearafterMins * 0.0167;
			Flash_Write_Data(FLASH_USER_START_ADDR + 2048 * 3,&FilterClearafterMins, 1);
            print_logln("Address: 0x1B, Variable: FilterClearafterMins, Value: %d\n", data);

			break;
		}

		case 0x1C:
		{
			ConditionValueOfHours = data;
			Flash_Write_Data(FLASH_USER_START_ADDR + 2048 * 4,&ConditionValueOfHours, 1);
            print_logln("Address: 0x1C, Variable: ConditionValueOfHours, Value: %d\n", data);

			break;
		}

        default:
        {
            printf("Invalid address: 0x%02X\n", address);
            break;
        }
    }
}

