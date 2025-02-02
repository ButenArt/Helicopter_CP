#include "CAN_Interface.h"
//#include <CanApi4.h>
#include "PCAN.h"
#include "MOXA.h"
#include "CAN4.h"

ICAN_Board* ICAN_Board::CreateInstance( QString name )
{
    qDebug( ) << __FUNCTION__ << name;
	name = name.toUpper( );
	if ( name.indexOf( "MOXA" ) != -1 ) return static_cast<ICAN_Board*>( new CAN_MOXA( ) );
	if ( name.indexOf( "PCAN" ) != -1 ) return static_cast<ICAN_Board*>( new CAN_PCAN( ) );
	if ( name.indexOf( "CAN4" ) != -1 ) return static_cast<ICAN_Board*>( new CAN_CAN4( ) );
    return static_cast<ICAN_Board*>( new CAN_VIRT( ) );
}

int CAN_VIRT::ReadBuffer( int /*port*/ )
{
	return message.size();
}

int CAN_VIRT::SendPacket( DWORD addr, BYTE /*FrameType*/, BYTE /*Length*/, BYTE* /*frame*/, int port )
{
	//qDebug( ) << __FUNCTION__;
	if ( addr >= 0x300 && addr < 0x400 ){
		addr = addr + 0x09;
	}
	else if ( addr >= 0x500 && addr < 0x700 ){
		addr = addr + 0x09;
	}
	else if ( addr >= 0x700 && addr < 0x800 ){
		addr = addr + 0x0E;
	}
	else { // все VID
		addr = addr + 0x0B;
	}
	message.push( { addr, port } );
	return CNIO_SUCCESS;
};

int CAN_VIRT::ReadPacket( DWORD& addr, BYTE& FrameType, BYTE& Length, BYTE frame[ ], int port )
{
	//qDebug( ) << __FUNCTION__;
	if ( message.empty( ) ) return 0;
	auto m = message.front() ;
	message.pop( );
	addr = m.first;
	port = m.second;
	FrameType = CNIO_EXTENDED_FRAME;
	Length = 8;
	frame[0] = -1;
	frame[1] = -1;
	frame[2] = -1;
	frame[3] = -1;
	frame[4] = -1;
	frame[5] = -1;
	frame[6] = -1;
	frame[7] = -1;
	return CNIO_SUCCESS;
};


