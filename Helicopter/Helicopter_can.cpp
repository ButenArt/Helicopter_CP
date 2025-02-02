#include "Helicopter_can.h"
#include <QDebug>
#include "Common/log_duration.h"
#include "Common/Printer.h"

int64_t cycle_time()
{
	static std::chrono::steady_clock::time_point prev_time;

	const auto start_time = std::chrono::steady_clock::now();
	const auto cycle = start_time - prev_time;
	prev_time = start_time;
	int64_t cycle_time = std::chrono::duration_cast<std::chrono::milliseconds>(cycle).count();
	return cycle_time;
}


// таймер на чтение не используется, вместо него после записи создается QTimer::singleShot
// с минимальным временем, который и вызывает однократное чтение из порта, прямой вызо чтени или emit приводят к чтению 0 байт
// так как данные еще не готовы, а таймер обеспечивает небольшую задержку
Helicopter_CAN::Helicopter_CAN( QSharedPointer<TSettingsFile> ini, QSharedPointer<TLogging> log, QObject* parent ) : CANBusComm( ini->Board( ) )
{
	_ini = ini;
	_log = log;
	_canReady = false;
	_GPIOOUT_Ready = false;
	_TxInterval = _ini->CanTxInterval();
	_RxInterval = _ini->CanRxInterval();
	_canHeartBeat = false;

	UNITS.MAB.reserve( N_MAB_UNITS );
	for ( size_t i = 0; i < N_MAB_UNITS; ++i )
	{
		UNITS.MAB.emplace_back( i, _ini->MABUnitAddr( i ), _ini->MABUnitPort( i ), _ini->MABUnitEnable( i ) );
	}
	UNITS.VID.reserve( N_VID_UNITS );
	for ( size_t i = 0; i < N_VID_UNITS; ++i )
	{
		UNITS.VID.emplace_back( i, _ini->VIDUnitAddr( i ), _ini->VIDUnitPort( i ), _ini->VIDUnitEnable( i ) );
	}
	UNITS.GPIN.reserve( N_IN_UNITS );
	for ( size_t i = 0; i < N_IN_UNITS; ++i )
	{
		UNITS.GPIN.emplace_back( i, _ini->GPIOINUnitAddr( i ), _ini->GPIOINUnitPort( i ), _ini->GPIOINUnitEnable( i ) );
	}
	UNITS.GPOUT.reserve( N_OUT_UNITS );
	for ( size_t i = 0; i < N_OUT_UNITS; ++i )
	{
		UNITS.GPOUT.emplace_back( i, _ini->GPIOOUTUnitAddr( i ), _ini->GPIOOUTUnitPort( i ), _ini->GPIOOUTUnitEnable( i ) );
	}
	UNITS.POT.reserve( N_POT_UNITS );
	for ( size_t i = 0; i < N_POT_UNITS; ++i )
	{
		UNITS.POT.emplace_back( i, _ini->POTUnitAddr( i ), _ini->POTUnitPort( i ), _ini->POTUnitEnable( i ) );
	}
	UNITS.ENC.reserve( N_ENC_UNITS );
	for ( size_t i = 0; i < N_ENC_UNITS; ++i )
	{
		UNITS.ENC.emplace_back( i, _ini->ENCUnitAddr( i ), _ini->ENCUnitPort( i ), _ini->ENCUnitEnable( i ) );
	}
	for ( size_t i = 0; i < N_IN_UNITS; ++i )
	{
		UNITS.LED.emplace_back( i, _ini->GPIOINUnitAddr( i ), _ini->GPIOINUnitPort( i ), _ini->GPIOINUnitEnable( i ) );
	}
	for ( size_t i = 0; i < N_IN_UNITS; ++i )
	{
		UNITS.LCD.emplace_back( i, _ini->MABUnitAddr( i ), _ini->MABUnitPort( i ), _ini->MABUnitEnable( i ) );
	}

	SOD.MAB_UNIT.resize( max( N_LCD_UNITS, UNITS.MAB.size( ) ) );
	SOD.VID_UNIT.resize( UNITS.VID.size() );
	SOD.IN_UNIT.resize( max( N_LED_UNITS, UNITS.GPIN.size( ) ) );
	SOD.OUT_UNIT.resize( UNITS.GPOUT.size() );
	SOD.TMP_OUT_UNIT.resize( UNITS.GPOUT.size() );
	SOD.POT_UNIT.resize( UNITS.POT.size() );
	SOD.ENC_UNIT.resize( UNITS.ENC.size() );
	SOD.LED_UNIT.resize( max( N_LED_UNITS, UNITS.GPIN.size( ) ) ); // LED наложен на IN
	SOD.LCD_UNIT.resize( max( N_LCD_UNITS, UNITS.MAB.size() ) );   // LCD наложен на MAB

	SOD_Clear_IN_Data();
	SOD_Clear_OUT_Data();
	SOD_Clear_TMP_OUT_Data();
	
	TimerTX->setInterval( _TxInterval );
	connect( TimerTX, SIGNAL( timeout() ), this, SLOT( slotTX() ) );

	// схема вызова обработчиков чтения и записи сода
	// Tx0 > slotTXOther -> slotRX -> slotTXVID1 -> slotRX -> slotTXVID2 -> slotRX -> slotTXVID2 -> slotRX

	setTXEnabled( false );
	setRXEnabled( false );

	_canReady = true;
}

Helicopter_CAN::~Helicopter_CAN()
{
	qDebug() << __FUNCTION__;
	_canReady = false;
	TimerTX->stop();
}

void Helicopter_CAN::slotTX()
{
//    qDebug() << __FUNCTION__;
	LOG_DURATION( "TX" );
	CNIO_MSG tx_frame = {0};
	bool sending = false;
	if ( TXEnabled( ) )
	{
		if ( _GPIOOUT_Ready ) // вообщето тут даже не надо копировать их, можно было бы переключать структуры через указатель
		{
			for ( size_t i = 0; i < UNITS.GPOUT.size( ); ++i )
			{
				SOD.OUT_UNIT[i].GPIO_OUT[0] = SOD.TMP_OUT_UNIT[i].GPIO_OUT[0];
				SOD.OUT_UNIT[i].GPIO_OUT[1] = SOD.TMP_OUT_UNIT[i].GPIO_OUT[1];
				SOD.OUT_UNIT[i].GPIO_OUT[2] = SOD.TMP_OUT_UNIT[i].GPIO_OUT[2];
				SOD.OUT_UNIT[i].GPIO_OUT[3] = SOD.TMP_OUT_UNIT[i].GPIO_OUT[3];
				SOD.OUT_UNIT[i].GPIO_OUT[4] = SOD.TMP_OUT_UNIT[i].GPIO_OUT[4];
				SOD.OUT_UNIT[i].GPIO_OUT[5] = SOD.TMP_OUT_UNIT[i].GPIO_OUT[5];
				SOD.OUT_UNIT[i].GPIO_OUT[6] = SOD.TMP_OUT_UNIT[i].GPIO_OUT[6];
				SOD.OUT_UNIT[i].GPIO_OUT[7] = SOD.TMP_OUT_UNIT[i].GPIO_OUT[7];
			}
		}

		// проходим по всем MAB-ам
		{
			//LOG_DURATION( "MAB" );
			for ( size_t i = 0; i < UNITS.MAB.size( ); ++i )
			{
				if ( UNITS.MAB[i].ENABLE )
				{
					//LOG_DURATION( "MAB"+QString::number( i ) );
					SOD.MAB_UNIT[i].ADDR = UNITS.MAB[i].ADDR_TX;
					SOD.LCD_UNIT[i].ADDR = UNITS.MAB[i].ADDR_TX;

					tx_frame.ID = UNITS.MAB[i].ADDR_TX;
					tx_frame.FrameType = CNIO_EXTENDED_FRAME;
					tx_frame.Length = 8;
					//tx_frame.DATA = SOD.LCD_UNIT[i].DATA;
					tx_frame.Data[0] = SOD.LCD_UNIT[i].CONTENT[0];
					tx_frame.Data[1] = SOD.LCD_UNIT[i].CONTENT[1];
					tx_frame.Data[2] = SOD.LCD_UNIT[i].CONTENT[2];
					tx_frame.Data[3] = SOD.LCD_UNIT[i].CONTENT[3];
					tx_frame.Data[4] = SOD.LCD_UNIT[i].CONTENT[4];
					tx_frame.Data[5] = SOD.LCD_UNIT[i].CONTENT[5];
					tx_frame.Data[6] = SOD.LCD_UNIT[i].CONTENT[6];
					tx_frame.Data[7] = SOD.LCD_UNIT[i].CONTENT[7];
					if ( SendPacket( &tx_frame, UNITS.MAB[i].PORT ) == CNIO_SUCCESS )
					{
						UNITS.MAB[i].TX++;
					}
					//qDebug() << "Sending:" << hex << tx_frame.ID;
					sending = true;
				}
			}
		}
		// шаговые двигатели
		{
			//LOG_DURATION( "VID" );
			for ( size_t i = 0; i < UNITS.VID.size( ); ++i )
			{
				if ( UNITS.VID[i].ENABLE )
				{
					//LOG_DURATION( "VID"+QString::number( i ) );
					SOD.VID_UNIT[i].ADDR = UNITS.VID[i].ADDR_TX;
					tx_frame.ID = UNITS.VID[i].ADDR_TX;
					tx_frame.FrameType = CNIO_EXTENDED_FRAME;
					tx_frame.Length = 8;
					//tx_frame.DATA = SOD.VID_UNIT[i].DATA;
					tx_frame.Data[0] = Hi( SOD.VID_UNIT[i].VID[0] );
					tx_frame.Data[1] = Lo( SOD.VID_UNIT[i].VID[0] );
					tx_frame.Data[2] = Hi( SOD.VID_UNIT[i].VID[1] );
					tx_frame.Data[3] = Lo( SOD.VID_UNIT[i].VID[1] );
					tx_frame.Data[4] = Hi( SOD.VID_UNIT[i].VID[2] );
					tx_frame.Data[5] = Lo( SOD.VID_UNIT[i].VID[2] );
					tx_frame.Data[6] = Hi( SOD.VID_UNIT[i].VID[3] );
					tx_frame.Data[7] = Lo( SOD.VID_UNIT[i].VID[3] );
					{
						//LOG_DURATION( "SendPacket" );
						if ( SendPacket( &tx_frame, UNITS.VID[i].PORT ) == CNIO_SUCCESS )
						{
							UNITS.VID[i].TX++;
						}
					}
					//qDebug() << "Sending:" << hex << tx_frame.ID;
					sending = true;
					//QCoreApplication::processEvents( );
				}
			}
		}
		{
			//LOG_DURATION( "IN" );
			for ( size_t i = 0; i < UNITS.GPIN.size( ); ++i )
			{
				if ( UNITS.GPIN[i].ENABLE )
				{
					SOD.IN_UNIT[i].ADDR = UNITS.GPIN[i].ADDR_TX;

					tx_frame.ID = UNITS.GPIN[i].ADDR_TX;
					tx_frame.FrameType = CNIO_EXTENDED_FRAME;
					tx_frame.Length = 8;
					tx_frame.Data[0] = SOD.LED_UNIT[i].DIGIT[0]; //
					tx_frame.Data[1] = SOD.LED_UNIT[i].DIGIT[1]; //
					tx_frame.Data[2] = SOD.LED_UNIT[i].DIGIT[2]; //
					tx_frame.Data[3] = SOD.LED_UNIT[i].DIGIT[3]; //
					tx_frame.Data[4] = SOD.LED_UNIT[i].DIGIT[4]; //
					tx_frame.Data[5] = SOD.LED_UNIT[i].DIGIT[5]; //
					tx_frame.Data[6] = SOD.LED_UNIT[i].DIGIT[6]; //
					tx_frame.Data[7] = SOD.LED_UNIT[i].DIGIT[7]; //
					if ( SendPacket( &tx_frame, UNITS.GPIN[i].PORT ) == CNIO_SUCCESS )
					{
						UNITS.GPIN[i].TX++;
					}
					//qDebug() << "Sending:" << hex << tx_frame.ID;
					sending = true;
				}
			}
		}
		// UNITS.GPIOIN НЕ UNITS.ENC пересекаются по адресам 500 диапазона
		{
			//LOG_DURATION( "ENC" );
			for ( size_t i = 0; i < UNITS.ENC.size( ); ++i )
			{
				if ( UNITS.ENC[i].ENABLE )
				{
					if ( SOD.ENC_UNIT[i].CLR == true ) // отправляем пакет в энкодер только если он сброшен в 0 иначе ждем пока его прочитают в логике
					{
						SOD.ENC_UNIT[i].ADDR = UNITS.ENC[i].ADDR_TX;
						SOD.ENC_UNIT[i].CLR = false;

						tx_frame.ID = UNITS.ENC[i].ADDR_TX;
						tx_frame.FrameType = CNIO_EXTENDED_FRAME;
						tx_frame.Length = 1;
						tx_frame.Data[0] = 0xFF; //
						if ( SendPacket( &tx_frame, UNITS.ENC[i].PORT ) == CNIO_SUCCESS )
						{
							UNITS.ENC[i].TX++;
						}
						//qDebug( ) << "Sending:" << hex << tx_frame.ID;
						sending = true;
					}
				}
			}
		}
		{
			//LOG_DURATION( "OUT" );
			for ( size_t i = 0; i < UNITS.GPOUT.size( ); ++i )
			{
				if ( UNITS.GPOUT[i].ENABLE )
				{
					SOD.OUT_UNIT[i].ADDR = UNITS.GPOUT[i].ADDR_TX;
					tx_frame.ID = UNITS.GPOUT[i].ADDR_TX;
					tx_frame.FrameType = CNIO_EXTENDED_FRAME;
					tx_frame.Length = 8;
					tx_frame.Data[0] = SOD.OUT_UNIT[i].GPIO_OUT[0];
					tx_frame.Data[1] = SOD.OUT_UNIT[i].GPIO_OUT[1];
					tx_frame.Data[2] = SOD.OUT_UNIT[i].GPIO_OUT[2];
					tx_frame.Data[3] = SOD.OUT_UNIT[i].GPIO_OUT[3];
					tx_frame.Data[4] = SOD.OUT_UNIT[i].GPIO_OUT[4];
					tx_frame.Data[5] = SOD.OUT_UNIT[i].GPIO_OUT[5];
					tx_frame.Data[6] = SOD.OUT_UNIT[i].GPIO_OUT[6];
					tx_frame.Data[7] = SOD.OUT_UNIT[i].GPIO_OUT[7];
					if ( SendPacket( &tx_frame, UNITS.GPOUT[i].PORT ) == CNIO_SUCCESS )
					{
						UNITS.GPOUT[i].TX++;
					}
					//qDebug() << "Sending:" << hex << tx_frame.ID;
					sending = true;
				}
			}
		}
		//
		{
			//LOG_DURATION( "POT" );
			for ( size_t i = 0; i < UNITS.POT.size( ); ++i )
			{
				if ( UNITS.POT[i].ENABLE )
				{
					SOD.POT_UNIT[i].ADDR = UNITS.POT[i].ADDR_TX;
					tx_frame.ID = UNITS.POT[i].ADDR_TX;
					tx_frame.FrameType = CNIO_EXTENDED_FRAME;
					tx_frame.Length = 1;
					tx_frame.Data[0] = 0xFF; // 
					if ( SendPacket( &tx_frame, UNITS.POT[i].PORT ) == CNIO_SUCCESS )
					{
						UNITS.POT[i].TX++;
					}
					//qDebug() << "Sending:" << hex << tx_frame.ID;
					sending = true;
				}
			}
		}
	}

	if ( sending )
	{
		QTimer::singleShot( 0, this, SLOT( slotRX() ) );
	}
	//qDebug() << "cycle" << __FUNCTION__ << delta_time << "micro";
}

void Helicopter_CAN::ReadCAN( BYTE port )
{
	CNIO_MSG rx_frame = {0};
	//qDebug() << __FUNCTION__;
	if ( RXEnabled() )
	{
		while ( ReadBuffer( port ) > 0 )
		{
			if ( ReadPacket( &rx_frame, port ) == CNIO_SUCCESS )
			{
				// пробегаем по всем MAB и проверяем их ли это пакет
				//qDebug() << "Reading:" << hex << rx_frame.ID;
				for ( size_t i = 0; i < UNITS.MAB.size(); ++i )
				{
					UNITS.MAB[i].RX_PREV = UNITS.MAB[i].RX; // равное знаение счетчиков это ошибка
					if ( rx_frame.ID == UNITS.MAB[i].ADDR_RX && port == UNITS.MAB[i].PORT )
					{
						UNITS.MAB[i].RX++;
						//SOD.MAB_UNIT[i].DATA = rx_frame.DATA;
						SOD.MAB_UNIT[i].MAB[0] = MAB_VALUE( rx_frame.Data[0], rx_frame.Data[1], 4095 );
						SOD.MAB_UNIT[i].MAB[1] = MAB_VALUE( rx_frame.Data[2], rx_frame.Data[3], 4095 );
						SOD.MAB_UNIT[i].MAB[2] = MAB_VALUE( rx_frame.Data[4], rx_frame.Data[5], 4095 );
						SOD.MAB_UNIT[i].MAB[3] = MAB_VALUE( rx_frame.Data[6], rx_frame.Data[7], 4095 );
					}
				}

				// пробегаем по всем GPIOIN1 и проверяем их ли это пакет
				for ( size_t i = 0; i < UNITS.GPIN.size(); ++i )
				{
					UNITS.GPIN[i].RX_PREV = UNITS.GPIN[i].RX;
					if ( rx_frame.ID == UNITS.GPIN[i].ADDR_RX && port == UNITS.GPIN[i].PORT )
					{
						UNITS.GPIN[i].RX++;

						SOD.IN_UNIT[i].GPIO_IN[0] = rx_frame.Data[0];
						SOD.IN_UNIT[i].GPIO_IN[1] = rx_frame.Data[1];
						SOD.IN_UNIT[i].GPIO_IN[2] = rx_frame.Data[2];
						SOD.IN_UNIT[i].GPIO_IN[3] = rx_frame.Data[3];
						SOD.IN_UNIT[i].GPIO_IN[4] = rx_frame.Data[4];
						SOD.IN_UNIT[i].GPIO_IN[5] = rx_frame.Data[5];
						SOD.IN_UNIT[i].GPIO_IN[6] = rx_frame.Data[6];
						SOD.IN_UNIT[i].GPIO_IN[7] = rx_frame.Data[7];
					}
				}

				for ( size_t i = 0; i < UNITS.ENC.size( ); ++i )
				{
					UNITS.ENC[i].RX_PREV = UNITS.ENC[i].RX;
					if ( rx_frame.ID == UNITS.ENC[i].ADDR_RX )
					{
						UNITS.ENC[i].RX++;
						SOD.ENC_UNIT[i].CLR = false;
						SOD.ENC_UNIT[i].ENC = *(int64_t*)rx_frame.Data;
						//PRINT_CHANGED_VAR( SOD.ENC_UNIT[i].ENC );
					}
				}

				for ( size_t i = 0; i < UNITS.POT.size(); ++i )
				{
					UNITS.POT[i].RX_PREV = UNITS.POT[i].RX;
					if ( rx_frame.ID == UNITS.POT[i].ADDR_RX && port == UNITS.POT[i].PORT )
					{
						UNITS.POT[i].RX++;
						//SOD.POT_UNIT[i].DATA = rx_frame.DATA;
						SOD.POT_UNIT[i].POT[0] = MAKE_WORD( rx_frame.Data[0], rx_frame.Data[1] );
						SOD.POT_UNIT[i].POT[1] = MAKE_WORD( rx_frame.Data[2], rx_frame.Data[3] );
						SOD.POT_UNIT[i].POT[2] = MAKE_WORD( rx_frame.Data[4], rx_frame.Data[5] );
						SOD.POT_UNIT[i].POT[3] = MAKE_WORD( rx_frame.Data[6], rx_frame.Data[7] );
					}
				}

				for ( size_t i = 0; i < UNITS.GPOUT.size(); ++i )
				{
					UNITS.GPOUT[i].RX_PREV = UNITS.GPOUT[i].RX;
					if ( rx_frame.ID == UNITS.GPOUT[i].ADDR_RX )
					{
						UNITS.GPOUT[i].RX++;
					}
				}

				for ( size_t i = 0; i < UNITS.VID.size(); ++i )
				{
					UNITS.VID[i].RX_PREV = UNITS.VID[i].RX;
					if ( rx_frame.ID == UNITS.VID[i].ADDR_RX && port == UNITS.VID[i].PORT )
					{
						UNITS.VID[i].RX++;
					}
				}
			}
			slotHeartBeat();
		}
	}
}

void Helicopter_CAN::slotHeartBeat()
{
	if ( !_canReady ) return;
	if ( !_canHeartBeat ) return;

	const auto HeartBeatOverlookCount = 80;

	for ( auto& MAB : UNITS.MAB )
	{
		if ( MAB.ENABLE )
		{
			if ( MAB.RX != MAB.RX_PREV )
			{
				MAB.HEARTBEAT = HeartBeatOverlookCount;
				MAB.FAULT = false;
			} else
			{
				if ( --MAB.HEARTBEAT <= 0 )
				{
					MAB.FAULT = true;
					MAB.HEARTBEAT = 0;
				}
			}
		}
	}

	for ( auto& VID : UNITS.VID )
	{
		if ( VID.ENABLE )
		{
			if ( VID.RX != VID.RX_PREV )
			{
				VID.HEARTBEAT = HeartBeatOverlookCount;
				VID.FAULT = false;
			} else
			{
				if ( --VID.HEARTBEAT <= 0 )
				{
					VID.FAULT = true;
					VID.HEARTBEAT = 0;
				}
			}
		}
	}

	for ( auto& GPIN : UNITS.GPIN )
	{
		if ( GPIN.ENABLE )
		{
			if ( GPIN.RX != GPIN.RX_PREV )
			{
				GPIN.HEARTBEAT = HeartBeatOverlookCount;
				GPIN.FAULT = false;
			} else
			{
				if ( --GPIN.HEARTBEAT <= 0 )
				{
					GPIN.FAULT = true;
					GPIN.HEARTBEAT = 0;
				}
			}
		}
	}

	for ( auto& GPOUT : UNITS.GPOUT )
	{
		if ( GPOUT.ENABLE )
		{
			if ( GPOUT.RX != GPOUT.RX_PREV )
			{
				GPOUT.HEARTBEAT = HeartBeatOverlookCount;
				GPOUT.FAULT = false;
			} else
			{
				if ( --GPOUT.HEARTBEAT <= 0 )
				{
					GPOUT.FAULT = true;
					GPOUT.HEARTBEAT = 0;
				}
			}
		}
	}
	for ( auto& POT : UNITS.POT )
	{
		if ( POT.ENABLE )
		{
			if ( POT.RX != POT.RX_PREV )
			{
				POT.HEARTBEAT = HeartBeatOverlookCount;
				POT.FAULT = false;
			} else
			{
				if ( --POT.HEARTBEAT <= 0 )
				{
					POT.FAULT = true;
					POT.HEARTBEAT = 0;
				}
			}
		}
	}
}

void Helicopter_CAN::HeartBeatEnable()
{
	_canHeartBeat = true;
}

void Helicopter_CAN::HeartBeatDisable()
{
	_canHeartBeat = false;
}

void Helicopter_CAN::slotRX()
{
	LOG_DURATION( "RX" );
	ReadCAN( 0 );
	ReadCAN( 1 );
}

void Helicopter_CAN::Start()
{
	TimerTX->start();
	this->setTXEnabled( true );
	this->setRXEnabled( true );
	this->HeartBeatEnable();
}

void Helicopter_CAN::Stop()
{
	TimerTX->stop();
	this->setTXEnabled( false );
	this->setRXEnabled( false );
	this->HeartBeatDisable();
}

void Helicopter_CAN::Reset( bool &reset)
{
	reset = true;		// устанавливаем вывод ресета в тру
	QTimer::singleShot( 100, this, [&]( ) 
		{ 
			reset = false; 
			setRXEnabled( false );
			QTimer::singleShot( 0, this, [&]( ) { setRXEnabled( false );setTXEnabled( false ); } ); // на срабатывание таймера разрешаем чтение из СОД
		} ); // на срабатывание таймера разрешаем чтение из СОД
	QTimer::singleShot( 5000, this, [&]( ) { setRXEnabled( true );setTXEnabled( true ); } ); // на срабатывание таймера разрешаем чтение из СОД
}

//void Helicopter_CAN::slotTXVID1( )
//{
//	//qDebug() << __FUNCTION__;
//	//LOG_DURATION( __FUNCTION__ );
//	CNIO_MSG tx_frame = { 0 };
//	bool sending = false;
//	if ( TXEnabled( ) )
//	{
//		// шаговые двигатели
//		{
//			//LOG_DURATION( "VID" );
//			for ( size_t i = 0; i < UNITS.VID.size( ) / 5; ++i )
//			{
//				if ( UNITS.VID[i].ENABLE/* && UNITS.VID[i].PORT == 0*/ )
//				{
//					//LOG_DURATION( "VID" + QString::number( i ) );
//					SOD.VID_UNIT[i].ADDR = UNITS.VID[i].ADDR_TX;
//					tx_frame.ID = UNITS.VID[i].ADDR_TX;
//					tx_frame.FrameType = CNIO_EXTENDED_FRAME;
//					tx_frame.Length = 8;
//					//tx_frame.DATA = SOD.VID_UNIT[i].DATA;
//					tx_frame.Data[0] = Hi( SOD.VID_UNIT[i].VID[0] );
//					tx_frame.Data[1] = Lo( SOD.VID_UNIT[i].VID[0] );
//					tx_frame.Data[2] = Hi( SOD.VID_UNIT[i].VID[1] );
//					tx_frame.Data[3] = Lo( SOD.VID_UNIT[i].VID[1] );
//					tx_frame.Data[4] = Hi( SOD.VID_UNIT[i].VID[2] );
//					tx_frame.Data[5] = Lo( SOD.VID_UNIT[i].VID[2] );
//					tx_frame.Data[6] = Hi( SOD.VID_UNIT[i].VID[3] );
//					tx_frame.Data[7] = Lo( SOD.VID_UNIT[i].VID[3] );
//					{
//						//LOG_DURATION( "SendPacket" );
//						if ( SendPacket( &tx_frame, UNITS.VID[i].PORT ) == CNIO_SUCCESS )
//						{
//							UNITS.VID[i].TX++;
//						}
//					}
//					//qDebug() << "Sending:" << hex << tx_frame.ID;
//					//sending = true;
//				}
//			}
//		}
//	}
//	//QTimer::singleShot( 0, this, SLOT( slotRX( ) ) );
//	QCoreApplication::processEvents( );
//	QTimer::singleShot( 0, this, SLOT( slotTXVID2( ) ) );
//
//	//if ( sending )
//	//{
//	//	QTimer::singleShot( 0, this, SLOT( slotRX( ) ) );
//	//}
//	//qDebug() << "cycle" << __FUNCTION__ << delta_time << "micro";
//}
//
//void Helicopter_CAN::slotTXVID2( )
//{
//	//qDebug() << __FUNCTION__;
//	//LOG_DURATION( __FUNCTION__ );
//	CNIO_MSG tx_frame = { 0 };
//	bool sending = false;
//	if ( TXEnabled( ) )
//	{
//		// шаговые двигатели
//		{
//			//LOG_DURATION( "VID" );
//			for ( size_t i = UNITS.VID.size( ) / 5; i < UNITS.VID.size( ) / 2; ++i )
//			{
//				if ( UNITS.VID[i].ENABLE )
//				{
//					SOD.VID_UNIT[i].ADDR = UNITS.VID[i].ADDR_TX;
//					tx_frame.ID = UNITS.VID[i].ADDR_TX;
//					tx_frame.FrameType = CNIO_EXTENDED_FRAME;
//					tx_frame.Length = 8;
//					//tx_frame.DATA = SOD.VID_UNIT[i].DATA;
//					tx_frame.Data[0] = Hi( SOD.VID_UNIT[i].VID[0] );
//					tx_frame.Data[1] = Lo( SOD.VID_UNIT[i].VID[0] );
//					tx_frame.Data[2] = Hi( SOD.VID_UNIT[i].VID[1] );
//					tx_frame.Data[3] = Lo( SOD.VID_UNIT[i].VID[1] );
//					tx_frame.Data[4] = Hi( SOD.VID_UNIT[i].VID[2] );
//					tx_frame.Data[5] = Lo( SOD.VID_UNIT[i].VID[2] );
//					tx_frame.Data[6] = Hi( SOD.VID_UNIT[i].VID[3] );
//					tx_frame.Data[7] = Lo( SOD.VID_UNIT[i].VID[3] );
//					{
//						//LOG_DURATION( "SendPacket" );
//						if ( SendPacket( &tx_frame, UNITS.VID[i].PORT ) == CNIO_SUCCESS )
//						{
//							UNITS.VID[i].TX++;
//						}
//					}
//					//qDebug() << "Sending:" << hex << tx_frame.ID;
//					//sending = true;
//				}
//			}
//		}
//		//QTimer::singleShot( 0, this, SLOT( slotRX( ) ) );
//		QCoreApplication::processEvents( );
//		QTimer::singleShot( 0, this, SLOT( slotTXVID3( ) ) );
//
//		//if ( sending )
//		//{
//		//	QTimer::singleShot( 0, this, SLOT( slotRX( ) ) );
//		//}
//		//qDebug() << "cycle" << __FUNCTION__ << delta_time << "micro";
//	}
//}
//
//void Helicopter_CAN::slotTXVID3( )
//{
//	//qDebug() << __FUNCTION__;
//	//LOG_DURATION( __FUNCTION__ );
//	CNIO_MSG tx_frame = { 0 };
//	bool sending = false;
//	if ( TXEnabled( ) )
//	{
//		// шаговые двигатели
//		{
//			//LOG_DURATION( "VID" );
//			for ( size_t i = UNITS.VID.size( ) / 2; i < UNITS.VID.size( ); ++i )
//			{
//				if ( UNITS.VID[i].ENABLE )
//				{
//					SOD.VID_UNIT[i].ADDR = UNITS.VID[i].ADDR_TX;
//					tx_frame.ID = UNITS.VID[i].ADDR_TX;
//					tx_frame.FrameType = CNIO_EXTENDED_FRAME;
//					tx_frame.Length = 8;
//					//tx_frame.DATA = SOD.VID_UNIT[i].DATA;
//					tx_frame.Data[0] = Hi( SOD.VID_UNIT[i].VID[0] );
//					tx_frame.Data[1] = Lo( SOD.VID_UNIT[i].VID[0] );
//					tx_frame.Data[2] = Hi( SOD.VID_UNIT[i].VID[1] );
//					tx_frame.Data[3] = Lo( SOD.VID_UNIT[i].VID[1] );
//					tx_frame.Data[4] = Hi( SOD.VID_UNIT[i].VID[2] );
//					tx_frame.Data[5] = Lo( SOD.VID_UNIT[i].VID[2] );
//					tx_frame.Data[6] = Hi( SOD.VID_UNIT[i].VID[3] );
//					tx_frame.Data[7] = Lo( SOD.VID_UNIT[i].VID[3] );
//					{
//						//LOG_DURATION( "SendPacket" );
//						if ( SendPacket( &tx_frame, UNITS.VID[i].PORT ) == CNIO_SUCCESS )
//						{
//							UNITS.VID[i].TX++;
//						}
//					}
//					//qDebug() << "Sending:" << hex << tx_frame.ID;
//					//sending = true;
//				}
//			}
//		}
//		QTimer::singleShot( 0, this, SLOT( slotRX( ) ) );
//		//if ( sending )
//		//{
//		//	QTimer::singleShot( 0, this, SLOT( slotRX( ) ) );
//		//}
//		//qDebug() << "cycle" << __FUNCTION__ << delta_time << "micro";
//	}
//}
//
//void Helicopter_CAN::slotTXother( )
//{
//	//    qDebug() << __FUNCTION__;
//	//LOG_DURATION( __FUNCTION__ );
//	CNIO_MSG tx_frame = { 0 };
//	bool sending = false;
//	if ( TXEnabled( ) )
//	{
//		if ( _GPIOOUT_Ready ) // вообщето тут даже не надо копировать их, можно было бы переключать структуры через указатель
//		{
//			for ( size_t i = 0; i < UNITS.GPOUT.size( ); ++i )
//			{
//				SOD.OUT_UNIT[i].GPIO_OUT[0] = SOD.TMP_OUT_UNIT[i].GPIO_OUT[0];
//				SOD.OUT_UNIT[i].GPIO_OUT[1] = SOD.TMP_OUT_UNIT[i].GPIO_OUT[1];
//				SOD.OUT_UNIT[i].GPIO_OUT[2] = SOD.TMP_OUT_UNIT[i].GPIO_OUT[2];
//				SOD.OUT_UNIT[i].GPIO_OUT[3] = SOD.TMP_OUT_UNIT[i].GPIO_OUT[3];
//				SOD.OUT_UNIT[i].GPIO_OUT[4] = SOD.TMP_OUT_UNIT[i].GPIO_OUT[4];
//				SOD.OUT_UNIT[i].GPIO_OUT[5] = SOD.TMP_OUT_UNIT[i].GPIO_OUT[5];
//				SOD.OUT_UNIT[i].GPIO_OUT[6] = SOD.TMP_OUT_UNIT[i].GPIO_OUT[6];
//				SOD.OUT_UNIT[i].GPIO_OUT[7] = SOD.TMP_OUT_UNIT[i].GPIO_OUT[7];
//			}
//		}
//
//		// проходим по всем MAB-ам
//		{
//			//LOG_DURATION( "MAB" );
//			for ( size_t i = 0; i < UNITS.MAB.size( ); ++i )
//			{
//				if ( UNITS.MAB[i].ENABLE )
//				{
//					//LOG_DURATION( "MAB" + QString::number( i ) );
//					SOD.MAB_UNIT[i].ADDR = UNITS.MAB[i].ADDR_TX;
//					SOD.LCD_UNIT[i].ADDR = UNITS.MAB[i].ADDR_TX;
//
//					tx_frame.ID = UNITS.MAB[i].ADDR_TX;
//					tx_frame.FrameType = CNIO_EXTENDED_FRAME;
//					tx_frame.Length = 8;
//					//tx_frame.DATA = SOD.LCD_UNIT[i].DATA;
//					tx_frame.Data[0] = SOD.LCD_UNIT[i].CONTENT[0];
//					tx_frame.Data[1] = SOD.LCD_UNIT[i].CONTENT[1];
//					tx_frame.Data[2] = SOD.LCD_UNIT[i].CONTENT[2];
//					tx_frame.Data[3] = SOD.LCD_UNIT[i].CONTENT[3];
//					tx_frame.Data[4] = SOD.LCD_UNIT[i].CONTENT[4];
//					tx_frame.Data[5] = SOD.LCD_UNIT[i].CONTENT[5];
//					tx_frame.Data[6] = SOD.LCD_UNIT[i].CONTENT[6];
//					tx_frame.Data[7] = SOD.LCD_UNIT[i].CONTENT[7];
//					if ( SendPacket( &tx_frame, UNITS.MAB[i].PORT ) == CNIO_SUCCESS )
//					{
//						UNITS.MAB[i].TX++;
//					}
//					//qDebug() << "Sending:" << hex << tx_frame.ID;
//					sending = true;
//				}
//			}
//		}
//		{
//			//LOG_DURATION( "IN" );
//			for ( size_t i = 0; i < UNITS.GPIN.size( ); ++i )
//			{
//				if ( UNITS.GPIN[i].ENABLE )
//				{
//					SOD.IN_UNIT[i].ADDR = UNITS.GPIN[i].ADDR_TX;
//
//					tx_frame.ID = UNITS.GPIN[i].ADDR_TX;
//					tx_frame.FrameType = CNIO_EXTENDED_FRAME;
//					tx_frame.Length = 8;
//					tx_frame.Data[0] = SOD.LED_UNIT[i].DIGIT[0]; //
//					tx_frame.Data[1] = SOD.LED_UNIT[i].DIGIT[1]; //
//					tx_frame.Data[2] = SOD.LED_UNIT[i].DIGIT[2]; //
//					tx_frame.Data[3] = SOD.LED_UNIT[i].DIGIT[3]; //
//					tx_frame.Data[4] = SOD.LED_UNIT[i].DIGIT[4]; //
//					tx_frame.Data[5] = SOD.LED_UNIT[i].DIGIT[5]; //
//					tx_frame.Data[6] = SOD.LED_UNIT[i].DIGIT[6]; //
//					tx_frame.Data[7] = SOD.LED_UNIT[i].DIGIT[7]; //
//					if ( SendPacket( &tx_frame, UNITS.GPIN[i].PORT ) == CNIO_SUCCESS )
//					{
//						UNITS.GPIN[i].TX++;
//					}
//					//qDebug() << "Sending:" << hex << tx_frame.ID;
//					sending = true;
//				}
//			}
//		}
//		// UNITS.GPIOIN НЕ UNITS.ENC пересекаются по адресам 500 диапазона
//		{
//			//LOG_DURATION( "ENC" );
//			for ( size_t i = 0; i < UNITS.ENC.size( ); ++i )
//			{
//				if ( UNITS.ENC[i].ENABLE )
//				{
//					if ( SOD.ENC_UNIT[i].CLR == true ) // отправляем пакет в энкодер только если он сброшен в 0 иначе ждем пока его прочитают в логике
//					{
//						SOD.ENC_UNIT[i].ADDR = UNITS.ENC[i].ADDR_TX;
//						SOD.ENC_UNIT[i].CLR = false;
//
//						tx_frame.ID = UNITS.ENC[i].ADDR_TX;
//						tx_frame.FrameType = CNIO_EXTENDED_FRAME;
//						tx_frame.Length = 1;
//						tx_frame.Data[0] = 0xFF; //
//						if ( SendPacket( &tx_frame, UNITS.ENC[i].PORT ) == CNIO_SUCCESS )
//						{
//							UNITS.ENC[i].TX++;
//						}
//						//qDebug( ) << "Sending:" << hex << tx_frame.ID;
//						sending = true;
//					}
//				}
//			}
//		}
//		{
//			//LOG_DURATION( "OUT" );
//			for ( size_t i = 0; i < UNITS.GPOUT.size( ); ++i )
//			{
//				if ( UNITS.GPOUT[i].ENABLE )
//				{
//					SOD.OUT_UNIT[i].ADDR = UNITS.GPOUT[i].ADDR_TX;
//					tx_frame.ID = UNITS.GPOUT[i].ADDR_TX;
//					tx_frame.FrameType = CNIO_EXTENDED_FRAME;
//					tx_frame.Length = 8;
//					tx_frame.Data[0] = SOD.OUT_UNIT[i].GPIO_OUT[0];
//					tx_frame.Data[1] = SOD.OUT_UNIT[i].GPIO_OUT[1];
//					tx_frame.Data[2] = SOD.OUT_UNIT[i].GPIO_OUT[2];
//					tx_frame.Data[3] = SOD.OUT_UNIT[i].GPIO_OUT[3];
//					tx_frame.Data[4] = SOD.OUT_UNIT[i].GPIO_OUT[4];
//					tx_frame.Data[5] = SOD.OUT_UNIT[i].GPIO_OUT[5];
//					tx_frame.Data[6] = SOD.OUT_UNIT[i].GPIO_OUT[6];
//					tx_frame.Data[7] = SOD.OUT_UNIT[i].GPIO_OUT[7];
//					if ( SendPacket( &tx_frame, UNITS.GPOUT[i].PORT ) == CNIO_SUCCESS )
//					{
//						UNITS.GPOUT[i].TX++;
//					}
//					//qDebug() << "Sending:" << hex << tx_frame.ID;
//					sending = true;
//				}
//			}
//		}
//		//
//		{
//			//LOG_DURATION( "POT" );
//			for ( size_t i = 0; i < UNITS.POT.size( ); ++i )
//			{
//				if ( UNITS.POT[i].ENABLE )
//				{
//					SOD.POT_UNIT[i].ADDR = UNITS.POT[i].ADDR_TX;
//					tx_frame.ID = UNITS.POT[i].ADDR_TX;
//					tx_frame.FrameType = CNIO_EXTENDED_FRAME;
//					tx_frame.Length = 1;
//					tx_frame.Data[0] = 0xFF; // 
//					if ( SendPacket( &tx_frame, UNITS.POT[i].PORT ) == CNIO_SUCCESS )
//					{
//						UNITS.POT[i].TX++;
//					}
//					//qDebug() << "Sending:" << hex << tx_frame.ID;
//					sending = true;
//				}
//			}
//		}
//	}
//	//QTimer::singleShot( 0, this, SLOT( slotRX( ) ) );
//	QCoreApplication::processEvents( );
//	QTimer::singleShot( 0, this, SLOT( slotTXVID1( ) ) );
//
//	//if ( sending )
//	//{
//	//	QTimer::singleShot( 0, this, SLOT( slotRX( ) ) );
//	//}
//	//qDebug() << "cycle" << __FUNCTION__ << delta_time << "micro";
//}
