#ifndef CAN_COMMON_H
#define CAN_COMMON_H

#include <Windows.h>
#include <QTimer>
#include <QObject>
#include <QVector>
//#include "MXERROR.h"
#include "Logging.h"
#include "Common/CAN_Interface.h"


const DWORD CAN_timeout = 10;         // интервал ожидания ответа TODO: тут было 10 мс
//const auto N_MAX_UNITS = 8;           // максимальное количество блоков каждого вида
//const auto N_MAX_MAB_UNITS = N_MAX_UNITS;
//const auto N_MAX_VID_UNITS = 16;
//const auto N_MAX_IN_UNITS = 9;
//const auto N_MAX_OUT_UNITS = N_MAX_UNITS;
//const auto N_MAX_POT_UNITS = N_MAX_UNITS;
//const auto N_MAX_LCD_UNITS = N_MAX_UNITS;
//const auto N_MAX_ENC_UNITS = N_MAX_UNITS;

struct TMAB_Unit
{
	WORD MAB[4];           // значения МАБов
	WORD ADDR;             // адрес исходящего пакета
};

struct TVID_Unit
{
	WORD VID[4];           // значение на шаговик 0-4320
	WORD ADDR;             // адрес пакета
};

struct TGPIO_IN_Unit
{
	union{
		BYTE	GPIO_IN[8];       // тумблеры (блок по 64 бит)
		UINT8   data[8];
		UINT16  data_x2[4];
		UINT32  data_x4[2];
		UINT64  data_x8[1];
	};
	WORD ADDR;             // адрес пакета
};

struct TENC_Unit
{
	UINT64 ENC;				// энкодеры (блок по 64 бит) почти то же что и GPIO_IN но выставляют 1 только до чтения и сбрасывают
	bool   CLR;				// флаг разрешения сброса значения энкодера после его чтения
	WORD ADDR;				// адрес пакета
};

struct TGPIO_OUT_Unit
{
	union {
		BYTE    GPIO_OUT[8];      // лампочки (блок по 64 бит)
		UINT8   data[8];
		UINT16  data_x2[4];
		UINT32  data_x4[2];
		UINT64  data_x8[1];
	};
	WORD ADDR;             // адрес пакета
};

struct TLED_Unit
{
	BYTE DIGIT[8];         // числовые значения
	WORD ADDR;             // адрес исходящего пакета
};

struct TLCD_Unit
{
	BYTE CONTENT[8];       // символьные данные
	WORD ADDR;             // адрес исходящего пакета
};

struct TPOT_Unit
{
	WORD POT[4]; // значения потенциометров
	WORD ADDR;             // адрес исходящего пакета
};

struct TUnitData
{
	explicit TUnitData() {};
	TUnitData( size_t i, int port, int enable );
	size_t index = 0;   	   // порядковый номер юнита, он же индекс в SOD
	DWORD ADDR_TX = 0;         // адрес запроса
	DWORD ADDR_RX = 0;         // адрес ответа
	BYTE PORT = 0;             // порт
	BOOL ENABLE = false;       // активность блока
	LONG TX = 0;               // количество отправленных пакетов
	LONG RX = 0;               // количество принятых пакетов
	LONG RX_PREV = 0;          // предыдущее количество принятых пакетов
	BOOL FAULT = false;        // работоспособность блока
	SHORT HEARTBEAT = 0;       // счетчик сердцебиения
	QString DESC;          // постоянная часть описание блока
	QString INFO;          // описание блока со счетчиком TX/RX
	QString sFAULT;         // описание ошибки блока
};

struct TMABData : public TUnitData
{
	explicit TMABData( size_t i, int addr, int port, int enable );
};

struct TVIDData : public TUnitData
{
	explicit TVIDData( size_t i, int addr, int port, int enable );
};

struct TGPIOINData : public TUnitData
{
	explicit TGPIOINData( size_t i, int addr, int port, int enable );
};

struct TGPIOOUTData : public TUnitData
{
	explicit TGPIOOUTData( size_t i, int addr, int port, int enable );
};

struct TPOTData : public TUnitData
{
	explicit TPOTData( size_t i, int addr, int port, int enable);
};

struct TENCData : public TUnitData
{
	explicit TENCData( size_t i, int addr, int port, int enable );
};

struct TLEDData : public TUnitData
{
	explicit TLEDData( size_t i, int addr, int port, int enable );
};

struct TLCDData : public TUnitData
{
	explicit TLCDData( size_t i, int addr, int port, int enable );
};

struct TENUM_Unit
{
	USHORT ENUM[2];         // значения инкрементальных МАБов (-32000 .. 32000)
	USHORT SHIFT[2];        // смещение
	WORD ADDR;             // адрес пакета
};

struct T_SOD
{
	std::vector<TMAB_Unit>           MAB_UNIT;           // 300
	std::vector<TVID_Unit>           VID_UNIT;           // 200
	std::vector<TGPIO_IN_Unit>       IN_UNIT;        // 500
	std::vector<TGPIO_OUT_Unit>      OUT_UNIT;      // 700
	std::vector<TGPIO_OUT_Unit>      TMP_OUT_UNIT;
	std::vector<TPOT_Unit>			 POT_UNIT; // 500 700
	std::vector<TENC_Unit>			 ENC_UNIT;   // 
	std::vector<TLED_Unit>			 LED_UNIT;   // 
	std::vector<TLCD_Unit>			 LCD_UNIT;   // дисплей с Микроконтроллером
	std::vector<TENUM_Unit>			 ENUM_UNIT;			// ENUM
};

struct T_UNITS
{
	std::vector<TMABData> MAB;
	std::vector<TVIDData> VID;
	std::vector<TGPIOINData> GPIN;
	std::vector<TGPIOOUTData> GPOUT;
	std::vector<TPOTData> POT;
	std::vector<TENCData> ENC;
	std::vector<TLEDData> LED;
	std::vector<TLCDData> LCD;
};

class CANBusComm : public QObject
{

	Q_OBJECT

public:
	CANBusComm( QString sboard = "MOXA" );
	~CANBusComm();
	void Start();
	void Stop();
	T_SOD SOD;
	T_UNITS UNITS;
	int BoardCount( ) { return _board!=nullptr; }
	LONG SendPacket( PCNIO_MSG txframe, BYTE port ); // отсылка пакета
	LONG ReadPacket( PCNIO_MSG rxframe, BYTE port ); // прием пакета
	LONG ReadBuffer( BYTE port );                    // считать состояние CAN буфера
	LONG TXpacktsCNT() { return _TXpacktsCNT; }
	LONG RXpacktsCNT() { return _RXpacktsCNT; }
	WORD TXpacktsPPS() { return _TXpacktsPPS; }
	WORD RXpacktsPPS() { return _RXpacktsPPS; }
	bool CAN_Active() { return _RXpacktsPPS; }
	bool TXEnabled() { return _TXEnabled; }
	bool RXEnabled() { return _RXEnabled; }
	bool CAN_Ready() { return _canReady; }
	bool StopWorking() { return _stopWorking; }
	HANDLE Port0( ) { return _board; }
	HANDLE Port1( ) { return _board; }
	void setTXEnabled( bool TXEnabled ) { _TXpacktsCNT = 0; _RXpacktsCNT = 0; _TXEnabled = TXEnabled; }
	void setRXEnabled( bool RXEnabled ) { _RXEnabled = RXEnabled; }
	void setStopWorking( bool stopWorking ) { _stopWorking = stopWorking; }
	void setGPIOOUT_Ready( bool ready ) { _GPIOOUT_Ready = ready; }
	void SOD_Clear_IN_Data();
	void SOD_Clear_OUT_Data();
	void SOD_Clear_TMP_OUT_Data();
private:
	ICAN_Board* _board = nullptr;

	QTimer* Timer1sec;
	LONG _TXpacktsCNT;      // кол-во отправленных пакетов
	LONG _RXpacktsCNT;      // кол-во принятых пакетов
	WORD _TXpacktsCNT1s;    // кол-во отправленных пакетов в секунду
	WORD _RXpacktsCNT1s;    // кол-во принятых пакетов в секунду
	WORD _TXpacktsPPS;      // скорость отправки пакетов в секунду
	WORD _RXpacktsPPS;      // скорость приёма пакетов в секунду
	bool _TXEnabled;        // флаг разрешения передачи пакетов
	bool _RXEnabled;        // флаг разрешения приема пакетов
	bool _stopWorking;      // флаг включения временной бомбы

protected:
	QSharedPointer<TLogging> _log;
	QTimer* TimerTX;
	bool _canReady;         // флаг готовности работы CAN
	//bool _onceTXPackets;    // флаг отправки пакетов только один раз
	bool _GPIOOUT_Ready;    // флаг готовности структуры GPIO_OUT к отправке
	BYTE Hi( WORD value ) const;
	BYTE Lo( WORD value ) const;
	WORD MAB_VALUE( BYTE byte_Hi, BYTE byte_Lo, WORD max_val ) const ;
	WORD MAB_VALUE( BYTE byte_Hi, BYTE byte_Lo ) const ;  // 12 бит мабы
	WORD MAKE_WORD( BYTE byte_Hi, BYTE byte_Lo ) const ;
	USHORT ENUM_VALUE( BYTE byte_Hi, BYTE byte_Lo ) const;
private slots:
};


#endif // CAN_COMMON_H
