/*  Copyright (C) MOXA Inc. All rights reserved.

    This software is distributed under the terms of the
    MOXA License.  See the file COPYING-MOXA for details.
*/

/*
    mxcanfunc.h
    
    Defines and structures of Moxa CAN Bus Function Library.
    
    2009-07-27	Eric Lo
		Official released ver 1.0
*/

#ifndef _MXCANFUNC_H_
#define _MXCANFUNC_H_
#endif


//------------------------------------------------------------------------------

typedef struct _CNIO_DEV_INFO_STRUCT
{
	DWORD CNIOID;
	DWORD Reserved;
	CHAR ProductName[32];
	CHAR DriverName[32];
	CHAR DeviceLocation[32];
	BYTE PortCount;
} CNIO_DEV_INFO, *PCNIO_DEV_INFO;

typedef struct _CNIO_MSG_STRUCT
{
	DWORD ID;
	BYTE FrameType;
	BYTE Length;
	BYTE Data[8];
} CNIO_MSG, *PCNIO_MSG;

//------------------------------------------------------------------------------

#ifndef EXTERN_C
#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C extern
#endif
#endif

#define CNIO_MAX_BOARDS             4
#define CNIO_MAX_PORTS_PER_BOARD	2
#define CNIO_SUCCESS				1

#define CNIO_PORT1_INDEX			0
#define CNIO_PORT2_INDEX			1

#define CNIO_STANDARD_FRAME         0x00
#define CNIO_EXTENDED_FRAME         0x80
#define CNIO_ERROR_FRAME			0x20
#define CNIO_RTR					0x40
#define CNIO_SRR					0x10
#define CNIO_DATA_LENGTH0			0x00
#define CNIO_DATA_LENGTH1			0x01
#define CNIO_DATA_LENGTH2			0x02
#define CNIO_DATA_LENGTH3			0x03
#define CNIO_DATA_LENGTH4			0x04
#define CNIO_DATA_LENGTH5			0x05
#define CNIO_DATA_LENGTH6			0x06
#define CNIO_DATA_LENGTH7			0x07
#define CNIO_DATA_LENGTH8			0x08

#define CNIO_OPMODE_LISTEN_ONLY     0x01
#define CNIO_OPMODE_SELF_TEST		0x02
#define CNIO_OPMODE_SINGLE_ACC      0x00
#define CNIO_OPMODE_DUAL_ACC		0x04
#define CNIO_OPMODE_ERROR_FRAME     0x08

#define CNIO_OPT_STANDARD			0x00
#define CNIO_OPT_EXTENDED			0x01
#define CNIO_OPT_ACCEPT_RTR         0x02
#define CNIO_OPT_ACCEPT_RTR_ONLY	0x04

#define CNIO_STATUS_TX_PENDING      0x20
#define CNIO_STATUS_OVRRUN          0x02
#define CNIO_STATUS_ERRLIM			0x40
#define CNIO_STATUS_BUS_OFF         0x80
#define CNIO_STATUS_RESET_MODE      0x0100

#define B_10K						0x311C
#define B_20K						0x181C
#define B_50K						0x091C
#define B_100K						0x041C
#define B_125K						0x031C
#define B_250K						0x011C
#define B_500K						0x001C
#define B_800K						0x0016
#define B_1000K						0x0014

//------------------------------------------------------------------------------

//
//Functions
//

EXTERN_C
LONG WINAPI cnio_enum_devices(
	PCNIO_DEV_INFO dev_info,
	LONG size
	);

//------------------------------------------------------------------------------

EXTERN_C
HANDLE WINAPI cnio_open(
	DWORD cnioid_device,
	LONG port_index
	);

EXTERN_C
LONG WINAPI cnio_close(
	HANDLE hPort
	);

EXTERN_C
LONG WINAPI cnio_start(
	HANDLE hPort
	);

EXTERN_C
LONG WINAPI cnio_stop(
	HANDLE hPort
	);

EXTERN_C
LONG WINAPI cnio_reset(
	HANDLE hPort
	);

EXTERN_C
LONG WINAPI cnio_init(
	HANDLE hPort,
	BYTE opmode,
	WORD wBTR0BTR1
	);

EXTERN_C
LONG WINAPI cnio_set_filters_ex(
	HANDLE hPort,
	DWORD acc_code,
	DWORD acc_mask
	);

EXTERN_C
LONG WINAPI cnio_set_filters(
	HANDLE hPort,
	DWORD acc_ID_code,
	DWORD acc_ID_mask,
	BYTE options
	);

EXTERN_C
LONG WINAPI cnio_send_message(
	HANDLE hPort,
	PCNIO_MSG msg,
	DWORD timeout
	);

EXTERN_C
LONG WINAPI cnio_receive_message(
	HANDLE hPort,
	PCNIO_MSG msg,
	DWORD timeout	
	);

EXTERN_C
LONG WINAPI cnio_receive_queue(
	HANDLE hPort
	);

EXTERN_C
LONG WINAPI cnio_status(
	HANDLE hPort,
	DWORD *dwstatus
	);

EXTERN_C
LONG WINAPI cnio_clear_data_overrun(
	HANDLE hPort
	);

//------------------------------------------------------------------------------
