/*
 * card.h
 *
 *  Created on: Jul 8, 2024
 *      Author: apple
 */

#ifndef SYSTEM_CARD_INC_CARD_H_
#define SYSTEM_CARD_INC_CARD_H_

#define IOT_ID 126
#define NETWORK_ID 125
#define CARD_ID 120

#define CARD_COM_TIMEOUT 5000

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "system_types.h"

typedef enum iot_card_status_e
{
    IOT_CARD_STATUS_OK = 0,
    IOT_CARD_STATUS_NOK,
    IOT_CARD_STATUS_UART_RX_DISCARD,

} iot_card_status;

// Frame structure
typedef struct {
    uint8_t start_byte;      // Start of the frame
    uint8_t dest_id;         // Destination ID
    uint8_t src_id;          // Source ID
    uint8_t address;      // Variable address (2 bytes in this example)
    uint8_t data_length;     // Length of the data
    uint16_t data; // Data payload
    uint16_t checksum;       // 16-bit checksum
    uint8_t end_byte;        // End of the frame
} Frame_t;

typedef enum packet_type_e
{
    packet_type_check_comms = 0,
    packet_type_commands_query = 1,
    packet_type_pending_operations = 2,
    packet_type_log_data = 3,
    packet_type_log_data_now = 4,
    packet_type_ack_with_data = 5,
    packet_type_ack_wait = 6,
    packet_type_nack = 7,
    packet_type_max
} packet_types;

typedef enum commamd_type_e
{
    READ = 1,
    WRITE = 2,
    Enable_Locked_Data_Write = 3,
    Read_Locked_Data = 4,
    Write_Locked_Data = 5,
    Disable_Locked_Data_Write = 6,
    Command_type_max
} command_types;

void ConstructTestFrame(void);
iot_card_status iotCard_init_module(void);

/**
 * @brief Parser for RAW Bytes received from UART
 * @return iot_card_status
 */
extern iot_card_status iotCard_parse_raw_data(uint8_t * data);

#endif /* SYSTEM_CARD_INC_CARD_H_ */
