#ifndef CONTROLS_H
#define CONTROLS_H

#include <Windows.h>
#include <QTimer>
#include <QCoreApplication>
#include <QFileInfo>
#include <QSettings>
//#include "queuetimer.h"
#include "CAN_common.h"


const static WORD COMPUTING_INTERVAL = 20; // 15; //20;
const BYTE N_STEP_POINTS_COUNT = 8;        // количество точек шкалы нелинейности для шаговиков
const BYTE N_SELECTOR_POINTS_COUNT = 10;   // количество точек шкалы нелинейности для МАБов и потенциометров
constexpr auto N_PARAM_IN_MAX = 512;

struct TGPIO_IN_OUT
{
    BYTE UnitNum;              // номер юнита
    BYTE RemapBitNum;          // переназначенный номер бита
    bool InverseBit;           // инвертировать бит
    QString ParameterCaption;  // описание параметра
};

struct TMAB
{
    BYTE UnitNum;              // номер юнита
    BYTE RemapPosition;        // переназначенная позиция МАБа в юните
    bool Reverse;              // реверсивное положение МАБа (0 - нет, 1 - да)
    WORD OffSet;               // смещение МАБа относительно нуля
    WORD CutOff;               // отсечка по значению МАБа
    WORD MaxValue;             // максимальное значение МАБа в зависимости от разрядности (10бит=1023 или 12бит=4095)
    WORD NormalValue;          // нормализация к значению
    FLOAT Ratio;               // передаточное число редуктора
    bool OutOfRange;           // флаг выхода значения МАБа за пределы диапазона настроек
    FLOAT Value;               // текущее посчитанное нормализованое значение МАБа
    QString Caption;           // описание МАБа
};

struct TSELECTOR
{
    FLOAT Value_Scale_Point[N_SELECTOR_POINTS_COUNT];  // значение показания селектора в каждой точке нелинейности
    WORD Value_Steps_Point[N_SELECTOR_POINTS_COUNT];   // значение шагов в каждой точке нелинейности
    FLOAT Value;               // текущее значение показания селектора
    QString Caption;           // описание селектора
};

struct TVID
{
    BYTE UnitNum;              // номер юнита
    BYTE RemapPosition;        // переназначенная позиция шаговика в юните
    FLOAT Value_Scale_Point[N_STEP_POINTS_COUNT];  // значение показания прибора в каждой точке нелинейности для всех шаговиков
    WORD Value_Steps_Point[N_STEP_POINTS_COUNT];   // значение шагов в каждой точке нелинейности для всех шаговиков
    BYTE MAX_VAL_ID;
    float MIN_VAL_SCALE_POINT;    // минимальное значение прибора
    float MAX_VAL_SCALE_POINT;     // максимальное значение прибора
    FLOAT Value;               // текущее значение показания прибора
    QString Caption;           // описание прибора
};

struct TCLOCK
{
    BYTE UnitNum;              // номер юнита
    BYTE RemapPosition;        // переназначенная позиция шаговика в юните
    float Range;               // максимальное значение шкалы за 1 оборот (60 минут, 360градусов, 1000 метров итд)
    WORD Start;                // шаг начального положения, логического нуля полный круг всегда 4320 отсчетов
    //WORD Revers;               // реверсивное движение 1 или -1
    FLOAT Value;               // текущее значение показания прибора
    QString Caption;           // описание прибора
};

struct TENUM
{
    BYTE UnitNum;              // номер юнита
    BYTE RemapPosition;        // переназначенная позиция счетчика в юните
    bool Reverse;              // реверсивное положение счетчика (0 - нет, 1 - да)
    SHORT OffSet;              // минимальное значение счетчика в диапазоне (-32768 .. 32767)
    USHORT CutOff;              // максимальное значение счетчика в диапазоне (-32768 .. 32767)
    WORD NormalValue;          // нормализация к значению
    FLOAT Ratio;               // передаточное число редуктора
    WORD Multiple;             // кратность
    bool OutOfRange;           // флаг выхода значения счетчика за пределы диапазона настроек
    FLOAT Value;               // текущее посчитанное нормализованое значение счетчика
    QString Caption;           // описание счетчика
};

struct TPOTENTIOMETER
{
    BYTE UnitNum;              // номер юнита
    BYTE RemapPosition;        // переназначенная позиция потенциометра в юните
    bool Reverse;              // реверсивное положение потенциометра (0 - нет, 1 - да)
    WORD OffSet;               // смещение потенциометра относительно нуля
    WORD CutOff;               // отсечка по значению потенциометра
    WORD MaxValue;             // максимальное значение потенциометра в зависимости от разрядности (10бит=1023 или 12бит=4095)
    WORD NormalValue;          // нормализация к значению
    bool OutOfRange;           // флаг выхода значения потенциометра за пределы диапазона настроек
    FLOAT Value;               // текущее посчитанное нормализованое значение потенциометра
    QString Caption;           // описание потенциометра
};

struct TLEDINDICATOR
{
    BYTE UnitNum;              // номер юнита
    BYTE Digits;               // количество сегментов индикатора
    QString Caption;           // описание параметра
};

struct TLCDDISPLAY
{
    BYTE UnitNum;              // номер юнита
    BYTE Lines;                // количество строк дисплея
    QString Caption;           // описание параметра
};

struct TADS
{
    size_t UnitNum;              // номер юнита
    size_t RemapPosition;        // переназначенная позиция ADSа в юните
    bool   Reverse;              // реверсивное положение ADSа (0 - нет, 1 - да)
    float  OffSet;               // смещение ADSа относительно нуля
    float  CutOff;               // отсечка по значению ADSа
    float  NormalValue;          // нормализация к значению
    float  Value;                // текущее посчитанное нормализованое значение ADSа
    QString Caption;             // описание ADSа
};

struct TENCODER
{
    BYTE UnitNum;              // номер юнита
    BYTE BitLEFT;              // номер бита влево
    BYTE BitRIGHT;             // номер бита впаво
    int  Value;                // текущее значение ENC
    QString ParameterCaption;  // описание параметра
};

class T_Controls : public QObject
{
	Q_OBJECT
public:
	T_Controls( CANBusComm* CanBus );
	~T_Controls();
	bool ManualControl() { return manualControl; }
	void SetManualControl( bool val ) { manualControl = val; }
	QString Byte2Bin( BYTE val );

    template<typename T>
    QString BinView( T val );
protected:
    QVector<TGPIO_IN_OUT> PARAM_IN;
    QVector<TGPIO_IN_OUT> PARAM_OUT;
    QVector<TMAB> PARAM_MAB;
    QVector<TADS> PARAM_ADS;
    QVector<TVID> PARAM_VID;
    QVector<TCLOCK> PARAM_CLOCK;
    QVector<TPOTENTIOMETER> PARAM_POT;
    QVector<TLEDINDICATOR> PARAM_LED;
    QVector<TLCDDISPLAY> PARAM_LCD;
    QVector<TENUM> PARAM_ENUM;
    QVector<TENCODER> PARAM_ENC;
    
    std::vector <bool> gpio_in = std::vector <bool>( N_PARAM_IN_MAX, false ); // для определения изменения переключателя
    //std::vector <UINT64> enc = std::vector <UINT64>( 2, false );
    CANBusComm* can;
	QTimer* OnBoardTimer;
	bool manualControl;
	float RemoveSpeedVibrations( float speed );
	BYTE GetBitMask( BYTE bitnum );
	quint8 GetBitFromByte( quint8 byte, quint8 bitNumber );
public:
    QString GetVIDaddr( BYTE param_id );
	QString GetVIDCaption( BYTE param_id );
	float   GetVIDValue( BYTE param_id );
	WORD    GetVIDSteps( BYTE param_id );
	void    SetVIDSteps( BYTE param_id, float val );
//	void    SetVIDSteps( BYTE param_id, WORD step );
    void    SetVID( BYTE param_id, WORD val );
	void    SetVIDSteps( BYTE param_id, bool val );
	void    SetVIDSteps( BYTE param_id, quint8 val );

    QString GetCLOCKaddr( BYTE param_id );
	QString GetCLOCKCaption( BYTE param_id );
	float   GetCLOCKValue( BYTE param_id );
	WORD    GetCLOCKSteps( BYTE param_id );
	void    SetCLOCKSteps( BYTE param_id );
	void    SetCLOCKSteps( BYTE param_id, float val );
    void    SetCLOCKStepsUGR(BYTE param_id);
    void    SetCLOCKStepsUGR(BYTE param_id, float val);

	QString GetMABaddr( BYTE param_id );
	QString GetMABCaption( BYTE param_id );
	bool    MABOutOfRange( BYTE param_id );
	WORD    GetMABValue( BYTE param_id );
	float   GetMABNormalValue( BYTE param_id );
	float   MABDeflectionValue( BYTE param_id );
	float   MABNormalValue( BYTE param_id );
	float   MABAngleValue( BYTE param_id );

	WORD    GetGPIOINaddr( BYTE unitnum );
	BYTE    GetGPIOINrow( BYTE unitnum, BYTE rownum );
    UINT64  GetGPIOIN( BYTE unitnum );
    bool    GetGPIOINBitValue( WORD param_id );

	WORD    GetGPIOOUTaddr( BYTE unitnum );
	BYTE    GetGPIOOUTrow( BYTE unitnum, BYTE rownum );
    UINT64  GetGPIOOUT( BYTE unitnum );
    void    SetGPIOOUTBitValue( bool value, BYTE param_id );

    WORD    GetENCaddr( BYTE unitnum );
    int     GetENCValue( WORD param_id );   // возвращает -1 или +1 в зависимости от направления вращения, или 0 если не менялось
    void    ENCReset( );

	QString GetPOTaddr( BYTE param_id );
	QString GetPOTCaption( BYTE param_id );

	bool    POTOutOfRange( BYTE param_id );
	WORD    GetPOTValue( BYTE param_id );
	float   GetPOTNormalValue( BYTE param_id );
	float   POTNormalValue( BYTE param_id );

	float   JoystickNormalValue( BYTE param_id, int value );

    QString GetLEDINDICATORCaption( BYTE param_id );
    QString GetLEDINDICATORValue( BYTE param_id );
    WORD    GetLEDINDICATORaddr( BYTE unitnum );
    BYTE    GetLEDINDICATORrow( BYTE unitnum, BYTE rownum );
    void    SetLEDINDICATORValue( qint32 value, BYTE param_id );
    void    SetLEDINDICATORValue(QVector<BYTE> value, BYTE param_id);
    
	WORD    GetLCDDISPLAYaddr( BYTE unitnum );
	BYTE    GetLCDDISPLAYrow( BYTE unitnum, BYTE rownum );
	void    SetLCDDISPLAYValue( QByteArray value, BYTE& valuepart, BYTE param_id );

    QString GetENUMaddr( BYTE param_id );
    QString GetENUMCaption( BYTE param_id );
    bool    ENUMOutOfRange( BYTE param_id );
    USHORT  GetENUMValue( BYTE param_id );
    USHORT  GetENUMShift( BYTE param_id );
    float   GetENUMNormalValue( BYTE param_id );
    float   ENUMNormalValue( BYTE param_id );

};

template<typename T>
QString T_Controls::BinView( T val )
{
    //    constexpr len = sizeof( T ) * 8;
    return QString::number( val, 2 ).rightJustified( sizeof( T ) * 8, '0' );
}

#endif // CONTROLS_H
