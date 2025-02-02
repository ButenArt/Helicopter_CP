#include "MOXA.h"

//  подключаемся сразу ко обоим портам
CAN_MOXA::CAN_MOXA( )
{
	qDebug( ) << __FUNCTION__;
	dev_node[0] = { 0 };
	dev_node[1] = { 0 };
	dev_node[2] = { 0 };
	dev_node[3] = { 0 };
	int brd_cnt = 0;
	brd_cnt = cnio_enum_devices( dev_node, 4 );
	if ( brd_cnt > 0 )
	{
		fd[0] = cnio_open( dev_node[0].CNIOID, CNIO_PORT1_INDEX );
		cnio_init( fd[0], CNIO_OPMODE_SINGLE_ACC, B_500K );
		if ( cnio_start( fd[0] ) != CNIO_SUCCESS )
			qDebug( ) << "Start CAN Port0 Fail";

		fd[1] = cnio_open( dev_node[0].CNIOID, CNIO_PORT2_INDEX );
		cnio_init( fd[1], CNIO_OPMODE_SINGLE_ACC, B_500K );
		if ( cnio_start( fd[1] ) != CNIO_SUCCESS )
			qDebug( ) << "Start CAN Port0 Fail";
	}
}

CAN_MOXA::~CAN_MOXA( )
{
	cnio_close( fd[0] );
	cnio_close( fd[1] );
}

int CAN_MOXA::ReadBuffer( int port )
{
	//qDebug( ) << __FUNCTION__;
	int ret = 0;
	assert( port == 0 || port == 1 );
	if ( fd[port] != INVALID_HANDLE_VALUE )
	{
		ret = cnio_receive_queue( fd[port] );
		//qDebug( ) << "cnio_receive_queue( fd[" << port << "] ); incoming " << ret << "packet";
	}
	return ret;
}

int CAN_MOXA::SendPacket( DWORD addr, BYTE FrameType, BYTE Length, BYTE frame[ ], int port )
{
	CNIO_MSG tx_frame = { 0 };
	int ret = 0;
	assert( port == 0 || port == 1 );

	if ( fd[port] != INVALID_HANDLE_VALUE )
	{
		tx_frame.ID = addr;
		tx_frame.FrameType = FrameType;
		tx_frame.Length = Length;
		for ( size_t i = 0; i < Length; ++i )
		{
			tx_frame.Data[i] = frame[i];
		}
		ret = cnio_send_message( fd[port], &tx_frame, 10 );
		//if ( ret != CNIO_SUCCESS ) {
		//	qDebug( ).nospace( ) << "Tx CAN Port " << port << " Fail[0x" << hex << addr << "] Error: 0x" << ret;
		//}
	}
	//qDebug( ).nospace( ) << "Tx CAN Port" << port << " Ok[0x" << hex << addr << "]  " << ret;
	return ret;
}

// проверяет наличие данных в очереди, и при их наличии читает без блокировки, возвращает 1 при наличии данных, и 0 при отсутствии
int CAN_MOXA::ReadPacket( DWORD& addr, BYTE& FrameType, BYTE& Length, BYTE frame[ ], int port )
{
	CNIO_MSG rx_frame = { 0 };
	int ret = 0;
	assert( port == 0 || port == 1 );
	if ( fd[port] != INVALID_HANDLE_VALUE )
	{
		//if ( ret = cnio_receive_queue( fd[port] ) == CNIO_SUCCESS )
		{
			ret = cnio_receive_message( fd[port], &rx_frame, 10 );
			addr = rx_frame.ID;
			FrameType = rx_frame.FrameType;
			Length = rx_frame.Length;
			for ( size_t i = 0; i < Length; ++i )
			{
				frame[i] = rx_frame.Data[i];
			}
			//qDebug( ).nospace( ) << "Rx CAN Port" << port << " Ok[0x" << hex << addr << "]" << ret;
		}
	}
	else
	{
		qDebug( ) << "Port:" << port << "not open";
	}
	//qDebug( ) << __FUNCTION__ << hex << addr << ":" << dec << port << ret;
	return ret;
}
