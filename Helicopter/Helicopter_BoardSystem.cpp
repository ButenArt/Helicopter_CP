#include "Helicopter_BoardSystem.h"
#include "Helicopter_core.h"
#include "Helicopter_instructor.h"
#include "cassert"

float Magnetic::magneticDeclination = 0.0;  // Инициализация статического поля

// Функция checkBOM проверяет файл на наличие BOM в начале и устанавливает точку чтения за ним или на начало файла если его нет.
bool checkBOM( std::ifstream& file ) {
	char bom[3];
	file.read( bom, 3 );
	if ( bom[0] == '\xEF' && bom[1] == '\xBB' && bom[2] == '\xBF' ) {
		file.seekg( 3 );
		return true;
	}
	else {
		file.seekg( 0 );
		return false;
	}
}

// для ОУ не запоминаем 
quint8 RightPilotBoard::sw_fuel_mm( )
{
	const quint8 tabl[] = { 0,1,3,4,5,2 };
	return sw_fuel_ < sizeof( tabl ) ? tabl[sw_fuel_] : 0;
}

quint8& RightPilotBoard::sw_fuel()
{
	if (t_fuel_0) { sw_fuel_ = 0; } // 0 - ВЫКЛ; 
	else if (t_fuel_1) { sw_fuel_ = 1; } // 1 - СУММА - суммарный запас топлива;
	else if (t_fuel_2) { sw_fuel_ = 2; } // 2 - Дл - левый дополнительный;
	else if (t_fuel_3) { sw_fuel_ = 3; } // 3 - Пл - левый подвесной; 
	else if (t_fuel_4) { sw_fuel_ = 4; } // 4 - Ппр - правый подвесной; 
	else if (t_fuel_5) { sw_fuel_ = 5; } // 5 - РАСХ. - расходный;
	else if (t_fuel_6) { sw_fuel_ = 6; } // 6 - Дпр - правый дополнительный
	return sw_fuel_;  //
}

quint8 PultControlDISS::get_sw_mode()
{
	if (t_p1) { return 1; } // 1 - 
	if (t_p2) { return 2; } // 2 - 
	if (t_p3) { return 3; } // 3 - 
	if (t_p4) { return 4; } // 4 - 
	if (t_p5) { return 5; } // 5 - 

	return 5;  // 6 - 
}

quint8 LeftPilotBoard::get_sw_pvd()
{
	if (t_pvd_left) { return 0; } // 1 - 
	if (t_pvd_unitd) { return 1; } // 2 - 
	if (t_pvd_right) { return 2; } // 3 - 
	return 3;
}

void CentralElectricPult::StartAll( )
{
	t_firefighting = true;     //  Тумблер ОГНЕТУШЕНИЕ
	t_1 = true;   //  Тумблер КОНТРОЛЬ ПИРОПАТРОНОВ
	t_contr_sensors_0 = true;

	t_off_second_hidro = true; //  Кнопка ОТКЛ ДУБЛИР
	t_hidro_main = true;       //  Тумблер ГИДРОСИСТЕМА ОСНОВН
	t_hidro_second = true;     //  Тумблер ГИДРОСИСТЕМА ДУБЛИР

	t_ta_sel_false_start = false;  //  Тумблер ТА ПРОКРУТ ЛОЖН ЗАПУСК
	t_ta_sel_scroll = false;       //  Тумблер ТА ПРОКРУТ 
	t_ta_sel_start = true;        //  Тумблер ТА ПРОКРУТ ЗАПУСК
	t_ta_start = false;            //  Кнопка ЗАПУСК ТУРБОАГРЕГАТА
	t_ta_stop = false;             //  Кнопка ВЫКЛ АИ-9В
	t_eng_start = false;           //  Кнопка ЗАПУСК ДВИГАТЕЛЕЙ
	t_eng_stop = false;            //  Кнопка ПРЕКРАЩ ЗАПУСКА ДВИГАТЕЛЕЙ
	t_eng_left = false;            //  Тумблер ЗАПУСК ДВИГАТЕЛЕЙ ЛЕВ
	t_eng_rght = false;            //  Тумблер ЗАПУСК ДВИГАТЕЛЕЙ ПРАВ
	t_eng_sel_start = true;       //  Тумблер ДВ положении запуск
	t_eng_sel_scroll = false;      //  Тумблер ДВ в положении ПРОКРУТ
	t_eng_left_ign_check = false;  //  Тумблер ПРОВЕРКА ЗАЖИГ ЛЕВ
	t_eng_right_ign_check = false; //  Тумблер ПРОВЕРКА ЗАЖИГ ПРАВ
	t_fuel_left_close = true;     //  Тумблер ПЕРЕКР ЛЕВ ТОПЛИВНЫЙ КРАН
	t_fuel_right_close = true;    //  Тумблер ПЕРЕКР ПРАВ ТОПЛИВНЫЙ КРАН
	t_fuel_bypass_close = true;   //  Тумблер ПЕРЕКР ОБХОДНОЙ БАКОВ
	t_fuel_perepusk = true;       //  Тумблер ПЕРЕПУСК
	t_fpump_refil = true;         //  Тумблер НАСОС ЗАПРАВ
	t_fpump_contr = true;         //  Тумблер НАСОС КОНТР
	t_fpump_consum = true;        //  Тумблер АИ-9В НАСОС РАСХОД
	t_fpump_left   = true;          //  Тумблер АИ-9В НАСОС ЛЕВЫЙ
	t_fpump_right  = true;         //  Тумблер АИ-9В НАСОС ПРАВЫЙ
}

quint8 CentralElectricPult::get_sw_contr_sensors()
{
	if (t_contr_sensors_0) { return 0; }
	if (t_contr_sensors_1) { return 1; }
	if (t_contr_sensors_2) { return 2; }
	if (t_contr_sensors_3) { return 3; }
	if (t_contr_sensors_4) { return 4; }
	if (t_contr_sensors_5) { return 5; }
	if (t_contr_sensors_6) { return 6; }
	if (t_contr_sensors_7) { return 7; }

	return 0;
}

void CentralElectricPult::clr_sw_contr_sensors()
{
	t_contr_sensors_0 = false;
	t_contr_sensors_1 = false;
	t_contr_sensors_2 = false;
	t_contr_sensors_3 = false;
	t_contr_sensors_4 = false;
	t_contr_sensors_5 = false;
	t_contr_sensors_6 = false;
	t_contr_sensors_7 = false;
}

void CentralElectricPult::get_sw_contr_sensors(int val)
{
	clr_sw_contr_sensors();
	switch (val) {
	case 1:
		t_contr_sensors_1 = true;
		break;
	case 2:
		t_contr_sensors_2 = true;
		break;
	case 3:
		t_contr_sensors_3 = true;
		break;
	case 4:
		t_contr_sensors_4 = true;
		break;
	case 5:
		t_contr_sensors_5 = true;
		break;
	case 6:
		t_contr_sensors_6 = true;
		break;
	case 7:
		t_contr_sensors_7 = true;
		break;
	default:
		break;
	}
}

quint8 LeftPanelElectroPult::get_consumers()
{
	if (t_A_offret)       { sw_consumers = 0; return 0;	}
	if ( t_A_blade_1 )    { sw_consumers = 1; return 1; }
	if ( t_A_blade_2 )    { sw_consumers = 2; return 2; }
	if ( t_A_blade_3 )    { sw_consumers = 3; return 3; }
	if ( t_A_blade_4 )    { sw_consumers = 4; return 4; }
	if ( t_A_blade_5 )    { sw_consumers = 5; return 5; }
	if ( t_A_blade_tail ) { sw_consumers = 6; return 6; }
	if ( t_A_glass )      { sw_consumers = 7; return 7; }
	if ( t_A_PZU_l )      { sw_consumers = 8; return 8; }
	if ( t_A_PZU_r )      { sw_consumers = 9; return 9; }
	return sw_consumers;
}

quint8 RightPilotBoard::get_dop_fuel()
{
	if (t_fuel_low_left) { return 0; }
	if (t_fuel_low_right) { return 1; }
	if (t_fuel_hi_left) { return 2; }
	if (t_fuel_hi_right) { return 3; }
	return 0;
}

quint8 ElectroPanelElectroPult::get_ctrl()
{
	if (t_ctrl_vu1) { sw_ctrl = 1; return 1; }
	if (t_ctrl_vu2) { sw_ctrl = 2; return 2; }
	return sw_ctrl;
}

quint8 ElectroPanelElectroPult::get_dc_volt_mode()
{
	if (t_dc_volt_mode_0) { sw_dc_volt_mode = 0; return 0; }
	if (t_dc_volt_mode_1) { sw_dc_volt_mode = 1; return 1; }
	if (t_dc_volt_mode_2) { sw_dc_volt_mode = 2; return 2; }
	if (t_dc_volt_mode_3) { sw_dc_volt_mode = 3; return 3; }
	if (t_dc_volt_mode_4) { sw_dc_volt_mode = 4; return 4; }
	if (t_dc_volt_mode_5) { sw_dc_volt_mode = 5; return 5; }
	if (t_dc_volt_mode_6) { sw_dc_volt_mode = 6; return 6; }
	if (t_dc_volt_mode_7) { sw_dc_volt_mode = 7; return 7; }
	if (t_dc_volt_mode_8) { sw_dc_volt_mode = 8; return 8; }
	if (t_dc_volt_mode_9) { sw_dc_volt_mode = 9; return 9; }
	return sw_dc_volt_mode;
}

quint8 ElectroPanelElectroPult::get_ac_volt_mode()
{
	if (t_ac_volt_mode_pts_gen1a) { sw_ac_volt_mode = 0; return 0; }
	if (t_ac_volt_mode_pts_gen1b) { sw_ac_volt_mode = 1; return 1; }
	if (t_ac_volt_mode_pts_gen1c) { sw_ac_volt_mode = 2; return 2; }
	if (t_ac_volt_mode_aer_gen1a) { sw_ac_volt_mode = 3; return 3; }
	if (t_ac_volt_mode_aer_gen1b) { sw_ac_volt_mode = 4; return 4; }
	if (t_ac_volt_mode_aer_gen1c) { sw_ac_volt_mode = 5; return 5; }
	return sw_ac_volt_mode;
}

BoardSystem::BoardSystem(Helicopter_core* Helicopter)
	:Helicopter(Helicopter)
{
	timer_05 = new QTimer(this);
	timer_05->setInterval(500);
	connect(timer_05, SIGNAL(timeout()), this, SLOT(updateTimer()));
	_VSU = new VSU(Helicopter); // Класс отвечает за АИ9 вертолета
	_Pneumatic = new Pneumatic(Helicopter);
	_Hydraulic = new Hydraulic(Helicopter);
	_Electrics = new Electrics(Helicopter); // Класс отвечает за электрические системы вертолета
	_Engine_1 = new Engine(Helicopter); // Класс отвечает за двигатели вертолета
	_Engine_2 = new Engine(Helicopter); // Класс отвечает за двигатели вертолета
	enc_course = new Encoder(10.f, 0.1f);
	enc_roll = new Encoder(10.f, 0.1f);
	enc_pitch = new Encoder(10.f, 0.1f);
	enc_orlan_1 = new Encoder(10.f, 0.1f);
	enc_orlan_2 = new Encoder(10.f, 0.1f);
	enc_spuu52 = new Encoder(10.f, 0.1f);
	reset();
}

BoardSystem::~BoardSystem()
{
	qDebug() << __FUNCTION__;
	delete _Engine_2;
	delete _Engine_1;
	delete _Electrics;
	delete _Hydraulic;
	delete _Pneumatic;
	delete _VSU;
	delete timer_05;
}

void BoardSystem::reset()
{
	b_light_cab = false;  // подсветка приборов и кабины при включении аккумулятора
	b_blink = false;      // магалка для приборов, будет менять свое состояние раз в полсекунды
	// 
	b_autopilot = false;  // автопилот включен
	b_autopilot_dir = false;  // автопилот включен  направления
	b_autopilot_alt = false;  // автопилот включен высоты
	b_break_press = false;
	b_break_release = true;

	f_roll = 0.f;         // крен ЛА
	f_pitch = 0.f;        // тангаж ЛА
	f_course = 0.f;       // направлене ЛА

	f_ms_roll = 0.f;         // крен системы подвжности
	f_ms_pitch = 0.f;        // тангаж системы подвижности
	f_ms_course = 0.f;       // направлене системы подвижности
	eng_automat_on = false; // работает автомат двигателя
	ta_starter_on = false; // стартер работает
	NV = 0.;              // обороты несущего винта в %
	b_POS = false;
	counter38_5 = 0;

	pps[0] = true;
	pps[1] = true;

	enc_course->reset();
	enc_roll->reset();
	enc_pitch->reset();
	enc_orlan_1->reset();
	enc_orlan_2->reset();
	enc_spuu52->reset();
}

void BoardSystem::updateTimer() // таймер 0.5сек
{
	b_blink = !b_blink;
	counter38_5 = (counter38_5 < 77) ? ++counter38_5 : 0;
	u_pos_switch = (counter38_5 == 77) ? ++u_pos_switch : u_pos_switch;
	u_pos_switch = (u_pos_switch < 4) ? u_pos_switch : 0;
}
void BoardSystem::Start()
{
	reset();
	_VSU->Start();
	_Engine_1->Start();
	_Engine_2->Start();
	_Electrics->Start();
	_Hydraulic->Start();
	_Pneumatic->Start();
	timer_05->start();
}

void BoardSystem::Stop()
{
	reset();
	_VSU->Stop();
	_Engine_1->Stop();
	_Engine_2->Stop();
	_Electrics->Stop();
	_Hydraulic->Stop();
	_Pneumatic->Stop();
	timer_05->stop();
}

// TODO уточнить напавление, знак коррекции платформы
float BoardSystem::get_vis_roll() const
{
	return  f_roll - f_ms_roll;
}
float BoardSystem::get_vis_pitch() const
{
	return  f_pitch - f_ms_pitch;
}
float BoardSystem::get_vis_course() const
{
	return f_course;
}

float BoardSystem::get_roll() const
{
	return f_roll;
}
float BoardSystem::get_pitch() const
{
	return f_pitch;
}
float BoardSystem::get_course() const
{
	return f_course;
}

float BoardSystem::get_ms_roll() const
{
	return f_roll;
}

float BoardSystem::get_ms_pitch() const
{
	return f_pitch;
}

float BoardSystem::get_ms_course() const
{
	return 0.f;
}

// тут раскидываем на визуальную и платформенную часть
void BoardSystem::set_roll(float f)
{
	f_roll = f;
	f_ms_roll = platform ? f : 0.f;
}

void BoardSystem::set_pitch(float f)
{
	f_pitch = f;
	f_ms_pitch = platform ? f : 0.f;
}

void BoardSystem::set_course(float f)
{
	f_course = f;
}

void BoardSystem::set_ms_roll(float f)
{
	f_ms_roll = f;
}

void BoardSystem::set_ms_pitch(float f)
{
	f_ms_pitch = f;
}
//тут идут функции пересчета координат из ММ в ГК принятую в визуалке
double BoardSystem::CalcLat(float Xg) {
	return StartLatitude + Xg * (1. / 111234.866);
}

double BoardSystem::CalcLon(float Zg) {
	return StartLongitude + Zg * (1. / (111321. * cos(Latitude * M_PI / 180)));
}

double BoardSystem::CalcAlt(float Yg) {
	//static float cor = 2.52f;
	return static_cast<double> (Yg - Helicopter->runway_.base_alt);
}
double BoardSystem::CalcDistance(double lat1, double lon1, double alt1, double lat2, double lon2) {
	const double R = 6371.0; // Радиус Земли в километрах
	double lat1_rad = deg2rad(lat1);
	double lon1_rad = deg2rad(lon1);
	double lat2_rad = deg2rad(lat2);
	double lon2_rad = deg2rad(lon2);

	double dlat = lat2_rad - lat1_rad;
	double dlon = lon2_rad - lon1_rad;

	double a = sin(dlat / 2) * sin(dlat / 2) +
		cos(lat1_rad) * cos(lat2_rad) *
		sin(dlon / 2) * sin(dlon / 2);
	double c = 2 * atan2(sqrt(a), sqrt(1 - a));
	double distance = R * c;

	// Учитываем высоту первой точки
	alt1 = 0.; // пока оставляем без учета высоты
	distance = sqrt(distance * distance + alt1 * alt1);

	// Конвертируем расстояние из километров в морские мили
	return distance / 1.852;
}

float BoardSystem::CalcGlis(float Hgeo, float Sgeo) {
	float angle = 2.25f * (M_PI / 180.0); // угол медианы ILS маяков
	float Hradio = (Sgeo * 1000.f) * tan(angle);
	float res = Hradio - Hgeo;
	if (res < -10.f)
		res = -10.f;
	if (res > 10.f)
		res = 10.f;
	return res / -10.f;
}

void VSU::reset()
{
	time_ = 0.f;
	state_ = State::OFF;
	temp_ = temp_0;
	temp_comp = temp_0;
	presure_ = presure_0;
	ta_automat_on = false;
	ta_p_oil_norm = false;
	ta_rpm_norm = false;
	ta_rpm_max = false;
	Helicopter->audio_->Stop(5);
	Helicopter->audio_->Stop(6);
	Helicopter->audio_->Stop(7);
}

VSU::VSU(Helicopter_core* Helicopter) :
	OnBoardTimer(nullptr),
	Helicopter(Helicopter)
{
	reset();
	OnBoardTimer = new QTimer(this);
	OnBoardTimer->setInterval(COMPSTEP);
	connect(OnBoardTimer, SIGNAL(timeout()), this, SLOT(on_Compute()));
	OnBoardTimer->start();
}

QDebug operator<<(QDebug out, const VSU::State& state_) {
	QString name;
	switch (state_) {
	case VSU::State::OFF:
		name = "OFF";
		break;
	case VSU::State::STARTING:
		name = "STARTING";
		break;
	case VSU::State::START_FALSE:
		name = "START_FALSE";
		break;
	case VSU::State::SCROLL:
		name = "SCROLL";
		break;
	case VSU::State::WORK:
		name = "WORK";
		break;
	case VSU::State::STOPING:
		name = "STOPING";
		break;
	}
	out.nospace() << name;
	return out;
};

void VSU::start_TA()
{
	if (state_ == State::OFF)
	{
		time_ = 0;
		temp_comp = temp_0;
		state_ = State::STARTING;
		Helicopter->audio_->PlayOnce(6);
	}
}

void VSU::false_start_TA()
{
	if (state_ == State::OFF) {
		time_ = 0;
		state_ = State::START_FALSE;
		Helicopter->audio_->PlayOnce(5);
	}
}

void VSU::scroll_TA()
{
	if (state_ == State::OFF) {
		time_ = 0;
		state_ = State::SCROLL;
		Helicopter->audio_->PlayOnce(5);
	}
}

void VSU::stop_TA()
{
	qDebug( ) << __FUNCTION__ ;
	if (state_ != State::OFF || state_ == State::STOPING) {
		Helicopter->audio_->PlayOnce(8);
		Helicopter->audio_->Stop(5);
		Helicopter->audio_->Stop(6);
		Helicopter->audio_->Stop(7);

	}
	time_ = 0;
	state_ = State::STOPING;
}

void VSU::starting()
{
	static int dt[] = { 10,10,5,5,5,5,5,5,5,5,3,3,2,0,0,0,-2,-2,-3,-3,-3,-3,-2,-3,-3,-2,0,0};
	assert(time_ < 25000);
	time_ += d_time;
	ind_sim_time += 0.16f;
	if (state_ == State::STARTING && ind_sim_time > 1.f)
	{
		ta_automat_on = ta_automat_on ? false : true;
		ind_sim_time = 0.f;
	}
	// ta_automat_on = true;
	dT = dt[time_ / 1000];
	temp_comp += dT;
	presure_ += dP;
	if (presure_ > 2.2f) presure_ = 2.2f;
	//temp_ = temp_ < temp_comp ? temp_comp : temp_; // температура не повышается, пока остаточное тепло превышает подведеное
	temp_ = temp_comp;
	if (temp_ >= 100) ta_p_oil_norm = true;
	if (time_ >= stage_4) state_ = State::WORK;
}

void VSU::stoping()
{
	ta_rpm_norm = false;
	presure_ -= dP;
	if (presure_ < presure_0) presure_ = presure_0;
	time_ += d_time;
	if (temp_ > 400) dT = 10;
	else if (temp_ > 300) dT = 5;
	else if (temp_ > 200) dT = 3;
	else if (temp_ > 150) dT = 1;
	if (time_ >= 15000) state_ = State::OFF;

	temp_ -= temp_ > temp_0 ? dT : 0;
	if (temp_ <= 100) ta_p_oil_norm = false;
}

void VSU::on_Compute()
{
	switch (state_)
	{
	case State::OFF:
		ta_automat_on = false;
		ta_p_oil_norm = false;
		ta_rpm_norm = false;
		ta_rpm_max = false;
		dT = 0.2f;
		presure_ = presure_0;
		temp_ -= temp_ > temp_0 ? dT : 0;
		if (temp_ <= 100) ta_p_oil_norm = false;
		break;
	case State::STARTING:
		starting();
		break;
	case State::START_FALSE:
	case State::SCROLL:
		time_ += COMPSTEP;
		if (time_ >= time_scroll) {
			state_ = State::STOPING;
			Helicopter->audio_->PlayOnce(8); // /wav/TA_stop.wav
			Helicopter->audio_->Stop(5); // /wav/TA_scroll.wav
		}
		break;
	case State::WORK:
		ta_automat_on = true;
		ta_p_oil_norm = true;
		ta_rpm_norm = true;
		Helicopter->audio_->Play(7, true); ///wav/TA_work.wav
		Helicopter->audio_->Stop(6); // /wav/TA_start.wav
		break;
	case State::STOPING:
		stoping();
	}
}

void Engine::reset()
{
	qDebug( ) << __FUNCTION__ ;
	time_ = 0;
	state_ = State::OFF;
	eng = 0.;
	//Helicopter->audio_->Stop(2);
	//Helicopter->audio_->Stop(9);
	//Helicopter->audio_->Stop(10);
	//Helicopter->audio_->Stop(11);
	//Helicopter->audio_->Stop(12);
	//Helicopter->audio_->Stop(13);
	//Helicopter->audio_->Stop(14);
	//Helicopter->audio_->Stop(15);
	//Helicopter->audio_->Stop(16);
}

Engine::Engine(Helicopter_core* Helicopter) : Helicopter(Helicopter)
{
	OnBoardTimer = new QTimer(this);
	OnBoardTimer->setInterval(COMPSTEP);
	connect(OnBoardTimer, SIGNAL(timeout()), this, SLOT(on_Compute()));
	OnBoardTimer->start();
	state_ = State::OFF;
};

// включили стартер на ФМ идет прокрутка
void Engine::start_eng()
{
	if (state_ == State::OFF)
	{
		time_ = 0;
		state_ = State::STARTING;
		//Helicopter->audio_->Stop(Helicopter_audio::ENG_START);
		//Helicopter->audio_->PlayOnce(Helicopter_audio::ENG_START);
	}
};
// подали топливо на фм идет - двигатель включен, переход в WORK по 60% 
void Engine::fuel_eng()
{
	if (state_ == State::OFF)
	{
		time_ = 0;
		state_ = State::STARTING_1;
		//Helicopter->audio_->Stop( Helicopter_audio::ENG_START );
		//Helicopter->audio_->PlayOnce(Helicopter_audio::ENG_START);
	}
	if (state_ == State::STARTING)
	{
		state_ = State::STARTING_1;
	}
};
void Engine::scroll_eng()
{
	if (state_ == State::OFF)
	{
		time_ = 0;
		state_ = State::SCROLL;
		//Helicopter->audio_->Stop( Helicopter_audio::ENG_START );
		//Helicopter->audio_->PlayOnce(Helicopter_audio::ENG_START);
	}
};
void Engine::false_start_eng()
{
	if (state_ == State::OFF)
	{
		time_ = 0;
		state_ = State::START_FALSE;
		//Helicopter->audio_->Stop( Helicopter_audio::ENG_START );
		//Helicopter->audio_->PlayOnce(Helicopter_audio::ENG_START);
	}
};
void Engine::stop_eng()
{
	// остановка при запуске
	if (state_ == State::START_FALSE || state_ == State::SCROLL || state_ == State::STARTING || state_ == State::STARTING_1)
	{
		time_ = 0;
		state_ = State::STOPING;
		//Helicopter->audio_->PlayOnce(Helicopter_audio::ENG_1_DOWN);
		//Helicopter->audio_->Stop(Helicopter_audio::ENG_START);
	}
	if (state_ == State::WORK)
	{
		time_ = 0;
		state_ = State::STOPING;
		//Helicopter->audio_->PlayOnce(Helicopter_audio::ENG_2_DOWN);
		//_Helicopter->audio_->Stop(Helicopter_audio::ENG_START);
	}
};

void Engine::on_Compute()
{
	switch (state_)
	{
	case State::OFF:
		break;
	case State::STARTING:
		// не уверен, что это правильное решение, управлять полями владельца, зато не загромаждает тот код
		Helicopter->_BoardSystem->eng_automat_on = true;
		if (time_ >= 700)
			Helicopter->_BoardSystem->ta_starter_on = true; // стартер включиться с задержкой
		if (time_ >= 55000) { // время работы стартера 55c
			Helicopter->_BoardSystem->eng_automat_on = false;
			Helicopter->_BoardSystem->ta_starter_on = false;
			stop_eng();
		};
		time_ += COMPSTEP;
		break;
	case State::STARTING_1:
		Helicopter->_BoardSystem->eng_automat_on = true;
		if (time_ >= 700)
			Helicopter->_BoardSystem->ta_starter_on = true; // стартер включиться с задержкой
		if (time_ >= 55000) { // время работы стартера 55c
			Helicopter->_BoardSystem->eng_automat_on = false;
			stop_eng();
		};
		time_ += COMPSTEP;
		if (eng > 60.f) { // раскрутка двигателя до 60%
			Helicopter->_BoardSystem->ta_starter_on = false;
			state_ = State::WORK;
		};
		break;
	case State::START_FALSE:
		[[fallthrough]];
	case State::SCROLL:
		Helicopter->_BoardSystem->eng_automat_on = true;
		Helicopter->_BoardSystem->ta_starter_on = true;
		if ((time_ += COMPSTEP) >= 55000) { stop_eng(); }; // время работы стартера 55c
		break;
	case State::WORK:
		time_ += COMPSTEP;
		if (time_ >= 28000) { // По окончании цикла работы панели табло АВТОМАТ.ВКЛЮЧЕН должен погаснуть через 25-30 с
			Helicopter->_BoardSystem->eng_automat_on = false;
			time_ = 0;
		};
		break;
	case State::STOPING:
		Helicopter->_BoardSystem->eng_automat_on = false;
		Helicopter->_BoardSystem->ta_starter_on = false;
		if ((time_ += COMPSTEP) >= 60000) { time_ = 0;  state_ = State::OFF; }; // остановка двигателя (для звука) 60с, подобрать длительность
		break;
	}
}

/*
		WORK=0,         // работает
		OFF=1,            // остановлен
		STARTING=0,       // запускается
		START_FALSE=3,    // ложный запуск
		SCROLL=4,         // прокрутка
		STOPING=1,        // останавливается
*/
// значение для FM 0 - работает; 1 - выключен; 2 - нормальная работа; 3 - прокрутка; 4 - ложный запуск
// [hack. необходимость запуска через false_start это ограничение FM
int  Engine::mode_eng()
{
	int ret = -1;
	switch (state_)
	{
	case State::WORK: ret = 0; break;
	case State::OFF: ret = 1; break;            // остановлен
	case State::STARTING: ret = 3; break;       // запускается, ФМ говорим, что это ложный запуск, пусть крутится без топлива
	case State::STARTING_1: ret = 0; break;     // запускается, ФМ говорим, что это нормальный запуск запуск, топливо подали
	case State::START_FALSE: ret = 3; break;    // ложный запуск
	case State::SCROLL: ret = 4; break;         // прокрутка
	case State::STOPING: ret = 1; break;        // останавливается
	}
	return ret;
};

QDebug operator<<(QDebug out, const Engine::State& state_) {
	QString name;
	switch (state_) {
	case Engine::State::OFF:
		name = "OFF";
		break;
	case Engine::State::STARTING:
		name = "STARTING";
		break;
	case Engine::State::STARTING_1:
		name = "STARTING_1";
		break;
	case Engine::State::START_FALSE:
		name = "START_FALSE";
		break;
	case Engine::State::SCROLL:
		name = "SCROLL";
		break;
	case Engine::State::WORK:
		name = "WORK";
		break;
	case Engine::State::STOPING:
		name = "STOPING";
		break;
	}
	out.nospace() << name;
	return out;
};

void Hydraulic::reset()
{
	time_ = 0;
	main = 0.f;    // давление в главной системе
	second = 0.f;  // давление в резервной системе
	broken_main = false;   // выход из строя насоса основной системы
	broken_second = false;   // выход из строя насоса резервной системы
	broken_akk = false;   // выход из строя аккумулятора системы
}

Hydraulic::Hydraulic(Helicopter_core* Helicopter) : Helicopter(Helicopter) {
	OnBoardTimer = new QTimer(this);
	OnBoardTimer->setInterval(COMPSTEP);
	connect(OnBoardTimer, SIGNAL(timeout()), this, SLOT(on_Compute()));
	OnBoardTimer->start();
	reset();
}

void Hydraulic::on_Compute( ) {

	broken_main = Helicopter->instructor->fault.HidroMain;
	broken_second = Helicopter->instructor->fault.HidroSecond;
	int broken_diafrag = Helicopter->instructor->fault.HidroDiafrag ? 2 : 1;

	if ( Helicopter->_BoardSystem->NV > 30 ) // включились насосы
	{
		if ( Helicopter->cep.t_hidro_main )  // включен тумблер
		{
			if ( ( main < min_pres ) )
			{    // дваление меньше минимального просто качаем
				main_pump = true;
			}
			if ( main >= max_pres )
			{
				main_pump = false;
			}
		}
		else
		{
			main_pump = false;
		}
	}
	else {
		main = 0;
	}

	if ( broken_main )	// насос сломан инструктором
	{
		main_pump = false;
	}

	if ( main_pump ) // насос качает
	{
		main += main < air_pres ? dP * 3 * broken_diafrag : dP;
	}

	if ( Helicopter->_BoardSystem->NV > 40 ) // включились с задежкой резервный насос
	{
		if ( Helicopter->cep.t_hidro_second ) 	// включен тумблер
		{
			if ( ( main < switch_pres ) || ( ( second < min_pres ) && !Helicopter->cep.i_hidro_main ) )
			{ // давление меньше минимального просто качаем или основной выключен
				second_pump = true;
			}
			if ( second >= max_pres )
			{
				second_pump = false;
			}
		}
		else
		{
			second_pump = false;
		}
	}
	else {
		second = 0;
	}

	if ( broken_second ) // насос сломан инструктором
	{
		second_pump = false;
	}

	if ( Helicopter->cep.t_off_second_hidro ) // отключение резевной системы кнопкой
	{
		second_pump = false;
		second -= 5;
	}

	if ( second_pump ) // насос качает
	{
		//main = 50;
		second += second < air_pres ? dP * 3 * broken_diafrag : dP;
	}

	// расход есть только когда работает данный насос, если резервная включилась работает всегда, пока ее не вырубят
	if ((Helicopter->cep.i_hidro_main || !Helicopter->cep.t_hidro_main || broken_main) && !Helicopter->cep.i_hidro_second)
		main -= rashod * broken_diafrag * ( main < air_pres ? 3 : 1 );

	if ( Helicopter->cep.i_hidro_second || !Helicopter->cep.t_hidro_second || broken_second )
		second -= rashod * broken_diafrag * ( second < air_pres ? 3 : 1 );

	main = qBound( 0.f, main, 68.f );
	second = qBound( 0.f, second, 68.f );
}

Electrics::Electrics(Helicopter_core* Helicopter) : Helicopter(Helicopter) {
	OnBoardTimer = new QTimer(this);
	OnBoardTimer->setInterval(COMPSTEP);
	connect(OnBoardTimer, SIGNAL(timeout()), this, SLOT(on_Compute()));
	OnBoardTimer->start();
	reset();
}

void Electrics::reset()
{
	// постоянное напряжение
	Uakk1 = 0.;  // шина постоянного питание с аккумулятора 1
	Uakk2 = 0.;  // шина постоянного питание с аккумулятора 2
	Uakk = 0.;  // шина постоянного питание с аккумуляторов
	Uvu1 = 0.;   // постоянного питание выпрямителя1
	Uvu2 = 0.;   // постоянного питание выпрямителя2
	Uvu = 0.;   // шина постоянного питание выпрямителей
	Usgu = 0.;    // резервный генератор на 29 вольт
	Isgu = 0.;   // Ток резевного генератора

	// переменное напряжение
	Ugen36 = 0.;    // шина переменного трехфазного питания 36в 400Гц
	Ugen36r = 0.;    // шина переменного трехфазного питания резервная 36в 400Гц
	Ugen115 = 0.;   // шина переменного однофазного питяния 115в 400Гц
	Ugen115r = 0.;   // шина переменного однофазного питяния резервная 115в 400Гц
	Ugen204 = 0.;   // шина переменного трехфазного питяния 204в 400Гц
	Udim = 0.;    // шина переменного однофазного питяния 36в 400Гц
}

void Electrics::on_Compute()
{
	t_akk = Helicopter->ep_elp.t_akkum_1 || Helicopter->ep_elp.t_akkum_2;
	// если обороты больдше 86% и включены генераторы. Нормальный режим работы
	if ((Helicopter->_BoardSystem->NV > 86) && (Helicopter->ep_elp.t_gen_1 || Helicopter->ep_elp.t_gen_2))
	{
		Ugen204 = 204.f;
		Ugen115 = 115.f;
		Ugen115r = 115.f;
		Ugen36 = 36.f;
		Ugen36r = 36.f;
		Igen1 = 60.f;
		Igen2 = 100.f;
	}
	else
	{
		Ugen204 = 0.f;
		Ugen115 = 0.f;
		Ugen115r = 0.f;
		Ugen36 = 0.f;
		Ugen36r = 0.f;
		Igen1 = 0.f;
		Igen2 = 0.f;
	}
	// подано аэродромное питание оно переопределяет работу генераторов выше
	// TODO надо его как то отключть при взлете и выдать большую ошибку пилота за оторваный штепсель
	if ( Helicopter->ep_elp.t_aero_pit_ac && Helicopter->ep_elp.i_rap_on && Hgeo < 3 && (Helicopter->ep_elp.t_aero_pit_dc || t_akk))
	{
		Ugen204 = 204.f;
		Ugen115 = 115.f;
		Ugen115r = 115.f;
		Ugen36 = 36.f;
		Ugen36r = 36.f;
		Igen1 = 0.f;
		Igen2 = 0.f;
	}
	// если генераторы отключен, то тока нет
	if ( Helicopter->ep_elp.t_gen_1 == false  && Helicopter->ep_elp.t_gen_1_contr == false )
	{
		Igen2 += Igen1;
		Igen1 = 0;
	}
	if ( Helicopter->ep_elp.t_gen_2 == false && Helicopter->ep_elp.t_gen_2_contr == false )
	{
		Igen1 += Igen2;
		Igen2 = 0;
	}

	// моделирование отказов генератора можно объединить с выключателями, но лучше отдельной проверкой
	if ( Helicopter->instructor->fault.Gen1 == true )
	{
		Igen2 += Igen1;
		Igen1 = 0;
	}
	if ( Helicopter->instructor->fault.Gen2 == true )
	{
		Igen1 += Igen2;
		Igen2 = 0;
	}
	// если есть напряжение на линии генератора и включены ВУ
	Uvu1 = ( Ugen204 >= 200 && ( Helicopter->ep_elp.t_ACDC_1 ) ) ? 28.f : 0.f; // что бы визуально отличать работу разных каналов для отладки
	Uvu2 = ( Ugen204 >= 200 && ( Helicopter->ep_elp.t_ACDC_2 ) ) ? 29.f : 0.f;
	// если ВУ1 включен, то он дает ток
	// TODO надо сделать ток зависимым от клиентов, пока просто назначаем
	Ivu1 = ( Ugen204 >= 200 && ( Helicopter->ep_elp.t_ACDC_1 ) ) ? 130.f : 0.f;
	Ivu2 = ( Ugen204 >= 200 && ( Helicopter->ep_elp.t_ACDC_2 ) ) ? 120.f : 0.f;

	// моделирование отказов выпрямителей
	if ( Helicopter->instructor->fault.VU1 == true )
	{
		Helicopter->_BoardSystem->_Electrics->Ivu1 = 0;
		Helicopter->_BoardSystem->_Electrics->Uvu1 = 0;
		Helicopter->_BoardSystem->_Electrics->Uvu = std::max( Helicopter->_BoardSystem->_Electrics->Uvu1, Helicopter->_BoardSystem->_Electrics->Uvu2 );
	}
	if ( Helicopter->instructor->fault.VU2 == true )
	{
		Helicopter->_BoardSystem->_Electrics->Ivu2 = 0;
		Helicopter->_BoardSystem->_Electrics->Uvu2 = 0;
		Helicopter->_BoardSystem->_Electrics->Uvu = std::max( Helicopter->_BoardSystem->_Electrics->Uvu1, Helicopter->_BoardSystem->_Electrics->Uvu2 );
	}

	// если включен один из аккумуляторов
	Uakk1 = 24.3f;   // что бы визуально отличать работу разных каналов для отладки
	Uakk2 = 25.3f;

	Iakk1 = (Helicopter->ep_elp.t_akkum_1) ? 17.f : 0.f;   // что бы визуально отличать работу разных каналов для отладки
	Iakk2 = (Helicopter->ep_elp.t_akkum_2) ? 16.f : 0.f;
	// запуск ВСУ стартовый ток стартера
	if (Helicopter->_BoardSystem->_VSU->is_starting())
	{
		Iakk1 = Iakk2 = 200;
		Usgu = 0.f;
		Isgu = 0.f;
	}
	Ivu1 = 130.f;
	Ivu2 = 120.f;
	// TODO тут немного сложнее, акк подключается к ВУ после поднятия тока выше 15 ампер на канал
	// но ток пока не моделируется
	if (Uakk1 < Uvu1)
	{
		Uakk1 = Uvu1; //TODO тут надо сделать зарядку аккумуляторов, пока просто выравниваем напряжение
		Iakk1 = -2.5f;  // акк заряжается
	}
	// сюда попадаем если свой ВУ не работает, то проверяем работает ли другой
	else if (Uakk1 < Uvu2)
	{
		Uakk1 = Uvu2; // запитываем шину акк1  от ВУ2
		Iakk1 = -2.5f;  // акк заряжается
		Ivu2 += Ivu1;
		Ivu1 = 0;
	}
	if (Uakk2 < Uvu2)
	{
		Uakk2 = Uvu2; //TODO тут надо сделать зарядку аккумуляторов, пока просто выравниваем напряжение
		Iakk2 = -2.5f;  // акк заряжается
	}
	// сюда попадаем если свой ВУ не работает, то проверяем работает ли другой
	else if (Uakk2 < Uvu1)
	{
		Uakk2 = Uvu1; // запитываем шину акк2  от ВУ1
		Iakk2 = -2.5f;  // акк заряжается
		Ivu1 += Ivu2;
		Ivu2 = 0;
	}
	if (Uvu1 < 10) // ВУ не работает вырубаем его ток
	{
		Ivu1 = 0.f;
	}
	if (Uvu2 < 10)
	{
		Ivu2 = 0.f;
	}
	if (Helicopter->_BoardSystem->_VSU->is_working())
	{
		Usgu = 27.f;
	}

	if (Helicopter->instructor->fault.aero_pts)
		Helicopter->ep_elp.i_aero_pit_dc = true;
	else
		Helicopter->ep_elp.i_aero_pit_dc = false;

	//включен АЭР ПТ и не работают оба ВУ
	if (Helicopter->ep_elp.i_aero_pit_dc && Helicopter->ep_elp.t_aero_pit_dc && ( Uvu1 < 10 ) && ( Uvu2 < 10 ) && (Helicopter->_BoardSystem->_Electrics->Hgeo < 3 ) )
	{
		Uakk = Uakk1 = Uakk2 = Uvu = 27.f;
		Iakk1 = Iakk2 = -2.5;
	}
	else
	{
		//включен резервный ренератор и не работают оба ВУ
		if ( Helicopter->ep_elp.t_res_gen && ( Uvu1 < 10 ) && ( Uvu2 < 10 ) )
		{
			if ( Usgu > 24.f )
			{
				Uakk = Uakk1 = Uakk2 = Uvu = Usgu;
				Isgu = Iakk1 + Iakk2;
				Iakk1 = Iakk2 = 8;
			}
		}
		else
		{
			Uakk = ( Helicopter->ep_elp.t_akkum_1 || Helicopter->ep_elp.t_akkum_2 ) ? fmax( Uakk1, Uakk2 ) : 0;
			Isgu = 0;
			Uvu = 0;
		}
	}

	// если генераторы отключен, то тока нет
	if (Helicopter->ep_elp.t_gen_1 == false && Helicopter->ep_elp.t_gen_1_contr == false)
	{
		Igen2 += Igen1;
		Igen1 = 0;
	}
	if (Helicopter->ep_elp.t_gen_2 == false && Helicopter->ep_elp.t_gen_2_contr == false )
	{
		Igen1 += Igen2;
		Igen2 = 0;
	}
	// если аккумуляторы включенв, а тока нет, то включаем лампочки Ген. отключен
	//Helicopter->ep_elp.i_gen_1_off = ((Helicopter->ep_elp.t_akkum_1 || Helicopter->ep_elp.t_akkum_2) && (Igen1 < 10)) ? true : false;
	//Helicopter->ep_elp.i_gen_2_off = ((Helicopter->ep_elp.t_akkum_1 || Helicopter->ep_elp.t_akkum_2) && (Igen2 < 10)) ? true : false;

	// Если включен ПОС800
	//Helicopter->ep_elp.i_ptc_on = Helicopter->ep_elp.t_pts_man || (Helicopter->ep_elp.t_pts_auto && Helicopter->ep_elp.i_gen_1_off && Helicopter->ep_elp.i_gen_2_off && !Helicopter->ep_elp.i_rap_on);
	Ugen115r = ((Uakk > 24.f) && (Helicopter->ep_elp.t_pts_man || Helicopter->ep_elp.t_pts_auto)) ? 115.f : Ugen115;
	//Helicopter->ep_elp.i_reser36 = Helicopter->ep_elp.t_r36_man || ( Helicopter->ep_elp.t_r36_auto && Helicopter->ep_elp.i_gen_1_off && Helicopter->ep_elp.i_gen_2_off && !Helicopter->ep_elp.i_rap_on );
	// если есть напряжение на резервной линии генераторной шины 115 и включен ТП36
	if (Ugen115r >= 115 && Helicopter->ep_elp.i_reser36 )
	{
		Udim = 36.f; // напряженние на шине ДИМ ~36 вольт
		Ugen36r = 36.f;
	}
	else
	{
		Udim = 0.f;
		Ugen36r = 0.f;
	}	
}

Pneumatic::Pneumatic(Helicopter_core* Helicopter) : Helicopter(Helicopter)
{
	//OnBoardTimer = new QTimer(this);
	//OnBoardTimer->setInterval(COMPSTEP); 
	//connect(OnBoardTimer, SIGNAL(timeout()), this, SLOT(on_Compute()));
	//OnBoardTimer->start();
	reset();
}

void PilotControl::set_trim()
{
	f_trg_roll = f_roll;
	f_trg_pitch = f_pitch;
}

// get_axis реализуют мертвую зону вокруг точки тригирования, коэффициенты подобраны опытным путем на тренажере Ми17В5
// для других могут отличаться. На данном тренажере, после тригирования ручка и педали отклоняются вправо под действие пружин
// более сильные стоят с левой стороны, особенно заметно на педалях. ЗЫ проблема не в разной силе пружин, а силе тажести
//     \--*----/
float PilotControl::get_roll()
{
	if (fabs(f_roll - f_trg_roll) < dead_zone) return f_trg_roll;
	if (f_roll >= (f_trg_roll + dead_zone)) return f_roll - (dead_zone - 1);
	if (f_roll <= (f_trg_roll - dead_zone)) return f_roll + (dead_zone - 1);
	return f_roll;
}

float PilotControl::get_pitch()
{
	if (fabs(f_pitch - f_trg_pitch) < dead_zone) return f_trg_pitch;
	if (f_pitch >= (f_trg_pitch + dead_zone)) return f_pitch - (dead_zone - 1);
	if (f_pitch <= (f_trg_pitch - dead_zone)) return f_pitch + (dead_zone - 1);
	return f_pitch;
}

// после триммирования педаль уходит влево на 8.9 единиц, создаем МЗ на 9 вправо.
// так же на 2 единицы имеется люфт при даче правой педали за тригерную точку, возможно ее надо убрать, но 9 вправо
// необходимо, что бы вертолет не разворачивало
// return f_yawl + 8; равно разнице между текущим положением ручки и точной трегирования чуть усилено, что бы не было 'скачка' при выходе из МЗ.
float PilotControl::get_yawl()
{
	if ((f_yawl > f_trg_yawl) && ((f_yawl - f_trg_yawl) < dead_zone_pd)) return f_trg_yawl; // МЗ слева от тригирования 
	if ((f_yawl < f_trg_yawl) && (f_trg_yawl - f_yawl) < dead_zone_pd) return f_trg_yawl; // МЗ справа от тригирования
	if (f_yawl > (f_trg_yawl + dead_zone_pd)) return f_yawl - dead_zone_pd;
	if (f_yawl < (f_trg_yawl - dead_zone_pd)) return f_yawl + dead_zone_pd;
	return f_yawl;
}

// TODO после смены курса нужно разобраться с боковым смещением
void DISS_15::fill()
{
	if (state)
	{
		// пройденный путь по осям 
		float dx = x_cur - x_start;
		float dz = z_cur - z_start;
		if (dx == 0 && dz == 0)
		{
			return;
		}

		// переводим градусы в радианы 
		float dir = set_angle * M_PI / 180;

		// пройденный путь 
		float dist = sqrt(dx * dx + dz * dz);

		float beta;
		if (180 < set_angle && set_angle < 360)
		{
			// угол между заданным и текущим напрвлением 
			beta = 2 * M_PI - dir - acos(dx / dist);
			sw_side = (beta < 0) ? 1 : 0;
		}
		else
		{
			beta = dir - acos(dx / dist);
			sw_side = (beta < 0) ? 0 : 1;
		}

		// боковое уклонение
		f_side = dist * sin((beta));
		// БУ влево вправо 

		// путь (по курсу) 
		way_cur = sqrt(dist * dist - f_side * f_side);
		f_way = set_way + way_cur + way_prev;
		f_side = set_side + f_side;
		// ПУТЬ вперед назад 
		sw_way = (f_way < 0) ? 1 : 0;
		//if (std::isnan(f_way) || std::isnan(f_side))
		//{
		//	f_way = f_side = 0.f;
		//}
		// УГОЛ
		f_angle = set_angle;
	}
	else  // DISS отключен
	{
		f_way = set_way;
		f_angle = set_angle;
		f_side = set_side;
		way_prev = 0.f;
	}
}

void DISS_15::off()
{
	state = false;
	set_way = 0;
	set_angle = 0;
	set_side = 0;

	f_side = 0.f;// Боковое уклонение-значение в метрах
	f_way = 0.f;// Путь-значение в метрах
	way_cur = 0.f; // пройденый маршрут после изменения курса
	way_prev = 0.f; // пройденый маршрут до изменения курса
	step_ = 100.f;
	step_a_ = 0.01f;
	pressed_ = 0;
}

void DISS_15::button_proc()
{
	if (t_left) { set_side += step_; ++pressed_; }
	if (t_rght) { set_side -= step_; ++pressed_; }
	if (t_v) { set_way += step_; ++pressed_; }
	if (t_n) { set_way -= step_; ++pressed_; }
	if (t_plus) { set_angle += step_a_; ++pressed_; }
	if (t_minus) { set_angle -= step_a_; ++pressed_; }
	if (pressed_ > 150)
	{
		step_ = 600.f;
		step_a_ = 0.3f;
	}
	if (!(t_left || t_rght || t_v || t_n || t_plus || t_minus))
	{
		step_ = 100.f;
		step_a_ = 0.01f;
		pressed_ = 0;
	}
	else // нажата любая кнопка. это приводит к сбросу начальной возиции
	{
		z_start = z_cur;
		x_start = x_cur;
		way_prev = f_way;
	}
}

bool Stantions::loadBeaconFromFile( const std::string& filename )
{
	std::ifstream inputFile( filename, std::ios::binary );
	std::string line;
	bool ret = false;
	checkBOM( inputFile );

	while ( std::getline( inputFile, line ) ) {
		std::istringstream iss( line );

		float freq, lat, lon;
		std::string id;

		// Извлекаем данные из строки, используя разделитель табуляции
		if ( iss >> freq >> lat >> lon >> id ) {
			station s{ freq, lat, lon, QString( id.c_str( ) ) };
			vs.push_back( s );
			ret = true;
		}
	}

	return ret;
}

// https://ans.kz/ru/ais/eaip
// https://www.ans.kz/AIP/eAIP/2023-06-15-AIRAC/html/index-ru-RU.html
ARK_9::ARK_9()
{
	//vs = Stantions( "Beacon.txt" );
	// TODO пока нет селектора назначаем частоты тут
	//freq_selector1.set_freq( 400.f );
	//freq_selector2.set_freq( 700.f );
}

float ARK_9::get_curs( double latitude, double longitude, double course )
{
	// выбираем рабочуюю частоту в зависимости от селектора Б-Д или 1-2
	freq = !t_rk_chanel ? freq_selector1.getSelectedFrequency() : freq_selector2.getSelectedFrequency();

	qDebug() << "Freq ARK15 =" << freq;
	//	freq = 400.f;
	// ищем подходящий маяк с приемлемой дальностью TODO добавить дальность до маяка
	auto it = std::find_if( vs.begin( ), vs.end( ), [this]( auto& ark ) { return freq == ark.freq; } );

	if ( it == vs.end( ) )
	{
		return 0;
	}
	// определяем напра
	double lat = ( it->lat - latitude );
	double lon = ( it->lon - longitude ) * cos( latitude * GtoR );
	double ARK = atan2( lon, lat ) * Ra;
	ARK -= course; //был минус
	//ARK -= Magnetic::magneticDeclination;
	ARK = norm360( ARK * ( -1 ));
	//if ( ARK < 0 )   ARK += 360;
	//if ( ARK > 360 ) ARK -= 360;
	return ARK;
}

float ARK_9::calc_dist( )
{
	// Формула расстояния между двумия точками в угловых величинах
	return sqrt( pow( x - x_station, 2 ) + pow( y - y_station, 2 ) );
}

quint8 ARK_9::sw_rk_mode( )
{
	if ( t_rk_off ) { return 0; }
	if ( t_rk_com ) { return 1; }
	if ( t_rk_ant ) { return 2; }
	if ( t_rk_ram ) { return 3; }
	return 0;
}

ARK_UD::ARK_UD( )
{
	//vs.push_back( { 114166.f, 43.34448f, 76.87836f, "" } ); // УКВ Бурундай
	//vs.push_back( { 114333.f, 50.28548f, 57.15122f, "" } ); // УКВ Актобе тренажер
}

float ARK_UD::get_curs( double latitude, double longitude, double course )
{
	auto chanel = sw_rkud_chanel( );
	freq = ukv_fixed_freqs[chanel];

	// ищем подходящий маяк с приемлемой дальностью TODO добавить дальность до маяка
	auto it = std::find_if( vs.begin( ), vs.end( ), [=]( auto& station_ ) { return freq == station_.freq; } );

	if ( it == vs.end( ) )
	{
		return 0;
	}
	// определяем напра
	double lon = ( it->lon - longitude ) * cos( latitude * GtoR );
	double lat = ( it->lat - latitude );
	double ARK = atan2( lon, lat ) * Ra;
	ARK -= course; //был минус
	ARK = norm360( ARK * ( - 1 ));
	return ARK;
}

quint8 ARK_UD::sw_rkud_mode( )
{
	if ( t_rkud_off ) { return 0; }
	if ( t_rkud_up ) { return 1; }
	if ( t_rkud_shp ) { return 2; }
	if ( t_rkud_i ) { return 3; }
	if ( t_rkud_rpk ) { return 4; }
	return 0;
}

quint8 ARK_UD::sw_rkud_chanel( )
{
	if ( t_rkud_1 ) { return 1; }
	if ( t_rkud_2 ) { return 2; }
	if ( t_rkud_3 ) { return 3; }
	if ( t_rkud_4 ) { return 4; }
	if ( t_rkud_5 ) { return 5; }
	if ( t_rkud_6 ) { return 6; }
	return 1;
}

bool BoardSystem::StateChanged()
{
	static int startLeftEngine = false;
	static int startRightEngine = false;
	static bool signalShot = false;
	static VSU::State signalAPU = VSU::State::OFF;
	static bool rightBoardTermalHook = false;
	static bool leftBoardTermalHook = false;

	auto ret = false;

	if (startLeftEngine != _Engine_1->mode_eng()) ret = true;
	if (startRightEngine != _Engine_2->mode_eng()) ret = true;
	if (signalShot != b_fire) ret = true;
	if (signalAPU != _VSU->get_state()) ret = true;
	if (rightBoardTermalHook != _Engine_2->is_working()) ret = true;
	if (leftBoardTermalHook != _Engine_1->is_working()) ret = true;

	startLeftEngine = _Engine_1->mode_eng();
	startRightEngine = _Engine_2->mode_eng();
	signalShot = b_fire;
	signalAPU = _VSU->get_state();
	rightBoardTermalHook = _Engine_2->is_working();
	leftBoardTermalHook = _Engine_1->is_working();
	return ret;
}

void BoardSystem::StartAll( )
{
	Helicopter->ep_elp.t_akkum_1 = true;
	Helicopter->ep_elp.t_akkum_2 = true;
	Helicopter->ep_elp.t_gen_1 = true;
	Helicopter->ep_elp.t_gen_2 = true;
	Helicopter->ep_elp.t_ACDC_2 = true;
	Helicopter->ep_elp.t_ACDC_1 = true;
	Helicopter->ep_elp.t_aero_pit_dc  = true;
	Helicopter->lsh_elp.t_ag_left  = true;
	Helicopter->lsh_elp.t_ag_res   = true;
	Helicopter->rsh_elp.t_ag_right = true;
	Helicopter->rsh_elp.t_diss     = true;
	Helicopter->PC.t_NV_brake_on    = false;
	Helicopter->PC.t_NV_brake_off  = !false;
	Helicopter->PC.t_eng_left_brake = false;
	Helicopter->PC.t_eng_rght_brake = false;
}

QString BoardSystem::json()
{
	std::stringstream ss;
	ss << std::fixed << std::boolalpha << /*std::setw(11) <<*/ std::setprecision(1) << std::setfill('0') <<
		"{" <<
		"\"startLeftEngine\":" << _Engine_1->mode_eng() << ","
		"\"startRightEngine\":" << _Engine_2->mode_eng() << ","
		"\"restart\":" << false << ","
		"\"pause\":" << false << ","
		"\"crash\":" << false << ","
		"\"signalShot1\":" << b_fire << ","
		"\"signalShot2\":" << b_fire << ","
		"\"signalShot3\":" << b_fire << ","
		"\"signalShot4\":" << b_fire << ","
		"\"signalNormalStartAPU\":" << (_VSU->get_state() == VSU::State::STARTING) << ","
		"\"signalFalseStartAPU\":" << (_VSU->get_state() == VSU::State::START_FALSE) << ","
		"\"signalColdStartAPU\":" << (_VSU->get_state() == VSU::State::SCROLL) << ","
		"\"startLeftFuelPump\":" << false << ","
		"\"startRightFuelPump\":" << false << ","
		"\"startDrainFuelPump\":" << false << ","
		"\"startPowerConverter\":" << false << ","
		"\"switchLampANO\":" << (ANO && (GeoHeight < 5)) << ","
		"\"switchTopBlinker\":" << true << ","
		"\"switchBottomBlinker\":" << true << ","
		"\"switchCombatantLight\":" << true << ","
		"\"switchContourLight\":" << true << ","
		"\"switchSteeringLight\":" << true << ","
		"\"switchLeftFPP\":" << true << ","
		"\"switchRightFPP\":" << true << ","
		"\"switchSX16\":" << true << ","
		"\"rightBoardTermalHook\":" << _Engine_2->is_working() << ","
		"\"leftBoardTermalHook\":" << _Engine_1->is_working() << ","
		"\"signalRocket11\":" << false << ","
		"\"signalRocket12\":" << false << ","
		"\"signalRocket13\":" << false << ","
		"\"signalRocket14\":" << false << ","
		"\"signalRocket21\":" << false << ","
		"\"signalRocket22\":" << false << ","
		"\"signalRocket23\":" << false << ","
		"\"signalRocket24\":" << false <<
		"}";
	return QString::fromStdString(ss.str());
};

quint8 AllWeaponPult::sw_weapon_type( )
{
	if ( t_MV ) { sw_weapon_type_ = 17; }
	if ( t_PKT ) { sw_weapon_type_ = 2; }
	if ( t_FAB ) { sw_weapon_type_ = 18; }
	if ( t_NAR ) { sw_weapon_type_ = 31; }
	if ( t_UPK23 ) { sw_weapon_type_ = 27; }
	if ( t_AGS17 ) { sw_weapon_type_ = 21; }
	return sw_weapon_type_;
}

quint8 AllWeaponPult::get_sw_weapon_type( )
{
	{
		switch ( sw_weapon_type_ )
		{
		case (quint8)GunTypes::FAB:
			return 0;
			break;
		case (quint8)GunTypes::UB32:
		case (quint8)GunTypes::UB20:
		case (quint8)GunTypes::UB16:
		case (quint8)GunTypes::UB9:
			return 1;
			break;
		case (quint8)GunTypes::UPK23:
			return 2;
			break;
		case (quint8)GunTypes::PKT:
			return 3;
			break;
		default:
			return 1;
		}
	}
}

quint8 AllWeaponPult::sw_burst( )
{
	if ( t_fire_short ) { return 1; }
	//if ( t_fire_midle ) { return 2; } // позиции мидл нет в СОД
	if ( t_fire_long ) { return 3; }
	t_fire_midle = true;
	return 2;
}

KI_204::KI_204( )
{
	vs.push_back( { 112, 112.3f, 43.37481f,  77.08523f } );	  // VOR-DME Almaty 23L ALM (.- .-.. --)
	vs.push_back( { 116, 116.4f, 43.37650f,  77.08409f } );   // VOR-DME Almaty 23R ATA (.- - .-)
	vs.push_back( { 113, 113.4f, { 50.263416f, 57.181888f } } ); // VOR-DME Aqtobe
	vs.push_back( { 111, 111.7f, { 50.257210f, 57.188342f } } ); // ILS Aqtobe заход с курсом 124
	vs.push_back( { 110, 110.5f, { 50.235223f, 57.222449f } } ); // ILS Aqtobe заход с курсом 304
	vs.push_back( { 1, 0.f, {0.f, 0.f} } ); // VOR-DME Test
}

// кроме поиска по частоте не плохо бы добавть и растояние до маяка, чтоб через всю страну не добивал
auto KI_204::find( )
{
	return std::find_if( vs.begin( ), vs.end( ), [this]( auto& vor ) { return std::abs( freq_ - vor.freq ) < 0.05; } );
}

// задание радиала вращением энкодера который передает -1 0 +1
void KI_204::set_radial( int encoder )
{
	course_ += encoder;
	course_ = norm360( course_ );
}

void KI_204::set_radial( float rad )
{
	course_ = norm360( 360.0 - rad - Magnetic::magneticDeclination );
}

// расчет отклонения планки DIR и счетчика DME
float KI_204::distance( Point pos )
{
	//double alpha_rad = ( 90.0 - course_ ) * GtoR;

	double dx = pos.longitude - OBS_geo.longitude;
	double dy = pos.latitude - OBS_geo.latitude;

	float diff = sin( ( course_ - radial_ ) * GtoR ) * 8;
	dir = std::clamp( diff, -1.f, 1.f );

	double d_lat = dy * GtoR;
	double d_lon = dx * GtoR;
	double a = pow( sin( d_lat / 2 ), 2 ) + cos( ( OBS_geo.latitude ) * GtoR ) * cos( ( pos.latitude ) * GtoR ) * pow( sin( d_lon / 2 ), 2 );
	double c = 2 * atan2( sqrt( a ), sqrt( 1 - a ) );
	DME_ = kEarthRadiusKM * c;
	return  DME_;
}

// расчет значения флагов прибора на основании направления радиала и OBS
void KI_204::fromto( )
{
	// если угол OBS course совпадает по направлению с radial +-90градусов то FROM иначе TO
	// переводим углы в положительную область для сравнения
	auto a = cos( ( radial_ - course_ ) * GtoR );

	i_fr = i_to = false;
	if ( a > 0 )
		i_fr = true;
	else
		i_to = true;
	i_from_to = i_fr;	// это для стрелочного прибора, возможно надо на to заменить или инверсию в конфиге
}

// задание частоты VOR при минимальных изменения частоты сразу выходит, если частота поменялсась, то ищет подходящий по частоте VOR в своей базе
float KI_204::freq( float freq )
{
	// если частота значимо не изменилась, то выходим
	if ( std::fabs( freq_ - freq ) < 0.001 ) return freq_;
	freq_ = freq;
	auto it = find( );

	if ( it != vs.end( ) )
	{
		//OBS = { convertToCartesian( it->pos ), course_ * GtoR };
		OBS_geo = it->pos;
	}

	return freq_;
}

void KI_204::Calc( float latitude, float longitude )
{
	auto it = find( );

	if ( it == vs.end( ) )
	{
		dir = 0.f;
		//radial_ = 0.f;
		i_fr = i_to = false;
		i_nav = true;
		return;
	}
	i_nav = false;
	// определяем радиал на ЛА
	double lat = ( it->pos.latitude - latitude );
	double lon = ( it->pos.longitude - longitude ) * cos( latitude * GtoR );
	radial_ = atan2( lon, lat ) * Ra + 180;
	radial_ = norm360( radial_ );

	fromto( );  // i_fr i_to
	distance( { latitude, longitude } );  // dir DME

	return;
}

auto KI_204::VOR( ) -> Point
{
	if ( auto it = find( ); it != vs.end( ) )
	{
		return it->pos;
	}
	return Point( );
}

std::ostream& operator<<( std::ostream& out, const KI_204& obj )
{
	// TODO: вставьте здесь оператор return
	out << obj.radial_ << "; " << obj.dir << "; " << obj.DME_ << "; " << obj.i_fr << "; " << obj.i_to;
	return out;
}

void PNP_72::set_D1( int encoder )
{
	d1_ += encoder;
	if ( d1_ > 360 ) d1_ -= 360;
	if ( d1_ < 0 ) d1_ += 360;
}

void UGR_4UK::set_D1( int encoder )
{
	d1_ += encoder;
	if ( d1_ > 360 ) d1_ -= 360;
	if ( d1_ < 0 ) d1_ += 360;
}

void UGR_4UK::set_ark( float ark )
{
	ark_ = norm360( ark );
}

void RghtPanelAZS::StartAll( )
{
	//i_peash = true;         // Индикатор ПЭАШ
	//i_pevu = true;          // Индикатор ПЭВУ
	//i_vis_otstr = true;     // Индикатор ВИЗУАЛИЗАЦИЯ ОТСТРЕЛА
	//i_ash1 = true;          // Индикатор АШ-1
	//i_ash2 = true;          // Индикатор АШ-2
	//i_shakk_vsu = true;     // Индикатор ШАКК ВСУ
	//i_shvu_vsu = true;      // Индикатор ШВУ ВСУ
	//i_shvu_1k = true;       // Индикатор ШВУ 1К
	//i_shvu_2k = true;       // Индикатор ШВУ 2К

	//t_ta_start = true;      // Тумблер ТУРБОАГРЕГАТ ЗАПУСК
	//t_ta_ign = true;        // Тумблер ТУРБОАГРЕГАТ ЗАЖИГ
	//t_eng_start = true;    // Тумблер ДВИГАТЕЛИ ЗАПУСК
	//t_eng_ign = true;       // Тумблер ДВИГАТЕЛИ ЗАЖИГ
	//t_fuel_gauge = true;    // Тумблер ТОПЛИВОМЕР
	//t_pump_consum = true;   // Тумблер НАСОСЫ ТОПЛИВН БАКОВ РАСХОД
	//t_pump_left = true;     // Тумблер НАСОСЫ ТОПЛИВН БАКОВ  ЛЕВОГО
	//t_pump_right = true;    // Тумблер НАСОСЫ ТОПЛИВН БАКОВ  ПРАВОГО
	//t_sas = true;           // Тумблер САС
	//t_auto_main = true;     // Тумблер АВТОПИЛОТ ОСНОВН
	//t_auto_fric = true;     // Тумблер АВТОПИЛОТ ФРИКЦ
	//t_auto_emufta = true;   // Тумблер АВТОПИЛОТ ЭЛЕКТРОМУФТА
	//t_hidro_main = true;    // Тумблер ГИДРОСИСТ ОСНОВН
	//t_hidro_sec = true;     // Тумблер ГИДРОСИСТ ДУБЛИР
	//t_apparel = true;       // Тумблер УПРАВЛ АППАРЕЛЬ
	//t_turn_signal = true;   // Тумблер УКАЗАТ ПОВОРОТ
	//t_spuu_52 = true;       // Тумблер СПУУ-52
	//t_rentgen = true;       // Тумблер РЕНТГЕНОМЕТР
	//t_ctrl_start = true;    // Тумблер УПРАВЛ ЗАПУСК
	//t_close_left = true;    // Тумблер ПЕРЕКРЫВ КРАНЫ ЛЕВЫЙ
	//t_close_rght = true;    // Тумблер ПЕРЕКРЫВ КРАНЫ ПРАВЫЙ
	//t_close_bypass = true;  // Тумблер КРАН ПЕРЕПУС
	//t_ppo_signal = true;    // Тумблер ППО СИГНАЛИЗАЦ
	//t_ppo_1_auto = true;    // Тумблер I ОЧЕРЕДЬ АВТОМ
	//t_ppo_1_hand = true;    // Тумблер I ОЧЕРЕДЬ РУЧН
	//t_ppo_2_auto = true;    // Тумблер 2 ОЧЕРЕДЬ АВТОМ
	//t_ppo_2_hand = true;    // Тумблер 2 ОЧЕРЕДЬ РУЧНАЯ
	//t_r_spu = true;         // Тумблер СПУ
	//t_r_altimetr = true;    // Тумблер ВЫСОТОМЕР
	//t_r_comand_rs = true;   // Тумблер КОМАНД РС
	//t_r_ils = true;         // Тумблер HAB ILS
	//t_r_empty = true;       // Тумблер РАДИО - ПУСТО
	//t_r_compas_sv = true;   // Тумблер КОМПАС СВ
	//t_r_compas_ukv = true;  // Тумблер КОМПАС УКВ
	//t_r_diss = true;        // Тумблер ДИСС
	//t_r_rls = true;         // Тумблер РЛС
	//t_far_left_ctrl = true; // Тумблер ЛЕВ ФАРА УПРАВЛ
	//t_far_left_flash = true;// Тумблер ЛЕВ ФАРА СВЕТ
	//t_far_right_ctrl = true;// Тумблер ПРАВ ФАРА УПРАВЛ
	//t_far_right_flash = true;// Тумблер ПРАВ ФАРА СВЕТ
	//t_far_empty = true;     // Тумблер ПУСТО
	//t_strov_light = true;   // Тумблер СТРОЕВ ОГНИ
	//t_ano = true;           // Тумблер АНО
	//t_cut_main_lock = true; // Тумблер ОБРУБ ТРОС СПГ УПРАВЛ ЗАМКА ОСНОВН
	//t_cut_sec_lock = true;  // Тумблер ОБРУБ ТРОС СПГ УПРАВЛ ЗАМКА ДУБЛИР
	//t_ais_ctrl = true;      // Тумблер ПРОТИВООБЛЕД СИСТЕМА УПРАВЛЕНИЕ
	//t_ais_pzu_left = true;  // Тумблер ПРОТИВООБЛЕД СИСТЕМА ПЗУ ЛЕВ ДВИГАТ
	//t_ais_pzu_rght = true;  // Тумблер ПРОТИВООБЛЕД СИСТЕМА ПЗУ ПРАВ ДВИГАТ
	//t_ais_co_121 = true;    // Тумблер ПРОТИВООБЛЕД СИСТЕМА СО-121
	//t_ais_glass = true;     // Тумблер ПРОТИВООБЛЕД СИСТЕМА СТЕКОЛ
	//t_ais_wipe_left = true; // Тумблер СТЕКЛООЧИСТ ЛЕВЫЙ
	//t_ais_wipe_rght = true; // Тумблер СТЕКЛООЧИСТ ПРАВЫЙ
	//t_l166_81a = true;      // Тумблер Л166-81A
	//t_ko_50 = true;         // Тумблер КО-50  
}

float CentralControlPanel::f_get_roll_grad( )
{
	return f_get_roll * 45;
}

[[nodiscard]] float Magnetic::norm360( float angle )
{
	angle = std::fmod( angle + 360.0, 360.0 );
	return angle;
}

[[nodiscard]] float Magnetic::norm180( float angle )
{
	angle = norm360( angle );
	if ( angle > 180.0 )
		angle -= 360.0;
	return angle;
}
