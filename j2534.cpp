//*****************************************************************************
//					Dearborn Group Technology
//*****************************************************************************
// Project Name			: J2534 Gryphon API
// File Name			: J2534.h
// Description			: This is the main header file for J2534
// Date					: May 07, 2001
// Version				: 1.0
// Author				: Sanjay Mehta
// Revision				: 
//*****************************************************************************
//
//	Last Revised by		:	$Author: chang $
//  Last Modified on	:	$Date: 2003/08/27 13:59:59 $
//	Revision #			:	$Revision: 1.3 $
//	Previous Revision Comments
//	$Log: J2534.h,v $
//	Revision 1.3  2003/08/27 13:59:59  chang
//	*** empty log message ***
//	
//	Revision 1.10  2003/07/22 17:30:06  chang
//	*** empty log message ***
//	
//	Revision 1.9  2003/06/18 12:48:23  chang
//	*** empty log message ***
//	
//	Revision 1.8  2003/05/23 19:38:14  chang
//	*** empty log message ***
//	
//	Revision 1.7  2003/02/07 22:04:04  chang
//	*** empty log message ***
//	
//	Revision 1.6  2002/08/08 20:13:16  chang
//	*** empty log message ***
//	

#ifndef _J2534_H_
#define _J2534_H_

#define PASSTHRU_MSG_DATA_SIZE		4128

#define J2534_TX_MSG_BUFFER_SIZE	4096
#define J2534_RX_MSG_BUFFER_SIZE	4096

// J2534 Message Structure
typedef struct
{
	unsigned long ulProtocolID;
	unsigned long ulRxStatus;
	unsigned long ulTxFlags;
	unsigned long ulTimeStamp;
	unsigned long ulDataSize;
	unsigned long ulExtraDataIndex;
	unsigned char ucData[PASSTHRU_MSG_DATA_SIZE];
}
PASSTHRU_MSG;

#define J2534_TX_MSG_SIZE	sizeof(PASSTHRU_MSG)
#define J2534_RX_MSG_SIZE	sizeof(PASSTHRU_MSG)

// J2534 API Document Version String.
#define J2534_APIDOC_VERSION TEXT("03.05")

// Protocols supported
typedef enum
{
	J1850VPW = 0x01,						// J1850VPW Protocol
	J1850PWM,								// J1850PWM Protocol
	ISO9141,								// ISO9141 Protocol
	ISO14230,								// ISO14230 Protocol
	CAN,									// CAN Protocol
	ISO15765,
	SCI_A_ENGINE,
	SCI_A_TRANS,
	SCI_B_ENGINE,
	SCI_B_TRANS,
	ISO9141_FORD = 0x10000,					// ISO9141 FORD Protocol
	UBP,									// UBP Protocol
	DDL,									// DDL Protocol
	//**** NOTE **** 
	//		 ALWAYS ADD NEW PROTOCOL
	//		 BEFORE J2534_PROTOCOL_NUM
	//		 SO THAT THIS VALUE AUTOMATICALLY
	//		 GETS UPDATED WHEN A NEW PROTOCOL
	//		 IS ADDED.
	J2534_PROTOCOL_NUM
} 
J2534_PROTOCOL;

// J2534 Error codes
typedef enum
{
	J2534_STATUS_NOERROR,				// Function call successful.
	J2534_ERR_NOT_SUPPORTED,			// Function not supported.
	J2534_ERR_INVALID_CHANNEL_ID,		// Invalid ChannelID value.
	J2534_ERR_INVALID_PROTOCOL_ID,		// Invalid ProtocolID value.
	J2534_ERR_NULLPARAMETER,			// NULL pointer supplied where a valid 
										// pointer
										// is required.
	J2534_ERR_INVALID_IOCTL_VALUE,		// Invalid value for Ioctl parameter 
	J2534_ERR_INVALID_FLAGS,			// Invalid flag values.
	J2534_ERR_FAILED,					// Undefined error, use 
										// PassThruGetLastError for description
										// of error.
	J2534_ERR_DEVICE_NOT_CONNECTED,		// Device not connected to PC
	J2534_ERR_TIMEOUT,					// Timeout. No message available to 
										// read or 
										// could not read the specified no. of 
										// msgs.
	J2534_ERR_INVALID_MSG,				// Invalid message structure pointed 
										// to by pMsg.
	J2534_ERR_INVALID_TIME_INTERVAL,	// Invalid TimeInterval value.
	J2534_ERR_EXCEEDED_LIMIT,			// ALL periodic message IDs have been 
										// used.
	J2534_ERR_INVALID_MSG_ID,			// Invalid MsgID value.
	J2534_ERR_INVALID_ERROR_ID,			// Invalid ErrorID value.
	J2534_ERR_INVALID_IOCTL_ID,			// Invalid IoctlID value.
	J2534_ERR_BUFFER_EMPTY,				// Protocol message buffer empty.
	J2534_ERR_BUFFER_FULL,				// Protocol message buffer full.
	J2534_ERR_BUFFER_OVERFLOW,			// Protocol message buffer overflow.
	J2534_ERR_PIN_INVALID,				// Invalid pin number.
	J2534_ERR_CHANNEL_IN_USE,			// Channel already in use.
	J2534_ERR_MSG_PROTOCOL_ID,			// Protocol type does not match the
										// protocol associated with the 
										// Channel ID
	J2534_ERR_INVALID_FILTER_ID,		// Invalid MsgID value
	J2534_ERROR_NO_FLOW_CONTROL			// An attempt was made to send a message
										// on an ISO15765 ChannelID before a flow
										// control filter was established.
} 
J2534ERROR;

// Filter Types
// Filter Message Error codes
typedef enum
{
	J2534_FILTER_NONE,
	J2534_FILTER_PASS,
	J2534_FILTER_BLOCK,
	J2534_FILTER_FLOW_CONTROL
} 
J2534_FILTER;

// Ioctl ID Values
typedef enum
{
	GET_CONFIG = 0x01,
	SET_CONFIG,
	READ_VBATT,
	FIVE_BAUD_INIT,
	FAST_INIT,
	SET_PIN_USE,
	CLEAR_TX_BUFFER,
	CLEAR_RX_BUFFER,
	CLEAR_PERIODIC_MSGS,
	CLEAR_MSG_FILTERS,
	CLEAR_FUNCT_MSG_LOOKUP_TABLE,
	ADD_TO_FUNCT_MSG_LOOKUP_TABLE,
	DELETE_FROM_FUNCT_MSG_LOOKUP_TABLE,
	READ_PROG_VOLTAGE
}
J2534IOCTLID;

// Ioctl Parameter ID Values
typedef enum
{
	DATA_RATE = 0x01,
	LOOPBACK = 0x03,
	NODE_ADDRESS,
	NETWORK_LINE,
	P1_MIN,
	P1_MAX,
	P2_MIN,
	P2_MAX,
	P3_MIN,
	P3_MAX,
	P4_MIN,
	P4_MAX,
	W1,
	W2,
	W3,
	W4,
	W5,
	TIDLE,
	TINIL,
	TWUP,
	PARITY,
	BIT_SAMPLE_POINT,
	SYNC_JUMP_WIDTH,
	W0,
	T1_MAX,
	T2_MAX,
	T4_MAX,
	T5_MAX,
	ISO15765_BS,
	ISO15765_STMIN,
	DATA_BITS
}
J2534IOCTLPARAMID;

// SCONFIG Structure
typedef struct
{
	J2534IOCTLPARAMID Parameter;	// name of parameter
	unsigned long ulValue;			// value of the parameter
}
SCONFIG;

// SCONFIG_LIST Structure
typedef struct
{
	unsigned long ulNumOfParams;	// number of SCONFIG elements
	SCONFIG *pConfigPtr;			// array of SCONFIG
}
SCONFIG_LIST;

// SBYTE_ARRAY Structure
typedef struct
{
	unsigned long ulNumOfBytes;		// number of bytes in the array
	unsigned char *pucBytePtr;		// array of bytes
}
SBYTE_ARRAY;

// Declaration of Exported functions.
extern "C" J2534ERROR WINAPI PassThruConnect(
										J2534_PROTOCOL ulProtocolID,
										unsigned long ulFlags,
										unsigned long *pulChannelID);
extern "C" J2534ERROR WINAPI PassThruDisconnect(
										unsigned long ulChannelID);
extern "C" J2534ERROR WINAPI PassThruReadMsgs(
										unsigned long ulChannelID,
										PASSTHRU_MSG	   *pstrucJ2534Msg,
										unsigned long *pulNumMsgs,
										unsigned long ulTimeout);
extern "C" J2534ERROR WINAPI PassThruWriteMsgs(
										unsigned long ulChannelID,
										PASSTHRU_MSG	   *pstrucJ2534Msg,
										unsigned long *pulNumMsgs,
										unsigned long ulTimeout);
extern "C" J2534ERROR WINAPI PassThruStartPeriodicMsg(
										unsigned long ulChannelID, 
										PASSTHRU_MSG *pMsgJ2534, 
										unsigned long *pulMsgID, 
										unsigned long ulTimeout);
extern "C" J2534ERROR WINAPI PassThruStopPeriodicMsg(
										unsigned long ulChannelID,
										unsigned long ulMsgID);
extern "C" J2534ERROR WINAPI PassThruStartMsgFilter(
										unsigned long	ulChannelID,
										J2534_FILTER	enumFilterType,
										PASSTHRU_MSG		*pstrucJ2534Mask,
										PASSTHRU_MSG		*pstrucJ2534Pattern,
										PASSTHRU_MSG		*pstrucJ2534FlowControl,
										unsigned long *pulFilterID);
extern "C" J2534ERROR WINAPI PassThruStopMsgFilter(
									    unsigned long ulChannelID,
									    unsigned long ulFilterID);
extern "C" J2534ERROR WINAPI PassThruSetProgrammingVoltage(
									    unsigned long ulPin,
									    unsigned long ulVoltage);
extern "C" J2534ERROR WINAPI PassThruReadVersion(
										char *pchFirmwareVersion,
									    char *pchDllVersion,
									    char *pchApiVersion);
extern "C" J2534ERROR WINAPI PassThruGetLastError(
									    //unsigned long ulErrorID,
									    char *pchErrorDescription);
extern "C" J2534ERROR WINAPI PassThruIoctl(
										unsigned long ulChannelID,
										J2534IOCTLID enumIoctlID,
										void *pInput,
										void *pOutput);

#endif