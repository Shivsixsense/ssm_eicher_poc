/*
 * flash.c
 *
 *  Created on: Nov 25, 2024
 *      Author: shiv
 */
#include "flash.h"
#include "string.h"
#include "stdio.h"




#define FLASHWORD		8



uint8_t bytes_temp[4];

void float2Bytes(uint8_t * ftoa_bytes_temp,float float_variable)
{
    union {
      float a;
      uint8_t bytes[4];
    } thing;

    thing.a = float_variable;

    for (uint8_t i = 0; i < 4; i++) {
      ftoa_bytes_temp[i] = thing.bytes[i];
    }

}

float Bytes2float(uint8_t * ftoa_bytes_temp)
{
    union {
      float a;
      uint8_t bytes[4];
    } thing;

    for (uint8_t i = 0; i < 4; i++) {
    	thing.bytes[i] = ftoa_bytes_temp[i];
    }

   float float_variable =  thing.a;
   return float_variable;
}



uint32_t Flash_Write_Data (uint32_t StartAddress, uint64_t *data, uint32_t length)
{
    uint32_t errorCode = 0;

    // Check if StartAddress is within valid flash memory range
    if ((StartAddress < FLASH_USER_START_ADDR) || (StartAddress > FLASH_USER_END_ADDR))
    {
        return 1; // Invalid address
    }

    /* Unlock the Flash */
    HAL_FLASH_Unlock();

    // Erase the flash sector before writing
    FLASH_EraseInitTypeDef eraseInitStruct;
    uint32_t pageError = 0;

    eraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
    eraseInitStruct.Page        = (StartAddress - FLASH_BASE) / FLASH_PAGE_SIZE;
    eraseInitStruct.NbPages     = 1; // Erase one page
    eraseInitStruct.Banks       = FLASH_BANK_2;

    if (HAL_FLASHEx_Erase(&eraseInitStruct, &pageError) != HAL_OK)
    {
        // Handle erase error
        errorCode = HAL_FLASH_GetError();
    }

    // Write data sequentially
    for (uint32_t i = 0; i < length; i++)
    {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, StartAddress, data[i]) != HAL_OK)
        {
            errorCode = HAL_FLASH_GetError();
            break; // Stop on error
        }
        StartAddress += 8; // Move to next 64-bit address
    }

    /* Lock the Flash */
    HAL_FLASH_Lock();

    return errorCode;
}

void Flash_Read_Data (uint32_t StartAddress, uint64_t *data, uint16_t length)
{
    for (uint16_t i = 0; i < length; i++)
    {
        data[i] = *(__IO uint64_t *)StartAddress;
        StartAddress += 8; // Move to next 64-bit address
    }
}

void Convert_To_Str (uint32_t *Data, char *Buf)
{
	int numberofbytes = ((strlen((char *)Data)/4) + ((strlen((char *)Data) % 4) != 0)) *4;

	for (int i=0; i<numberofbytes; i++)
	{
		Buf[i] = Data[i/4]>>(8*(i%4));
	}
}

HAL_StatusTypeDef Flash_WriteString(uint32_t StartAddress,const char *str)
{
	uint32_t length = strlen(str) + 1; // Include null terminator
    uint32_t i = 0;
    uint32_t errorCode = 0;
   // strcat(str,'\0');

        if((StartAddress < FLASH_USER_START_ADDR) || (StartAddress >  FLASH_USER_END_ADDR))
    	{
           return 1;
    	}

	 /* Unlock the Flash to enable the flash control register access *************/
		HAL_FLASH_Unlock();

		// Erase the flash sector before writing
		FLASH_EraseInitTypeDef eraseInitStruct;
		uint32_t pageError = 0;

		eraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
		eraseInitStruct.Page        = ((StartAddress - 0x8000000) / 2048);
		eraseInitStruct.NbPages     = 1; // Erase one page
		eraseInitStruct.Banks       = FLASH_BANK_2;

		if (HAL_FLASHEx_Erase(&eraseInitStruct, &pageError) != HAL_OK)
		{
			// Handle erase error
			errorCode = HAL_FLASH_GetError();
		}

	    // Write each word (4 bytes) to Flash
	    for (i = 0; i < length; i += 8)
	    {
	        uint64_t word = 0;
	        memcpy(&word, &str[i], sizeof(word)); // Copy up to 4 bytes into a word
			if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, StartAddress, word) != HAL_OK)
			{
				// Handle programming error
				errorCode = HAL_FLASH_GetError();
				break; // Stop writing further if there's an error
			}

	        StartAddress += 8; // Increment address by 8 bytes
	    }
   return errorCode;
}


//void Flash_ReadString(uint32_t StartAddress,char *buffer,uint32_t max_length)
//{
//	uint32_t i = 0;
//
//	// Read words from Flash until null terminator or max length
//	while (i < max_length)
//	{
//		uint64_t word = *(volatile uint64_t *)StartAddress;
//		memcpy(buffer + i, &word, sizeof(word)); // Copy word to buffer
//
////		if (strchr((char *)&word, 0)) // Stop if null terminator is found
////		{
////			break;
////		}
//
//		StartAddress += 8; // Increment address by 4 bytes
//		i += 8;       // Increment buffer index by 4 bytes
//	}
//
//	buffer[max_length - 1] = '\0'; // Ensure null termination
//
//
//}

void Flash_ReadString(uint32_t StartAddress, char *buffer, uint32_t max_length) {
    uint32_t i = 0;

    // Read words from Flash until null terminator or max length
    while (i < max_length) {
        uint64_t word = *(volatile uint64_t *)StartAddress;
        memcpy(buffer + i, &word, sizeof(word)); // Copy word to buffer

        // Check for null terminator in the copied word
        for (size_t j = 0; j < sizeof(word) && (i + j) < max_length; j++) {
            if (buffer[i + j] == '\0') {
                buffer[i + j] = '\0'; // Ensure null termination
                return;
            }
        }

        StartAddress += sizeof(word); // Increment address by 8 bytes
        i += sizeof(word);           // Increment buffer index by 8 bytes
    }

    buffer[max_length - 1] = '\0'; // Ensure null termination if max length reached
}


void Flash_Write_NUM (uint32_t StartSectorAddress, float Num)
{

	float2Bytes(bytes_temp, Num);

	//Flash_Write_Data (StartSectorAddress, (uint32_t *)bytes_temp, 1);
}


float Flash_Read_NUM (uint32_t StartSectorAddress)
{
	uint8_t buffer[4];
	float value;

	Flash_Read_Data(StartSectorAddress, (uint64_t *)buffer, 1);
	value = Bytes2float(buffer);
	return value;
}
