#pragma once

#include "CAN_Interface.h"
//#include "CAN_Creator.h"
#include "Common/PCAN/PCANBasic.h"

class CAN_PCAN final : public ICAN_Board
{
	TPCANHandle PcanHandle[ 2 ] = { PCAN_USBBUS1, PCAN_USBBUS2 };
	const TPCANBaudrate Bitrate = PCAN_BAUD_500K;
	//TPCANMsg CANMsg[2] = {0};
	std::queue<TPCANMsg>  message[ 2 ];
private:
	CAN_PCAN( );
protected:
	~CAN_PCAN( );
public:
	friend ICAN_Board* ICAN_Board::CreateInstance( QString name );
	void Delete( ) override { delete this; };
	int ReadBuffer( int port ); // есть ли данные на прием
	int SendPacket( DWORD addr, BYTE FrameType, BYTE Length, BYTE frame[ ], int port ) override; // отсылка пакета
	int ReadPacket( DWORD& addr, BYTE& FrameType, BYTE& Length, BYTE frame[ ], int port ) override; // прием пакета
};

