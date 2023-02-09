

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */

/**
 * @page c_le_tpf third party download API
 *
 * @ref le_tpf_interface.h "API Reference" <br>
 *
 *
 * The Third Party FOTA (TPF) service provides a set of APIs to communicate with a third party
 * server in order to download a package from this server and install it.
 *
 * @section c_le_tpf_update Firmware/Application Updates
 * This FOTA API can be used to download from a third party server an update package. When this
 * package is successfully downloaded, it can be installed.
 *
 * @subsection c_le_tpf_update_control Update Control
 * le_tpf_Start() can be used to establish a HTTP or HTTPS connection and trigger a download of
 * package whose address was set by le_tpf_SetPackageUri(). Then the update procedure is executued
 * (depending on user agreement, see @ref c_le_avc).
 */

#ifndef LE_TPF_INTERFACE_H_INCLUDE_GUARD
#define LE_TPF_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"

// Internal includes for this interface
#include "le_tpf_common.h"
//--------------------------------------------------------------------------------------------------
/**
 * Type for handler called when a server disconnects.
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_tpf_DisconnectHandler_t)(void *);

//--------------------------------------------------------------------------------------------------
/**
 *
 * Connect the current client thread to the service providing this API. Block until the service is
 * available.
 *
 * For each thread that wants to use this API, either ConnectService or TryConnectService must be
 * called before any other functions in this API.  Normally, ConnectService is automatically called
 * for the main thread, but not for any other thread. For details, see @ref apiFilesC_client.
 *
 * This function is created automatically.
 */
//--------------------------------------------------------------------------------------------------
void le_tpf_ConnectService
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 *
 * Try to connect the current client thread to the service providing this API. Return with an error
 * if the service is not available.
 *
 * For each thread that wants to use this API, either ConnectService or TryConnectService must be
 * called before any other functions in this API.  Normally, ConnectService is automatically called
 * for the main thread, but not for any other thread. For details, see @ref apiFilesC_client.
 *
 * This function is created automatically.
 *
 * @return
 *  - LE_OK if the client connected successfully to the service.
 *  - LE_UNAVAILABLE if the server is not currently offering the service to which the client is
 *    bound.
 *  - LE_NOT_PERMITTED if the client interface is not bound to any service (doesn't have a binding).
 *  - LE_COMM_ERROR if the Service Directory cannot be reached.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_tpf_TryConnectService
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Set handler called when server disconnection is detected.
 *
 * When a server connection is lost, call this handler then exit with LE_FATAL.  If a program wants
 * to continue without exiting, it should call longjmp() from inside the handler.
 */
//--------------------------------------------------------------------------------------------------
LE_FULL_API void le_tpf_SetServerDisconnectHandler
(
    le_tpf_DisconnectHandler_t disconnectHandler,
    void *contextPtr
);

//--------------------------------------------------------------------------------------------------
/**
 *
 * Disconnect the current client thread from the service providing this API.
 *
 * Normally, this function doesn't need to be called. After this function is called, there's no
 * longer a connection to the service, and the functions in this API can't be used. For details, see
 * @ref apiFilesC_client.
 *
 * This function is created automatically.
 */
//--------------------------------------------------------------------------------------------------
void le_tpf_DisconnectService
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * Function to set the package URI for download from 3rd party server using HTTP(S) protocol.
 *
 * @return
 *      - LE_OK on success.
 *      - LE_FAULT if failed to configure the package URI.
 *      - LE_BAD_PARAMETER if the given URI or port number are not in the right format.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_tpf_SetPackageUri
(
    const char* LE_NONNULL packageUri
        ///< [IN] Package address
);

//--------------------------------------------------------------------------------------------------
/**
 * Function to get the package URI for download from 3rd party server.
 *
 * @return
 *      - LE_OK on success.
 *      - LE_FAULT if failed to configure the package URI.
 *      - LE_BAD_PARAMETER if the given URI or port number are not in the right format.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_tpf_GetPackageUri
(
    char* packageUri,
        ///< [OUT] Package address
    size_t packageUriSize
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Start a package download from a 3rd party server
 *
 * This will cause a query to be sent to the server, for pending updates.
 *
 * @return
 *      - LE_OK if connection request has been sent.
 *      - LE_FAULT on failure.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_tpf_Start
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the current state of TPF service.
 *
 * @return
 *      - true if TPF service is enabled.
 *      - false if TPF service is disabled.
 */
//--------------------------------------------------------------------------------------------------
bool le_tpf_IsTpfStarted
(
    void
);

#endif // LE_TPF_INTERFACE_H_INCLUDE_GUARD
