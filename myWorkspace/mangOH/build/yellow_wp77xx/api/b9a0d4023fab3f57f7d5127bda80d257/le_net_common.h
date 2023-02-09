
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */
#ifndef LE_NET_COMMON_H_INCLUDE_GUARD
#define LE_NET_COMMON_H_INCLUDE_GUARD


#include "legato.h"

// Interface specific includes
#include "le_dcs_common.h"

#define IFGEN_LE_NET_PROTOCOL_ID "b3b34031c1adcb1b6bf3372b9bab2f5b"
#define IFGEN_LE_NET_MSG_SIZE 136



//--------------------------------------------------------------------------------------------------
/**
 * Interface name string length.
 */
//--------------------------------------------------------------------------------------------------
#define LE_NET_INTERFACE_NAME_MAX_LEN 100

//--------------------------------------------------------------------------------------------------
/**
 * IPv4 addr string's max length
 */
//--------------------------------------------------------------------------------------------------
#define LE_NET_IPV4ADDR_MAX_LEN 16

//--------------------------------------------------------------------------------------------------
/**
 * IPv6 addr string's max length
 */
//--------------------------------------------------------------------------------------------------
#define LE_NET_IPV6ADDR_MAX_LEN 46

//--------------------------------------------------------------------------------------------------
/**
 * IP addr string's max length
 */
//--------------------------------------------------------------------------------------------------
#define LE_NET_IPADDR_MAX_LEN 46

//--------------------------------------------------------------------------------------------------
/**
 * Structure used to communitcate a channel's DNS Server address
 */
//--------------------------------------------------------------------------------------------------
typedef struct
{
    char ipv4Addr1[16 + 1];
    char ipv4Addr2[16 + 1];
    char ipv6Addr1[46 + 1];
    char ipv6Addr2[46 + 1];
}
le_net_DnsServerAddresses_t;


//--------------------------------------------------------------------------------------------------
/**
 * Structure used to communitcate a channel's Default Gateway
 */
//--------------------------------------------------------------------------------------------------
typedef struct
{
    char ipv4Addr[16 + 1];
    char ipv6Addr[46 + 1];
}
le_net_DefaultGatewayAddresses_t;



//--------------------------------------------------------------------------------------------------
/**
 * Get if this client bound locally.
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED bool ifgen_le_net_HasLocalBinding
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * Init data that is common across all threads
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED void ifgen_le_net_InitCommonData
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * Perform common initialization and open a session
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_net_OpenSession
(
    le_msg_SessionRef_t _ifgen_sessionRef,
    bool isBlocking
);

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
LE_SHARED le_result_t ifgen_le_net_ChangeRoute
(
    le_msg_SessionRef_t _ifgen_sessionRef,
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
LE_SHARED le_result_t ifgen_le_net_SetDefaultGW
(
    le_msg_SessionRef_t _ifgen_sessionRef,
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
LE_SHARED le_result_t ifgen_le_net_GetDefaultGW
(
    le_msg_SessionRef_t _ifgen_sessionRef,
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
LE_SHARED void ifgen_le_net_BackupDefaultGW
(
    le_msg_SessionRef_t _ifgen_sessionRef
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
LE_SHARED le_result_t ifgen_le_net_RestoreDefaultGW
(
    le_msg_SessionRef_t _ifgen_sessionRef
);

//--------------------------------------------------------------------------------------------------
/**
 * Set the DNS addresses for the given data channel retrieved from its technology
 *
 * @return
 *      - LE_OK upon success, otherwise LE_FAULT
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_net_SetDNS
(
    le_msg_SessionRef_t _ifgen_sessionRef,
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
LE_SHARED le_result_t ifgen_le_net_GetDNS
(
    le_msg_SessionRef_t _ifgen_sessionRef,
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
LE_SHARED void ifgen_le_net_RestoreDNS
(
    le_msg_SessionRef_t _ifgen_sessionRef
);

#endif // LE_NET_COMMON_H_INCLUDE_GUARD
