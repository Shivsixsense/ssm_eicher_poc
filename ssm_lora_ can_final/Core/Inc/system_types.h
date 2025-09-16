/**
 * system_types.h
 *
 *  Created on: Oct 10th, 2023
 *      Author: 
 * 
 * @brief:  Defination of the avalible std_types
 */
#pragma once
#include <stdint.h>
//#include "system_logging.h"
#include "system_util.h"

/************************************************
 * Device All Task config and Priority Grouping
 * **********************************************/

/* CIMSIS base Priority state */
#define CMSIS_OS_PRIOR_NORMAL 24

/* Appl Device Main tasks Schedules */
#define APPL_MAIN_TASK_PRIORITY (CMSIS_OS_PRIOR_NORMAL)

/* Module CAN Interface Task Schedules */
#define CAN_RX_TASK_PRIORITY (CMSIS_OS_PRIOR_NORMAL)

/* Module OBD2 Application Task Schedules */
#define CAN_SERVICE_TASK_PRIORITY (CMSIS_OS_PRIOR_NORMAL)

/* Module IMU runtime Task Schedules */
#define IMU_RUNTIME_TASK_PRIORITY (CMSIS_OS_PRIOR_NORMAL)

/* Module CAN runtime Task Schedules */
#define CAN_RUNTIME_TASK_PRIORITY (CMSIS_OS_PRIOR_NORMAL)

#define PULSE_RUNTIME_TASK_PRIORITY (CMSIS_OS_PRIOR_NORMAL)


/************************************************
 * Device typedefs
 * **********************************************/
typedef uint8_t         U8;
typedef uint16_t        U16;
typedef uint32_t        U32;
typedef uint64_t        U64;
typedef int8_t          I8;

typedef uint8_t*        pU8;
typedef uint16_t*       pU16;
typedef uint32_t*       pU32;
typedef uint64_t*       pU64;
