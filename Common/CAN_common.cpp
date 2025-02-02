#include "CAN_common.h"
#include "QDebug"
#include "log_duration.h"
#include <cassert>
#include "Printer.h"
//#include "CAN_Creator.h"

CANBusComm::CANBusComm( QString sboard ) : QObject(nullptr)
{
	_board = ICAN_Board::CreateInstance( sboard );
	PRINT_CHANGED_VAR( sboard );

	_TXEnabled = false;        // флаг разрешения передачи пакетов
	_RXEnabled = false;        // флаг разрешения приема пакетов
	_canReady = false;
	_stopWorking = false;
	//_onceTXPackets = false;
	_GPIOOUT_Ready = false;

	_TXpacktsCNT = 0;
	_RXpacktsCNT = 0;
	_TXpacktsCNT1s = 0;
	_RXpacktsCNT1s = 0;
	_TXpacktsPPS = 0;
	_RXpacktsPPS = 0;

	TimerTX = new QTimer( this );
	TimerTX->setTimerType( Qt::PreciseTimer );
	TimerTX->stop( );

	Timer1sec = new QTimer( this );
	Timer1sec->setInterval( 1000 );
	connect( Timer1sec, &QTimer::timeout, [=] ( ) {
			_TXpacktsPPS = _TXpacktsCNT1s;
			_RXpacktsPPS = _RXpacktsCNT1s;
			_TXpacktsCNT1s = 0;
			_RXpacktsCNT1s = 0;
		} );
	Timer1sec->start( );
}

CANBusComm::~CANBusComm( )
{
	Stop( );
	_board->Delete( );
	delete TimerTX;
	delete Timer1sec;
}

void CANBusComm::Start( )
{
	Timer1sec->start( );
}

void CANBusComm::Stop( )
{
	TimerTX->stop( );
	Timer1sec->stop( );
}

LONG CANBusComm::SendPacket( PCNIO_MSG txframe, BYTE port )
{
	auto ret = _board->SendPacket( txframe->ID, txframe->FrameType, txframe->Length, txframe->Data, port );;
	if ( ret == CNIO_SUCCESS )
	{
		_TXpacktsCNT++;
		_TXpacktsCNT1s++;
	}
	return ret;
}

LONG CANBusComm::ReadPacket( PCNIO_MSG rxframe, BYTE port )
{
	auto ret = _board->ReadPacket( rxframe->ID, rxframe->FrameType, rxframe->Length, rxframe->Data, port );
	if ( ret == CNIO_SUCCESS )
	{
		_RXpacktsCNT1s++;
		_RXpacktsCNT++;
	}
	return ret;
}

LONG CANBusComm::ReadBuffer( BYTE port )
{
	return _board->ReadBuffer( port );
}

// Higher byte
BYTE CANBusComm::Hi( WORD value ) const
{
	BYTE val = value >> 8;
	return val;
}

// Lower byte
BYTE CANBusComm::Lo( WORD value ) const
{
	BYTE val = value & 0xFF;
	return val;
}

WORD CANBusComm::MAB_VALUE( BYTE byte_Hi, BYTE byte_Lo, WORD max_val ) const
{
	WORD ret = ( ( byte_Hi << 8 ) | byte_Lo & 0x00FF );
	return max_val > 1023 ? ret : ret >> 2;
}

WORD CANBusComm::MAB_VALUE( BYTE byte_Hi, BYTE byte_Lo) const
{
	return ( ( byte_Hi << 8 ) | byte_Lo & 0x00FF );
}

WORD CANBusComm::MAKE_WORD( BYTE byte_Hi, BYTE byte_Lo ) const
{
	return ( ( byte_Hi << 8 ) | byte_Lo & 0x00FF );
}

USHORT CANBusComm::ENUM_VALUE( BYTE byte_Hi, BYTE byte_Lo ) const
{
	return (ushort)( ( byte_Hi << 8 ) | byte_Lo & 0xFF );
}

void CANBusComm::SOD_Clear_IN_Data( )
{
	//LOG_DURATION( __FUNCTION__ );
	for ( auto& MAB_UNIT : SOD.MAB_UNIT )
	{
		MAB_UNIT.ADDR = 0x00;
		for ( int j = 0; j <= 3; j++ )
		{
			MAB_UNIT.MAB[j] = 0x00;
		}
	}

	for ( auto& IN_UNIT : SOD.IN_UNIT )
	{
		IN_UNIT.ADDR = 0x00;
		for ( int j = 0; j <= 7; j++ )
		{
			IN_UNIT.GPIO_IN[j] = 0x00;
		}
	}

	for ( auto& POT_UNIT : SOD.POT_UNIT )
	{
		POT_UNIT.ADDR = 0x00;
		for ( int j = 0; j <= 3; j++ )
		{
			POT_UNIT.POT[j] = 0x00;
		}
	}
}

void CANBusComm::SOD_Clear_OUT_Data( )
{
	for ( auto& VID_UNIT : SOD.VID_UNIT )
	{
		VID_UNIT.ADDR = 0x00;
		for ( int j = 0; j <= 3; j++ ) // TODO: уточнить количество блоков, заменить на константы
		{
			VID_UNIT.VID[j] = 0x00;
		}
	}

	for ( auto& OUT_UNIT : SOD.OUT_UNIT )
	{
		OUT_UNIT.ADDR = 0x00;
		for ( int j = 0; j <= 7; j++ )
		{
			OUT_UNIT.GPIO_OUT[j] = 0x00;
		}
	}

	for ( auto& LCD_UNIT : SOD.LCD_UNIT )
	{
		LCD_UNIT.ADDR = 0x00;
		for ( int j = 0; j <= 7; j++ )
		{
			LCD_UNIT.CONTENT[j] = 0x00;
		}
	}
}

void CANBusComm::SOD_Clear_TMP_OUT_Data( )
{
	//LOG_DURATION( "SOD_Clear_TMP_OUT_Data" );
	for ( auto& TMP_OUT : SOD.TMP_OUT_UNIT )
	{
		TMP_OUT.ADDR = 0x00;
		TMP_OUT.data_x8[0] = 0;
		assert( TMP_OUT.GPIO_OUT[0] == 0 );
	}
}

TUnitData::TUnitData( size_t i, int port, int enable )
{
	index = i;
	PORT = port;
	ENABLE = enable;
	TX = 0;
	RX = 0;
	RX_PREV = 0;
	FAULT = false;
	HEARTBEAT = 0;
	INFO = "";
}

TMABData::TMABData( size_t i, int addr, int port, int enable ) : TUnitData( i, port, enable )
{
	ADDR_TX = addr + 0x3;  // 0x3[i]3
	ADDR_RX = addr + 0xC;  // 0x3[i]C
	auto number = QString::number( i );
	DESC = "MAB" + number + " - Addr TX / RX: [0x" + QString::number( ADDR_TX, 16 ).toUpper( ) + "] / [0x" + QString::number( ADDR_RX, 16 ).toUpper( ) + "] Port : [" + QString::number( PORT ) + "] Enabled : [" + QString::number( ENABLE ) + "] ";

	sFAULT = "MAB" + number + ": [0x" + QString::number( ADDR_TX, 16 ).toUpper( ) + "] Port: [" + QString::number( PORT ) + "]\r\n";
}

TVIDData::TVIDData( size_t i, int addr, int port, int enable ) : TUnitData( i, port, enable )
{
	ADDR_TX = addr + 0x4;
	ADDR_RX = addr + 0xF;
	auto number = QString::number( i );
	DESC = "VID" + number + " - Addr TX/RX: [0x" + QString::number( ADDR_TX, 16 ).toUpper( ) + "]/[0x" + QString::number( ADDR_RX, 16 ).toUpper( ) + "] Port : [" + QString::number( PORT ) + "] Enabled : [" + QString::number( ENABLE ) + "] ";

	sFAULT = "VID" + number + ": [0x" + QString::number( ADDR_TX, 16 ).toUpper( ) + "] Port: [" + QString::number( PORT ) + "]\r\n";
}

TGPIOINData::TGPIOINData( size_t i, int addr, int port, int enable ) : TUnitData( i, port, enable )
{
	ADDR_TX = addr + 0;
	ADDR_RX = addr + 0x9;
	auto number = QString::number( i );
	DESC = "GPIOIN" + number + " - Addr TX/RX: [0x" + QString::number( ADDR_TX, 16 ).toUpper( ) + "]/[0x" + QString::number( ADDR_RX, 16 ).toUpper( ) + "] Port : [" + QString::number( PORT ) + "] Enabled : [" + QString::number( ENABLE ) + "] ";

	sFAULT = "GPIOIN" + number + ": [0x" + QString::number( ADDR_TX, 16 ).toUpper( ) + "] Port: [" + QString::number( PORT ) + "] \r\n";
}

TGPIOOUTData::TGPIOOUTData( size_t i, int addr, int port, int enable ) : TUnitData( i, port, enable )
{
	ADDR_TX = addr + 0x1;
	ADDR_RX = addr + 0xF;
	auto number = QString::number( i );
	DESC = "GPIOOUT" + number + " - Addr TX/RX: [0x" + QString::number( ADDR_TX, 16 ).toUpper( ) + "]/[0x" + QString::number( ADDR_RX, 16 ).toUpper( ) + "] Port: [" + QString::number( PORT ) + "] Enabled: [" + QString::number( ENABLE ) + "]";
	sFAULT = "GPIOOUT" + number + ": [0x" + QString::number( ADDR_TX, 16 ).toUpper( ) + "] Port: [" + QString::number( PORT ) + "] \r\n";
}

TPOTData::TPOTData( size_t i, int addr, int port, int enable ) : TUnitData( i, port, enable )
{
	ADDR_TX = addr + 0x2;
	ADDR_RX = addr + 0xA;
	auto number = QString::number( i );
	DESC = "POTENTIOMETER" + number + " - Addr TX/RX: [0x" + QString::number( ADDR_TX, 16 ).toUpper( ) + "]/[0x" + QString::number( ADDR_RX, 16 ).toUpper( ) + "] Port: [" + QString::number( PORT ) + "] Enabled: [" + QString::number( ENABLE ) + "]";
	sFAULT = "POTENTIOMETER" + number + ": [0x" + QString::number( ADDR_TX, 16 ).toUpper( ) + "] Port: [" + QString::number( PORT ) + "] \r\n";
}

TENCData::TENCData( size_t i, int addr, int port, int enable ) : TUnitData( i, port, enable )
{
	ADDR_TX = addr + 0x0;
	ADDR_RX = addr + 0x9;
	auto number = QString::number( i );
	DESC = "ENCODER" + number + " - Addr TX/RX: [0x" + QString::number( ADDR_TX, 16 ).toUpper( ) + "]/[0x" + QString::number( ADDR_RX, 16 ).toUpper( ) + "] Port: [" + QString::number( PORT ) + "] Enabled: [" + QString::number( ENABLE ) + "]";
	sFAULT = "ENCODER" + number + ": [0x" + QString::number( ADDR_TX, 16 ).toUpper( ) + "] Port: [" + QString::number( PORT ) + "] \r\n";
}
TLEDData::TLEDData( size_t i, int addr, int port, int enable ) : TUnitData( i, port, enable )
{
	ADDR_TX = addr + 0;
	ADDR_RX = addr + 0x9;
	auto number = QString::number( i );
	DESC = "LED" + number + " - Addr TX/RX: [0x" + QString::number( ADDR_TX, 16 ).toUpper( ) + "]/[0x" + QString::number( ADDR_RX, 16 ).toUpper( ) + "] Port : [" + QString::number( PORT ) + "] Enabled : [" + QString::number( ENABLE ) + "] ";

	sFAULT = "LED" + number + ": [0x" + QString::number( ADDR_TX, 16 ).toUpper( ) + "] Port: [" + QString::number( PORT ) + "] \r\n";
}

TLCDData::TLCDData( size_t i, int addr, int port, int enable ) : TUnitData( i, port, enable )
{
	ADDR_TX = addr + 0;
	ADDR_RX = addr + 0x9;  // TODO узнать адрес LCD модуля
	auto number = QString::number( i );
	DESC = "LCD" + number + " - Addr TX/RX: [0x" + QString::number( ADDR_TX, 16 ).toUpper( ) + "]/[0x" + QString::number( ADDR_RX, 16 ).toUpper( ) + "] Port : [" + QString::number( PORT ) + "] Enabled : [" + QString::number( ENABLE ) + "] ";

	sFAULT = "LCD" + number + ": [0x" + QString::number( ADDR_TX, 16 ).toUpper( ) + "] Port: [" + QString::number( PORT ) + "] \r\n";
}
