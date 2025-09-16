#include <card_uart_interface.h>
#include "card.h"
#include "com_interface.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

#include "system_types.h"
#include "system_util.h"
#include "sys_log.h"
//#include "usbd_cdc_if.h"

#define TAG "COM: "

/* variables */
static SemaphoreHandle_t tx_semphore;
static uint8_t Inbuff[256];

static void com_rx_frame_task(void *args);

void card_init_com_interface(void)
{
    init_uart();

    tx_semphore = xSemaphoreCreateMutex();

    if (tx_semphore == NULL)
    {
        print_logln(TAG "Semaphore not created");
        return;
    }

    if (xTaskCreate(com_rx_frame_task, "com_rx_frame_task",
                    2048, NULL, 25, NULL) != pdPASS)
    {
        return;
    }
}

int card_com_send_reponse(uint8_t *pData, uint16_t len)
{
    int ret = 0;

#ifdef MODULE_COM_INTFR_LOGGING
    print_logln(TAG "");
#else

    if (tx_semphore != NULL)
    {
        if (xSemaphoreTake(tx_semphore, (TickType_t)5000) == pdTRUE)
        {
            print_log("[DEV] [");
            for (int i = 0; i < len; i++)
            {
                print_log(" %d", pData[i]);
            }
            print_logln("]");

            ret = uart_write_data(pData, len);
            xSemaphoreGive(tx_semphore);
        }
        else
        {
            print_logln(TAG "Error Tx Busy");
        }
    }

    return ret;
#endif
}

static void com_rx_frame_task(void *args)
{
#ifdef RTOS_ANALYZE_STACK_USAGE
    /* Inspect high water mark on entering the task. */
    UBaseType_t uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
    print_logln("STACK1: <%s> [%lu]", __func__, uxHighWaterMark);
#endif

    for (;;)
    {
        if (usb_recive_data(&Inbuff) == 0)
        {
        	iotCard_parse_raw_data(&Inbuff);
           // print_logln("msg recieved");
        }
        vTaskDelay(pdMS_TO_TICKS((1000)));
    }

    vTaskDelete(NULL);
}
