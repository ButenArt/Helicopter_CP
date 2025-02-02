#include "CAN4.h"
#pragma warning(disable: 4100)

int CAN_CAN4::ReadBuffer( int port )
{
	return 0;
}

int CAN_CAN4::SendPacket( DWORD addr, BYTE FrameType, BYTE Length, BYTE frame[ ], int port )
{
	return CNIO_SUCCESS;
}

int CAN_CAN4::ReadPacket( DWORD& addr, BYTE& FrameType, BYTE& Length, BYTE frame[ ], int port )
{
	return CNIO_SUCCESS;
}
#pragma warning(default: 4100)