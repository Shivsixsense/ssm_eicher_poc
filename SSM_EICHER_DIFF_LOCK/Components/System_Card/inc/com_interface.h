
#ifndef __COM_INTERFACE__H
#define __COM_INTERFACE__H

#include "system_types.h"
#include <string.h>

void card_init_com_interface(void);
int card_com_send_reponse(uint8_t *pData, uint16_t len);

#endif
