

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef LE_NET_INTERFACE_H_INCLUDE_GUARD
#define LE_NET_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"

// Interface specific includes
#include "le_dcs_server.h"

// Internal includes for this interface
#include "le_net_common.h"
//--------------------------------------------------------------------------------------------------
/**
 * Get the server service reference
 */
//--------------------------------------------------------------------------------------------------
le_msg_ServiceRef_t le_net_GetServiceRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the client session reference for the current message
 */
//--------------------------------------------------------------------------------------------------
le_msg_SessionRef_t le_net_GetClientSessionRef
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Initialize the server and advertise the service.
 */
//--------------------------------------------------------------------------------------------------
void le_net_AdvertiseService
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * Structure used to communitcate a channel's DNS Server address
 */
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/**
 * Structure used to communitcate a channel's Default Gateway
 */
//--------------------------------------------------------------------------------------------------



//--------------------------------------------------------------------------------------------------
/**
 * Add or remove a route on the given channel according to the input flag in the last argument for
 * the given destination address its given subnet's mask prefix length.
 *
 * The channel reference in the first input argument identifies the network interface which a route
 * is to be added onto or removed from. Whether the operation is an add or a remove depends on the
 * isAdd boolean value of the last API input argument.
 *
 * The IP address and subnet input arguments specify the destination address and subnet for the
 * route. If it is a network route, the formats used for them are the same as used in the Linux
 * command "route add -net <ipAddr>/<prefixLength> dev <netInterface>". If the route is a
 * host route, the prefixLength input argument should be given as "" (i.e. a null string).
 *
 * @note The prefixLength parameter used to take a subnet mask (255.255.255.0) for IPv4 and prefix
 *       length for IPv6. Now it only takes prefix length, although compatibility code is present
 *       to make it compatible with previous API declaration. Providing a subnet mask is however
 *       deprecated and the compatibility code will be removed in a latter version.
 *
 * @return
 *      - LE_OK upon success, otherwise another le_result_t failure code
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_net_ChangeRoute
(
    le_dcs_ChannelRef_t channelRef,
        ///< [IN] the channel onto which the route change is made
    const char* LE_NONNULL destAddr,
        ///< [IN] Destination IP address for the route
    const char* LE_NONNULL prefixLength,
        ///< [IN] Destination's subnet prefix length
    bool isAdd
        ///< [IN] the change is to add (true) or delete (false)
);



//--------------------------------------------------------------------------------------------------
/**
 * Set the default GW addr for the given data channel retrieved from its technology
 *
 * @return
 *      - LE_OK upon success, otherwise LE_FAULT
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_net_SetDefaultGW
(
    le_dcs_ChannelRef_t channelRef
        ///< [IN] the channel on which interface its default GW
        ///< addr is to be set
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the default GW address for the given data channel.
 *
 * @note
 *      Accomodates dual-stack IPv4/IPv6 addresses. If the default GW address only is only IPv4 or
 *      IPv6, but not both, the unused field of "addr" will be nulled.
 *
 * @return
 *      - LE_OK upon success, otherwise LE_FAULT
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_net_GetDefaultGW
(
    le_dcs_ChannelRef_t channelRef,
        ///< [IN] Channel to retrieve GW addresses
    le_net_DefaultGatewayAddresses_t * addrPtr
        ///< [OUT] Channel's Default GW addresses
);



//--------------------------------------------------------------------------------------------------
/**
 * Backup the current default GW configs of the system, including both IPv4 and IPv6 as applicable.
 * For each client application using this API to back up this system setting, only one backup copy
 * is kept. When a client application makes a second call to this API, its first backup copy will
 * be overwritten by the newer.
 * Thus, le_net keeps one single backup copy per client application, and, thus, multiple backup
 * copies altogether.  They are kept in their LIFO (last-in-first-out) chronological order that
 * the sequence for client applications to call le_net_RestoreDefaltGW() should be in the exact
 * reverse order of their calling le_net_BackupDefaultGW() such that config backups and restorations
 * happen in the correct LIFO manner.
 */
//--------------------------------------------------------------------------------------------------
void le_net_BackupDefaultGW
(
    void
);



//--------------------------------------------------------------------------------------------------
/**
 * Restore the default GW configs of the system to the last backed up ones, including IPv4 and/or
 * IPv6 depending on whether this same client application has called le_net_SetDefaultGW() to
 * change the system's IPv4 and/or IPv6 configs or not. The le_net interface remembers it and
 * perform config restoration only as necessary when le_net_RestoreDefaultGW() is called. When
 * le_net_BackupDefaultGW() is called, both IPv4 and IPv6 default GW configs are archived when
 * present.
 * As le_net keeps one single backup copy per client application, and, thus, multiple backup
 * copies altogether, they are kept in their LIFO (last-in-first-out) chronological order that
 * the sequence for client applications to call le_net_RestoreDefaltGW() should be in the exact
 * reverse order of their calling le_net_BackupDefaultGW() such that config backups and restorations
 * happen in the correct LIFO manner.
 * If these applications do not follow this order, the le_net interface will first generate a
 * warning in the system log and then still go ahead to restore the configs as directed. These
 * applications hold the responsibility for the resulting routing configs on the device.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_net_RestoreDefaultGW
(
    void
);



//--------------------------------------------------------------------------------------------------
/**
 * Set the DNS addresses for the given data channel retrieved from its technology
 *
 * @return
 *      - LE_OK upon success, otherwise LE_FAULT
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_net_SetDNS
(
    le_dcs_ChannelRef_t channelRef
        ///< [IN] the channel from which the DNS addresses retrieved
        ///< will be set into the system config
);



//--------------------------------------------------------------------------------------------------
/**
 * Get the DNS server addresses for the given data channel retrieved from its technology
 *
 * @note
 *      Can accomodate up to two dual-stack DNS server addresses. In the case that there are more
 *      than two server addresses, the first two addresses of each IP version will be returned. If
 *      there are fewer than two dual-stack addresses, or contain only one type of IPv4 or IPv6
 *      addresses, the unused portions of the passed structure will be nulled and returned.
 *
 * @return
 *      - LE_OK upon success, otherwise LE_FAULT
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_net_GetDNS
(
    le_dcs_ChannelRef_t channelRef,
        ///< [IN] Channel to retrieve DNS server addresses
    le_net_DnsServerAddresses_t * addrPtr
        ///< [OUT] DNS server addresses
);



//--------------------------------------------------------------------------------------------------
/**
 * Remove the last added DNS addresses via the le_dcs_SetDNS API
 */
//--------------------------------------------------------------------------------------------------
void le_net_RestoreDNS
(
    void
);


#endif // LE_NET_INTERFACE_H_INCLUDE_GUARD
