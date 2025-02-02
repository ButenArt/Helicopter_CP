#include "Helicopter_onboard.h"
#include "Helicopter_Model.h"
#include "autopilot.h"
#include "Helicopter_instructor.h"
#include "Helicopter_imager_tcp_client.h"
#include "Helicopter_imager_tcp_client_sfera.h"
#include "Helicopter_instructor_tcp_client.h"

Helicopter_Onboard::Helicopter_Onboard(Helicopter_core* Helicopter)
{
	Helicopter = Helicopter;
	Flash = false;

	FlashTimer = new QTimer(this);
	FlashTimer->setInterval(500);
	
	connect( FlashTimer, &QTimer::timeout, [=]{
		Flash = !Flash;
		} );
	
	FlashTimer->start();

	ImitatioColdTimer = new QTimer(this);
	connect(ImitatioColdTimer, &QTimer::timeout, this, &Helicopter_Onboard::checkCondition);

	UV26Timer = new QTimer(this);
	connect(UV26Timer, &QTimer::timeout, this, &Helicopter_Onboard::PuskUV26Slot);
	connect(this, &Helicopter_Onboard::PuskUV26Signal, this, &Helicopter_Onboard::PuskUV26);


	pr = new Helicopter_Prima(Helicopter);
	connect(this, &Helicopter_Onboard::prima_signal, pr, &Helicopter_Prima::Algorithm);
	// последовательность информации о выстреле меняется
	// событие SendFire() вызывает отправку пакета в Imager о факте стрельбы
	// Imager присылает пакет об с новым остатоком боеприпасов
	// Instructor отпарвляет на инструктора измененное количество боеприпасов
	connect(this, SIGNAL(SendFire()), Helicopter->tcpclient_imager_.data(), SLOT(SendFireDataPacket()), Qt::QueuedConnection);
	connect(this, SIGNAL(SendFire()), Helicopter->tcpclient_imager_1.data(), SLOT(SendFireDataPacket()), Qt::QueuedConnection);
	connect(this, SIGNAL(SendFire()), Helicopter->tcpclient_imager_2.data(), SLOT(SendFireDataPacket()), Qt::QueuedConnection);
	connect(this, SIGNAL(SendFire()), Helicopter->tcpclient_imager_3.data(), SLOT(SendFireDataPacket()), Qt::QueuedConnection);
	connect(this, SIGNAL(SendFire()), Helicopter->tcpclient_imager_4.data(), SLOT(SendFireDataPacket()), Qt::QueuedConnection);
	connect(this, SIGNAL(SendFire()), Helicopter->tcpclient_imager_5.data(), SLOT(SendFireDataPacket()), Qt::QueuedConnection);
	connect(this, SIGNAL(SendFire()), Helicopter->tcpclient_imager_6.data(), SLOT(SendFireDataPacket()), Qt::QueuedConnection);
	connect(this, SIGNAL(SendFire()), Helicopter->tcpclient_imager_stend.data(), SLOT(SendFireDataPacket()), Qt::QueuedConnection);

	connect(this, SIGNAL(SendPKTFire( bool )), Helicopter->tcpclient_imager_.data(), SLOT(SendPKTFireDataPacket( bool )), Qt::QueuedConnection);
	connect(this, SIGNAL(SendPKTFire(bool)), Helicopter->tcpclient_imager_1.data(), SLOT(SendPKTFireDataPacket(bool)), Qt::QueuedConnection);
	connect(this, SIGNAL(SendPKTFire(bool)), Helicopter->tcpclient_imager_2.data(), SLOT(SendPKTFireDataPacket(bool)), Qt::QueuedConnection);
	connect(this, SIGNAL(SendPKTFire(bool)), Helicopter->tcpclient_imager_3.data(), SLOT(SendPKTFireDataPacket(bool)), Qt::QueuedConnection);
	connect(this, SIGNAL(SendPKTFire(bool)), Helicopter->tcpclient_imager_4.data(), SLOT(SendPKTFireDataPacket(bool)), Qt::QueuedConnection);
	connect(this, SIGNAL(SendPKTFire(bool)), Helicopter->tcpclient_imager_5.data(), SLOT(SendPKTFireDataPacket(bool)), Qt::QueuedConnection);
	connect(this, SIGNAL(SendPKTFire(bool)), Helicopter->tcpclient_imager_6.data(), SLOT(SendPKTFireDataPacket(bool)), Qt::QueuedConnection);
	connect(this, SIGNAL(SendPKTFire(bool)), Helicopter->tcpclient_imager_stend.data(), SLOT(SendPKTFireDataPacket(bool)), Qt::QueuedConnection);

	connect(this, SIGNAL(SendFire()), Helicopter->Model_.data(), SLOT( sendfire()), Qt::QueuedConnection);

	connect(this, SIGNAL(CargoDrop()), Helicopter->tcpclient_imager_.data(), SLOT(SendCargoControlDataPacket()), Qt::QueuedConnection);
	connect(this, SIGNAL(CargoDrop()), Helicopter->tcpclient_imager_1.data(), SLOT(SendCargoControlDataPacket()), Qt::QueuedConnection);
	connect(this, SIGNAL(CargoDrop()), Helicopter->tcpclient_imager_2.data(), SLOT(SendCargoControlDataPacket()), Qt::QueuedConnection);
	connect(this, SIGNAL(CargoDrop()), Helicopter->tcpclient_imager_3.data(), SLOT(SendCargoControlDataPacket()), Qt::QueuedConnection);
	connect(this, SIGNAL(CargoDrop()), Helicopter->tcpclient_imager_4.data(), SLOT(SendCargoControlDataPacket()), Qt::QueuedConnection);
	connect(this, SIGNAL(CargoDrop()), Helicopter->tcpclient_imager_5.data(), SLOT(SendCargoControlDataPacket()), Qt::QueuedConnection);
	connect(this, SIGNAL(CargoDrop()), Helicopter->tcpclient_imager_6.data(), SLOT(SendCargoControlDataPacket()), Qt::QueuedConnection);
	connect(this, SIGNAL(CargoDrop()), Helicopter->tcpclient_imager_stend.data(), SLOT(SendCargoControlDataPacket()), Qt::QueuedConnection);

	connect(Helicopter->tcpclient_imager_.data(), SIGNAL(CargoHook( quint32 )), this, SLOT(on_CargoHooked( quint32 )), Qt::QueuedConnection);
	connect(Helicopter->tcpclient_imager_1.data(), SIGNAL(CargoHook(quint32)), this, SLOT(on_CargoHooked(quint32)), Qt::QueuedConnection);
	connect(Helicopter->tcpclient_imager_2.data(), SIGNAL(CargoHook(quint32)), this, SLOT(on_CargoHooked(quint32)), Qt::QueuedConnection);
	connect(Helicopter->tcpclient_imager_3.data(), SIGNAL(CargoHook(quint32)), this, SLOT(on_CargoHooked(quint32)), Qt::QueuedConnection);
	connect(Helicopter->tcpclient_imager_4.data(), SIGNAL(CargoHook(quint32)), this, SLOT(on_CargoHooked(quint32)), Qt::QueuedConnection);
	connect(Helicopter->tcpclient_imager_5.data(), SIGNAL(CargoHook(quint32)), this, SLOT(on_CargoHooked(quint32)), Qt::QueuedConnection);
	connect(Helicopter->tcpclient_imager_6.data(), SIGNAL(CargoHook(quint32)), this, SLOT(on_CargoHooked(quint32)), Qt::QueuedConnection);
	connect(Helicopter->tcpclient_imager_stend.data(), SIGNAL(CargoHook(quint32)), this, SLOT(on_CargoHooked(quint32)), Qt::QueuedConnection);

	_audio = Helicopter->audio_;
}

Helicopter_Onboard::~Helicopter_Onboard()
{
	FlashTimer->stop( );
	if ( FlashTimer )
		delete FlashTimer;
	delete ImitatioColdTimer;
}

// проверка всех необходимых условий для запуска двигателя, прокрутка и ложный запуск двигателя
void Helicopter_Onboard::StartEngineProc()
{
	auto starting = false;
	if (Helicopter->PC.t_NV_brake_off)   // проверяем тормоз вента отключен?
	{
		if (Helicopter->rp_azs.t_eng_start
			&& Helicopter->rp_azs.t_eng_ign
			//			&& Helicopter->rp_azs.t_pump_consum
			//			&& (Helicopter->rp_azs.t_pump_left || Helicopter->rp_azs.t_pump_right)  // один или оба? или на каждом двигателе свой?
			//			&& Helicopter->rp_azs.t_ctrl_start
			&& Helicopter->rp_azs.t_close_left
			&& Helicopter->rp_azs.t_close_rght
			&& Helicopter->rp_azs.t_close_bypass
			) // включены все необходимые АЗС
		{
			// тут не совсем запуск, а скорее ложный запуск в положении запуск двигателя. 
			// Запуском он станет при подаче топлива.
			// для этого надо отпустить тормоз дигателя, и что бы был открыт пожарный кран
			// это делается в другом месте
			if (Helicopter->_BoardSystem->_VSU->is_working())
			{
				if (Helicopter->cep.t_eng_left) // левый двигатель
				{
					if (Helicopter->cep.t_eng_sel_start) {

						Helicopter->_BoardSystem->_Engine_1->start_eng();
						starting = true;
					}
					else if (Helicopter->cep.t_eng_sel_scroll) {
						Helicopter->_BoardSystem->_Engine_1->scroll_eng();
						starting = true;
					}
					else {
						qDebug() << "Не выбран режим запуска двигателя";
					}
				}
				else if (Helicopter->cep.t_eng_rght) // правый двигатель
				{
					if (Helicopter->cep.t_eng_sel_start) {
						Helicopter->_BoardSystem->_Engine_2->start_eng();
						starting = true;
					}
					else if (Helicopter->cep.t_eng_sel_scroll) {
						Helicopter->_BoardSystem->_Engine_2->scroll_eng();
						starting = true;
					}
					else {
						qDebug() << "Не выбран режим запуска двигателя";
					}
				}
				else {
					qDebug() << "Не выбран двигатель";
				}
			}
			else {
				qDebug() << "ВСУ не включен";
			}
		}
		else {
			// TODO сюда сделать вывод ошибок курсана инструктору
			qDebug() << "Не все необходимые АЗС для запуска двигателя включены";
		}
	}
	else {
		qDebug() << "Тормоз винта не снят";
	}
}

void Helicopter_Onboard::StartTAProc()
{
	qDebug( ) << __FUNCTION__;
	if (Helicopter->rp_azs.t_ta_start &&
		Helicopter->rp_azs.t_ta_ign &&
		Helicopter->rp_azs.t_pump_consum && 
		(Helicopter->Is27V() || Helicopter->Is115V())
		) // включены все необходимые АЗС
	{
		if (Helicopter->cep.t_ta_sel_start) {
			Helicopter->_BoardSystem->_VSU->start_TA();
		}
		else if (Helicopter->cep.t_ta_sel_false_start) { // ложный запуск
			Helicopter->_BoardSystem->_VSU->false_start_TA();
		}
		else if (Helicopter->cep.t_ta_sel_scroll) {
			Helicopter->_BoardSystem->_VSU->scroll_TA();
		}
	}
	else {
		// TODO сюда сделать вывод ошибок курсана инструктору
		qDebug() << "Не все необходимые АЗС для запуска ТА включены";
		return;
	}
}

void Helicopter_Onboard::on_CargoHooked( quint32 /*Load*/ )
{
	qDebug( ) << __FUNCTION__;
	if (Helicopter->imager->Cargo.Load)
	{ 
		auto IPar = Helicopter->Model_->GetIPar( );
		IPar->Par064 = 1;
		IPar->Par065 = 0;
		IPar->Par066 = 1;
		IPar->Par067 = Helicopter->imager->Cargo.Load;
		IPar->Par068 = 12;
		qDebug( ) << "Груз подцеплен, длинна троса 10м, вес: " << Helicopter->imager->Cargo.Load;
	}
}

void Helicopter_Onboard::Onboard_Compute(float /*dT*/)
{
	//	FParam* FPar = Helicopter->Model_->GetFPar();
		/****************************************************************************************************/
		// блок проигрывания звуков
	//auto engine = (Helicopter->_BoardSystem->_Engine_1->is_working() || Helicopter->_BoardSystem->_Engine_2->is_working());
	//auto NV = Helicopter->_BoardSystem->NV;
	//if (!engine)
	//{
	//	_audio->Play(0, (Helicopter->cep.t_fpump_consum && Helicopter->rp_azs.t_pump_consum) ||
	//		(Helicopter->cep.t_fpump_left && Helicopter->rp_azs.t_pump_left) ||
	//		(Helicopter->cep.t_fpump_right && Helicopter->rp_azs.t_pump_right));
	//}
	//else if (NV > 40.f) // если двигатель WORK и обороты НВ выше  40%
	//{
	//	_audio->Stop(Helicopter_audio::ENG_START);
	//}
	_audio->Play( 0, ( Helicopter->cep.t_fpump_consum && Helicopter->rp_azs.t_pump_consum ) ||
		( Helicopter->cep.t_fpump_left && Helicopter->rp_azs.t_pump_left ) ||
		( Helicopter->cep.t_fpump_right && Helicopter->rp_azs.t_pump_right ) );
	//_audio->Play(Helicopter_audio::ENG_TA, engine && (NV >= 40.0f && NV <= 62.0f));
	//_audio->Play(Helicopter_audio::ENG_75, engine && (NV >= 60.0f && NV <= 77.0f));
	//_audio->Play(Helicopter_audio::ENG_80, engine && (NV >= 75.0f && NV <= 90.20f));
	//_audio->Play(Helicopter_audio::ENG_90, engine && (NV >= 88.0f && NV <= 93.20f));
	//_audio->Play(Helicopter_audio::ENG_MAX, engine && (NV >= 91.0f));

	//****виброплатформа****************************************************************/
	CalcVibraion();
	/************РАСТЧЕТ МЕСТОПОЛОЖЕНИЯ*************************************************/
	CalcCoordinate();
	/************УПРАВЛЕНИЕ ФАРАМИ******************************************************/
	CalcLighting();
	/************ГИДРАВЛИКА*************************************************************/
	CalcHidravlic();
	/****электрика**********************************************************************/
	CalcElectric();
	/********СТРЕЛЬБА*******************************************************************/
	CalcFiring();
	/***пневматика тормоза**************************************************************/
	CalcPnevmo();
	/*****АВТОПИЛОТ*********************************************************************/
	CalcAutoPilot();
	/***управление ТА*******************************************************************/
	CalcVSU();
	/*******управление двигателем ******************************************************/
	CalcEngine();
	/************ПОС********************************************************************/
	CalcIcing();
	/**ППС противопожарная система******************************************************/
	CalcPPS();
	/**Пульт контроля ДИСС**************************************************************/
	CalcDISSContr( );
	/**Расчет показаний АРК*************************************************************/
	CalcARK();
	CalcARK_15();
	CalcGMK();
	CalcVOR( );
	/********Груз***********************************************************************/
	CalcCargo( );
	/********KO50***********************************************************************/
	CalcKO50( ); 
	CalcSPUU52( );
	CalcPZU( );
	/*******************управление и проверка БУР самописца*******************************/
	CalcBUR();
	/*************Управление и проверка приборов**************/
	CalcOrlan();
	CalcKT76C();
	CalcKN53();
	CalcKDI();
	CalcUV26();
	//CalcPrima();
	// Приборы
	Helicopter->lpb.i_ag_net_kontr = (!(Helicopter->lsh_elp.t_bkk_18) && Helicopter->Is27V()) || Helicopter->instructor->fault.BKK18;

	// индикаторы
	Helicopter->lsh_elp.i_pvd_heat_ok = Helicopter->lsh_elp.t_pvd_heat && Helicopter->Is27V() && !Helicopter->instructor->fault.pos_pvd; // нажатие кнопки - зажигает лампочку всегда
	Helicopter->rsh_elp.i_pvd_heat_ok = Helicopter->rsh_elp.t_pvd_heat_contr && Helicopter->Is27V() && !Helicopter->instructor->fault.pos_pvd;

	// Helicopter->lsh_elp.i_bkk_ok = Helicopter->lsh_elp.t_bkk_18 && Helicopter->Is27V(); // TODO если нужно включтьб по тумблеру БКК18
	Helicopter->lsh_elp.i_bkk_ok = (Helicopter->lsh_elp.t_bkk_contr_up || Helicopter->lsh_elp.t_bkk_contr_down) && Helicopter->Is27V();
	
	//Helicopter->rsp_ep.i_pzu_left_on = Helicopter->rsp_ep.t_pzu_left && Helicopter->Is27V();
	//Helicopter->rsp_ep.i_pzu_right_on = Helicopter->rsp_ep.t_pzu_right && Helicopter->Is27V();

	//РИ65
	Helicopter->lsh_elp.i_ri_65 = !Helicopter->lsh_elp.t_ri_65 && Helicopter->Is27V();

}

// 12		13			14		15			16		17			18		19			20
// 0,7125	0,771875	0,83125	0,890625	0,95	1,009375	1,06875	1,128125	1,1875
void Helicopter_Onboard::CalcVibraion( )
{
	Helicopter->PC.vibro = false;
	Helicopter->PC.f_vibro = Helicopter->_BoardSystem->NV;
}

void Helicopter_Onboard::CalcCoordinate()
{
	auto FPar = Helicopter->Model_->GetFPar();
	Helicopter->_BoardSystem->Latitude = Helicopter->_BoardSystem->CalcLat(FPar->Xg);
	Helicopter->_BoardSystem->Longitude = Helicopter->_BoardSystem->CalcLon(FPar->Zg);
}

void Helicopter_Onboard::CalcLighting()
{
	if (Helicopter->PC.t_fpp_l_up && (Helicopter->_BoardSystem->fpp_l_vert > FPP_DOWN))
	{
		Helicopter->_BoardSystem->fpp_l_vert -= 1;
	}
	if (Helicopter->PC.t_fpp_l_down && (Helicopter->_BoardSystem->fpp_l_vert < FPP_UP))
	{
		Helicopter->_BoardSystem->fpp_l_vert += 1;
	}
	if (Helicopter->PC.t_fpp_l_left && Helicopter->_BoardSystem->fpp_l_horiz > FPP_LEFT)
	{
		Helicopter->_BoardSystem->fpp_l_horiz -= 1;
	}
	if (Helicopter->PC.t_fpp_l_right && (Helicopter->_BoardSystem->fpp_l_horiz < FPP_RIGHT))
	{
		Helicopter->_BoardSystem->fpp_l_horiz += 1;
	}

	if (Helicopter->PC.t_fpp_r_up && (Helicopter->_BoardSystem->fpp_r_vert > FPP_DOWN))
	{
		Helicopter->_BoardSystem->fpp_r_vert -= 1;
	}
	if (Helicopter->PC.t_fpp_r_down && (Helicopter->_BoardSystem->fpp_r_vert < FPP_UP))
	{
		Helicopter->_BoardSystem->fpp_r_vert += 1;
	}
	if (Helicopter->PC.t_fpp_r_left && (Helicopter->_BoardSystem->fpp_r_horiz > FPP_LEFT))
	{
		Helicopter->_BoardSystem->fpp_r_horiz -= 1;
	}
	if (Helicopter->PC.t_fpp_r_right && (Helicopter->_BoardSystem->fpp_r_horiz < FPP_RIGHT))
	{
		Helicopter->_BoardSystem->fpp_r_horiz += 1;
	}
	Helicopter->_BoardSystem->ANO = Helicopter->rsp_ep.t_ano;
	Helicopter->_BoardSystem->beacon_up = Helicopter->rsp_ep.t_flashlight;
	Helicopter->_BoardSystem->beacon_down = Helicopter->rsp_ep.t_beacon_down;
	Helicopter->_BoardSystem->light_drill = Helicopter->rsp_ep.t_light_drill;
	Helicopter->_BoardSystem->light_contur = Helicopter->rsp_ep.t_light_contur;

}

void Helicopter_Onboard::CalcHidravlic()
{
	// индикация резевной гидросистемы
	if ( Helicopter->_BoardSystem->_Hydraulic->second > 35.f )
	{
		Helicopter->cep.i_hidro_second = true;
	}
	else
	{
		Helicopter->cep.i_hidro_second = false;
	}
	// индикация основной гидросистемы
	if ( ( Helicopter->_BoardSystem->_Hydraulic->main > 35.f ) && Helicopter->cep.t_hidro_main && !Helicopter->cep.i_hidro_second)
	{
		Helicopter->cep.i_hidro_main = true;
	}
	else
	{
		Helicopter->cep.i_hidro_main = false;
	}
	Helicopter->cep.f_p_hidro_main = Helicopter->_BoardSystem->_Hydraulic->main;
	Helicopter->cep.f_p_hidro_second = Helicopter->_BoardSystem->_Hydraulic->second;

	// переключения на основную систему по кнопке отключения резервной
	//if ( Helicopter->cep.t_hidro_main && Helicopter->_BoardSystem->_Hydraulic->main > 40.f && Helicopter->cep.t_off_second_hidro )
	//{
	//	Helicopter->cep.i_hidro_main = true;
	//	Helicopter->cep.i_hidro_second = false;
	//	Helicopter->_BoardSystem->_Hydraulic->second = 0;
	//}
}

void Helicopter_Onboard::CalcElectric()
{
	auto FPar = Helicopter->Model_->GetFPar();
	Helicopter->_BoardSystem->_Electrics->Hgeo = FPar->Hgeo;

	Helicopter->ep_elp.f_amper_1 = 0;
	Helicopter->ep_elp.f_amper_2 = 0;
	Helicopter->ep_elp.f_amper_3 = 0;
	if (Helicopter->ep_elp.t_akkum_1 || Helicopter->ep_elp.t_akkum_2)
	{
		Helicopter->_BoardSystem->b_light_cab = true;
	}
	else
	{
		Helicopter->_BoardSystem->b_light_cab = false;
	}
	// амперметры постоянного тока
	//Helicopter->ep_elp.f_amper_1 = Helicopter->_BoardSystem->_Electrics->Iakk1;
	//Helicopter->ep_elp.f_amper_2 = Helicopter->_BoardSystem->_Electrics->Iakk2;
	switch (Helicopter->ep_elp.sw_ctrl)
	{
	case 1:
		Helicopter->ep_elp.f_amper_1 = Helicopter->_BoardSystem->_Electrics->Iakk1;
		break;
	case 2:
		Helicopter->ep_elp.f_amper_2 = Helicopter->_BoardSystem->_Electrics->Iakk2;
		break;
	case 3:
		Helicopter->ep_elp.f_amper_3 = Helicopter->_BoardSystem->_Electrics->Isgu;
		break;
	}
	// вольтметр постоянного тока
	/*switch (Helicopter->ep_elp.get_dc_volt_mode())
	{
	case 0:
		Helicopter->ep_elp.f_volt = 0;
		break;
	case 1:
		Helicopter->ep_elp.f_volt = Helicopter->_BoardSystem->_Electrics->Uakk1;
		break;
	case 2:
		Helicopter->ep_elp.f_volt = Helicopter->_BoardSystem->_Electrics->Uakk2;
		break;
	case 3:
		Helicopter->ep_elp.f_volt = Helicopter->_BoardSystem->_Electrics->Uvu1;
		break;
	case 4:
		Helicopter->ep_elp.f_volt = Helicopter->_BoardSystem->_Electrics->Uvu2;
		break;
	case 5:
		Helicopter->ep_elp.f_volt = (Helicopter->ep_elp.t_akkum_1) ? Helicopter->_BoardSystem->_Electrics->Uakk1 : 0.f;
		break;
	case 6:
		Helicopter->ep_elp.f_volt = (Helicopter->ep_elp.t_akkum_2) ? Helicopter->_BoardSystem->_Electrics->Uakk2 : 0.f;
		break;
	case 7:
		Helicopter->ep_elp.f_volt = Helicopter->_BoardSystem->_Electrics->Uvu;
		break;
	case 8:
		Helicopter->ep_elp.f_volt = Helicopter->_BoardSystem->_Electrics->Usgu;
		break;
	case 9:
		Helicopter->ep_elp.f_volt = 0;
		break;
	}*/
	if (Helicopter->ep_elp.get_ac_volt_mode() <= 2)  // Ген 1 и ПТС
	{
		if (Helicopter->ep_elp.t_gen == true)
		{
			if (Helicopter->ep_elp.i_gen_1_off)
			{
				Helicopter->ep_elp.f_ac_volt = 0;
			}
			else
			{
				Helicopter->ep_elp.f_ac_volt = Helicopter->_BoardSystem->_Electrics->Ugen115;
			}
		}
		else // ПТС
		{
			if (Helicopter->ep_elp.i_ptc_on)
			{
				Helicopter->ep_elp.f_ac_volt = Helicopter->_BoardSystem->_Electrics->Ugen115r;
			}
			else
			{
				Helicopter->ep_elp.f_ac_volt = 0;
			}
		}
	}
	else // Ген 2 и Аэродром
	{
		if (Helicopter->ep_elp.t_gen == true)
		{
			if (Helicopter->ep_elp.i_gen_2_off)
			{
				Helicopter->ep_elp.f_ac_volt = 0;
			}
			else
			{
				Helicopter->ep_elp.f_ac_volt = Helicopter->_BoardSystem->_Electrics->Ugen115;
			}
		}
		else // АЭР
		{
			if (Helicopter->ep_elp.i_rap_on)
			{
				Helicopter->ep_elp.f_ac_volt = Helicopter->_BoardSystem->_Electrics->Ugen115r;
			}
			else
			{
				Helicopter->ep_elp.f_ac_volt = 0;
			}
		}
	}
	Helicopter->ep_elp.f_ac_amper_1 = Helicopter->_BoardSystem->_Electrics->Igen1;// / 10.f; // компенсация кривых приборов до 15А
	Helicopter->ep_elp.f_ac_amper_2 = Helicopter->_BoardSystem->_Electrics->Igen2;// / 10.f;
	// если аккумуляторы включенв, а тока нет, то включаем лампочки Ген. отключен
	Helicopter->ep_elp.i_gen_1_off = ((Helicopter->ep_elp.t_akkum_1 || Helicopter->ep_elp.t_akkum_2) && (Helicopter->_BoardSystem->_Electrics->Igen1 < 10)) ? true : false;
	Helicopter->ep_elp.i_reser36 = Helicopter->ep_elp.t_r36_man || (Helicopter->ep_elp.t_r36_auto && Helicopter->ep_elp.i_gen_1_off && Helicopter->ep_elp.i_gen_2_off && !Helicopter->ep_elp.i_rap_on);
	Helicopter->ep_elp.i_ptc_on = Helicopter->ep_elp.t_r36_man || (Helicopter->ep_elp.t_r36_auto && Helicopter->ep_elp.i_gen_1_off && Helicopter->ep_elp.i_gen_2_off && !Helicopter->ep_elp.i_rap_on);
	Helicopter->ep_elp.i_gen_2_off = ((Helicopter->ep_elp.t_akkum_1 || Helicopter->ep_elp.t_akkum_2) && (Helicopter->_BoardSystem->_Electrics->Igen2 < 10)) ? true : false;
	Helicopter->ep_elp.i_resev_on = ((Helicopter->ep_elp.t_akkum_1 || Helicopter->ep_elp.t_akkum_2) && (Helicopter->_BoardSystem->_Electrics->Igen2 < 10)) ? true : false;
	// если включатель ВУ включен, но напряжения на нем нет, то горит отказ
	Helicopter->ep_elp.i_vu1_brok = ((Helicopter->ep_elp.t_akkum_1 || Helicopter->ep_elp.t_akkum_2) && (Helicopter->_BoardSystem->_Electrics->Uvu1 < 10)) ? true : false;
	Helicopter->ep_elp.i_vu2_brok = ((Helicopter->ep_elp.t_akkum_1 || Helicopter->ep_elp.t_akkum_2) && (Helicopter->_BoardSystem->_Electrics->Uvu2 < 10)) ? true : false;
	//Helicopter->ep_elp.i_rap_on = (Helicopter->ep_elp.t_aero_pit_ac && Helicopter->ep_elp.i_aero_pit_dc && (Helicopter->ep_elp.t_akkum_1 /*||*/ && Helicopter->ep_elp.t_akkum_2)) ? true : false;
	
	// Переменный ток
	if (Helicopter->instructor->fault.aero_pts)
		Helicopter->ep_elp.i_rap_on = true;
	else
		Helicopter->ep_elp.i_rap_on = false;

	// Проверка состояния источников электропитания
	
		// Постоянного тока
		switch (Helicopter->ep_elp.get_dc_volt_mode())
		{
		case 0:
			Helicopter->ep_elp.f_volt = 0.f;
			break;
		case 1:
			Helicopter->ep_elp.f_volt = 0.f;
			// вольтметр постоянного тока
			if (Helicopter->ep_elp.t_akkum_1) {
				Helicopter->ep_elp.f_volt = 27.6f;
			}
			if (Helicopter->ep_elp.t_akkum_1 && !Helicopter->ep_elp.t_akkum_2) {
				float Volt = 30.f;
				float Amper = 0.f;
				if (Helicopter->rp_azs.t_pump_consum) {
					Volt -= 0.4f;
					Amper += 9.3f;
				}
				if (Helicopter->rp_azs.t_pump_left) {
					Volt -= 0.4f;
					Amper += 9.3f;
				}
				if (Helicopter->rp_azs.t_pump_right) {
					Volt -= 0.4f;
					Amper += 9.3f;
				}
				if (Helicopter->rp_azs.t_far_left_ctrl) {
					Volt -= 0.4f;
					Amper += 9.3f;
				}
				if (Helicopter->rp_azs.t_far_right_ctrl) {
					Volt -= 0.4f;
					Amper += 9.3f;
				}
				if (Helicopter->rp_azs.t_far_left_flash) {
					Volt -= 0.4f;
					Amper += 9.3f;
				}
				if (Helicopter->rp_azs.t_far_right_flash) {
					Volt -= 0.4f;
					Amper += 9.3f;
				}
				if (Helicopter->rsp_ep.t_pvd_heat_left) {
					Volt -= 0.4f;
					Amper += 9.3f;
				}
				if (Helicopter->rsp_ep.t_pvd_heat_right) {
					Volt -= 0.4f;
					Amper += 9.3f;
				}
				if (Helicopter->rp_azs.t_ano) {
					Volt -= 0.4f;
					Amper += 9.3f;
				}
				Helicopter->ep_elp.f_volt = Volt; 
				if (Helicopter->ep_elp.t_ctrl_vu1) {
					Helicopter->ep_elp.f_amper_1 = Amper;
					Helicopter->ep_elp.f_amper_2 = 0;
					Helicopter->ep_elp.f_amper_3 = 0;
				}
				else if (Helicopter->ep_elp.t_ctrl_vu2) {
					Helicopter->ep_elp.f_amper_1 = 0;
					Helicopter->ep_elp.f_amper_2 = 0;
					Helicopter->ep_elp.f_amper_3 = 0;
				}
				else if (Helicopter->ep_elp.get_ctrl() == 3) {
					Helicopter->ep_elp.f_amper_1 = 0;
					Helicopter->ep_elp.f_amper_2 = 0;
					Helicopter->ep_elp.f_amper_3 = Amper;
				}
			}
			else if (Helicopter->ep_elp.i_aero_pit_dc)
				Helicopter->ep_elp.f_volt = 29.5f;
			break;
		case 2:
			Helicopter->ep_elp.f_volt = 0.f;
			if (Helicopter->ep_elp.t_akkum_2) {
				Helicopter->ep_elp.f_volt = 28.0f;
			}
			if (!Helicopter->ep_elp.t_akkum_1 && Helicopter->ep_elp.t_akkum_2) {
				float Volt = 30.f;
				float Amper = 0.f;
				if (Helicopter->rp_azs.t_pump_consum) {
					Volt -= 0.4f;
					Amper += 9.3f;
				}
				if (Helicopter->rp_azs.t_pump_left) {
					Volt -= 0.4f;
					Amper += 9.3f;
				}
				if (Helicopter->rp_azs.t_pump_right) {
					Volt -= 0.4f;
					Amper += 9.3f;
				}
				if (Helicopter->rp_azs.t_far_left_ctrl) {
					Volt -= 0.4f;
					Amper += 9.3f;
				}
				if (Helicopter->rp_azs.t_far_right_ctrl) {
					Volt -= 0.4f;
					Amper += 9.3f;
				}
				if (Helicopter->rp_azs.t_far_left_flash) {
					Volt -= 0.4f;
					Amper += 9.3f;
				}
				if (Helicopter->rp_azs.t_far_right_flash) {
					Volt -= 0.4f;
					Amper += 9.3f;
				}
				if (Helicopter->rsp_ep.t_pvd_heat_left) {
					Volt -= 0.4f;
					Amper += 9.3f;
				}
				if (Helicopter->rsp_ep.t_pvd_heat_right) {
					Volt -= 0.4f;
					Amper += 9.3f;
				}
				if (Helicopter->rp_azs.t_ano) {
					Volt -= 0.4f;
					Amper += 9.3f;
				}
				Helicopter->ep_elp.f_volt = Volt;
				if (Helicopter->ep_elp.t_ctrl_vu1) {
					Helicopter->ep_elp.f_amper_1 = 0;
					Helicopter->ep_elp.f_amper_2 = 0;
					Helicopter->ep_elp.f_amper_3 = 0;
				}
				else if (Helicopter->ep_elp.t_ctrl_vu2) {
					Helicopter->ep_elp.f_amper_1 = 0;
					Helicopter->ep_elp.f_amper_2 = Amper;
					Helicopter->ep_elp.f_amper_3 = 0;
				}
				else if (Helicopter->ep_elp.get_ctrl() == 3) {
					Helicopter->ep_elp.f_amper_1 = 0;
					Helicopter->ep_elp.f_amper_2 = 0;
					Helicopter->ep_elp.f_amper_3 = Amper;
				}
			}
			else if (Helicopter->ep_elp.i_aero_pit_dc)
				Helicopter->ep_elp.f_volt = 29.5f;
			break;
		case 3:
			Helicopter->ep_elp.f_volt = 0.f;
			if (Helicopter->ep_elp.i_aero_pit_dc)
				Helicopter->ep_elp.f_volt = 27.5f;
			break;
		case 4:
			Helicopter->ep_elp.f_volt = 0.f;
			if (Helicopter->ep_elp.i_aero_pit_dc)
				Helicopter->ep_elp.f_volt = 27.5f;
			break;
		case 5:
			Helicopter->ep_elp.f_volt = 0.f;
			if (Helicopter->ep_elp.i_aero_pit_dc && Helicopter->ep_elp.t_aero_pit_dc)
				Helicopter->ep_elp.f_volt = 28.5f;
			break;
		case 6:
			Helicopter->ep_elp.f_volt = 0.f;
			if (Helicopter->ep_elp.i_aero_pit_dc && Helicopter->ep_elp.t_aero_pit_dc)
				Helicopter->ep_elp.f_volt = 29.5f;
			break;
		case 7:
			Helicopter->ep_elp.f_volt = 0;// Helicopter->_BoardSystem->_Electrics->Uvu;
			if (Helicopter->ep_elp.t_res_gen && !Helicopter->_BoardSystem->_VSU->is_off())
				Helicopter->ep_elp.f_volt = Helicopter->_BoardSystem->_Electrics->Usgu;
			break;
		case 8:
			Helicopter->ep_elp.f_volt = 0.f;
			if (Helicopter->ep_elp.t_res_gen && !Helicopter->_BoardSystem->_VSU->is_off())
				Helicopter->ep_elp.f_volt = Helicopter->_BoardSystem->_Electrics->Usgu;
			break;
		case 9:
			Helicopter->ep_elp.f_volt = 0.f;
			break;
		}

		if (Helicopter->ep_elp.get_ac_volt_mode() > 2)  // АЭР ПИТ ПОДКЛ и ПТС
		{
			if (Helicopter->ep_elp.i_rap_on) {
				if (Helicopter->ep_elp.t_gen) {
					Helicopter->ep_elp.f_ac_volt = 117.0f;
					if (Helicopter->ep_elp.t_gen_1 && Helicopter->ep_elp.t_ACDC_1)
						Helicopter->ep_elp.f_ac_amper_1 = 95.f;
				}
				else if (Helicopter->ep_elp.t_gen_pts_contr) {
					Helicopter->ep_elp.f_ac_volt = 117.0f;
					if (Helicopter->ep_elp.t_gen_1 && Helicopter->ep_elp.t_ACDC_1)
						Helicopter->ep_elp.f_ac_amper_1 = 95.f;
				}
				else {
					Helicopter->ep_elp.f_ac_volt = 0.0f;
					Helicopter->ep_elp.f_ac_amper_1 = 0.f;
				}

				if (Helicopter->ep_elp.t_ctrl_vu1) {
					//Helicopter->ep_elp.f_volt = 26.0f;
					if (Helicopter->ep_elp.t_gen_1 && Helicopter->ep_elp.t_ACDC_1)
						Helicopter->ep_elp.f_ac_amper_1 = 95.5f;
					Helicopter->ep_elp.f_ac_amper_2 = 0.5f;
					Helicopter->ep_elp.f_ac_amper_3 = 0.5f;
				}
				if (Helicopter->ep_elp.t_ctrl_vu2) {
					//Helicopter->ep_elp.f_volt = 26.5f;
					Helicopter->ep_elp.f_ac_amper_1 = 0.5f;
					if (Helicopter->ep_elp.t_gen_2 && Helicopter->ep_elp.t_ACDC_2)
						Helicopter->ep_elp.f_ac_amper_2 = 95.5f;
					Helicopter->ep_elp.f_ac_amper_3 = 0.5f;
				}
			}
		}
		if (Helicopter->ep_elp.get_ac_volt_mode() <= 2)  // Ген 1 и ПТС
		{
			if (Helicopter->ep_elp.i_rap_on && Helicopter->ep_elp.t_aero_pit_ac) {
				if (Helicopter->ep_elp.t_gen_pts_contr || Helicopter->ep_elp.t_gen) {
					Helicopter->ep_elp.f_ac_volt = 117.0f;
					if (Helicopter->ep_elp.t_gen_1 && Helicopter->ep_elp.t_ACDC_1)
						Helicopter->ep_elp.f_ac_amper_1 = 95.f;
				}
				if (Helicopter->ep_elp.t_gen_pts_contr && (Helicopter->ep_elp.get_dc_volt_mode() == 3 || Helicopter->ep_elp.get_dc_volt_mode() == 4))
					Helicopter->ep_elp.f_volt = 27.5f;
				if (Helicopter->ep_elp.t_ctrl_vu1) {
					if (Helicopter->ep_elp.t_gen_1 && Helicopter->ep_elp.t_ACDC_1)
						Helicopter->ep_elp.f_ac_amper_1 = 95.5f;
					Helicopter->ep_elp.f_ac_amper_2 = 0.f;
					Helicopter->ep_elp.f_ac_amper_3 = 0.f;
				}
				if (Helicopter->ep_elp.t_ctrl_vu2) {
					Helicopter->ep_elp.f_ac_amper_1 = 0.f;
					if (Helicopter->ep_elp.t_gen_2 && Helicopter->ep_elp.t_ACDC_2)
						Helicopter->ep_elp.f_ac_amper_2 = 95.5f;
					Helicopter->ep_elp.f_ac_amper_3 = 0.f;
				}
			}
		}
	if (!Helicopter->_BoardSystem->_VSU->is_off()) {
		if (Helicopter->ep_elp.t_akkum_1 || Helicopter->ep_elp.t_akkum_2) { // Проверка генераторов ПТ
			if ((Helicopter->ep_elp.get_dc_volt_mode() == 8 || Helicopter->ep_elp.get_dc_volt_mode() == 7) && Helicopter->ep_elp.t_res_gen)
				Helicopter->ep_elp.f_volt = 28.5f;
			if (Helicopter->ep_elp.t_pts_auto || Helicopter->ep_elp.t_pts_man){
				Helicopter->ep_elp.i_ptc_on = true;
			}
			else {
				Helicopter->ep_elp.i_ptc_on = false;
			}
			if (Helicopter->ep_elp.t_r36_auto || Helicopter->ep_elp.t_r36_man) {
				Helicopter->ep_elp.i_reser36 = true;
				Helicopter->ep_elp.i_resev_on = true;
			}
			else {
				Helicopter->ep_elp.i_reser36 = false;
				Helicopter->ep_elp.i_resev_on = false;
			}
			if (Helicopter->ep_elp.t_r36_man && Helicopter->ep_elp.t_pts_man){
				Helicopter->ep_elp.i_resev_on = false;
			}
			Helicopter->ep_elp.f_ac_volt = 117.f;
		}
		if (Helicopter->lpb.ite_1t.nv > 88.) { // Проверка правой коррекции
			if (!Helicopter->ep_elp.t_gen) {
				Helicopter->ep_elp.f_ac_volt = 116.0;
			}
			if (Helicopter->ep_elp.t_pts_auto && Helicopter->ep_elp.t_r36_auto) {
				if (Helicopter->rsp_ep.t_main_reserv || Helicopter->rsp_ep.t_main_second) {
					if (Helicopter->ep_elp.t_ACDC_1)
						Helicopter->ep_elp.i_vu1_brok = false;
					if (Helicopter->ep_elp.t_ACDC_2)
						Helicopter->ep_elp.i_vu2_brok = false;
					if ((Helicopter->ep_elp.sw_dc_volt_mode == 5 || Helicopter->ep_elp.sw_dc_volt_mode == 6))
						Helicopter->ep_elp.f_volt = 28.5;
					Helicopter->ep_elp.i_reser36 = false;
					Helicopter->ep_elp.i_ptc_on = false;
					Helicopter->ep_elp.i_resev_on = false;
				}
			}
			if (Helicopter->ep_elp.t_gen_1_contr || Helicopter->ep_elp.t_gen_1) {
				Helicopter->ep_elp.i_gen_1_off = false;
				Helicopter->ep_elp.f_ac_volt = 115.0;
				Helicopter->ep_elp.f_ac_amper_1 = 95.f;
			}
			if (Helicopter->ep_elp.t_gen_2_contr || Helicopter->ep_elp.t_gen_2) {
				Helicopter->ep_elp.f_ac_volt = 117.0;
				Helicopter->ep_elp.f_ac_amper_2 = 95.f;
				Helicopter->ep_elp.i_gen_2_off = false;
			}
			if (!Helicopter->ep_elp.t_gen_1_contr && !Helicopter->ep_elp.t_gen_1) {
				Helicopter->ep_elp.f_ac_volt = 0.0;
				Helicopter->ep_elp.f_ac_amper_1 = 0.f;
				Helicopter->ep_elp.i_gen_1_off = true;
			}
			if (!Helicopter->ep_elp.t_gen_2_contr && !Helicopter->ep_elp.t_gen_2) {
				Helicopter->ep_elp.f_ac_volt = 0.0;
				Helicopter->ep_elp.f_ac_amper_2 = 0.f;
				Helicopter->ep_elp.i_gen_2_off = true;
			}
		}
		else {
			Helicopter->ep_elp.f_ac_volt = 0.0f;
			Helicopter->ep_elp.i_gen_1_off = true;
			Helicopter->ep_elp.i_gen_2_off = true;
			if (Helicopter->ep_elp.t_aero_pit_ac && Helicopter->ep_elp.i_rap_on)
				Helicopter->ep_elp.f_ac_volt = 118.0f;
			if (Helicopter->ep_elp.get_ac_volt_mode() <= 2 && (Helicopter->ep_elp.get_dc_volt_mode() == 8 || Helicopter->ep_elp.get_dc_volt_mode() == 7) && Helicopter->ep_elp.t_res_gen && Helicopter->ep_elp.t_gen)  // Ген 1 и ПТС
			{
				Helicopter->ep_elp.f_ac_volt = 117.0f;
				Helicopter->ep_elp.f_ac_amper_3 = 98.f;
			}
		}
		if (Helicopter->ep_elp.t_res_gen) {
			if (Helicopter->ep_elp.t_gen_pts_contr && Helicopter->ep_elp.get_dc_volt_mode() == 5 || Helicopter->ep_elp.get_dc_volt_mode() == 6)
				Helicopter->ep_elp.f_volt = 0.f;
		}
	}
	if (!Helicopter->ep_elp.t_gen_pts_contr && !Helicopter->ep_elp.t_gen)
		Helicopter->ep_elp.f_ac_volt = 0.0f;

	if (Helicopter->instructor->fault.Gen1 == true)
	{
		if (Helicopter->ep_elp.get_ac_volt_mode() <= 2)
			Helicopter->ep_elp.f_ac_volt = 0.0f;
		Helicopter->ep_elp.f_ac_amper_1 = 0.f;
		Helicopter->ep_elp.i_gen_1_off = true;
		Helicopter->ep_elp.i_vu1_brok = true;
	}
	if (Helicopter->instructor->fault.Gen2 == true)
	{
		if (Helicopter->ep_elp.get_ac_volt_mode() > 2)
			Helicopter->ep_elp.f_ac_volt = 0.0f;
		Helicopter->ep_elp.f_ac_amper_2 = 0.f;
		Helicopter->ep_elp.i_gen_2_off = true;
		Helicopter->ep_elp.i_vu2_brok = true;
	}
	if (Helicopter->instructor->fault.VU1 == true)
	{
		Helicopter->ep_elp.f_amper_1 = 0.f;
		Helicopter->ep_elp.i_vu1_brok = true;
	}
	if (Helicopter->instructor->fault.VU2 == true)
	{
		Helicopter->ep_elp.f_amper_2 = 0.f;
		Helicopter->ep_elp.i_vu2_brok = true;
	}
	if (Helicopter->instructor->fault.Gen2 == true)
	{
		Helicopter->ep_elp.f_amper_2 = 0.f;
		Helicopter->ep_elp.i_vu2_brok = true;
	}
	if (Helicopter->instructor->fault.AkbTempHight_1 == true)
	{
		Helicopter->rsp_elp.akb_hight_temp_1 = true;
		if (!Helicopter->ep_elp.t_akkum_1)
			Helicopter->rsp_elp.akb_hight_temp_1 = false;
	}
	else
		Helicopter->rsp_elp.akb_hight_temp_1 = false;
}

void Helicopter_Onboard::CalcFiring()
{
#ifndef _DEBUG
	//if (Helicopter->lp_azs.t_main_switch_on && Helicopter->PC.t_safety_lock )
#endif
	{
		auto WeaponType = Helicopter->WeaponPult.sw_weapon_type( );
		//auto WeaponMode = Helicopter->WeaponPult.sw_burst( );
		if ( WeaponType != 0 ) // тип оружия выбирается на пульте вооружения, не должен быть 0, но все же
		{
			if ( ( Helicopter->PC.t_left_fire == true ) && ( Helicopter->PC.t_left_fire_prev == false ) )
			{
				qDebug( ) << "Fire";
				Helicopter->_BoardSystem->b_fire = true;
				if (WeaponType == 27) {
					Helicopter->lp_azs.i_upk_right = true;
					//Helicopter->lp_azs.f_ammo_count_upk_right_1 = true;
					Helicopter->lp_azs.i_upk_left = true;
					//Helicopter->lp_azs.f_ammo_count_upk_left_1 = true;
				}
				emit SendFire( ); // факт переключения тригера стрельбы
			}
			if ( ( Helicopter->PC.t_left_fire == false ) && ( Helicopter->PC.t_left_fire_prev == true ) )
			{
				qDebug( ) << "Stop Fire";
				Helicopter->_BoardSystem->b_fire = false;
				if (WeaponType == 27) {
					Helicopter->lp_azs.i_upk_right = false;
					//Helicopter->lp_azs.f_ammo_count_upk_right_1 = false;
					Helicopter->lp_azs.i_upk_left = false;
					//Helicopter->lp_azs.f_ammo_count_upk_left_1 = false;
				}
				emit SendFire( ); // факт переключения тригера стрельбы
			}
		}
		else
		{
			qDebug( ) << "Оружие не выбрано";
		}
	}
	// блое стрельбы из ПКТ
	{
		if ( Helicopter->PC.pkt_load )
			Helicopter->PC.pkt_loaded = true; // TODO надо сделать загрузку ленты
		if( ( Helicopter->PC.pkt_loaded == true ) && (Helicopter->PC.pkt_fire == true ) && (Helicopter->PC.pkt_fire_prev == false) )
		{ 
			qDebug( ) << "PKT Fire";
			Helicopter->PC.pkt_fire_prev = true;
			emit SendPKTFire( true );
		}
		if( ( Helicopter->PC.pkt_fire == false ) && ( Helicopter->PC.pkt_fire_prev == true ) )
		{ 
			qDebug( ) << "PKT Stop Fire";
			Helicopter->PC.pkt_fire_prev = false;
			emit SendPKTFire( false );
		}
	}
}

void Helicopter_Onboard::CalcPnevmo()
{
	if (Helicopter->PC.f_break < 200.f && Helicopter->_BoardSystem->b_break_press == false)
	{
		Helicopter->audio_->PlayOnce(18);
		Helicopter->_BoardSystem->b_break_press = true;
		Helicopter->_BoardSystem->b_break_release = false;
	}
	if (Helicopter->PC.f_break > 230.f && Helicopter->_BoardSystem->b_break_release == false)
	{
		Helicopter->audio_->PlayOnce(19);
		Helicopter->_BoardSystem->b_break_press = false;
		Helicopter->_BoardSystem->b_break_release = true;
	}
	Helicopter->_BoardSystem->_Pneumatic->Push(Helicopter->PC.f_break);
}

void Helicopter_Onboard::CalcAutoPilot()
{
	// работа с автопилотом на центральной панели
	// выключение автопилота
	Helicopter->ccp.i_ap_alt_off = false;
	Helicopter->ccp.i_ap_dir_off = false;
	if (Helicopter->PC.t_left_ap_off || Helicopter->cep.i_hidro_second || !Helicopter->Is115V() || Helicopter->PC.t_right_ap_off)
	{
		Helicopter->_BoardSystem->b_autopilot = false;
		Helicopter->_BoardSystem->b_autopilot_dir = false;
		Helicopter->_BoardSystem->b_autopilot_alt = false;
		Helicopter->_BoardSystem->enc_course->reset();
		Helicopter->_BoardSystem->enc_roll->reset();
		Helicopter->_BoardSystem->enc_pitch->reset();
		Helicopter->AP_->turnOff_roll_pitch();
		Helicopter->ccp.t_ap_on = Helicopter->_BoardSystem->b_autopilot && Helicopter->_BoardSystem->Is27V();
		Helicopter->ccp.i_ap_dir_on = false;
		//Helicopter->AP_->turnOff_yaw( );
		//Helicopter->AP_->turnOff_height( );
	}
	// включение автопилота по крену и тангажу
	//Helicopter->ccp.t_ap_on = true;   // TODO автопилот включен ВСЕГДА, убрать после возвращения пульта
	if (Helicopter->ccp.t_ap_on && (std::abs(std::abs(Helicopter->PC.f_roll) - std::abs(Helicopter->PC.f_ap_roll)) < Helicopter->AP_->getRangeRoll()) && (std::abs(std::abs(Helicopter->PC.f_pitch) - std::abs(Helicopter->PC.f_ap_pitch)) < Helicopter->AP_->getRangePitch()))
	{
		Helicopter->_BoardSystem->b_autopilot = true;
		Helicopter->_BoardSystem->enc_roll->reset();
		Helicopter->_BoardSystem->enc_pitch->reset();
		Helicopter->AP_->turnOn_roll_pitch();
		Helicopter->ccp.t_ap_on = Helicopter->_BoardSystem->b_autopilot && Helicopter->_BoardSystem->Is27V();
	}
	else
		Helicopter->ccp.t_ap_on = false;

	if (Helicopter->ccp.t_ap_dir_on)
	{
		Helicopter->_BoardSystem->b_autopilot_dir = true;
		Helicopter->_BoardSystem->enc_course->reset();
		//Helicopter->ccp.i_ap_dir_off = !Helicopter->_BoardSystem->b_autopilot_dir && Helicopter->_BoardSystem->Is27V();
		Helicopter->ccp.i_ap_dir_on = Helicopter->_BoardSystem->b_autopilot_dir && Helicopter->_BoardSystem->Is27V();
		//		Helicopter->AP_->turnOn_yaw( );
		Helicopter->PC.shet_ap_dir = 0;
	}
	else if (Helicopter->ccp.t_ap_dir_off)
	{
		Helicopter->_BoardSystem->b_autopilot_dir = false;
		Helicopter->_BoardSystem->enc_course->reset();
		//Helicopter->ccp.i_ap_dir_off = !Helicopter->_BoardSystem->b_autopilot_dir && Helicopter->_BoardSystem->Is27V();
		Helicopter->ccp.i_ap_dir_on = Helicopter->_BoardSystem->b_autopilot_dir && Helicopter->_BoardSystem->Is27V();
		Helicopter->PC.shet_ap_dir = 0;
		//		Helicopter->AP_->turnOff_yaw( );
	}
	else if (Helicopter->PC.t_ap_dir_sopr && Helicopter->ccp.i_ap_dir_on) {
		Helicopter->PC.shet_ap_dir += 1;
		if (Helicopter->PC.shet_ap_dir < 4) {
			Helicopter->_BoardSystem->b_autopilot_dir = false;
			Helicopter->_BoardSystem->enc_course->reset();
		}
		else {
			Helicopter->PC.shet_ap_dir = 150;
			Helicopter->_BoardSystem->b_autopilot_dir = true;
			Helicopter->ccp.i_ap_dir_on = Helicopter->_BoardSystem->b_autopilot_dir && Helicopter->_BoardSystem->Is27V();
		}
	}

	if (Helicopter->ccp.t_ap_alt_on)
	{
		Helicopter->_BoardSystem->b_autopilot_alt = true;
		Helicopter->ccp.i_ap_alt_on = Helicopter->_BoardSystem->b_autopilot_alt && Helicopter->_BoardSystem->Is27V();
		//Helicopter->ccp.i_ap_alt_off = !Helicopter->_BoardSystem->b_autopilot_alt && Helicopter->_BoardSystem->Is27V();
//		Helicopter->AP_->turnOn_height();
	}
	if (Helicopter->ccp.t_ap_speed && !Helicopter->AP_->getSpeedState()) // не включаем если уже включен
	{
		Helicopter->_BoardSystem->b_autopilot_speed = true;
		Helicopter->AP_->turnOn_speed();
	}
	if (!Helicopter->ccp.t_ap_speed)
	{
		Helicopter->_BoardSystem->b_autopilot_speed = false;
		Helicopter->AP_->turnOff_speed();
	}
	if (Helicopter->ccp.t_ap_alt_off || Helicopter->PC.t_fr_l_fosh)
	{
		Helicopter->_BoardSystem->b_autopilot_alt = false;
		//Helicopter->ccp.i_ap_alt_on  = Helicopter->_BoardSystem->b_autopilot_alt && Helicopter->_BoardSystem->Is27V();
		//Helicopter->ccp.i_ap_alt_off = !Helicopter->_BoardSystem->b_autopilot_alt && Helicopter->_BoardSystem->Is27V();
//		Helicopter->AP_->turnOff_height( );
	}
	Helicopter->ccp.f_get_curs_math = Helicopter->ccp.f_get_curs;
	if (std::abs(abs(Helicopter->ccp.pred_set_curs) - abs(Helicopter->ccp.f_set_curs)) > 0.9) {
		int forward = (static_cast<qint8>(Helicopter->ccp.f_set_curs - Helicopter->ccp.pred_set_curs + 10.f)) % 10;
		int backward = (static_cast<qint8>(Helicopter->ccp.pred_set_curs - Helicopter->ccp.f_set_curs + 10.f)) % 10;
		Helicopter->ccp.pred_set_curs = Helicopter->ccp.f_set_curs;
		if (forward < backward) {
			Helicopter->ccp.res_f_set_curs = forward * 1;
			if (!Helicopter->PC.t_ap_dir_sopr && Helicopter->ccp.i_ap_dir_on) {
				Helicopter->ccp.f_get_curs_math -= 0.1f;
				if (Helicopter->ccp.f_get_curs_math < -1)
					Helicopter->ccp.f_get_curs_math = -1;
				Helicopter->ccp.f_get_curs = Helicopter->ccp.f_get_curs_math;
			}
		}
		else {
			Helicopter->ccp.res_f_set_curs = forward * -1;
			if (!Helicopter->PC.t_ap_dir_sopr && Helicopter->ccp.i_ap_dir_on)
			{
				Helicopter->ccp.f_get_curs_math += 0.1f;
				if (Helicopter->ccp.f_get_curs_math > 1)
					Helicopter->ccp.f_get_curs_math = 1;
				Helicopter->ccp.f_get_curs = Helicopter->ccp.f_get_curs_math;
			}
		}
	}
	else
		Helicopter->ccp.res_f_set_curs = 0;

	if (std::abs(abs(Helicopter->ccp.pred_set_roll) - abs(Helicopter->ccp.f_set_roll)) > 0.9) {
		int forward = (static_cast<qint8>(Helicopter->ccp.f_set_roll - Helicopter->ccp.pred_set_roll + 10.f)) % 10;
		int backward = (static_cast<qint8>(Helicopter->ccp.pred_set_roll - Helicopter->ccp.f_set_roll + 10.f)) % 10;
		Helicopter->ccp.pred_set_roll = Helicopter->ccp.f_set_roll;
		if (forward < backward)
			Helicopter->ccp.res_f_set_roll = forward * (5);
		else
			Helicopter->ccp.res_f_set_roll = backward * (-5);
		if (Helicopter->lpb.rv_5.alt() > 3.f) 
			Helicopter->ccp.res_f_set_roll *= (-1);  // Так как на предполетной планки отрабатывают нормально, а в полете реверс по крену
	}
	else
		Helicopter->ccp.res_f_set_roll = 0;

	if (std::abs(abs(Helicopter->ccp.pred_set_pitch) - abs(Helicopter->ccp.f_set_pitch)) > 0.9) {
		int forward = (static_cast<qint8>(Helicopter->ccp.f_set_pitch - Helicopter->ccp.pred_set_pitch + 10.f)) % 10;
		int backward = (static_cast<qint8>(Helicopter->ccp.pred_set_pitch - Helicopter->ccp.f_set_pitch + 10.f)) % 10;
		Helicopter->ccp.pred_set_pitch = Helicopter->ccp.f_set_pitch;
		if (forward < backward)
			Helicopter->ccp.res_f_set_pitch = forward * 5;
		else
			Helicopter->ccp.res_f_set_pitch = backward * (- 5);
	}
	else
		Helicopter->ccp.res_f_set_pitch = 0;

	//отказы с инструктора
	if (Helicopter->instructor->fault.Autopilot) {
		Helicopter->_BoardSystem->b_autopilot = false;
		Helicopter->_BoardSystem->b_autopilot_dir = false;
		Helicopter->_BoardSystem->b_autopilot_alt = false;
		Helicopter->_BoardSystem->enc_course->reset();
		Helicopter->_BoardSystem->enc_roll->reset();
		Helicopter->_BoardSystem->enc_pitch->reset();
		Helicopter->AP_->turnOff_roll_pitch();
		Helicopter->ccp.t_ap_on = Helicopter->_BoardSystem->b_autopilot && Helicopter->_BoardSystem->Is27V();
		Helicopter->ccp.i_ap_alt_off = true;
		Helicopter->ccp.i_ap_dir_off = true;
	}
	if (Helicopter->instructor->fault.Autopilot_curs) {
		Helicopter->_BoardSystem->b_autopilot_dir = false;
		Helicopter->_BoardSystem->enc_course->reset();
		Helicopter->ccp.i_ap_dir_off = true;
	}
	if (Helicopter->instructor->fault.Autopilot_pitch) {
		Helicopter->_BoardSystem->b_autopilot = false;
		Helicopter->_BoardSystem->enc_roll->reset();
		Helicopter->_BoardSystem->enc_pitch->reset();
		Helicopter->AP_->turnOff_roll_pitch();
		Helicopter->ccp.t_ap_on = Helicopter->_BoardSystem->b_autopilot && Helicopter->_BoardSystem->Is27V();
	}
	if (Helicopter->instructor->fault.Autopilot_alt) {
		Helicopter->_BoardSystem->b_autopilot_alt = false;
		Helicopter->AP_->turnOff_roll_pitch();
		Helicopter->ccp.i_ap_dir_off = true;
	}
}

void Helicopter_Onboard::CalcVSU()
{
	// ЗАПУСК ТА
	if (Helicopter->cep.t_ta_start == true) // если нажата кнопка старта ТА
	{
		if (Helicopter->_BoardSystem->_VSU->is_off())
		{
			StartTAProc();
		}
	}
	// останов ТА
	if (Helicopter->cep.t_ta_stop == true) // если нажата кнопка старта ТА
	{
		if (!Helicopter->_BoardSystem->_VSU->is_off())
		{
			Helicopter->_BoardSystem->_VSU->stop_TA();
		}
	}
}

void Helicopter_Onboard::CalcEngine()
{
	// ЗАПУСК ДВИГАТЕЛЯ
	if (Helicopter->cep.t_eng_start == true) // если нажата кнопка старта двигателя
	{
		StartEngineProc();
	}
	/****************************************************************************************************/
	// во время запуска включили подачу топлива, то запускаем двигатель
	if (Helicopter->_BoardSystem->_Engine_1->is_starting() &&
		(!Helicopter->PC.t_eng_left_brake && Helicopter->cep.t_fuel_left_close))
	{
		Helicopter->_BoardSystem->_Engine_1->fuel_eng();
	}
	// во время ложного запуска включили подачу топлива, то запускаем двигатель
	if (Helicopter->_BoardSystem->_Engine_2->is_starting() &&
		(!Helicopter->PC.t_eng_rght_brake && Helicopter->cep.t_fuel_right_close))
	{
		Helicopter->_BoardSystem->_Engine_2->fuel_eng();
	}
	/*****************************************************************************************************/
	// прерывание запуска, прокрутки или ложного запуска двигателя кнопкой STOP
	if (Helicopter->cep.t_eng_stop)
	{
		if (Helicopter->_BoardSystem->_Engine_1->is_starting())
		{
			Helicopter->_BoardSystem->_Engine_1->stop_eng();
		}
		if (Helicopter->_BoardSystem->_Engine_2->is_starting())
		{
			Helicopter->_BoardSystem->_Engine_2->stop_eng();
		}
		Helicopter->_BoardSystem->eng_automat_on = false;
		Helicopter->_BoardSystem->ta_starter_on = false;
	}
	/*****************************************************************************************************/
	// ОСТАНОВКА ДВИГАТЕЛЯ ЛЕВОГО
	if (Helicopter->_BoardSystem->_Engine_1->is_working() &&
		(Helicopter->PC.t_eng_left_brake || !Helicopter->cep.t_fuel_left_close))
	{
		Helicopter->_BoardSystem->_Engine_1->stop_eng();
	}
	// ОСТАНОВКА ДВИГАТЕЛЯ ПРАВОГО
	if (Helicopter->_BoardSystem->_Engine_2->is_working() &&
		(Helicopter->PC.t_eng_rght_brake || !Helicopter->cep.t_fuel_right_close))
	{
		Helicopter->_BoardSystem->_Engine_2->stop_eng();
	}
}

void Helicopter_Onboard::CalcPPS()
{
	// Иф для отработки предполетной подготовки
	if (!Helicopter->cep.t_contr_sensors){
		Helicopter->cep.i_eng_left_fire = Helicopter->instructor->fault.Fire; // включаем пожар левого двигателя
		Helicopter->cep.i_eng_rght_fire = Helicopter->instructor->fault.Fire_R; // включаем пожар левого двигателя
		Helicopter->cep.i_ko_50_fire = Helicopter->instructor->fault.Fire_KO; // включаем пожар левого двигателя
		Helicopter->cep.i_red_vsu_fire = Helicopter->instructor->fault.Fire_VSU; // включаем пожар левого двигателя
	}

	if (Helicopter->cep.t_contr_sensors_0)
		Helicopter->cep.sw_contr_sensors = 0;
	else if (Helicopter->cep.t_contr_sensors_1)
		Helicopter->cep.sw_contr_sensors = 1;
	else if (Helicopter->cep.t_contr_sensors_2)
		Helicopter->cep.sw_contr_sensors = 2;
	else if (Helicopter->cep.t_contr_sensors_3)
		Helicopter->cep.sw_contr_sensors = 3;
	else if (Helicopter->cep.t_contr_sensors_4)
		Helicopter->cep.sw_contr_sensors = 4;
	else if (Helicopter->cep.t_contr_sensors_5)
		Helicopter->cep.sw_contr_sensors = 5;
	else if (Helicopter->cep.t_contr_sensors_6)
		Helicopter->cep.sw_contr_sensors = 6;
	else if (Helicopter->cep.t_contr_sensors_7)
		Helicopter->cep.sw_contr_sensors = 7;
	switch (Helicopter->cep.sw_contr_sensors)
	{
	case 0:
		if (Helicopter->cep.t_contr_sensors) {
			Helicopter->cep.i_eng_left_fire = false;
			Helicopter->cep.i_eng_rght_fire = false;
			Helicopter->cep.i_ko_50_fire = false;
			Helicopter->cep.i_red_vsu_fire = false;
		}
	case 1:
		if (Helicopter->cep.t_contr_sensors) {
			Helicopter->cep.i_eng_left_fire = false;
			Helicopter->cep.i_eng_rght_fire = false;
			Helicopter->cep.i_ko_50_fire = false;
			Helicopter->cep.i_red_vsu_fire = false;
		}
		break;
	case 2:
		if (Helicopter->cep.t_contr_sensors) {
			Helicopter->cep.i_eng_left_fire = true;
			Helicopter->cep.i_eng_rght_fire = true;
			Helicopter->cep.i_ko_50_fire = true;
			Helicopter->cep.i_red_vsu_fire = true;
		}
		break;
	case 3:
		if (Helicopter->cep.t_contr_sensors) {
			Helicopter->cep.i_eng_left_fire = true;
			Helicopter->cep.i_eng_rght_fire = true;
			Helicopter->cep.i_ko_50_fire = true;
			Helicopter->cep.i_red_vsu_fire = true;
		}
		if (Helicopter->cep.t_firefighting)
		{ // включен режим огнетушения ППС
			Helicopter->cep.i_eng_left_1_queue |= true;
			Helicopter->cep.i_eng_rght_1_queue |= true;
			Helicopter->cep.i_ko_50_1_queue |= true;
			Helicopter->cep.i_red_vsu_1_queue |= true;
		}
		break;
	case 4:
		if (Helicopter->cep.t_contr_sensors) {
			Helicopter->cep.i_eng_left_fire = true;
			Helicopter->cep.i_eng_rght_fire = true;
			Helicopter->cep.i_ko_50_fire = false;
			Helicopter->cep.i_red_vsu_fire = true;
		}
		if (Helicopter->cep.t_firefighting)
		{ // включен режим огнетушения ППС
			Helicopter->cep.i_eng_left_1_queue |= true;
			Helicopter->cep.i_eng_rght_1_queue |= true;
			Helicopter->cep.i_ko_50_1_queue |= false;
			Helicopter->cep.i_red_vsu_1_queue |= true;
		}
		break;
	case 5:
		if (Helicopter->cep.t_contr_sensors) {
			Helicopter->cep.i_eng_left_fire = false;
			Helicopter->cep.i_eng_rght_fire = false;
			Helicopter->cep.i_ko_50_fire = false;
			Helicopter->cep.i_red_vsu_fire = true;
		}
	case 6:
		if (Helicopter->cep.t_contr_sensors) {
			Helicopter->cep.i_eng_left_fire = false;
			Helicopter->cep.i_eng_rght_fire = false;
			Helicopter->cep.i_ko_50_fire = false;
			Helicopter->cep.i_red_vsu_fire = true;
		}
	case 7:
		Helicopter->cep.i_eng_left_fire = false;
		Helicopter->cep.i_eng_rght_fire = false;
		Helicopter->cep.i_ko_50_fire = false;
		Helicopter->cep.i_red_vsu_fire = true;
		if (Helicopter->cep.t_firefighting)
		{ // включен режим огнетушения ППС
			Helicopter->cep.i_eng_left_1_queue |= false;
			Helicopter->cep.i_eng_rght_1_queue |= false;
			Helicopter->cep.i_ko_50_1_queue |= false;
			Helicopter->cep.i_red_vsu_1_queue |= true;
		}
	}

	if (Helicopter->cep.t_firefighting)
	{ // включен режим огнетушения ППС
		Helicopter->cep.i_eng_left_1_queue |= Helicopter->cep.t_eng_left_1_queue && Helicopter->_BoardSystem->pps[0] != 0;
		Helicopter->cep.i_eng_left_2_queue |= Helicopter->cep.t_eng_left_2_queue && Helicopter->_BoardSystem->pps[1] != 0;
		Helicopter->cep.i_eng_rght_1_queue |= Helicopter->cep.t_eng_rght_1_queue && Helicopter->_BoardSystem->pps[0] != 0;
		Helicopter->cep.i_eng_rght_2_queue |= Helicopter->cep.t_eng_rght_2_queue && Helicopter->_BoardSystem->pps[1] != 0;
		Helicopter->cep.i_ko_50_1_queue |= Helicopter->cep.t_ko_50_1_queue && Helicopter->_BoardSystem->pps[0] != 0;
		Helicopter->cep.i_ko_50_2_queue |= Helicopter->cep.t_ko_50_2_queue && Helicopter->_BoardSystem->pps[1] != 0;
		Helicopter->cep.i_red_vsu_1_queue |= Helicopter->cep.t_red_vsu_1_queue && Helicopter->_BoardSystem->pps[0] != 0;
		Helicopter->cep.i_red_vsu_2_queue |= Helicopter->cep.t_red_vsu_2_queue && Helicopter->_BoardSystem->pps[1] != 0;

		if (Helicopter->_BoardSystem->pps[0] && Helicopter->cep.i_eng_left_1_queue)
		{
			_audio->PlayOnce(17);
			Helicopter->_BoardSystem->pps[0] = false;
			Helicopter->instructor->fault.Fire = false;
		}
		if (Helicopter->_BoardSystem->pps[0] && Helicopter->cep.i_eng_rght_1_queue)
		{
			_audio->PlayOnce(17);
			Helicopter->instructor->fault.Fire_R = false;
			Helicopter->_BoardSystem->pps[0] = false;
		}
		if (Helicopter->_BoardSystem->pps[0] && Helicopter->cep.i_ko_50_1_queue)
		{
			_audio->PlayOnce(17);
			Helicopter->_BoardSystem->pps[0] = false;
			Helicopter->instructor->fault.Fire_KO = false;
		}
		if (Helicopter->_BoardSystem->pps[0] && Helicopter->cep.i_red_vsu_1_queue)
		{
			_audio->PlayOnce(17);
			Helicopter->_BoardSystem->pps[0] = false;
			Helicopter->instructor->fault.Fire_VSU = false;
		}
		if (Helicopter->_BoardSystem->pps[1] && Helicopter->cep.i_eng_left_2_queue)
		{
			_audio->PlayOnce(17);
			Helicopter->_BoardSystem->pps[1] = false;
			Helicopter->instructor->fault.Fire = false;
		}
		if (Helicopter->_BoardSystem->pps[1] && Helicopter->cep.i_eng_rght_2_queue)
		{
			_audio->PlayOnce(17);
			Helicopter->_BoardSystem->pps[1] = false;
			Helicopter->instructor->fault.Fire_R = false;
		}
		if (Helicopter->_BoardSystem->pps[1] && Helicopter->cep.i_ko_50_2_queue)
		{
			_audio->PlayOnce(17);
			Helicopter->_BoardSystem->pps[1] = false;
			Helicopter->instructor->fault.Fire_KO = false;
		}
		if (Helicopter->_BoardSystem->pps[1] && Helicopter->cep.i_red_vsu_2_queue)
		{
			_audio->PlayOnce(17);
			Helicopter->_BoardSystem->pps[1] = false;
			Helicopter->instructor->fault.Fire_VSU = false;
		}
	}
	if (!Helicopter->rp_azs.t_ppo_signal) {
		Helicopter->cep.i_eng_left_fire = false;
		Helicopter->cep.i_eng_rght_fire = false;
		Helicopter->cep.i_ko_50_fire = false;
		Helicopter->cep.i_red_vsu_fire = false;
	}
}

void Helicopter_Onboard::CalcIcing()
{
	// условие автоматического включения ПОС - начало облединения или ручная работа(всегда включен)
	// выключение только по кнопке отключения
	// Облединение возникает при температуре ниже +5 при полете в облачности если она привышает условне 5 балов
	// так же при дальности видимости меньше 2 км по туману.
	// TODO условие возникновления облединения нужно расчитать.
	if (Helicopter->_BoardSystem->Temp < -5)
	{
		Helicopter->_BoardSystem->b_icing = true;
	}
	else
	{
		Helicopter->_BoardSystem->b_icing = false;
	}

	if (Helicopter->_BoardSystem->b_icing && !Helicopter->lp_elp.t_deicing_auto && !Helicopter->instructor->fault.pos_vint)
	{
		Helicopter->_BoardSystem->b_POS = Helicopter->Is208V();
	}
	// такая хитрая конструкция нужна, так как автоматически POS не выключается автоматически
	if (Helicopter->lp_elp.t_deicing_off) // нажата кнопка отключения ПОС, сработает если нет условий работы для ПОС
	{
		Helicopter->_BoardSystem->b_POS = false;
	}
	// расчет показаний амперетра
	if (Helicopter->Is115V())
	{
		switch (Helicopter->lp_elp.get_consumers())
		{
		case 0:	// выкл
			Helicopter->lp_elp.f_ampermetr = 0;
			Helicopter->lp_elp.i_section_1 = false;
			Helicopter->lp_elp.i_section_2 = false;
			Helicopter->lp_elp.i_section_3 = false;
			Helicopter->lp_elp.i_section_4 = false;
			break;
		case 1:	// лопасть 1
			if (Helicopter->lp_elp.i_pos_on && Helicopter->rp_azs.t_ais_ctrl)
				Helicopter->lp_elp.f_ampermetr = 60.f;
			else
				Helicopter->lp_elp.f_ampermetr = 0.f;
			if (Helicopter->instructor->fault.pos_vint)
				Helicopter->lp_elp.f_ampermetr = 150.f;
			Helicopter->lp_elp.i_section_1 = true;
			Helicopter->lp_elp.i_section_2 = false;
			Helicopter->lp_elp.i_section_3 = false;
			Helicopter->lp_elp.i_section_4 = false;
			break;
		case 2:	// лопасть 2
			if (Helicopter->lp_elp.i_pos_on && Helicopter->rp_azs.t_ais_ctrl)
				Helicopter->lp_elp.f_ampermetr = 65.f;
			else
				Helicopter->lp_elp.f_ampermetr = 0.f;
			if (Helicopter->instructor->fault.pos_vint)
				Helicopter->lp_elp.f_ampermetr = 150.f;
			Helicopter->lp_elp.i_section_2 = true;
			Helicopter->lp_elp.i_section_1 = false;
			Helicopter->lp_elp.i_section_3 = false;
			Helicopter->lp_elp.i_section_4 = false;
			break;
		case 3:	// лопасть 3
			if (Helicopter->lp_elp.i_pos_on && Helicopter->rp_azs.t_ais_ctrl)
				Helicopter->lp_elp.f_ampermetr = 70.f;
			else
				Helicopter->lp_elp.f_ampermetr = 0.f;
			if (Helicopter->instructor->fault.pos_vint)
				Helicopter->lp_elp.f_ampermetr = 150.f;
			Helicopter->lp_elp.i_section_3 = true;
			Helicopter->lp_elp.i_section_1 = false;
			Helicopter->lp_elp.i_section_2 = false;
			Helicopter->lp_elp.i_section_4 = false;
			break;
		case 4:	// лопасть 4
			if (Helicopter->lp_elp.i_pos_on && Helicopter->rp_azs.t_ais_ctrl)
				Helicopter->lp_elp.f_ampermetr = 75.f;
			else
				Helicopter->lp_elp.f_ampermetr = 0.f;
			if (Helicopter->instructor->fault.pos_vint)
				Helicopter->lp_elp.f_ampermetr = 150.f;
			Helicopter->lp_elp.i_section_4 = true;
			Helicopter->lp_elp.i_section_1 = false;
			Helicopter->lp_elp.i_section_2 = false;
			Helicopter->lp_elp.i_section_3 = false;
			break;
		case 5:	// лопасть 5
			if (Helicopter->lp_elp.i_pos_on && Helicopter->rp_azs.t_ais_ctrl)
				Helicopter->lp_elp.f_ampermetr = 80.f;
			else
				Helicopter->lp_elp.f_ampermetr = 0.f;
			if (Helicopter->instructor->fault.pos_vint)
				Helicopter->lp_elp.f_ampermetr = 150.f;
			Helicopter->lp_elp.i_section_1 = false;
			Helicopter->lp_elp.i_section_2 = false;
			Helicopter->lp_elp.i_section_3 = false;
			Helicopter->lp_elp.i_section_4 = false;
			break;
		case 6:	// рулевой
			if (Helicopter->lp_elp.i_pos_on && Helicopter->rp_azs.t_ais_ctrl)
				Helicopter->lp_elp.f_ampermetr = 120.f;
			else
				Helicopter->lp_elp.f_ampermetr = 0.f;
			if (Helicopter->instructor->fault.pos_vint)
				Helicopter->lp_elp.f_ampermetr = 150.f;
			Helicopter->lp_elp.i_section_1 = false;
			Helicopter->lp_elp.i_section_2 = false;
			Helicopter->lp_elp.i_section_3 = false;
			Helicopter->lp_elp.i_section_4 = false;
			break;
		case 7:	// стекол
			if (Helicopter->lp_elp.t_deicing_glass && Helicopter->rp_azs.t_ais_glass)
				Helicopter->lp_elp.f_ampermetr = 70.f;
			else
				Helicopter->lp_elp.f_ampermetr = 0.f;
			if (Helicopter->instructor->fault.pos_glass)
				Helicopter->lp_elp.f_ampermetr = 150.f;
			Helicopter->lp_elp.i_section_1 = false;
			Helicopter->lp_elp.i_section_2 = false;
			Helicopter->lp_elp.i_section_3 = false;
			Helicopter->lp_elp.i_section_4 = false;
			break;
		case 8:	// пзу правого
			Helicopter->lp_elp.f_ampermetr = Helicopter->lp_elp.i_heat_right ? 110.0f : 0.f;
			if (Helicopter->instructor->fault.pos_pzu)
				Helicopter->lp_elp.f_ampermetr = 150.f;
			Helicopter->lp_elp.i_section_1 = false;
			Helicopter->lp_elp.i_section_2 = false;
			Helicopter->lp_elp.i_section_3 = false;
			Helicopter->lp_elp.i_section_4 = false;
			break;
		case 9:	// пзу левого
			Helicopter->lp_elp.f_ampermetr = (Helicopter->lp_elp.i_heat_left && Helicopter->lp_elp.t_deicing_left_pzu) ? 100.0f : 0.f;
			if (Helicopter->instructor->fault.pos_pzu)
				Helicopter->lp_elp.f_ampermetr = 150.f;
			Helicopter->lp_elp.i_section_1 = false;
			Helicopter->lp_elp.i_section_2 = false;
			Helicopter->lp_elp.i_section_3 = false;
			Helicopter->lp_elp.i_section_4 = false;
			break;
		}
	}
	else
	{
		Helicopter->lp_elp.f_ampermetr = 0.f;
		Helicopter->lp_elp.i_section_1 = false;
		Helicopter->lp_elp.i_section_2 = false;
		Helicopter->lp_elp.i_section_3 = false;
		Helicopter->lp_elp.i_section_4 = false;
	}

	if ((!Helicopter->_BoardSystem->_Engine_1->is_working() || !Helicopter->_BoardSystem->_Engine_2->is_working())) {
		if (Helicopter->Is115V() && Helicopter->rp_azs.t_ais_pzu_left && Helicopter->rp_azs.t_ais_pzu_rght) {
			if (Helicopter->lp_elp.t_deicing_auto) {
				Helicopter->lp_elp.i_pos_on = Helicopter->_BoardSystem->b_POS = true;
				Helicopter->lp_elp.i_heat_right = true;
				Helicopter->lp_elp.i_right_pzu_front = true;
				Helicopter->lp_elp.i_right_pzu_back = true;
				Helicopter->lp_elp.t_deicing_off = false;
				if (Helicopter->lp_elp.t_deicing_left_pzu && !Helicopter->_BoardSystem->b_POS) {
					Helicopter->lp_elp.i_heat_left = true;
					Helicopter->lp_elp.i_left_pzu_front = true;
					Helicopter->lp_elp.i_left_pzu_back = true;
				}
			}
			else {
				Helicopter->lp_elp.i_pos_on = Helicopter->_BoardSystem->b_POS;
				//Helicopter->lp_elp.i_fault_pos = Helicopter->_BoardSystem->b_POS;
				Helicopter->lp_elp.i_heat_right = Helicopter->_BoardSystem->b_POS;
				Helicopter->lp_elp.i_right_pzu_front = Helicopter->_BoardSystem->b_POS;
				Helicopter->lp_elp.i_right_pzu_back = Helicopter->_BoardSystem->b_POS;

				if (Helicopter->lp_elp.t_deicing_right_pzu && !Helicopter->_BoardSystem->b_POS) {
					Helicopter->lp_elp.i_heat_right = true;
					Helicopter->lp_elp.i_right_pzu_front = true;
					Helicopter->lp_elp.i_right_pzu_back = true;
				}
				else if (!Helicopter->_BoardSystem->b_POS){
					Helicopter->lp_elp.i_heat_right = false;
					Helicopter->lp_elp.i_right_pzu_front = false;
					Helicopter->lp_elp.i_right_pzu_back = false;
				}
				if (Helicopter->lp_elp.t_deicing_left_pzu && !Helicopter->_BoardSystem->b_POS) {
					Helicopter->lp_elp.i_heat_left = true;
					Helicopter->lp_elp.i_left_pzu_front = true;
					Helicopter->lp_elp.i_left_pzu_back = true;
				}
				else if (!Helicopter->_BoardSystem->b_POS){
					Helicopter->lp_elp.i_heat_left = false;
					Helicopter->lp_elp.i_left_pzu_front = false;
					Helicopter->lp_elp.i_left_pzu_back = false;
				}
			}
		}
		if (Helicopter->Is27V() && Helicopter->rp_azs.t_ais_co_121) {
			if (Helicopter->lp_elp.t_imitation && counter == 0) {
				Helicopter->lp_elp.i_imitation_cold = true;
				Helicopter->lp_elp.i_imitation_hot = true;
				Helicopter->lp_elp.i_icing = true;
				startCounter();
				ImitatioColdTimer->start(1000);
			}
			if (counter != 0) {
				if (getCheckCondition() > 7 && getCheckCondition() <= 10) {
					Helicopter->lp_elp.i_icing = false;
					Helicopter->lp_elp.i_imitation_cold = false;
					Helicopter->lp_elp.i_imitation_hot = false;
				}
				if (getCheckCondition() > 35 && getCheckCondition() < 40)
					Helicopter->lp_elp.i_so_121_prop = true;
				if (getCheckCondition() > 85) {
					Helicopter->lp_elp.i_so_121_prop = false;
					ImitatioColdTimer->stop();
					counter = 0;
				}
			}
		}
	}
	else if (Helicopter->Is115V()) {
		if (Helicopter->rp_azs.t_ais_pzu_left && Helicopter->rp_azs.t_ais_pzu_rght) {
			if (Helicopter->lp_elp.t_deicing_auto) {
				Helicopter->lp_elp.i_pos_on = Helicopter->_BoardSystem->b_POS = true;
				Helicopter->lp_elp.i_heat_right = true;
				Helicopter->lp_elp.i_right_pzu_front = true;
				Helicopter->lp_elp.i_right_pzu_back = true;
				Helicopter->lp_elp.t_deicing_off = false;
			}
			else {
				Helicopter->lp_elp.i_pos_on = Helicopter->_BoardSystem->b_POS;
				//Helicopter->lp_elp.i_fault_pos = Helicopter->_BoardSystem->b_POS;
				Helicopter->lp_elp.i_heat_right = Helicopter->_BoardSystem->b_POS;
				Helicopter->lp_elp.i_right_pzu_front = Helicopter->_BoardSystem->b_POS;
				Helicopter->lp_elp.i_right_pzu_back = Helicopter->_BoardSystem->b_POS;

				if (Helicopter->lp_elp.t_deicing_right_pzu && !Helicopter->_BoardSystem->b_POS) {
					Helicopter->lp_elp.i_heat_right = true;
					Helicopter->lp_elp.i_right_pzu_front = true;
					Helicopter->lp_elp.i_right_pzu_back = true;
				}
				else if (!Helicopter->_BoardSystem->b_POS) {
					Helicopter->lp_elp.i_heat_right = false;
					Helicopter->lp_elp.i_right_pzu_front = false;
					Helicopter->lp_elp.i_right_pzu_back = false;
				}
				if (Helicopter->lp_elp.t_deicing_left_pzu && !Helicopter->_BoardSystem->b_POS) {
					Helicopter->lp_elp.i_heat_left = true;
					Helicopter->lp_elp.i_left_pzu_front = true;
					Helicopter->lp_elp.i_left_pzu_back = true;
				}
				else if (!Helicopter->_BoardSystem->b_POS) {
					Helicopter->lp_elp.i_heat_left = false;
					Helicopter->lp_elp.i_left_pzu_front = false;
					Helicopter->lp_elp.i_left_pzu_back = false;
				}
			}
		}
	}
	if (Helicopter->lpb.rv_5.alt() > 50.f && Helicopter->rp_azs.t_ais_pzu_left && Helicopter->rp_azs.t_ais_pzu_rght) {
		Helicopter->lp_elp.i_icing = Helicopter->_BoardSystem->b_icing && Helicopter->_BoardSystem->Is27V();  // облединение
		Helicopter->lp_elp.i_pos_on = Helicopter->_BoardSystem->b_POS && Helicopter->_BoardSystem->Is27V() && Helicopter->_BoardSystem->Is208V();
		Helicopter->lp_elp.i_heat_right = (Helicopter->_BoardSystem->b_POS || Helicopter->rsp_ep.t_pzu_right) &&
			Helicopter->_BoardSystem->Is27V() && Helicopter->_BoardSystem->Is208V();
		Helicopter->lp_elp.i_right_pzu_front = (Helicopter->_BoardSystem->b_POS || Helicopter->rsp_ep.t_pzu_right)
			&& Helicopter->_BoardSystem->Is27V() && Helicopter->_BoardSystem->Is208V();
		Helicopter->lp_elp.i_right_pzu_back = (Helicopter->_BoardSystem->b_POS || Helicopter->rsp_ep.t_pzu_right) &&
			Helicopter->_BoardSystem->Is27V() && Helicopter->_BoardSystem->Is208V();

		// TODO включать секции по таймеру через 38,5 секунды
		Helicopter->lp_elp.i_section_1 = Helicopter->_BoardSystem->b_POS && Helicopter->_BoardSystem->u_pos_switch == 0 &&
			Helicopter->_BoardSystem->Is27V() && Helicopter->_BoardSystem->Is208V();
		Helicopter->lp_elp.i_section_2 = Helicopter->_BoardSystem->b_POS && Helicopter->_BoardSystem->u_pos_switch == 1 &&
			Helicopter->_BoardSystem->Is27V() && Helicopter->_BoardSystem->Is208V();
		Helicopter->lp_elp.i_section_3 = Helicopter->_BoardSystem->b_POS && Helicopter->_BoardSystem->u_pos_switch == 2 &&
			Helicopter->_BoardSystem->Is27V() && Helicopter->_BoardSystem->Is208V();
		Helicopter->lp_elp.i_section_4 = Helicopter->_BoardSystem->b_POS && Helicopter->_BoardSystem->u_pos_switch == 3 &&
			Helicopter->_BoardSystem->Is27V() && Helicopter->_BoardSystem->Is208V(); 
		
		Helicopter->lp_elp.i_heat_left = Helicopter->lp_elp.t_deicing_left_pzu &&
			Helicopter->_BoardSystem->Is27V() && Helicopter->_BoardSystem->Is208V();

		Helicopter->lp_elp.i_left_pzu_front = Helicopter->lp_elp.t_deicing_left_pzu &&
			Helicopter->_BoardSystem->Is27V() && Helicopter->_BoardSystem->Is208V();

		Helicopter->lp_elp.i_left_pzu_back = Helicopter->lp_elp.t_deicing_left_pzu &&
			Helicopter->_BoardSystem->Is27V() && Helicopter->_BoardSystem->Is208V();

		Helicopter->lp_elp.i_heat_right = Helicopter->lp_elp.t_deicing_right_pzu &&
			Helicopter->_BoardSystem->Is27V() && Helicopter->_BoardSystem->Is208V();

		Helicopter->lp_elp.i_right_pzu_front = Helicopter->lp_elp.t_deicing_right_pzu &&
			Helicopter->_BoardSystem->Is27V() && Helicopter->_BoardSystem->Is208V();

		Helicopter->lp_elp.i_right_pzu_back = Helicopter->lp_elp.t_deicing_right_pzu &&
			Helicopter->_BoardSystem->Is27V() && Helicopter->_BoardSystem->Is208V();
	}
	if (Helicopter->instructor->fault.pos_vint) {
		Helicopter->lp_elp.i_section_1 = false;
		Helicopter->lp_elp.i_section_2 = false;
		Helicopter->lp_elp.i_section_3 = false;
		Helicopter->lp_elp.i_section_4 = false;
		Helicopter->lp_elp.i_fault_pos = true;
		Helicopter->lp_elp.i_pos_on = false;
	}
	if (Helicopter->instructor->fault.pos_pzu) {
		Helicopter->lp_elp.i_heat_left = false;
		Helicopter->lp_elp.i_heat_right = false;
		Helicopter->lp_elp.i_left_pzu_back = false;
		Helicopter->lp_elp.i_left_pzu_front = false;
		Helicopter->lp_elp.i_right_pzu_back = false;
		Helicopter->lp_elp.i_right_pzu_front = false;
	}
}		 		

void Helicopter_Onboard::CalcDISSContr()
{
	// этот код перенесен в Model::GetParametrs что бы они не спорили с моделью
	//Helicopter->pc_diss.i_contr = false;
	//Helicopter->pc_diss.i_work = false;
	//Helicopter->pc_diss.i_m = false;
	//Helicopter->pc_diss.i_b = false;
	//switch ( Helicopter->pc_diss.get_sw_mode( ) )
	//{
	//case 1:
	//	Helicopter->pc_diss.i_contr = true;
	//	Helicopter->lpb.ss.speed_x = 17;
	//	Helicopter->lpb.ss.speed_x_down = 0;
	//	Helicopter->lpb.ss.speed_y = 4.5;
	//	Helicopter->lpb.ss.speed_z = 0;
	//	Helicopter->lpb.ss.speed_z_left = 0;
	//	break;
	//case 2:
	//	Helicopter->pc_diss.i_contr = true;
	//	Helicopter->lpb.ss.speed_x = 0;
	//	Helicopter->lpb.ss.speed_x_down = 17;
	//	Helicopter->lpb.ss.speed_y = 3;
	//	Helicopter->lpb.ss.speed_z = 17;
	//	Helicopter->lpb.ss.speed_z_left = 0;
	//	break;
	//case 3:
	//	Helicopter->pc_diss.i_contr = true;
	//	Helicopter->rpb.snos.speed = 136;
	//	Helicopter->rpb.snos.angle = 0;
	//	break;
	//case 4:
	//	Helicopter->pc_diss.i_contr = true;
	//	break;
	//case 5:
	//	Helicopter->pc_diss.i_work = true;
	//	break;
	//}
}

void Helicopter_Onboard::CalcARK()
{
	if (Helicopter->rsp_elp.ark_ud.sw_rkud_mode() == 0 || Helicopter->lpb.t_apk_sv || Helicopter->instructor->fault.Curs || !Helicopter->_BoardSystem->Is27V() || !Helicopter->rp_azs.t_r_compas_ukv) {
		Helicopter->rsp_elp.ark_ud.i_rkud_1 = false;
		Helicopter->rsp_elp.ark_ud.i_rkud_2 = false;
		Helicopter->rsp_elp.ark_ud.i_rkud_3 = false;
		return;
	}
	Helicopter->rsp_elp.ark_9.flag_pomeh = false;
	float ark = 0.f;
	// Предполетная
	if ((Helicopter->rsp_elp.ark_ud.sw_rkud_mode() < 4) && (Helicopter->rsp_elp.ark_ud.sw_rkud_mode() > 0)) {
		if (Helicopter->rsp_elp.ark_ud.sw_rkud_chanel() == 4 && Helicopter->rsp_elp.ark_ud.t_rkud_ukv) {
			if (Helicopter->rsp_elp.ark_ud.t_contr) {
				Helicopter->rsp_elp.ark_ud.i_rkud_1 = Helicopter->rsp_elp.ark_ud.sw_rkud_mode() == 1 || Helicopter->rsp_elp.ark_ud.sw_rkud_mode() == 2 ? true : false; // УП индикатор
				Helicopter->rsp_elp.ark_ud.i_rkud_2 = Helicopter->rsp_elp.ark_ud.sw_rkud_mode() == 2 ? true : false; // ШП индикатор
				Helicopter->rsp_elp.ark_ud.i_rkud_3 = Helicopter->rsp_elp.ark_ud.sw_rkud_mode() == 3 ? true : false; // И индикатор
				if (Helicopter->rp_azs.t_r_spu/* && еще РАДИО на пульте связи и еще РК 2 галетник на пульте СПУ */) {
					Helicopter->rsp_elp.ark_9.flag_pomeh = true;
				}
				else {
					Helicopter->rsp_elp.ark_9.flag_pomeh = false;
				}
				// удерживать ее в таком положении, пока стрелка КУР на приборе УГР - 4УК установится в положение(180 10)
				if (!Helicopter->rsp_elp.ark_ud.t_ant_l && !Helicopter->rsp_elp.ark_ud.t_ant_r) {
					ark = Helicopter->rsp_elp.ark_ud.get_curs(Helicopter->_BoardSystem->Latitude, Helicopter->_BoardSystem->Longitude, Helicopter->_BoardSystem->get_course()) + 180.;
					Helicopter->lpb.ugr_4uk.set_ark(ark);
					Helicopter->rpb.ugr_4uk.set_ark(ark);
				}
				Helicopter->lpb.ugr_4uk.set_ark(ark);
				if (Helicopter->rsp_elp.ark_ud.t_ant_l) {
					ark = Helicopter->rsp_elp.ark_ud.get_curs(Helicopter->_BoardSystem->Latitude, Helicopter->_BoardSystem->Longitude, Helicopter->_BoardSystem->get_course()) + 90.;
					Helicopter->lpb.ugr_4uk.set_ark(ark);
					Helicopter->rpb.ugr_4uk.set_ark(ark);
				}
				else if (Helicopter->rsp_elp.ark_ud.t_ant_r) {
					ark = Helicopter->rsp_elp.ark_ud.get_curs(Helicopter->_BoardSystem->Latitude, Helicopter->_BoardSystem->Longitude, Helicopter->_BoardSystem->get_course()) - 90.;
					Helicopter->lpb.ugr_4uk.set_ark(ark);
					Helicopter->rpb.ugr_4uk.set_ark(ark);
				}
			}
			else {
				Helicopter->rsp_elp.ark_9.flag_pomeh = false;
				if (!Helicopter->lpb.t_apk_sv) {
					ark = Helicopter->rsp_elp.ark_ud.get_curs(Helicopter->_BoardSystem->Latitude, Helicopter->_BoardSystem->Longitude, Helicopter->_BoardSystem->get_course());
					Helicopter->lpb.ugr_4uk.set_ark(ark);
					Helicopter->rpb.ugr_4uk.set_ark(ark);
				}
			}
		}
		if (!Helicopter->rsp_elp.ark_ud.t_rkud_ukv && (Helicopter->rsp_elp.ark_ud.sw_rkud_chanel() == 1)) {
			Helicopter->rsp_elp.ark_9.flag_pomeh = false;
			if (Helicopter->rsp_elp.ark_ud.t_contr) {
				Helicopter->rsp_elp.ark_ud.i_rkud_1 = Helicopter->rsp_elp.ark_ud.sw_rkud_mode() == 1 || Helicopter->rsp_elp.ark_ud.sw_rkud_mode() == 2 ? true : false; // УП индикатор
				Helicopter->rsp_elp.ark_ud.i_rkud_2 = Helicopter->rsp_elp.ark_ud.sw_rkud_mode() == 2 ? true : false; // ШП индикатор
				Helicopter->rsp_elp.ark_ud.i_rkud_3 = Helicopter->rsp_elp.ark_ud.sw_rkud_mode() == 3 ? true : false; // И индикатор
				// удерживать ее в таком положении, пока стрелка КУР на приборе УГР - 4УК установится в положение(180 10)
				if (!Helicopter->rsp_elp.ark_ud.t_ant_l && !Helicopter->rsp_elp.ark_ud.t_ant_r) {
					ark = Helicopter->rsp_elp.ark_ud.get_curs(Helicopter->_BoardSystem->Latitude, Helicopter->_BoardSystem->Longitude, Helicopter->_BoardSystem->get_course()) + 180.;
					Helicopter->lpb.ugr_4uk.set_ark(ark);
					Helicopter->rpb.ugr_4uk.set_ark(ark);
				}
				Helicopter->lpb.ugr_4uk.set_ark(ark);
				if (Helicopter->rsp_elp.ark_ud.t_ant_l) {
					ark = Helicopter->rsp_elp.ark_ud.get_curs(Helicopter->_BoardSystem->Latitude, Helicopter->_BoardSystem->Longitude, Helicopter->_BoardSystem->get_course()) + 90.;
					Helicopter->lpb.ugr_4uk.set_ark(ark);
					Helicopter->rpb.ugr_4uk.set_ark(ark);
				}
				else if (Helicopter->rsp_elp.ark_ud.t_ant_r) {
					ark = Helicopter->rsp_elp.ark_ud.get_curs(Helicopter->_BoardSystem->Latitude, Helicopter->_BoardSystem->Longitude, Helicopter->_BoardSystem->get_course()) - 90.;
					Helicopter->lpb.ugr_4uk.set_ark(ark);
					Helicopter->rpb.ugr_4uk.set_ark(ark);
				}
			}
			else {
				ark = Helicopter->rsp_elp.ark_ud.get_curs(Helicopter->_BoardSystem->Latitude, Helicopter->_BoardSystem->Longitude, Helicopter->_BoardSystem->get_course()); 
				Helicopter->lpb.ugr_4uk.set_ark(ark);
				Helicopter->rpb.ugr_4uk.set_ark(ark);
			}
		}
	}
}

void Helicopter_Onboard::CalcARK_15() {
	if (Helicopter->instructor->fault.ARK || !Helicopter->lpb.t_apk_sv || !Helicopter->rp_azs.t_r_compas_sv || !Helicopter->_BoardSystem->Is27V()) {
		Helicopter->rsp_elp.ark_9.ritaHleb = false;
		Helicopter->rsp_elp.ark_9.ritaAktobe = false;
		return;
	}
	Helicopter->rsp_elp.ark_9.flag_pomeh = false;
	float ark = 0.f;
	if (Helicopter->rsp_elp.ark_9.t_rk_ant && Helicopter->rp_azs.t_r_spu/*&& Добавить галетник РК1 и Радио с пульта связи*/) {
		ark = Helicopter->rsp_elp.ark_9.get_curs(Helicopter->_BoardSystem->Latitude, Helicopter->_BoardSystem->Longitude, Helicopter->_BoardSystem->get_course());
		if (Helicopter->rsp_elp.ark_9.get_freq() == 400 || Helicopter->rsp_elp.ark_9.get_freq() == 490 || Helicopter->rsp_elp.ark_9.get_freq() == 323 || Helicopter->rsp_elp.ark_9.get_freq() == 310 || Helicopter->rsp_elp.ark_9.get_freq() == 10 || Helicopter->rsp_elp.ark_9.get_freq() == 1300 || Helicopter->rsp_elp.ark_9.get_freq() == 1630 || Helicopter->rsp_elp.ark_9.get_freq() == 550) {
			Helicopter->rsp_elp.ark_9.ritaAktobe = true;
		}
		else {
			Helicopter->rsp_elp.ark_9.ritaAktobe = false;
		}
		if (Helicopter->rsp_elp.ark_9.get_freq() == 600 || Helicopter->rsp_elp.ark_9.get_freq() == 1500 || Helicopter->rsp_elp.ark_9.get_freq() == 1710 || Helicopter->rsp_elp.ark_9.get_freq() == 700 || Helicopter->rsp_elp.ark_9.get_freq() == 1610) {
			Helicopter->rsp_elp.ark_9.ritaHleb = true;
		}
		else {
			Helicopter->rsp_elp.ark_9.ritaHleb = false;
		}
	}
	else if (Helicopter->rsp_elp.ark_9.t_rk_com) {
		Helicopter->rsp_elp.ark_9.ritaHleb = false;
		Helicopter->rsp_elp.ark_9.ritaAktobe = false;
		if (!Helicopter->rsp_elp.ark_9.t_rk_tlf && Helicopter->rp_azs.t_r_spu/*&& Добавить галетник РК1 и Радио с пульта связи*/)
			Helicopter->rsp_elp.ark_9.flag_pomeh = true;
		else
			Helicopter->rsp_elp.ark_9.flag_pomeh = false;
		if (Helicopter->rsp_elp.ark_9.t_rk_ramka)
			ark = Helicopter->rsp_elp.ark_9.get_curs(Helicopter->_BoardSystem->Latitude, Helicopter->_BoardSystem->Longitude, Helicopter->_BoardSystem->get_course()) - 100.;
		else
			ark = Helicopter->rsp_elp.ark_9.get_curs(Helicopter->_BoardSystem->Latitude, Helicopter->_BoardSystem->Longitude, Helicopter->_BoardSystem->get_course());
	}
	else if (Helicopter->rsp_elp.ark_9.t_rk_ram) {
		Helicopter->rsp_elp.ark_9.ritaHleb = false;
		Helicopter->rsp_elp.ark_9.ritaAktobe = false;
		if (Helicopter->rsp_elp.ark_9.t_rk_ramka)
			ark = Helicopter->rsp_elp.ark_9.get_curs(Helicopter->_BoardSystem->Latitude, Helicopter->_BoardSystem->Longitude, Helicopter->_BoardSystem->get_course()) - 100.;
		else
			ark = Helicopter->rsp_elp.ark_9.get_curs(Helicopter->_BoardSystem->Latitude, Helicopter->_BoardSystem->Longitude, Helicopter->_BoardSystem->get_course());
	}
	else {
		Helicopter->rsp_elp.ark_9.ritaHleb = false;
		Helicopter->rsp_elp.ark_9.ritaAktobe = false;
		//ark = Helicopter->rsp_elp.ark_9.get_curs(Helicopter->_BoardSystem->Latitude, Helicopter->_BoardSystem->Longitude, Helicopter->_BoardSystem->get_course());
	}
	Helicopter->lpb.ugr_4uk.set_ark(ark);
	Helicopter->rpb.ugr_4uk.set_ark(ark);
}

void Helicopter_Onboard::CalcGMK() {
	if (Helicopter->instructor->fault.Curs) {
		Helicopter->rsp_elp.i_dubl = true;
		Helicopter->rsp_elp.i_osnv = true;
		return;
	}
	auto FPar = Helicopter->Model_->GetFPar();

	Helicopter->rsp_elp.i_dubl = false;
	Helicopter->rsp_elp.i_osnv = false;
	if (Helicopter->Is115V() && Helicopter->rsh_elp.t_gmk && (Helicopter->_BoardSystem->_Electrics->Hgeo < 3)) {
		if (!Helicopter->rsp_elp.t_mk) {
			if (Helicopter->rsp_elp.t_N && Helicopter->rsp_elp.gmk.t_contr0) {
				if (Helicopter->lpb.ugr_4uk.dir() > 2.) {
					Helicopter->lpb.ugr_4uk.set_dir(Helicopter->lpb.ugr_4uk.dir() - 1.0f);
					Helicopter->rpb.ugr_4uk.set_dir(Helicopter->rpb.ugr_4uk.dir() - 1.0f);
				}
				Helicopter->rsp_elp.i_dubl = true;
				Helicopter->rsp_elp.i_osnv = true;
			}
			else if (Helicopter->rsp_elp.t_N && Helicopter->rsp_elp.gmk.t_contr300) {
				if (Helicopter->lpb.ugr_4uk.dir() < 300.) {
					Helicopter->lpb.ugr_4uk.set_dir(Helicopter->lpb.ugr_4uk.dir() + 1.0f);
					Helicopter->rpb.ugr_4uk.set_dir(Helicopter->rpb.ugr_4uk.dir() + 1.0f);
				}
				Helicopter->rsp_elp.i_dubl = true;
				Helicopter->rsp_elp.i_osnv = true;
			}
			else if (Helicopter->rsp_elp.t_zk_l) {
				Helicopter->rpb.ugr_4uk.magnetic += 0.2f;
				if (abs(Helicopter->rpb.ugr_4uk.dir() - (360.f - FPar->Yaw * Ra - Helicopter->rpb.ugr_4uk.magnetic)) > 1.f) {
					if ((static_cast<quint16>(Helicopter->rpb.ugr_4uk.dir() - (360.f - FPar->Yaw * Ra - Helicopter->rpb.ugr_4uk.magnetic) + 360) % 360) <= (static_cast<quint16>((360.f - FPar->Yaw * Ra - Helicopter->rpb.ugr_4uk.magnetic) - Helicopter->rpb.ugr_4uk.dir() + 360) % 360)) {
						Helicopter->lpb.ugr_4uk.set_dir(Helicopter->rpb.ugr_4uk.dir() - 0.2);
						Helicopter->rpb.ugr_4uk.set_dir(Helicopter->rpb.ugr_4uk.dir() - 0.2);
					}
					else {
						Helicopter->lpb.ugr_4uk.set_dir(Helicopter->rpb.ugr_4uk.dir() + 0.2);
						Helicopter->rpb.ugr_4uk.set_dir(Helicopter->rpb.ugr_4uk.dir() + 0.2);
					}
				}
			}
			else if (Helicopter->rsp_elp.t_zk_r) {
				Helicopter->rpb.ugr_4uk.magnetic -= 0.2f;
				if (abs(Helicopter->rpb.ugr_4uk.dir() - (360.f - FPar->Yaw * Ra - Helicopter->rpb.ugr_4uk.magnetic)) > 1.f) {
					if ((static_cast<quint16>(Helicopter->rpb.ugr_4uk.dir() - (360.f - FPar->Yaw * Ra - Helicopter->rpb.ugr_4uk.magnetic) + 360) % 360) <= (static_cast<quint16>((360.f - FPar->Yaw * Ra - Helicopter->rpb.ugr_4uk.magnetic) - Helicopter->rpb.ugr_4uk.dir() + 360) % 360)) {
						Helicopter->lpb.ugr_4uk.set_dir(Helicopter->rpb.ugr_4uk.dir() - 0.2);
						Helicopter->rpb.ugr_4uk.set_dir(Helicopter->rpb.ugr_4uk.dir() - 0.2);
					}
					else {
						Helicopter->lpb.ugr_4uk.set_dir(Helicopter->rpb.ugr_4uk.dir() + 0.2);
						Helicopter->rpb.ugr_4uk.set_dir(Helicopter->rpb.ugr_4uk.dir() + 0.2);
					}
				}
			}
			else {
				if (abs(Helicopter->rpb.ugr_4uk.dir() - (360.f - FPar->Yaw * Ra - Helicopter->rpb.ugr_4uk.magnetic)) > 1.5f) {
					if ((static_cast<quint16>(Helicopter->rpb.ugr_4uk.dir() - (360.f - FPar->Yaw * Ra - Helicopter->rpb.ugr_4uk.magnetic) + 360) % 360) <= (static_cast<quint16>((360.f - FPar->Yaw * Ra - Helicopter->rpb.ugr_4uk.magnetic) - Helicopter->rpb.ugr_4uk.dir() + 360) % 360)) {
						Helicopter->lpb.ugr_4uk.set_dir(Helicopter->rpb.ugr_4uk.dir() - 1.0f);
						Helicopter->rpb.ugr_4uk.set_dir(Helicopter->rpb.ugr_4uk.dir() - 1.0f);
					}
					else {
						Helicopter->lpb.ugr_4uk.set_dir(Helicopter->rpb.ugr_4uk.dir() + 1.0f);
						Helicopter->rpb.ugr_4uk.set_dir(Helicopter->rpb.ugr_4uk.dir() + 1.0f);
					}
				}
			}
		}
		else if (Helicopter->rsp_elp.t_mk) {
			if (Helicopter->rsp_elp.t_zk_l) {
				Helicopter->lpb.ugr_4uk.set_dir(Helicopter->lpb.ugr_4uk.dir() - 1.0f);
				Helicopter->rpb.ugr_4uk.set_dir(Helicopter->rpb.ugr_4uk.dir() - 1.0f);
			}
			else if (Helicopter->rsp_elp.t_zk_r) {
				Helicopter->lpb.ugr_4uk.set_dir(Helicopter->lpb.ugr_4uk.dir() + 1.0f);
				Helicopter->rpb.ugr_4uk.set_dir(Helicopter->rpb.ugr_4uk.dir() + 1.0f);
			}
			else {
				Helicopter->rpb.ugr_4uk.magnetic = 10.0f;
				if (abs(Helicopter->rpb.ugr_4uk.dir() - (360.f - FPar->Yaw * Ra - Helicopter->rpb.ugr_4uk.magnetic)) > 1.5f) {
					if ((static_cast<quint16>(Helicopter->rpb.ugr_4uk.dir() - (360.f - FPar->Yaw * Ra - Helicopter->rpb.ugr_4uk.magnetic) + 360) % 360) <= (static_cast<quint16>((360.f - FPar->Yaw * Ra - Helicopter->rpb.ugr_4uk.magnetic) - Helicopter->rpb.ugr_4uk.dir() + 360) % 360)) {
						Helicopter->lpb.ugr_4uk.set_dir(Helicopter->lpb.ugr_4uk.dir() - 1.0f);
						Helicopter->rpb.ugr_4uk.set_dir(Helicopter->rpb.ugr_4uk.dir() - 1.0f);
					}
					else {
						Helicopter->lpb.ugr_4uk.set_dir(Helicopter->lpb.ugr_4uk.dir() + 1.0f);
						Helicopter->rpb.ugr_4uk.set_dir(Helicopter->rpb.ugr_4uk.dir() + 1.0f);
					}
				}
			}
		}
	}
	else if (Helicopter->Is115V() && Helicopter->rsh_elp.t_gmk) {
		Helicopter->lpb.ugr_4uk.set_dir(360.f - FPar->Yaw * Ra - Helicopter->rpb.ugr_4uk.magnetic);
		Helicopter->rpb.ugr_4uk.set_dir(360.f - FPar->Yaw * Ra - Helicopter->rpb.ugr_4uk.magnetic);
	}
}

void Helicopter_Onboard::CalcVOR( )
{
	//	auto FPar = Helicopter->Model_->GetFPar();
	Helicopter->lpb.ki_206.Calc( Helicopter->_BoardSystem->Latitude, Helicopter->_BoardSystem->Longitude );
	Helicopter->rpb.ki_206.Calc( Helicopter->_BoardSystem->Latitude, Helicopter->_BoardSystem->Longitude );
}

void Helicopter_Onboard::CalcCargo( )
{
	if (Helicopter->PC.t_avar_drop || Helicopter->PC.t_tact_drop)
	{
		auto IPar = Helicopter->Model_->GetIPar( );
		//IPar->Par064 = 1;
		IPar->Par065 = 1;
		//IPar->Par066 = 1;
		IPar->Par067 = 0;
		IPar->Par068 = 0;
		emit CargoDrop( );
		qDebug( ) << "Сброс подвески";
	}
}

void Helicopter_Onboard::CalcKO50( )
{
	// для запуска если ЕЩЕ НЕ включен, включен АЗС КО50 и переключатель в положении авто или ручной
	if ( !( Helicopter->rsp_elp.i_ko50_heater || Helicopter->rsp_elp.i_ko50_ignition || Helicopter->rsp_elp.i_ko50_work ) && 
		Helicopter->rp_azs.t_ko_50 &&  Helicopter->rsp_elp.t_ko50_ta_start && ( Helicopter->rsp_elp.t_ko50_auto || Helicopter->rsp_elp.t_ko50_man ) )
	{ 
		qDebug( ) << "Запуск КО-50";
		Helicopter->rsp_elp.i_ko50_heater = true;
		KO_start = std::chrono::steady_clock::now( );
	}
	// выключается если УЖЕ работает и отключен АЗС или режим в положении нетраль, не авто и не ручной.
	if ( ( Helicopter->rsp_elp.i_ko50_heater || Helicopter->rsp_elp.i_ko50_ignition || Helicopter->rsp_elp.i_ko50_work ) && 
		!( Helicopter->rsp_elp.t_ko50_auto || Helicopter->rsp_elp.t_ko50_man ) || !Helicopter->rp_azs.t_ko_50 )
	{
		//qDebug( ) << "Выключение КО-50";
		Helicopter->rsp_elp.i_ko50_work = false;
		Helicopter->rsp_elp.i_ko50_heater = false;
		Helicopter->rsp_elp.i_ko50_ignition = false;
	}
	if ( Helicopter->rsp_elp.i_ko50_heater || Helicopter->rsp_elp.i_ko50_ignition )
	{
		auto now = std::chrono::steady_clock::now( );
		// от момента запуска прошла минута
		if ( ( now - KO_start ) < std::chrono::minutes( 1 ) )
		{
			Helicopter->rsp_elp.i_ko50_heater = true;
		}
		// от момента запуска прошло 1,5 минуты
		else if ( ( now - KO_start ) < std::chrono::seconds( 90 ) )
		{
			Helicopter->rsp_elp.i_ko50_heater = false;
			Helicopter->rsp_elp.i_ko50_ignition = true;
			Helicopter->rsp_elp.i_ko50_work = true;
		}
		// от момента включения прошло больше 1,5 минут
		else
		{
			Helicopter->rsp_elp.i_ko50_heater = false;
			Helicopter->rsp_elp.i_ko50_ignition = false;
			Helicopter->rsp_elp.i_ko50_work = true;
		}
	}
	// если работает КО-50 то ткмпература в салоне равна выставленной на КО-50
	// TODO надо сделать плавное изменение температуры
	if ( Helicopter->rsp_elp.i_ko50_work )
	{
		//Helicopter->rpb.f_temp = Helicopter->rsp_elp.f_t_ko50;
		//Helicopter->rpb.f_temp = Helicopter->instructor->weather.temp;
		Helicopter->rpb.f_temp = Helicopter->_BoardSystem->Temp;
	}
	Helicopter->rpb.f_temp = Helicopter->_BoardSystem->Temp;
	if (Helicopter->instructor->fault.VU1 || Helicopter->instructor->fault.VU2)
	{
		Helicopter->rsp_elp.i_ko50_work = false;
		Helicopter->rsp_elp.i_ko50_heater = false;
		Helicopter->rsp_elp.i_ko50_ignition = false;
	}
}

void Helicopter_Onboard::CalcSPUU52( )
{
	if (Helicopter->instructor->fault.spuu) {
		Helicopter->ccp.i_spu52_off = true;
		Helicopter->ccp.f_spu52_ind = 1;
		return;
	}
	//Helicopter->ccp.f_spu52 = (Helicopter->_BoardSystem->enc_spuu52->pos(Helicopter->ccp.f_spu52) - 5.f) / 5.f;
	if (!Helicopter->ccp.t_spu52_p && !Helicopter->ccp.t_spu52_t) {
		Helicopter->ccp.f_spu52 = (Helicopter->ccp.f_spu52 - 5.f) / 5.f;
		Helicopter->ccp.f_spu52_pot.push_back(Helicopter->ccp.f_spu52);

		Helicopter->ccp.f_spu52_pot.pop_front();

		for (int i = 0; i < Helicopter->ccp.f_spu52_pot.size(); ++i) {
			Helicopter->ccp.f_spu52_ind += Helicopter->ccp.f_spu52_pot[i];
		}

		Helicopter->ccp.f_spu52_ind /= Helicopter->ccp.f_spu52_pot.size();
		Helicopter->ccp.f_spu52 = Helicopter->ccp.f_spu52_ind;
		// включение
		if (Helicopter->rp_azs.t_spuu_52)  // включен АЗС и выключен СПУУ горит индикатор
		{
			Helicopter->ccp.i_spu52_off = !Helicopter->lsh_elp.t_spuu_52 && Helicopter->_BoardSystem->Is27V();
			Helicopter->ccp.f_spu52_ind = 0; //TODO тут можно сделать имитацию работы СПУУ
		}
		if (Helicopter->ccp.i_spu52_off)
			Helicopter->ccp.f_spu52_ind = -1;
	}

	// тестирование 
	if (Helicopter->rp_azs.t_spuu_52 && (Helicopter->_BoardSystem->_Engine_1->is_working() || Helicopter->_BoardSystem->_Engine_2->is_working()) && Helicopter->lsh_elp.t_spuu_52 && (Helicopter->ccp.t_spu52_t || Helicopter->ccp.t_spu52_p))
	{
		// нажаты одновременно ОФФ и Р стрелку влево
		if ( Helicopter->ccp.t_spu52_p && Helicopter->ccp.t_spu52_off)
		{
			Helicopter->ccp.f_spu52_ind = Helicopter->ccp.f_spu52_ind  - 0.01;
			if (Helicopter->ccp.f_spu52_ind < -1)
				Helicopter->ccp.f_spu52_ind = -1;
		}
		// нажаты одновременно ОФФ и t стрелку вправо
		else if ( Helicopter->ccp.t_spu52_t && Helicopter->ccp.t_spu52_off)
		{
			Helicopter->ccp.f_spu52_ind = Helicopter->ccp.f_spu52_ind + 0.01;
			if (Helicopter->ccp.f_spu52_ind > 1)
				Helicopter->ccp.f_spu52_ind = 1;
		}
	}
	// нажаты ОФФ и вращаем ручку стрелка движется за ручкой
	else if (Helicopter->rp_azs.t_spuu_52 && (Helicopter->_BoardSystem->_Engine_1->is_working() || Helicopter->_BoardSystem->_Engine_2->is_working()) && Helicopter->lsh_elp.t_spuu_52 && !Helicopter->ccp.t_spu52_t && !Helicopter->ccp.t_spu52_p)
	{
		if (!Helicopter->ccp.t_spu52_off) {
			Helicopter->ccp.i_spu52_off = false;
			Helicopter->ccp.f_spu52_ind = 0.;
		}
		if (Helicopter->ccp.t_spu52_off) {
			Helicopter->ccp.f_spu52_ind = Helicopter->ccp.f_spu52;
		}
	}
	else if (Helicopter->rp_azs.t_spuu_52 && (Helicopter->_BoardSystem->_Engine_1->is_working() || Helicopter->_BoardSystem->_Engine_2->is_working()) && !Helicopter->lsh_elp.t_spuu_52) {
		if (!Helicopter->ccp.t_spu52_off){
			Helicopter->ccp.f_spu52_ind = -1;
			Helicopter->ccp.i_spu52_off = !Helicopter->lsh_elp.t_spuu_52 && Helicopter->_BoardSystem->Is27V();
		}
	}
	else if(Helicopter->rp_azs.t_spuu_52 && Helicopter->lsh_elp.t_spuu_52 && Helicopter->_BoardSystem->Is27V())
		Helicopter->ccp.f_spu52_ind = Helicopter->ccp.f_spu52;
}

void Helicopter_Onboard::CalcPZU( )
{
	// сбрасываем таймер запуска ПЗУ пока он не включен
	if (Helicopter->rsp_ep.t_pzu_left && ( Helicopter->rsp_ep.flag_pzu_left != Helicopter->rsp_ep.t_pzu_left))
		PZUL_start = std::chrono::steady_clock::now( );
	if (Helicopter->rsp_ep.t_pzu_right && (Helicopter->rsp_ep.flag_pzu_right != Helicopter->rsp_ep.t_pzu_right))
		PZUR_start = std::chrono::steady_clock::now( );

	auto now = std::chrono::steady_clock::now( );
	// от момента запуска прошло 15 секунд
	Helicopter->rsp_ep.i_pzu_left_on = Helicopter->rsp_ep.i_pzu_right_on = false;
	if ( ( now - PZUL_start ) > std::chrono::seconds( 20 ) )
	{
		Helicopter->rsp_ep.i_pzu_left_on = Helicopter->rsp_ep.t_pzu_left && Helicopter->Is27V( );
	}

	if ( ( now - PZUR_start ) > std::chrono::seconds( 20 ) )
	{
		Helicopter->rsp_ep.i_pzu_right_on = Helicopter->rsp_ep.t_pzu_right && Helicopter->Is27V( );
	}
	Helicopter->rsp_ep.flag_pzu_left = Helicopter->rsp_ep.t_pzu_left;
	Helicopter->rsp_ep.flag_pzu_right = Helicopter->rsp_ep.t_pzu_right;
}

void Helicopter_Onboard::Calc()
{
	//	auto FPar = Helicopter->Model_->GetFPar();
}

void Helicopter_Onboard::CalcBUR() {
	Helicopter->ccp.i_otkaz_bur = false;
	Helicopter->ccp.i_otkaz_en = false;
	if (!Helicopter->_BoardSystem->Is27V() && !Helicopter->Is115V()) {
		Helicopter->ccp.led_bur = { 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12 };
		return;
	}
	if (!Helicopter->ccp.t_bur_on && !Helicopter->lsp_elp.t_bur && Helicopter->_BoardSystem->_Electrics->Hgeo < 3) {
		Helicopter->ccp.led_bur = { 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12 };
		return;
	}
	// Предполетная проверка и отображение
	if (Helicopter->lsp_elp.t_bur && (Helicopter->Is27V() || Helicopter->Is115V())) {
		Helicopter->ccp.i_otkaz_bur = false;
		Helicopter->ccp.i_otkaz_en = false;
		if (Helicopter->ccp.time_on > 3000 && Helicopter->ccp.time_on < 3010) {
			Helicopter->ccp.time_on = 15010;
			Helicopter->ccp.led_OD = 0;
		}
		else if (Helicopter->lsp_elp.t_bur && Helicopter->ccp.time_on <= 3000) {
			Helicopter->ccp.time_on += 1;
			Helicopter->ccp.led_left = 88888888; // "8.8.8.8.8.8"
			Helicopter->ccp.led_bur = { 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8 };
		}
		else if (Helicopter->lsp_elp.t_bur){
			Helicopter->ccp.led_bur = Helicopter->ccp.bur[Helicopter->ccp.led_OD];
			if (Helicopter->ccp.led_OD > 2)
				Helicopter->ccp.led_left = Helicopter->ccp.bur[Helicopter->ccp.led_OD][3] * 100 + Helicopter->ccp.bur[Helicopter->ccp.led_OD][4] * 10 + Helicopter->ccp.bur[Helicopter->ccp.led_OD][5];
			else if(Helicopter->ccp.led_OD == 2)
				Helicopter->ccp.led_left = Helicopter->ccp.bur[Helicopter->ccp.led_OD][2] * 1000 + Helicopter->ccp.bur[Helicopter->ccp.led_OD][3] * 100 + Helicopter->ccp.bur[Helicopter->ccp.led_OD][4] * 10 + Helicopter->ccp.bur[Helicopter->ccp.led_OD][5];
			else
				Helicopter->ccp.led_left = Helicopter->ccp.bur[Helicopter->ccp.led_OD][0] * 100000 + Helicopter->ccp.bur[Helicopter->ccp.led_OD][1] * 10000 + Helicopter->ccp.bur[Helicopter->ccp.led_OD][2] * 1000 + Helicopter->ccp.bur[Helicopter->ccp.led_OD][3] * 100 + Helicopter->ccp.bur[Helicopter->ccp.led_OD][4] * 10 + Helicopter->ccp.bur[Helicopter->ccp.led_OD][5];

		}
	}
	else if (Helicopter->ccp.time_on < 15000)
		return;
	
	// Логика
	if (Helicopter->ccp.t_od) {
		Helicopter->ccp.led_VVOD = 0;
		if (Helicopter->ccp.t_od != Helicopter->ccp.t_od_pred) {
			Helicopter->ccp.led_OD += 1;
			Helicopter->ccp.t_od_pred = true;
		}
		if (Helicopter->ccp.led_OD > 4)
			Helicopter->ccp.led_OD = 0;
	}
	if (Helicopter->ccp.t_cbros) {
		if (Helicopter->ccp.led_OD == 1)
			Helicopter->ccp.bur[Helicopter->ccp.led_OD] = { 0x0,0x1,0x0,0x1,0x0,0x2,0x1,0x0 };
		else if (Helicopter->ccp.led_OD == 2)
			Helicopter->ccp.bur[Helicopter->ccp.led_OD] = { 0x12,0x12,0x0,0x0,0x0,0x0,0x2,0x0 };
		else if (Helicopter->ccp.led_OD == 3) 
			Helicopter->ccp.bur[Helicopter->ccp.led_OD] = { 0x12,0x12,0x12,0x0,0x0,0x0,0x3,0x0 };
		else if ( Helicopter->ccp.led_OD == 4)
			Helicopter->ccp.bur[Helicopter->ccp.led_OD] = { 0x12,0x12,0x12,0x0,0x0,0x0,0x4,0x0 };
		else 
			Helicopter->ccp.bur[Helicopter->ccp.led_OD] = { 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0 };
	}
	if (!Helicopter->ccp.t_od)
		Helicopter->ccp.t_od_pred = false;

	if (Helicopter->ccp.t_vvod) {
		if (Helicopter->ccp.led_OD == 2) {
			if (Helicopter->ccp.led_VVOD < 2)
				Helicopter->ccp.led_VVOD = 2;
			if (Helicopter->ccp.t_vvod != Helicopter->ccp.t_vvod_pred) {
				Helicopter->ccp.t_vvod_pred = true;
				Helicopter->ccp.led_VVOD += 1;
				if (Helicopter->ccp.led_VVOD > 5)
					Helicopter->ccp.led_VVOD = 0;
				if (Helicopter->ccp.led_VVOD < 2)
					Helicopter->ccp.led_VVOD = 2;
			}
		}
		else if (Helicopter->ccp.led_OD == 3 || Helicopter->ccp.led_OD == 4) {
			if (Helicopter->ccp.led_VVOD < 3)
				Helicopter->ccp.led_VVOD = 3;
			if (Helicopter->ccp.t_vvod != Helicopter->ccp.t_vvod_pred) {
				Helicopter->ccp.t_vvod_pred = true;
				Helicopter->ccp.led_VVOD += 1;
				if (Helicopter->ccp.led_VVOD > 5)
					Helicopter->ccp.led_VVOD = 0;
				if (Helicopter->ccp.led_VVOD < 3)
					Helicopter->ccp.led_VVOD = 3;
			}
		}
		else {
			if (Helicopter->ccp.t_vvod != Helicopter->ccp.t_vvod_pred) {
				Helicopter->ccp.t_vvod_pred = true;
				Helicopter->ccp.led_VVOD += 1;
				if (Helicopter->ccp.led_VVOD > 5)
					Helicopter->ccp.led_VVOD = 0;
			}
		}
	}
	if (!Helicopter->ccp.t_vvod)
		Helicopter->ccp.t_vvod_pred = false;

	if (Helicopter->ccp.t_ust) {
		if (Helicopter->ccp.led_OD == 2) {
			if (Helicopter->ccp.led_VVOD < 2)
				Helicopter->ccp.led_VVOD = 2;
		}
		if (Helicopter->ccp.led_OD == 3 || Helicopter->ccp.led_OD == 4) {
			if (Helicopter->ccp.led_VVOD < 3)
				Helicopter->ccp.led_VVOD = 3;
		}
		if (Helicopter->ccp.t_ust != Helicopter->ccp.t_ust_pred) {
			Helicopter->ccp.t_ust_pred = true;
			Helicopter->ccp.led_UST += 1;
			if (Helicopter->ccp.led_VVOD == 0) {
				if (Helicopter->ccp.led_OD == 0) {
					if (Helicopter->ccp.led_UST > 2)
						Helicopter->ccp.led_UST = 0;
				}
			}
			if (Helicopter->ccp.led_VVOD == 1) {
				if (Helicopter->ccp.led_OD == 0) {
					if (Helicopter->ccp.bur[Helicopter->ccp.led_OD][0] == 2) {
						if (Helicopter->ccp.led_UST > 3)
							Helicopter->ccp.led_UST = 0;
					}
				}
				if (Helicopter->ccp.led_OD == 1) {
					if (Helicopter->ccp.bur[Helicopter->ccp.led_OD][0] == 3) {
						if (Helicopter->ccp.led_UST > 1)
							Helicopter->ccp.led_UST = 0;
					}
				}
			}
			if (Helicopter->ccp.led_VVOD == 2) {
				if (Helicopter->ccp.led_OD == 0) {
					if (Helicopter->ccp.led_UST > 5)
						Helicopter->ccp.led_UST = 0;
				}
			}
			if (Helicopter->ccp.led_VVOD == 3) {
				if (Helicopter->ccp.led_OD == 1) {
					if (Helicopter->ccp.bur[Helicopter->ccp.led_OD][2] == 1) {
						if (Helicopter->ccp.led_UST > 2)
							Helicopter->ccp.led_UST = 0;
					}
				}
			}
			if (Helicopter->ccp.led_VVOD == 4) {
				if (Helicopter->ccp.led_OD == 0) {
					if (Helicopter->ccp.led_UST > 5)
						Helicopter->ccp.led_UST = 0;
				}
			}
			if (Helicopter->ccp.led_VVOD == 0) {
				if (Helicopter->ccp.led_OD == 1) {
					if (Helicopter->ccp.led_UST > 3)
						Helicopter->ccp.led_UST = 0;
				}
			}
			if (Helicopter->ccp.led_VVOD == 2) {
				if (Helicopter->ccp.led_OD == 1) {
					if (Helicopter->ccp.led_UST > 1)
						Helicopter->ccp.led_UST = 0;
				}
			}
			if (Helicopter->ccp.led_UST > 9)
				Helicopter->ccp.led_UST = 0;
			Helicopter->ccp.bur[Helicopter->ccp.led_OD][Helicopter->ccp.led_VVOD] = 0x1 * Helicopter->ccp.led_UST;
			//Helicopter->ccp.bur[Helicopter->ccp.led_OD][Helicopter->ccp.led_VVOD] = Helicopter->ccp.led_UST;
		}
	}
	if (!Helicopter->ccp.t_ust)
		Helicopter->ccp.t_ust_pred = false;

	// Кнопка контроль при не срабатывание 
	if ((Helicopter->ccp.t_kontr_lamp || Helicopter->instructor->fault.BUR) && (Helicopter->Is27V() || Helicopter->Is115V())) {
		Helicopter->ccp.i_otkaz_bur = true;
		Helicopter->ccp.i_otkaz_en = true;
	}
	// отказ БУР
	if (Helicopter->ccp.i_otkaz_bur) {
		Helicopter->ccp.led_bur = { 0x13, 0x13, 0xf, 0x13, 0x13, 0x13, 0x08, 0x08 }; // "--F---08"
	}
	if (!Helicopter->lsp_elp.t_bur && Helicopter->ccp.t_bur_on && (Helicopter->Is27V() || Helicopter->Is115V())){
		Helicopter->ccp.i_otkaz_bur = true;
		Helicopter->ccp.led_bur = { 0x13, 0x13, 0xf, 0x13, 0x13, 0x13, 0x00, 0x08 }; // "--F---08"
	}
	if (Helicopter->_BoardSystem->_Electrics->Hgeo > 3) {
		if (Helicopter->ccp.t_kontr_lamp) {
			Helicopter->ccp.i_otkaz_bur = true;
			Helicopter->ccp.led_bur = { 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8 };
		}
	}
}

void Helicopter_Onboard::CalcOrlan() {
	if (!Helicopter->Is27V())
		Helicopter->lp_elp.freq = 0xffff;		//Радио Баклан LED
	else {/*
		Helicopter->lp_elp.f_radio_baklan20_1 = Helicopter->_BoardSystem->enc_orlan_1->pos(Helicopter->lp_elp.f_radio_baklan20_1);
		Helicopter->lp_elp.f_radio_baklan20_2 = Helicopter->_BoardSystem->enc_orlan_2->pos(Helicopter->lp_elp.f_radio_baklan20_2);
		Helicopter->lp_elp.left_MGc.push_back(Helicopter->lp_elp.f_radio_baklan20_1);
		Helicopter->lp_elp.right_KGc.push_back(Helicopter->lp_elp.f_radio_baklan20_2);
		Helicopter->lp_elp.left_MGc.pop_front();
		Helicopter->lp_elp.right_KGc.pop_front();
		for (int i = 0; i < Helicopter->lp_elp.right_KGc.size(); ++i) {
			Helicopter->lp_elp.left_freq += Helicopter->lp_elp.left_MGc[i];
			Helicopter->lp_elp.right_freq += Helicopter->lp_elp.right_KGc[i];
		}

		Helicopter->lp_elp.left_freq /= Helicopter->lp_elp.right_KGc.size();
		Helicopter->lp_elp.right_freq /= Helicopter->lp_elp.right_KGc.size();*/

		Helicopter->lp_elp.left_freq += Helicopter->lp_elp.e_radio_baklan20_1;
		if (Helicopter->lp_elp.left_freq > 136)
			Helicopter->lp_elp.left_freq = 118;
		if (Helicopter->lp_elp.left_freq < 118)
			Helicopter->lp_elp.left_freq = 136;
		Helicopter->lp_elp.right_freq += Helicopter->lp_elp.e_radio_baklan20_2;
		if (Helicopter->lp_elp.right_freq > 999)
			Helicopter->lp_elp.right_freq = 0;
		if (Helicopter->lp_elp.right_freq < 0)
			Helicopter->lp_elp.right_freq = 999;
		Helicopter->lp_elp.freq = (Helicopter->lp_elp.left_freq) * 100000 + Helicopter->lp_elp.right_freq * 100;
	}
}

void Helicopter_Onboard::CalcKT76C() {
	Helicopter->ccp.KT76_i_FL = false;
	Helicopter->ccp.KT76_i_ALT = false;
	Helicopter->ccp.KT76_i_ON = false;
	Helicopter->ccp.KT76_i_R = false;
	Helicopter->ccp.KT76_i_SBY = false;

	if ((Helicopter->rp_azs.t_resp_uvd1 || Helicopter->rp_azs.t_resp_uvd2) && Helicopter->lp_azs.t_uvd_count && !Helicopter->ccp.KT76_gal_OFF && Helicopter->Is27V()) {
		if (Helicopter->ccp.KT76_gal_TST)
			Helicopter->ccp.KT76_i_R = !Helicopter->ccp.KT76_i_R;
		if (Helicopter->ccp.KT76_gal_ON || Helicopter->ccp.KT76_gal_SBY) {
			Helicopter->ccp.KT76_i_R = false;
			if (Helicopter->ccp.KT76_t_0) {
				if (Helicopter->ccp.KT76_t_0 != Helicopter->ccp.KT76_t_0_pred) {
					Helicopter->ccp.KT76_BORT = Helicopter->ccp.KT76_BORT * 10 + 0;
					Helicopter->ccp.KT76_t_0_pred = true;
				}
			}
			if (!Helicopter->ccp.KT76_t_0)
				Helicopter->ccp.KT76_t_0_pred = false;
			if (Helicopter->ccp.KT76_t_1) {
				if (Helicopter->ccp.KT76_t_1 != Helicopter->ccp.KT76_t_1_pred) {
					Helicopter->ccp.KT76_BORT = Helicopter->ccp.KT76_BORT * 10 + 1;
					Helicopter->ccp.KT76_t_1_pred = true;
				}
			}
			if (!Helicopter->ccp.KT76_t_1)
				Helicopter->ccp.KT76_t_1_pred = false;
			if (Helicopter->ccp.KT76_t_2) {
				if (Helicopter->ccp.KT76_t_2 != Helicopter->ccp.KT76_t_2_pred) {
					Helicopter->ccp.KT76_BORT = Helicopter->ccp.KT76_BORT * 10 + 2;
					Helicopter->ccp.KT76_t_2_pred = true;
				}
			}
			if (!Helicopter->ccp.KT76_t_2)
				Helicopter->ccp.KT76_t_2_pred = false;
			if (Helicopter->ccp.KT76_t_3) {
				if (Helicopter->ccp.KT76_t_3 != Helicopter->ccp.KT76_t_3_pred) {
					Helicopter->ccp.KT76_BORT = Helicopter->ccp.KT76_BORT * 10 + 3;
					Helicopter->ccp.KT76_t_3_pred = true;
				}
			}
			if (!Helicopter->ccp.KT76_t_3)
				Helicopter->ccp.KT76_t_3_pred = false;
			if (Helicopter->ccp.KT76_t_4) {
				if (Helicopter->ccp.KT76_t_4 != Helicopter->ccp.KT76_t_4_pred) {
					Helicopter->ccp.KT76_BORT = Helicopter->ccp.KT76_BORT * 10 + 4;
					Helicopter->ccp.KT76_t_4_pred = true;
				}
			}
			if (!Helicopter->ccp.KT76_t_4)
				Helicopter->ccp.KT76_t_4_pred = false;
			if (Helicopter->ccp.KT76_t_5) {
				if (Helicopter->ccp.KT76_t_5 != Helicopter->ccp.KT76_t_5_pred) {
					Helicopter->ccp.KT76_BORT = Helicopter->ccp.KT76_BORT * 10 + 5;
					Helicopter->ccp.KT76_t_5_pred = true;
				}
			}
			if (!Helicopter->ccp.KT76_t_5)
				Helicopter->ccp.KT76_t_5_pred = false;
			if (Helicopter->ccp.KT76_t_6) {
				if (Helicopter->ccp.KT76_t_6 != Helicopter->ccp.KT76_t_6_pred) {
					Helicopter->ccp.KT76_BORT = Helicopter->ccp.KT76_BORT * 10 + 6;
					Helicopter->ccp.KT76_t_6_pred = true;
				}
			}
			if (!Helicopter->ccp.KT76_t_6)
				Helicopter->ccp.KT76_t_6_pred = false;
			if (Helicopter->ccp.KT76_t_7) {
				if (Helicopter->ccp.KT76_t_7 != Helicopter->ccp.KT76_t_7_pred) {
					Helicopter->ccp.KT76_BORT = Helicopter->ccp.KT76_BORT * 10 + 7;
					Helicopter->ccp.KT76_t_7_pred = true;
				}
			}
			if (!Helicopter->ccp.KT76_t_7)
				Helicopter->ccp.KT76_t_7_pred = false;
			if (Helicopter->ccp.KT76_BORT > 9999)
				Helicopter->ccp.KT76_BORT = Helicopter->ccp.KT76_BORT % 10000;

			Helicopter->ccp.KT76_LCD = Helicopter->ccp.KT76_BORT * 10;
		}
		Helicopter->ccp.KT76_i_ALT = Helicopter->ccp.KT76_gal_ALT;
		Helicopter->ccp.KT76_i_ON = Helicopter->ccp.KT76_gal_ON;
		Helicopter->ccp.KT76_i_SBY = Helicopter->ccp.KT76_gal_SBY;
		if (Helicopter->ccp.KT76_gal_ALT) {
			Helicopter->ccp.KT76_i_R = false;
			if (static_cast<qint32>(Helicopter->lpb.vd_10vk.get_alt()) > 699)
				Helicopter->ccp.KT76_LCD = 999 * 100000 + Helicopter->ccp.KT76_BORT * 10;
			else {
				qint32 alt = static_cast<qint32>(Helicopter->lpb.vd_10vk.get_alt() - 2.);
				if (alt < 0)
					alt = 0;
				Helicopter->ccp.KT76_LCD = (alt) * 100000 + Helicopter->ccp.KT76_BORT * 10;
			}
		}
		if (Helicopter->ccp.KT76_t_CLR) {
			Helicopter->ccp.KT76_BORT = 0;
		}
	}
	else
		Helicopter->ccp.KT76_LCD = 0xffff;
}
void Helicopter_Onboard::CalcKN53() {
	Helicopter->ccp.KN53_pozuv_110 = false;
	Helicopter->ccp.KN53_pozuv_111 = false;
	Helicopter->ccp.KN53_pozuv_113 = false;
	if ((Helicopter->rp_azs.t_nav1 || Helicopter->rp_azs.t_nav2) && Helicopter->lp_azs.t_sns_nav && Helicopter->ccp.KN53_on && Helicopter->Is27V()) {
		if (Helicopter->ccp.KN53_pozuv && (Helicopter->rpb.kdi_572.i_S < 10 || Helicopter->rpb.kdi_572.i_S_R < 10)) {
			if ((Helicopter->ccp.KN53_LCD_leftM == 10 && Helicopter->ccp.KN53_LCD_leftK == 50) || (Helicopter->ccp.KN53_LCD_rightM == 10 && Helicopter->ccp.KN53_LCD_rightK == 50))
				Helicopter->ccp.KN53_pozuv_110 = true;
			else if ((Helicopter->ccp.KN53_LCD_leftM == 11 && Helicopter->ccp.KN53_LCD_leftK == 70) || (Helicopter->ccp.KN53_LCD_rightM == 11 && Helicopter->ccp.KN53_LCD_rightK == 70))
				Helicopter->ccp.KN53_pozuv_111 = true;
			else if ((Helicopter->ccp.KN53_LCD_leftM == 13 && Helicopter->ccp.KN53_LCD_leftK == 40) || (Helicopter->ccp.KN53_LCD_rightM == 13 && Helicopter->ccp.KN53_LCD_rightK == 40))
				Helicopter->ccp.KN53_pozuv_113 = true;
		}
		if (Helicopter->ccp.KN53_str) {     // кнопка стрелки
			Helicopter->ccp.KN53_str_freq += 1;
			if (Helicopter->ccp.KN53_str_freq > 2)
				Helicopter->ccp.KN53_str_freq = 1;
		}
		if (Helicopter->ccp.KN53_str) {
			if (Helicopter->ccp.KN53_str != Helicopter->ccp.KN53_str_pred) {
				Helicopter->ccp.KN53_str_freq += 1;
				if (Helicopter->ccp.KN53_str_freq > 2)
					Helicopter->ccp.KN53_str_freq = 1;
				Helicopter->ccp.KN53_str_pred = true;
			}
		}
		if (!Helicopter->ccp.KN53_str)
			Helicopter->ccp.KN53_str_pred = false;
		if (Helicopter->ccp.KN53_str_freq == 1) {
			Helicopter->ccp.KN53_LCD_leftM += 0 - Helicopter->ccp.KN53_freqM;
			Helicopter->ccp.KN53_LCD_leftK += Helicopter->ccp.KN53_freqK;
			if (Helicopter->ccp.KN53_LCD_leftM > 17)
				Helicopter->ccp.KN53_LCD_leftM = 8;
			if (Helicopter->ccp.KN53_LCD_leftM < 8)
				Helicopter->ccp.KN53_LCD_leftM = 17;
			if (Helicopter->ccp.KN53_LCD_leftK > 95)
				Helicopter->ccp.KN53_LCD_leftK = 0;
			if (Helicopter->ccp.KN53_LCD_leftK < 0)
				Helicopter->ccp.KN53_LCD_leftK = 95;
		}
		if (Helicopter->ccp.KN53_str_freq == 2) {
			Helicopter->ccp.KN53_LCD_rightM += 0 - Helicopter->ccp.KN53_freqM;
			Helicopter->ccp.KN53_LCD_rightK += Helicopter->ccp.KN53_freqK;
			if (Helicopter->ccp.KN53_LCD_rightM > 11)
				Helicopter->ccp.KN53_LCD_rightM = 8;
			if (Helicopter->ccp.KN53_LCD_rightM < 8)
				Helicopter->ccp.KN53_LCD_rightM = 11;
			if (Helicopter->ccp.KN53_LCD_rightK > 95)
				Helicopter->ccp.KN53_LCD_rightK = 0;
			if (Helicopter->ccp.KN53_LCD_rightK < 0)
				Helicopter->ccp.KN53_LCD_rightK = 95;
		}
		Helicopter->ccp.KN53_LCD = Helicopter->ccp.KN53_LCD_leftM * 1000000 + Helicopter->ccp.KN53_LCD_leftK * 10000 + Helicopter->ccp.KN53_LCD_rightM * 100 + Helicopter->ccp.KN53_LCD_rightK;
		Helicopter->ccp.KN53_LCD_16 = 11000000;
		//float freqKN_second = 100.f + static_cast<float>(Helicopter->ccp.KN53_LCD_leftM) + static_cast<float>(Helicopter->ccp.KN53_LCD_leftK) / 100;
		//Helicopter->lpb.ki_206.freq(freqKN_second);
		//Helicopter->lpb.ki_206.freq(freqKN_second);
	}
	else {
		Helicopter->ccp.KN53_LCD = 0xffff;
		Helicopter->ccp.KN53_LCD_16 = 0xffff;
	}
}

void Helicopter_Onboard::CalcKDI() {
	// ЛЭД покаывает растояние скорость и время до маяка если маяк не найден то везде тирэ
		// Установка начальных значений
	Helicopter->rpb.kdi_572.sw_switch = 0;
	bool is_N1 = false, is_H = false, is_N2 = false;

	// Упрощение логики установки значений
	if (Helicopter->rpb.kdi_572.t_N1) {
		Helicopter->rpb.kdi_572.sw_switch = 1;
		is_N1 = true;
	}
	else if (Helicopter->rpb.kdi_572.t_HLD) {
		Helicopter->rpb.kdi_572.sw_switch = 2;
		is_H = true;
	}
	else if (Helicopter->rpb.kdi_572.t_N2) {
		Helicopter->rpb.kdi_572.sw_switch = 3;
		is_N2 = true;
	}
	// Переназначение значений
	Helicopter->rpb.kdi_572.i_N1 = is_N1;
	Helicopter->rpb.kdi_572.i_H = is_H;
	Helicopter->rpb.kdi_572.i_N2 = is_N2;

	// Ранний выход из функции
	if (Helicopter->rpb.kdi_572.sw_switch == 0 || !Helicopter->Is27V() || (!Helicopter->rp_azs.t_distmeter || !Helicopter->lp_azs.t_distmeter || (/*!Helicopter->rpb.t_dnmr && */!Helicopter->lpb.t_dnmr))) {
		Helicopter->rpb.kdi_572.led = { 0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12 };
		Helicopter->rpb.kdi_572.i_N1 = false;
		Helicopter->rpb.kdi_572.i_H =	false;
		Helicopter->rpb.kdi_572.i_N2 = false;
		return;
	}

	if (!Helicopter->rp_azs.t_distmeter || !Helicopter->lp_azs.t_distmeter || (/*!Helicopter->rpb.t_dnmr && */!Helicopter->lpb.t_dnmr)) {
		Helicopter->rpb.kdi_572.led = { 0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13 };
		return;
	}
	//Вывод скорости
	Helicopter->rpb.kdi_572.i_Speed = static_cast<quint32>(Helicopter->instructor->Avia.Speed / 1.852);
	Helicopter->rpb.kdi_572.led[3] = 0x1 * (Helicopter->rpb.kdi_572.i_Speed / 100);
	Helicopter->rpb.kdi_572.led[4] = 0x1 * (Helicopter->rpb.kdi_572.i_Speed % 100 / 10);
	Helicopter->rpb.kdi_572.led[5] = 0x1 * (Helicopter->rpb.kdi_572.i_Speed % 10);

	float freqKN_reserv = 100.f + static_cast<float>(Helicopter->ccp.KN53_LCD_rightM) + static_cast<float>(Helicopter->ccp.KN53_LCD_rightK) / 100;
	float freqKN_second = 100.f + static_cast<float>(Helicopter->ccp.KN53_LCD_leftM) + static_cast<float>(Helicopter->ccp.KN53_LCD_leftK) / 100.00f;
	if (freqKN_reserv != Helicopter->rpb.kdi_572.freq_reserv || freqKN_second != Helicopter->rpb.kdi_572.freq_ready) {
		Helicopter->rpb.kdi_572.freq_reserv = freqKN_reserv;
		Helicopter->rpb.kdi_572.freq_ready = freqKN_second;
		Helicopter->rpb.kdi_572.freq_ok = false;
	}
	if (!Helicopter->rpb.kdi_572.freq_ok)
	{
		if (Helicopter->Is115V())
			Helicopter->rpb.kdi_572.led = { 0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13 };
		Helicopter->lpb.ki_206.freq(freqKN_second);
		Helicopter->rpb.ki_206.freq(freqKN_second);
		if (Helicopter->rpb.kdi_572.t_HLD && !Helicopter->rpb.kdi_572.freq_HLD_flag) {
			Helicopter->rpb.kdi_572.freq_HLD = freqKN_second;
		}
		auto it = std::find_if(Helicopter->rpb.ki_206.vs.begin(), Helicopter->rpb.ki_206.vs.end(), [this, &freqKN_second](auto& vor) { return std::abs(freqKN_second - vor.freq) < 0.005; });//Helicopter->rpb.ki_206.find();
		if (it == Helicopter->rpb.ki_206.vs.end())
		{
			Helicopter->lpb.ki_206.freq(freqKN_reserv);
			Helicopter->rpb.ki_206.freq(freqKN_reserv);
			if (Helicopter->rpb.kdi_572.t_HLD && !Helicopter->rpb.kdi_572.freq_HLD_flag) {
				Helicopter->rpb.kdi_572.freq_HLD = freqKN_reserv;
			}
			it = std::find_if(Helicopter->rpb.ki_206.vs.begin(), Helicopter->rpb.ki_206.vs.end(), [this, &freqKN_reserv](auto& vor) { return std::abs(freqKN_reserv - vor.freq) < 0.005; });//Helicopter->rpb.ki_206.find();
			if (it == Helicopter->rpb.ki_206.vs.end()) {
				Helicopter->rpb.kdi_572.freq_ok = false;
				Helicopter->rpb.kdi_572.led = { 0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13 };
			}
			else {
				Helicopter->rpb.kdi_572.freq_reserv = freqKN_reserv;
				Helicopter->rpb.kdi_572.freq_ok = true;
				Helicopter->rpb.kdi_572.i_S_R = static_cast<quint32>(Helicopter->_BoardSystem->CalcDistance(Helicopter->_BoardSystem->Latitude, Helicopter->_BoardSystem->Longitude, Helicopter->_BoardSystem->_Electrics->Hgeo, it->pos.latitude, it->pos.longitude) * 10);
			}
		}
		else {
			Helicopter->rpb.kdi_572.i_S = static_cast<quint32>(Helicopter->_BoardSystem->CalcDistance(Helicopter->_BoardSystem->Latitude, Helicopter->_BoardSystem->Longitude, Helicopter->_BoardSystem->_Electrics->Hgeo, it->pos.latitude, it->pos.longitude) * 10);
			Helicopter->rpb.kdi_572.freq_ready = freqKN_second;
			Helicopter->rpb.kdi_572.freq_ok = true;
		}
	}
	else {
		if (Helicopter->rpb.kdi_572.t_HLD) {
			if (!Helicopter->rpb.kdi_572.freq_HLD_flag)
				Helicopter->rpb.kdi_572.freq_HLD = Helicopter->rpb.kdi_572.freq_ready;
			Helicopter->lpb.ki_206.freq(Helicopter->rpb.kdi_572.freq_HLD);
			Helicopter->rpb.ki_206.freq(Helicopter->rpb.kdi_572.freq_HLD);
			Helicopter->rpb.kdi_572.freq_HLD_flag = true;
		}
		else if (Helicopter->rpb.kdi_572.t_N1) {
			Helicopter->rpb.kdi_572.freq_HLD_flag = false;
			Helicopter->lpb.ki_206.freq(Helicopter->rpb.kdi_572.freq_ready);
			Helicopter->rpb.ki_206.freq(Helicopter->rpb.kdi_572.freq_ready);
		}
		//Определяем расстояние до маяка
		if (Helicopter->rpb.kdi_572.i_S > 165) {
			Helicopter->rpb.kdi_572.led[0] = 0x13;
			Helicopter->rpb.kdi_572.led[1] = 0x13;
			Helicopter->rpb.kdi_572.led[2] = 0x13;
			Helicopter->lpb.ki_206.i_gs = true;
			Helicopter->lpb.ki_206.i_nav = true;
			Helicopter->lpb.ki_206.freq(Helicopter->rpb.kdi_572.freq_reserv);
			Helicopter->rpb.ki_206.freq(Helicopter->rpb.kdi_572.freq_reserv);
			if (Helicopter->rpb.kdi_572.i_S_R > 50) {
				Helicopter->rpb.kdi_572.led[0] = 0x13;
				Helicopter->rpb.kdi_572.led[1] = 0x13;
				Helicopter->rpb.kdi_572.led[2] = 0x13;
				Helicopter->lpb.ki_206.i_gs  = true;
				Helicopter->lpb.ki_206.i_nav = true;
			}
			else {
				auto it = std::find_if(Helicopter->rpb.ki_206.vs.begin(), Helicopter->rpb.ki_206.vs.end(), [this](auto& vor) { return std::abs(Helicopter->rpb.kdi_572.freq_reserv - vor.freq) < 0.005; });//Helicopter->rpb.ki_206.find();
				if (it == Helicopter->rpb.ki_206.vs.end())
					Helicopter->rpb.kdi_572.i_S_R = 170;
				else
					Helicopter->rpb.kdi_572.i_S_R = static_cast<quint32>(abs(Helicopter->_BoardSystem->CalcDistance(Helicopter->_BoardSystem->Latitude, Helicopter->_BoardSystem->Longitude, Helicopter->_BoardSystem->_Electrics->Hgeo, it->pos.latitude, it->pos.longitude)) * 10);// умножаем на 10 так как приходит десятичное вывод потом в int и делится на 10 для вывода десятых
				Helicopter->rpb.kdi_572.led[0] = 0x1 * (Helicopter->rpb.kdi_572.i_S_R / 100);
				Helicopter->rpb.kdi_572.led[1] = static_cast<BYTE>(30 + (Helicopter->rpb.kdi_572.i_S_R % 100 / 10));
				Helicopter->rpb.kdi_572.led[2] = 0x1 * (Helicopter->rpb.kdi_572.i_S_R % 10);
				Helicopter->lpb.ki_206.i_gs = false;
				Helicopter->lpb.ki_206.i_nav = false;
				Helicopter->lpb.ki_206.glis = Helicopter->_BoardSystem->CalcGlis(Helicopter->_BoardSystem->_Electrics->Hgeo, static_cast<float>(Helicopter->rpb.kdi_572.i_S_R) / 10.f * 1.852);
				Helicopter->rpb.ki_206.glis = Helicopter->_BoardSystem->CalcGlis(Helicopter->_BoardSystem->_Electrics->Hgeo, static_cast<float>(Helicopter->rpb.kdi_572.i_S_R) / 10.f * 1.852);
				//Определяем время									     возвращаем с десятыми													в минуты
				Helicopter->rpb.kdi_572.i_Time = (static_cast<float>(Helicopter->rpb.kdi_572.i_S_R) / 10.f) / static_cast<float>(Helicopter->rpb.kdi_572.i_Speed) * 60.f;
			}
		}
		else {
			auto it = std::find_if(Helicopter->rpb.ki_206.vs.begin(), Helicopter->rpb.ki_206.vs.end(), [this](auto& vor) { return std::abs(Helicopter->rpb.kdi_572.freq_ready - vor.freq) < 0.005; });//Helicopter->rpb.ki_206.find();
			if (it == Helicopter->rpb.ki_206.vs.end())
				Helicopter->rpb.kdi_572.i_S = 170;
			else
				Helicopter->rpb.kdi_572.i_S = static_cast<quint32>(abs(Helicopter->_BoardSystem->CalcDistance(Helicopter->_BoardSystem->Latitude, Helicopter->_BoardSystem->Longitude, Helicopter->_BoardSystem->_Electrics->Hgeo, it->pos.latitude, it->pos.longitude)) * 10); // умножаем на 10 так как приходит десятичное вывод потом в int и делится на 10 для вывода десятых
			Helicopter->rpb.kdi_572.led[0] = 0x1 * (Helicopter->rpb.kdi_572.i_S / 100);
			Helicopter->rpb.kdi_572.led[1] = static_cast<BYTE>(30 + (Helicopter->rpb.kdi_572.i_S % 100 / 10));
			Helicopter->rpb.kdi_572.led[2] = 0x1 * (Helicopter->rpb.kdi_572.i_S % 10);
			Helicopter->lpb.ki_206.i_gs = false;
			Helicopter->lpb.ki_206.i_nav = false;
			Helicopter->lpb.ki_206.glis = Helicopter->_BoardSystem->CalcGlis(Helicopter->_BoardSystem->_Electrics->Hgeo, static_cast<float>(Helicopter->rpb.kdi_572.i_S) / 10.f * 1.852);
			Helicopter->rpb.ki_206.glis = Helicopter->_BoardSystem->CalcGlis(Helicopter->_BoardSystem->_Electrics->Hgeo, static_cast<float>(Helicopter->rpb.kdi_572.i_S) / 10.f * 1.852);
			//Определяем время									     возвращаем с десятыми												в минуты
			Helicopter->rpb.kdi_572.i_Time = (static_cast<float>(Helicopter->rpb.kdi_572.i_S) / 10.f) / static_cast<float>(Helicopter->rpb.kdi_572.i_Speed) * 60.f;
		}
		if (Helicopter->rpb.kdi_572.i_Time > 99) {
			Helicopter->rpb.kdi_572.led[6] = 0x13;
			Helicopter->rpb.kdi_572.led[7] = 0x13;
		}
		else {
			Helicopter->rpb.kdi_572.led[6] = 0x1 * (Helicopter->rpb.kdi_572.i_Time / 10);
			Helicopter->rpb.kdi_572.led[7] = 0x1 * (Helicopter->rpb.kdi_572.i_Time % 10);
		}
	}
}

/*void Helicopter_Onboard::CalcPrima() {
	if (!Helicopter->rsh_elp.t_rs || Helicopter->rsp_ep.prima.on_prima || Helicopter->rsp_ep.prima.PrdU || !Helicopter->Is27V() || Helicopter->instructor->fault.radio) {
		Helicopter->rsp_ep.prima.led_up_left = { 0x12,0x12,0x12,0x12,0x12 }; //экран с первой строкой 12 - ти разрядных
		Helicopter->rsp_ep.prima.led_down_left = { 0x12,0x12,0x12,0x12,0x12 }; //экран со второй строкой 12-ти разрядных
		Helicopter->rsp_ep.prima.led_up_right = { 0x12,0x12,0x12,0x12,0x12,0x12,0x12 }; //экран с первой строкой 12 - ти разрядных
		Helicopter->rsp_ep.prima.led_down_right = { 0x12,0x12,0x12,0x12,0x12,0x12,0x12 }; //экран со второй строкой 12-ти разрядных
		return;
	}
	emit prima_signal();
}*/

void Helicopter_Onboard::PuskUV26Slot() {
	emit PuskUV26Signal(Helicopter->UV26.t_left, Helicopter->UV26.t_right, static_cast<size_t>(Helicopter->UV26.seria_start_), static_cast<size_t>(Helicopter->UV26.zalp_));
}

void Helicopter_Onboard::PuskUV26(bool left, bool right, size_t s, size_t z) {
	if (s > 0) {
		if (left) {
			Helicopter->UV26.left_zar -= z;
			Helicopter->_BoardSystem->thermal_trap_l = true;
			Helicopter->_BoardSystem->thermal_trap_r = false;
		}
		else if (right) {
			Helicopter->UV26.right_zar -= z;
			Helicopter->_BoardSystem->thermal_trap_r = true;
			Helicopter->_BoardSystem->thermal_trap_l = false;
		}
		else {
			Helicopter->UV26.right_zar -= (z / 2);
			Helicopter->UV26.left_zar -= (z / 2);
			Helicopter->_BoardSystem->thermal_trap_l = true;
			Helicopter->_BoardSystem->thermal_trap_r = true;
		}
		--s;
	}
	else {
		Helicopter->_BoardSystem->thermal_trap_l = false;
		Helicopter->_BoardSystem->thermal_trap_r = false;
	}
}

void Helicopter_Onboard::CalcUV26() {
	if (Helicopter->UV26.t_left && !Helicopter->UV26.t_prog) {       // кнопка выбора левого борта
		Helicopter->UV26.tablo = Helicopter->UV26.left_zar;
		Helicopter->UV26.i_left = true;         // индикатор левого борта
		Helicopter->UV26.i_right = false;        // индикатор правого борта
	}
	else if (Helicopter->UV26.t_right && !Helicopter->UV26.t_prog) {      // кнопка выбора правого борта
		Helicopter->UV26.tablo = Helicopter->UV26.right_zar;
		Helicopter->UV26.i_left = false;         // индикатор левого борта
		Helicopter->UV26.i_right = true;        // индикатор правого борта
	}
	else if (!Helicopter->UV26.t_prog) 
		Helicopter->UV26.tablo = Helicopter->UV26.right_zar + Helicopter->UV26.left_zar;
	else if (Helicopter->UV26.t_prog) {
		Helicopter->UV26.tablo = Helicopter->UV26.seria_ * 100 + Helicopter->UV26.zalp_ * 10 + Helicopter->UV26.interval_;
		if (Helicopter->UV26.t_seria) {
			if (Helicopter->UV26.t_seria != Helicopter->UV26.t_seria_pred) {
				Helicopter->UV26.t_seria_pred = true;
				Helicopter->UV26.seria_ += 1;
				if (Helicopter->UV26.seria_ > 8)
					Helicopter->UV26.seria_ = 0;
			}
		}
		if (!Helicopter->UV26.t_seria)
			Helicopter->UV26.t_seria_pred = false;

		if (Helicopter->UV26.t_zalp) {
			if (Helicopter->UV26.t_zalp != Helicopter->UV26.t_zalp_pred) {
				Helicopter->UV26.t_zalp_pred = true;
				Helicopter->UV26.zalp_ += 1;
				if (Helicopter->UV26.zalp_ > 8)
					Helicopter->UV26.zalp_ = 1;
			}
		}
		if (!Helicopter->UV26.t_zalp)
			Helicopter->UV26.t_zalp_pred = false;

		if (Helicopter->UV26.t_interval) {
			if (Helicopter->UV26.t_interval != Helicopter->UV26.t_interval_pred) {
				Helicopter->UV26.t_interval_pred = true;
				Helicopter->UV26.interval_ += 1;
				if (Helicopter->UV26.interval_ > 9)
					Helicopter->UV26.interval_ = 0;
			}
		}
		if (!Helicopter->UV26.t_interval)
			Helicopter->UV26.t_interval_pred = false;

		if (Helicopter->UV26.t_reset) {
			Helicopter->UV26.seria_ = 1;
			Helicopter->UV26.zalp_ = 1;
			Helicopter->UV26.interval_ = 0;
		}
	}
	if (Helicopter->UV26.t_pusk && !Helicopter->UV26.t_prog)
		Helicopter->UV26.t_pusk_pred = true;
	if (Helicopter->UV26.t_pusk_pred) {
		Helicopter->UV26.seria_start_ = Helicopter->UV26.seria_;
		if (Helicopter->UV26.interval_ == 7)
			UV26Timer->start(250);
		else if (Helicopter->UV26.interval_ == 9)
			UV26Timer->start(500);
		else
			UV26Timer->start(Helicopter->UV26.interval_ * 1000);
		Helicopter->UV26.t_pusk_pred = false;
	}
	if (Helicopter->UV26.t_stop) {
		Helicopter->UV26.t_pusk_pred = false;
		UV26Timer->stop();
	}	
}