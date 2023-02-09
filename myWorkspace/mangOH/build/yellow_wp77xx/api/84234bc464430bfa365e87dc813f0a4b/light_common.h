
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */
#ifndef LIGHT_COMMON_H_INCLUDE_GUARD
#define LIGHT_COMMON_H_INCLUDE_GUARD


#include "legato.h"

#define IFGEN_LIGHT_PROTOCOL_ID "92c3cd37d7f8927125b83c81b3f7f27e"
#define IFGEN_LIGHT_MSG_SIZE 20




//--------------------------------------------------------------------------------------------------
/**
 * Get if this client bound locally.
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED bool ifgen_light_HasLocalBinding
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * Init data that is common across all threads
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED void ifgen_light_InitCommonData
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * Perform common initialization and open a session
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_light_OpenSession
(
    le_msg_SessionRef_t _ifgen_sessionRef,
    bool isBlocking
);

//--------------------------------------------------------------------------------------------------
/**
 * Read the light intensity measurement.
 *
 * @return LE_OK if successful.
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_light_Read
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        double* readingPtr
        ///< [OUT] Where the light intensity reading will be put if LE_OK is returned.
);

#endif // LIGHT_COMMON_H_INCLUDE_GUARD
