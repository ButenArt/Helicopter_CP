#pragma once

#include "CAN_Interface.h"

class CAN_CAN4 final : public ICAN_Board
{
private:
	CAN_CAN4( ){};
protected:
	~CAN_CAN4( ){};
public:
	friend ICAN_Board* ICAN_Board::CreateInstance( QString name );
	void Delete( ) override { delete this; };
	int ReadBuffer( int port ); // есть ли данные на прием
	int SendPacket( DWORD addr, BYTE FrameType, BYTE Length, BYTE frame[ ], int port ) override; // отсылка пакета
	int ReadPacket( DWORD& addr, BYTE& FrameType, BYTE& Length, BYTE frame[ ], int port ) override; // прием пакета
};

