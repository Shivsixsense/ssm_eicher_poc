/*
 * eicher.h
 *
 *  Created on: Jan 13, 2025
 *      Author: shiv
 */

#ifndef EICHER_INC_EICHER_H_
#define EICHER_INC_EICHER_H_
#include "main.h"


/* obd2 application parameter list */
typedef struct
{
	float lattitude;

	float longitude;

	float RPM;

    U8 ELD;

	U8 ECL;

	U8 Sec;

	U8 Min;

	U8 Hour;

	U8 Month;

	U8 Day;

	U8 Year;

	U8 MinOffset;

	U8 HourOffset;

    int CTM;

    float Hours;

    uint8_t SPD;

    float TotolVehicleDistane;

    U8 FLV;

    U16 EngineOilPressure;

    float BVL;

    float UQL;

    int AftertreatmentTempreature;

    float AftertreatmentFluidLevel;

    float Catalyst_upstreamtemperature;

    U8 WaterInFuelIndicator;

    int IAT;

    float Engine_total_idle_fuel_used;

    float Engine_total_time_idle_hours;

    char Hex18FD7C00[16];

    float HRLFC;

    float Fan_speed;

    int accel_prdal_pos;

    int ActualEnginePercentTorque;

    int EngineDemandPercentTorque;
} Ecu_appl_parameter_list_t;


typedef enum engine_status_types_e
{
    ON,
	OFF,
	ENGINE_STATUS_LEVEL_MAX
} engine_status_types_e;

int initialise_eicher_task(void);
char *get_vehicle_data_JSON_string(void);
void reset_application_parameter_list(void);

#endif /* EICHER_INC_EICHER_H_ */
