
/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */
#ifndef LE_MRC_COMMON_H_INCLUDE_GUARD
#define LE_MRC_COMMON_H_INCLUDE_GUARD


#include "legato.h"

#define IFGEN_LE_MRC_PROTOCOL_ID "60ecb96d487478a24f595fb1ca27d9ab"
#define IFGEN_LE_MRC_MSG_SIZE 116



//--------------------------------------------------------------------------------------------------
/**
 * Mobile Country Code length
 */
//--------------------------------------------------------------------------------------------------
#define LE_MRC_MCC_LEN 3

//--------------------------------------------------------------------------------------------------
/**
 * Mobile Country Code length
 * One extra byte is added for the null character.
 */
//--------------------------------------------------------------------------------------------------
#define LE_MRC_MCC_BYTES 4

//--------------------------------------------------------------------------------------------------
/**
 * Mobile Network Code length
 */
//--------------------------------------------------------------------------------------------------
#define LE_MRC_MNC_LEN 3

//--------------------------------------------------------------------------------------------------
/**
 * Mobile Network Code length
 * One extra byte is added for the null character.
 */
//--------------------------------------------------------------------------------------------------
#define LE_MRC_MNC_BYTES 4

//--------------------------------------------------------------------------------------------------
/**
 * Cellular Network length.
 *
 */
//--------------------------------------------------------------------------------------------------
#define LE_MRC_NETWORK_NAME_MAX_LEN 100

//--------------------------------------------------------------------------------------------------
/**
 * 2G/3G Band Bit Mask (64 bits)
 */
//--------------------------------------------------------------------------------------------------
#define LE_MRC_BITMASK_BAND_CLASS_0_A_SYSTEM 0x1
#define LE_MRC_BITMASK_BAND_CLASS_0_B_SYSTEM 0x2
#define LE_MRC_BITMASK_BAND_CLASS_1_ALL_BLOCKS 0x4
#define LE_MRC_BITMASK_BAND_CLASS_2_PLACEHOLDER 0x8
#define LE_MRC_BITMASK_BAND_CLASS_3_A_SYSTEM 0x10
#define LE_MRC_BITMASK_BAND_CLASS_4_ALL_BLOCKS 0x20
#define LE_MRC_BITMASK_BAND_CLASS_5_ALL_BLOCKS 0x40
#define LE_MRC_BITMASK_BAND_CLASS_6 0x80
#define LE_MRC_BITMASK_BAND_CLASS_7 0x100
#define LE_MRC_BITMASK_BAND_CLASS_8 0x200
#define LE_MRC_BITMASK_BAND_CLASS_9 0x400
#define LE_MRC_BITMASK_BAND_CLASS_10 0x800
#define LE_MRC_BITMASK_BAND_CLASS_11 0x1000
#define LE_MRC_BITMASK_BAND_CLASS_12 0x2000
#define LE_MRC_BITMASK_BAND_CLASS_14 0x4000
#define LE_MRC_BITMASK_BAND_CLASS_15 0x8000
#define LE_MRC_BITMASK_BAND_CLASS_16 0x10000
#define LE_MRC_BITMASK_BAND_CLASS_17 0x20000
#define LE_MRC_BITMASK_BAND_CLASS_18 0x40000
#define LE_MRC_BITMASK_BAND_CLASS_19 0x80000
#define LE_MRC_BITMASK_BAND_GSM_DCS_1800 0x100000
#define LE_MRC_BITMASK_BAND_EGSM_900 0x200000
#define LE_MRC_BITMASK_BAND_PRI_GSM_900 0x400000
#define LE_MRC_BITMASK_BAND_GSM_450 0x800000
#define LE_MRC_BITMASK_BAND_GSM_480 0x1000000
#define LE_MRC_BITMASK_BAND_GSM_750 0x2000000
#define LE_MRC_BITMASK_BAND_GSM_850 0x4000000
#define LE_MRC_BITMASK_BAND_GSMR_900 0x8000000
#define LE_MRC_BITMASK_BAND_GSM_PCS_1900 0x10000000
#define LE_MRC_BITMASK_BAND_WCDMA_EU_J_CH_IMT_2100 0x20000000
#define LE_MRC_BITMASK_BAND_WCDMA_US_PCS_1900 0x40000000
#define LE_MRC_BITMASK_BAND_WCDMA_EU_CH_DCS_1800 0x80000000
#define LE_MRC_BITMASK_BAND_WCDMA_US_1700 0x100000000
#define LE_MRC_BITMASK_BAND_WCDMA_US_850 0x200000000
#define LE_MRC_BITMASK_BAND_WCDMA_J_800 0x400000000
#define LE_MRC_BITMASK_BAND_WCDMA_EU_2600 0x800000000
#define LE_MRC_BITMASK_BAND_WCDMA_EU_J_900 0x1000000000
#define LE_MRC_BITMASK_BAND_WCDMA_J_1700 0x2000000000
typedef uint64_t le_mrc_BandBitMask_t;


//--------------------------------------------------------------------------------------------------
/**
 * LTE Band Bit Mask (64 bits)
 *
 */
//--------------------------------------------------------------------------------------------------
#define LE_MRC_BITMASK_LTE_BAND_E_UTRA_OP_BAND_1 0x1
#define LE_MRC_BITMASK_LTE_BAND_E_UTRA_OP_BAND_2 0x2
#define LE_MRC_BITMASK_LTE_BAND_E_UTRA_OP_BAND_3 0x4
#define LE_MRC_BITMASK_LTE_BAND_E_UTRA_OP_BAND_4 0x8
#define LE_MRC_BITMASK_LTE_BAND_E_UTRA_OP_BAND_5 0x10
#define LE_MRC_BITMASK_LTE_BAND_E_UTRA_OP_BAND_6 0x20
#define LE_MRC_BITMASK_LTE_BAND_E_UTRA_OP_BAND_7 0x40
#define LE_MRC_BITMASK_LTE_BAND_E_UTRA_OP_BAND_8 0x80
#define LE_MRC_BITMASK_LTE_BAND_E_UTRA_OP_BAND_9 0x100
#define LE_MRC_BITMASK_LTE_BAND_E_UTRA_OP_BAND_10 0x200
#define LE_MRC_BITMASK_LTE_BAND_E_UTRA_OP_BAND_11 0x400
#define LE_MRC_BITMASK_LTE_BAND_E_UTRA_OP_BAND_12 0x800
#define LE_MRC_BITMASK_LTE_BAND_E_UTRA_OP_BAND_13 0x1000
#define LE_MRC_BITMASK_LTE_BAND_E_UTRA_OP_BAND_14 0x2000
#define LE_MRC_BITMASK_LTE_BAND_E_UTRA_OP_BAND_17 0x4000
#define LE_MRC_BITMASK_LTE_BAND_E_UTRA_OP_BAND_18 0x8000
#define LE_MRC_BITMASK_LTE_BAND_E_UTRA_OP_BAND_19 0x10000
#define LE_MRC_BITMASK_LTE_BAND_E_UTRA_OP_BAND_20 0x20000
#define LE_MRC_BITMASK_LTE_BAND_E_UTRA_OP_BAND_21 0x40000
#define LE_MRC_BITMASK_LTE_BAND_E_UTRA_OP_BAND_24 0x80000
#define LE_MRC_BITMASK_LTE_BAND_E_UTRA_OP_BAND_25 0x100000
#define LE_MRC_BITMASK_LTE_BAND_E_UTRA_OP_BAND_26 0x200000
#define LE_MRC_BITMASK_LTE_BAND_E_UTRA_OP_BAND_28 0x400000
#define LE_MRC_BITMASK_LTE_BAND_E_UTRA_OP_BAND_33 0x800000
#define LE_MRC_BITMASK_LTE_BAND_E_UTRA_OP_BAND_34 0x1000000
#define LE_MRC_BITMASK_LTE_BAND_E_UTRA_OP_BAND_35 0x2000000
#define LE_MRC_BITMASK_LTE_BAND_E_UTRA_OP_BAND_36 0x4000000
#define LE_MRC_BITMASK_LTE_BAND_E_UTRA_OP_BAND_37 0x8000000
#define LE_MRC_BITMASK_LTE_BAND_E_UTRA_OP_BAND_38 0x10000000
#define LE_MRC_BITMASK_LTE_BAND_E_UTRA_OP_BAND_39 0x20000000
#define LE_MRC_BITMASK_LTE_BAND_E_UTRA_OP_BAND_40 0x40000000
#define LE_MRC_BITMASK_LTE_BAND_E_UTRA_OP_BAND_41 0x80000000
#define LE_MRC_BITMASK_LTE_BAND_E_UTRA_OP_BAND_42 0x100000000
#define LE_MRC_BITMASK_LTE_BAND_E_UTRA_OP_BAND_43 0x200000000
#define LE_MRC_BITMASK_LTE_BAND_E_UTRA_OP_BAND_66 0x400000000
#define LE_MRC_BITMASK_LTE_BAND_E_UTRA_OP_BAND_71 0x800000000
typedef uint64_t le_mrc_LteBandBitMask_t;


//--------------------------------------------------------------------------------------------------
/**
 * TDSCDMA Band Bit Mask
 *
 */
//--------------------------------------------------------------------------------------------------
#define LE_MRC_BITMASK_TDSCDMA_BAND_A 0x1
#define LE_MRC_BITMASK_TDSCDMA_BAND_B 0x2
#define LE_MRC_BITMASK_TDSCDMA_BAND_C 0x4
#define LE_MRC_BITMASK_TDSCDMA_BAND_D 0x8
#define LE_MRC_BITMASK_TDSCDMA_BAND_E 0x10
#define LE_MRC_BITMASK_TDSCDMA_BAND_F 0x20
typedef uint32_t le_mrc_TdScdmaBandBitMask_t;


//--------------------------------------------------------------------------------------------------
/**
 * Reference type for referring to MRC Scan Information objects.
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_mrc_ScanInformation* le_mrc_ScanInformationRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Reference type for Scan Information Listing.
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_mrc_ScanInformationList* le_mrc_ScanInformationListRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Reference type for PCI Scan Information Listing.
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_mrc_PciScanInformationList* le_mrc_PciScanInformationListRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Reference type for PCI Scan Information .
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_mrc_PciScanInformation* le_mrc_PciScanInformationRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Reference type for PLMN Information .
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_mrc_PlmnInformation* le_mrc_PlmnInformationRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Reference type for referring to the Preferred Operator objects.
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_mrc_PreferredOperator* le_mrc_PreferredOperatorRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Reference type for Preferred Operator Listing.
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_mrc_PreferredOperatorList* le_mrc_PreferredOperatorListRef_t;


//--------------------------------------------------------------------------------------------------
/**
 *  Reference type for one Cell Information.
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_mrc_CellInfo* le_mrc_CellInfoRef_t;


//--------------------------------------------------------------------------------------------------
/**
 *  Reference type for all Neighboring Cells Information.
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_mrc_NeighborCells* le_mrc_NeighborCellsRef_t;


//--------------------------------------------------------------------------------------------------
/**
 *  Reference type for the signal metrics.
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_mrc_Metrics* le_mrc_MetricsRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Network Registration states.
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_MRC_REG_NONE = 0,
        ///< Not registered and not currently searching for new operator.
    LE_MRC_REG_HOME = 1,
        ///< Registered, home network.
    LE_MRC_REG_SEARCHING = 2,
        ///< Not registered but currently searching for a new operator.
    LE_MRC_REG_DENIED = 3,
        ///< Registration was denied, usually because of invalid access credentials.
    LE_MRC_REG_ROAMING = 4,
        ///< Registered to a roaming network.
    LE_MRC_REG_UNKNOWN = 5
        ///< Unknown state.
}
le_mrc_NetRegState_t;


//--------------------------------------------------------------------------------------------------
/**
 * Radio Access Technology enum
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_MRC_RAT_UNKNOWN = 0,
        ///< Unknown
    LE_MRC_RAT_GSM = 1,
        ///< GSM network
    LE_MRC_RAT_UMTS = 2,
        ///< UMTS networks
    LE_MRC_RAT_TDSCDMA = 3,
        ///< TD-SCDMA networks
    LE_MRC_RAT_LTE = 4,
        ///< LTE network
    LE_MRC_RAT_CDMA = 5
        ///< CDMA network
}
le_mrc_Rat_t;


//--------------------------------------------------------------------------------------------------
/**
 * Jamming detection report type enum
 *
 * @note If the reported status is the final status (end of the procedure), this shall be indicated
 * within the status report.
 * @note Other reports, called intermediate reports, gives only intermediate results issuing from
 * the algorithm
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_MRC_JAMMING_REPORT_FINAL = 0,
        ///< Final jamming status report
    LE_MRC_JAMMING_REPORT_INTERMEDIATE = 1
        ///< Intermediate jamming status report
}
le_mrc_JammingReport_t;


//--------------------------------------------------------------------------------------------------
/**
 * Jamming detection status type enum
 *
 * @note The final status can be only NULL or JAMMED.
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_MRC_JAMMING_STATUS_UNKNOWN = 0,
        ///< Status is unknown
    LE_MRC_JAMMING_STATUS_NULL = 1,
        ///< No jamming suspicion; radio environment is considered normal
    LE_MRC_JAMMING_STATUS_LOW = 2,
        ///< Low probability that the device is jammed, but some radio
        ///< environment parameters are considered abnormal
    LE_MRC_JAMMING_STATUS_MEDIUM = 3,
        ///< Medium probability that the device is jammed; a lot of
        ///< interferences in the radio spectrum
    LE_MRC_JAMMING_STATUS_HIGH = 4,
        ///< High probability that the device is jammed; radio environment is
        ///< considered jammed, but there is still a possibility that the module
        ///< succeeds in synchronizing a cell
    LE_MRC_JAMMING_STATUS_JAMMED = 5
        ///< Module is jammed; cell synchronization impossible while sufficient
        ///< power level is detected on a large number of frequencies
}
le_mrc_JammingStatus_t;


//--------------------------------------------------------------------------------------------------
/**
 * Radio Bitmask Access Technology Bit Mask
 */
//--------------------------------------------------------------------------------------------------/// GSM RAT Bit Mask
#define LE_MRC_BITMASK_RAT_GSM 0x1/// UMTS RAT Bit Mask
#define LE_MRC_BITMASK_RAT_UMTS 0x2/// TD-SCDMA RAT Bit Mask
#define LE_MRC_BITMASK_RAT_TDSCDMA 0x4/// LTE RAT Bit Mask
#define LE_MRC_BITMASK_RAT_LTE 0x8/// CDMA RAT Bit Mask
#define LE_MRC_BITMASK_RAT_CDMA 0x10/// To Set all RAT preference Bit Mask
#define LE_MRC_BITMASK_RAT_ALL 0x20
#define LE_MRC_BITMASK_RAT_MAX 0x40
typedef uint32_t le_mrc_RatBitMask_t;


//--------------------------------------------------------------------------------------------------
/**
 * Network Registration reject cause type enum.
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_MRC_IMSI_UNKNOWN = 0,
        ///< IMSI unknown in HLR.
    LE_MRC_ILLEGAL_MS = 1,
        ///< Illegal MS.
    LE_MRC_IMEI_NOT_ACCEPTED = 2,
        ///< IMEI not accepted.
    LE_MRC_ILLEGAL_ME = 3,
        ///< Illegal ME.
    LE_MRC_GPRS_NOT_ALLOWED = 4,
        ///< GPRS services not allowed.
    LE_MRC_GPRS_NON_GPRS_NOT_ALLOWED = 5,
        ///< GPRS services and non-GPRS services not allowed.
    LE_MRC_MS_IDENTITY_UNKNOWN = 6,
        ///< Network cannot derive the MS’s identity.
    LE_MRC_NETWORK_IMPLICIT_DETACH = 7,
        ///< Implicitly detached.
    LE_MRC_PLMN_NOT_ALLOWED = 8,
        ///< PLMN not allowed.
    LE_MRC_LOCATION_NOT_ALLOWED = 9,
        ///< Location Area not allowed.
    LE_MRC_ROAMING_NOT_ALLOWED = 10,
        ///< Roaming not allowed in this location area.
    LE_MRC_GPRS_NOT_ALLOWED_PLMN = 11,
        ///< GPRS services not allowed in this PLMN.
    LE_MRC_NO_SUITABLE_CELL = 12,
        ///< No Suitable Cells In Location Area.
    LE_MRC_MSC_NOT_REACHABLE = 13,
        ///< MSC temporarily not reachable.
    LE_MRC_NETWORK_FAILURE = 14,
        ///< Network failure.
    LE_MRC_MAC_FAILURE = 15,
        ///< MAC failure.
    LE_MRC_SYNCH_FAILURE = 16,
        ///< Synch failure.
    LE_MRC_CONGESTION = 17,
        ///< Congestion.
    LE_MRC_GSM_AUTH_FAILURE = 18,
        ///< GSM authentication unacceptable.
    LE_MRC_CSG_NOT_AUTHORIZED = 19,
        ///< Not authorized for this CSG.
    LE_MRC_SMS_VIA_GPRS = 20,
        ///< SMS provided via GPRS in this routing area.
    LE_MRC_No_PDP_ACTIVATED = 21,
        ///< No PDP context activated.
    LE_MRC_RETRY_NEW_CELL = 22,
        ///< Retry upon entry into a new cell.
    LE_MRC_SEMANTICAL_INCORRECT = 23,
        ///< Semantically incorrect message.
    LE_MRC_INVALID_INFORMATION = 24,
        ///< Invalid mandatory information.
    LE_MRC_MESSAGE_TYPE_WRONG = 25,
        ///< Message type non-existent or not implemented.
    LE_MRC_MESSAGE_TYPE_NOT_COMPATIBLE = 26,
        ///< Message type not compatible with the protocol state.
    LE_MRC_WRONG_IE = 27,
        ///< Information element non-existent or not implemented.
    LE_MRC_IE_ERROR = 28,
        ///< Conditional IE error.
    LE_MRC_MESSAGE_NOT_COMPATIBLE = 29,
        ///< Message not compatible with the protocol state.
    LE_MRC_PROTOCOL_ERROR = 30
        ///< Protocol error, unspecified.
}
le_mrc_NetRejCause_t;


//--------------------------------------------------------------------------------------------------
/**
 * Network Registration service domain type enum.
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_MRC_NO_SERVICE = 0,
        ///< Scan is ongoing, the module cannot see any networks.
    LE_MRC_CS = 1,
        ///< Circuit-switched only.
    LE_MRC_PS = 2,
        ///< Packet-switched only.
    LE_MRC_CS_PS = 3,
        ///< Circuit-switched and packet-switched.
    LE_MRC_LIMITED_SERVICE = 4
        ///< No network register after scan.
}
le_mrc_NetRegDomain_t;


//--------------------------------------------------------------------------------------------------
/**
 * Network registration reject indication structure.
 */
//--------------------------------------------------------------------------------------------------
typedef struct
{
    le_mrc_Rat_t rat;
    char mcc[4 + 1];
    char mnc[4 + 1];
    le_mrc_NetRejCause_t cause;
    le_mrc_NetRegDomain_t rejDomain;
}
le_mrc_NetRegRejectInd_t;


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'le_mrc_NetRegStateEvent'
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_mrc_NetRegStateEventHandler* le_mrc_NetRegStateEventHandlerRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'le_mrc_RatChange'
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_mrc_RatChangeHandler* le_mrc_RatChangeHandlerRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'le_mrc_PacketSwitchedChange'
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_mrc_PacketSwitchedChangeHandler* le_mrc_PacketSwitchedChangeHandlerRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'le_mrc_SignalStrengthChange'
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_mrc_SignalStrengthChangeHandler* le_mrc_SignalStrengthChangeHandlerRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'le_mrc_NetworkReject'
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_mrc_NetworkRejectHandler* le_mrc_NetworkRejectHandlerRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'le_mrc_NetRegReject'
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_mrc_NetRegRejectHandler* le_mrc_NetRegRejectHandlerRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'le_mrc_JammingDetectionEvent'
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_mrc_JammingDetectionEventHandler* le_mrc_JammingDetectionEventHandlerRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Handler for Network registration state changes.
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_mrc_NetRegStateHandlerFunc_t)
(
        le_mrc_NetRegState_t state,
        ///< Parameter ready to receive the Network Registration state.
        void* contextPtr
        ///<
);

//--------------------------------------------------------------------------------------------------
/**
 * Handler for Radio Access Technology changes.
 *
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_mrc_RatChangeHandlerFunc_t)
(
        le_mrc_Rat_t rat,
        ///< Parameter ready to receive the Radio Access Technology.
        void* contextPtr
        ///<
);

//--------------------------------------------------------------------------------------------------
/**
 * Handler for Packet Switched service state changes.
 *
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_mrc_PacketSwitchedChangeHandlerFunc_t)
(
        le_mrc_NetRegState_t state,
        ///< Parameter ready to receive the Packet Switched service state.
        void* contextPtr
        ///<
);

//--------------------------------------------------------------------------------------------------
/**
 * Handler for Signal Strength value changes.
 *
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_mrc_SignalStrengthChangeHandlerFunc_t)
(
        int32_t ss,
        ///< Signal strength in dBm
        void* contextPtr
        ///<
);

//--------------------------------------------------------------------------------------------------
/**
 * Handler for cellular asynchronous network scan Sending result.
 *  Returns the reference to the List object. Null pointer if the scan failed.
 *
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_mrc_CellularNetworkScanHandlerFunc_t)
(
        le_mrc_ScanInformationListRef_t listRef,
        ///< Reference to the List object. Null pointer if the
        ///< scan failed.
        void* contextPtr
        ///<
);

//--------------------------------------------------------------------------------------------------
/**
 * Handler for PCI asynchronous network scan Sending result.
 *  Returns the reference to the List object. Null pointer if the scan failed.
 *
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_mrc_PciNetworkScanHandlerFunc_t)
(
        le_mrc_PciScanInformationListRef_t listRef,
        ///< Reference to the List object. Null pointer if the
        ///< scan failed.
        void* contextPtr
        ///<
);

//--------------------------------------------------------------------------------------------------
/**
 * Handler for asynchronous manual selection response
 *
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_mrc_ManualSelectionHandlerFunc_t)
(
        le_result_t result,
        ///< Manual selection response
        void* contextPtr
        ///<
);

//--------------------------------------------------------------------------------------------------
/**
 * Handler to report network reject indication.
 *
 * @deprecated le_mrc_NetworkRejectHandler() will be removed in near future.
 * It will be replaced by le_mrc_AddNetRegRejectHandler().
 *
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_mrc_NetworkRejectHandlerFunc_t)
(
        const char* LE_NONNULL mcc,
        ///< Mobile Country Code
        const char* LE_NONNULL mnc,
        ///< Mobile Network Code
        le_mrc_Rat_t rat,
        ///< Radio Access Technology.
        void* contextPtr
        ///<
);

//--------------------------------------------------------------------------------------------------
/**
 * Handler to report network registration reject indication.
 * NetworkRejectHandler is deprecated.
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_mrc_NetRegRejectHandlerFunc_t)
(
        const le_mrc_NetRegRejectInd_t * LE_NONNULL networkRejectIndPtr,
        ///< NetRegRejectInd Reference.
        void* contextPtr
        ///<
);

//--------------------------------------------------------------------------------------------------
/**
 * Handler for jamming detection event.
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_mrc_JammingDetectionHandlerFunc_t)
(
        le_mrc_JammingReport_t report,
        ///< Report type.
        le_mrc_JammingStatus_t status,
        ///< Jamming detection status.
        void* contextPtr
        ///<
);


//--------------------------------------------------------------------------------------------------
/**
 * Get if this client bound locally.
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED bool ifgen_le_mrc_HasLocalBinding
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * Init data that is common across all threads
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED void ifgen_le_mrc_InitCommonData
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * Perform common initialization and open a session
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_mrc_OpenSession
(
    le_msg_SessionRef_t _ifgen_sessionRef,
    bool isBlocking
);

//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'le_mrc_NetRegStateEvent'
 *
 * This event provides information on network registration state changes.
 *
 * @note <b>multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_mrc_NetRegStateEventHandlerRef_t ifgen_le_mrc_AddNetRegStateEventHandler
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_NetRegStateHandlerFunc_t handlerPtr,
        ///< [IN]
        void* contextPtr
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'le_mrc_NetRegStateEvent'
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED void ifgen_le_mrc_RemoveNetRegStateEventHandler
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_NetRegStateEventHandlerRef_t handlerRef
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'le_mrc_RatChange'
 *
 * This event provides information on Radio Access Technology changes.
 *
 * @note <b>multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_mrc_RatChangeHandlerRef_t ifgen_le_mrc_AddRatChangeHandler
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_RatChangeHandlerFunc_t handlerPtr,
        ///< [IN]
        void* contextPtr
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'le_mrc_RatChange'
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED void ifgen_le_mrc_RemoveRatChangeHandler
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_RatChangeHandlerRef_t handlerRef
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'le_mrc_PacketSwitchedChange'
 *
 * This event provides information on Packet Switched service changes.
 *
 * @note <b>multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_mrc_PacketSwitchedChangeHandlerRef_t ifgen_le_mrc_AddPacketSwitchedChangeHandler
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_PacketSwitchedChangeHandlerFunc_t packetHandlerPtr,
        ///< [IN]
        void* contextPtr
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'le_mrc_PacketSwitchedChange'
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED void ifgen_le_mrc_RemovePacketSwitchedChangeHandler
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_PacketSwitchedChangeHandlerRef_t handlerRef
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'le_mrc_SignalStrengthChange'
 *
 * This event provides information on Signal Strength value changes.
 *
 * @note <b>NOT multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_mrc_SignalStrengthChangeHandlerRef_t ifgen_le_mrc_AddSignalStrengthChangeHandler
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_Rat_t rat,
        ///< [IN] Radio Access Technology
        int32_t lowerRangeThreshold,
        ///< [IN] Lower-range Signal strength threshold in dBm
        int32_t upperRangeThreshold,
        ///< [IN] Upper-range Signal strength threshold in dBm
        le_mrc_SignalStrengthChangeHandlerFunc_t handlerPtr,
        ///< [IN]
        void* contextPtr
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'le_mrc_SignalStrengthChange'
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED void ifgen_le_mrc_RemoveSignalStrengthChangeHandler
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_SignalStrengthChangeHandlerRef_t handlerRef
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * This function sets signal strength indication thresholds for a specific RAT.
 *
 * @return
 *  - LE_OK             Function succeeded.
 *  - LE_BAD_PARAMETER  Bad parameters
 *  - LE_FAULT          Function failed.
 *
 * @note <b>NOT multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_mrc_SetSignalStrengthIndThresholds
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_Rat_t rat,
        ///< [IN] Radio Access Technology
        int32_t lowerRangeThreshold,
        ///< [IN] Lower-range Signal strength threshold in dBm
        int32_t upperRangeThreshold
        ///< [IN] Upper-range Signal strength threshold in dBm
);

//--------------------------------------------------------------------------------------------------
/**
 * This function sets a signal strength indication delta for a specific RAT.
 *
 * @return
 *  - LE_OK             Function succeeded.
 *  - LE_BAD_PARAMETER  Bad parameters.
 *  - LE_FAULT          Function failed.
 *
 * @note The signal delta is set in units of 0.1 dBm. For example, to set a delta of 10.6 dBm, the
 *       delta value must be set to 106.
 *
 * @warning The signal delta resolution is platform dependent. Please refer to
 *          @ref platformConstraintsMdc section for full details.
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_mrc_SetSignalStrengthIndDelta
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_Rat_t rat,
        ///< [IN] Radio Access Technology
        uint16_t delta
        ///< [IN] Signal delta in units of 0.1 dBm
);

//--------------------------------------------------------------------------------------------------
/**
 * Enable the automatic Selection Register mode.
 *
 * @return
 *  - LE_FAULT  Function failed.
 *  - LE_OK     Function succeeded.
 *
 * @note <b>NOT multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_mrc_SetAutomaticRegisterMode
(
    le_msg_SessionRef_t _ifgen_sessionRef
);

//--------------------------------------------------------------------------------------------------
/**
 * Set the manual Selection Register mode with the MCC/MNC parameters.
 *
 * @return
 *  - LE_FAULT  Function failed.
 *  - LE_OK     Function succeeded.
 *
 * @note If strings are not set, too long (bigger than LE_MRC_MCC_LEN/LE_MRC_MNC_LEN digits), or too
 *       short (less than LE_MRC_MCC_LEN/LE_MRC_MNC_LEN-1 digits) it's a fatal error, the function
 *       won't return.
 *
 * @note <b>NOT multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_mrc_SetManualRegisterMode
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        const char* LE_NONNULL mcc,
        ///< [IN] Mobile Country Code
        const char* LE_NONNULL mnc
        ///< [IN] Mobile Network Code
);

//--------------------------------------------------------------------------------------------------
/**
 * Set the manual selection register mode asynchronously. This function is not blocking,
 *  the response will be returned with a handler function.
 *
 * @note If strings are not set, too long (bigger than LE_MRC_MCC_LEN/LE_MRC_MNC_LEN digits), or too
 *       short (less than LE_MRC_MCC_LEN/LE_MRC_MNC_LEN-1 digits) it's a fatal error, the function
 *       won't return.
 *
 *@note <b>NOT multi-app safe</b>
 *
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED void ifgen_le_mrc_SetManualRegisterModeAsync
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        const char* LE_NONNULL mcc,
        ///< [IN] Mobile Country Code
        const char* LE_NONNULL mnc,
        ///< [IN] Mobile Network Code
        le_mrc_ManualSelectionHandlerFunc_t handlerPtr,
        ///< [IN] handler for sending result
        void* contextPtr
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the selected Selection Register mode.
 *
 * @return
 *  - LE_FAULT  Function failed.
 *  - LE_OK     Function succeeded.
 *
 * @note <b>multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_mrc_GetRegisterMode
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        bool* isManualPtrPtr,
        ///< [OUT] true if the scan mode is manual, false if the scan mode is automatic.
        char* mccPtr,
        ///< [OUT] Mobile Country Code
        size_t mccPtrSize,
        ///< [IN]
        char* mncPtr,
        ///< [OUT] Mobile Network Code
        size_t mncPtrSize
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the Packet Switched state.
 *
 * @return
 *  - LE_FAULT  Function failed.
 *  - LE_OK     Function succeeded.
 *
 * @note <b>multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_mrc_GetPacketSwitchedState
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_NetRegState_t* statePtr
        ///< [OUT] The current Packet switched state.
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the platform specific network registration error code.
 *
 * Refer to @ref platformConstraintsSpecificErrorCodes for platform specific
 * registration error code description.
 *
 * @return the platform specific registration error code
 *
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED int32_t ifgen_le_mrc_GetPlatformSpecificRegistrationErrorCode
(
    le_msg_SessionRef_t _ifgen_sessionRef
);

//--------------------------------------------------------------------------------------------------
/**
 * Set the Radio Access Technology preferences by using a bit mask.
 *
 * @return
 *  - LE_FAULT           Function failed.
 *  - LE_OK              Function succeeded.
 *  - LE_UNSUPPORTED     Not supported by platform.
 *
 * @note <b>NOT multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_mrc_SetRatPreferences
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_RatBitMask_t ratMask
        ///< [IN] Bit mask for the Radio Access Technology preferences.
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the Radio Access Technology preferences
 *
 * @return
 * - LE_FAULT  Function failed.
 * - LE_OK     Function succeeded.
 *
 * @note <b>multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_mrc_GetRatPreferences
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_RatBitMask_t* ratMaskPtrPtr
        ///< [OUT] Bit mask for the Radio Access Technology preferences.
);

//--------------------------------------------------------------------------------------------------
/**
 * Set the 2G/3G Band preferences by using a bit mask.
 *
 * @return
 *  - LE_FAULT  Function failed.
 *  - LE_OK     Function succeeded.
 *
 * @note <b>NOT multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_mrc_SetBandPreferences
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_BandBitMask_t bandMask
        ///< [IN] Bit mask for 2G/3G Band preferences.
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the Bit mask for 2G/3G Band preferences.
 *
 * @return
 *  - LE_FAULT  Function failed.
 *  - LE_OK     Function succeeded.
 *
 * @note <b>multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_mrc_GetBandPreferences
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_BandBitMask_t* bandMaskPtrPtr
        ///< [OUT] Bit mask for 2G/3G Band preferences.
);

//--------------------------------------------------------------------------------------------------
/**
 * Set the LTE Band preferences by using a bit mask.
 *
 * @return
 *  - LE_FAULT        Function failed.
 *  - LE_OK           Function succeeded.
 *  - LE_UNSUPPORTED  The platform doesn't support setting LTE Band preferences.
 *
 * @note <b>NOT multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_mrc_SetLteBandPreferences
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_LteBandBitMask_t bandMask
        ///< [IN] Bit mask for LTE Band preferences.
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the Bit mask for LTE Band preferences.
 *
 * @return
 *  - LE_FAULT  Function failed.
 *  - LE_OK     Function succeeded.
 *
 * @note <b>multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_mrc_GetLteBandPreferences
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_LteBandBitMask_t* bandMaskPtrPtr
        ///< [OUT] Bit mask for LTE Band preferences.
);

//--------------------------------------------------------------------------------------------------
/**
 * Set the TD-SCDMA Band preferences by using a bit mask.
 *
 * @return
 *  - LE_OK           Function succeeded.
 *  - LE_FAULT        Function failed.
 *  - LE_UNSUPPORTED  The platform doesn't support setting TD-SCDMA Band preferences.
 *
 * @note <b>NOT multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_mrc_SetTdScdmaBandPreferences
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_TdScdmaBandBitMask_t bandMask
        ///< [IN] Bit mask for TD-SCDMA Band preferences.
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the Bit mask for TD-SCDMA Band preferences.
 *
 * @return
 *  - LE_OK           Function succeeded.
 *  - LE_FAULT        Function failed.
 *  - LE_UNSUPPORTED  The platform doesn't support getting TD-SCDMA Band preferences.
 *
 * @note <b>multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_mrc_GetTdScdmaBandPreferences
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_TdScdmaBandBitMask_t* bandMaskPtrPtr
        ///< [OUT] Bit mask for TD-SCDMA Band preferences.
);

//--------------------------------------------------------------------------------------------------
/**
 * Add a preferred operator by specifying the MCC/MNC and the Radio Access Technology.
 *
 * @return
 *  - LE_UNSUPPORTED   List of User Preferred operators not available.
 *  - LE_FAULT         Function failed.
 *  - LE_BAD_PARAMETER RAT mask is invalid.
 *  - LE_OK            Function succeeded.
 *
 * @note If strings are not set, too long (bigger than LE_MRC_MCC_LEN/LE_MRC_MNC_LEN digits), or too
 *       short (less than LE_MRC_MCC_LEN/LE_MRC_MNC_LEN-1 digits) it's a fatal error, the function
 *       won't return.
 *
 * @note <b>NOT multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_mrc_AddPreferredOperator
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        const char* LE_NONNULL mcc,
        ///< [IN] Mobile Country Code
        const char* LE_NONNULL mnc,
        ///< [IN] Mobile Network Code
        le_mrc_RatBitMask_t ratMask
        ///< [IN] Bit mask for the Radio Access Technology preferences.
);

//--------------------------------------------------------------------------------------------------
/**
 * Remove a preferred operator by specifying the MCC/MNC.
 *
 * @return
 *  - LE_UNSUPPORTED    List of User Preferred operators not available.
 *  - LE_NOT_FOUND      Operator not found in the User Preferred operators list.
 *  - LE_FAULT          Function failed.
 *  - LE_OK             Function succeeded.
 *
 * @note If strings are not set, too long (bigger than LE_MRC_MCC_LEN/LE_MRC_MNC_LEN digits), or too
 *       short (less than LE_MRC_MCC_LEN/LE_MRC_MNC_LEN-1 digits) it's a fatal error, the function
 *       won't return.
 *
 * @note <b>NOT multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_mrc_RemovePreferredOperator
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        const char* LE_NONNULL mcc,
        ///< [IN] Mobile Country Code
        const char* LE_NONNULL mnc
        ///< [IN] Mobile Network Code
);

//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to retrieve a list of the preferred operators.
 *
 * @return
 * - Reference to the List object.
 * - Null pointer if there is no preferences list.
 *
 * @note <b>multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_mrc_PreferredOperatorListRef_t ifgen_le_mrc_GetPreferredOperatorsList
(
    le_msg_SessionRef_t _ifgen_sessionRef
);

//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to get the first Operator object reference in the list of the
 * preferred operators retrieved with le_mrc_GetPreferredOperators().
 *
 * @return
 *  - NULL                          No operator information found.
 *  - le_mrc_PreferredOperatorRef   The Operator object reference.
 *
 * @note If the caller is passing a bad reference into this function, it's a fatal error, the
 *       function won't return.
 *
 * @note <b>multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_mrc_PreferredOperatorRef_t ifgen_le_mrc_GetFirstPreferredOperator
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_PreferredOperatorListRef_t preferredOperatorListRef
        ///< [IN] The list of the preferred operators.
);

//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to get the next Operator object reference in the list of the
 * preferred operators retrieved with le_mrc_GetPreferredOperators().
 *
 * @return
 *  - NULL                          No operator information found.
 *  - le_mrc_PreferredOperatorRef   The Operator object reference.
 *
 * @note If the caller is passing a bad reference into this function, it's a fatal error, the
 *       function won't return.
 *
 * @note <b>multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_mrc_PreferredOperatorRef_t ifgen_le_mrc_GetNextPreferredOperator
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_PreferredOperatorListRef_t preferredOperatorListRef
        ///< [IN] The list of the preferred operators.
);

//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to delete the list of the preferred operators retrieved with
 * le_mrc_GetPreferredOperators().
 *
 * @note On failure, the process exits, so you don't have to worry about checking the returned
 *       reference for validity.
 *
 * @note <b>multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED void ifgen_le_mrc_DeletePreferredOperatorsList
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_PreferredOperatorListRef_t preferredOperatorListRef
        ///< [IN] The list of the preferred operators.
);

//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to get the Operator information details.
 *
 * @return
 *      - LE_OK on success
 *      - LE_OVERFLOW if the MCC or MNC would not fit in buffer
 *      - LE_FAULT for all other errors
 *
 * @note <b>multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_mrc_GetPreferredOperatorDetails
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_PreferredOperatorRef_t preferredOperatorRef,
        ///< [IN] Operator object reference.
        char* mccPtr,
        ///< [OUT] Mobile Country Code.
        size_t mccPtrSize,
        ///< [IN]
        char* mncPtr,
        ///< [OUT] Mobile Network Code.
        size_t mncPtrSize,
        ///< [IN]
        le_mrc_RatBitMask_t* ratMaskPtr
        ///< [OUT] Bit mask for the RAT preferences.
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the network registration state.
 *
 * @return LE_FAULT         The function failed to get the Network registration state.
 * @return LE_BAD_PARAMETER A bad parameter was passed.
 * @return LE_OK            The function succeeded.
 *
 * @note If the caller is passing a bad pointer into this function, it's a fatal error, the
 *       function won't return.
 *
 * @note <b>multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_mrc_GetNetRegState
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_NetRegState_t* statePtr
        ///< [OUT] Network Registration state.
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the signal quality.
 *
 * @return LE_FAULT         The function failed to get the Signal Quality information.
 * @return LE_BAD_PARAMETER A bad parameter was passed.
 * @return LE_OK            The function succeeded.
 *
 * @note If the caller is passing a bad pointer into this function, it's a fatal error, the
 *       function won't return.
 *
 * @note <b>multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_mrc_GetSignalQual
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        uint32_t* qualityPtr
        ///< [OUT] [OUT] Received signal strength quality (0 = no signal strength,
        ///<        5 = very good signal strength).
);

//--------------------------------------------------------------------------------------------------
/**
 * Set the power of the Radio Module.
 *
 * @return LE_BAD_PARAMETER Bad power mode specified.
 * @return LE_FAULT         Function failed.
 * @return LE_OK            Function succeed.
 *
 * @note <b>NOT multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_mrc_SetRadioPower
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_onoff_t power
        ///< [IN] The power state.
);

//--------------------------------------------------------------------------------------------------
/**
 * Must be called to get the Radio Module power state.
 *
 * @return LE_FAULT         The function failed to get the Radio Module power state.
 * @return LE_BAD_PARAMETER if powerPtr is NULL.
 * @return LE_OK            The function succeed.
 *
 * @note If the caller is passing a bad pointer into this function, it's a fatal error, the
 *       function won't return.
 *
 * @note <b>multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_mrc_GetRadioPower
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_onoff_t* powerPtr
        ///< [OUT] Power state.
);

//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to perform a cellular network scan.
 *
 * @return Reference to the List object. Null pointer if the scan failed.
 *
 * @note <b>multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_mrc_ScanInformationListRef_t ifgen_le_mrc_PerformCellularNetworkScan
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_RatBitMask_t ratMask
        ///< [IN] Radio Access Technology mask
);

//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to perform a PCI network scan.
 *
 * @return Reference to the List object. Null pointer if the scan failed.
 *
 * @warning PCI scan is platform dependent. Please refer to @ref platformConstraintsMdc for further
 *          details.
 *
 * @note <b>multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_mrc_PciScanInformationListRef_t ifgen_le_mrc_PerformPciNetworkScan
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_RatBitMask_t ratMask
        ///< [IN] Radio Access Technology mask
);

//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to perform a cellular network scan asynchronously. This function
 * is not blocking, the response will be returned with a handler function.
 *
 *@note <b>multi-app safe</b>
 *
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED void ifgen_le_mrc_PerformCellularNetworkScanAsync
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_RatBitMask_t ratMask,
        ///< [IN] Radio Access Technology mask
        le_mrc_CellularNetworkScanHandlerFunc_t handlerPtr,
        ///< [IN] handler for sending result.
        void* contextPtr
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to perform a PCI network scan asynchronously. This function
 * is not blocking, the response will be returned with a handler function.
 *
 * @warning PCI scan is platform dependent. Please refer to @ref platformConstraintsMdc for further
 *          details.
 *
 *@note <b>multi-app safe</b>
 *
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED void ifgen_le_mrc_PerformPciNetworkScanAsync
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_RatBitMask_t ratMask,
        ///< [IN] Radio Access Technology mask
        le_mrc_PciNetworkScanHandlerFunc_t handlerPtr,
        ///< [IN] handler for sending result.
        void* contextPtr
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to get the first Scan Information object reference in the list of
 * scan Information retrieved with le_mrc_PerformCellularNetworkScan().
 *
 * @return NULL                         No scan information found.
 * @return le_mrc_ScanInformationRef_t  The Scan Information object reference.
 *
 * @note If the caller is passing a bad pointer into this function, it's a fatal error, the
 *       function won't return.
 *
 * @note <b>multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_mrc_ScanInformationRef_t ifgen_le_mrc_GetFirstCellularNetworkScan
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_ScanInformationListRef_t scanInformationListRef
        ///< [IN] The list of scan information.
);

//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to get the next Scan Information object reference in the list of
 * scan Information retrieved with le_mrc_PerformCellularNetworkScan().
 *
 * @return NULL                         No scan information found.
 * @return le_mrc_ScanInformationRef_t  The Scan Information object reference.
 *
 * @note If the caller is passing a bad pointer into this function, it's a fatal error, the
 *       function won't return.
 *
 * @note <b>multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_mrc_ScanInformationRef_t ifgen_le_mrc_GetNextCellularNetworkScan
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_ScanInformationListRef_t scanInformationListRef
        ///< [IN] The list of scan information.
);

//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to get the first Pci Scan Information object reference in the list of
 * scan Information retrieved with le_mrc_PerformPciNetworkScan().
 *
 * @return NULL                         No scan information found.
 * @return The Scan Information object reference.
 *
 * @note If the caller is passing a bad pointer into this function, it's a fatal error, the
 *       function won't return.
 *
 * @note <b>multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_mrc_PciScanInformationRef_t ifgen_le_mrc_GetFirstPciScanInfo
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_PciScanInformationListRef_t scanInformationListRef
        ///< [IN] The list of scan information.
);

//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to get the next Scan Information object reference in the list of
 * scan Information retrieved with le_mrc_PerformPciNetworkScan().
 *
 * @return NULL                         No scan information found.
 * @return The Scan Information object reference.
 *
 * @note If the caller is passing a bad pointer into this function, it's a fatal error, the
 *       function won't return.
 *
 * @note <b>multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_mrc_PciScanInformationRef_t ifgen_le_mrc_GetNextPciScanInfo
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_PciScanInformationListRef_t scanInformationListRef
        ///< [IN] The list of scan information.
);

//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to get the first Plmn Information object reference in the list of
 * Plmn on each cell.
 *
 * @return NULL                         No scan information found.
 * @return le_mrc_PlmnInformationRef_t  The Plmn Information object reference.
 *
 * @note If the caller is passing a bad pointer into this function, it's a fatal error, the
 *       function won't return.
 *
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_mrc_PlmnInformationRef_t ifgen_le_mrc_GetFirstPlmnInfo
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_PciScanInformationRef_t pciScanInformationRef
        ///< [IN] [IN] The reference to the cell information.
);

//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to get the next Plmn Information object reference in the list of
 * Plmn on each cell.
 *
 * @return NULL                         No scan information found.
 * @return le_mrc_PlmnInformationRef_t  The Plmn Information object reference.
 *
 * @note If the caller is passing a bad pointer into this function, it's a fatal error, the
 *       function won't return.
 *
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_mrc_PlmnInformationRef_t ifgen_le_mrc_GetNextPlmnInfo
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_PciScanInformationRef_t plmnRef
        ///< [IN] [IN] The reference to the cell information.
);

//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to get the physical cell id referenced by PciScanInformation which
 * is returned by le_mrc_GetFirstPciScanInfo() and le_mrc_GetNextPciScanInfo().
 *
 * @return The Physical Cell Identifier.
 *
 * @note If the caller is passing a bad pointer into this function, it's a fatal error, the
 *       function won't return.
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED uint16_t ifgen_le_mrc_GetPciScanCellId
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_PciScanInformationRef_t pciScanInformationRef
        ///< [IN] [IN] The reference to the cell information.
);

//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to get the global cell id referenced by PciScanInformation which is
 * returned by le_mrc_GetFirstPciScanInfo() and le_mrc_GetNextPciScanInfo().
 *
 * @return The Global Cell Identifier.
 *
 * @note If the caller is passing a bad pointer into this function, it's a fatal error, the
 *       function won't return.
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED uint32_t ifgen_le_mrc_GetPciScanGlobalCellId
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_PciScanInformationRef_t pciScanInformationRef
        ///< [IN] [IN] The reference to the cell information.
);

//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to get the Mcc/Mnc of each PLMN referenced in the list of
 * Plmn Information retrieved with le_mrc_GetFirstPlmnInfo() and le_mrc_GetNextPlmnInfo().
 *
 * @return
 *      - LE_OK on success
 *      - LE_OVERFLOW if the MCC or MNC would not fit in buffer
 *      - LE_FAULT for all other errors
 *
 *
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_mrc_GetPciScanMccMnc
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_PlmnInformationRef_t plmnRef,
        ///< [IN] [IN] The reference to the cell information.
        char* mccPtr,
        ///< [OUT] Mobile Country Code
        size_t mccPtrSize,
        ///< [IN]
        char* mncPtr,
        ///< [OUT] Mobile Network Code
        size_t mncPtrSize
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to delete the list of the Scan Information retrieved with
 * le_mrc_PerformCellularNetworkScan().
 *
 * @note
 *      On failure, the process exits, so you don't have to worry about checking the returned
 *      reference for validity.
 *
 * @note <b>multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED void ifgen_le_mrc_DeleteCellularNetworkScan
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_ScanInformationListRef_t scanInformationListRef
        ///< [IN] The list of scan information.
);

//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to delete the list of the Pci Scan Information retrieved with
 * le_mrc_PerformPciNetworkScan() or le_mrc_PerformPciNetworkScanAsync().
 *
 * @note
 *      On failure, the process exits, so you don't have to worry about checking the returned
 *      reference for validity.
 *
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED void ifgen_le_mrc_DeletePciNetworkScan
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_PciScanInformationListRef_t scanInformationListRef
        ///< [IN] The list of scan information.
);

//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to get the Cellular Network Code [mcc:mnc]
 *
 * @return
 *      - LE_OK on success
 *      - LE_OVERFLOW if the MCC or MNC would not fit in buffer
 *      - LE_FAULT for all other errors
 *
 * @note On failure, the process exits, so you don't have to worry about checking the returned
 *       reference for validity.
 *
 * @note <b>multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_mrc_GetCellularNetworkMccMnc
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_ScanInformationRef_t scanInformationRef,
        ///< [IN] Scan information reference
        char* mccPtr,
        ///< [OUT] Mobile Country Code
        size_t mccPtrSize,
        ///< [IN]
        char* mncPtr,
        ///< [OUT] Mobile Network Code
        size_t mncPtrSize
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to get the Cellular Network Name.
 *
 * @return
 *      - LE_OK on success
 *      - LE_OVERFLOW if the operator name would not fit in buffer
 *      - LE_FAULT for all other errors
 *
 * @note On failure, the process exits, so you don't have to worry about checking the returned
 *       reference for validity.
 *
 * @note <b>multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_mrc_GetCellularNetworkName
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_ScanInformationRef_t scanInformationRef,
        ///< [IN] Scan information reference
        char* namePtr,
        ///< [OUT] Name of operator
        size_t namePtrSize
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to get the radio access technology of a scanInformationRef.
 *
 * @return the radio access technology
 *
 * @note On failure, the process exits.
 *
 * @note <b>multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_mrc_Rat_t ifgen_le_mrc_GetCellularNetworkRat
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_ScanInformationRef_t scanInformationRef
        ///< [IN] Scan information reference
);

//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to check if a cellular network is currently in use.
 *
 * @return true     The network is in use
 * @return false    The network isn't in use
 *
 * @note On failure, the process exits, so you don't have to worry about checking the returned
 *       reference for validity.
 *
 * @note <b>multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED bool ifgen_le_mrc_IsCellularNetworkInUse
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_ScanInformationRef_t scanInformationRef
        ///< [IN] Scan information reference
);

//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to check if a cellular network is available.
 *
 * @return true     The network is available
 * @return false    The network isn't available
 *
 * @note On failure, the process exits, so you don't have to worry about checking the returned
 *       reference for validity.
 *
 * @note <b>multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED bool ifgen_le_mrc_IsCellularNetworkAvailable
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_ScanInformationRef_t scanInformationRef
        ///< [IN] Scan information reference
);

//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to check if a cellular network is currently in home mode.
 *
 * @return true     The network is home
 * @return false    The network is roaming
 *
 * @note On failure, the process exits, so you don't have to worry about checking the returned
 *       reference for validity.
 *
 * @note <b>multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED bool ifgen_le_mrc_IsCellularNetworkHome
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_ScanInformationRef_t scanInformationRef
        ///< [IN] Scan information reference
);

//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to check if a cellular network is forbidden by the operator.
 *
 * @return true     The network is forbidden
 * @return false    The network is allowed
 *
 * @note On failure, the process exits, so you don't have to worry about checking the returned
 *       reference for validity.
 *
 * @note <b>multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED bool ifgen_le_mrc_IsCellularNetworkForbidden
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_ScanInformationRef_t scanInformationRef
        ///< [IN] Scan information reference
);

//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to get the current network name information.
 *
 * @return
 *      - LE_OK             on success
 *      - LE_BAD_PARAMETER  if nameStr is NULL
 *      - LE_OVERFLOW       if the Home Network Name can't fit in nameStr
 *      - LE_FAULT          on any other failure
 *
 * @note If the caller is passing a bad pointer into this function, it's a fatal error, the
 *       function won't return.
 *
 * @note <b>multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_mrc_GetCurrentNetworkName
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        char* nameStr,
        ///< [OUT] the home network Name
        size_t nameStrSize
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to get the current network PLMN information.
 *
 * @return
 *      - LE_OK       on success
 *      - LE_FAULT    on any other failure
 *
 * @note <b>multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_mrc_GetCurrentNetworkMccMnc
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        char* mccStr,
        ///< [OUT] the mobile country code
        size_t mccStrSize,
        ///< [IN]
        char* mncStr,
        ///< [OUT] the mobile network code
        size_t mncStrSize
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to get the current Radio Access Technology in use.
 *
 * @return LE_OK            Function succeeded.
 * @return LE_BAD_PARAMETER Invalid parameter.
 * @return LE_FAULT         Function failed to get the Radio Access Technology.
 *
 * @note If the caller is passing a bad pointer into this function, it's a fatal error, the
 *       function won't return.
 *
 * @note The API returns the RAT only if the device is registered on the network.
 *       le_mrc_GetNetRegState() function can be called first to obtain the network registration
 *       state.
 *
 * @note <b>multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_mrc_GetRadioAccessTechInUse
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_Rat_t* ratPtr
        ///< [OUT] The Radio Access Technology.
);

//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to retrieve the Neighboring Cells information. It creates and
 * returns a reference to the Neighboring Cells information.
 *
 * @return A reference to the Neighboring Cells information.
 * @return NULL if no Cells Information are available.
 *
 * @note <b>multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_mrc_NeighborCellsRef_t ifgen_le_mrc_GetNeighborCellsInfo
(
    le_msg_SessionRef_t _ifgen_sessionRef
);

//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to delete the Neighboring Cells information.
 *
 * @note On failure, the process exits, so you don't have to worry about checking the returned
 *       reference for validity.
 *
 * @note <b>multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED void ifgen_le_mrc_DeleteNeighborCellsInfo
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_NeighborCellsRef_t ngbrCellsRef
        ///< [IN] Neighboring Cells reference.
);

//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to get the first Cell Information reference in the list of
 * Neighboring Cells information retrieved with le_mrc_GetNeighborCellsInfo().
 *
 * @return NULL                   No Cell information object found.
 * @return le_mrc_CellInfoRef_t   The Cell information object reference.
 *
 * @note If the caller is passing a bad pointer into this function, it's a fatal error, the
 *       function won't return.
 *
 * @note <b>multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_mrc_CellInfoRef_t ifgen_le_mrc_GetFirstNeighborCellInfo
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_NeighborCellsRef_t ngbrCellsRef
        ///< [IN] Neighboring Cells reference.
);

//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to get the next Cell Information reference in the list of
 * Neighboring Cells information retrieved with le_mrc_GetNeighborCellsInfo().
 *
 * @return NULL                   No Cell information object found.
 * @return le_mrc_CellInfoRef_t   Cell information object reference.
 *
 * @note If the caller is passing a bad pointer into this function, it's a fatal error, the
 *       function won't return.
 *
 * @note <b>multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_mrc_CellInfoRef_t ifgen_le_mrc_GetNextNeighborCellInfo
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_NeighborCellsRef_t ngbrCellsRef
        ///< [IN] Neighboring Cells reference.
);

//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to get the Physical Cell Identifier.
 *
 * @return The Physical Cell Identifier. UINT32_MAX value is returned if the Cell Identifier is not
 * available.
 *
 * @note If the caller is passing a bad pointer into this function, it's a fatal error, the
 *       function won't return.
 *
 * @note <b>multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED uint32_t ifgen_le_mrc_GetNeighborCellId
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_CellInfoRef_t ngbrCellInfoRef
        ///< [IN] Cell information reference.
);

//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to get the Location Area Code of a cell.
 *
 * @return The Location Area Code of a cell. UINT16_MAX value is returned if the value is not
 * available.
 *
 * @note If the caller is passing a bad pointer into this function, it's a fatal error, the
 *       function won't return.
 *
 * @note <b>multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED uint32_t ifgen_le_mrc_GetNeighborCellLocAreaCode
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_CellInfoRef_t ngbrCellInfoRef
        ///< [IN] Cell information reference.
);

//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to get the signal strength of a cell.
 *
 * @return The signal strength of a cell in dBm.
 *
 * @note If the caller is passing a bad pointer into this function, it's a fatal error, the
 *       function won't return.
 *
 * @note <b>multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED int32_t ifgen_le_mrc_GetNeighborCellRxLevel
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_CellInfoRef_t ngbrCellInfoRef
        ///< [IN] Cell information reference.
);

//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to get the Radio Access Technology of a cell.
 *
 * @return The Radio Access Technology of a cell.
 *
 * @note If the caller is passing a bad pointer into this function, it's a fatal error, the
 *       function won't return.
 *
 * @note <b>multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_mrc_Rat_t ifgen_le_mrc_GetNeighborCellRat
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_CellInfoRef_t ngbrCellInfoRef
        ///< [IN] Cell information reference.
);

//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to get the Ec/Io; the received energy per chip divided by the power
 * density in the band measured in dBm on the primary CPICH channel of serving cell (negative value)
 *
 * @return
 *  - The Ec/Io of a cell given in dB with 1 decimal place. (only applicable for UMTS network).
 *  - INT32_MAX when the value isn't available.
 *
 * @note If the caller is passing a bad pointer into this function, it's a fatal error, the
 *       function won't return.
 *
 * @note <b>multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED int32_t ifgen_le_mrc_GetNeighborCellUmtsEcIo
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_CellInfoRef_t ngbrCellInfoRef
        ///< [IN] Cell information reference.
);

//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to get the RSRP and RSRQ of the Intrafrequency of a LTE cell.
 *
 * @return
 *      - LE_OK on success
 *      - LE_FAULT on failure
 *
 * @note If the caller is passing a bad pointer into this function, it's a fatal error, the
 *       function won't return.
 *
 * @note <b>multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_mrc_GetNeighborCellLteIntraFreq
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_CellInfoRef_t ngbrCellInfoRef,
        ///< [IN] Cell information reference.
        int32_t* rsrqPtr,
        ///< [OUT] Reference Signal Received Quality value in dB with 1 decimal
        ///< place
        int32_t* rsrpPtr
        ///< [OUT] Reference Signal Receiver Power value in dBm with 1 decimal
        ///< place
);

//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to get the RSRP and RSRQ of the Interfrequency of a LTE cell.
 *
 * @return
 *      - LE_OK on success
 *      - LE_FAULT on failure
 *
 * @note If the caller is passing a bad pointer into this function, it's a fatal error, the
 *       function won't return.
 *
 * @note <b>multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_mrc_GetNeighborCellLteInterFreq
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_CellInfoRef_t ngbrCellInfoRef,
        ///< [IN] Cell information reference.
        int32_t* rsrqPtr,
        ///< [OUT] Reference Signal Received Quality value in dB with 1 decimal
        ///< place
        int32_t* rsrpPtr
        ///< [OUT] Reference Signal Receiver Power value in dBm with 1 decimal
        ///< place
);

//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to measure the signal metrics. It creates and returns a reference
 * to the signal metrics.
 *
 * @return A reference to the signal metrics.
 * @return NULL if no signal metrics are available.
 *
 * @note <b>multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_mrc_MetricsRef_t ifgen_le_mrc_MeasureSignalMetrics
(
    le_msg_SessionRef_t _ifgen_sessionRef
);

//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to delete the the signal metrics.
 *
 * @note On failure, the process exits, so you don't have to worry about checking the returned
 *       reference for validity.
 *
 * @note <b>multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED void ifgen_le_mrc_DeleteSignalMetrics
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_MetricsRef_t MetricsRef
        ///< [IN] Signal metrics reference.
);

//--------------------------------------------------------------------------------------------------
/**
 * This function returns the Radio Access Technology of the signal metrics.
 *
 * @return The Radio Access Technology of the signal measure.
 *
 * @note If the caller is passing a bad pointer into this function, it's a fatal error, the
 *       function won't return.
 *
 * @note <b>multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_mrc_Rat_t ifgen_le_mrc_GetRatOfSignalMetrics
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_MetricsRef_t MetricsRef
        ///< [IN] Signal metrics reference.
);

//--------------------------------------------------------------------------------------------------
/**
 * This function returns the signal strength in dBm and the bit error rate measured on GSM network.
 *
 * @return
 *      - LE_OK on success
 *      - LE_FAULT on failure
 *
 * @note If the caller is passing a bad pointer into this function, it's a fatal error, the
 *       function won't return.
 *
 * @note <b>multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_mrc_GetGsmSignalMetrics
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_MetricsRef_t MetricsRef,
        ///< [IN] Signal metrics reference.
        int32_t* rssiPtr,
        ///< [OUT] Signal strength in dBm
        uint32_t* berPtr
        ///< [OUT] Bit error rate.
);

//--------------------------------------------------------------------------------------------------
/**
 * This function returns the signal metrics measured on UMTS or TD-SCDMA networks.
 *
 * @return
 *      - LE_OK on success
 *      - LE_FAULT on failure
 *
 * @note If the caller is passing a bad pointer into this function, it's a fatal error, the
 *       function won't return.
 *
 * @note <b>multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_mrc_GetUmtsSignalMetrics
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_MetricsRef_t MetricsRef,
        ///< [IN] Signal metrics reference.
        int32_t* ssPtr,
        ///< [OUT] Signal strength in dBm
        uint32_t* blerPtr,
        ///< [OUT] Block error rate
        int32_t* ecioPtr,
        ///< [OUT] Ec/Io value in dB with 1 decimal place (-15 = -1.5 dB) (Negative
        ///< value)
        int32_t* rscpPtr,
        ///< [OUT] Measured RSCP in dBm (negative value, value INT32_MAX means
        ///< that RSCP is not available)
        int32_t* sinrPtr
        ///< [OUT] Measured SINR in dB (only applicable for TD-SCDMA network, value
        ///< INT32_MAX means that the SINR is not available)
);

//--------------------------------------------------------------------------------------------------
/**
 * This function returns the signal metrics measured on LTE network.
 *
 * @return
 *      - LE_OK on success
 *      - LE_FAULT on failure
 *      - sinrPtr and ioPtr return INT32_MAX when the value isn't available.
 *
 * @note If the caller is passing a bad pointer into this function, it's a fatal error, the
 *       function won't return.
 *
 * @note <b>multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_mrc_GetLteSignalMetrics
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_MetricsRef_t MetricsRef,
        ///< [IN] Signal metrics reference.
        int32_t* ssPtr,
        ///< [OUT] Signal strength in dBm
        uint32_t* blerPtr,
        ///< [OUT] Block error rate
        int32_t* rsrqPtr,
        ///< [OUT] RSRQ value in dB as measured by L1 with 1 decimal place
        int32_t* rsrpPtr,
        ///< [OUT] Current RSRP in dBm as measured by L1 with 1 decimal place
        int32_t* snrPtr
        ///< [OUT] SNR level in dB with 1 decimal place (15 = 1.5 dB)
);

//--------------------------------------------------------------------------------------------------
/**
 * This function returns the signal metrics measured on CDMA network.
 *
 * @return
 *      - LE_OK on success
 *      - LE_FAULT on failure
 *      - rscpPtr and sinrPtr return INT32_MAX when the value isn't available.
 *
 * @note If the caller is passing a bad pointer into this function, it's a fatal error, the
 *       function won't return.
 *
 * @note <b>multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_mrc_GetCdmaSignalMetrics
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_MetricsRef_t MetricsRef,
        ///< [IN] Signal metrics reference.
        int32_t* ssPtr,
        ///< [OUT] Signal strength in dBm
        uint32_t* erPtr,
        ///< [OUT] Frame/Packet error rate
        int32_t* ecioPtr,
        ///< [OUT] ECIO value in dB with 1 decimal place (-15 = -1.5 dB) (Negative
        ///< value)
        int32_t* sinrPtr,
        ///< [OUT] SINR level in dB with 1 decimal place, (only applicable for
        ///< 1xEV-DO, value INT32_MAX means that the value is not available)
        int32_t* ioPtr
        ///< [OUT] Received IO in dBm (only applicable for 1xEV-DO, value INT32_MAX
        ///< means that the value is not available)
);

//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to get the serving Cell Identifier.
 *
 * @return The Cell Identifier. UINT32_MAX value is returned if the value is not available.
 *
 * @note When the module is in UMTS network, the API returns the serving UTRAN Cell Identity (UC-Id)
 * which is used to identify the cell uniquely.
 * It is composed of the Controlling Radio Network Controller Identifier (CRNC-Id, 12 bits) and the
 * Cell Identifier (C-Id, 16 bits). (3GPP 25.401, section 6.1.5)
 * The Cell Identifier is coded in the lower 2 bytes of the 4 bytes UC-Id and the CRNC-Id is coded
 * in the upper 2 bytes.
 * Example: the API returns 7807609 -> 0x772279 (CRNC-Id = 0x77 , cell ID = 0x2279)
 *
 * @note <b>multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED uint32_t ifgen_le_mrc_GetServingCellId
(
    le_msg_SessionRef_t _ifgen_sessionRef
);

//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to get the Location Area Code of the serving cell.
 *
 * @return The Location Area Code. UINT32_MAX value is returned if the value is not available.
 *
 * @note <b>multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED uint32_t ifgen_le_mrc_GetServingCellLocAreaCode
(
    le_msg_SessionRef_t _ifgen_sessionRef
);

//--------------------------------------------------------------------------------------------------
/**
 * This function must be called to get the Tracking Area Code of the serving cell (LTE only).
 *
 * @return The Tracking Area Code. UINT16_MAX value is returned if the value is not available.
 *
 * @note <b>multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED uint16_t ifgen_le_mrc_GetServingCellLteTracAreaCode
(
    le_msg_SessionRef_t _ifgen_sessionRef
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the Bit mask for 2G/3G Band capabilities.
 *
 * @return
 *  - LE_OK              on success
 *  - LE_FAULT           on failure
 *  - LE_UNSUPPORTED     Unable to get the 2G/3G Band capabilities on this platform
 *
 * @note <b>multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_mrc_GetBandCapabilities
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_BandBitMask_t* bandMaskPtrPtr
        ///< [OUT] Bit mask for 2G/3G Band capabilities.
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the Bit mask for LTE Band capabilities.
 *
 * @return
 *  - LE_OK              on success
 *  - LE_FAULT           on failure
 *  - LE_UNSUPPORTED     Unable to get the LTE Band capabilities on this platform
 *
 * @note <b>multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_mrc_GetLteBandCapabilities
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_LteBandBitMask_t* bandMaskPtrPtr
        ///< [OUT] Bit mask for LTE Band capabilities.
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the Bit mask for TD-SCDMA Band capabilities.
 *
 * @return
 *  - LE_OK              on success
 *  - LE_FAULT           on failure
 *  - LE_UNSUPPORTED     Unable to get the TD-SCDMA Band Capabilities on this platform
 *
 * @note <b>multi-app safe</b>
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_mrc_GetTdScdmaBandCapabilities
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_TdScdmaBandBitMask_t* bandMaskPtrPtr
        ///< [OUT] Bit mask for TD-SCDMA Band capabilities.
);

//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'le_mrc_NetworkReject'
 *
 * Event to report network reject indication.
 *
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_mrc_NetworkRejectHandlerRef_t ifgen_le_mrc_AddNetworkRejectHandler
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_NetworkRejectHandlerFunc_t handlerPtr,
        ///< [IN]
        void* contextPtr
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'le_mrc_NetworkReject'
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED void ifgen_le_mrc_RemoveNetworkRejectHandler
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_NetworkRejectHandlerRef_t handlerRef
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'le_mrc_NetRegReject'
 *
 * Event to report network registration reject indication.
 * NetworkReject will be deprecated.
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_mrc_NetRegRejectHandlerRef_t ifgen_le_mrc_AddNetRegRejectHandler
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_NetRegRejectHandlerFunc_t handlerPtr,
        ///< [IN]
        void* contextPtr
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'le_mrc_NetRegReject'
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED void ifgen_le_mrc_RemoveNetRegRejectHandler
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_NetRegRejectHandlerRef_t handlerRef
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'le_mrc_JammingDetectionEvent'
 *
 * This event provides information on jamming detection.
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_mrc_JammingDetectionEventHandlerRef_t ifgen_le_mrc_AddJammingDetectionEventHandler
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_JammingDetectionHandlerFunc_t handlerPtr,
        ///< [IN]
        void* contextPtr
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'le_mrc_JammingDetectionEvent'
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED void ifgen_le_mrc_RemoveJammingDetectionEventHandler
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        le_mrc_JammingDetectionEventHandlerRef_t handlerRef
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Start the jamming detection monitoring.
 *
 * @warning The jamming detection feature might be limited by the platform.
 *          Please refer to the platform documentation @ref platformConstraintsMdc.
 *
 * @return
 *  - LE_OK             The function succeeded.
 *  - LE_FAULT          The function failed.
 *  - LE_DUPLICATE      The feature is already activated and an activation is requested.
 *  - LE_UNSUPPORTED    The feature is not supported.
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_mrc_StartJammingDetection
(
    le_msg_SessionRef_t _ifgen_sessionRef
);

//--------------------------------------------------------------------------------------------------
/**
 * Stop the jamming detection monitoring.
 *
 * @return
 *  - LE_OK             The function succeeded.
 *  - LE_FAULT          The function failed or the application did not start the jamming detection.
 *  - LE_UNSUPPORTED    The feature is not supported.
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_mrc_StopJammingDetection
(
    le_msg_SessionRef_t _ifgen_sessionRef
);

//--------------------------------------------------------------------------------------------------
/**
 * Set the SAR backoff state
 *
 * @warning The SAR backoff feature might be unsupported by some platforms.
 *          Please refer to the platform documentation @ref platformConstraintsMdc.
 *
 * @return
 *  - LE_OK             The function succeeded.
 *  - LE_FAULT          The function failed.
 *  - LE_UNSUPPORTED    The feature is not supported.
 *  - LE_OUT_OF_RANGE   The provided index is out of range.
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_mrc_SetSarBackoffState
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        uint8_t state
        ///< [IN] New state to enable. By default, SAR is disabled (state = 0).
        ///< Refer to @ref platformConstraitsMdc for the number of maximum states.
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the SAR backoff state
 *
 * @warning The SAR backoff feature might be unsupported by some platforms.
 *          Please refer to the platform documentation @ref platformConstraintsMdc.
 *
 * @return
 *  - LE_OK             The function succeeded.
 *  - LE_FAULT          The function failed.
 *  - LE_UNSUPPORTED    The feature is not supported.
 */
//--------------------------------------------------------------------------------------------------
LE_SHARED le_result_t ifgen_le_mrc_GetSarBackoffState
(
    le_msg_SessionRef_t _ifgen_sessionRef,
        uint8_t* statePtr
        ///< [OUT] Current state. By default, SAR is disabled (state = 0).
        ///< Refer to @ref platformConstraitsMdc for the number of maximum states.
);

#endif // LE_MRC_COMMON_H_INCLUDE_GUARD
