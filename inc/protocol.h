
/******************************************************************************/
/**
* @file Protocol.h
* @brief Header file for state machine for bootloader protocol
*
*******************************************************************************/
#ifndef PROTO_SM
#define PROTO_SM
/* ***************** Header / include files ( #include ) **********************/
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "common.h"
#include "usart.h"
#include "command.h"
#include "flash.h"
#include "packet.h"
#include "bsp.h"
/* *************** Constant / macro definitions ( #define ) *******************/
/* ********************* Type definitions ( typedef ) *************************/
typedef union myCmd{
	uint8_t 				bufferCMD[2];
	eRESPONSE_ID 		returnValue;
	eCOMMAND_ID 		receivedvalue;
}tCmdUnion;

typedef union myPayload{
	tDATA_PACKET 		packet;
	uint8_t 				bufferPLD[68];
}tPldUnion;

typedef union myAppData{
	tFIRMWARE_PARAM Firmware;
	uint8_t 				bufferData[4];
}tAppDataUnion;

typedef enum {
	eDefaultState = 0,
	eFlashEraseCMD,
	eWriteMemory,
	ePayloadReceive,
	ePayloadCheck,
	eWriteAppCRC,
	eFinishUpdate,
	eFlashVerifyApplication,
	eStartAppCMD
}tProtoState;
/* ***************** Global data declarations ( extern ) **********************/
/* ***** External parameter / constant declarations ( extern const ) **********/
/* ********************** Global func/proc prototypes *************************/
eFUNCTION_RETURN ProtocolSM_Run(const tBSPStruct *);

#endif
