#pragma warning (push)
#pragma warning(disable: 4275)
#include "Helicopter_controls.h"
#include "Helicopter_can.h"
#include "Helicopter_server.h"
#include "Helicopter_Model.h"
#include "Helicopter_instructor.h"
#include "autopilot.h"
#pragma warning (pop)
#include <QDebug>
#include "Common/log_duration.h"
#include "Common/Printer.h"

constexpr int file_size = 30 * 60;
Helicopter_controls::Helicopter_controls( Helicopter_core* Helicopter ) :T_Controls( Helicopter->can.data( ) )
{
	Helicopter = Helicopter;

	PARAM_IN.resize( N_PARAM_IN );
	PARAM_OUT.resize( N_PARAM_OUT );
	PARAM_MAB.resize( N_PARAM_MAB );
	PARAM_ADS.resize( N_PARAM_ADS );
	PARAM_VID.resize( N_PARAM_VID );
	PARAM_CLOCK.resize( N_PARAM_CLOCK );
	PARAM_POT.resize( N_PARAM_POT );
	PARAM_LED.resize( N_PARAM_LED );
	PARAM_LCD.resize( N_PARAM_LCD );
	PARAM_ENC.resize( N_PARAM_ENC );

	OnBoard_ = new Helicopter_Onboard( Helicopter );
	OnBoard_->_audio->SoundsEnabled = Helicopter->_ini->SoundsEnabled( );

	ReadLogicINI( QCoreApplication::applicationDirPath( ).append( "/Logic_IN_OUT.ini" ) );

	LogTimer = new QTimer(this);
	LogTimer->setInterval(100);
	connect(LogTimer, SIGNAL(timeout()), this, SLOT(slotLogsSave()));

	OnBoardTimer->setInterval( 15 );  // 15мс
	connect( OnBoardTimer, SIGNAL( timeout( ) ), this, SLOT( slotValidateParameters( ) ) );
	OnBoardTimer->start( );

	//manualControl = false;
	SetManualControl( Helicopter->_ini->ManualMode( ) );
	Helicopter->can->Reset( Helicopter->PC.reset );
}

Helicopter_controls::~Helicopter_controls( )
{
	qDebug( ) << __FUNCTION__;
	OnBoardTimer->stop( );
	delete OnBoard_;
	//	delete ParamsList;
}

void Helicopter_controls::slotValidateParameters( )
{
	static int tic = 100;

	auto BoardCount = Helicopter->can->BoardCount( );
	auto CAN_Ready = Helicopter->can->CAN_Ready( );
	PRINT_CHANGED_VAR( BoardCount );
	PRINT_CHANGED_VAR( CAN_Ready );
	PRINT_CHANGED_VAR( manualControl );

	if ( Helicopter->can->CAN_Ready( ) )
	{
		if ( --tic <= 0 ) { CanHeartBeat( ); tic = 100; }; // выполняем проверку кана только каждый 100 вызов
		// чтение датчиков и управлеление приборами только при наличии КАНа
		ValidateParameters( );
		if ( Helicopter->instructor->TestVidSteps.PacketReady )
		{
			// Helicopter->PC.reset = true;
			Helicopter->can->Reset( Helicopter->PC.reset );
			// if ( TestVidSteps( ) )
			{
				Helicopter->instructor->TestVidSteps.PacketReady = false;
			}
		}
	}
	// эта функция работает при включенноом джойстике
	SubstitutionParameters( );
	// расчет параметров движения и бортовых систем работает только при запущеной модели
	ComputeParameters( );
	PrintParametrs( );
}

// функция вызывается многократно пока она возвращает false, после завершения тестирования она вернет true, 
// после этого вызов функции прекратить
bool Helicopter_controls::TestVidSteps( )
{
	struct test
	{
		int id;
		float min;
		float max;
	};
	static int time;

	static std::vector<test> _test = {
			{1,0,150},
			{2,0,100},
			{3,0,100},
			{4,0,900},
			{5,0,3},
			{6,0,100},
			{7,0,100},
			{8,-20,500},
			{9,-20,500},
			{10,-20,500},
			{11,0,30},
			{12,0,15},
			{13,0,15},
			{14,0,15},
			{15,-50,150},
			{16,-50,150},
			{17,0,8},
			{18,-50,150},
			{19,0,8},
			{20,-50,150},
			{21,0,8},
			{22,-50,150},
			{23,-1,1},
			{24,-1,1},
			{25,-1,1},
			{27,-1,1},
	};
	++time;
	switch ( time )
	{
	case 1:
		qDebug( ) << QTime::currentTime( ) << "Сбрасываем стрелки на минимум";
		for ( auto& t : _test ) { SetVIDSteps( t.id, t.min ); }
		break;
	case 300:
		qDebug( ) << QTime::currentTime( ) << "Устнавливаем стрелки на максимум";
		for ( auto& t : _test ) { SetVIDSteps( t.id, t.max ); }
		break;
	case 600:
		qDebug( ) << QTime::currentTime( ) << "Сбрасываем стрелки на минимум";
		for ( auto& t : _test ) { SetVIDSteps( t.id, t.min ); }
		break;
	case 900:
		qDebug( ) << QTime::currentTime( ) << "Завершаем тест";
		return true;
	}
	return false;
}


void Helicopter_controls::ReadLogicINI( QString INIfilename )
{
	//LOG_DURATION( "ReadLogicINI" );
	QString GPIO_IN( "GPIO_IN" );
	QString GPIO_OUT( "GPIO_OUT" );
	QString MAB( "MAB" );
	QString POT( "POT" );
	QString VID( "VID" );
	QString LCD( "LCD" );
	QString CLOCK( "CLOCK" );
	QString LED( "LED" );
	QString ENC( "ENC" );

	QSettings* ini = new QSettings( INIfilename, QSettings::IniFormat );

	if ( QFileInfo( INIfilename ).exists( ) )
	{
		ini->beginGroup( GPIO_IN );
		for ( int i = 0; i < N_PARAM_IN; i++ )
		{
			PARAM_IN[i].ParameterCaption = ini->value( "NAME#" + QString::number( i ), "NC" ).toString( );
			PARAM_IN[i].UnitNum = ini->value( "UNIT#" + QString::number( i ), 0 ).toInt( );
			PARAM_IN[i].RemapBitNum = ini->value( "BIT#" + QString::number( i ), 0 ).toInt( );
			PARAM_IN[i].InverseBit = ini->value( "INV#" + QString::number( i ), 0 ).toBool( );
		}
		ini->endGroup( );

		ini->beginGroup( ENC );
		for ( int i = 0; i < N_PARAM_ENC; i++ )
		{
			PARAM_ENC[i].ParameterCaption = ini->value( "NAME#" + QString::number( i ), "NC" ).toString( );
			PARAM_ENC[i].UnitNum = ini->value( "UNIT#" + QString::number( i ), 0 ).toInt( );
			PARAM_ENC[i].BitLEFT = ini->value( "BITLEFT#" + QString::number( i ), 0 ).toInt( );
			PARAM_ENC[i].BitRIGHT = ini->value( "BITRIGHT#" + QString::number( i ), 0 ).toInt( );
		}
		ini->endGroup( );

		ini->beginGroup( GPIO_OUT );
		for ( int i = 0; i < N_PARAM_OUT; i++ )
		{
			PARAM_OUT[i].ParameterCaption = ini->value( "NAME#" + QString::number( i ), "NC" ).toString( );
			PARAM_OUT[i].UnitNum = ini->value( "UNIT#" + QString::number( i ), 0 ).toInt( );
			PARAM_OUT[i].RemapBitNum = ini->value( "BIT#" + QString::number( i ), 0 ).toInt( );
			PARAM_OUT[i].InverseBit = ini->value( "INV#" + QString::number( i ), 0 ).toBool( );
		}
		ini->endGroup( );

		ini->beginGroup( LCD );
		for ( int i = 0; i < N_PARAM_LCD; i++ )
		{
			PARAM_LCD[i].Caption = ini->value( "NAME#" + QString::number( i ), "NC" ).toString( );
			PARAM_LCD[i].UnitNum = ini->value( "UNIT#" + QString::number( i ), 0 ).toInt( );
			PARAM_LCD[i].Lines = ini->value( "LINES#" + QString::number( i ), 0 ).toInt( );
		}
		ini->endGroup( );

		ini->beginGroup( LED );
		for ( int i = 0; i < N_PARAM_LED; i++ )
		{
			PARAM_LED[i].Caption = ini->value( "NAME#" + QString::number( i ), "NC" ).toString( );
			PARAM_LED[i].UnitNum = ini->value( "UNIT#" + QString::number( i ), 0 ).toInt( );
			PARAM_LED[i].Digits = ini->value( "DIGITS#" + QString::number( i ), 0 ).toInt( );
		}
		ini->endGroup( );

		ini->beginGroup( MAB );
		for ( int i = 0; i < N_PARAM_MAB; i++ )
		{
			PARAM_MAB[i].Caption = ini->value( "NAME#" + QString::number( i ), "NC" ).toString( );
			PARAM_MAB[i].UnitNum = ini->value( "UNIT#" + QString::number( i ), 0 ).toInt( );
			PARAM_MAB[i].RemapPosition = ini->value( "WORD#" + QString::number( i ), 0 ).toInt( );
			PARAM_MAB[i].Reverse = ini->value( "REVERSE#" + QString::number( i ), 0 ).toBool( );
			PARAM_MAB[i].OffSet = ini->value( "OFFSET#" + QString::number( i ), 0 ).toInt( );
			PARAM_MAB[i].CutOff = ini->value( "CUTOFF#" + QString::number( i ), 0 ).toInt( );
			PARAM_MAB[i].Ratio = ini->value( "RATIO#" + QString::number( i ), 1.0 ).toFloat( );
			PARAM_MAB[i].MaxValue = ini->value( "MAXVAL#" + QString::number( i ), 0 ).toInt( );
			PARAM_MAB[i].NormalValue = ini->value( "NORMVAL#" + QString::number( i ), 0 ).toInt( );
			PARAM_MAB[i].OutOfRange = false;
			PARAM_MAB[i].Value = 0;
		}
		ini->endGroup( );

		ini->beginGroup( POT );
		for ( int i = 0; i < N_PARAM_POT; i++ )
		{
			PARAM_POT[i].Caption = ini->value( "NAME#" + QString::number( i ), "NC" ).toString( );
			PARAM_POT[i].UnitNum = ini->value( "UNIT#" + QString::number( i ), 0 ).toInt( );
			PARAM_POT[i].RemapPosition = ini->value( "WORD#" + QString::number( i ), 0 ).toInt( );
			PARAM_POT[i].Reverse = ini->value( "REVERSE#" + QString::number( i ), 0 ).toBool( );
			PARAM_POT[i].OffSet = ini->value( "OFFSET#" + QString::number( i ), 0 ).toInt( );
			PARAM_POT[i].CutOff = ini->value( "CUTOFF#" + QString::number( i ), 0 ).toInt( );
			PARAM_POT[i].MaxValue = ini->value( "MAXVAL#" + QString::number( i ), 0 ).toInt( );
			PARAM_POT[i].NormalValue = ini->value( "NORMVAL#" + QString::number( i ), 0 ).toInt( );
			PARAM_POT[i].OutOfRange = false;
			PARAM_POT[i].Value = 0;
		}
		ini->endGroup( );

		ini->beginGroup( VID );
		for ( int i = 0; i < N_PARAM_VID; i++ )
		{
			PARAM_VID[i].Caption = ini->value( "NAME#" + QString::number( i ), "NC" ).toString( );
			PARAM_VID[i].UnitNum = ini->value( "UNIT#" + QString::number( i ), 0 ).toInt( );
			PARAM_VID[i].RemapPosition = ini->value( "WORD#" + QString::number( i ), 0 ).toInt( );
			for ( int j = 0; j < N_STEP_POINTS_COUNT; j++ )
			{
				PARAM_VID[i].Value_Scale_Point[j] = static_cast<FLOAT>( ini->value( "SCALE#" + QString::number( i ) + ": Point" + QString::number( j + 1 ), 0 ).toInt( ) );
				PARAM_VID[i].Value_Steps_Point[j] = ini->value( "STEPS#" + QString::number( i ) + ": Point" + QString::number( j + 1 ), 0 ).toInt( );
			}
			PARAM_VID[i].Value = 0;

			PARAM_VID[i].MIN_VAL_SCALE_POINT = PARAM_VID[i].MAX_VAL_SCALE_POINT = PARAM_VID[i].Value_Scale_Point[0]; // мин и макс равны первому значению
			for ( int j = 0; j < N_STEP_POINTS_COUNT; j++ ) // выясняем макимальную по значению прибора точку
			{
				if ( PARAM_VID[i].Value_Scale_Point[j] > PARAM_VID[i].MAX_VAL_SCALE_POINT )
				{
					PARAM_VID[i].MAX_VAL_SCALE_POINT = PARAM_VID[i].Value_Scale_Point[j];
					PARAM_VID[i].MAX_VAL_ID = j;
				}
			}
		}
		ini->endGroup( );

		ini->beginGroup( CLOCK );
		for ( int i = 0; i < N_PARAM_CLOCK; i++ )
		{
			PARAM_CLOCK[i].Caption = ini->value( "NAME#" + QString::number( i ), "NC" ).toString( );
			PARAM_CLOCK[i].UnitNum = ini->value( "UNIT#" + QString::number( i ), 0 ).toInt( );
			PARAM_CLOCK[i].RemapPosition = ini->value( "WORD#" + QString::number( i ), 0 ).toInt( );
			PARAM_CLOCK[i].Range = ini->value( "RANGE#" + QString::number( i ), 0 ).toFloat( );
			PARAM_CLOCK[i].Start = ini->value( "START#" + QString::number( i ), 0 ).toInt( );
			//PARAM_CLOCK[i].Revers = (ini->value( "REVERSE#" + QString::number( i ), 0 ).toInt( ) == 0 )? 1: -1;
		}
		ini->endGroup( );
	}
	ini->destroyed( );
}

//чтение данных из CAN и FM
void Helicopter_controls::ValidateParameters( )
{
	Helicopter->_BoardSystem->tablo = 12345600;
	if ( !manualControl && ( Helicopter->can->BoardCount( ) > 0 ) )
	{
		// тут можно сделать необходимые мвнипуляции с данными до момента отправки их в СОД
		// например объедить строки для отправки их в LED
		Helicopter->_BoardSystem->tablo = Helicopter->UV26.tablo*100000 + static_cast<int>(Helicopter->rpb.snos.speed) * 100;
		if (!Helicopter->Is27V())
			Helicopter->_BoardSystem->tablo = 0xffff;
		//Helicopter->PC.f_compass = 360.f - FPar->Yaw * Ra - Helicopter->rpb.ugr_4uk.magnetic; // Helicopter->lpb.ugr_4uk.dir();

		Helicopter->can->setGPIOOUT_Ready( false );
		Helicopter->can->SOD_Clear_TMP_OUT_Data( );
		CanExchange( );
		ENCReset( );
		// Тут мы при необходимости модифицируем считанных из КАН значения
		/*Helicopter->rsp_elp.t_ko50_full = !Helicopter->rsp_elp.t_ko50_midl;
		if (fabs(Helicopter->ccp.f_set_curs - Helicopter->_BoardSystem->enc_course->get_prev()) < 0.1f)
			Helicopter->ccp.f_set_curs = 0;
		else
			Helicopter->ccp.f_set_roll = Helicopter->_BoardSystem->enc_roll->pos(Helicopter->ccp.f_set_roll - 5.);
		if (fabs(Helicopter->ccp.f_set_roll - Helicopter->_BoardSystem->enc_roll->get_prev()) < 0.1f)
			Helicopter->ccp.f_set_roll = 0;
		else
			Helicopter->ccp.f_set_pitch = Helicopter->_BoardSystem->enc_pitch->pos(Helicopter->ccp.f_set_pitch - 5.);
		if (fabs(Helicopter->ccp.f_set_pitch - Helicopter->_BoardSystem->enc_pitch->get_prev()) < 0.1f)
			Helicopter->ccp.f_set_pitch = 0;
		else
			Helicopter->ccp.f_set_pitch = Helicopter->_BoardSystem->enc_pitch->pos(Helicopter->ccp.f_set_pitch - 5.);
		qDebug() << "Manual AP curs=" << Helicopter->ccp.f_set_curs;
		qDebug() << "Manual AP pitch=" << Helicopter->ccp.f_set_pitch;
		qDebug() << "Manual AP roll=" << Helicopter->ccp.f_set_roll;*/


		PRINT_CHANGED_VAR(Helicopter->ccp.f_set_pitch);
		if ( Helicopter->rpb.avg.up ) Helicopter->rpb.avg.encoder = 1;
		if ( Helicopter->rpb.avg.down ) Helicopter->rpb.avg.encoder = -1;
		//Helicopter->lpb.ki_206.norm( );

		Helicopter->can->setGPIOOUT_Ready( true );
	}
	// пояснение: тут можно было бы сразу присвоить false без условия, думаю компилятор досточно умный
	// что бы сделать это свмостоятельно не добавляя тактов на проверку
	// это написано для человека, суть в том, что переход из true в false включает вибру и производит 
	// сброс СОДа, если же просто написать Helicopter->PC.reset = false то у программиста может возникнуть 
	// мысль, что это инициализация переменной и ее можно убрать из цикла в конструктор, а это ошибка.
	/*if (Helicopter->PC.reset == true)
	{
		Helicopter->PC.reset = false;
		Helicopter->can->setRXEnabled( false ); 
		Helicopter->can->setTXEnabled( false );
	}*/
	if ( Helicopter->PC.vibro == true ) Helicopter->PC.vibro = false;
}

void Helicopter_controls::Start( )
{
	Helicopter->can->Reset( Helicopter->PC.reset );
	Helicopter->PC.vibro = false;
}

void Helicopter_controls::Stop( )
{
	//Helicopter->PC.reset = true;
	Helicopter->can->Reset( Helicopter->PC.reset );
	Helicopter->PC.vibro = true;
}

// расчет дополнительных параметров
void Helicopter_controls::ComputeParameters( )
{
	if ( Helicopter->instructor->exerciseReInit )
	{
		Helicopter->Start( );
		Helicopter->instructor->exerciseReInit = false;
	}
	if ( Helicopter->instructor->exerciseStop )
	{
		Helicopter->Stop( );
		Helicopter->instructor->exerciseStop = false;
	}
	// если упражнение не запущено дальше не обрабатываем
	if ( !Helicopter->instructor->exerciseStarted )
	{
		Helicopter->PC.vibro = true;  // виро включется при переходе с высокого уровня на низкий,поэтому держим высокий пока не включена симуляция
		return;
	}
	// вычисление показателей бортовых систем
	OnBoard_->Onboard_Compute( COMPUTING_INTERVAL / 1000.0f );
	if ( Helicopter->PC.t_left_trim )
	{
		Helicopter->PC.set_trim( );
	}
	
	Helicopter->cep.f_t_ta = Helicopter->_BoardSystem->_VSU->temp( );
	Helicopter->cep.f_p_ta = Helicopter->_BoardSystem->_VSU->presure( );
	Helicopter->cep.i_ta_automat_on = Helicopter->_BoardSystem->_VSU->ta_automat_on;
	Helicopter->cep.i_p_oil_norm = Helicopter->_BoardSystem->_VSU->ta_p_oil_norm;
	Helicopter->cep.i_rpm_norm = Helicopter->_BoardSystem->_VSU->ta_rpm_norm;
	Helicopter->cep.i_rpm_max = Helicopter->_BoardSystem->_VSU->ta_rpm_max;
	Helicopter->cep.i_eng_automat_on = Helicopter->_BoardSystem->eng_automat_on;
	Helicopter->cep.i_ta_work = Helicopter->_BoardSystem->ta_starter_on;

	//SetIndicators(Helicopter_->ccp.t_check_lamp);

	Helicopter->cep.i_fuel_left_close = !Helicopter->cep.t_fuel_left_close && Helicopter->rp_azs.t_close_left && Helicopter->_BoardSystem->Is27V( );
	Helicopter->cep.i_fuel_right_close = !Helicopter->cep.t_fuel_right_close && Helicopter->rp_azs.t_close_rght && Helicopter->_BoardSystem->Is27V( );
	Helicopter->cep.i_fuel_bypass_close = !Helicopter->cep.t_fuel_bypass_close && Helicopter->rp_azs.t_close_bypass && Helicopter->_BoardSystem->Is27V( );
	Helicopter->cep.i_fpump_consum_off = (!Helicopter->cep.t_fpump_consum && Helicopter->rp_azs.t_pump_consum && Helicopter->_BoardSystem->Is27V( )) || Helicopter->instructor->fault.FuelPumpRash;
	Helicopter->cep.i_fpump_left_off = (!Helicopter->cep.t_fpump_left && Helicopter->rp_azs.t_pump_left && Helicopter->_BoardSystem->Is27V( )) || (Helicopter->instructor->fault.FuelPumpPereLeft || Helicopter->instructor->fault.FuelPumpPereTWO);
	Helicopter->cep.i_fpump_right_off = (!Helicopter->cep.t_fpump_right || !Helicopter->rp_azs.t_pump_right || !Helicopter->_BoardSystem->Is28V() || Helicopter->instructor->fault.FuelPumpPereTWO) && Helicopter->_BoardSystem->Is27V( );
	Helicopter->cep.i_contr_sensors = (Helicopter->cep.t_contr_sensors && Helicopter->rp_azs.t_ppo_signal) && Helicopter->_BoardSystem->Is27V( );
	Helicopter->rpb.diss_15.i_on = Helicopter->rpb.diss_15.state && Helicopter->_BoardSystem->Is27V( );

	Helicopter->lsp_elp.f_p_air = Helicopter->_BoardSystem->_Pneumatic->_main;
	Helicopter->lsp_elp.f_p_brake = Helicopter->_BoardSystem->_Pneumatic->_break;

	//Helicopter->rsp_ep.i_pzu_left_on = Helicopter->rsp_ep.t_pzu_left && Helicopter->_BoardSystem->Is27V( );
	//Helicopter->rsp_ep.i_pzu_right_on = Helicopter->rsp_ep.t_pzu_right && Helicopter->_BoardSystem->Is27V( );


	//Helicopter->lp_elp.i_heat_left = Helicopter->rsp_ep.t_pzu_left &&
	//	Helicopter->_BoardSystem->Is27V( ) && Helicopter->_BoardSystem->Is208V( );
	//Helicopter->lp_elp.i_left_pzu_front = Helicopter->rsp_ep.t_pzu_left &&
	//	Helicopter->_BoardSystem->Is27V( ) && Helicopter->_BoardSystem->Is208V( );
	//Helicopter->lp_elp.i_left_pzu_back = Helicopter->rsp_ep.t_pzu_left &&
	//	Helicopter->_BoardSystem->Is27V( ) && Helicopter->_BoardSystem->Is208V( );

	// TODO надо разобраться какие индикаторы точно красные
	// горят красные аварии
	Helicopter->lpb.i_cso1_red = (Helicopter->ep_elp.i_gen_1_off || Helicopter->ep_elp.i_gen_2_off || Helicopter->rpb.i_ag_brok || Helicopter->rpb.i_diss_brok ||
		Helicopter->cep.i_eng_left_fire || Helicopter->cep.i_eng_rght_fire || Helicopter->cep.i_ko_50_fire || Helicopter->cep.i_red_vsu_fire ||
		Helicopter->cep.i_hidro_second || Helicopter->rsp_ep.i_low_lock || Helicopter->lpb.i_ag_left_brok || Helicopter->lpb.i_ag_res_brok || Helicopter->cep.i_ta_brok ||
		Helicopter->rpb.i_fuel_270) && Helicopter->_BoardSystem->Is27V();
	// Helicopter->lpb.i_eng_left_off || Helicopter->lpb.i_eng_right_off;  // << вроде, должны быть красными у нас желные
// желтенкие горят
	Helicopter->lpb.i_cso1_yellow = (Helicopter->lpb.i_ag_net_kontr || Helicopter->lpb.i_roll_max || Helicopter->lsh_elp.i_ri_65 ||
		Helicopter->cep.i_eng_left_1_queue || Helicopter->cep.i_eng_left_2_queue ||
		Helicopter->cep.i_eng_rght_1_queue || Helicopter->cep.i_eng_rght_2_queue ||
		Helicopter->cep.i_ko_50_1_queue || Helicopter->cep.i_ko_50_2_queue ||
		Helicopter->cep.i_red_vsu_1_queue || Helicopter->cep.i_red_vsu_2_queue ||
		Helicopter->cep.i_contr_sensors || Helicopter->cep.i_fpump_left_off ||
		Helicopter->cep.i_fpump_right_off || Helicopter->cep.i_fpump_consum_off ||
		Helicopter->cep.i_fuel_bypass_close || Helicopter->cep.i_fuel_left_close || Helicopter->cep.i_fuel_right_close ||
		Helicopter->rsp_elp.i_ko50_heater || Helicopter->rsp_elp.i_ko50_ignition ||
		Helicopter->lpb.i_main_gear_swarf || Helicopter->lpb.i_inter_gear_swarf || Helicopter->lpb.i_tail_gear_swarf ||
		Helicopter->lpb.i_eng_left_vibr_max || Helicopter->lpb.i_eng_left_swarf || Helicopter->lpb.i_eng_left_tf ||
		Helicopter->lpb.i_eng_left_max_rpm || Helicopter->lpb.i_eng_left_emrg ||
		Helicopter->lpb.i_eng_left_p_oil || Helicopter->lpb.i_eng_left_erd_off ||
		Helicopter->lpb.i_eng_right_vibr_max || Helicopter->lpb.i_eng_right_swarf || Helicopter->lpb.i_eng_right_tf ||
		Helicopter->lpb.i_eng_right_max_rpm || Helicopter->lpb.i_eng_right_emrg ||
		Helicopter->lpb.i_eng_right_p_oil || Helicopter->lpb.i_eng_right_erd_off ||
		Helicopter->ep_elp.i_vu1_brok || Helicopter->ep_elp.i_vu2_brok || Helicopter->ep_elp.i_reser36 || Helicopter->ep_elp.i_ptc_on ||
		Helicopter->ep_elp.i_resev_on/* || Helicopter->ep_elp.i_rap_on*/) && Helicopter->_BoardSystem->Is27V();
	
	Helicopter->PC.i_weapon_warning = Helicopter->PC.t_safety_lock;

	auto FPar = Helicopter->Model_->GetFPar( );

	//Helicopter->lpb.i_roll_max = ( qAbs( FPar->Roll * Ra ) > (Helicopter->lpb.t_roll_33 ? 33.f : 15.f) );
	// Предполетная проверка БКК
	if (Helicopter->lsh_elp.t_bkk_18 && (Helicopter->lsh_elp.t_bkk_contr_down || Helicopter->lsh_elp.t_bkk_contr_up)) {
		Helicopter->lpb.i_roll_max = true && !Helicopter->instructor->fault.BKK18; //интеграция отказа БКК с инструктора
		Helicopter->rpb.i_roll_max = true && !Helicopter->instructor->fault.BKK18; //интеграция отказа БКК с инструктора
		Helicopter->lpb.i_ag_left_brok = Helicopter->lpb.i_ag_res_brok = Helicopter->rpb.i_ag_brok = Helicopter->lpb.avg_res.blank = Helicopter->lpb.avg_left.blank  = Helicopter->rpb.avg.blank = Helicopter->Is27V();
	}
	else {
		Helicopter->lpb.i_roll_max = (qAbs(FPar->Roll * Ra) > (Helicopter->lpb.t_roll_33 ? 33.f : 15.f)) && !Helicopter->instructor->fault.BKK18; //интеграция отказа БКК с инструктора
		Helicopter->rpb.i_roll_max = (qAbs(FPar->Roll * Ra) > (Helicopter->lpb.t_roll_33 ? 33.f : 15.f)) && !Helicopter->instructor->fault.BKK18; //интеграция отказа БКК с инструктора
		Helicopter->lpb.avg_left.blank = !(Helicopter->lsh_elp.t_ag_left && Helicopter->Is115V());
		Helicopter->lpb.i_ag_left_brok = (Helicopter->lpb.avg_left.blank && Helicopter->Is27V()) || Helicopter->instructor->fault.AviaHorizont_two;
		Helicopter->lpb.avg_res.blank = !(Helicopter->lsh_elp.t_ag_res && Helicopter->Is115V()) || Helicopter->instructor->fault.AviaHorizont;
		Helicopter->lpb.i_ag_res_brok = Helicopter->lpb.avg_res.blank && Helicopter->Is27V();
		Helicopter->rpb.avg.blank = !(Helicopter->rsh_elp.t_ag_right && Helicopter->Is115V()) || Helicopter->instructor->fault.AviaHorizont_two;
		Helicopter->rpb.i_ag_brok = Helicopter->rpb.avg.blank && Helicopter->Is27V();
	}
	Helicopter->instructor->Avia.Pitch = FPar->Pitch * Ra;
	Helicopter->instructor->Avia.Roll  = FPar->Roll * Ra;
	Helicopter->instructor->Avia.Curse = 360.f - FPar->Yaw * Ra;
	Helicopter->instructor->Avia.Speed = FPar->Vpr * MS2KM;
	Helicopter->instructor->Avia.Longitude = static_cast<float>( Helicopter->_BoardSystem->Longitude );
	Helicopter->instructor->Avia.Latitude = static_cast<float>( Helicopter->_BoardSystem->Latitude );
	Helicopter->instructor->Avia.Altitude = FPar->Hbar;
	Helicopter->instructor->Avia.Eng1 = Helicopter->_BoardSystem->_Engine_1->is_working( );
	Helicopter->instructor->Avia.Eng2 = Helicopter->_BoardSystem->_Engine_2->is_working( );
	Helicopter->instructor->Avia.Colision = 0;
	Helicopter->instructor->Avia.TechID = Helicopter->_ini->SimulatorType();
	Helicopter->instructor->Avia.PacketReady = true;

	Helicopter->PC.t_left_fire_prev = Helicopter->PC.t_left_fire;
	Helicopter->PC.pkt_fire_prev = Helicopter->PC.pkt_fire;
}

// подмена параметров при использовании джойстика
void Helicopter_controls::SubstitutionParameters( )
{
	// TODO добавить инструктора
	//if (_Instructor->ExerciseParameters->Joystick && _Joystick->JoystickActive() && !ManualControl())
	if ( Helicopter->joystick_->JoystickActive( ) )
	{
		// тут передаем в ММ данные с джойстика
		JoystickInput js = Helicopter->joystick_->joystickInput;
		Helicopter->PC.f_roll = js.X; // Штурвал ВЛЕВО-ВПРАВО
		Helicopter->PC.f_pitch = js.Y; // Штурвал ВПЕРЕД-НАЗАД
		Helicopter->PC.f_yawl = js.Xp; // Педали Пилот
		Helicopter->PC.f_eng1 = js.CorLEng; // Руд левый
		Helicopter->PC.f_eng2 = js.CorREng; // Руд правый
		Helicopter->PC.f_Fosh = js.Fosh; // Рукоятка шаг-газ
		Helicopter->PC.f_Ecor = js.Ecor; // Ручка коррекции
		Helicopter->PC.f_break = js.Break; // тормоз колес

		//Helicopter->PC.t_left_fire_prev = Helicopter->PC.t_left_fire;
		Helicopter->PC.t_left_fire = js.Fire;
		Helicopter->PC.t_right_fire = js.Save;

		Helicopter->PC.t_left_ap_off = js.APOff;  // Лев штурвал - Кнопка ОТКЛЮЧЕНИЕ АВТОПИЛОТА
		Helicopter->PC.t_left_trim = js.Trim;     // Лев штурвал - Кнопка триммирования

		if (js.Engine1) 
		{
			Helicopter->_BoardSystem->StartAll( );
			Helicopter->_BoardSystem->_Engine_1->start_eng( );
		}
		if (js.Engine2) {
			Helicopter->_BoardSystem->_Engine_2->start_eng( );
		}
		if ( js.ADirOn )
		{
			Helicopter->ccp.t_ap_dir_on = true;
			Helicopter->ccp.t_ap_dir_off = false;
		}
		if ( js.ADirOff )
		{
			Helicopter->ccp.t_ap_dir_off = true;
			Helicopter->ccp.t_ap_dir_on = false;
		}
		if ( js.APitchOn )
		{
			Helicopter->ccp.t_ap_on = true;
		}
		if ( js.AAltOn )
		{
			Helicopter->ccp.t_ap_alt_on = true;
			Helicopter->ccp.t_ap_alt_off = false;
		}
		if ( js.AAltOff )
		{
			Helicopter->ccp.t_ap_alt_off = true;
			Helicopter->ccp.t_ap_alt_on = false;
		}
	}
}

void Helicopter_controls::slotLogsSave()
{
	FParam* FPar = Helicopter->Model_->GetFPar();
	IParam* IPar = Helicopter->Model_->GetIPar();
	if (creating) {
		QString filename = QDateTime::currentDateTime().toString("'data_'yyyy_MM_dd_hh_mm'.csv'");
		out = new QFile(filename);
		if (out->open(QIODevice::WriteOnly)) {
			stream = new QTextStream(out);
		}
		creating = false;
		*stream << Helicopter->Model_->header().toUtf8( ) << endl;
	}
	if (saving)
	{
		*stream << *FPar << "," << *IPar << endl;
	}
	if (closing)
	{
		closing = false;
		saving = false;
		creating = true;
		LogTimer->stop();
		delete stream;
		out->close();
		delete out;
	}
}

void Helicopter_controls::PrintTumblers( )
{

//Левая боковая панель электропульта
	PRINT_CHANGED_VAR( Helicopter->lsp_elp.t_IV_500 );//Кнопка НВ-500Е
	PRINT_CHANGED_VAR( Helicopter->lsp_elp.t_ground );//Кнопка ЗЕМЛЯ
	PRINT_CHANGED_VAR( Helicopter->lsp_elp.t_air );//Кнопка ВОЗДУХ
	PRINT_CHANGED_VAR( Helicopter->lsp_elp.t_ano );//Кнопка КОД-АНО
	PRINT_CHANGED_VAR( Helicopter->lsp_elp.t_siren );//Кнопка СИРЕНА
	PRINT_CHANGED_VAR( Helicopter->lsp_elp.t_ext_lock );//Тумблер ВНЕШНЯЯ ПОДВЕСКА
	PRINT_CHANGED_VAR( Helicopter->lsp_elp.t_ri_off );//Кнопка ОТКЛ
	PRINT_CHANGED_VAR( Helicopter->lsp_elp.t_ri_rep );//Кнопка ПОВТ
	PRINT_CHANGED_VAR( Helicopter->lsp_elp.t_ri_check );//Кнопка ПРОВЕРКА
	PRINT_CHANGED_VAR( Helicopter->lsp_elp.t_ri_gain );//Тумблер УСИЛ
	PRINT_CHANGED_VAR( Helicopter->lsp_elp.t_autostart );//Тумблер АВТОПУСК
	PRINT_CHANGED_VAR( Helicopter->lsp_elp.i_siren_on );//Индикатор СИРЕНА ВКЛЮЧЕНА

//ЛЕВЫЙ ЩИТОК ЭЛЕКТРОПУЛЬТА
	PRINT_CHANGED_VAR( Helicopter->lsh_elp.t_plafon );//Тумблер ПЛАФОН	bool
	PRINT_CHANGED_VAR( Helicopter->lsh_elp.t_ag_left );//Тумблер АГ ЛЕВ	bool
	PRINT_CHANGED_VAR( Helicopter->lsh_elp.t_ag_res );//Тумблер АГ РЕЗЕРВ	bool
	PRINT_CHANGED_VAR( Helicopter->lsh_elp.t_bkk_18 );//Тумблер БКК-18	bool
	PRINT_CHANGED_VAR( Helicopter->lsh_elp.t_bkk_contr_up );//Тумблер I КОНТР БКК	bool
	PRINT_CHANGED_VAR( Helicopter->lsh_elp.t_bkk_contr_down );//Тумблер II КОНТР БКК	bool
	PRINT_CHANGED_VAR( Helicopter->lsh_elp.t_ri_65 );//Тумблер РИ-65	bool
	PRINT_CHANGED_VAR( Helicopter->lsh_elp.t_pvd_heat );//Кнопка КОНТРОЛЬ ОБОГРЕВА ПВД	bool
	PRINT_CHANGED_VAR( Helicopter->lsh_elp.sw_glass_wiper );//Переключатель СТЕКЛООЧИСТИТЕЛЬ	uint8
	PRINT_CHANGED_VAR( Helicopter->lsh_elp.t_rv );//Тумблер выключатель радиовысотомера	bool
	PRINT_CHANGED_VAR( Helicopter->lsh_elp.t_spuu_52 );//Тумблер Выключатель СПУУ-52 bool
	PRINT_CHANGED_VAR( Helicopter->lsh_elp.i_ri_65 );//Индикатор ВКЛЮЧИ РИ-65	bool
	PRINT_CHANGED_VAR( Helicopter->lsh_elp.i_bkk_ok );//Индикатор ИСПРАВН. БКК	bool
	PRINT_CHANGED_VAR( Helicopter->lsh_elp.i_pvd_heat_ok );//Индикатор ОБОГРЕВ ИСПРАВЕН	bool

//ЛЕВАЯ ПАНЕЛЬ ЭЛЕКТРОПУЛЬТА
	PRINT_CHANGED_VAR( Helicopter->lp_elp.t_bark_eng_left_on );              //Тумблер ЛЕВЫЙ ВКЛ	bool
	PRINT_CHANGED_VAR( Helicopter->lp_elp.t_bark_eng_left_wep );              //Тумблер ЛЕВЫЙ ЧР	bool
	PRINT_CHANGED_VAR( Helicopter->lp_elp.t_bark_eng_left_ct1 );              //Тумблер СТ1 прав	bool
	PRINT_CHANGED_VAR( Helicopter->lp_elp.t_bark_eng_left_ct2 );              //Тубмлер СТ2 прав	bool
	PRINT_CHANGED_VAR( Helicopter->lp_elp.t_bark_eng_tk_right );              //Тумблер ТК ПРАВ	bool
	PRINT_CHANGED_VAR( Helicopter->lp_elp.t_bark_eng_tk_left );              //Тубмлер ТК ЛЕВ	bool
	PRINT_CHANGED_VAR( Helicopter->lp_elp.t_bark_eng_right_ct1 );              //Тумблер СТ1 лев	bool
	PRINT_CHANGED_VAR( Helicopter->lp_elp.t_bark_eng_right_ct2 );              //Тубмлер СТ2 лев	bool
	PRINT_CHANGED_VAR( Helicopter->lp_elp.t_bark_eng_right_wep );              //Тумблер ПРАВЫЙ ЧР	bool
	PRINT_CHANGED_VAR( Helicopter->lp_elp.t_bark_eng_right_on );              //Тумблер ПРАВЫЙ ВКЛ	bool

// Пульт Контроля ДИСС
	PRINT_CHANGED_VAR( Helicopter->pc_diss.i_m );//Индикатор М	bool
	PRINT_CHANGED_VAR( Helicopter->pc_diss.i_contr );//Индикатор КОНТР	bool
	PRINT_CHANGED_VAR( Helicopter->pc_diss.i_work );//Индикатор РАБОТА	bool
	PRINT_CHANGED_VAR( Helicopter->pc_diss.i_b );//Индикатор В	bool
	PRINT_CHANGED_VAR( Helicopter->pc_diss.sw_mode );//Галетный Переключатель	uint8

// Левая Приборная Панель
	PRINT_CHANGED_VAR( Helicopter->lpb.t_rulejka );                //Тумблер РУЛЕЖ	bool
	PRINT_CHANGED_VAR( Helicopter->lpb.t_svet_ubran );                //Тумблер СВЕТ	bool	Убрана
	PRINT_CHANGED_VAR( Helicopter->lpb.t_svet );                //Тумблер СВЕТ	bool	Свет
	PRINT_CHANGED_VAR( Helicopter->lpb.t_fpp );                //Тумблер УПРАВЛ	bool	ФПП
	PRINT_CHANGED_VAR( Helicopter->lpb.t_project );                //Тумблер УПРАВЛ	bool	ПРОЖЕКТ
	PRINT_CHANGED_VAR( Helicopter->lpb.sw_PVD );              //Переключатель СТАТИЧЕСКАЯ СИСТЕМА ПВД	uint8	0-левая, 1-объединенная, 2-правая

	PRINT_CHANGED_VAR( Helicopter->lpb.rv_5.t_test );                //ВЫСОТОМЕР-Кнопка ТЕСТ
	PRINT_CHANGED_VAR( Helicopter->lpb.rv_5.t_triangle );                //ВЫСОТОМЕР-Кнопка с треугольником
	PRINT_CHANGED_VAR( Helicopter->lpb.t_apk_sv );                //Тумблер АРК	bool	АРК СВ
	PRINT_CHANGED_VAR( Helicopter->lpb.t_apk_ukv );                //Тумблер АРК	bool	АРК УКВ
	PRINT_CHANGED_VAR( Helicopter->lpb.avg_left.blank );         //АВИАГОРИЗОНТ ЛЕВ:Флажок отс. питания	bool
	PRINT_CHANGED_VAR( Helicopter->lpb.avg_left.shift_pitch );         //АВИАГОРИЗОНТ ЛЕВ
	PRINT_CHANGED_VAR( Helicopter->lpb.avg_left.encoder );         //АВИАГОРИЗОНТ ЛЕВ:
	PRINT_CHANGED_VAR( Helicopter->lpb.ss.i_ss );               //Индикатор МС - лампа ВЫК
	PRINT_CHANGED_VAR( Helicopter->lpb.avg_res.blank );    //АВИАГОРИЗОНТ РЕЗ:Флажок отс. питания	bool
	PRINT_CHANGED_VAR( Helicopter->lpb.avg_res.shift_pitch );    //АВИАГОРИЗОНТ РЕЗ
	PRINT_CHANGED_VAR( Helicopter->lpb.avg_res.encoder );    //АВИАГОРИЗОНТ РЕЗ:
	PRINT_CHANGED_VAR( Helicopter->lpb.i_ag_left_brok );    //Индикатор ОТКАЗ АГ ЛЕВ	bool
	PRINT_CHANGED_VAR( Helicopter->lpb.i_roll_max );        //Индикатор КРЕН ВЕЛИК	bool
	PRINT_CHANGED_VAR( Helicopter->lpb.i_ag_net_kontr );    //Индикатор НЕТ КОНТР АГ	bool
	PRINT_CHANGED_VAR( Helicopter->lpb.i_ag_res_brok );     //Индикатор ОТКАЗ АГ РЕЗЕРВ	bool
	PRINT_CHANGED_VAR( Helicopter->lpb.i_main_gear_swarf ); //Индикатор СТРУЖКА ГЛАВ. РЕД.	bool
	PRINT_CHANGED_VAR( Helicopter->lpb.i_inter_gear_swarf );//Индикатор СТРУЖКА ПРОМ. РЕД.	bool
	PRINT_CHANGED_VAR( Helicopter->lpb.i_tail_gear_swarf ); //Индикатор СТРУЖКА ХВ. РЕДУК.	bool
	PRINT_CHANGED_VAR( Helicopter->lpb.i_eng_left_vibr_max );//Индикатор ЛЕВ. ДВ. ВИБР. ПОВ.	bool
	PRINT_CHANGED_VAR( Helicopter->lpb.i_eng_left_swarf );   //Индикатор СТРУЖКА ЛЕВ.ДВ.	bool
	PRINT_CHANGED_VAR( Helicopter->lpb.i_eng_left_tf );      //Индикатор ЗАСОР. ТФ. ЛЕВ. ДВ.	bool
	PRINT_CHANGED_VAR( Helicopter->lpb.i_eng_left_max_rpm ); //Индикатор ПЕРЕВ ПСТ ЛЕВ. ДВ.	bool
	PRINT_CHANGED_VAR( Helicopter->lpb.i_eng_left_emrg );    //Индикатор Ч.Р.ЛЕВ.ДВ.	bool
	PRINT_CHANGED_VAR( Helicopter->lpb.i_eng_left_p_oil );   //Индикатор МАЛО РМ ЛЕВ. ДВ.	bool
	PRINT_CHANGED_VAR( Helicopter->lpb.i_eng_left_erd_off ); //Индикатор ОТКЛ. ЭРД. ЛЕВ. ДВ.	bool
	PRINT_CHANGED_VAR( Helicopter->lpb.i_eng_right_vibr_max );//Индикатор ПРАВ.ДВ.ВИБР.ПОВ.	bool
	PRINT_CHANGED_VAR( Helicopter->lpb.i_eng_right_swarf );   //Индикатор СТРУЖКА ПРАВ.ДВ.	bool
	PRINT_CHANGED_VAR( Helicopter->lpb.i_eng_right_tf );      //Индикатор ЗАСОР. ТФ. ПРАВ. ДВ.	bool
	PRINT_CHANGED_VAR( Helicopter->lpb.i_eng_right_max_rpm ); //Индикатор ПЕРЕВ ПСТ ПРАВ. ДВ.	bool
	PRINT_CHANGED_VAR( Helicopter->lpb.i_eng_right_emrg );    //Индикатор Ч.Р.ПРАВ.ДВ.	bool
	PRINT_CHANGED_VAR( Helicopter->lpb.i_eng_right_p_oil );   //Индикатор МАЛО РМ. ПРАВ. ДВ.	bool
	PRINT_CHANGED_VAR( Helicopter->lpb.i_eng_right_erd_off ); //Индикатор ОТКЛ. ЭРД. ПРАВ. ДВ	bool
	PRINT_CHANGED_VAR( Helicopter->lpb.t_max_roll );          //Тумблер ПРЕДЕЛЬН КРЕН	bool

// Средняя Панель Электропульта
	PRINT_CHANGED_VAR( Helicopter->cep.i_eng_left_fire );    //Индикатор ПОЖАР ЛЕВ ДВ	bool
	PRINT_CHANGED_VAR( Helicopter->cep.i_eng_left_1_queue );    //Индикатор ЛЕВ ДВ I ОЧЕРЕДЬ	bool
	PRINT_CHANGED_VAR( Helicopter->cep.i_eng_left_2_queue );    //Индикатор ЛЕВ ДВ II ОЧЕРЕДЬ	bool
	PRINT_CHANGED_VAR( Helicopter->cep.t_eng_left_1_queue );    //Кнопка I ОЧЕРЕДЬ ЛЕВ ДВ	bool
	PRINT_CHANGED_VAR( Helicopter->cep.t_eng_left_2_queue );    //Кнопка II ОЧЕРЕДЬ ЛЕВ ДВ	bool
	PRINT_CHANGED_VAR( Helicopter->cep.i_eng_rght_fire );    //Индикатор ПОЖАР ПРАВ ДВ	bool
	PRINT_CHANGED_VAR( Helicopter->cep.i_eng_rght_1_queue );    //Индикатор ПРАВ ДВ I ОЧЕРЕДЬ	bool
	PRINT_CHANGED_VAR( Helicopter->cep.i_eng_rght_2_queue );    //Индикатор ПРАВ ДВ II ОЧЕРЕДЬ	bool
	PRINT_CHANGED_VAR( Helicopter->cep.t_eng_rght_1_queue );    //Кнопка I ОЧЕРЕДЬ ПРАВ ДВ	bool
	PRINT_CHANGED_VAR( Helicopter->cep.t_eng_rght_2_queue );    //Кнопка II ОЧЕРЕДЬ ПРАВ ДВ	bool
	PRINT_CHANGED_VAR( Helicopter->cep.i_ko_50_fire );    //Индикатор ПОЖАР КО-50	bool
	PRINT_CHANGED_VAR( Helicopter->cep.i_ko_50_1_queue );    //Индикатор КО-50 I ОЧЕРЕДЬ	bool
	PRINT_CHANGED_VAR( Helicopter->cep.i_ko_50_2_queue );    //Индикатор КО-50 II ОЧЕРЕДЬ	bool
	PRINT_CHANGED_VAR( Helicopter->cep.t_ko_50_1_queue );    //Кнопка I ОЧЕРЕДЬ КО-50	bool
	PRINT_CHANGED_VAR( Helicopter->cep.t_ko_50_2_queue );    //Кнопка II ОЧЕРЕДЬ КО-50	bool
	PRINT_CHANGED_VAR( Helicopter->cep.i_red_vsu_fire );    //Индикатор ПОЖАР РЕД. ВСУ	bool
	PRINT_CHANGED_VAR( Helicopter->cep.i_red_vsu_1_queue );    //Индикатор РЕД ВСУ I ОЧЕРЕДЬ	bool
	PRINT_CHANGED_VAR( Helicopter->cep.i_red_vsu_2_queue );    //Индикатор РЕД ВСУ II ОЧЕРЕДЬ	bool
	PRINT_CHANGED_VAR( Helicopter->cep.t_red_vsu_1_queue );    //Кнопка I ОЧЕРЕДЬ РЕД ВСУ	bool
	PRINT_CHANGED_VAR( Helicopter->cep.t_red_vsu_2_queue );    //Кнопка II ОЧЕРЕДЬ РЕД ВСУ	bool
	PRINT_CHANGED_VAR( Helicopter->cep.t_contr_sensors );    //Тумблер КОНТРОЛЬ ДАТЧИКОВ	bool
	PRINT_CHANGED_VAR( Helicopter->cep.t_firefighting );    //Тумблер ОГНЕТУШЕНИЕ	bool
	PRINT_CHANGED_VAR( Helicopter->cep.t_1 );    //Тумблер КОНТРОЛЬ ПИРОПАТРОНОВ	bool
	PRINT_CHANGED_VAR( Helicopter->cep.t_2 );    //Тумблер II	bool
	PRINT_CHANGED_VAR( Helicopter->cep.sw_contr_sensors );    //Переключатель КОНТРОЛЬ ДАТЧИКОВ	uint8	0-вык,1-контроль, 2-7:1-6 каналы
	PRINT_CHANGED_VAR( Helicopter->cep.i_contr_sensors );    //Индикатор КОНТРОЛЬ ДАТЧИКОВ	bool
	PRINT_CHANGED_VAR( Helicopter->cep.t_off_second_hidro );    // Кнопка ОТКЛ ДУБЛИР
	PRINT_CHANGED_VAR( Helicopter->cep.t_hidro_main );    //Тумблер ГИДРОСИСТЕМА ОСНОВН	bool	true=вкл
	PRINT_CHANGED_VAR( Helicopter->cep.i_hidro_main );    //Индикатор ОСНОВН ВКЛЮЧЕНА	bool
	PRINT_CHANGED_VAR( Helicopter->cep.i_hidro_second );    //Тумблер ГИДРОСИСТЕМА ДУБЛИР	bool	true=вкл
	PRINT_CHANGED_VAR( Helicopter->cep.t_hidro_second );    //Индикатор ДУБЛИР ВКЛЮЧЕНА	bool
	PRINT_CHANGED_VAR( Helicopter->cep.t_ta_sel_false_start );    //Тумблер ПРОКРУТ bool
	PRINT_CHANGED_VAR( Helicopter->cep.t_ta_sel_start );    //Тумблер ЗАПУСК	bool
	PRINT_CHANGED_VAR( Helicopter->cep.t_ta_start );    //Кнопка ЗАПУСК ТУРБОАГРЕГАТА	bool
	PRINT_CHANGED_VAR( Helicopter->cep.i_ta_automat_on );    //Индикатор АВТОМАТ ВКЛЮЧЕН	bool
	PRINT_CHANGED_VAR( Helicopter->cep.t_ta_stop );    //Кнопка ВЫКЛ АИ-9В	bool
	PRINT_CHANGED_VAR( Helicopter->cep.i_p_oil_norm );    //Индикатор ДАВ. МАСЛА НОРМА	bool
	PRINT_CHANGED_VAR( Helicopter->cep.i_rpm_norm );    //Индикатор ОБОРОТЫ НОРМА	bool
	PRINT_CHANGED_VAR( Helicopter->cep.i_rpm_max );    //Индикатор ОБОРОТЫ ПРЕДЕЛ	bool
	PRINT_CHANGED_VAR( Helicopter->cep.t_eng_start );    //Кнопка ЗАПУСК ДВИГАТЕЛЕЙ
	PRINT_CHANGED_VAR( Helicopter->cep.i_eng_automat_on );    //Индикатор АВТОМАТ ВКЛЮЧЕН	bool
	PRINT_CHANGED_VAR( Helicopter->cep.t_eng_stop );    //Кнопка ПРЕКРАЩ ЗАПУСКА ДВИГАТЕЛЕЙ	bool
	PRINT_CHANGED_VAR( Helicopter->cep.t_eng_left );    //Тумблер ЗАПУСК ЛЕВ	bool
	PRINT_CHANGED_VAR( Helicopter->cep.t_eng_rght );    //Тумблер ЗАПУСК ПРАВ	bool
	PRINT_CHANGED_VAR( Helicopter->cep.t_eng_sel_start );    //Тумблер ПРОКРУТ	bool	true=вкл
	PRINT_CHANGED_VAR( Helicopter->cep.i_ta_work );    //Индикатор СТАРТЕР РАБОТАЕТ	bool
	PRINT_CHANGED_VAR( Helicopter->cep.t_eng_left_ign_check );    //Тумблер ПРОВЕРКА ЗАЖИГ ЛЕВ	bool
	PRINT_CHANGED_VAR( Helicopter->cep.t_eng_right_ign_check );    //Тумблер ПРОВЕРКА ЗАЖИГ ПРАВ	bool
	PRINT_CHANGED_VAR( Helicopter->cep.t_fuel_left_close );    //Тумблер АИ-9В ПЕРЕКР ЛЕВ КРАН	bool	true=откр
	PRINT_CHANGED_VAR( Helicopter->cep.t_fuel_right_close );    //Тумблер АИ-9В ПЕРЕКР ПРАВ КРАН	bool	true=откр
	PRINT_CHANGED_VAR( Helicopter->cep.t_fuel_bypass_close );    //Тумблер АИ-9В ПЕРЕКР КОЛЬЦЕВ БАКОВ	bool	true=откр
	PRINT_CHANGED_VAR( Helicopter->cep.t_fuel_perepusk );    //Тумблер ПЕРЕПУСК	bool

	PRINT_CHANGED_VAR( Helicopter->cep.i_fuel_left_close );    //Индикатор ЛЕВЫЙ ЗАКРЫТ
	PRINT_CHANGED_VAR( Helicopter->cep.i_fuel_right_close );    //Индикатор ПРАВЫЙ ЗАКРЫТ
	PRINT_CHANGED_VAR( Helicopter->cep.i_fuel_bypass_close );    //Индикатор КОЛЬЦЕВ. ОТКЛ.

	PRINT_CHANGED_VAR( Helicopter->cep.t_fpump_refil );    //Тумблер АИ-9В ЗАПРАВ КОНТР	bool	true=вверх
	PRINT_CHANGED_VAR( Helicopter->cep.t_fpump_consum );    //Тумблер АИ-9В ЗАПРАВ РАСХОД	bool	true=вверх
	PRINT_CHANGED_VAR( Helicopter->cep.t_fpump_left );    //Тумблер АИ-9В ЗАПРАВ ЛЕВЫЙ	bool	true=вверх
	PRINT_CHANGED_VAR( Helicopter->cep.t_fpump_right );    //Тумблер АИ-9В ЗАПРАВ ПРАВЫЙ	bool	true=вверх
	PRINT_CHANGED_VAR( Helicopter->cep.i_fpump_consum_off );    //Индикатор РАСХОД НЕ РАБОТ	bool
	PRINT_CHANGED_VAR( Helicopter->cep.i_fpump_left_off );    //Индикатор ЛЕВЫЙ НЕ РАБОТ	bool
	PRINT_CHANGED_VAR( Helicopter->cep.i_fpump_right_off );    //Индикатор ПРАВЫЙ НЕ РАБОТ	bool

//Центральный Пульт
	PRINT_CHANGED_VAR( Helicopter->ccp.t_searchlight );                //Тумблер ПРОЖЕКТОР	bool
	PRINT_CHANGED_VAR( Helicopter->ccp.t_ignition );                //Кнопка ПОДЖИГ	bool
	PRINT_CHANGED_VAR( Helicopter->ccp.t_focus );                //Кнопка ФОКУС	bool
	PRINT_CHANGED_VAR( Helicopter->ccp.t_spu52_off );              //Кнопка КОНТРОЛЬ ОТКЛ	bool
	PRINT_CHANGED_VAR( Helicopter->ccp.t_spu52_p );              //Тумблер КОНТРОЛЬ P	bool
	PRINT_CHANGED_VAR( Helicopter->ccp.t_spu52_t );              //Тумблер КОНТРОЛЬ t	bool
	PRINT_CHANGED_VAR( Helicopter->ccp.t_glass_wash );              //Тумблер ОМЫВ СТЕКОЛ	bool
	PRINT_CHANGED_VAR( Helicopter->ccp.t_cmd_zu );              //Тумблер КОМАНД РС ЗУ	bool
	PRINT_CHANGED_VAR( Helicopter->ccp.t_cmd_nu );             //Тубмлер КОМАНД РС НУ	bool
	PRINT_CHANGED_VAR( Helicopter->ccp.t_cmd_psh );              //Тумблер ПШ	bool
	PRINT_CHANGED_VAR( Helicopter->ccp.t_ap_alt_up );              //АВТОПИЛОТ - Тумблер Контроль
	PRINT_CHANGED_VAR( Helicopter->ccp.t_ap_alt_down );              //АВТОПИЛОТ - Тумблер Контроль
	PRINT_CHANGED_VAR( Helicopter->ccp.i_ap_dir_off );              //АВТОПИЛОТ - Кнопка НАПРАВЛЕНИЕ ОТКЛ	bool
	PRINT_CHANGED_VAR( Helicopter->ccp.i_ap_dir_on );              //АВТОПИЛОТ - Кнопка НАПРАВЛЕНИЕ ВКЛ	bool
	PRINT_CHANGED_VAR( Helicopter->ccp.i_ap_on );              //АВТОПИЛОТ - Кнопка средняя ВКЛ	bool
	PRINT_CHANGED_VAR( Helicopter->ccp.i_ap_alt_on );              //Кнопка ВЫСОТА ВКЛ	bool
	PRINT_CHANGED_VAR( Helicopter->ccp.i_ap_alt_off );              //Кнопка ВЫСОТА ОТКЛ	bool
	PRINT_CHANGED_VAR( Helicopter->ccp.t_trans_main );              //Тумблер ТРАНСФ ДИМ ОСНОВ	bool
	PRINT_CHANGED_VAR( Helicopter->ccp.t_trans_reserv );              //Тубмлер ТРАНСФ ДИМ ЗАПАСН	bool
	PRINT_CHANGED_VAR( Helicopter->ccp.t_1 );              //Кнопка АП-РК-ПШ	bool
	PRINT_CHANGED_VAR( Helicopter->ccp.t_ap );              //Тумблер АП	bool
	PRINT_CHANGED_VAR( Helicopter->ccp.t_rk );              //Тумблер РК	bool
	PRINT_CHANGED_VAR( Helicopter->ccp.t_psh );              //Тумблер ПШ	bool
	PRINT_CHANGED_VAR( Helicopter->ccp.i_apparel_up );              //Индикатор АППАРЕЛЬ УБРАНА	bool
	PRINT_CHANGED_VAR( Helicopter->ccp.i_apparel_down );              //Индикатор АППАРЕЛЬ ВЫПУЩЕНА	bool
	PRINT_CHANGED_VAR(Helicopter->ccp.f_set_curs );
	PRINT_CHANGED_VAR(Helicopter->ccp.f_set_roll );
	PRINT_CHANGED_VAR(Helicopter->ccp.f_set_pitch );
//Центральный Пульт Управления
	PRINT_CHANGED_VAR(Helicopter->ccp.t_bur_on );	//Тумблер БУР ВКЛ
	PRINT_CHANGED_VAR(Helicopter->ccp.t_kontr_lamp );	//Кнопка КОНТР ЛАМП(КОНТР ИНД)
	PRINT_CHANGED_VAR(Helicopter->ccp.i_otkaz_bur );	//Индикатор ОТКАЗ БУР-1
	PRINT_CHANGED_VAR(Helicopter->ccp.i_otkaz_en );	//Индикатор ОТКАЗ ЭН
	PRINT_CHANGED_VAR(Helicopter->ccp.t_vvod );	//Кнопка ВВОД
	PRINT_CHANGED_VAR(Helicopter->ccp.t_cbros );	//Кнопка СБРОС
	PRINT_CHANGED_VAR(Helicopter->ccp.t_ust );	//Кнопка УСТ
	PRINT_CHANGED_VAR(Helicopter->ccp.t_od );	//Кнопка ОД
	PRINT_CHANGED_VAR(Helicopter->ccp.led_left );	//БУР ЛЕД
	PRINT_CHANGED_VAR(Helicopter->ccp.led_OD);
//Панель Выбора Режима
	PRINT_CHANGED_VAR( Helicopter->psm.i_K1 );//Кнопка К1	bool
	PRINT_CHANGED_VAR( Helicopter->psm.i_G1 );//Кнопка  Г1	bool
	PRINT_CHANGED_VAR( Helicopter->psm.i_K2 );//Кнопка  К2	bool
	PRINT_CHANGED_VAR( Helicopter->psm.i_G2 );//Кнопка  Г2	bool
	PRINT_CHANGED_VAR( Helicopter->psm.i_K );//Кнопка  К	bool
	PRINT_CHANGED_VAR( Helicopter->psm.i_G );//Кнопка  Г	bool
	PRINT_CHANGED_VAR( Helicopter->psm.t_day );//Тумблер ДЕНЬ	bool
	PRINT_CHANGED_VAR( Helicopter->psm.t_night );//Тумблер НОЧЬ	bool
	PRINT_CHANGED_VAR( Helicopter->psm.t_marsh );//Тумблер МАРШР	bool
	PRINT_CHANGED_VAR( Helicopter->psm.t_posad );//Тубмлер ПОСАД	bool
	PRINT_CHANGED_VAR( Helicopter->psm.t_ils );//Галетный Переключ	uint8	0-ILS, 1-СП-50

//Панель выбора курса

//Правая Панель АЗС
	PRINT_CHANGED_VAR( Helicopter->rp_azs.i_peash );//Индикатор ПЭАШ
	PRINT_CHANGED_VAR( Helicopter->rp_azs.i_pevu );//Индикатор ПЭВУ
	PRINT_CHANGED_VAR( Helicopter->rp_azs.i_vis_otstr );//Индикатор ВИЗУАЛИЗАЦИЯ ОТСТРЕЛА
	PRINT_CHANGED_VAR( Helicopter->rp_azs.i_ash1 );//Индикатор АШ-1
	PRINT_CHANGED_VAR( Helicopter->rp_azs.i_ash2 );//Индикатор АШ-2
	PRINT_CHANGED_VAR( Helicopter->rp_azs.i_shakk_vsu );//Индикатор ШАКК ВСУ
	PRINT_CHANGED_VAR( Helicopter->rp_azs.i_shvu_vsu );//Индикатор ШВУ ВСУ
	PRINT_CHANGED_VAR( Helicopter->rp_azs.i_shvu_1k );//Индикатор ШВУ 1К
	PRINT_CHANGED_VAR( Helicopter->rp_azs.i_shvu_2k );//Индикатор ШВУ 2К
	PRINT_CHANGED_VAR( Helicopter->rp_azs.t_ta_start );//Тумблер ТУРБОАГРЕГАТ ЗАПУСК	bool
	PRINT_CHANGED_VAR( Helicopter->rp_azs.t_ta_ign );//Тумблер ТУРБОАГРЕГАТ ЗАЖИГ	bool
	PRINT_CHANGED_VAR( Helicopter->rp_azs.t_eng_start );//Тумблер ДВИГАТЕЛИ ЗАПУСК	bool
	PRINT_CHANGED_VAR( Helicopter->rp_azs.t_eng_ign );//Тумблер ДВИГАТЕЛИ ЗАЖИГ	bool
	PRINT_CHANGED_VAR( Helicopter->rp_azs.t_fuel_gauge );//Тумблер ТОПЛИВОМЕР	bool
	PRINT_CHANGED_VAR( Helicopter->rp_azs.t_pump_consum );//Тумблер НАСОСЫ РАСХОД	bool
	PRINT_CHANGED_VAR( Helicopter->rp_azs.t_pump_left );//Тумблер ТОПЛИВН ЛЕВОГО	bool
	PRINT_CHANGED_VAR( Helicopter->rp_azs.t_pump_right );//Тумблер ТОПЛИВН ПРАВОГО	bool
	PRINT_CHANGED_VAR( Helicopter->rp_azs.t_sas );//Тумблер САС	bool
	PRINT_CHANGED_VAR( Helicopter->rp_azs.t_auto_main ); //Тумблер АВТОПИЛОТ ОСНОВН	bool
	PRINT_CHANGED_VAR( Helicopter->rp_azs.t_auto_fric );//Тумблер АВТОПИЛОТ ФРИКЦ	bool
	PRINT_CHANGED_VAR( Helicopter->rp_azs.t_auto_emufta );//Тумблер АВТОПИЛОТ ЭЛЕКТРОМУФТА	bool
	PRINT_CHANGED_VAR( Helicopter->rp_azs.t_hidro_main );//Тумблер ГИДРОСИСТ ОСНОВН	bool
	PRINT_CHANGED_VAR( Helicopter->rp_azs.t_hidro_sec );//Тумблер ГИДРОСИСТ ДУБЛИР	bool
	PRINT_CHANGED_VAR( Helicopter->rp_azs.t_apparel );//Тумблер УПРАВЛ АППАРЕЛЬ	bool
	PRINT_CHANGED_VAR( Helicopter->rp_azs.t_turn_signal );//Тумблер УКАЗАТ ПОВОРОТ	bool
	PRINT_CHANGED_VAR( Helicopter->rp_azs.t_spuu_52 );//Тумблер СПУУ-52	bool
	PRINT_CHANGED_VAR( Helicopter->rp_azs.t_rentgen );//Тумблер РЕНТГЕНОМЕТР	bool
	PRINT_CHANGED_VAR( Helicopter->rp_azs.t_close_left );//Тумблер ПЕРЕКРЫВ КРАНЫ ЛЕВЫЙ	bool
	PRINT_CHANGED_VAR( Helicopter->rp_azs.t_close_rght );//Тумблер ПЕРЕКРЫВ КРАНЫ ПРАВЫЙ	bool
	PRINT_CHANGED_VAR( Helicopter->rp_azs.t_close_bypass );//Тумблер КРАН ПЕРЕПУС	bool
	PRINT_CHANGED_VAR( Helicopter->rp_azs.t_ppo_signal );//Тумблер СИГНАЛИЗАЦ	bool
	PRINT_CHANGED_VAR( Helicopter->rp_azs.t_ppo_1_auto );//Тумблер I ОЧЕРЕДЬ АВТОМ	bool
	PRINT_CHANGED_VAR( Helicopter->rp_azs.t_ppo_1_hand );//Тумблер I ОЧЕРЕДЬ РУЧН	bool
	PRINT_CHANGED_VAR( Helicopter->rp_azs.t_ppo_2_auto );//Тумблер 2 ОЧЕРЕДЬ АВТОМ	bool
	PRINT_CHANGED_VAR( Helicopter->rp_azs.t_ppo_2_hand );//Тумблер 2 ОЧЕРЕДЬ РУЧНАЯ	bool
	PRINT_CHANGED_VAR( Helicopter->rp_azs.t_r_spu );//Тумблер СПУ	bool
	PRINT_CHANGED_VAR( Helicopter->rp_azs.t_r_altimetr );//Тумблер ВЫСОТОМЕР	bool
	PRINT_CHANGED_VAR( Helicopter->rp_azs.t_r_comand_osnv );//Тумблер КОМАНД РС	bool
	PRINT_CHANGED_VAR( Helicopter->rp_azs.t_r_ils );//Тумблер HAB ILS	bool
	PRINT_CHANGED_VAR( Helicopter->rp_azs.t_r_empty );//Тумблер РАДИО - ПУСТО	bool
	PRINT_CHANGED_VAR( Helicopter->rp_azs.t_r_compas_sv );//Тумблер КОМПАС СВ	bool
	PRINT_CHANGED_VAR( Helicopter->rp_azs.t_r_compas_ukv );//Тумблер КОМПАС УКВ	bool
	PRINT_CHANGED_VAR( Helicopter->rp_azs.t_r_rls );//Тумблер РЛС	bool
	PRINT_CHANGED_VAR( Helicopter->rp_azs.t_far_left_ctrl );//Тумблер ЛЕВ ФАРА УПРАВЛ	bool
	PRINT_CHANGED_VAR( Helicopter->rp_azs.t_far_left_flash );//Тумблер ЛЕВ ФАРА СВЕТ	bool
	PRINT_CHANGED_VAR( Helicopter->rp_azs.t_far_right_ctrl );//Тумблер ПРАВ ФАРА УПРАВЛ	bool
	PRINT_CHANGED_VAR( Helicopter->rp_azs.t_far_right_flash );//Тумблер ПРАВ ФАРА СВЕТ	bool
	PRINT_CHANGED_VAR( Helicopter->rp_azs.t_far_empty );//Тумблер ПУСТО	bool
	PRINT_CHANGED_VAR( Helicopter->rp_azs.t_strov_light );//Тумблер СТРОЕВ ОГНИ	bool
	PRINT_CHANGED_VAR( Helicopter->rp_azs.t_ano );//Тумблер АНО	bool
	PRINT_CHANGED_VAR( Helicopter->rp_azs.t_cut_main_lock );//Тумблер ОБРУБ ТРОС СПГ УПРАВЛ ЗАМКА ОСНОВН	bool
	PRINT_CHANGED_VAR( Helicopter->rp_azs.t_cut_sec_lock );//Тумблер ОБРУБ ТРОС СПГ УПРАВЛ ЗАМКА ДУБЛИР	bool
	PRINT_CHANGED_VAR( Helicopter->rp_azs.t_ais_ctrl );//Тумблер ПРОТИВООБЛЕД СИСТЕМА УПРАВЛЕНИЕ	bool
	PRINT_CHANGED_VAR( Helicopter->rp_azs.t_ais_pzu_left );//Тумблер ПРОТИВООБЛЕД СИСТЕМА ПЗУ ЛЕВ ДВИГАТ	bool
	PRINT_CHANGED_VAR( Helicopter->rp_azs.t_ais_pzu_rght );//Тумблер ПРОТИВООБЛЕД СИСТЕМА ПЗУ ПРАВ ДВИГАТ	bool
	PRINT_CHANGED_VAR( Helicopter->rp_azs.t_ais_co_121 );//Тумблер ПРОТИВООБЛЕД СИСТЕМА СО-121	bool
	PRINT_CHANGED_VAR( Helicopter->rp_azs.t_ais_glass );//Тумблер ПРОТИВООБЛЕД СИСТЕМА СТЕКОЛ	bool
	PRINT_CHANGED_VAR( Helicopter->rp_azs.t_ais_wipe_left );//Тумблер СТЕКЛООЧИСТ ЛЕВЫЙ	bool
	PRINT_CHANGED_VAR( Helicopter->rp_azs.t_ais_wipe_rght );//Тумблер СТЕКЛООЧИСТ ПРАВЫЙ	bool
	PRINT_CHANGED_VAR( Helicopter->rp_azs.t_l166_81a );//Тумблер Л166-BIA	bool
	PRINT_CHANGED_VAR( Helicopter->rp_azs.t_ko_50 );//Тумблер КО-50	bool

// Правая Панель Электропульта
	PRINT_CHANGED_VAR( Helicopter->rsp_elp.ark_9.t_rk_tlf );//Тумблер ТЛФ/ТЛГ	bool
	PRINT_CHANGED_VAR( Helicopter->rsp_elp.ark_9.t_rk_ramka );//Кнопка РАМКА	bool
	PRINT_CHANGED_VAR( Helicopter->rsp_elp.ark_9.t_rk_ctrl );//Кнопка УПР	bool
	PRINT_CHANGED_VAR( Helicopter->rsp_elp.ark_9.t_rk_chanel );//Тумблер КАНАЛ	bool
	PRINT_CHANGED_VAR( Helicopter->rsp_elp.ark_9.sw_rk_mode() );//Галетный Переключатель	uint8	0-ВЫК,1-КОМ,2-АНТ,3-РАМ
	PRINT_CHANGED_VAR( Helicopter->rsp_elp.ark_ud.t_rkud_sens );//Тумблер ЧУВСТВ	bool	true-Б
	PRINT_CHANGED_VAR( Helicopter->rsp_elp.ark_ud.t_rkud_ukv );//Тумблер УКВ-ДЦБ	bool	true-ДЦБ
	PRINT_CHANGED_VAR( Helicopter->rsp_elp.ark_ud.i_rkud_1 );//Кнопка УП	bool
	PRINT_CHANGED_VAR( Helicopter->rsp_elp.ark_ud.i_rkud_2 );//Кнопка ШП	bool
	PRINT_CHANGED_VAR( Helicopter->rsp_elp.ark_ud.i_rkud_3 );//Кнопка И	bool
	PRINT_CHANGED_VAR( Helicopter->rsp_elp.ark_ud.sw_rkud_mode() );//Галетный Переключатель	АРК-УД uint8	0-ВЫК,1-УП,2-ШП,3-И,4-РПК
	PRINT_CHANGED_VAR( Helicopter->rsp_elp.ark_ud.sw_rkud_chanel() );//Галетный переключатель КАНАЛЫ	uint8
	PRINT_CHANGED_VAR( Helicopter->rsp_elp.gmk.t_contr0 );//Кнопка КОНТР	bool
	PRINT_CHANGED_VAR( Helicopter->rsp_elp.gmk.t_contr300 );//Кнопка КОНТР	bool
	PRINT_CHANGED_VAR( Helicopter->rsp_elp.ark_ud.t_ant_l );//Кнопка АНТ Л	bool
	PRINT_CHANGED_VAR( Helicopter->rsp_elp.ark_ud.t_ant_r );//Кнопка АНТ П	bool
	PRINT_CHANGED_VAR( Helicopter->rsp_elp.i_ko50_heater );//Индикатор ПОДОГРЕВАТЕЛЬ	bool
	PRINT_CHANGED_VAR( Helicopter->rsp_elp.i_ko50_ignition );//Индикатор ЗАЖИГАНИЕ	bool
	PRINT_CHANGED_VAR( Helicopter->rsp_elp.i_ko50_work );//Индикатор КО-50 РАБОТАЕТ	bool
	PRINT_CHANGED_VAR( Helicopter->rsp_elp.t_ko50_ta_start );//Кнопка ЗАПУСК ТУРБОАГРЕГАТА	bool
	PRINT_CHANGED_VAR( Helicopter->rsp_elp.t_ko50_auto );//Тумблер КО50-РУЧН-АВТ	bool	true-РУЧН
	PRINT_CHANGED_VAR( Helicopter->rsp_elp.t_ko50_man );//Тумблер КО50-РУЧН-АВТ	bool	true-РУЧН
	PRINT_CHANGED_VAR( Helicopter->rsp_elp.t_ko50_fill );//Тумблер КО50-ЗАЛИВКА	bool
	PRINT_CHANGED_VAR( Helicopter->rsp_elp.t_ko50_midl );//Тумблер КО50-СРЕДН РЕЖИМ	bool
	PRINT_CHANGED_VAR( Helicopter->rsp_elp.t_ko50_full );//Тумблер КО50-ПОЛНЫЙ РЕЖИМ	bool
	PRINT_CHANGED_VAR( Helicopter->rsp_elp.t_ko50_vent );//Тумблер Вентил	bool
	PRINT_CHANGED_VAR( Helicopter->rsp_elp.t_N );//Тумблер СЕВ ЮГ	bool	true-ЮГ
	PRINT_CHANGED_VAR( Helicopter->rsp_elp.ark_ud.t_contr );//Тумблер КОНТР	bool	true-300
	PRINT_CHANGED_VAR( Helicopter->rsp_elp.t_mk );//Тумблер МК ГК	bool	true-ГК
	PRINT_CHANGED_VAR( Helicopter->rsp_elp.t_zk_l );//Тумблер ЗК ЛЕВ	bool
	PRINT_CHANGED_VAR( Helicopter->rsp_elp.t_zk_r );//Тумблер ЗК ПРАВ	bool

//Правый Щиток Электропульта
	PRINT_CHANGED_VAR( Helicopter->rsh_elp.t_pvd_heat_contr );//Кнопка КОНТРОЛЬ ОБОГРЕВА ПВД
	PRINT_CHANGED_VAR( Helicopter->rsh_elp.i_pvd_heat_ok );//Индикатор ОБОГРЕВ ИСПРАВЕН	bool
	PRINT_CHANGED_VAR( Helicopter->rsh_elp.t_diss );//Тумблер ДИСС	bool
	PRINT_CHANGED_VAR( Helicopter->rsh_elp.t_rs );//Тумблер СВЯЗНАЯ РС	bool
	PRINT_CHANGED_VAR( Helicopter->rsh_elp.t_gmk );//Тумблер ГМК-1	bool
	PRINT_CHANGED_VAR( Helicopter->rsh_elp.t_sgu35 );//Тумблер ВК-53	bool
	PRINT_CHANGED_VAR( Helicopter->rsh_elp.t_ag_right );//Тумблер ПРАВЫЙ АГ	bool
	PRINT_CHANGED_VAR( Helicopter->rsh_elp.t_plafon );//Тумблер ПЛАФОН	bool
	PRINT_CHANGED_VAR( Helicopter->rsh_elp.sw_wiper_mode );//Переключатель СТЕКЛООЧИСТИТЕЛЬ	uint8	0-выкл, 1-пуск, 2-сброс, 3-1скор, 4-2 скор
	PRINT_CHANGED_VAR( Helicopter->rsh_elp.t_laring );//Тумблер ЛАРИНГ bool
	PRINT_CHANGED_VAR( Helicopter->rsh_elp.t_rkud );//Тумблер АРК-УД	bool

//Правая Боковая Панель Электропульта
	PRINT_CHANGED_VAR( Helicopter->rsp_ep.t_contr );//Кнопка КОНТРОЛЬ
	PRINT_CHANGED_VAR( Helicopter->rsp_ep.i_contr );//Лампа КОНТРОЛЬ
	PRINT_CHANGED_VAR( Helicopter->rsp_ep.i_pzu_left_on );//Индикатор ЛЕВ. ПЗУ ВКЛЮЧЕН	bool
	PRINT_CHANGED_VAR( Helicopter->rsp_ep.i_pzu_right_on );//Индикатор ПРАВ. ПЗУ ВКЛЮЧЕН	bool
	PRINT_CHANGED_VAR( Helicopter->rsp_ep.t_light_emeg );//Тумблер ДЕЖУРНОЕ ОСВЕЩЕНИЕ	bool
	PRINT_CHANGED_VAR( Helicopter->rsp_ep.t_light_generl );//Тумблер ОБЩЕЕ ОСВЕЩЕНИЕ	bool
	PRINT_CHANGED_VAR( Helicopter->rsp_ep.t_ano );//Тумблер АНО	bool	true=ЯРКО
	PRINT_CHANGED_VAR( Helicopter->rsp_ep.t_light_drill );//Тумблер ОГНИ СТРОЕВ КОНТУР ЛЕВ	bool
	PRINT_CHANGED_VAR( Helicopter->rsp_ep.t_light_contur );//Тумблер ОГНИ СТРОЕВ КОНТУР ПРАВ	bool
	PRINT_CHANGED_VAR( Helicopter->rsp_ep.t_beacon_down );//Тумблер МАЯК НИЗ	bool
	PRINT_CHANGED_VAR( Helicopter->rsp_ep.t_pzu_left );//Тумблер ПЗУ ДВИГАТ ЛЕВ	bool
	PRINT_CHANGED_VAR( Helicopter->rsp_ep.t_pzu_right );//Тумблер ПЗУ ДВИГАТ ПРАВ	bool
	PRINT_CHANGED_VAR( Helicopter->rsp_ep.t_pvd_heat_left );//Тумблер ОБОГРЕВ ПВД ЛЕВ	bool
	PRINT_CHANGED_VAR( Helicopter->rsp_ep.t_pvd_heat_right );//Тумблер ОБОГРЕВ ПВД ПРАВ	bool
	PRINT_CHANGED_VAR( Helicopter->rsp_ep.t_clock_heat );//Тумблер ОБОГРЕВ ЧАСОВ	bool

//Правая Приборная Панель
	PRINT_CHANGED_VAR( Helicopter->rpb.t_fara_close );//Тумблер СВЕТ	bool	Убрана
	PRINT_CHANGED_VAR( Helicopter->rpb.t_fara_on );//Тумблер СВЕТ	bool	Свет
	PRINT_CHANGED_VAR( Helicopter->rpb.t_projector );//Тумблер УПРАВЛ	bool	Свет
	PRINT_CHANGED_VAR( Helicopter->rpb.i_fuel_270 );//Индикатор ОСТАЛОСЬ 270 Л	bool
	//PRINT_CHANGED_VAR( Helicopter->rpb.sw_fuel() );//Галетный Переключатель БАКИ	uint8	0:05
	//PRINT_CHANGED_VAR( Helicopter->rpb.f_fuel );
	PRINT_CHANGED_VAR( Helicopter->rpb.ki_206.i_from_to ); //НАВИГАТОР Индикатор FR
	PRINT_CHANGED_VAR( Helicopter->rpb.ki_206.i_gs ); //Навигатор флажок GS
	PRINT_CHANGED_VAR( Helicopter->rpb.ki_206.i_nav ); //НАВИГАТОР Флажок NAV
	PRINT_CHANGED_VAR( Helicopter->rpb.avg.blank );//АВИАГОРИЗОНТ ЛЕВ:Флажок отс. питания	bool
	PRINT_CHANGED_VAR( Helicopter->rpb.avg.shift_pitch );//АВИАГОРИЗОНТ 
	PRINT_CHANGED_VAR( Helicopter->rpb.avg.encoder );//АВИАГОРИЗОНТ 
	PRINT_CHANGED_VAR( Helicopter->rpb.snos.i_angle );//УГОЛ СНОСА - индикатор	bool	
//	PRINT_CHANGED_VAR(/*Helicopter_->rpb.snos.t_pk*/ FP->Par169);//УГОЛ СНОСА - Переключатель Р-К	bool	true-К
	PRINT_CHANGED_VAR( Helicopter->rpb.snos.t_cm );//УГОЛ СНОСА - Переключатель С-М	bool	true = М
	PRINT_CHANGED_VAR( Helicopter->rpb.diss_15.t_rght );//Кнопка ВПР
	PRINT_CHANGED_VAR( Helicopter->rpb.diss_15.t_left );//Кнопка ВЛ
	PRINT_CHANGED_VAR( Helicopter->rpb.diss_15.sw_side );//БОКОВОЕ УКЛОНЕНИЕ ВЛЕВО-ВПРАВО
	PRINT_CHANGED_VAR( Helicopter->rpb.diss_15.t_v );//Кнопка В
	PRINT_CHANGED_VAR( Helicopter->rpb.diss_15.t_n );//Кнопка Н
	PRINT_CHANGED_VAR( Helicopter->rpb.diss_15.sw_way );//ПУТЬ ВПЕРЕД-НАЗАД
	PRINT_CHANGED_VAR( Helicopter->rpb.diss_15.t_plus );//Кнопка +
	PRINT_CHANGED_VAR( Helicopter->rpb.diss_15.t_minus );//Кнопка -
	PRINT_CHANGED_VAR( Helicopter->rpb.diss_15.t_on );//Кнопка ВКЛ
	PRINT_CHANGED_VAR( Helicopter->rpb.diss_15.t_off );//Кнопка ОТКЛ
	PRINT_CHANGED_VAR( Helicopter->rpb.diss_15.i_on );//ДИСС - лампа ВКЛЮЧЕНО	bool
	PRINT_CHANGED_VAR( Helicopter->rpb.i_diss_brok );//Индикатор ДИСС ОТКАЗ	bool
	PRINT_CHANGED_VAR( Helicopter->rpb.i_ag_brok );//Индикатор ОТКАЗ АГ ПРАВ	bool
	PRINT_CHANGED_VAR( Helicopter->rpb.i_roll_max );//Индикатор КРЕН ВЕЛИК	bool IP->Par022

//Щиток Электросбрасывателя
	//PRINT_CHANGED_VAR(0);//Индикатор БД1 ЗАГРУЖ	bool
	//PRINT_CHANGED_VAR(0);//Индикатор БД2 ЗАГРУЖ	bool
	//PRINT_CHANGED_VAR(0);//Индикатор БД3 ЗАГРУЖ	bool
	//PRINT_CHANGED_VAR(0);//Индикатор БД4 ЗАГРУЖ	bool
	//PRINT_CHANGED_VAR(0);//Индикатор АВАР ВЗРЫВ	bool
	//PRINT_CHANGED_VAR(0);//Индикатор СЕТЬ БВ ВКЛЮЧ.	bool
	//PRINT_CHANGED_VAR(0);//Тумблер СБРОС БОМБ	bool
	//PRINT_CHANGED_VAR(0);//Тумблер ВЗРЫВ	bool
	//PRINT_CHANGED_VAR(0);//Тумблер ГЛАВНЫЙ ВЫКЛЮЧ	bool
	//PRINT_CHANGED_VAR(0);//Тумблер ПО1-ПО2	bool

// Правый Щиток Электропульта
	PRINT_CHANGED_VAR( Helicopter->ep_elp.t_akkum_1 );//Тумблер АККУМУЛЯТОРЫ 1	bool
	PRINT_CHANGED_VAR( Helicopter->ep_elp.t_akkum_2 );//Тумблер АККУМУЛЯТОРЫ 2	bool
	PRINT_CHANGED_VAR( Helicopter->ep_elp.sw_ctrl );//Переключ КОНТРОЛЬ ТОКА	uint8	1-Выпрямит 1, 2-Выпрямит 2, 3-резерв генер, 
	PRINT_CHANGED_VAR( Helicopter->ep_elp.sw_dc_volt_mode );//Переключатель ЦЕНТР	uint8	0-откл,1-Аккумулят1,2-Аккумулят2,3-Выпрямит1,4-Выпрямит2, 5-Шина ВУ Кан1,6-Шина ВУ Кан2, 7-Шина ВУ ВСУ, 8-РЕЗЕР ГЕНЕР,9-ОТКЛ
	PRINT_CHANGED_VAR( Helicopter->ep_elp.f_volt_reg );//Переключатель РЕГУЛИР НАПРЯЖ	uint8	0-М, 1-Б
	PRINT_CHANGED_VAR( Helicopter->ep_elp.t_ACDC_1 );//Тумблер ВЫПРЯМИТЕЛИ 1	bool
	PRINT_CHANGED_VAR( Helicopter->ep_elp.t_ACDC_2 );//Тумблер ВЫПРЯМИТЕЛИ 2	bool
	PRINT_CHANGED_VAR( Helicopter->ep_elp.t_test_sys );//Тумблер ПРОВЕРКА ОБОРУД	bool
	PRINT_CHANGED_VAR( Helicopter->ep_elp.t_aero_pit_dc );//Тумблер АЭР ПИТ	bool
	PRINT_CHANGED_VAR( Helicopter->ep_elp.i_vu1_brok );//Индикатор ВУ1 НЕ РАБОТ	bool
	PRINT_CHANGED_VAR( Helicopter->ep_elp.i_vu2_brok );//Индикатор ВУ2 НЕ РАБОТ	bool
	PRINT_CHANGED_VAR( Helicopter->ep_elp.i_aero_pit_dc );//Индикатор ПРОВЕРКА ОБОРУД	bool
	PRINT_CHANGED_VAR( Helicopter->ep_elp.i_gen_1_off );//Индикатор ГЕН. 1 ОТКЛ	bool
	PRINT_CHANGED_VAR( Helicopter->ep_elp.i_gen_2_off );//Индикатор ГЕН. 2 ОТКЛ	bool
	PRINT_CHANGED_VAR( Helicopter->ep_elp.t_gen_1 );//Тумблер ГЕНЕРАТОРЫ 1	bool
	PRINT_CHANGED_VAR( Helicopter->ep_elp.t_gen_2 );//Тумблер ГЕНЕРАТОРЫ 2	bool
	PRINT_CHANGED_VAR( Helicopter->ep_elp.t_r36_auto );//Тумблер ТР-Р36В АВТОМАТ	bool
	PRINT_CHANGED_VAR( Helicopter->ep_elp.t_r36_man );//Тумблер ТР-Р36В РУЧНОЕ	bool
	PRINT_CHANGED_VAR( Helicopter->ep_elp.t_pts_auto );//Тумблер ПТС АВТОМАТ	bool
	PRINT_CHANGED_VAR( Helicopter->ep_elp.t_pts_man );//Тумблер ПТС РУЧНОЕ	bool
	PRINT_CHANGED_VAR( Helicopter->ep_elp.t_aero_pit_ac );//Тумблер АЗР ПИТАН	bool
	PRINT_CHANGED_VAR( Helicopter->ep_elp.t_gen );//Тумблер ПТС АЗР ПИТАН	bool
	PRINT_CHANGED_VAR( Helicopter->ep_elp.sw_ac_volt_mode );//Переключатель КОНТРОЛЬ НАПРЯЖЕНИЯ	uint8	0-2 ПТС ГЕНЕРАТОР,3-5 АЗР ПИТАН ГЕНЕРАТОР 2
	PRINT_CHANGED_VAR( Helicopter->ep_elp.i_reser36 );//Индикатор -38В РЕЗЕРВ	bool
	PRINT_CHANGED_VAR( Helicopter->ep_elp.i_ptc_on );//Индикатор ПТ ВКЛЮЧЕН	bool
	PRINT_CHANGED_VAR( Helicopter->ep_elp.i_resev_on );//Индикатор РЕЗЕРВ ЛИНИЯ ВКЛ	bool
	PRINT_CHANGED_VAR( Helicopter->ep_elp.i_rap_on );//Индикатор РАП ПОДКЛ	bool

//Рычаги и педали
//	PRINT_CHANGED_VAR(0);//Лев штурвал - Кнопка СПУ РАДИО
	PRINT_CHANGED_VAR( Helicopter->PC.t_left_trim );//Лев штурвал - Кнопка ЭЛЕКТРОМАГНИТНЫЙ ТОРМОЗ
	PRINT_CHANGED_VAR( Helicopter->PC.t_left_fire_prev );//Лев штурвал - Левая красная кнопка
	PRINT_CHANGED_VAR( Helicopter->PC.t_left_fire );//Лев штурвал - Левая красная кнопка
	PRINT_CHANGED_VAR( Helicopter->_BoardSystem->b_fire );// Стрельба
	PRINT_CHANGED_VAR( Helicopter->PC.t_left_ap_off );//Лев штурвал - Кнопка ОТКЛЮЧЕНИЕ АВТОПИЛОТА
//	PRINT_CHANGED_VAR(0);//Прав штурвал -Кнопка СПУ РАДИО 0-ВЫКЛ, 1-КАБИНА, 2- ДИСПЕТЧЕР
//	PRINT_CHANGED_VAR(0);//Прав штурвал - Кнопка ЭЛЕКТРОМАГНИТНЫЙ ТОРМОЗ
	PRINT_CHANGED_VAR( Helicopter->PC.t_right_fire );//Прав штурвал-Левая красная кнопка	bool
	PRINT_CHANGED_VAR( Helicopter->PC.t_right_ap_off );//Прав штурвал - Кнопка ОТКЛЮЧЕНИЕ АВТОПИЛОТА	bool
	PRINT_CHANGED_VAR( Helicopter->PC.t_fpp_l_up );//Кнопка управления фарой вверх	bool
	PRINT_CHANGED_VAR( Helicopter->PC.t_fpp_l_down );//Кнопка управления фарой вниз	bool
	PRINT_CHANGED_VAR( Helicopter->PC.t_fpp_l_left );//Кнопка управления фарой влево	bool
	PRINT_CHANGED_VAR( Helicopter->PC.t_fpp_l_right );//Кнопка управления фарой вправо	bool
	PRINT_CHANGED_VAR( Helicopter->PC.t_avar_drop );//Кнопка Аварийного сброса груза	bool
	PRINT_CHANGED_VAR( Helicopter->PC.t_tact_drop );//Кнопка тактического сброса груза	bool
	PRINT_CHANGED_VAR( Helicopter->PC.t_fr_l_fosh );//Кнопка фрикциона	bool
	PRINT_CHANGED_VAR( Helicopter->PC.t_ap_dir_sopr );// Педали отработали	bool
	PRINT_CHANGED_VAR( Helicopter->PC.t_rpm_up );//Тумблер	bool
	PRINT_CHANGED_VAR( Helicopter->PC.t_rpm_down );//Тумблер	bool
	PRINT_CHANGED_VAR( Helicopter->PC.t_fpp_r_up );//Кнопка управления фарой вверх	bool
	PRINT_CHANGED_VAR( Helicopter->PC.t_fpp_r_down );//Кнопка управления фарой вниз	bool
	PRINT_CHANGED_VAR( Helicopter->PC.t_fpp_r_left );//Кнопка управления фарой влево	bool
	PRINT_CHANGED_VAR( Helicopter->PC.t_fpp_r_right );//Кнопка управления фарой вправо	bool
	PRINT_CHANGED_VAR( Helicopter->PC.t_fr_l_fosh );//Кнопка фрикциона	bool она одна на всех
	PRINT_CHANGED_VAR( Helicopter->PC.t_eng_left_brake );//Рычаг тормоза винта кнопка
	PRINT_CHANGED_VAR( Helicopter->PC.t_eng_rght_brake );//Рычаг тормоза винта кнопка
	PRINT_CHANGED_VAR( Helicopter->PC.t_NV_brake_off );//Рычаг тормоза винта кнопка
	PRINT_CHANGED_VAR( Helicopter->PC.t_NV_brake_on );//Рычаг тормоза винта кнопка
	PRINT_CHANGED_VAR( Helicopter->PC.t_safety_lock );
	PRINT_CHANGED_VAR( Helicopter->PC.i_weapon_warning );
	PRINT_CHANGED_VAR( Helicopter->PC.pkt_x );					// ПКТ-горизонтальный угол
	PRINT_CHANGED_VAR( Helicopter->PC.pkt_y );					// ПКТ-вертикальный угол
	PRINT_CHANGED_VAR( Helicopter->PC.pkt_fire );					// ПКТ-кнопки
	
	PRINT_CHANGED_VAR( Helicopter->WeaponPult.t_pus_load_PO1);    //Тумблер ПО1
	PRINT_CHANGED_VAR( Helicopter->WeaponPult.t_pus_load_PO2);    //Тумблер ПО2(новая переменная
	PRINT_CHANGED_VAR( Helicopter->WeaponPult.t_main_switch);     //Тумблер ГЛАВНЫЙ ВЫКЛЮЧ
	PRINT_CHANGED_VAR( Helicopter->WeaponPult.t_vzriv);           //Тумблер ВЗРЫВ
	PRINT_CHANGED_VAR( Helicopter->WeaponPult.t_sbros);           //Тумблер СБРОС БОМБ
	PRINT_CHANGED_VAR( Helicopter->WeaponPult.t_MV);				// минирование?
	PRINT_CHANGED_VAR( Helicopter->WeaponPult.t_PKT);				// РПК
	PRINT_CHANGED_VAR( Helicopter->WeaponPult.t_FAB);				// бомбы
	PRINT_CHANGED_VAR( Helicopter->WeaponPult.t_NAR);				// неуправляемые ракеты
	PRINT_CHANGED_VAR( Helicopter->WeaponPult.t_UPK23);			// пушки 23х2
	PRINT_CHANGED_VAR( Helicopter->WeaponPult.t_AGS17);			// гранатомет
	PRINT_CHANGED_VAR( Helicopter->WeaponPult.i_weapon_line );
	PRINT_CHANGED_VAR( Helicopter->WeaponPult.i_AB );
	PRINT_CHANGED_VAR( Helicopter->WeaponPult.i_vzriv );
	PRINT_CHANGED_VAR( Helicopter->WeaponPult.i_pus_bd_1_6 );
	PRINT_CHANGED_VAR( Helicopter->WeaponPult.i_npc );
	PRINT_CHANGED_VAR( Helicopter->WeaponPult.i_pus_bd_2_5 );
	PRINT_CHANGED_VAR( Helicopter->WeaponPult.i_upk );
	PRINT_CHANGED_VAR( Helicopter->WeaponPult.i_bd_1_load );
	PRINT_CHANGED_VAR( Helicopter->WeaponPult.i_bd_2_load );
	PRINT_CHANGED_VAR( Helicopter->WeaponPult.i_bd_3_load );
	PRINT_CHANGED_VAR( Helicopter->WeaponPult.i_bd_4_load );
	PRINT_CHANGED_VAR( Helicopter->WeaponPult.i_bd_5_load );
	PRINT_CHANGED_VAR( Helicopter->WeaponPult.i_bd_6_load );
	PRINT_CHANGED_VAR( Helicopter->WeaponPult.c_upk_left );
	PRINT_CHANGED_VAR( Helicopter->WeaponPult.c_upk_right );
	PRINT_CHANGED_VAR( Helicopter->WeaponPult.t_fire_short );
	PRINT_CHANGED_VAR( Helicopter->WeaponPult.t_fire_midle );
	PRINT_CHANGED_VAR( Helicopter->WeaponPult.t_fire_long );
	PRINT_CHANGED_VAR( Helicopter->WeaponPult.t_pod_ext );
	PRINT_CHANGED_VAR( Helicopter->WeaponPult.t_pod_all );
	PRINT_CHANGED_VAR( Helicopter->WeaponPult.t_pod_inter );

}

void Helicopter_controls::PrintParametrs( )
{
	//FParam* FPar = Helicopter->Model_->GetFPar();
	IParam* IPar = Helicopter->Model_->GetIPar( );

	PrintTumblers( );
	//bool vibration = Helicopter->_BoardSystem->vibration[0];
	//PRINT_CHANGED_VAR( vibration );

	PRINT_CHANGED_VAR( Helicopter->_BoardSystem->_Electrics->Uakk1 );
	PRINT_CHANGED_VAR( Helicopter->_BoardSystem->_Electrics->Uakk2 );
	PRINT_CHANGED_VAR( Helicopter->_BoardSystem->_Electrics->Uvu1 );
	PRINT_CHANGED_VAR( Helicopter->_BoardSystem->_Electrics->Uvu2 );
	PRINT_CHANGED_VAR( Helicopter->_BoardSystem->_Electrics->Ugen36 );
	PRINT_CHANGED_VAR( Helicopter->_BoardSystem->_Electrics->Ugen36r );
	PRINT_CHANGED_VAR( Helicopter->_BoardSystem->_Electrics->Ugen115 );
	PRINT_CHANGED_VAR( Helicopter->_BoardSystem->_Electrics->Ugen115r );
	PRINT_CHANGED_VAR( Helicopter->_BoardSystem->_Electrics->Ugen204 );
	PRINT_CHANGED_VAR( Helicopter->_BoardSystem->_Electrics->Udim );
	//PRINT_CHANGED_VAR( Helicopter->_BoardSystem->tablo );
	
	PRINT_CHANGED_VAR( Helicopter->_BoardSystem->_Engine_1->state_ );
	PRINT_CHANGED_VAR( Helicopter->_BoardSystem->_Engine_2->state_ );
	PRINT_CHANGED_VAR( Helicopter->_BoardSystem->_VSU->state_ );
	PRINT_CHANGED_VAR( Helicopter->lp_azs.t_main_switch_on );
	
	PRINT_CHANGED_VAR( IPar->EngineL );
	PRINT_CHANGED_VAR( IPar->EngineR );
	//PRINT_CHANGED_VAR(IPar->Par072);
	//PRINT_CHANGED_VAR(IPar->Par158);
	//PRINT_CHANGED_VAR(IPar->Par159);
	//PRINT_CHANGED_VAR(IPar->Par160);
	PRINT_CHANGED_VAR( IPar->Par042 );
	PRINT_CHANGED_VAR( IPar->Par043 );
	PRINT_CHANGED_VAR( IPar->Par044 );
	PRINT_CHANGED_VAR( Helicopter->PC.reset );

}

// TODO: Этот код опасен с точки зрения добавления элементов!
T_UNITS& Helicopter_controls::GetUnitsData( )
{
	//	qDebug() << __FUNCTION__;
	for ( auto& UNIT : Helicopter->can->UNITS.MAB )
	{
		UNIT.INFO = UNIT.DESC;
		if ( UNIT.ENABLE )
			UNIT.INFO += "Pack TX / RX : [" + QString::number( UNIT.TX ) + "]/[" + QString::number( UNIT.RX ) + "]";
	}
	for ( auto& UNIT : Helicopter->can->UNITS.VID )
	{
		UNIT.INFO = UNIT.DESC;
		if ( UNIT.ENABLE )
			UNIT.INFO += "Pack TX / RX : [" + QString::number( UNIT.TX ) + "]/[" + QString::number( UNIT.RX ) + "]";
	}
	for ( auto& UNIT : Helicopter->can->UNITS.GPIN )
	{
		UNIT.INFO = UNIT.DESC;
		if ( UNIT.ENABLE )
			UNIT.INFO += "Pack TX / RX : [" + QString::number( UNIT.TX ) + "]/[" + QString::number( UNIT.RX ) + "]";
	}
	for ( auto& UNIT : Helicopter->can->UNITS.ENC )
	{
		UNIT.INFO = UNIT.DESC;
		if ( UNIT.ENABLE )
			UNIT.INFO += "Pack TX / RX : [" + QString::number( UNIT.TX ) + "]/[" + QString::number( UNIT.RX ) + "]";
	}
	for ( auto& UNIT : Helicopter->can->UNITS.GPOUT )
	{
		UNIT.INFO = UNIT.DESC;
		if ( UNIT.ENABLE )
			UNIT.INFO += "Pack TX / RX : [" + QString::number( UNIT.TX ) + "]/[" + QString::number( UNIT.RX ) + "]";
	}
	for ( auto& UNIT : Helicopter->can->UNITS.POT )
	{
		UNIT.INFO = UNIT.DESC;
		if ( UNIT.ENABLE )
			UNIT.INFO += "Pack TX / RX : [" + QString::number( UNIT.TX ) + "]/[" + QString::number( UNIT.RX ) + "]";
	}

	return Helicopter->can->UNITS;
}


// TODO: Этот код опасен с точки зрения добавления элементов!
void Helicopter_controls::CanHeartBeat( )
{
	//	qDebug() << __FUNCTION__;
	CANBusStateMsg.clear( );
	if ( Helicopter->can->BoardCount( ) > 0 )
	{
		if ( Helicopter->can->Port0( ) == INVALID_HANDLE_VALUE )
		{
			CANBusState = ElementsStateTypes::CanBlockNoAnswer;
			CANBusStateMsg += "Open CAN Port0 Fail \r\n";
		}
		if ( Helicopter->can->Port1( ) == INVALID_HANDLE_VALUE )
		{
			CANBusState = ElementsStateTypes::CanBlockNoAnswer;
			CANBusStateMsg += "Open CAN Port1 Fail \r\n";
		}
	}
	for ( auto& unit : Helicopter->can->UNITS.MAB )
	{
		if ( unit.FAULT )
		{
			CANBusState = ElementsStateTypes::CanBlockNoAnswer;
			CANBusStateMsg += unit.sFAULT;
		}
	}
	for ( auto& unit : Helicopter->can->UNITS.VID )
	{
		if ( unit.FAULT )
		{
			CANBusState = ElementsStateTypes::CanBlockNoAnswer;
			CANBusStateMsg += unit.sFAULT;
		}
	}
	for ( auto& unit : Helicopter->can->UNITS.GPIN )
	{
		if ( unit.FAULT )
		{
			CANBusState = ElementsStateTypes::CanBlockNoAnswer;
			CANBusStateMsg += unit.sFAULT;
		}
	}
	for ( auto& unit : Helicopter->can->UNITS.GPOUT )
	{
		if ( unit.FAULT )
		{
			CANBusState = ElementsStateTypes::CanBlockNoAnswer;
			CANBusStateMsg += unit.sFAULT;
		}
	}
	for ( auto& unit : Helicopter->can->UNITS.POT )
	{
		if ( unit.FAULT )
		{
			CANBusState = ElementsStateTypes::CanBlockNoAnswer;
			CANBusStateMsg += unit.sFAULT;
		}
	}
	for ( int i = 0; i < N_PARAM_MAB; i++ )
	{
		WORD value = GetMABValue( i );
		if ( value == 4096 ) CANBusStateMsg += "MAB" + GetMABaddr( i ) + " disconnected \r\n";
	}
	Helicopter->server_->CANBusState = CANBusState;
	Helicopter->server_->CANBusStateMsg = CANBusStateMsg;
}

// метод генерируется автоматически на основе CVS файла
//void Helicopter_controls::CanExchange( )
//{
//}