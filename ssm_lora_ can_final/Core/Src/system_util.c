/*
 * system_util.c
 *
 *  Created on: Jun 10, 2024
 *      Author: apple
 */
#include"system_util.h"





void swapBytesHexArray(unsigned char *array, size_t start, size_t end, unsigned char *result)
{
    size_t length = end - start + 1;
    for (size_t i = 0; i < length; i++)
    {
        result[i] = array[end - i];
    }
}


unsigned int hexToDecimal(unsigned char hexValue[], int numBytes, int startByte)
{
    unsigned int decimalValue = 0;

    for (int i = startByte; i < startByte + numBytes; i++)
    {
        decimalValue = (decimalValue << 8) | hexValue[i];
    }

    return decimalValue;
}


