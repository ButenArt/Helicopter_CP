#include "PCAN.h"
#include <utility>

CAN_PCAN::CAN_PCAN( )
{
	qDebug( ) << "Ports init:"
		<< "PcanHandle[0]" << PcanHandle[0]
		<< "PcanHandle[1]" << PcanHandle[1];

	int count = 2;
	auto stsResult = CAN_Initialize( PcanHandle[0], Bitrate );
	if ( stsResult != PCAN_ERROR_OK )
	{
		qDebug() << "USB1: Can not initialize. Please check the defines in the code.";
		qDebug() << "Error code: " << hex << stsResult;
		count--;
		//return;
	}
	stsResult = CAN_Initialize( PcanHandle[1], Bitrate );
	if ( stsResult != PCAN_ERROR_OK )
	{
		qDebug() << "USB2: Can not initialize. Please check the defines in the code.";
		qDebug() << "Error code: " << hex << stsResult;
		count--;
		//return;
	}
	// ���� ���������� 2 ����������, �� ���� ��������, ��� �� ��� ��� �� ������.
	// ��� ������������� ���� ���������, ����� ���� ���������� ������ �����������
	// ��� ������� ���������� ������� � ������ ���� ��� ���� 0 ������ ����� ����� 1
	// 0 ������������ �� ���������, ��� ��� ��� ����� �� ��������� � ���� ���������
	if ( count == 2 )  
	{
		DWORD deviceId_0;
		DWORD deviceId_1;
		if ( CAN_GetValue( PcanHandle[0], PCAN_DEVICE_ID, &deviceId_0, 4 ) == PCAN_ERROR_OK )
		{
			qDebug( ) << "PcanHandle[0] =" << PcanHandle[0] << "deviceId =" << deviceId_0;
			if ( CAN_GetValue( PcanHandle[1], PCAN_DEVICE_ID, &deviceId_1, 4 ) == PCAN_ERROR_OK )
			{
				qDebug( ) << "PcanHandle[1] =" << PcanHandle[1] << "deviceId =" << deviceId_1;
			}
			// ���� ������ �� ������, �� ������ ������� ID ������������, �� �������� ������ � ����� ������������
			if ( deviceId_0 != 1 )
			{
				std::swap( PcanHandle[0], PcanHandle[1] );
				qDebug( ) << "Change ports:" 
					<< "PcanHandle[0]" << PcanHandle[0]
					<< "PcanHandle[1]" << PcanHandle[1];
			}
		}
	}
	qDebug( ) << "Ports:"
		<< "PcanHandle[0]" << PcanHandle[0]
		<< "PcanHandle[1]" << PcanHandle[1];
}

CAN_PCAN::~CAN_PCAN( )
{
	CAN_Uninitialize( PCAN_NONEBUS );
}

int CAN_PCAN::ReadBuffer( int port )
{
	TPCANMsg CANMsg;
	TPCANStatus stsResult;
	while ( stsResult = CAN_Read( PcanHandle[port], &CANMsg, nullptr ) == PCAN_ERROR_OK )
	{
		message[port].push( CANMsg );
	}
	return message[port].size( );
}

int CAN_PCAN::SendPacket( DWORD addr, BYTE /*FrameType*/, BYTE Length, BYTE frame[ ], int port )
{
	TPCANMsg msgCanMessage;
	msgCanMessage.ID = addr;
	msgCanMessage.LEN = Length;
	msgCanMessage.MSGTYPE = PCAN_MESSAGE_EXTENDED; // ������ ���� ��� FrameType � PCAN � MOXA
	for ( size_t i = 0; i < Length; ++i )
	{
		msgCanMessage.DATA[i] = frame[i];
	}
	auto stsResult = CAN_Write( PcanHandle[port], &msgCanMessage );
	
	return stsResult == PCAN_ERROR_OK ? CNIO_SUCCESS : 0;
}

// ������ ��� ������� �������� ReadBuffer � �������� � �������
int CAN_PCAN::ReadPacket( DWORD& addr, BYTE& FrameType, BYTE& Length, BYTE frame[ ], int port )
{
	if ( message[port].empty( ) ) 
		return 0;

	auto m = message[port].front( );
	message[port].pop( );
	addr = m.ID;
	FrameType = CNIO_EXTENDED_FRAME;
	Length = m.LEN;
	for ( size_t i = 0; i < Length; ++i )
	{
		frame[i] = m.DATA[i];
	}
	return CNIO_SUCCESS;
}

