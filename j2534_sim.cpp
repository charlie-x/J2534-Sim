// j2534_sim.cpp : Defines the entry point for the DLL application.
//#undef BUILD_0404


//
// This is old code, and i hacked it along as i needed it for different J2534 / hacking tools, sometimes while working on a dyno on a freezing, hot or raining day.
// but it works with the drewtech tools i usually use
// maybe posting it will make me clean it up or rewrite it :)
//



#include "stdafx.h"

// this is for straight 0404 mode.  set by VC+ Project
//#define BUILD_0404						( 1 )

// this converts 0202 to 0404
// WIP, it works well enough for testing
//#define BUILD_TRANSLATE_0202                ( 1  )

// simple simulation when no hardware
//#define SIMULATION_MODE 1

#define FAIL	( 0 )
#define PASS	( 1 )

// where to write a log file too ( c:\ usually needs admin)
#define LOG_FILE_FILENAME "C:\\j2534_logger.txt"

#include "j2534_sim.h"

#ifndef BUILD_0404

#include "J2534.h"

#else

#include "J2534_v0404.h"

#define J2534_STATUS_NOERROR NO_ERROR
#define ulDataSize DataSize
#define J2534IOCTLID long
#define J2534_PROTOCOL long
#define J2534IOCTLPARAMID long
#define J2534ERROR long

#endif

// helper functions
static char *GetJ2534IOCTLIDText ( J2534IOCTLID enumIoctlID );
static char *GetJ2534_PROTOCOLText ( J2534_PROTOCOL protocol );
static char* GetJ2534IOCTLPARAMIDText ( J2534IOCTLPARAMID value );
static void GetJ2534ErrorText ( J2534ERROR err );


static void LogMsg1 ( char *str );
static void LogMsg2 ( char *strm, char*str );

BOOL APIENTRY DllMain ( HANDLE hModule,
                        DWORD  ul_reason_for_call,
                        LPVOID lpReserved
                      )
{

    switch ( ul_reason_for_call ) {
        case DLL_PROCESS_ATTACH:

        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
    }

    return TRUE;
}

void PrintBuffer ( int length, unsigned char *data )
{
    char buffer[1024];
    char buffer2[1024];
    int i;

    if ( data == NULL ) { return; }

    if ( length == 0 ) { return; }


    memset ( buffer, 0, sizeof ( buffer ) );
    memset ( buffer2, 0, sizeof ( buffer2 ) );

    for ( i = 0; i < length; i++ ) {

        sprintf_s ( buffer2, sizeof ( buffer2 ), "0x%02x ", *data++ );

        strcat_s ( buffer, sizeof ( buffer ), buffer2 );
    }

    LogMsg1 ( buffer );
}

// Declaration of Exported functions.

/*
	long PassThruConnect(unsigned long ProtocolID, unsigned long Flags, unsigned long *pChannelID);

	Description
		The PassThruConnect function is used to establish a logical communication channel between the User Application
		and the vehicle network (via the PassThru device) using the specified network layer protocol and selected
		protocol options.

	Parameters
		ProtocolID
			The protocol identifier selects the network layer protocol that will be used for the communications channel.
		Flags
			Protocol specific options that are defined by bit fields. This parameter is usually set to zero.
		pChannelID
			Pointer to an unsigned long (4 bytes) that receives the handle to the open communications channel. The returned
			handle is used as an argument to other PassThru functions which require a communications channel reference.
		See Also
			PassThruDisconnect

	Example
		unsigned long status;
		unsigned long Flags;
		unsigned long ChannelID;	/* Logical channel identifier returned by PassThruConnect
		char errstr[256];

		*
		** Select Extended CAN Data Frame and Network Address Extension.
		*
		Flags = (CAN_29BIT_ID | ISO15765_ADDR_TYPE);

		status = PassThruConnect(CAN, Flags, &ChannelID);
		if (status != STATUS_NOERROR)
		{
			*
			** PassThruConnect failed! Get descriptive error string.
			*
			PassThruGetLastError(&errstr[0]);

			*
			** Display Error dialog box and/or write error description to Log file.
			*
		}


*/

// 0404
#if defined( BUILD_0404 ) || defined( BUILD_TRANSLATE_0202)
typedef long ( CALLBACK * PTOPEN ) ( void *, unsigned long * );
typedef long ( CALLBACK * PTCLOSE ) ( unsigned long );

//#define J2534ERROR long


typedef long ( CALLBACK * PTCONNECT ) ( unsigned long, unsigned long, unsigned long, unsigned long, unsigned long * );
typedef long ( CALLBACK * PTDISCONNECT ) ( unsigned long );
typedef long ( CALLBACK * PTREADMSGS ) ( unsigned long, void *, unsigned long *, unsigned long );
typedef long ( CALLBACK * PTWRITEMSGS ) ( unsigned long, void *, unsigned long *, unsigned long );
typedef long ( CALLBACK * PTSTARTPERIODICMSG ) ( unsigned long, void *, unsigned long *, unsigned long );
typedef long ( CALLBACK * PTSTOPPERIODICMSG ) ( unsigned long, unsigned long );
typedef long ( CALLBACK * PTSTARTMSGFILTER ) ( unsigned long, unsigned long, void *, void *, void *, unsigned long * );
typedef long ( CALLBACK * PTSTOPMSGFILTER ) ( unsigned long, unsigned long );
typedef long ( CALLBACK * PTSETPROGRAMMINGVOLTAGE ) ( unsigned long, unsigned long, unsigned long );
typedef long ( CALLBACK * PTREADVERSION ) ( unsigned long, char *, char *, char * );
typedef long ( CALLBACK * PTGETLASTERROR ) ( char * );
typedef long ( CALLBACK * PTIOCTL ) ( unsigned long, unsigned long, void *, void * );
// Drew Tech specific function calls
typedef long ( CALLBACK * PTLOADFIRMWARE ) ( void );
typedef long ( CALLBACK * PTRECOVERFIRMWARE ) ( void );
typedef long ( CALLBACK * PTREADIPSETUP ) ( unsigned long DeviceID, char *host_name, char *ip_addr, char *subnet_mask,
        char *gateway, char *dhcp_addr );
typedef long ( CALLBACK * PTWRITEIPSETUP ) ( unsigned long DeviceID, char *host_name, char *ip_addr, char *subnet_mask,
        char *gateway, char *dhcp_addr );
typedef long ( CALLBACK * PTREADPCSETUP ) ( char *host_name, char *ip_addr );
typedef long ( CALLBACK * PTGETPOINTER ) ( long vb_pointer );
typedef long ( CALLBACK * PTGETNEXTCARDAQ ) ( char **name, unsigned long * version, char **addr );


#else

typedef long ( CALLBACK * PTCONNECT ) ( unsigned long, unsigned long, unsigned long * );
typedef long ( CALLBACK * PTDISCONNECT ) ( unsigned long );
typedef long ( CALLBACK * PTREADMSGS ) ( unsigned long, void *, unsigned long *, unsigned long );
typedef long ( CALLBACK * PTWRITEMSGS ) ( unsigned long, void *, unsigned long *, unsigned long );
typedef long ( CALLBACK * PTSTARTPERIODICMSG ) ( unsigned long, void *, unsigned long *, unsigned long );
typedef long ( CALLBACK * PTSTOPPERIODICMSG ) ( unsigned long, unsigned long );
typedef long ( CALLBACK * PTSTARTMSGFILTER ) ( unsigned long, unsigned long, void *, void *, void *, unsigned long * );
typedef long ( CALLBACK * PTSTOPMSGFILTER ) ( unsigned long, unsigned long );
typedef long ( CALLBACK * PTSETPROGRAMMINGVOLTAGE ) ( unsigned long, unsigned long );
typedef long ( CALLBACK * PTREADVERSION ) ( char *, char *, char * );
typedef long ( CALLBACK * PTGETLASTERROR ) ( char * );
typedef long ( CALLBACK * PTIOCTL ) ( unsigned long, unsigned long, void *, void * );

#endif

extern unsigned long gUserNumEcusReprgm;
extern unsigned long gRespTimeOutofRange;
extern unsigned long gRespTimeTooSoon;
extern unsigned long gRespTimeTooLate;
extern unsigned long gDetermineProtocol;

typedef struct globData {
#if defined( BUILD_0404 ) || defined(   BUILD_TRANSLATE_0202)
    PTOPEN pPassThruOpen;
    PTCLOSE pPassThruClose;
#endif

#ifdef BUILD_0404

    PTCONNECT pPassThruConnect;
    PTDISCONNECT pPassThruDisconnect;
    PTREADMSGS pPassThruReadMsgs;
    PTWRITEMSGS pPassThruWriteMsgs;
    PTSTARTPERIODICMSG pPassThruStartPeriodicMsg;
    PTSTOPPERIODICMSG pPassThruStopPeriodicMsg;
    PTSTARTMSGFILTER pPassThruStartMsgFilter;
    PTSTOPMSGFILTER pPassThruStopMsgFilter;
    PTSETPROGRAMMINGVOLTAGE pPassThruSetProgrammingVoltage;
    PTREADVERSION pPassThruReadVersion;
    PTGETLASTERROR pPassThruGetLastError;
    PTIOCTL pPassThruIoctl;
    PTLOADFIRMWARE pPassThruLoadFirmware;
    PTRECOVERFIRMWARE pPassThruRecoverFirmware;
    PTREADIPSETUP pPassThruReadIPSetup;
    PTWRITEIPSETUP pPassThruWriteIPSetup;
    PTREADPCSETUP pPassThruReadPCSetup;
    PTGETPOINTER pPassThruGetPointer;
    PTGETNEXTCARDAQ pPassThruGetNextCarDAQ;

#else

    PTCONNECT pPassThruConnect;
    PTDISCONNECT pPassThruDisconnect;
    PTREADMSGS pPassThruReadMsgs;
    PTWRITEMSGS pPassThruWriteMsgs;
    PTSTARTPERIODICMSG pPassThruStartPeriodicMsg;
    PTSTOPPERIODICMSG pPassThruStopPeriodicMsg;
    PTSTARTMSGFILTER pPassThruStartMsgFilter;
    PTSTOPMSGFILTER pPassThruStopMsgFilter;
    PTSETPROGRAMMINGVOLTAGE pPassThruSetProgrammingVoltage;
    PTREADVERSION pPassThruReadVersion;
    PTGETLASTERROR pPassThruGetLastError;
    PTIOCTL pPassThruIoctl;

#endif

} globData_t;

typedef struct stPassThrough {

    unsigned long ulChannel;
    globData data;

} stPassThrough_t;


static int Load_J2534DLL (  char *szLibrary, globData * data );

// eek, very bad for a dll. fix pls!
static stPassThrough_t *pGlobalPtr = NULL;

#ifdef BUILD_0404

// this loads the original DLL, set the paths in here to the DLL you want to log.
//
int Load_J2534DLL ( void )
{
    stPassThrough *pPtr;
    //load the real dll (change to registry key, or popup dialog

    // SCANDAQ
    //allocate some memory
    pPtr = pGlobalPtr = ( stPassThrough* ) malloc ( sizeof ( stPassThrough ) );

    if ( pPtr == NULL ) {
        return ERR_FAILED;
    }

    int id;

    id = MessageBox ( NULL, "SCANDAQ/CARDAQ/MONGOOSE", "DLL", MB_YESNOCANCEL );

    if ( _access ( "C:\\scandaq", 0 ) == 0 ) {

        printf ( "found c:\\scandaq\n" );
        id = IDYES;

    } else
        if ( _access ( "C:\\cardaq", 0 ) == 0 ) {

            printf ( "found c:\\cardaq\n" );
            id = IDNO;

        } else
            if ( _access ( "C:\\mongoose", 0 ) == 0 ) {

                printf ( "found c:\\mongoose\n" );
                id = IDCANCEL;

            } else {
                printf ( "asking\n" );
                id = MessageBox ( NULL, "SCANDAQ/CARDAQ/MONGOOSE", "DLL", MB_YESNOCANCEL );

            }

    switch ( id ) {


        case IDYES:

            printf ( "\nYou selected ScanDAQ\n" );
            Load_J2534DLL ( "C:\\WINDOWS\\system32\\CDPLS232.DLL", &pPtr->data );
            break;

        case IDNO:
            printf ( "\nYou selected CARDAQ\n" );
#ifdef BUILD_0404
            Load_J2534DLL ( "C:\\Windows\\SYSTEM32\\CARDA432.DLL", &pPtr->data );
#else
            Load_J2534DLL ( "C:\\windows\\system32\\CARDAQ32.DLL", &pPtr->data );
#endif
            break;

        case IDCANCEL:
            printf ( "\nYou selected MONGOOSE\n" );
            Load_J2534DLL ( "C:\\WINDOWS\\system32\\mongI432.dll", &pPtr->data );
            break;
    }


    return 1;

}

#define JTYPE extern  long  J2534_SIM_API

JTYPE PassThruConnect ( unsigned long DeviceID, unsigned long ProtocolID, unsigned long Flags, unsigned long Baudrate, unsigned long * pChannelID )
{

    J2534ERROR ret = J2534_STATUS_NOERROR;
    stPassThrough *pPtr = pGlobalPtr ;

    if ( pPtr == NULL ) {
        Load_J2534DLL();
        pPtr = pGlobalPtr ;
    }

    char buffer[1024];
    sprintf_s ( buffer, sizeof ( buffer ), "PassThruConnect 0404: DeviceID = %d, ProtocolID = %d, Flags =%d, BaudRate = %d\n", DeviceID, ProtocolID, Flags, Baudrate );
    LogMsg1 ( buffer );

    LogMsg2 ( "protocol = %s\n", GetJ2534_PROTOCOLText ( ( J2534_PROTOCOL ) ProtocolID ) );

    if ( pPtr && pPtr->data.pPassThruConnect )	{

        ret = ( J2534ERROR ) pPtr->data.pPassThruConnect ( DeviceID, ProtocolID, Flags, Baudrate, &pPtr->ulChannel );
    }

    else {

        pPtr->ulChannel = 0;
    }

    if ( pChannelID ) {
        *pChannelID = ( unsigned long ) pGlobalPtr;
    }

    return ret;
}

JTYPE PassThruDisconnect ( unsigned long ChannelID )
{
    J2534ERROR ret = J2534_STATUS_NOERROR;
    stPassThrough *pPtr = ( stPassThrough* ) ChannelID ;

    if ( pPtr )
    { ChannelID = pPtr->ulChannel; }

    char buffer[1024];
    sprintf_s ( buffer, sizeof ( buffer ), "PassThruDisconnect : ChannelID = %d\n", ChannelID );
    LogMsg1 ( buffer );

    pPtr = pGlobalPtr;


    if ( pPtr && pPtr->data.pPassThruDisconnect ) {
        ret = ( J2534ERROR ) pPtr->data.pPassThruDisconnect ( ChannelID );
    }

    return ret;

}

JTYPE PassThruReadMsgs ( unsigned long ChannelID, PASSTHRU_MSG * pMsg, unsigned long * pNumMsgs, unsigned long Timeout )
{

    J2534ERROR ret = J2534_STATUS_NOERROR;

    stPassThrough *pPtr = ( stPassThrough* ) ChannelID ;

    if ( pPtr ) {
        ChannelID = pPtr->ulChannel;
    }

    char buffer[1024];
    sprintf_s ( buffer, sizeof ( buffer ), "PassThruReadMsgs : ChannelID = %d\n", ChannelID );
    LogMsg1 ( buffer );

    pPtr = pGlobalPtr;

    if ( pPtr && pPtr->data.pPassThruReadMsgs ) {

        //	Sleep(20);
        ret = ( J2534ERROR ) pPtr->data.pPassThruReadMsgs ( ChannelID, pMsg, pNumMsgs, Timeout );
    }

    if ( ret == J2534_STATUS_NOERROR && pMsg->ulDataSize ) {

        sprintf_s ( buffer, sizeof ( buffer ), "pMSg(%d,0x%lx,%d,%d)\n", ChannelID, pMsg, *pNumMsgs, Timeout );
        LogMsg1 ( buffer );
        LogMsg2 ( "\tprotocol    = %s\n", GetJ2534_PROTOCOLText ( ( J2534_PROTOCOL ) pMsg->ProtocolID ) );

        sprintf_s ( buffer, sizeof ( buffer ), "\trx status   = %lu\n", pMsg->RxStatus );
        LogMsg1 ( buffer );
        sprintf_s ( buffer, sizeof ( buffer ), "\ttx flags    = %lu\n", pMsg->TxFlags );
        LogMsg1 ( buffer );
        sprintf_s ( buffer, sizeof ( buffer ), "\ttime stamp  = %lu\n", pMsg->Timestamp );
        LogMsg1 ( buffer );
        sprintf_s ( buffer, sizeof ( buffer ), "\textra data  = %lu\n", pMsg->ExtraDataIndex );
        LogMsg1 ( buffer );

        sprintf_s ( buffer, sizeof ( buffer ), "\tdata size   = %lu\n", pMsg->DataSize );
        LogMsg1 ( buffer );

        sprintf_s ( buffer, sizeof ( buffer ), "\tdata        = 0x%lx = { ", pMsg->Data );
        LogMsg1 ( buffer );
        PrintBuffer ( pMsg->DataSize, &pMsg->Data[0] );
        LogMsg1 ( "}\n" );

        sprintf_s ( buffer, sizeof ( buffer ), "\textradata        = 0x%lx = { ", pMsg->Data );
        LogMsg1 ( buffer );
        PrintBuffer ( pMsg->DataSize, &pMsg->Data[pMsg->ExtraDataIndex] );
        LogMsg1 ( "}\n" );

    } else
        if ( ret != ERR_BUFFER_EMPTY ) {
            GetJ2534ErrorText ( ret );

        } else {
//		LogMsg1("");


        }

    return ret;

}

JTYPE PassThruWriteMsgs ( unsigned long ChannelID, PASSTHRU_MSG * pMsg, unsigned long * pNumMsgs, unsigned long Timeout )
{
    J2534ERROR ret = J2534_STATUS_NOERROR;

    stPassThrough *pPtr = ( stPassThrough* ) ChannelID ;

    if ( pPtr ) {
        ChannelID = pPtr->ulChannel;
    }

    char buffer[1024];
    sprintf_s ( buffer, sizeof ( buffer ), "PassThruWriteMsgs : ChannelID = %d\n", ChannelID );
    LogMsg1 ( buffer );

    pPtr = pGlobalPtr;


    sprintf_s ( buffer, sizeof ( buffer ), "PassThruWriteMsgs(%d,0x%lx,%d,%d)\n", ChannelID, pMsg, *pNumMsgs, Timeout );
    LogMsg1 ( buffer );

    LogMsg2 ( "\tprotocol    = %s\n", GetJ2534_PROTOCOLText ( ( J2534_PROTOCOL ) pMsg->ProtocolID ) );


// not used for write

//	_RPT1(_CRT_WARN,"\tex data ind = %lu\n",pMsg->ExtraDataIndex);
//	_RPT1(_CRT_WARN,"\ttimestamp   = %lu\n",pMsg->TimeStamp);


    sprintf_s ( buffer, sizeof ( buffer ), "\trx status   = %lu\n", pMsg->RxStatus );
    LogMsg1 ( buffer );
    sprintf_s ( buffer, sizeof ( buffer ), "\ttx flags    = %lu\n", pMsg->TxFlags );
    LogMsg1 ( buffer );
    sprintf_s ( buffer, sizeof ( buffer ), "\ttime stamp  = %lu\n", pMsg->Timestamp );
    LogMsg1 ( buffer );
    sprintf_s ( buffer, sizeof ( buffer ), "\textra data  = %lu\n", pMsg->ExtraDataIndex );
    LogMsg1 ( buffer );

    sprintf_s ( buffer, sizeof ( buffer ), "\tdata size   = %lu\n", pMsg->DataSize );
    LogMsg1 ( buffer );
    sprintf_s ( buffer, sizeof ( buffer ), "\tdata        = 0x%lx = { ", pMsg->Data );
    LogMsg1 ( buffer );

    if ( pMsg->Data != NULL ) {


        PrintBuffer ( pMsg->DataSize, &pMsg->Data[0] );

        PrintBuffer ( pMsg->DataSize, &pMsg->Data[pMsg->ExtraDataIndex] );

    }

    LogMsg1 ( "}\n" );

    if ( pPtr && pPtr->data.pPassThruWriteMsgs ) {

//		Sleep( 10 );

        ret = ( J2534ERROR ) pPtr->data.pPassThruWriteMsgs ( ChannelID, pMsg, pNumMsgs, Timeout );
    }

    return ret;
}

JTYPE PassThruStartPeriodicMsg ( unsigned long ChannelID, PASSTHRU_MSG * pMsg,
                                 unsigned long * pMsgID, unsigned long TimeInterval )
{
    J2534ERROR ret = J2534_STATUS_NOERROR;

    stPassThrough *pPtr = ( stPassThrough* ) ChannelID ;

    if ( pPtr ) {
        ChannelID = pPtr->ulChannel;
    }

    char buffer[1024];
    sprintf_s ( buffer, sizeof ( buffer ), "PassThruStartPeriodicMsg : ChannelID = %d\n", ChannelID );
    LogMsg1 ( buffer );

    pPtr = pGlobalPtr;

    LogMsg1 ( "PassThruStartPeriodicMsg" );

    if ( pPtr && pPtr->data.pPassThruStartPeriodicMsg ) {
        ret = ( J2534ERROR ) pPtr->data.pPassThruStartPeriodicMsg ( ChannelID, pMsg, pMsgID, TimeInterval );
    }

    GetJ2534ErrorText ( ret );

    return ret;
}

JTYPE PassThruStopPeriodicMsg ( unsigned long ChannelID, unsigned long MsgID )
{
    J2534ERROR ret = J2534_STATUS_NOERROR;

    stPassThrough *pPtr = ( stPassThrough* ) ChannelID ;
    ChannelID = pPtr->ulChannel;

    char buffer[1024];
    sprintf_s ( buffer, sizeof ( buffer ), "PassThruStopPeriodicMsg : ChannelID = %d\n", ChannelID );
    LogMsg1 ( buffer );

    pPtr = pGlobalPtr;

    LogMsg1 ( "PassThruStartPeriodicMsg" );

    if ( pPtr && pPtr->data.pPassThruStopPeriodicMsg ) {
        ret = ( J2534ERROR ) pPtr->data.pPassThruStopPeriodicMsg ( ChannelID, MsgID );
    }

    GetJ2534ErrorText ( ret );

    return ret;
}

JTYPE PassThruStartMsgFilter ( unsigned long ChannelID,
                               unsigned long FilterType, PASSTHRU_MSG * pMsg, PASSTHRU_MSG * pPatternMsg,
                               PASSTHRU_MSG * pFlowControlMsg, unsigned long * pMsgID )
{
    J2534ERROR ret = J2534_STATUS_NOERROR;
    stPassThrough *pPtr = ( stPassThrough* ) ChannelID;

    if ( pPtr ) {
        ChannelID = pPtr->ulChannel;
    }

    char buffer[1024];
    sprintf_s ( buffer, sizeof ( buffer ), "PassThruStartMsgFilter : ChannelID = %d\n", ChannelID );
    LogMsg1 ( buffer );

    sprintf_s ( buffer, sizeof ( buffer ), "FilterType = %d\n", FilterType );
    LogMsg1 ( buffer );

    if ( pMsg ) {
        LogMsg1 ( "MaskMsg\n" );
        sprintf_s ( buffer, sizeof ( buffer ), "ProtocolID = %d\n", pMsg->ProtocolID );
        LogMsg1 ( buffer );

        LogMsg2 ( "\tprotocol    = %s\n", GetJ2534_PROTOCOLText ( ( J2534_PROTOCOL ) pMsg->ProtocolID ) );

        sprintf_s ( buffer, sizeof ( buffer ), "\trx status   = %lu\n", pMsg->RxStatus );
        LogMsg1 ( buffer );
        sprintf_s ( buffer, sizeof ( buffer ), "\ttx flags    = %lu\n", pMsg->TxFlags );
        LogMsg1 ( buffer );
        sprintf_s ( buffer, sizeof ( buffer ), "\ttime stamp  = %lu\n", pMsg->Timestamp );
        LogMsg1 ( buffer );
        sprintf_s ( buffer, sizeof ( buffer ), "\textra data  = %lu\n", pMsg->ExtraDataIndex );
        LogMsg1 ( buffer );

        sprintf_s ( buffer, sizeof ( buffer ), "\tdata size   = %lu\n", pMsg->DataSize );
        LogMsg1 ( buffer );

        sprintf_s ( buffer, sizeof ( buffer ), "\tdata        = 0x%lx = { ", pMsg->Data );
        LogMsg1 ( buffer );
        PrintBuffer ( pMsg->DataSize, &pMsg->Data[0] );
        LogMsg1 ( "}\n" );


        sprintf_s ( buffer, sizeof ( buffer ), "\textradata   = 0x%lx = { ", pMsg->Data );
        LogMsg1 ( buffer );
        PrintBuffer ( pMsg->ExtraDataIndex, &pMsg->Data[pMsg->ExtraDataIndex] );
        LogMsg1 ( "}\n" );
    }

    if ( pPatternMsg ) {
        LogMsg1 ( "pPatternMsg\n" );
        LogMsg2 ( "\tprotocol    = %s\n", GetJ2534_PROTOCOLText ( ( J2534_PROTOCOL ) pPatternMsg->ProtocolID ) );

        sprintf_s ( buffer, sizeof ( buffer ), "\trx status   = %lu\n", pPatternMsg->RxStatus );
        LogMsg1 ( buffer );
        sprintf_s ( buffer, sizeof ( buffer ), "\ttx flags    = %lu\n", pPatternMsg->TxFlags );
        LogMsg1 ( buffer );
        sprintf_s ( buffer, sizeof ( buffer ), "\ttime stamp  = %lu\n", pPatternMsg->Timestamp );
        LogMsg1 ( buffer );
        sprintf_s ( buffer, sizeof ( buffer ), "\textra data  = %lu\n", pPatternMsg->ExtraDataIndex );
        LogMsg1 ( buffer );

        sprintf_s ( buffer, sizeof ( buffer ), "\tdata size   = %lu\n", pPatternMsg->DataSize );
        LogMsg1 ( buffer );

        sprintf_s ( buffer, sizeof ( buffer ), "\tdata        = 0x%lx = { ", pPatternMsg->Data );
        LogMsg1 ( buffer );
        PrintBuffer ( pPatternMsg->DataSize, &pPatternMsg->Data[0] );
        LogMsg1 ( "}\n" );


        sprintf_s ( buffer, sizeof ( buffer ), "\textradata   = 0x%lx = { ", pPatternMsg->Data );
        LogMsg1 ( buffer );
        PrintBuffer ( pPatternMsg->ExtraDataIndex, &pPatternMsg->Data[pPatternMsg->ExtraDataIndex] );
        LogMsg1 ( "}\n" );
    }

    if ( pFlowControlMsg ) {
        LogMsg1 ( "pFlowControlMsg\n" );
        LogMsg2 ( "\tprotocol    = %s\n", GetJ2534_PROTOCOLText ( ( J2534_PROTOCOL ) pFlowControlMsg->ProtocolID ) );

        sprintf_s ( buffer, sizeof ( buffer ), "\trx status   = %lu\n", pFlowControlMsg->RxStatus );
        LogMsg1 ( buffer );
        sprintf_s ( buffer, sizeof ( buffer ), "\ttx flags    = %lu\n", pFlowControlMsg->TxFlags );
        LogMsg1 ( buffer );
        sprintf_s ( buffer, sizeof ( buffer ), "\ttime stamp  = %lu\n", pFlowControlMsg->Timestamp );
        LogMsg1 ( buffer );
        sprintf_s ( buffer, sizeof ( buffer ), "\textra data  = %lu\n", pFlowControlMsg->ExtraDataIndex );
        LogMsg1 ( buffer );

        sprintf_s ( buffer, sizeof ( buffer ), "\tdata size   = %lu\n", pFlowControlMsg->DataSize );
        LogMsg1 ( buffer );

        sprintf_s ( buffer, sizeof ( buffer ), "\tdata        = 0x%lx = { ", pFlowControlMsg->Data );
        LogMsg1 ( buffer );
        PrintBuffer ( pFlowControlMsg->DataSize, &pFlowControlMsg->Data[0] );
        LogMsg1 ( "}\n" );


        sprintf_s ( buffer, sizeof ( buffer ), "\textradata   = 0x%lx = { ", pFlowControlMsg->Data );
        LogMsg1 ( buffer );
        PrintBuffer ( pFlowControlMsg->ExtraDataIndex, &pFlowControlMsg->Data[pFlowControlMsg->ExtraDataIndex] );
        LogMsg1 ( "}\n" );
    }

#if !defined ( SIMULATION_MODE )

    if ( pPtr && pPtr->data.pPassThruStartMsgFilter ) {
        ret = ( J2534ERROR ) pPtr->data.pPassThruStartMsgFilter ( pPtr->ulChannel, FilterType,
                pMsg, pPatternMsg, pFlowControlMsg, pMsgID );
    }

#else
    ret  = 0;
#endif

    GetJ2534ErrorText ( ret );

    return ret;
}

JTYPE PassThruStopMsgFilter ( unsigned long ChannelID, unsigned long ulFilterID )
{
    J2534ERROR ret = J2534_STATUS_NOERROR;

    char buffer[1024];
    sprintf_s ( buffer, sizeof ( buffer ), "PassThruStopMsgFilter : ChannelID = %d, MsgID = %d\n", ChannelID, ulFilterID );
    LogMsg1 ( buffer );

#if !defined ( SIMULATION_MODE )

    stPassThrough *pPtr = ( stPassThrough* ) ChannelID;

    if ( pPtr ) {
        ChannelID = pPtr->ulChannel;
    }


    if ( pPtr ) {
        ret = ( J2534ERROR ) pPtr->data.pPassThruStopMsgFilter ( pPtr->ulChannel, ulFilterID );
    }

    GetJ2534ErrorText ( ret );

#endif



    return ret;
}

JTYPE PassThruSetProgrammingVoltage ( unsigned long DeviceID, unsigned long Pin, unsigned long Voltage )
{
    J2534ERROR ret = J2534_STATUS_NOERROR;
    char buffer[1024];
    sprintf_s ( buffer, sizeof ( buffer ), "PassThruSetProgrammingVoltage : DeviceID = %d, Pin = %d, Voltage = %d\n", DeviceID, Pin, Voltage );
    LogMsg1 ( buffer );


#if !defined ( SIMULATION_MODE )

    stPassThrough *pPtr = pGlobalPtr;

    if ( pPtr ) {
        ret = ( J2534ERROR ) pPtr->data.pPassThruSetProgrammingVoltage ( DeviceID, Pin, Voltage );
    }

    GetJ2534ErrorText ( ret );

#endif




    return ret;
}

JTYPE PassThruReadVersion ( unsigned long DeviceID, char *pchFirmwareVersion, char *pchDllVersion, char *pchApiVersion )
{
    J2534ERROR ret = J2534_STATUS_NOERROR;
    stPassThrough *pPtr = pGlobalPtr ;

    char buffer[1024];
    sprintf_s ( buffer, sizeof ( buffer ), "PassThruReadVersion : DeviceID = %d\n", DeviceID );
    LogMsg1 ( buffer );

#if !defined ( SIMULATION_MODE )

    if ( pPtr && pPtr->data.pPassThruReadVersion ) {
        ret = ( J2534ERROR ) pPtr->data.pPassThruReadVersion ( DeviceID, pchFirmwareVersion, pchDllVersion, pchApiVersion );

        LogMsg1 ( pchFirmwareVersion );
        LogMsg1 ( pchDllVersion );
        LogMsg1 ( pchApiVersion );

    }

    else {

        if ( pchFirmwareVersion ) { strcpy_s ( pchFirmwareVersion, 4, "1.00" ); }

        if ( pchDllVersion ) { strcpy_s ( pchDllVersion, 4, "1.00" ); }

        if ( pchApiVersion ) { strcpy_s ( pchApiVersion, 4, "1.00" ); }
    }

#else

    strcpy ( pchFirmwareVersion, "1.00" );
    strcpy ( pchDllVersion, "1.00" );
    strcpy ( pchApiVersion, "1.00" );

#endif


    return ret;;

}


JTYPE PassThruGetLastError ( char *pErrorDescription )
{
    J2534ERROR ret = J2534_STATUS_NOERROR;

    return ret;
}

JTYPE PassThruIoctl ( unsigned long ChannelID, unsigned long IoctlID,
                      void *pInput, void *pOutput )
{
    int ret;

    stPassThrough *pPtr = ( stPassThrough* ) ChannelID ;

    if ( pPtr ) {
        ChannelID = pPtr->ulChannel;
    }


    {
        char buffer[1024];

        sprintf_s ( buffer, sizeof ( buffer ), "PassThruIoctl(%d,%s,0x%lx,0x%lx)\n", ChannelID, GetJ2534IOCTLIDText ( IoctlID ), pInput, pOutput );
        LogMsg1 ( buffer );
    }



#if !defined ( SIMULATION_MODE )

    if ( pPtr && pPtr->data.pPassThruIoctl ) {
        ret = ( J2534ERROR ) pPtr->data.pPassThruIoctl ( ChannelID, IoctlID, pInput, pOutput );

        char buffer[1024];
        sprintf_s ( buffer, sizeof ( buffer ), "%d = pPassThruIoctl()\n", ret );
        LogMsg1 ( buffer );

        GetJ2534ErrorText ( ret );
    }

#endif


    return ret;
}



JTYPE  PassThruOpen ( void* pName, unsigned long * pDeviceID )
{

    J2534ERROR ret = J2534_STATUS_NOERROR;

    stPassThrough *pPtr = STATUS_NOERROR ;
    {
        char buffer[1024];

        sprintf_s ( buffer, sizeof ( buffer ), "PassThruOpen(%s,0x%lx)\n", pName, pDeviceID );
        LogMsg1 ( buffer );
    }


    if ( pPtr == NULL )
    { Load_J2534DLL(); }


    if ( pPtr, pGlobalPtr->data.pPassThruOpen ) {
        ret = ( J2534ERROR ) pPtr->data.pPassThruOpen ( pName, pDeviceID );

        GetJ2534ErrorText ( ret );
    }

    return ret;
}


extern "C"  long J2534_SIM_API  PassThruClose ( unsigned long DeviceID )
{
    J2534ERROR ret = J2534_STATUS_NOERROR;

    LogMsg1 ( "PassThruClose" );

    if ( pGlobalPtr && pGlobalPtr->data.pPassThruClose ) {
        ret = ( J2534ERROR ) pGlobalPtr->data.pPassThruClose ( DeviceID );

        GetJ2534ErrorText ( ret );
    }

    return ret;

}

#else



extern "C"  J2534ERROR J2534_SIM_API  PassThruConnect (
    J2534_PROTOCOL ulProtocolID,
    unsigned long ulFlags,
    unsigned long * pulChannelID )

{
    J2534ERROR ret = J2534_STATUS_NOERROR;

    stPassThrough *pPtr = NULL ;
    {
        char buffer[1024];

        sprintf_s ( buffer, sizeof ( buffer ), "PassThruConnect(%s,0x%x,0x%lx)\n", GetJ2534_PROTOCOLText ( ulProtocolID ), ulFlags, ( unsigned long ) pulChannelID );
        LogMsg1 ( buffer );
    }

    // 0404 mode will have setup already
    if ( pGlobalPtr == NULL )  {


        //allocate some memory
        pPtr = pGlobalPtr = ( stPassThrough* ) malloc ( sizeof ( stPassThrough ) );

        if ( pPtr == NULL ) {
            return J2534_ERR_FAILED;
        }

        //load the real dll (change to registry key, or popup dialog

        // SCANDAQ

        int id;


        if ( _access ( "C:\\scandaq", 0 )  == 0 ) {

            printf ( "found c:\\scandaq\n" );
            id = IDYES ;

        } else
            if ( _access ( "C:\\cardaq", 0 ) == 0  ) {

                printf ( "found c:\\cardaq\n" );
                id = IDNO ;

            } else
                if ( _access ( "C:\\mongoose", 0 ) == 0  ) {

                    printf ( "found c:\\mongoose\n" );
                    id = IDCANCEL ;

                } else {
                    printf ( "asking\n" );
#ifdef BUILD_TRANSLATE_0202
                    id = MessageBox ( NULL, "SCANDAQ/CARDAQ/MONGOOSE02to04", "DLL", MB_YESNOCANCEL );
#else
                    id = MessageBox ( NULL, "SCANDAQ/CARDAQ/MONGOOSE", "DLL", MB_YESNOCANCEL );
#endif
                }

        switch ( id ) {


            case IDYES:

                printf ( "\nYou selected ScanDAQ\n" );
                Load_J2534DLL ( "C:\\WINDOWS\\system32\\CDPLS232.DLL", &pPtr->data );
                break;

            case IDNO:
                printf ( "\nYou selected CARDAQ\n" );
                Load_J2534DLL ( "C:\\windows\\system32\\CARDAQ32.DLL", &pPtr->data );
                break;

            case IDCANCEL:
#ifdef BUILD_TRANSLATE_0202
                printf ( "\nYou selected MONGOOSE 0404\n" );
                Load_J2534DLL ( "C:\\WINDOWS\\system32\\mongI432.dll", &pPtr->data );
#else
                printf ( "\nYou selected MONGOOSE\n" );
                Load_J2534DLL ( "C:\\WINDOWS\\system32\\mongI232.dll", &pPtr->data );
#endif
                break;
        }

    } else {

        pPtr = pGlobalPtr;
    }

    // call the real function, fill in the phony data handler, and store the return code


#ifdef BUILD_TRANSLATE_0202

    ULONG DeviceID = 0;
    ULONG baud = 1000 * 1000;

    if ( pPtr->data.pPassThruOpen ) {
        LogMsg1 ( "Calling PassThruOpen\n" );

        ret = ( J2534ERROR ) pPtr->data.pPassThruOpen ( NULL, &DeviceID );

        if ( ret != J2534_STATUS_NOERROR ) {
            LogMsg1 ( "Error Calling PassThruOpen\n" );

        } else {
            LogMsg1 ( "PassThruOpen OK\n" );
        }

    } else {
        LogMsg1 ( "PassThruOpen not opened!\n" );
    }

    printf ( "DeviceID = %d\n", DeviceID );

    if ( ulProtocolID == ISO9141 ) {
        baud = 28800;
    }

    ret = ( J2534ERROR ) pPtr->data.pPassThruConnect ( DeviceID, ulProtocolID, ulFlags,  baud, &pPtr->ulChannel );

    printf ( "%d\n", ret );

#endif

#if !defined ( SIMULATION_MODE )

#ifndef BUILD_TRANSLATE_0202
    ret = ( J2534ERROR ) pPtr->data.pPassThruConnect ( ulProtocolID, ulFlags, &pPtr->ulChannel );
#endif

#else


    // set no error
    ret = J2534_STATUS_NOERROR ;

    // set channel
    pPtr->ulChannel = 1;

#endif
    // set the phony handle which is the ptr to our shared memory
    *pulChannelID = ( unsigned long ) pPtr;

    GetJ2534ErrorText ( ret );

    return ret;
}


extern "C"  J2534ERROR J2534_SIM_API PassThruDisconnect ( unsigned long ulChannelID )
{

    J2534ERROR ret = J2534_STATUS_NOERROR;

    stPassThrough *pPtr = ( stPassThrough* ) ulChannelID ;

    LogMsg1 ( "PassThruDisconnect\n" );

#if !defined ( SIMULATION_MODE )

    if ( pPtr ) {
        ret = ( J2534ERROR ) pPtr->data.pPassThruDisconnect ( pPtr->ulChannel ) ;
    }

#else

    ret = J2534_STATUS_NOERROR ;

#endif
    GetJ2534ErrorText ( ret );

    free ( pPtr );

    // tsk
    pGlobalPtr = NULL;


    return ret;

}

/*
	long PassThruReadMsgs(unsigned long ChannelID, PASSTHRU_MSG *pMsg, unsigned long *pNumMsgs, unsigned long Timeout);

	Description
		The PassThruReadMsgs function is used to receive network protocol messages, receive indications, and transmit indications
		from an existing logical communication channel. The network protocol messages will flow from the PassThru device to the User Application.

	Parameters
		ChannelID
			The logical communication channel identifier assigned by the J2534 API/DLL when the communication channel was opened via the
			PassThruConnect function.
		pMsg
			Pointer to the message structure where the J2534 API/DLL will write the receive message(s). For reading more than one
			message, this must be a pointer to an array of PASSTHRU_MSG structures.
		pNumMsgs
			Pointer to the variable that contains the number of PASSTHRU_MSG structures that are allocated for receive frames.
			The API regards this value as the maximum number of receive frames that can be returned to the UserApplication. On
			function completion this variable will contain the actual number of receive frames contained in the PASSTHRU_MSG
			structure. The number of receive messages returned may be less than the number requested by the UserApplication.
		Timeout
			Timeout interval (in milliseconds) to wait for read completion. A value of zero instructs the API/DLL to read
			buffered receive messages and return immediately. A nonzero timeout value instructs the API/DLL to return after
			the timeout interval has expired. The API/DLL will not wait the entire timeout interval if an error occurs or
			the specified number of messages have been read.

		See Also
			PassThruWrite
			PassThruStartMsgFilter

		Example
			typedef struct {
				unsigned long ProtocolID;	/* vehicle network protocol * /
				unsigned long RxStatus;	/* receive message status * /
				unsigned long TxFlags;	/* transmit message flags * /
				unsigned long Timestamp;	/* receive message timestamp(in microseconds) * /
				unsigned long DataSize;	/* byte size of message payload in the Data array * /
				unsigned long ExtraDataIndex;	/* start of extra data(i.e. CRC, checksum, etc) in Data array * /
				unsigned char Data[4128];	/* message payload or data * /
			} PASSTHRU_MSG;

			unsigned long status;
			unsigned long ChannelID;	/* Logical channel identifier returned by PassThruConnect * /
			unsigned long NumMsgs;
			PASSTHRU_MSG Msg[2];
			unsigned long Timeout;
			char errstr[256];

			/*
			** Establish a ISO15765 communication channel to the vehicle network.
			* /
			status = PassThruConnect(ISO15765, 0x00000000, &ChannelID);

			/*
			** Initialize the PASSTHRU_MSG structure to all zeroes.
			** Set the ProtocolID to select protocol frames of interest.
			** The API/DLL will fill in RxStatus, TxStaus, Timestamp, DataSize, ExtraDataIndex and Data
			** after the function call completes.
			* /
			memset(&Msg, 0, sizeof(Msg));

			Msg[0].ProtocolID = ISO15765;
			Msg[1].ProtocolID = ISO15765;

			/*
			** Indicate that PASSTHRU_MSG array contains two messages.
			* /
			NumMsgs = 2;

			/*
			** API/DLL should read first two messages in receive queue and immediately return. If there aren’t any
			** messages in the receive queue then API/DLL will return ERR_BUFFER_EMPTY.
			* /
			Timeout = 0;

			status = PassThruReadMsgs(ChannelID, &Msg, &NumMsgs, Timeout);
			if (status != STATUS_NOERROR)
			{
				/*
				** PassThruReadMsgs failed! Get descriptive error string.
				* /
				PassThruGetLastError(&errstr[0]);

				/*
				** Display Error dialog box and/or write error description to Log file.
				* /
			}
*/

static int ReadyToSend = 0;

extern "C"  J2534ERROR J2534_SIM_API PassThruReadMsgs (
    unsigned long ulChannelID,
    PASSTHRU_MSG	   * pstrucJ2534Msg,
    unsigned long * pulNumMsgs,
    unsigned long ulTimeout )
{
    char buffer[1024];
    J2534ERROR ret = J2534_STATUS_NOERROR;

    stPassThrough *pPtr = ( stPassThrough* ) ulChannelID ;

    LogMsg1 ( "PassThruReadMsgs" );


#if !defined ( SIMULATION_MODE )

    if ( pPtr ) {
        //	Sleep(20);
        ret = ( J2534ERROR ) pPtr->data.pPassThruReadMsgs ( pPtr->ulChannel, pstrucJ2534Msg, pulNumMsgs, ulTimeout );
        GetJ2534ErrorText ( ret );
    }

#else
    ret = J2534_STATUS_NOERROR ;


    static int state = 0;

    pstrucJ2534Msg->ulTimeStamp = 0;//202840372+clock();

    if ( ReadyToSend == 1000 ) {

        ReadyToSend = 0 ;

        switch ( state ) {
            case 0 :

                // fill in some data
                pstrucJ2534Msg->ulProtocolID = ISO9141;
                pstrucJ2534Msg->ulDataSize  = 1;
                pstrucJ2534Msg->ulExtraDataIndex  = 1;
                pstrucJ2534Msg->ulRxStatus = 0;

                pstrucJ2534Msg->ucData[0] = 0x0;
                state++;
                break;

            case 1 :
                // fill in some data
                pstrucJ2534Msg->ulProtocolID = ISO9141;
                pstrucJ2534Msg->ulDataSize  = 2;
                pstrucJ2534Msg->ulExtraDataIndex  = 2;
                pstrucJ2534Msg->ulRxStatus = 0;

                pstrucJ2534Msg->ucData[0] = 0x0;
                pstrucJ2534Msg->ucData[1] = 0x0;
                state++;
                break;

            case 2 :
                // fill in some data
                pstrucJ2534Msg->ulProtocolID = ISO9141;
                pstrucJ2534Msg->ulDataSize  = 3;
                pstrucJ2534Msg->ulExtraDataIndex  = 3;
                pstrucJ2534Msg->ulRxStatus = 0;

                pstrucJ2534Msg->ucData[0] = 0x0;
                pstrucJ2534Msg->ucData[1] = 0x0;
                pstrucJ2534Msg->ucData[2] = 0x0;
                state++;
                break;

            case 3:
                // fill in some data
                pstrucJ2534Msg->ulProtocolID = ISO9141;
                pstrucJ2534Msg->ulDataSize  = 1;
                pstrucJ2534Msg->ulRxStatus = 0;

                pstrucJ2534Msg->ucData[0] = 0x7E;
                state++;
                break;

            case 4:
                pstrucJ2534Msg->ulProtocolID = ISO9141;
                pstrucJ2534Msg->ulDataSize  = 1;
                pstrucJ2534Msg->ulExtraDataIndex  = 1;
                pstrucJ2534Msg->ulRxStatus = 0;
                pstrucJ2534Msg->ucData[0] = 0x7D;
                pstrucJ2534Msg->ucData[1] = 0x94;
                state++;
                break;

            case 5:
                pstrucJ2534Msg->ulProtocolID = ISO9141;
                pstrucJ2534Msg->ulDataSize  = 1;
                pstrucJ2534Msg->ulExtraDataIndex  = 1;
                pstrucJ2534Msg->ulRxStatus = 0;
                pstrucJ2534Msg->ucData[0] = 0xff;
                pstrucJ2534Msg->ucData[1] = 0xe6;
                state++;
                break;

            case 6:
                pstrucJ2534Msg->ulProtocolID = ISO9141;
                pstrucJ2534Msg->ulDataSize  = 1;
                pstrucJ2534Msg->ulRxStatus = 0;
                pstrucJ2534Msg->ucData[0] = 0x8b;
                state++;
                break;

            default:
                state = 0 ;
                ret = J2534_ERR_BUFFER_EMPTY;
                break;
        }

    } else {
        ret = J2534_ERR_BUFFER_EMPTY ;
        ReadyToSend++;
    }

#endif

    if ( ret == J2534_STATUS_NOERROR && pstrucJ2534Msg->ulDataSize ) {

        sprintf_s ( buffer, sizeof ( buffer ), "PassThruReadMsgs(%d,0x%lx,%d,%d)\n", ulChannelID, ( unsigned long ) pstrucJ2534Msg, *pulNumMsgs, ulTimeout );
        LogMsg1 ( buffer );
        LogMsg2 ( "\tprotocol    = %s\n", GetJ2534_PROTOCOLText ( ( J2534_PROTOCOL ) pstrucJ2534Msg->ulProtocolID ) );

        sprintf_s ( buffer, sizeof ( buffer ), "\trx status   = %lu\n", pstrucJ2534Msg->ulRxStatus );
        LogMsg1 ( buffer );
        sprintf_s ( buffer, sizeof ( buffer ), "\ttx flags    = %lu\n", pstrucJ2534Msg->ulTxFlags );
        LogMsg1 ( buffer );
        sprintf_s ( buffer, sizeof ( buffer ), "\ttime stamp  = %lu\n", pstrucJ2534Msg->ulTimeStamp );
        LogMsg1 ( buffer );
        sprintf_s ( buffer, sizeof ( buffer ), "\textra data  = %lu\n", pstrucJ2534Msg->ulExtraDataIndex );
        LogMsg1 ( buffer );

        sprintf_s ( buffer, sizeof ( buffer ), "\tdata size   = %lu\n", pstrucJ2534Msg->ulDataSize );
        LogMsg1 ( buffer );
        sprintf_s ( buffer, sizeof ( buffer ), "\tdata        = 0x%lx = { ", ( unsigned long ) pstrucJ2534Msg->ucData );
        LogMsg1 ( buffer );

        for ( unsigned long i = 0; i < pstrucJ2534Msg->ulDataSize; i++ ) {

            sprintf_s ( buffer, sizeof ( buffer ), "0x%02x ", pstrucJ2534Msg->ucData[i] );
            LogMsg1 ( buffer );
        }

        if ( pstrucJ2534Msg->ulExtraDataIndex )
            for ( unsigned int i = pstrucJ2534Msg->ulExtraDataIndex ; i < pstrucJ2534Msg->ulExtraDataIndex + pstrucJ2534Msg->ulDataSize; i++ ) {

                sprintf_s ( buffer, sizeof ( buffer ), "0x%02x ", pstrucJ2534Msg->ucData[i] );
                LogMsg1 ( buffer );
            }

        LogMsg1 ( "}\n" );

    } else
        if ( ret != J2534_ERR_BUFFER_EMPTY ) {
            GetJ2534ErrorText ( ret );

        } else {
//		LogMsg1("");
        }


    return ret;
}

/*

	long PassThruWriteMsgs(unsigned long ChannelID, PASSTHRU_MSG *pMsg, unsigned long *pNumMsgs, unsigned long Timeout);

	Description
		The PassThruWriteMsgs function is used to transmit network protocol messages over an existing logical communication
		channel. The network protocol messages will flow from the User Application to the PassThru device.

	Parameters
		ChannelID
			The logical communication channel identifier assigned by the J2534 API/DLL when the communication channel was opened
			via the PassThruConnect function.
		pMsg
			Pointer to the message structure containing the UserApplication transmit message(s). For sending more than one message,
			this must be a pointer to an array of PASSTHRU_MSG structures.
		pNumMsgs
			Pointer to the variable that contains the number of PASSTHRU_MSG structures that are allocated for transmit frames. On
			function completion this variable will contain the actual number of messages sent to the vehicle network. The transmitted
			number of messages may be less than the number requested by the UserApplication.
		Timeout
			Timeout interval (in milliseconds) to wait for transmit completion. A value of zero instructs the API/DLL to queue as many
			transmit messages as possible and return immediately. A nonzero timeout value instructs the API/DLL to wait for the timeout
			interval to expire before returning. The API/DLL will not wait the entire timeout interval if an error occurs or the specified
			number of messages have been sent.
		See Also
			PassThruReadMsgs
			PassThruStartPeriodicMsg

		Example
			typedef struct {
				unsigned long ProtocolID;	/* vehicle network protocol * /
				unsigned long RxStatus;	/* receive message status * /
				unsigned long TxFlags;	/* transmit message flags * /
				unsigned long Timestamp;	/* receive message timestamp(in microseconds) * /
				unsigned long DataSize;	/* byte size of message payload in the Data array * /
				unsigned long ExtraDataIndex;	/* start of extra data(i.e. CRC, checksum, etc) in Data array * /
				unsigned char Data[4128];	/* message payload or data * /
			} PASSTHRU_MSG;

			unsigned long status;
			unsigned long ChannelID;	/* Logical channel identifier returned by PassThruConnect * /
			unsigned long NumMsgs;
			unsigned long Timeout;
			PASSTHRU_MSG Msg;
			char errstr[256];

			status = PassThruConnect(J1850VPW, 0x00000000, &ChannelID);

			/*
			** Initialize the PASSTHRU_MSG structure to all zeroes.
			** Set the ProtocolID to select protocol frames of interest.
			** Timestamp and ExtraDataIndex are not used for transmit messages.
			** TxFlags does not contain any J1850VPW specific options.
			* /
			memset(&Msg, 0, sizeof(Msg));
			Msg.ProtocolID = J1850VPW;

			/*
			** Create a Diagnostic command message to reset all Emission-related diagnostic information stored by
			** all ECUs within the vehicle network.
			** Program the Priority/Message type byte(first Header byte) for 3-byte form of the consolidated header,
			** functional addressing mode for the target address and a message body that will contain diagnostic
			** command/status messages.
			**
			** Priority/message type options selected(refer to J2178-1 for Detailed Header Formats):
			** Priority:		Header Bits (7-5) = 6, (0 = highest, 7 = lowest)
			** Header Type		Header Bit  (4)   = 0, three byte consolidated header
			** In-Frame Response	Header Bit  (3)   = 0, In-Frame response required
			** Addressing Type	Header Bit  (2)   = 0, Functional
			** Type Modifier	Header Bits (1-0) = 1, Message Type is broadcast
			* /
			Msg.Data[0] = 0x61;	/* Priority/Message type, first Header byte * /

			/*
			** Program the target address(second Header byte) for the legislated diagnostic command message type.
			** (refer to J2178-4 for Message Definitions)
			* /
			Msg.Data[1] = 0x6A;	/* Target functional address, second Header byte * /

			/*
			** Program the physical source address(third Header byte) of the PassThru device.
			** (refer to J2178-4 for Physical Address Assignments)
			* /
			Msg.Data[2] = 0xF1;	/* PassThru device physical source address, third Header byte * /

			/*
			** Program the Diagnostic Test Mode(first Data byte) value for selecting Reset Emission-Related
			** Diagnostic Information. It is expected that all ECUs will respond to this test mode command with
			** a status message that has a one byte data field that contains 0x44.
			** (refer to J1979 for Diagnostic Test Modes)
			* /
			Msg.Data[3] = 0x04;	/* Test Mode 4, reset emission-related diagnostic information, first Data byte * /
			Msg.DataSize = 4;	/* Mode 0x04 message contains 4 bytes * /

			/*
			** Indicate that PASSTHRU_MSG array contains just a single message.
			* /
			NumMsgs = 1;

			/*
			** API/DLL should place transmit message in transmit queue and immediately return.
			* /
			Timeout = 0;

			status = PassThruWriteMsgs(ChannelID, &Msg, &NumMsgs, Timeout);
			if (status != STATUS_NOERROR)
			{
				/*
				** PassThruWriteMsgs failed! Get descriptive error string.
				* /
				PassThruGetLastError(&errstr[0]);

				/*
				** Display Error dialog box and/or write error description to Log file.
				* /
			}
	unsigned long ulProtocolID;
	unsigned long ulRxStatus;
	unsigned long ulTxFlags;
	unsigned long ulTimeStamp;
	unsigned long ulDataSize;
	unsigned long ulExtraDataIndex;
	unsigned char ucData[PASSTHRU_MSG_DATA_SIZE];

*/

extern "C"  J2534ERROR J2534_SIM_API PassThruWriteMsgs (
    unsigned long ulChannelID,
    PASSTHRU_MSG	   * pstrucJ2534Msg,
    unsigned long * pulNumMsgs,
    unsigned long ulTimeout )
{
    J2534ERROR ret = J2534_STATUS_NOERROR;
    char buffer[1024];

    stPassThrough *pPtr = ( stPassThrough* ) ulChannelID ;

    sprintf_s ( buffer, sizeof ( buffer ), "PassThruWriteMsgs(%d,0x%lx,%d,%d)\n", ulChannelID, ( unsigned long ) pstrucJ2534Msg, *pulNumMsgs, ulTimeout );
    LogMsg1 ( buffer );

    LogMsg2 ( "\tprotocol    = %s\n", GetJ2534_PROTOCOLText ( ( J2534_PROTOCOL ) pstrucJ2534Msg->ulProtocolID ) );


// not used for write

//	_RPT1(_CRT_WARN,"\tex data ind = %lu\n",pstrucJ2534Msg->ulExtraDataIndex);
//	_RPT1(_CRT_WARN,"\ttimestamp   = %lu\n",pstrucJ2534Msg->ulTimeStamp);


    sprintf_s ( buffer, sizeof ( buffer ), "\trx status   = %lu\n", pstrucJ2534Msg->ulRxStatus );
    LogMsg1 ( buffer );
    sprintf_s ( buffer, sizeof ( buffer ), "\ttx flags    = %lu\n", pstrucJ2534Msg->ulTxFlags );
    LogMsg1 ( buffer );
    sprintf_s ( buffer, sizeof ( buffer ), "\ttime stamp  = %lu\n", pstrucJ2534Msg->ulTimeStamp );
    LogMsg1 ( buffer );
    sprintf_s ( buffer, sizeof ( buffer ), "\textra data  = %lu\n", pstrucJ2534Msg->ulExtraDataIndex );
    LogMsg1 ( buffer );

    sprintf_s ( buffer, sizeof ( buffer ), "\tdata size   = %lu\n", pstrucJ2534Msg->ulDataSize );
    LogMsg1 ( buffer );
    sprintf_s ( buffer, sizeof ( buffer ), "\tdata        = 0x%lx = { ", ( unsigned long ) pstrucJ2534Msg->ucData );
    LogMsg1 ( buffer );

    if ( pstrucJ2534Msg->ucData != NULL ) {

        for ( unsigned long i = 0; i < pstrucJ2534Msg->ulDataSize; i++ ) {

            sprintf_s ( buffer, sizeof ( buffer ), "0x%02x ", pstrucJ2534Msg->ucData[i] );
            LogMsg1 ( buffer );
        }

        if ( pstrucJ2534Msg->ulExtraDataIndex )
            for ( unsigned int i = pstrucJ2534Msg->ulExtraDataIndex ; i < pstrucJ2534Msg->ulExtraDataIndex + pstrucJ2534Msg->ulDataSize; i++ ) {
                sprintf_s ( buffer, sizeof ( buffer ), "0x%02x ", pstrucJ2534Msg->ucData[i] );
                LogMsg1 ( buffer );
            }
    }

    LogMsg1 ( "}\n" );

#if !defined ( SIMULATION_MODE )

    if ( pPtr ) {

        //Sleep(20);

        ret = ( J2534ERROR ) pPtr->data.pPassThruWriteMsgs ( pPtr->ulChannel, pstrucJ2534Msg, pulNumMsgs, ulTimeout );

        // hack for <REDACTED>
        if ( pstrucJ2534Msg->ulDataSize > 200 )
            ;//Sleep (50); yes it is the intent
    }

#else
    Sleep ( 10 );
    ReadyToSend = 1000;
#endif

    GetJ2534ErrorText ( ret );


    return ret;
}

extern "C"  J2534ERROR J2534_SIM_API PassThruStartPeriodicMsg (
    unsigned long ulChannelID,
    PASSTHRU_MSG * pMsgJ2534,
    unsigned long * pulMsgID,
    unsigned long ulTimeout )
{

    J2534ERROR ret = J2534_STATUS_NOERROR;

    stPassThrough *pPtr = ( stPassThrough* ) ulChannelID ;

    LogMsg1 ( "PassThruStartPeriodicMsg" );

    if ( pPtr )
    { ret = ( J2534ERROR ) pPtr->data.pPassThruStartPeriodicMsg ( pPtr->ulChannel, pMsgJ2534, pulMsgID, ulTimeout ); }

    GetJ2534ErrorText ( ret );

    return ret;
}

extern "C"  J2534ERROR J2534_SIM_API PassThruStopPeriodicMsg (
    unsigned long ulChannelID,
    unsigned long ulMsgID )
{

    J2534ERROR ret = J2534_STATUS_NOERROR;

    stPassThrough *pPtr = ( stPassThrough* ) ulChannelID ;

    LogMsg1 ( "PassThruStopPeriodicMsg\n" );

    if ( pPtr )
    { ret = ( J2534ERROR ) pPtr->data.pPassThruStopPeriodicMsg ( pPtr->ulChannel, ulMsgID ); }

    GetJ2534ErrorText ( ret );

    return ret;
}

extern "C"  J2534ERROR J2534_SIM_API PassThruStartMsgFilter (
    unsigned long	ulChannelID,
    J2534_FILTER	enumFilterType,
    PASSTHRU_MSG		* pstrucJ2534Mask,
    PASSTHRU_MSG		* pstrucJ2534Pattern,
    PASSTHRU_MSG		* pstrucJ2534FlowControl,
    unsigned long * pulFilterID )
{

    J2534ERROR ret = J2534_STATUS_NOERROR;

    stPassThrough *pPtr = ( stPassThrough* ) ulChannelID ;

    LogMsg1 ( "PassThruStartMsgFilter\n" );

#if !defined ( SIMULATION_MODE )

    if ( pPtr )
        ret = ( J2534ERROR ) pPtr->data.pPassThruStartMsgFilter ( pPtr->ulChannel, enumFilterType,
                pstrucJ2534Mask, pstrucJ2534Pattern, pstrucJ2534FlowControl, pulFilterID );

#endif

    GetJ2534ErrorText ( ret );

    return ret;
}

extern "C"  J2534ERROR J2534_SIM_API PassThruStopMsgFilter (
    unsigned long ulChannelID,
    unsigned long ulFilterID )
{
    J2534ERROR ret = J2534_STATUS_NOERROR;

    stPassThrough *pPtr = ( stPassThrough* ) ulChannelID ;

    LogMsg1 ( "PassThruStopMsgFilter\n" );

#if !defined ( SIMULATION_MODE )

    if ( pPtr )
    { ret = ( J2534ERROR ) pPtr->data.pPassThruStopMsgFilter ( pPtr->ulChannel, ulFilterID ); }

#endif
    GetJ2534ErrorText ( ret );

    return ret;
}
extern "C"  J2534ERROR J2534_SIM_API PassThruSetProgrammingVoltage (
    unsigned long ulPin,
    unsigned long ulVoltage )
{
    J2534ERROR ret = J2534_STATUS_NOERROR;


    stPassThrough *pPtr = pGlobalPtr ;


    LogMsg1 ( "PassThruSetProgrammingVoltage\n" );

#if !defined ( SIMULATION_MODE )

    if ( pPtr ) {
#if !defined( BUILD_TRANSLATE_0202 )
        ret = ( J2534ERROR ) pPtr->data.pPassThruSetProgrammingVoltage ( ulPin, ulVoltage );
#else
        ret = ( J2534ERROR ) pPtr->data.pPassThruSetProgrammingVoltage ( ulPin, ulVoltage, 0 );
#endif
    }

#endif

    GetJ2534ErrorText ( ret );

    return ret;
}

extern "C"  J2534ERROR J2534_SIM_API PassThruReadVersion (
    char *pchFirmwareVersion,
    char *pchDllVersion,
    char *pchApiVersion )
{
    J2534ERROR ret = J2534_STATUS_NOERROR;

    stPassThrough *pPtr = pGlobalPtr ;

    LogMsg1 ( "PassThruReadVersion\n" );

#if !defined ( SIMULATION_MODE )

    if ( pPtr )
#if !defined( BUILD_TRANSLATE_0202 )
    { ret = ( J2534ERROR ) pPtr->data.pPassThruReadVersion ( pchFirmwareVersion, pchDllVersion, pchApiVersion ); }

#else
    { ret = ( J2534ERROR ) pPtr->data.pPassThruReadVersion ( 0, pchFirmwareVersion, pchDllVersion, pchApiVersion ); }
#endif

#else

    strcpy ( pchFirmwareVersion, "1.00" );
    strcpy ( pchDllVersion, "1.00" );
    strcpy ( pchApiVersion, "1.00" );

#endif

    GetJ2534ErrorText ( ret );


    return ret;
}

extern "C"  J2534ERROR J2534_SIM_API PassThruGetLastError (
    //unsigned long ulErrorID,
    char *pchErrorDescription )
{
    J2534ERROR ret = J2534_STATUS_NOERROR;

    stPassThrough *pPtr = pGlobalPtr; ;

    LogMsg1 ( "PassThruGetLastError\n" );

#if !defined ( SIMULATION_MODE )

    if ( pPtr )
    { ret = ( J2534ERROR ) pPtr->data.pPassThruGetLastError ( pchErrorDescription ); }

#endif

    GetJ2534ErrorText ( ret );

    return ret;

}

/*

long PassThruIoctl(unsigned long ChannelID, unsigned long IoctlID, void *pInput, void *pOutput);

	Description
		This function sets the various network protocol timing parameters that control the transmission/reception of network frames by
		the PassThru device. On successful completion the PassThru device DATA_RATE value will be set to the value placed in CfgItem.Value.
		The values for multiple SET_CONFIG network parameters may be set with one function call by initializing an array of SCONFIG items.

	Parameters
		ChannelID
			The logical communication channel identifier assigned by the J2534 API/DLL when the communication channel was opened via the
			PassThruConnect function.
		IoctlID
			Ioctl function identifier.
		pInput
			Input structure pointer.
		pOutput
			Output structure pointer.

	See Also
		PassThruIoctl (GET_CONFIG)

	Example
		typedef struct {
			unsigned long Parameter;	// Name of configuration parameter
			unsigned long Value;		// Value of configuration parameter
		} SCONFIG;

		typedef struct {
			unsigned long NumOfParams;	// sizeof SCONFIG array
			SCONFIG *ConfigPtr;			// array containing configuration item(s)
		} SCONFIG_LIST;

		unsigned long status;
		unsigned long ChannelID;		// Logical channel identifier returned by PassThruConnect
		SCONFIG CfgItem;
		SCONFIG_LIST Input;
		char errstr[256];

		status = PassThruConnect(J1850VPW, 0x00000000, &ChannelID);

		CfgItem.Parameter = DATA_RATE;
		CfgItem.Value = 10400;

		Input.NumOfParams = 1;
		Input.ConfigPtr = &CfgItem;

		status = PassThruIoctl(ChannelID, SET_CONFIG, (void *)&Input, (void *)NULL);
		if (status != STATUS_NOERROR)
		{
		// PassThruIoctl(SET_CONFIG) failed! Get descriptive error string.

			PassThruGetLastError(&errstr[0]);

			// Display Error dialog box and/or write error description to Log file.
		}

*/

extern "C"  J2534ERROR J2534_SIM_API PassThruIoctl (
    unsigned long ulChannelID,
    J2534IOCTLID enumIoctlID,
    void *pInput,
    void *pOutput )
{
    char buffer[1024];

    SCONFIG_LIST *scl_ptr = ( SCONFIG_LIST* ) pInput;
    J2534ERROR ret = J2534_STATUS_NOERROR;

    stPassThrough *pPtr = ( stPassThrough* ) ulChannelID ;


    sprintf_s ( buffer, sizeof ( buffer ), "PassThruIoctl(%d,%s,0x%lx,0x%lx)\n", ulChannelID, GetJ2534IOCTLIDText ( enumIoctlID ), ( unsigned long ) pInput, ( unsigned long ) pOutput );
    LogMsg1 ( buffer );

    if ( enumIoctlID == SET_CONFIG ) {

        SCONFIG *sc_ptr ;


        if ( scl_ptr ) {

            sc_ptr = scl_ptr->pConfigPtr ;

            sprintf_s ( buffer, sizeof ( buffer ), "\tulNumOfParams = %d\n", scl_ptr->ulNumOfParams );
            LogMsg1 ( buffer );

        }

        if ( sc_ptr ) {

            if ( P2_MIN == sc_ptr->Parameter ) {
                return J2534_STATUS_NOERROR;
            }

            sprintf_s ( buffer, sizeof ( buffer ), "\tParameter = %s\n", GetJ2534IOCTLPARAMIDText ( sc_ptr->Parameter ) );
            LogMsg1 ( buffer );

            sprintf_s ( buffer, sizeof ( buffer ), "\tulValue   = %d\n", sc_ptr->ulValue );
            LogMsg1 ( buffer );

        }

    }

#if !defined ( SIMULATION_MODE )

    if ( pPtr ) {
        ret = ( J2534ERROR ) pPtr->data.pPassThruIoctl ( pPtr->ulChannel, enumIoctlID, pInput, pOutput );
    }

#endif

    if ( enumIoctlID == GET_CONFIG ) {

        SCONFIG *sc_ptr ;

        if ( scl_ptr ) {

            sc_ptr = scl_ptr->pConfigPtr ;

            sprintf_s ( buffer, sizeof ( buffer ), "\tulNumOfParams = %d\n", scl_ptr->ulNumOfParams );
            LogMsg1 ( buffer );

        }

        if ( sc_ptr ) {

            sprintf_s ( buffer, sizeof ( buffer ), "\tParameter = %s\n", GetJ2534IOCTLPARAMIDText ( sc_ptr->Parameter ) );
            LogMsg1 ( buffer );

            sprintf_s ( buffer, sizeof ( buffer ), "\tulValue   = %d\n", sc_ptr->ulValue );
            LogMsg1 ( buffer );

        }

    } else
        if ( enumIoctlID == CLEAR_RX_BUFFER || enumIoctlID == CLEAR_TX_BUFFER ) {

            ReadyToSend = 1000;

        }

    GetJ2534ErrorText ( ret );

    return ret;
}

#endif

static char *GetJ2534IOCTLIDText ( J2534IOCTLID enumIoctlID )
{
    switch ( enumIoctlID ) {

        case GET_CONFIG:
            return "GET_CONFIG";

        case SET_CONFIG:
            return "SET_CONFIG";

        case READ_VBATT:
            return "READ_VBATT";

        case FIVE_BAUD_INIT:
            return "FIVE_BAUD_INIT";

        case FAST_INIT:
            return "FAST_INIT";

#ifdef SET_PIN_USE

        case SET_PIN_USE:
            return "SET_PIN_USE";
#endif

        case CLEAR_TX_BUFFER:
            return "CLEAR_TX_BUFFER";

        case CLEAR_RX_BUFFER:
            return "CLEAR_RX_BUFFER";

        case CLEAR_PERIODIC_MSGS:
            return "CLEAR_PERIODIC_MSGS";

        case CLEAR_MSG_FILTERS:
            return "CLEAR_MSG_FILTERS";

        case CLEAR_FUNCT_MSG_LOOKUP_TABLE:
            return "CLEAR_FUNCT_MSG_LOOKUP_TABLE";

        case ADD_TO_FUNCT_MSG_LOOKUP_TABLE:
            return "ADD_TO_FUNCT_MSG_LOOKUP_TABLE";

        case DELETE_FROM_FUNCT_MSG_LOOKUP_TABLE:
            return "DELETE_FROM_FUNCT_MSG_LOOKUP_TABLE";

        case READ_PROG_VOLTAGE:
            return "READ_PROG_VOLTAGE";

        default:
            return "unknown";
    }
}

static char *GetJ2534_PROTOCOLText ( J2534_PROTOCOL protocol )
{
    switch ( protocol ) {

        case J1850VPW:								// J1850VPW Protocol
            return "J1850VPW";

        case J1850PWM:								// J1850PWM Protocol
            return "J1850PWM";

        case ISO9141:								// ISO9141 Protocol
            return "ISO9141";

        case ISO14230:								// ISO14230 Protocol
            return "ISO14230";

        case CAN:									// CAN Protocol
            return "CAN";

        case ISO15765:
            return "ISO15765";

        case SCI_A_ENGINE:
            return "SCI_A_ENGINE";

        case SCI_A_TRANS:
            return "SCI_A_TRANS";

        case SCI_B_ENGINE:
            return "SCI_B_ENGINE";

        case SCI_B_TRANS:
            return "SCI_B_TRANS";

#ifdef ISO9141_FORD

        case ISO9141_FORD:							// ISO9141 FORD Protocol
            return "ISO9141_FORD";

        case UBP:									// UBP Protocol
            return "UBP";

        case DDL:									// DDL Protocol
            return "DDL";

        //**** NOTE ****
        //		 ALWAYS ADD NEW PROTOCOL
        //		 BEFORE J2534_PROTOCOL_NUM
        //		 SO THAT THIS VALUE AUTOMATICALLY
        //		 GETS UPDATED WHEN A NEW PROTOCOL
        //		 IS ADDED.
        case J2534_PROTOCOL_NUM:
#endif
        default:
            return "unknown";
    }
}

static char* GetJ2534IOCTLPARAMIDText ( J2534IOCTLPARAMID value )
{

    switch ( value  ) {
        case DATA_RATE:
            return "DATA_RATE";

        case LOOPBACK:
            return "LOOPBACK";

        case NODE_ADDRESS:
            return "NODE_ADDRESS";

        case NETWORK_LINE:
            return "NETWORK_LINE";

        case P1_MIN:
            return "P1_MIN";

        case P1_MAX:
            return "P1_MAX";

        case P2_MIN:
            return "P2_MIN";

        case P2_MAX:
            return "P2_MAX";

        case P3_MIN:
            return "P3_MIN";

        case P3_MAX:
            return "P3_MAX";

        case P4_MIN:
            return "P4_MIN";

        case P4_MAX:
            return "P4_MAX";

        case W1:
            return "W1";

        case W2:
            return "W2";

        case W3:
            return "W3";

        case W4:
            return "W4";

        case W5:
            return "W5";

        case TIDLE:
            return "TIDLE";

        case TINIL:
            return "TINIL";

        case TWUP:
            return "TWUP";

        case PARITY:
            return "PARITY";

        case BIT_SAMPLE_POINT:
            return "BIT_SAMPLE_POINT";

        case SYNC_JUMP_WIDTH:
            return "SYNC_JUMP_WIDTH";

        case W0:
            return "W0";

        case T1_MAX:
            return "T1_MAX";

        case T2_MAX:
            return "T2_MAX";

        case T4_MAX:
            return "T4_MAX";

        case T5_MAX:
            return "T5_MAX";

        case ISO15765_BS:
            return "ISO15765_BS";

        case ISO15765_STMIN:
            return "ISO15765_STMIN";

        case DATA_BITS:
            return "DATA_BITS";

        default:
            return "GetJ2534IOCTLPARAMIDText unknown";
    }
}

//helllo have we heard of varags.

static void LogMsg1 ( char *str )
{

    // stops the T4 from starting a reflash
//	Sleep(10);

    FILE *fp = NULL;
    int errno = fopen_s ( &fp, LOG_FILE_FILENAME, "a" );

    _RPT0 ( _CRT_WARN, str );
//    printf ( "%s\n", str );

    if ( fp ) {
        fprintf ( fp, str );
        fclose ( fp );
    }
}

static void LogMsg2 ( char *str, char*str1 )
{
    FILE *fp = NULL;

    // stops t4 flashing
    //	Sleep( 50 );

    Sleep ( 10 );
    int errno = fopen_s ( &fp, LOG_FILE_FILENAME, "a" );
    _RPT1 ( _CRT_WARN, str, str1 );


    if ( fp ) {
        fprintf ( fp, str, str1 );

        fclose ( fp );
    }
}

#if !(defined(  BUILD_0404 ) || defined( BUILD_TRANSLATE_0202 ))

static void GetJ2534ErrorText ( J2534ERROR err )
{
    switch ( err ) {

        case J2534_STATUS_NOERROR:
            LogMsg1 ( "\tFunction call successful.\n" );
            break;

        case J2534_ERR_NOT_SUPPORTED:
            LogMsg1 ( "\tFunction not supported.\n" );
            break;

        case J2534_ERR_INVALID_CHANNEL_ID:
            LogMsg1 ( "\tInvalid ChannelID value.\n" );
            break;

        case J2534_ERR_INVALID_PROTOCOL_ID:
            LogMsg1 ( "\tInvalid ProtocolID value.\n" );
            break;

        case J2534_ERR_NULLPARAMETER:
            LogMsg1 ( "\tNULL pointer supplied where a valid \n\tpointer is required.\n" );
            break;

        case J2534_ERR_INVALID_IOCTL_VALUE:
            LogMsg1 ( "\tInvalid value for Ioctl parameter \n" );
            break;

        case J2534_ERR_INVALID_FLAGS:
            LogMsg1 ( "\tInvalid flag values.\n" );
            break;

        case J2534_ERR_FAILED:
            LogMsg1 ( "\tUndefined error: use \n\tPassThruGetLastError for description\nof error.\n" );
            break;

        case J2534_ERR_DEVICE_NOT_CONNECTED:
            LogMsg1 ( "\tDevice not connected to PC\n" );
            break;

        case J2534_ERR_TIMEOUT:
            LogMsg1 ( "\tTimeout. No message available to \n\tread or \n\tcould not read the specified no. of msgs.\n" );
            break;

        case J2534_ERR_INVALID_MSG:
            LogMsg1 ( "\tInvalid message structure pointed \n\tto by pMsg.\n" );
            break;

        case J2534_ERR_INVALID_TIME_INTERVAL:
            LogMsg1 ( "\tInvalid TimeInterval value.\n" );
            break;

        case J2534_ERR_EXCEEDED_LIMIT:
            LogMsg1 ( "\tALL periodic message IDs have been \n\tused.\n" );
            break;

        case J2534_ERR_INVALID_MSG_ID:
            LogMsg1 ( "\tInvalid MsgID value.\n" );
            break;

        case J2534_ERR_INVALID_ERROR_ID:
            LogMsg1 ( "\tInvalid ErrorID value.\n" );
            break;

        case J2534_ERR_INVALID_IOCTL_ID:
            LogMsg1 ( "\tInvalid IoctlID value.\n" );
            break;

        case J2534_ERR_BUFFER_EMPTY:
            LogMsg1 ( "\tProtocol message buffer empty.\n" );
            break;

        case J2534_ERR_BUFFER_FULL:
            LogMsg1 ( "\tProtocol message buffer full.\n" );
            break;

        case J2534_ERR_BUFFER_OVERFLOW:
            LogMsg1 ( "\tProtocol message buffer overflow.\n" );
            break;

        case J2534_ERR_PIN_INVALID:
            LogMsg1 ( "\tInvalid pin number.\n" );
            break;

        case J2534_ERR_CHANNEL_IN_USE:
            LogMsg1 ( "\tChannel already in use.\n" );
            break;

        case J2534_ERR_MSG_PROTOCOL_ID:
            LogMsg1 ( "\tProtocol type does not match the\n\tprotocol associated with the \n\tChannel ID\n" );
            break;

        case J2534_ERR_INVALID_FILTER_ID:
            LogMsg1 ( "\tInvalid MsgID value\n" );
            break;

        case J2534_ERROR_NO_FLOW_CONTROL:
            LogMsg1 ( "\tAn attempt was made to send a message on an ISO15765 ChannelID before a flow\n\tcontrol filter was established.\n" );
            break;

        default:
            LogMsg1 ( "\tUnknown error\n" );
            break;

    }
}

#else


// part of the translate from 202 to 404
#if defined( BUILD_TRANSLATE_0202 )

/*************/
/* Error IDs */
/*************/
#define STATUS_NOERROR						0x00
#define ERR_NOT_SUPPORTED					0x01
#define ERR_INVALID_CHANNEL_ID					0x02
#define ERR_INVALID_PROTOCOL_ID					0x03
#define ERR_NULL_PARAMETER					0x04
#define ERR_INVALID_IOCTL_VALUE					0x05
#define ERR_INVALID_FLAGS					0x06
#define ERR_FAILED						0x07
#define ERR_DEVICE_NOT_CONNECTED				0x08
#define ERR_TIMEOUT						0x09
#define ERR_INVALID_MSG						0x0A
#define ERR_INVALID_TIME_INTERVAL				0x0B
#define ERR_EXCEEDED_LIMIT					0x0C
#define ERR_INVALID_MSG_ID					0x0D
#define ERR_DEVICE_IN_USE					0x0E
#define ERR_INVALID_IOCTL_ID					0x0F
#define ERR_BUFFER_EMPTY					0x10
#define ERR_BUFFER_FULL						0x11
#define ERR_BUFFER_OVERFLOW					0x12
#define ERR_PIN_INVALID						0x13
#define ERR_CHANNEL_IN_USE					0x14
#define ERR_MSG_PROTOCOL_ID					0x15
#define ERR_INVALID_FILTER_ID					0x16
#define ERR_NO_FLOW_CONTROL					0x17
#define ERR_NOT_UNIQUE						0x18
#define ERR_INVALID_BAUDRATE					0x19
#define ERR_INVALID_DEVICE_ID					0x1A

#define ERR_NULLPARAMETER		/*v2*/			ERR_NULL_PARAMETER

#endif

/*
 * GetJ2534ErrorText - Convert error code to text
 *	Input: J2534ERROR err
 */

static void GetJ2534ErrorText ( J2534ERROR err )
{

    switch ( err ) {
        case STATUS_NOERROR						:         //    0x00
            LogMsg1 ( "STATUS_NOERROR\n" );
            break;

        case ERR_NOT_SUPPORTED					:         //    0x01
            LogMsg1 ( "ERR_NOT_SUPPORTED\n" );
            break;

        case ERR_INVALID_CHANNEL_ID					:         //    0x02
            LogMsg1 ( "ERR_INVALID_CHANNEL_ID\n" );
            break;

        case ERR_INVALID_PROTOCOL_ID					:         //    0x03
            LogMsg1 ( "ERR_INVALID_PROTOCOL_ID\n" );
            break;

        case ERR_NULL_PARAMETER					:         //    0x04
            LogMsg1 ( "ERR_NULL_PARAMETER\n" );
            break;

        case ERR_INVALID_IOCTL_VALUE					:         //    0x05
            LogMsg1 ( "ERR_INVALID_IOCTL_VALUE\n" );
            break;

        case ERR_INVALID_FLAGS					:         //    0x06
            LogMsg1 ( "ERR_INVALID_FLAGS\n" );
            break;

        case ERR_FAILED						:         //    0x07
            LogMsg1 ( "ERR_FAILED\n" );
            break;

        case ERR_DEVICE_NOT_CONNECTED				:         //    0x08
            LogMsg1 ( "ERR_DEVICE_NOT_CONNECTED\n" );
            break;

        case ERR_TIMEOUT						:         //    0x09
            LogMsg1 ( "ERR_TIMEOUT\n" );
            break;

        case ERR_INVALID_MSG						:         //    0x0A
            LogMsg1 ( "ERR_INVALID_MSG\n" );
            break;

        case ERR_INVALID_TIME_INTERVAL				:         //    0x0B
            LogMsg1 ( "ERR_INVALID_TIME_INTERVAL\n" );
            break;

        case ERR_EXCEEDED_LIMIT					:         //    0x0C
            LogMsg1 ( "ERR_EXCEEDED_LIMIT\n" );
            break;

        case ERR_INVALID_MSG_ID					:         //    0x0D
            LogMsg1 ( "ERR_INVALID_MSG_ID\n" );
            break;

        case ERR_DEVICE_IN_USE					:         //    0x0E
            LogMsg1 ( "ERR_DEVICE_IN_USE\n" );
            break;

        case ERR_INVALID_IOCTL_ID					:         //    0x0F
            LogMsg1 ( "ERR_INVALID_IOCTL_ID\n" );
            break;

        case ERR_BUFFER_EMPTY					:         //    0x10
            LogMsg1 ( "ERR_BUFFER_EMPTY\n" );
            break;

        case ERR_BUFFER_FULL						:         //    0x11
            LogMsg1 ( "ERR_BUFFER_FULL\n" );
            break;

        case ERR_BUFFER_OVERFLOW					:         //    0x12
            LogMsg1 ( "ERR_BUFFER_OVERFLOW\n" );
            break;

        case ERR_PIN_INVALID						:         //    0x13
            LogMsg1 ( "ERR_PIN_INVALID\n" );
            break;

        case ERR_CHANNEL_IN_USE					:         //    0x14
            LogMsg1 ( "ERR_CHANNEL_IN_USE\n" );
            break;

        case ERR_MSG_PROTOCOL_ID					:         //    0x15
            LogMsg1 ( "ERR_MSG_PROTOCOL_ID\n" );
            break;

        case ERR_INVALID_FILTER_ID					:         //    0x16
            LogMsg1 ( "ERR_INVALID_FILTER_ID\n" );
            break;

        case ERR_NO_FLOW_CONTROL					:         //    0x17
            LogMsg1 ( "ERR_NO_FLOW_CONTROL - No ISO15765 flow control filter is set, or no filter matches the header of an outgoing message\n" );
            break;

        case ERR_NOT_UNIQUE						:         //    0x18
            LogMsg1 ( "ERR_NOT_UNIQUE\n" );
            break;

        case ERR_INVALID_BAUDRATE					:         //    0x19
            LogMsg1 ( "ERR_INVALID_BAUDRATE\n" );
            break;

        case ERR_INVALID_DEVICE_ID					:         //    0x1A
            LogMsg1 ( "ERR_INVALID_DEVICE_ID\n" );
            break;
    }
}

#endif

static int Load_J2534DLL (  char *szLibrary, globData * data )
{
    HINSTANCE hDLL;

    memset ( data, 0, sizeof ( globData ) );

    /**********************************************************************/
    //
    //	Once user selects the specifie API & hardware, load the vendor
    //	supplied API calls.
    //
    /**********************************************************************/

    if ( ( hDLL = LoadLibrary ( szLibrary ) ) == NULL ) {
        LogMsg2 ( "ERROR: Cannot load %s\n", szLibrary );
        return ( FAIL );
    }

#if defined( BUILD_0404 ) || defined(   BUILD_TRANSLATE_0202)

    /// 0404
    if ( ( data->pPassThruOpen = ( PTOPEN ) GetProcAddress ( hDLL, "PassThruOpen" ) ) == NULL ) {
        LogMsg2 ( "ERROR: Cannot find PassThruOpen function in %s\n", szLibrary );
        return ( FAIL );
    }

    if ( ( data->pPassThruClose = ( PTCLOSE ) GetProcAddress ( hDLL, "PassThruClose" ) ) == NULL ) {
        LogMsg2 ( "ERROR: Cannot find PassThruClose function in %s\n", szLibrary );
        return ( FAIL );
    }


#endif

    if ( ( data->pPassThruConnect = ( PTCONNECT ) GetProcAddress ( hDLL, "PassThruConnect" ) ) == NULL ) {
        LogMsg2 ( "ERROR: Cannot find PassThruConnect function in %s\n", szLibrary );
        return ( FAIL );
    }

    if ( ( data->pPassThruDisconnect = ( PTDISCONNECT ) GetProcAddress ( hDLL, "PassThruDisconnect" ) ) == NULL ) {
        LogMsg2 ( "ERROR: Cannot find PassThruDisconnect function in %s\n", szLibrary );
        return ( FAIL );
    }

    if ( ( data->pPassThruReadMsgs = ( PTREADMSGS ) GetProcAddress ( hDLL, "PassThruReadMsgs" ) ) == NULL ) {
        LogMsg2 ( "ERROR: Cannot find PassThruReadMsgs function in %s\n", szLibrary );
        return ( FAIL );
    }

    if ( ( data->pPassThruWriteMsgs = ( PTWRITEMSGS ) GetProcAddress ( hDLL, "PassThruWriteMsgs" ) ) == NULL ) {
        LogMsg2 ( "ERROR: Cannot find PassThruWriteMsgs function in %s\n", szLibrary );
        return ( FAIL );
    }

    if ( ( data->pPassThruStartPeriodicMsg = ( PTSTARTPERIODICMSG ) GetProcAddress ( hDLL, "PassThruStartPeriodicMsg" ) ) == NULL ) {
        LogMsg2 ( "ERROR: Cannot find PassThruStartPeriodicMsg function in %s\n", szLibrary );
        return ( FAIL );
    }

    if ( ( data->pPassThruStopPeriodicMsg = ( PTSTOPPERIODICMSG ) GetProcAddress ( hDLL, "PassThruStopPeriodicMsg" ) ) == NULL ) {
        LogMsg2 ( "ERROR: Cannot find PassThruStopPeriodicMsg function in %s\n", szLibrary );
        return ( FAIL );
    }

    if ( ( data->pPassThruStartMsgFilter = ( PTSTARTMSGFILTER ) GetProcAddress ( hDLL, "PassThruStartMsgFilter" ) ) == NULL ) {
        LogMsg2 ( "ERROR: Cannot find PassThruStartMsgFilter function in %s\n", szLibrary );
        return ( FAIL );
    }

    if ( ( data->pPassThruStopMsgFilter = ( PTSTOPMSGFILTER ) GetProcAddress ( hDLL, "PassThruStopMsgFilter" ) ) == NULL ) {
        LogMsg2 ( "ERROR: Cannot find PassThruStopMsgFilter function in %s\n", szLibrary );
        return ( FAIL );
    }

    if ( ( data->pPassThruSetProgrammingVoltage = ( PTSETPROGRAMMINGVOLTAGE ) GetProcAddress ( hDLL, "PassThruSetProgrammingVoltage" ) ) == NULL ) {
        LogMsg2 ( "ERROR: Cannot find PassThruSetProgrammingVoltage function in %s\n", szLibrary );
        return ( FAIL );
    }

    if ( ( data->pPassThruReadVersion = ( PTREADVERSION ) GetProcAddress ( hDLL, "PassThruReadVersion" ) ) == NULL ) {
        LogMsg2 ( "ERROR: Cannot find PassThruReadVersion function in %s\n", szLibrary );
        return ( FAIL );
    }

    if ( ( data->pPassThruGetLastError = ( PTGETLASTERROR ) GetProcAddress ( hDLL, "PassThruGetLastError" ) ) == NULL ) {
        LogMsg2 ( "ERROR: Cannot find PassThruGetLastError function in %s\n", szLibrary );
        return ( FAIL );
    }

    if ( ( data->pPassThruIoctl = ( PTIOCTL ) GetProcAddress ( hDLL, "PassThruIoctl" ) ) == NULL ) {
        LogMsg2 ( "ERROR: Cannot find PassThruIoctl function in %s\n", szLibrary );
        return ( FAIL );
    }

    return ( PASS );
}


// rarely used
#ifdef BUILD_0404

JTYPE PassThruExConfigureWiFi ( void )
{
    LogMsg1 ( "PassThruExConfigureWiFi" );
    __asm int 3

    return 0;
}
JTYPE PassThruExDeviceWatchdog ( void )
{
    LogMsg1 ( "PassThruExDeviceWatchdog" );
    __asm int 3
    return 0;
}

JTYPE PassThruExDownloadCypheredFlashData ( void )
{
    LogMsg1 ( "PassThruExDownloadCypheredFlashData" );
    __asm int 3
    return 0;
}

JTYPE PassThruExEraseFlash ( void )
{
    LogMsg1 ( "PassThruExEraseFlash" );
    __asm int 3
    return 0;
}

JTYPE PassThruExInitiateCypheredFlashDownload ( void )
{
    LogMsg1 ( "" );
    __asm int 3
    return 0;
}

JTYPE PassThruExReadFlash ( void )
{
    LogMsg1 ( "PassThruExReadFlash" );
    __asm int 3
    return 0;
}

JTYPE PassThruExResetFlash ( void )
{
    LogMsg1 ( "PassThruExResetFlash" );
    __asm int 3
    return 0;
}

JTYPE PassThruExRunSelfTest ( void )
{
    LogMsg1 ( "PassThruExRunSelfTest" );
    __asm int 3
    return 0;
}

JTYPE PassThruExWriteFlash ( void )
{
    LogMsg1 ( "PassThruExWriteFlash" );
    __asm int 3
    return 0;
}


#endif
