/*
 * flash.h
 *
 *  Created on: Nov 25, 2024
 *      Author: shiv
 */

#include "stm32g0xx_hal.h"

#ifndef FLASH_INC_FLASH_H_
#define FLASH_INC_FLASH_H_


#define FLASH_USER_START_ADDR   ((uint32_t)0x08070000) // Example address (Change based on memory map)
#define FLASH_USER_END_ADDR     ((uint32_t)0x0807ffff)

uint32_t Flash_Write_Data (uint32_t StartAddress, uint64_t *data, uint32_t length);
void Flash_Read_Data (uint32_t StartAddress, uint64_t *data, uint16_t length);
HAL_StatusTypeDef Flash_WriteString(uint32_t StartAddress,const char *str);
void Flash_ReadString(uint32_t StartAddress,char *buffer,uint32_t max_length);
void Convert_To_Str (uint32_t *Data, char *Buf);

void Flash_Write_NUM (uint32_t StartSectorAddress, float Num);
float Flash_Read_NUM (uint32_t StartSectorAddress);

#endif /* FLASH_INC_FLASH_H_ */
