

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef TEMPERATURE_INTERFACE_H_INCLUDE_GUARD
#define TEMPERATURE_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"

// Internal includes for this interface
#include "temperature_common.h"
//--------------------------------------------------------------------------------------------------
/**
 * Get the server service reference
 */
//--------------------------------------------------------------------------------------------------
le_msg_ServiceRef_t temperature_GetServiceRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the client session reference for the current message
 */
//--------------------------------------------------------------------------------------------------
le_msg_SessionRef_t temperature_GetClientSessionRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Initialize the server and advertise the service.
 */
//--------------------------------------------------------------------------------------------------
void temperature_AdvertiseService
(
    void
);



//--------------------------------------------------------------------------------------------------
/**
 * Read the temperature measurement.
 *
 * @return LE_OK if successful.
 */
//--------------------------------------------------------------------------------------------------
le_result_t temperature_Read
(
    double* readingPtr
        ///< [OUT] Where the reading (in degrees C) will be put if LE_OK is returned.
);


#endif // TEMPERATURE_INTERFACE_H_INCLUDE_GUARD
