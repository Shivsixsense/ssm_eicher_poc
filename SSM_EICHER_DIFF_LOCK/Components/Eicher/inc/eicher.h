/*
 * eicher.h
 *
 *  Created on: Jan 13, 2025
 *      Author: shiv
 */

#ifndef EICHER_INC_EICHER_H_
#define EICHER_INC_EICHER_H_

extern xQueueHandle CanTxQueue;

/* obd2 application parameter list */
typedef struct
{
    U16 SPD;

    float accel_prdal_pos;

    float RelRAxl1Spd_L;

    float RelRAxl1Spd_R;

    float RelRAxl2Spd_L;

    float RelRAxl2Spd_R;

    float Steering_angle;

    uint8_t cluchsw;

    uint8_t breakSw;

    float Hrs;

    uint8_t key_position;

} Ecu_appl_parameter_list_t;


typedef enum engine_status_types_e
{
    ON,
	OFF,
	ENGINE_STATUS_LEVEL_MAX
} engine_status_types_e;

void reset_application_parameter_list(void);
int initialise_eicher_task(void);
uint32_t GetHourValue(void);

int rte_send_can_message_to_queue(void *canFrame, U32 waitTimeout);

#endif /* EICHER_INC_EICHER_H_ */
