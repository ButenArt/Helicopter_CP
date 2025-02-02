#include "Controls.h"
#include <QDebug>

T_Controls::T_Controls( CANBusComm* CanBus )
{
	can = CanBus;
	//ParamsList = new T_ParamsList;

	//PARAM_IN.resize( N_PARAM_IN );
	//PARAM_OUT.resize( N_PARAM_OUT );
	//PARAM_MAB.resize( N_PARAM_MAB );
	//PARAM_ADS.resize( N_PARAM_ADS );
	//PARAM_VID.resize( N_PARAM_VID );
	//PARAM_CLOCK.resize( N_PARAM_CLOCK );
	//PARAM_POT.resize( N_PARAM_POT );
	//PARAM_LED.resize( N_PARAM_LED );
	//PARAM_LCD.resize( N_PARAM_LCD );
	//PARAM_ENC.resize( N_PARAM_ENC );
	//PARAM_ENUM.resize( N_PARAM_ENUM );

    OnBoardTimer = new QTimer(this);
    OnBoardTimer->setTimerType(Qt::PreciseTimer);
    OnBoardTimer->setInterval(COMPUTING_INTERVAL);
    OnBoardTimer->stop();

    manualControl = false;
}

T_Controls::~T_Controls()
{
    delete OnBoardTimer;
	//delete ParamsList;
}

float T_Controls::RemoveSpeedVibrations(float speed)
{
    return (fabs(speed) < 0.5f) ? 0.f : speed;
}

BYTE T_Controls::GetBitMask(BYTE bitnum)
{
//    assert(bitnum > 0 && bitnum < 9);
    return 1 << (bitnum - 1);
}

quint8 T_Controls::GetBitFromByte(quint8 byte, quint8 bitNumber)
{
    return ((byte >> bitNumber)  & 0x01);
}

QString T_Controls::Byte2Bin(BYTE val)
{
    return QString::number(val, 2).rightJustified(8, '0');
}

QString T_Controls::GetVIDaddr( BYTE param_id )
{
	// qDebug( ) << __FUNCTION__;
	assert( param_id >= 0 && param_id < PARAM_VID.size( ) );

	auto unitnum = PARAM_VID[param_id].UnitNum;
	auto position = PARAM_VID[param_id].RemapPosition;
	if ( position == 0 ) return "";
	return " ADDR: [0x" + QString::number( can->SOD.VID_UNIT[unitnum].ADDR, 16 ).toUpper() + "][" + QString::number( position ) + "]";
}

QString T_Controls::GetVIDCaption( BYTE param_id )
{
	// qDebug( ) << __FUNCTION__;
	assert( param_id >= 0 && param_id < PARAM_VID.size( ) );

	return PARAM_VID[param_id].Caption;
}

float T_Controls::GetVIDValue( BYTE param_id )
{
	// qDebug( ) << __FUNCTION__;
	assert( param_id >= 0 && param_id < PARAM_VID.size( ) );

	return PARAM_VID[param_id].Value;
}

WORD T_Controls::GetVIDSteps( BYTE param_id )
{
	// qDebug( ) << __FUNCTION__;
	assert( param_id >= 0 && param_id < PARAM_VID.size( ) );

	auto unitnum = PARAM_VID[param_id].UnitNum;
	auto position = PARAM_VID[param_id].RemapPosition;
	if ( position == 0 ) return 0;

	return can->SOD.VID_UNIT[unitnum].VID[position - 1];
}

void T_Controls::SetVIDSteps( BYTE param_id, float val )
{
	// qDebug( ) << __FUNCTION__;
	assert( param_id >= 0 && param_id < PARAM_VID.size( ) );

	PARAM_VID[param_id].Value = val;
	auto& VID = PARAM_VID[param_id];   // небольшая замена длинного имени

	auto unitnum = VID.UnitNum;
	auto position = VID.RemapPosition;
	if ( position == 0 ) return;

	auto& RET = can->SOD.VID_UNIT[unitnum].VID[position - 1]; // небольшая замена длинного имени

	if ( VID.MAX_VAL_ID == 0 ) // если конфиг не заполнен (значения всех точек одинаковы)
	{
		RET = 0;
		return;
	}

	// проверка выхода за пределы шкалы и нормализация значения для вывода
	if ( VID.Value < VID.MIN_VAL_SCALE_POINT )
	{
		RET = VID.Value_Steps_Point[0];
		return;
	}
	if ( VID.Value > VID.MAX_VAL_SCALE_POINT )
	{
		RET = VID.Value_Steps_Point[VID.MAX_VAL_ID];
		return;
	}

	int range = 0;
	while ( VID.Value > VID.Value_Scale_Point[++range] ); // находим в каком диапазоне находится значение .Value

	//assert((range - 1) < 0);

	auto scale_range_min = VID.Value_Scale_Point[range - 1];
	auto scale_range_max = VID.Value_Scale_Point[range];
	auto step_range_min = VID.Value_Steps_Point[range - 1];
	auto step_range_max = VID.Value_Steps_Point[range];

	auto scale_range = scale_range_max - scale_range_min;
	auto step_range = step_range_max - step_range_min;
	auto scale = (VID.Value - scale_range_min);
	auto res = scale * step_range / scale_range + step_range_min;

	RET = static_cast<WORD>(res);
}

/// <summary>
/// Позиционирование стрелочного прибора логическим значением которое в LogicInOut должно быть в диапазоне 0..1
/// </summary>
/// <param name="param_id"></param>
/// <param name="step"></param>
/// <returns>none</returns>
void T_Controls::SetVIDSteps( BYTE param_id, bool step )
{
	// qDebug( ) << __FUNCTION__;
	SetVIDSteps( param_id, step ? 1.f : 0.f );
}

/// <summary>
/// Позиционирование стрелочного прибора uint8 значением которое в LogicInOut должно быть в диапазоне 0..255
/// </summary>
/// <param name="param_id"></param>
/// <param name="step"></param>
/// <returns>none</returns>
void T_Controls::SetVIDSteps( BYTE param_id, quint8 step )
{
	// qDebug( ) << __FUNCTION__;
	SetVIDSteps( param_id, static_cast<float>(step) );
}

void T_Controls::SetVID( BYTE param_id, WORD val )
{
	assert( param_id >= 0 && param_id < PARAM_VID.size( ) );

	auto unitnum = PARAM_VID[param_id].UnitNum;
	auto position = PARAM_VID[param_id].RemapPosition;
	if ( position == 0 ) return;

	can->SOD.VID_UNIT[unitnum].VID[position - 1] = val;
}

/// <summary>
/// 
/// </summary>
/// <param name="param_id"></param>
/// <returns></returns>
QString T_Controls::GetCLOCKaddr( BYTE param_id )
	{
	// qDebug( ) << __FUNCTION__;
	assert( param_id >= 0 && param_id < PARAM_CLOCK.size( ) );

	auto unitnum  = PARAM_CLOCK[param_id].UnitNum;
	auto position = PARAM_CLOCK[param_id].RemapPosition;
	if ( position == 0 ) return "";
	return " ADDR: [0x" + QString::number( can->SOD.VID_UNIT[unitnum].ADDR, 16 ).toUpper() + "][" + QString::number( position ) + "]";
}

QString T_Controls::GetCLOCKCaption( BYTE param_id )
{
	// qDebug( ) << __FUNCTION__;
	assert( param_id >= 0 && param_id < PARAM_CLOCK.size( ) );

	return PARAM_CLOCK[param_id].Caption;
}

float T_Controls::GetCLOCKValue( BYTE param_id )
{
	// qDebug( ) << __FUNCTION__;
	assert( param_id >= 0 && param_id < PARAM_CLOCK.size( ) );

	return PARAM_CLOCK[param_id].Value;
}

WORD T_Controls::GetCLOCKSteps( BYTE param_id )
	{
	// qDebug( ) << __FUNCTION__;
	assert( param_id >= 0 && param_id < PARAM_CLOCK.size( ) );

	auto unitnum  = PARAM_CLOCK[param_id].UnitNum;
	auto position = PARAM_CLOCK[param_id].RemapPosition;
	if ( position == 0 ) return 0;

	return can->SOD.VID_UNIT[unitnum].VID[position - 1];
	}

void T_Controls::SetCLOCKSteps( BYTE param_id, float val )
{
	//if ( !CheckVIDId( param_id ) )
	//{
	//	return;
	//}
	// qDebug( ) << __FUNCTION__;
	assert( param_id >= 0 && param_id < PARAM_VID.size( ) );
	PARAM_CLOCK[param_id].Value = val;
	SetCLOCKSteps( param_id );
}

void T_Controls::SetCLOCKSteps( BYTE param_id )
{
	// qDebug( ) << __FUNCTION__;
	assert( param_id >= 0 && param_id < PARAM_CLOCK.size( ) );

	auto& VID = PARAM_CLOCK[param_id];   // небольшая замена длинного имени

	auto unitnum = VID.UnitNum;
	auto position = VID.RemapPosition;
	if ( position == 0 ) return;

	auto& RET = can->SOD.VID_UNIT[unitnum].VID[position - 1]; // небольшая замена длинного имени

	if ( VID.Range == 0 ) // если конфиг не заполнен
	{
		RET = 0;
		return;
	}
	// загоняем значения в диапозон прибора, если получили -90град это равно 270
	while ( VID.Value > VID.Range ) VID.Value -= VID.Range;
	while ( VID.Value < 0 ) VID.Value += VID.Range;

	double res = VID.Value * 4320.f / VID.Range;

	RET = VID.Start + static_cast<WORD>(res);

	if ( RET > 4320 )
		RET -= 4320;
	else if ( RET < 0 )
		RET += 4320;
}

void T_Controls::SetCLOCKStepsUGR(BYTE param_id, float val)
{
	//if ( !CheckVIDId( param_id ) )
	//{
	//	return;
	//}
	// qDebug( ) << __FUNCTION__;
	assert(param_id >= 0 && param_id < PARAM_VID.size());
	PARAM_CLOCK[param_id].Value = val;
	SetCLOCKStepsUGR(param_id);
}

void T_Controls::SetCLOCKStepsUGR(BYTE param_id)
{
	// qDebug( ) << __FUNCTION__;
	assert(param_id >= 0 && param_id < PARAM_CLOCK.size());

	auto& VID = PARAM_CLOCK[param_id];   // небольшая замена длинного имени

	auto unitnum = VID.UnitNum;
	auto position = VID.RemapPosition;
	if (position == 0) return;

	auto& RET = can->SOD.VID_UNIT[unitnum].VID[position - 1]; // небольшая замена длинного имени

	if (VID.Range == 0) // если конфиг не заполнен
	{
		RET = 0;
		return;
	}
	// загоняем значения в диапозон прибора, если получили -90град это равно 270
	while (VID.Value > VID.Range) VID.Value -= VID.Range;
	while (VID.Value < 0) VID.Value += VID.Range;

	double res = VID.Value * 4450.f / VID.Range;

	RET = VID.Start + static_cast<WORD>(res);

	if (RET > 4450)
		RET -= 4450;
	else if (RET < 0)
		RET += 4450;
}

QString T_Controls::GetMABaddr( BYTE param_id )
{
	// qDebug( ) << __FUNCTION__;
	assert( param_id >= 0 && param_id < PARAM_MAB.size( ) );
	BYTE unitnum = PARAM_MAB[param_id].UnitNum;
	BYTE position = PARAM_MAB[param_id].RemapPosition;
	if ( position == 0 ) return "";

	return " ADDR: [0x" + QString::number( can->SOD.MAB_UNIT[unitnum].ADDR, 16 ).toUpper() + "][" + QString::number( position ) + "]";
}

QString T_Controls::GetMABCaption( BYTE param_id )
{
	// qDebug( ) << __FUNCTION__;
	assert( param_id >= 0 && param_id < PARAM_MAB.size( ) );

	return PARAM_MAB[param_id].Caption;
}

bool T_Controls::MABOutOfRange( BYTE param_id )
	{
	// qDebug( ) << __FUNCTION__;
	assert( param_id >= 0 && param_id < PARAM_MAB.size( ) );

	return PARAM_MAB[param_id].OutOfRange;
	}

WORD T_Controls::GetMABValue( BYTE param_id )
{
	// qDebug( ) << __FUNCTION__;
	assert( param_id >= 0 && param_id < PARAM_MAB.size( ) );

	BYTE unitnum = PARAM_MAB[param_id].UnitNum;
	BYTE position = PARAM_MAB[param_id].RemapPosition;
	if ( position == 0 ) return 0;

	return can->SOD.MAB_UNIT[unitnum].MAB[position - 1];
}

float T_Controls::GetMABNormalValue( BYTE param_id )
{
	// qDebug( ) << __FUNCTION__;
	assert( param_id >= 0 && param_id < PARAM_MAB.size( ) );
	return PARAM_MAB[param_id].Value;
}

float T_Controls::MABDeflectionValue( BYTE param_id )
{
	// qDebug( ) << __FUNCTION__;
	assert( param_id >= 0 && param_id < PARAM_MAB.size( ) );

	float maxval = PARAM_MAB[param_id].MaxValue;
	float deflection = qAbs( 360.0f * (PARAM_MAB[param_id].OffSet - PARAM_MAB[param_id].CutOff) / (maxval + 1.0f) );  // угол движения
	return deflection;
}

float T_Controls::MABNormalValue( BYTE param_id )
{
	// qDebug( ) << __FUNCTION__;
	assert( param_id >= 0 && param_id < PARAM_MAB.size( ) );

	auto& MAB = PARAM_MAB[param_id];

	BYTE unitnum = MAB.UnitNum;
	BYTE position = MAB.RemapPosition;
	if ( position == 0 ) return 0.f;

	WORD MABvalue = can->SOD.MAB_UNIT[unitnum].MAB[position - 1];
	WORD value = 0;
	WORD range = 0;
	float prescaler = 0.f;
	float res = 0.f;
	MAB.OutOfRange = false;

	// проверяем, не ушли ли мабы за границы и если да, то приравниваем их этим границам
	if ( MAB.OffSet < MAB.CutOff )
	{
		if ( MABvalue < MAB.OffSet ) MABvalue = MAB.OffSet;
		if ( MABvalue > MAB.CutOff ) MABvalue = MAB.CutOff;
	}
	// это не инверсный ход ручки, а переход через 0
	else
	{
		if ( (MABvalue < MAB.OffSet) && (MABvalue > MAB.CutOff) )
		{
			if ( qAbs( MABvalue - MAB.OffSet ) < qAbs( MABvalue - MAB.CutOff ) )
				MABvalue = MAB.OffSet;
			else
				MABvalue = MAB.CutOff;
		}
	}

	if ( MABvalue < MAB.OffSet )
	{
		value = MABvalue + MAB.MaxValue - MAB.OffSet;
	} else
	{
		value = MABvalue - MAB.OffSet;
	}

	if ( MAB.OffSet < MAB.CutOff )
	{
		range = MAB.CutOff - MAB.OffSet;
		if ( (MABvalue < MAB.OffSet) || (MABvalue > MAB.CutOff) ) MAB.OutOfRange = true;
	} else
	{
		range = MAB.CutOff + MAB.MaxValue - MAB.OffSet;
		if ( (MABvalue < MAB.OffSet) && (MABvalue > MAB.CutOff) ) MAB.OutOfRange = true;
	}
	if ( range > 0 )
	{
		if ( range < MAB.NormalValue ) range = MAB.NormalValue;
		if ( MAB.Reverse ) value = range - value;
		prescaler = (float)MAB.NormalValue / (float)range;
	}
	res = (float)value * prescaler;
	if ( res > MAB.NormalValue ) res = MAB.NormalValue;
	MAB.Value = res;
	return res;
}

float T_Controls::MABAngleValue( BYTE param_id )
{
	// qDebug( ) << __FUNCTION__;
	assert( param_id < PARAM_MAB.size( ) );

	auto& MAB = PARAM_MAB[param_id];

	auto unitnum_mab = MAB.UnitNum;
	auto position_mab = MAB.RemapPosition;
	if ( position_mab == 0 ) return 0;

	auto& MAB_value = can->SOD.MAB_UNIT[unitnum_mab].MAB[position_mab - 1];

	// mab
	WORD rude_value = 0;
	WORD rude_range = 0;
	MAB.OutOfRange = false;
	if ( MAB.OffSet < MAB.CutOff )
	{
		if ( MAB_value < MAB.OffSet ) MAB_value = MAB.OffSet;
		if ( MAB_value > MAB.CutOff ) MAB_value = MAB.CutOff;
	}
	if ( MAB.OffSet > MAB.CutOff )
	{
		if ( (MAB_value < MAB.OffSet) && (MAB_value > MAB.CutOff) )
		{
			if ( qAbs( MAB_value - MAB.OffSet ) < qAbs( MAB_value - MAB.CutOff ) )
				MAB_value = MAB.OffSet;
			else
				MAB_value = MAB.CutOff;
		}
	}
	if ( MAB_value < MAB.OffSet )
	{
		rude_value = MAB_value + MAB.MaxValue - MAB.OffSet;
	}
	if ( MAB_value > MAB.OffSet )
	{
		rude_value = MAB_value - MAB.OffSet;
	}
	if ( MAB.OffSet < MAB.CutOff )
	{
		rude_range = MAB.CutOff - MAB.OffSet;
		if ( (can->SOD.MAB_UNIT[unitnum_mab].MAB[position_mab - 1] < MAB.OffSet) || (can->SOD.MAB_UNIT[unitnum_mab].MAB[position_mab - 1] > MAB.CutOff) ) MAB.OutOfRange = true;
	}
	if ( MAB.OffSet > MAB.CutOff )
	{
		rude_range = MAB.CutOff + MAB.MaxValue - MAB.OffSet;
		if ( (can->SOD.MAB_UNIT[unitnum_mab].MAB[position_mab - 1] < MAB.OffSet) && (can->SOD.MAB_UNIT[unitnum_mab].MAB[position_mab - 1] > MAB.CutOff) ) MAB.OutOfRange = true;
	}
	if ( MAB.Reverse ) rude_value = rude_range - rude_value;
	// TODO разобраться с +1 в MAB.MaxValue + 1.0f
	float fullAngle = (rude_range / (MAB.MaxValue + 1.0f)) * 360.0f * MAB.Ratio;  // максимальный грубый угол сектора
	float currentAngle = (rude_value / 1.0f / rude_range) * fullAngle;                                  // текущий грубый угол
	MAB.Value = currentAngle;
	if ( currentAngle < 0 ) currentAngle = 0;
	if ( currentAngle > 360 ) currentAngle = 360;
	return currentAngle;
}

WORD T_Controls::GetGPIOINaddr( BYTE unitnum )
{
	// qDebug( ) << __FUNCTION__;
	if ( unitnum >= can->SOD.IN_UNIT.size( ) ) return 0;
	return can->SOD.IN_UNIT[unitnum].ADDR;
}

BYTE T_Controls::GetGPIOINrow( BYTE unitnum, BYTE rownum )
{
	// qDebug( ) << __FUNCTION__;
	if ( unitnum >= can->SOD.IN_UNIT.size( ) ) return 0;
	return can->SOD.IN_UNIT[unitnum].GPIO_IN[rownum];
}

UINT64 T_Controls::GetGPIOIN( BYTE unitnum )
{
	// qDebug( ) << __FUNCTION__;
	if ( unitnum >= can->SOD.IN_UNIT.size( ) ) return 0;
	return can->SOD.IN_UNIT[unitnum].data_x8[0];
}

WORD T_Controls::GetGPIOOUTaddr( BYTE unitnum )
{
	// qDebug( ) << __FUNCTION__;
	if ( unitnum >= can->SOD.OUT_UNIT.size( ) ) return 0;
	return can->SOD.OUT_UNIT[unitnum].ADDR;
}

BYTE T_Controls::GetGPIOOUTrow( BYTE unitnum, BYTE rownum )
{
	// qDebug( ) << __FUNCTION__;
	if ( unitnum >= can->SOD.OUT_UNIT.size( ) ) return 0;
	return can->SOD.OUT_UNIT[unitnum].GPIO_OUT[rownum];
}

UINT64 T_Controls::GetGPIOOUT( BYTE unitnum )
{
	if ( unitnum >= can->SOD.OUT_UNIT.size( ) ) return 0;
	return can->SOD.OUT_UNIT[unitnum].data_x8[0];
}

bool XOR( bool x, bool y )
{
	//return (x || y) && !(x && y);
	return ( !x != !y );
}

bool T_Controls::GetGPIOINBitValue( WORD param_id )
{
	// qDebug( ) << __FUNCTION__;
	assert( param_id < PARAM_IN.size( ) );

	auto& ParameterCaption = PARAM_IN[param_id].ParameterCaption;
	auto bitnum = PARAM_IN[param_id].RemapBitNum;
	if ( bitnum == 0 ) return false;

	auto unitnum = PARAM_IN[param_id].UnitNum;
	auto inverse = PARAM_IN[param_id].InverseBit;
	auto row = (bitnum - 1) / 8;
	auto mask = GetBitMask( bitnum - (row * 8) );
	auto val = can->SOD.IN_UNIT[unitnum].GPIO_IN[row];
	auto bit = static_cast<bool>(val & mask);
	auto ret = XOR( bit, inverse );

	if ( gpio_in[param_id] != bit )
		qDebug() << "param_id = " << param_id << ParameterCaption << "=" << bit << "; unitnum =" << unitnum << "bitnum =" << bitnum << "mask = " << mask << "invers = " << inverse << "return {" << ret << "}";
	gpio_in[param_id] = bit;


	return ret;
}

// эта функция записывает только 1, нули не сбрасывает, буфер нужно очистить заранее!
// при этом делает кучу не нужной работы, если не пишешь 0, то зачем счиать дальше????? подправил
void T_Controls::SetGPIOOUTBitValue( bool value, BYTE param_id )
{
	// qDebug( ) << __FUNCTION__;
	assert( param_id < PARAM_OUT.size( ) );

	BYTE unitnum = PARAM_OUT[param_id].UnitNum;
	BYTE bitnum = PARAM_OUT[param_id].RemapBitNum;
	bool inverse = PARAM_OUT[param_id].InverseBit;

	bool outval = (!inverse)?value:!value;

	if ( !outval || bitnum == 0 ) return; // выходим если записать нужно 0 или бит записи не определен

	BYTE row = (bitnum - 1) / 8;
	BYTE mask = GetBitMask( bitnum - (row * 8) );
	can->SOD.TMP_OUT_UNIT[unitnum].GPIO_OUT[row] |= mask;
}

WORD T_Controls::GetENCaddr( BYTE unitnum )
{
	// qDebug( ) << __FUNCTION__;
	return can->SOD.ENC_UNIT[unitnum].ADDR;
}

int T_Controls::GetENCValue( WORD param_id )
{
	//qDebug( ) << __FUNCTION__ << param_id;
	assert( param_id < PARAM_ENC.size( ) );

	//auto& ParameterCaption = PARAM_ENC[param_id].ParameterCaption;
	auto unitnum = PARAM_ENC[param_id].UnitNum;
	auto left_bit = PARAM_ENC[param_id].BitLEFT;
	auto right_bit = PARAM_ENC[param_id].BitRIGHT;
	auto ret = 0;
	if ( left_bit == 0 || right_bit == 0 ) return ret;

	auto val = can->SOD.ENC_UNIT[unitnum].ENC;

	auto left = ( val >> (left_bit-1) ) & 1u;
	auto right = ( val >> (right_bit-1) ) & 1u;
	if ( left ) 
		ret = -1;
	else if ( right ) 
		ret = 1;

	return ret;
}

void T_Controls::ENCReset( )
{
	// qDebug( ) << __FUNCTION__;
	for ( auto& UNIT : can->SOD.ENC_UNIT )
	{
		UNIT.CLR = true;
	}
}

QString T_Controls::GetPOTaddr( BYTE param_id )
{
	// qDebug( ) << __FUNCTION__;
	assert( param_id < PARAM_POT.size( ) );

	BYTE unitnum = PARAM_POT[param_id].UnitNum;
	BYTE position = PARAM_POT[param_id].RemapPosition;
	if ( position == 0 ) return "";

	return " ADDR: [0x" + QString::number( can->SOD.POT_UNIT[unitnum].ADDR, 16 ).toUpper() + "][" + QString::number( position ) + "]";
}

QString T_Controls::GetPOTCaption( BYTE param_id )
	{
	// qDebug( ) << __FUNCTION__;
	assert( param_id < PARAM_POT.size( ) );

	return PARAM_POT[param_id].Caption;
	}

bool T_Controls::POTOutOfRange( BYTE param_id )
{
	// qDebug( ) << __FUNCTION__;
	assert( param_id < PARAM_POT.size( ) );

	return PARAM_POT[param_id].OutOfRange;
}

WORD T_Controls::GetPOTValue( BYTE param_id )
{
	// qDebug( ) << __FUNCTION__;
	assert( param_id < PARAM_POT.size( ) );

	BYTE unitnum = PARAM_POT[param_id].UnitNum;
	BYTE position = PARAM_POT[param_id].RemapPosition;
	if ( position == 0 ) return 0;

	return can->SOD.POT_UNIT[unitnum].POT[position - 1];
}

float T_Controls::GetPOTNormalValue( BYTE param_id )
{
	// qDebug( ) << __FUNCTION__;
	assert( param_id < PARAM_POT.size( ) );

	return PARAM_POT[param_id].Value;
}

float T_Controls::POTNormalValue( BYTE param_id )
{
	// qDebug( ) << __FUNCTION__;
	assert( param_id < PARAM_POT.size( ) );

	auto& POT = PARAM_POT[param_id];

	BYTE unitnum = POT.UnitNum;
	BYTE position = POT.RemapPosition;
	if ( position == 0 ) return 0;

	WORD POTValue = can->SOD.POT_UNIT[unitnum].POT[position - 1];
	WORD value = 0;
	WORD range = 0;
	float prescaler = 0;
	float res = 0;
	POT.OutOfRange = false;

	if ( POTValue < POT.OffSet ) 
	{
		POTValue = POT.OffSet; POT.OutOfRange = true;
	}
	if ( POTValue > POT.CutOff ) 
	{
		POTValue = POT.CutOff; POT.OutOfRange = true;
	}

	if ( POTValue > POT.OffSet )
	{
		value = POTValue - POT.OffSet;
	}

	range = POT.CutOff - POT.OffSet;

	if ( range > 0 )
	{
		if ( range < POT.NormalValue ) range = POT.NormalValue;
		if ( POT.Reverse ) value = range - value;
		prescaler = (float)POT.NormalValue / range;
	}

	res = value * prescaler;
	if ( res > POT.NormalValue ) res = POT.NormalValue;
	POT.Value = res;
	return res;
}

float T_Controls::JoystickNormalValue( BYTE param_id, int value )
{
	if ( param_id <= 0 ) return 0;
	int range = 65535;
	float prescaler = 0;
	float res = 0;

	prescaler = (float)PARAM_MAB[param_id].NormalValue / range;

	res = (value + 32768) * prescaler;
	if ( res > PARAM_MAB[param_id].NormalValue ) res = PARAM_MAB[param_id].NormalValue;
	return res;
}

QString T_Controls::GetLEDINDICATORCaption( BYTE param_id )
{
	// qDebug( ) << __FUNCTION__;
	assert( param_id < PARAM_LED.size( ) );

	return PARAM_LED[param_id].Caption;
}

QString T_Controls::GetLEDINDICATORValue( BYTE param_id )
{
	// qDebug( ) << __FUNCTION__;
	assert( param_id < PARAM_LED.size( ) );

	BYTE unitnum = PARAM_LED[param_id].UnitNum;
	BYTE digits = PARAM_LED[param_id].Digits;
	if ( digits <= 0 ) return "error";
	if ( digits > 8 ) digits = 8;
	QString outval = "";
	for ( int d = 0; d < 8; d++ )
	{
		outval.append( QString::number( can->SOD.LED_UNIT[unitnum].DIGIT[d] ) );
	}
	return outval;
}

WORD T_Controls::GetLEDINDICATORaddr( BYTE unitnum )
{
	// qDebug( ) << __FUNCTION__;
	return can->SOD.LED_UNIT[unitnum].ADDR;
}

BYTE T_Controls::GetLEDINDICATORrow( BYTE unitnum, BYTE rownum )
{
	// qDebug( ) << __FUNCTION__;
	return can->SOD.LED_UNIT[unitnum].DIGIT[rownum];
}

void T_Controls::SetLEDINDICATORValue( qint32 value, BYTE param_id )
{
	// qDebug( ) << __FUNCTION__;
	//if ( param_id < PARAM_LED.size( ) ) return;
	assert( param_id < PARAM_LED.size( ) );
	BYTE unitnum = PARAM_LED[param_id].UnitNum;
	BYTE digits = PARAM_LED[param_id].Digits;
	if ( digits <= 0 ) return;
	if ( digits > 8 ) digits = 8;
	QString outval = QString::number( value ).rightJustified( digits, '0' );
	for ( int d = 0; d < 8; d++ )
	{
		can->SOD.LED_UNIT[unitnum].DIGIT[d] = 0x12; // гасим все секции индикатора
	}
	if ( value != 65535 )
	{
		for ( int i = 0; i < digits; i++ )
		{
			if ( outval.at( i ).isDigit( ) ) 
				can->SOD.LED_UNIT[unitnum].DIGIT[i] = outval.at( i ).digitValue( );
			else if (outval.at(i) == QChar('-'))
				can->SOD.LED_UNIT[unitnum].DIGIT[i] = 13;
		}
	}
}

void T_Controls::SetLEDINDICATORValue(QVector<BYTE> value, BYTE param_id)
{
	// qDebug( ) << __FUNCTION__;
	assert(param_id < PARAM_LED.size());
	BYTE unitnum = PARAM_LED[param_id].UnitNum;
	BYTE digits = PARAM_LED[param_id].Digits;
	if (digits <= 0) return;
	if (digits > 8) digits = 8;
	for (int d = 0; d < 8; d++)
	{
		can->SOD.LED_UNIT[unitnum].DIGIT[d] = 0xfff; // гасим все секции индикатора
	}
	for (int i = 0; i < digits; i++)
	{
		can->SOD.LED_UNIT[unitnum].DIGIT[i] = value[i];
		// "-" соответсвует 0x13
	}
}

WORD T_Controls::GetLCDDISPLAYaddr( BYTE unitnum )
{
	// qDebug( ) << __FUNCTION__;
	return can->SOD.LCD_UNIT[unitnum].ADDR;
}

BYTE T_Controls::GetLCDDISPLAYrow( BYTE unitnum, BYTE rownum )
{
	// qDebug( ) << __FUNCTION__;
	return can->SOD.LCD_UNIT[unitnum].CONTENT[rownum];
}

void T_Controls::SetLCDDISPLAYValue( QByteArray value, BYTE& valuepart, BYTE param_id )
{
	// qDebug( ) << __FUNCTION__;
	assert( param_id < PARAM_LCD.size( ) );

	BYTE unitnum = PARAM_LCD[param_id].UnitNum;
	BYTE lines = PARAM_LCD[param_id].Lines;
	if ( lines <= 0 ) return;

	if ( lines > 7 ) lines = 7;
	BYTE chunk[8] = {};
	chunk[0] = valuepart;                          // номер посылки (1-21)
	for ( int j = 0; j < 7; j++ )                         // 7 байт данных
	{
		chunk[j + 1] = value[((valuepart - 1) * 7) + j];   // всего 21 посылка по 7 байт + 21 байт с номерами посылок, итого 21*7+21=168 байт для обновления 7 строк дисплея
	}
	for ( int i = 0; i < 8; i++ )
	{
		can->SOD.LCD_UNIT[unitnum].CONTENT[i] = chunk[i];
	}
	valuepart++;
	if ( valuepart > 21 ) valuepart = 1;
}

QString T_Controls::GetENUMaddr( BYTE param_id )
{
	if ( param_id <= 0 ) return "";
	BYTE unitnum = PARAM_ENUM[param_id].UnitNum;
	BYTE position = PARAM_ENUM[param_id].RemapPosition;
	if ( position == 0 ) return "";
	return " ADDR: [0x" + QString::number( can->SOD.ENUM_UNIT[unitnum].ADDR, 16 ).toUpper( ) + "][" + QString::number( position ) + "]";
}

QString T_Controls::GetENUMCaption( BYTE param_id )
{
	if ( param_id <= 0 ) return "";
	return PARAM_ENUM[param_id].Caption;
}

bool T_Controls::ENUMOutOfRange( BYTE param_id )
{
	if ( param_id <= 0 ) return false;
	return PARAM_ENUM[param_id].OutOfRange;
}

USHORT T_Controls::GetENUMValue( BYTE param_id )
{
	if ( param_id <= 0 ) return 0;
	BYTE unitnum = PARAM_ENUM[param_id].UnitNum;
	BYTE position = PARAM_ENUM[param_id].RemapPosition;
	if ( position == 0 ) return 0;
	return (USHORT)can->SOD.ENUM_UNIT[unitnum].ENUM[position - 1];
}

USHORT T_Controls::GetENUMShift( BYTE param_id )
{
	if ( param_id <= 0 ) return 0;
	BYTE unitnum = PARAM_ENUM[param_id].UnitNum;
	BYTE position = PARAM_ENUM[param_id].RemapPosition;
	if ( position == 0 ) return 0;
	return (USHORT)can->SOD.ENUM_UNIT[unitnum].SHIFT[position - 1];
}

float T_Controls::GetENUMNormalValue( BYTE param_id )
{
	if ( param_id <= 0 ) return 0;
	return PARAM_ENUM[param_id].Value;
}

float T_Controls::ENUMNormalValue( BYTE param_id )
{
	if ( param_id <= 0 ) return 0;
	BYTE unitnum = PARAM_ENUM[param_id].UnitNum;
	BYTE position = PARAM_ENUM[param_id].RemapPosition;
	if ( position == 0 ) return 0;
	USHORT ENUMvalue = can->SOD.ENUM_UNIT[unitnum].ENUM[position - 1];
	int value = 0;
	int range = 0;
	float prescaler = 0;
	float res = 0;
	PARAM_ENUM[param_id].OutOfRange = false;

	if ( ENUMvalue < PARAM_ENUM[param_id].OffSet ) ENUMvalue = PARAM_ENUM[param_id].OffSet;
	if ( ENUMvalue > PARAM_ENUM[param_id].CutOff ) ENUMvalue = PARAM_ENUM[param_id].CutOff;

	if ( ENUMvalue > PARAM_ENUM[param_id].OffSet )
	{
		value = ENUMvalue - PARAM_ENUM[param_id].OffSet;
	}

	range = qAbs( PARAM_ENUM[param_id].CutOff - PARAM_ENUM[param_id].OffSet );
	if ( ( can->SOD.ENUM_UNIT[unitnum].ENUM[position - 1] < PARAM_ENUM[param_id].OffSet ) || ( can->SOD.ENUM_UNIT[unitnum].ENUM[position - 1] > PARAM_ENUM[param_id].CutOff ) ) PARAM_ENUM[param_id].OutOfRange = true;

	if ( range < PARAM_ENUM[param_id].NormalValue ) range = PARAM_ENUM[param_id].NormalValue;
	if ( PARAM_ENUM[param_id].Reverse ) value = range - value;
	prescaler = (float)PARAM_ENUM[param_id].NormalValue / range;

	res = value * prescaler;
	if ( res > PARAM_ENUM[param_id].NormalValue ) res = PARAM_ENUM[param_id].NormalValue;
	PARAM_ENUM[param_id].Value = res;
	return res;
}
