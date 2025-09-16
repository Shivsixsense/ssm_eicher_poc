#pragma once

#include "system_types.h"
#include "time.h"

/* obd2 status types */
typedef enum appls_status_type_e
{

    APPL_STATUS_OK = 0x00,
    APPL_STATUS_ERROR = 0x01,
    APPL_STATUS_NO_DATA = 0x02,
    APPL_STATUS_INIT_ERR = 0x03,

} ApplStatusType;

