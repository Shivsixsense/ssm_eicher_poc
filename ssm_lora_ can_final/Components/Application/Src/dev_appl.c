#include "dev_appl.h"
#include "FreeRTOS.h"
#include "task.h"
#include "lora_com.h"
#include "lora_com.h"
#include "storage.h"
#include "eicher.h"
#include "can_interface.h"
/******************************************************************************
 * Marco Defs
 *****************************************************************************/

/* Debug Log tag */
#define TAG "APPL "


/* Debug Log tag */
#define TAG "APPL "

#define dev_vehicle_params_file_name "/dev_vehicle_params.tmp"
#define non_vehicle_params_file_name "/non_vehicle_params.tmp"
#define vehicle_health_params_file_name "/health_params.tmp"
#define trip_data_file_name "/trip_data.tmp"

/******************************************************************************
 * Type Defs
 *****************************************************************************/


/******************************************************************************
 * Global variables
 *****************************************************************************/
extern uint8_t lora_ready;
extern uint8_t Just_OFF;
extern uint8_t two_sec,two_sec2;
extern uint16_t post_period_handler;
extern float Ax,Ay,Az;
extern IWDG_HandleTypeDef hiwdg;
/******************************************************************************
 * Local variables
 *****************************************************************************/

const char engine_status_str[4][10] = {
    "ON",
    "OFF",
    "PARKED",
    "LOCKED"
};


const char batt_status_str[2][10] = {
    "AUXILIARY",
    "PRIMARY",
};

/* dev_appl_rutime_info_t */
typedef struct dev_appl_rutime_info_t
{
    char VehicleParamsStr[1024];
    //char NonVehicleParamsStr[512];
    char VehicleHealthParams[1024];
    char JSON_Str[1024];
    char trip_str[200];
} dev_appl_rutime_info;


const char content_type[] = "application/vnd.kafka.json.v2+json\"" ;

//char msg[ ] = "{SSM005,792,85,24,16585,7567,12.5,53,35,52,55,95999,76.7,8081356556867688,5676,10000,75.865467,31.765465,2025-01-10T05:49:59}";

uint8_t msg[255];

uint8_t buffer[256];
char rcv_buffer[256];
char vehicle_data[255];
char utcString[30];
uint8_t trip_start , trip_end;
size_t offset = 0;
dev_appl_rutime_info JsonStr;
uint32_t sometime_frame_cntr = 0;
uint8_t just_off = 1;
uint8_t OverSpeed = 0, car_on_flag = 0;
uint8_t off_cnt = 0;
extern Ecu_appl_parameter_list_t appl_param_list;

/******************************************************************************
 * LOCAL FUNCTION PROTOTYPES
 *****************************************************************************/
 int parse_data(char* inBuffer, char* outBuffer, int pos,const char* delimiter);
 int hex_string_to_bytes(const char *hex_string, uint8_t *byte_array, size_t max_bytes);
 void parse_ecu_data(uint8_t *buffer, size_t *offset, Ecu_appl_parameter_list_t *data);
 static uint8_t hex_char_to_int(char c);
 void _lora_communication_handler(void);
/******************************************************************************
 * GLOBAL FUNCTION DEFINATIONS
 ******************************************************************************/



/**
 * @brief start device application
 * @retval 0 OK. 1 NOT_OK
 */

void StartDefaultTask(void *argument)
{

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
    init_lora_module();
	initialise_can_interface();
    initialise_eicher_task();
    for (;;)
    {
    //	initRTC();
    	HAL_IWDG_Refresh(&hiwdg);
    	if(GetEngineStatus()== ON)
    	{
    	   lora_send_packet((uint8_t*)get_vehicle_data_JSON_string(),strlen(get_vehicle_data_JSON_string()));
    	}
    	//_lora_communication_handler();
   		vTaskDelay(pdMS_TO_TICKS(3000));
    }

    /* Unexpected exit of task */
    vTaskDelete(NULL);
}


// Convert a single hex char (0-9, A-F) to an integer (0-15)
static uint8_t hex_char_to_int(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return 0;  // Return 0 for invalid input (avoid undefined behavior)
}

// Convert hex string to byte array safely
int hex_string_to_bytes(const char *hex_string, uint8_t *byte_array, size_t max_bytes) {
    size_t hex_length = strlen(hex_string);

    // Ensure the string has an even length
    if (hex_length % 2 != 0) {
        return -1; // Error: Invalid length or too large for buffer
    }

    for (size_t i = 0; i < hex_length / 2; i++) {
        char high = hex_string[i * 2];
        char low  = hex_string[i * 2 + 1];

        if (!((high >= '0' && high <= '9') || (high >= 'A' && high <= 'F') || (high >= 'a' && high <= 'f')) ||
            !((low >= '0' && low <= '9') || (low >= 'A' && low <= 'F') || (low >= 'a' && low <= 'f'))) {
            return -2; // Error: Invalid hex character detected
        }

        byte_array[i] = (hex_char_to_int(high) << 4) | hex_char_to_int(low);
    }

    return hex_length / 2; // Return number of bytes converted
}


void parse_ecu_data(uint8_t *buffer, size_t *offset, Ecu_appl_parameter_list_t *data) {
//    memcpy(data->lora_id, &buffer[*offset], 10);
//    *offset += 10 + 1; // Skip separator
//
//    memcpy(&data->lattitude, &buffer[*offset], sizeof(float));
//    *offset += sizeof(float) + 1;
//
//    memcpy(&data->longitude, &buffer[*offset], sizeof(float));
//    *offset += sizeof(float) + 1;

    memcpy(&data->RPM, &buffer[*offset], sizeof(float));
    *offset += sizeof(float) + 1;

//    memcpy(&data->ELD, &buffer[*offset], sizeof(uint8_t));
//    *offset += sizeof(uint8_t) + 1;

    memcpy(&data->CTM, &buffer[*offset], sizeof(int));
    *offset += sizeof(int) + 1;

    memcpy(&data->Hours, &buffer[*offset], sizeof(float));
    *offset += sizeof(float) + 1;

    memcpy(&data->SPD, &buffer[*offset], sizeof(float));
    *offset += sizeof(float) + 1;

    memcpy(&data->TotolVehicleDistane, &buffer[*offset], sizeof(float));
    *offset += sizeof(float) + 1;

    memcpy(&data->FLV, &buffer[*offset], sizeof(uint8_t));
    *offset += sizeof(uint8_t) + 1;

    memcpy(&data->EngineOilPressure, &buffer[*offset], sizeof(uint16_t));
    *offset += sizeof(uint16_t) + 1;

//    memcpy(&data->WaterInFuelIndicator, &buffer[*offset], sizeof(uint8_t));
//    *offset += sizeof(uint8_t) + 1;

    memcpy(&data->IAT, &buffer[*offset], sizeof(int));
    *offset += sizeof(int) + 1;

//    memcpy(&data->AftertreatmentFluidLevel, &buffer[*offset], sizeof(float));
//    *offset += sizeof(float) + 1;

//    memcpy(&data->BVL, &buffer[*offset], sizeof(float));
//    *offset += sizeof(float) + 1;

    memcpy(&data->Engine_total_idle_fuel_used, &buffer[*offset], sizeof(float));
    *offset += sizeof(float) + 1;

    memcpy(&data->Engine_total_time_idle_hours, &buffer[*offset], sizeof(float));
    *offset += sizeof(float) + 1;

    memcpy(&data->HRLFC, &buffer[*offset], sizeof(float));
    *offset += sizeof(float) + 1;

//    memcpy(&Ax, &buffer[*offset], sizeof(float));
//    *offset += sizeof(float) + 1;
//
//    memcpy(&Ay, &buffer[*offset], sizeof(float));
//    *offset += sizeof(float) + 1;
//
//    memcpy(&Az, &buffer[*offset], sizeof(float));
//    *offset += sizeof(float) + 1;

//    memcpy(&data->unix_timestamp, &buffer[*offset], sizeof(uint32_t));
//    *offset += sizeof(uint32_t) + 1;
//
//    // Convert Unix timestamp to UTC string
//    struct tm *tm_info = gmtime((time_t *)&data->unix_timestamp);
//    strftime(utcString, sizeof(utcString), "%Y-%m-%dT%H:%M:%S", tm_info);
}


void _lora_communication_handler(void)
{

}
