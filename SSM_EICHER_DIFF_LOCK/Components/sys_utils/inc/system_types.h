/**
 * system_types.h
 *
 *  Created on: Oct 10th, 2023
 *      Author:
 *
 * @brief:  Defination of the avalible std_types
 */

#ifndef _SYSTEM_TYPES_H
#define _SYSTEM_TYPES_H

#include <stdint.h>
// #include "stm32g0xx_hal.h"
// #include "cmsis_os.h"

/************************************************
 * Device All Task config and Priority Grouping
 * **********************************************/

/* CIMSIS base Priority state */
#define CMSIS_OS_PRIOR_NORMAL 24

/* Module RP_tp Task Schedules */
#define IMU_RUNTIME_TASK_PRIORITY (CMSIS_OS_PRIOR_NORMAL)
#define IMU_RUNTIME_CORE_TASK_STACK (1024)


#define CAN_RX_TASK_PRIORITY (CMSIS_OS_PRIOR_NORMAL)
#define CAN_SERVICE_TASK_PRIORITY (CMSIS_OS_PRIOR_NORMAL)

#define PULSE_RUNTIME_TASK_PRIORITY (CMSIS_OS_PRIOR_NORMAL + 2)
#define COM_TX_TASK_STACK_SIZE (1024)


#define SYSTEM_PARANS_UPDATE_TASK_PRIORITY (CMSIS_OS_PRIOR_NORMAL)
#define SYSTEM_PARANS_UPDATE_STACK_SIZE (1024)


/************************************************
 * Device typedefs
 * **********************************************/
typedef uint8_t U8;
typedef uint16_t U16;
typedef uint32_t U32;
typedef uint64_t U64;
typedef int8_t I8;

typedef uint8_t *pU8;
typedef uint16_t *pU16;
typedef uint32_t *pU32;
typedef uint64_t *pU64;

#endif /* _SYSTEM_TYPES_H */
