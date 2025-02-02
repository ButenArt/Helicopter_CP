#pragma once

#include "CAN_Interface.h"

class CAN_MOXA final : public ICAN_Board
{
	CNIO_DEV_INFO dev_node[4];
	HANDLE fd[2] = { INVALID_HANDLE_VALUE, INVALID_HANDLE_VALUE };         //
//	HANDLE fd1 = INVALID_HANDLE_VALUE;         //
private:
	CAN_MOXA( );
protected:
	~CAN_MOXA( );
public:
	friend ICAN_Board* ICAN_Board::CreateInstance( QString name );
	void Delete( ) override { delete this; };
	int ReadBuffer( int port ); // есть ли данные на прием
	int SendPacket( DWORD addr, BYTE FrameType, BYTE Length, BYTE frame[ ], int port ) override; // отсылка пакета
	int ReadPacket( DWORD& addr, BYTE& FrameType, BYTE& Length, BYTE frame[ ], int port ) override; // прием пакета
};
