#include "dev_appl.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "can_interface.h"
#include "eicher.h"
#include "stdlib.h"
#include "string.h"
#include "system_util.h"
#include "sys_log.h"
#include "Eicher.h"
#include "flash.h"
#include "com_interface.h"

/******************************************************************************
 * Marco Defs
 *****************************************************************************/

/* Debug Log tag */
#define TAG "APPL "

/******************************************************************************
 * Type Defs
 *****************************************************************************/
uint8_t CanDetected = 0;
uint8_t IWD_engaged = 0, IAD_engaged = 0;
uint64_t ConditionValueOfHours = 1;
uint64_t ThreeHourFlag = 0;
uint64_t TimeOfAction = 0;
uint64_t RestTimeOfRelay = 0;
float FilterClearafterHours;
uint64_t FilterClearafterMins = 0;

uint8_t con_data = 0;
uint64_t IAD_state=1, IWD_state=1, Cur_IAD=0 , Cur_IWD=0;

uint64_t Speed1 = 4, SteerAngle1 = 90;

uint64_t Kmph1=3,EngTor1=40, PadelPos1=10, Break1=0, TimeTor1=3, TimeIAWD1=5;

uint64_t  Kmph2=3, EngTor2=40, PadelPos2=10, Break2=0, TimeTor2=3, TimeIAWD2=5;
uint64_t  Kmph3=3, EngTor3=40, PadelPos3=10, Break3=0 , TimeTor3=3, TimeIAWD3=5;

uint8_t Engine_data1[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        Engine_data2[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        Engine_data3[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

uint64_t flash_save_flag = 0;

/******************************************************************************
 * Global variables
 *****************************************************************************/
extern uint8_t lora_ready;
extern uint8_t Just_OFF;
extern Ecu_appl_parameter_list_t appl_param_list;
/******************************************************************************
 * Local variables
 *****************************************************************************/

/******************************************************************************
 * LOCAL FUNCTION PROTOTYPES
 *****************************************************************************/
void condition1(void);
void condition2(void);
void condition3(void);
void consition5(void);
void SignalIAD(int Action);
void SignalIWD(int Action);
void check_vehicle_condition(void);
void check_air_filter_status(void);
int airfilter_init_module(void);
static void airfilter_monitor_task(void *args);
/******************************************************************************
 * GLOBAL FUNCTION DEFINATIONS
 ******************************************************************************/


int airfilter_init_module(void)
{
    if (xTaskCreate(airfilter_monitor_task, "card_idle_monitor_task",
                    1024, NULL, 24, NULL) != pdPASS)
    {
        return 0;
    }

    return 1;
}

/**
 * @brief start device application
 * @retval 0 OK. 1 NOT_OK
 */

void StartDefaultTask(void *argument)
{
    Flash_Read_Data(FLASH_USER_START_ADDR +  2048 * 29, &flash_save_flag, 1);

    if(flash_save_flag != 1)
    {
    	Flash_Write_Data(FLASH_USER_START_ADDR , &ThreeHourFlag, 1);
    	Flash_Write_Data(FLASH_USER_START_ADDR + 2048 , &TimeOfAction, 1);
    	Flash_Write_Data(FLASH_USER_START_ADDR + 2048 * 2 , &RestTimeOfRelay, 1);
    	Flash_Write_Data(FLASH_USER_START_ADDR + 2048 * 3 , &FilterClearafterMins, 1);
		Flash_Write_Data(FLASH_USER_START_ADDR + 2048 * 5, &IAD_state, 1);
		Flash_Write_Data(FLASH_USER_START_ADDR + 2048 * 6, &IWD_state, 1);
		Flash_Write_Data(FLASH_USER_START_ADDR + 2048 * 7, &Cur_IAD, 1);
		Flash_Write_Data(FLASH_USER_START_ADDR + 2048 * 8, &Cur_IWD, 1);
		Flash_Write_Data(FLASH_USER_START_ADDR + 2048 * 9, &Speed1, 1);
		Flash_Write_Data(FLASH_USER_START_ADDR + 2048 * 10, &SteerAngle1, 1);
		Flash_Write_Data(FLASH_USER_START_ADDR + 2048 * 11, &Kmph1, 1);
		Flash_Write_Data(FLASH_USER_START_ADDR + 2048 * 12, &EngTor1, 1);
		Flash_Write_Data(FLASH_USER_START_ADDR + 2048 * 13, &PadelPos1, 1);
		Flash_Write_Data(FLASH_USER_START_ADDR + 2048 * 14, &Break1, 1);
		Flash_Write_Data(FLASH_USER_START_ADDR + 2048 * 15, &TimeTor1, 1);
		Flash_Write_Data(FLASH_USER_START_ADDR + 2048 * 16, &TimeIAWD1, 1);
		Flash_Write_Data(FLASH_USER_START_ADDR + 2048 * 17, &Kmph2, 1);
		Flash_Write_Data(FLASH_USER_START_ADDR + 2048 * 18, &EngTor2, 1);
		Flash_Write_Data(FLASH_USER_START_ADDR + 2048 * 19, &PadelPos2, 1);
		Flash_Write_Data(FLASH_USER_START_ADDR + 2048 * 21, &TimeTor2, 1);
		Flash_Write_Data(FLASH_USER_START_ADDR + 2048 * 22, &TimeIAWD2, 1);
		Flash_Write_Data(FLASH_USER_START_ADDR + 2048 * 23, &Kmph3, 1);
		Flash_Write_Data(FLASH_USER_START_ADDR + 2048 * 24, &EngTor3, 1);
		Flash_Write_Data(FLASH_USER_START_ADDR + 2048 * 25, &PadelPos3, 1);
		Flash_Write_Data(FLASH_USER_START_ADDR + 2048 * 26, &Break3, 1);
		Flash_Write_Data(FLASH_USER_START_ADDR + 2048 * 27, &TimeTor3, 1);
		Flash_Write_Data(FLASH_USER_START_ADDR + 2048 * 28, &TimeIAWD3, 1);
		flash_save_flag = 1;
		Flash_Write_Data(FLASH_USER_START_ADDR +  2048 * 29, &flash_save_flag, 1);
    }
    Flash_Read_Data(FLASH_USER_START_ADDR , &ThreeHourFlag, 1);
    Flash_Read_Data(FLASH_USER_START_ADDR + 2048 , &TimeOfAction, 1);
    Flash_Read_Data(FLASH_USER_START_ADDR + 2048 * 2 , &RestTimeOfRelay, 1);
    Flash_Read_Data(FLASH_USER_START_ADDR + 2048 * 3 , &FilterClearafterMins, 1);
    FilterClearafterHours = (FilterClearafterMins * 0.0167);
	Flash_Read_Data(FLASH_USER_START_ADDR + 2048 * 5, &IAD_state, 1);
    Flash_Read_Data(FLASH_USER_START_ADDR + 2048 * 6, &IWD_state, 1);
    Flash_Read_Data(FLASH_USER_START_ADDR + 2048 * 7, &Cur_IAD, 1);
	Flash_Read_Data(FLASH_USER_START_ADDR + 2048 * 8, &Cur_IWD, 1);
	Flash_Read_Data(FLASH_USER_START_ADDR + 2048 * 9, &Speed1, 1);
	Flash_Read_Data(FLASH_USER_START_ADDR + 2048 * 10, &SteerAngle1, 1);
	Flash_Read_Data(FLASH_USER_START_ADDR + 2048 * 11, &Kmph1, 1);
	Flash_Read_Data(FLASH_USER_START_ADDR + 2048 * 12, &EngTor1, 1);
	Flash_Read_Data(FLASH_USER_START_ADDR + 2048 * 13, &PadelPos1, 1);
    Flash_Read_Data(FLASH_USER_START_ADDR + 2048 * 14, &Break1, 1);
    Flash_Read_Data(FLASH_USER_START_ADDR + 2048 * 15, &TimeTor1, 1);
	Flash_Read_Data(FLASH_USER_START_ADDR + 2048 * 16, &TimeIAWD1, 1);
	Flash_Read_Data(FLASH_USER_START_ADDR + 2048 * 17, &Kmph2, 1);
	Flash_Read_Data(FLASH_USER_START_ADDR + 2048 * 18, &EngTor2, 1);
	Flash_Read_Data(FLASH_USER_START_ADDR + 2048 * 19, &PadelPos2, 1);
	Flash_Read_Data(FLASH_USER_START_ADDR + 2048 * 21, &TimeTor2, 1);
	Flash_Read_Data(FLASH_USER_START_ADDR + 2048 * 22, &TimeIAWD2, 1);
	Flash_Read_Data(FLASH_USER_START_ADDR + 2048 * 23, &Kmph3, 1);
    Flash_Read_Data(FLASH_USER_START_ADDR + 2048 * 24, &EngTor3, 1);
	Flash_Read_Data(FLASH_USER_START_ADDR + 2048 * 25, &PadelPos3, 1);
	Flash_Read_Data(FLASH_USER_START_ADDR + 2048 * 26, &Break3, 1);
	Flash_Read_Data(FLASH_USER_START_ADDR + 2048 * 27, &TimeTor3, 1);
	Flash_Read_Data(FLASH_USER_START_ADDR + 2048 * 28, &TimeIAWD3, 1);

	airfilter_init_module();
	card_init_com_interface();
	initialise_can_interface();
	initialise_eicher_task();
    for (;;)
    {
      	check_vehicle_condition();
        vTaskDelay(pdMS_TO_TICKS((200)));
    }

    /* Unexpected exit of task */
    vTaskDelete(NULL);
}



static void airfilter_monitor_task(void *args)
{

#ifdef RTOS_ANALYZE_STACK_USAGE
    /* Inspect high water mark on entering the task. */
    UBaseType_t uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
    print_logln("STACK1: <%s> [%lu]", __func__, uxHighWaterMark);
#endif

    for (;;)
    {
    	check_air_filter_status();
        vTaskDelay(pdMS_TO_TICKS(100));
    }

    vTaskDelete(NULL);
}

void check_air_filter_status(void)
{
	if((ThreeHourFlag == 0) && (CanDetected == 1))
	{
		ConditionValueOfHours = GetHourValue() + FilterClearafterHours;

		Flash_Write_Data(FLASH_USER_START_ADDR + 2048 * 4,&ConditionValueOfHours, 1);

		ThreeHourFlag = 1;

		Flash_Write_Data(FLASH_USER_START_ADDR,&ThreeHourFlag, 1);
	}

	if(GetHourValue() >= ConditionValueOfHours)
	{
		uint32_t tmr = 0;
		StartTimer(tmr, TimeOfAction);
		while(IsTimerRunning(tmr)) {
		   HAL_GPIO_WritePin(AIR_INJECTION_LOAD_GPIO_Port, AIR_INJECTION_LOAD_Pin, GPIO_PIN_SET);
		}

		HAL_GPIO_WritePin(AIR_INJECTION_LOAD_GPIO_Port, AIR_INJECTION_LOAD_Pin, GPIO_PIN_RESET);
		StartTimer(tmr, RestTimeOfRelay * 1000);
		while(IsTimerRunning(tmr)) {

		}

		StartTimer(tmr, TimeOfAction);
		while(IsTimerRunning(tmr)) {
			   HAL_GPIO_WritePin(AIR_INJECTION_LOAD_GPIO_Port, AIR_INJECTION_LOAD_Pin, GPIO_PIN_SET);
		}

		HAL_GPIO_WritePin(AIR_INJECTION_LOAD_GPIO_Port, AIR_INJECTION_LOAD_Pin, GPIO_PIN_RESET);
		ThreeHourFlag = 0;
	}
}

void check_vehicle_condition(void)
{
	if(appl_param_list.SPD > 20)
	{
		con_data = 4 ;
	  // if(abs(StAngle) > 180)
	  //   { con_data = 5; }
	}
	else if(HAL_GPIO_ReadPin(GPIOD, HIGH_IN_SENSE_Pin) == GPIO_PIN_RESET || (HAL_GPIO_ReadPin(GPIOD, HIGH_IN_SENSE_Pin) == GPIO_PIN_RESET))
	{
		 con_data = 5;
	}
	else if ( abs(( appl_param_list.RelRAxl1Spd_L + appl_param_list.RelRAxl1Spd_R) - (appl_param_list.RelRAxl2Spd_L + appl_param_list.RelRAxl2Spd_R) ) >= Kmph1 )
	{
		con_data = 1;
		if( abs(appl_param_list.RelRAxl1Spd_L - appl_param_list.RelRAxl1Spd_R) >= Kmph2 ) //&& (abs(StAngle) < SteerAngle2) )
		{
			con_data = 2;
		}
		else if( abs(appl_param_list.RelRAxl2Spd_L - appl_param_list.RelRAxl2Spd_R) >= Kmph3 ) // && (abs(StAngle) < SteerAngle3) )
		{
			con_data = 3;
		}
	}
	else
	{
		con_data = 6;
		SignalIAD(0);
	    SignalIWD(0);
	}

	if(con_data != 6)
	{
		switch (con_data)
		{
			case 1: // Condition 1
			{
			   print_logln("CONDITION 1 : abs(w1+w2 - w3+w4) > x1 Kmph, has met\n");
			   //print_logln("%f,%f,%f,%f,%f,%d\n",appl_param_list.RelRAxl1Spd_L,appl_param_list.RelRAxl1Spd_R,appl_param_list.RelRAxl2Spd_L,appl_param_list.RelRAxl2Spd_R,appl_param_list.SPD);
			   print_logln("Axl1Spd_L :%f\n",appl_param_list.RelRAxl1Spd_L);
			   vTaskDelay(10);
			   print_logln("Axl1Spd_R :%f\n",appl_param_list.RelRAxl1Spd_R);
			   vTaskDelay(10);
			   print_logln("Axl2Spd_L :%f\n",appl_param_list.RelRAxl2Spd_L);
			   vTaskDelay(10);
			   print_logln("Axl2Spd_R :%f\n",appl_param_list.RelRAxl2Spd_R);
			   vTaskDelay(10);
			   print_logln("SPD :%d\n",appl_param_list.SPD);
			   vTaskDelay(10);
			   print_logln("PEDAL POS :%f\n",appl_param_list.accel_prdal_pos);
			   vTaskDelay(10);
			   print_logln("Inter-Axle Difference :%f\n",abs(( appl_param_list.RelRAxl1Spd_L + appl_param_list.RelRAxl1Spd_R) - (appl_param_list.RelRAxl2Spd_L + appl_param_list.RelRAxl2Spd_R)));
			   vTaskDelay(10);
			   condition1();
			   break;
			}

			case 2: // Condition 2
			{
			   print_logln("CONDITION 2 : (abs(w1-w2) > x2 Kmph), has met\n");
			  // print_logln("%f,%f,%f,%f,%f,%d\n",appl_param_list.RelRAxl1Spd_L,appl_param_list.RelRAxl1Spd_R,appl_param_list.RelRAxl2Spd_L,appl_param_list.RelRAxl2Spd_R,appl_param_list.SPD);
			   print_logln("Axl1Spd_L :%f\n",appl_param_list.RelRAxl1Spd_L);
			   vTaskDelay(10);
			   print_logln("Axl1Spd_R :%f\n",appl_param_list.RelRAxl1Spd_R);
			   vTaskDelay(10);
			   print_logln("Axl2Spd_L :%f\n",appl_param_list.RelRAxl2Spd_L);
			   vTaskDelay(10);
			   print_logln("Axl2Spd_R :%f\n",appl_param_list.RelRAxl2Spd_R);
			   vTaskDelay(10);
			   print_logln("SPD :%d\n",appl_param_list.SPD);
			   vTaskDelay(10);
			   print_logln("PEDAL POS :%f\n",appl_param_list.accel_prdal_pos);
			   vTaskDelay(10);
			   print_logln("W1 - W2: %f\n", abs(appl_param_list.RelRAxl1Spd_L - appl_param_list.RelRAxl1Spd_R));
			   vTaskDelay(10);
			   condition2();
			   break;
			}

			case 3: // Condition 3
			{
			   print_logln("CONDITION 3 : (abs(w3-w4) > x3 Kmph) , has met\n");
			   //print_logln("%f,%f,%f,%f,%f,%d\n",appl_param_list.RelRAxl1Spd_L,appl_param_list.RelRAxl1Spd_R,appl_param_list.RelRAxl2Spd_L,appl_param_list.RelRAxl2Spd_R,appl_param_list.SPD);
			   print_logln("Axl1Spd_L :%f\n",appl_param_list.RelRAxl1Spd_L);
			   vTaskDelay(10);
			   print_logln("Axl1Spd_R :%f\n",appl_param_list.RelRAxl1Spd_R);
			   vTaskDelay(10);
			   print_logln("Axl2Spd_L :%f\n",appl_param_list.RelRAxl2Spd_L);
			   vTaskDelay(10);
			   print_logln("Axl2Spd_R :%f\n",appl_param_list.RelRAxl2Spd_R);
			   vTaskDelay(10);
			   print_logln("SPD :%d\n",appl_param_list.SPD);
			   vTaskDelay(10);
			   print_logln("PEDAL POS :%f\n",appl_param_list.accel_prdal_pos);
			   vTaskDelay(10);
			   print_logln("W3 - W4: %f\n", abs(appl_param_list.RelRAxl2Spd_L - appl_param_list.RelRAxl2Spd_R));
			   vTaskDelay(10);
			   condition3();
			   break;
			}

			case 4: // Condition 4
			{
			   print_logln("CONDITION 4: (Vehicle Speed > 20 Kmph), has met.\n");
			  // print_logln("%f,%f,%f,%f,%f,%d\n",appl_param_list.RelRAxl1Spd_L,appl_param_list.RelRAxl1Spd_R,appl_param_list.RelRAxl2Spd_L,appl_param_list.RelRAxl2Spd_R,appl_param_list.SPD);
			   SignalIAD(0);
			   vTaskDelay(10);
			   SignalIWD(0);


			  break;
			}

			case 5:
			{
				print_logln("BUTTON PRESSED\n");
				consition5();
				break;
			}

			default:
			{
			    print_logln("  <<<<0000>>>>> NO CONDITION MET <<<<0000>>>>> \n ");
			   // print_logln("%f,%f,%f,%f,%f,%d\n",appl_param_list.RelRAxl1Spd_L,appl_param_list.RelRAxl1Spd_R,appl_param_list.RelRAxl2Spd_L,appl_param_list.RelRAxl2Spd_R,appl_param_list.SPD);
			    break;
			}
		}
	}
}


void condition1(void)
{
   CAN_User_msg TxFrame;
   if( (appl_param_list.SPD < Speed1) && (appl_param_list.breakSw == Break1) && (appl_param_list.accel_prdal_pos > PadelPos1) )
   {
	  uint32_t tmr = 0;
      StartTimer(tmr, TimeTor1 * 1000);
      print_logln("Sent Engine torque restriction command.\n");
      while(IsTimerRunning(tmr)) {
    	  TxFrame.ExtId = 0xC000003;
    	  Engine_data1[0] = 0x80;
    	  Engine_data1[3] = EngTor1 + 125;
    	  memcpy(TxFrame.data,Engine_data1,sizeof(Engine_data1));
	      rte_send_can_message_to_queue(&TxFrame,1000);
          vTaskDelay(5);
      }
      print_logln("HERE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
      SignalIAD(1);
      vTaskDelay(1000);
      SignalIWD(0);
    vTaskDelay(TimeIAWD1*1000);
  }
  else
  {
      SignalIAD(0);
	  vTaskDelay(1000);
	  SignalIWD(0);
  }
  //{ SignalIAD(0); vTaskDelay(500); SignalIWD(0);}
}

void condition2(void)
{
	CAN_User_msg TxFrame;

    if ( ( appl_param_list.SPD < Speed1 ) && ( appl_param_list.breakSw == Break2 ) && ( appl_param_list.accel_prdal_pos > PadelPos2 ) )
    { // Change torque
	    uint32_t tmr = 0;
	    StartTimer(tmr, TimeTor2 * 1000);
        print_logln("Sent Engine torque restriction command.\n");
        while(IsTimerRunning(tmr)) {
            TxFrame.ExtId = 0xC000003;
            Engine_data1[0] = 0x80;
            Engine_data1[3] = EngTor2 + 125;
          	memcpy(TxFrame.data, Engine_data1, sizeof(Engine_data1));
      	    rte_send_can_message_to_queue(&TxFrame,1000);
            vTaskDelay(5);
      }
        SignalIAD(1);
        vTaskDelay(1000);
        SignalIWD(1);
        vTaskDelay(TimeIAWD2*1000);
  }
  else
  {
	   SignalIAD(0);
	   vTaskDelay(500);
	   SignalIWD(0);
  }
  //{ SignalIAD(0); vTaskDelay(500); SignalIWD(0); }
}

void condition3(void)
{
    CAN_User_msg TxFrame;

    if ( (appl_param_list.SPD < Speed1) && (appl_param_list.breakSw == Break3) && (appl_param_list.accel_prdal_pos > PadelPos3) )
    { // Change torque
	    uint32_t tmr = 0;
	    StartTimer(tmr, TimeTor3 * 1000);
	    print_logln("Sent Engine torque restriction command.\n");
	    while(IsTimerRunning(tmr)) {
	         TxFrame.ExtId = 0xC000003;
	         Engine_data1[0] = 0x80;
	         Engine_data1[3] = EngTor3 + 125;
	         memcpy(TxFrame.data, Engine_data1, sizeof(Engine_data1));
	         rte_send_can_message_to_queue(&TxFrame,1000);
	         vTaskDelay(5);
	    }
        SignalIAD(1);
        vTaskDelay(1000);
        SignalIWD(1);
        vTaskDelay(TimeIAWD3 * 1000);
    }
    else
    {
      SignalIAD(0);
      vTaskDelay(500);
      SignalIWD(0);

    }
  //{ SignalIAD(0); vTaskDelay(500); SignalIWD(0); }
}

void consition5 (void)
{
    CAN_User_msg TxFrame;

	 uint32_t tmr = 0;
	 StartTimer(tmr, TimeTor2 * 1000);
	 print_logln("Sent Engine torque restriction command.\n");
	 while(IsTimerRunning(tmr)) {
		TxFrame.ExtId = 0xC000003;
		Engine_data1[0] = 0x80;
		Engine_data1[3] = EngTor2 + 125;
		memcpy(TxFrame.data, Engine_data1, sizeof(Engine_data1));
		rte_send_can_message_to_queue(&TxFrame,1000);
		vTaskDelay(5);
  }
	 SignalIAD(1);
	 vTaskDelay(1000);
	 SignalIWD(1);
	 vTaskDelay(TimeIAWD2*1000);
}

void SignalIAD(int Action)
{
   if ( Action && IAD_state )
   {
	  Cur_IAD=1;
      //digitalWrite(IAD, HIGH);
	  //HAL_GPIO_WritePin(GPIOA, RELAY_1_Pin, GPIO_PIN_SET);
	  HAL_GPIO_WritePin(GPIOA, IAD_LOAD_Pin, GPIO_PIN_SET);
	  if(IAD_engaged != 1)
	  {
	     IAD_engaged = 1;
         print_logln("  ===> IAD relay has been Engaged.\n");
	  }

   }
   else
   {
	  Cur_IAD=0;
	  //digitalWrite(IAD, LOW);
	  //HAL_GPIO_WritePin(GPIOA, RELAY_1_Pin, GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(GPIOA, IAD_LOAD_Pin, GPIO_PIN_RESET);
	  if(IAD_engaged == 1)
	  {
	 	IAD_engaged = 0;
	    print_logln("  ===> IAD relay has been Dis-Engaged.\n");
	  }
   }
}

void SignalIWD(int Action)
{
   if( Action && IWD_state)
   {
	   Cur_IWD=1;
	   //digitalWrite(IWG, HIGH);
	   //HAL_GPIO_WritePin(GPIOA, RELAY_2_Pin, GPIO_PIN_SET);
	   HAL_GPIO_WritePin(GPIOC, IWD_LOAD_Pin, GPIO_PIN_SET);
	   if(IWD_engaged != 1)
	   {
		  IWD_engaged = 1;
	      print_logln("  ===> IWD relay has been Engaged.\n");
	   }
   }
   else
   {
	   Cur_IWD=0;
	   //digitalWrite(IWG, LOW);
	   HAL_GPIO_WritePin(GPIOC, IWD_LOAD_Pin, GPIO_PIN_RESET);
	   if(IWD_engaged == 1)
	   {
		  IWD_engaged = 0;
	      print_logln("  ===> IWD relay has been Dis-Engaged.\n");
	   }
   }
}

