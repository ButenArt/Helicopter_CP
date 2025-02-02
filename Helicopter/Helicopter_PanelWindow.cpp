#include "Helicopter_PanelWindow.h"
#include "ui_Helicopter_PanelWindow.h"
#include "Helicopter_core.h"
#include "Helicopter_Model.h"
#include "Helicopter_controls.h"
#include "autopilot.h"
#include "controlgrid.h"
#include <QDebug>

Helicopter_PanelWindow::Helicopter_PanelWindow( Helicopter_core* Helicopter, QWidget* parent )
	: QWidget( parent )
	, Helicopter( Helicopter )
	, ui( new Ui::Helicopter_PanelWindow )
{
	ui->setupUi( this );
	this->setWindowFlags( Qt::Window | Qt::WindowCloseButtonHint );
	this->setFixedSize( this->width( ), this->height( ) );
	this->setWindowTitle( "Pilot" );
	_ControlGrid = new ControlGrid( ui->widget );
	_ControlGrid->setFrame( 180 );

	PanelRadio_ = new Helicopter_PanelRadio(Helicopter, this);

	UpdateTimer = new QTimer( this );
	UpdateTimer->setInterval( 50 );
	connect( UpdateTimer, SIGNAL( timeout( ) ), this, SLOT( on_ParamsUpdate( ) ) );
	connect( ui->Pitch, SIGNAL( sliderReleased( ) ), this, SLOT( PitchSliderReleased( ) ) );
	connect( ui->Roll, SIGNAL( sliderReleased( ) ), this, SLOT( RollSliderReleased( ) ) );

	connect( ui->sbFPar, QOverload<int>::of( &QSpinBox::valueChanged ),
		[=]( int i ) 
		{
			assert( i > 0 );
			//ui->FPar->display( Helicopter->Model_->GetFPar()->operator[](i-1) );
			ui->lbFParamNotes->setText( FParam_notes[i-1] );
			ui->lbFParamNotes->adjustSize( );
		} );
	connect( ui->sbIPar, QOverload<int>::of( &QSpinBox::valueChanged ),
		[=]( int i ) 
		{
			ui->IPar->display( Helicopter->Model_->GetIPar()->operator[](i-1) );
		} );
	ui->FPar->display( Helicopter->Model_->GetFPar()->operator[]( ui->sbFPar->value( ) - 1 ) );
	ui->lbFParamNotes->setText( FParam_notes[ui->sbFPar->value( ) - 1] );
	ui->lbFParamNotes->adjustSize( );

}

Helicopter_PanelWindow::~Helicopter_PanelWindow( )
{
	delete _ControlGrid;
	delete ui;
}

void Helicopter_PanelWindow::closeEvent( QCloseEvent* event )
{
	event->ignore( );
	UpdateTimer->stop( );
	this->hide( );
}

void Helicopter_PanelWindow::showEvent(QShowEvent* /*event*/ )
{
	qDebug() << __FUNCTION__;
	//QWidget::showEvent(event);
	SetML2Window();

}

void Helicopter_PanelWindow::SetML2Window()
{
	ui->t_IV_500->setChecked(Helicopter->lsp_elp.t_IV_500);
	ui->p_Fosh->setValue(Helicopter->PC.f_Fosh);
	ui->p_korrector->setValue(Helicopter->PC.f_Ecor);

	//        ui->f_barometr->setValue(Helicopter_->lpb.vd_10vk.p);

	ui->p_break->setValue(Helicopter->PC.f_break);
	ui->pbAPOff->setDown(Helicopter->PC.t_left_ap_off);// || Helicopter_->PC.t_rght_ap_off);

	ui->cb_1->setChecked(Helicopter->rp_azs.t_ta_start);
	ui->cb_2->setChecked(Helicopter->rp_azs.t_ta_ign);
	ui->cb_3->setChecked(Helicopter->rp_azs.t_eng_start);
	ui->cb_4->setChecked(Helicopter->rp_azs.t_eng_ign);
	ui->cb_5->setChecked(Helicopter->rp_azs.t_fuel_gauge);
	ui->cb_6->setChecked(Helicopter->rp_azs.t_pump_consum);
	ui->cb_7->setChecked(Helicopter->rp_azs.t_pump_left);
	ui->cb_8->setChecked(Helicopter->rp_azs.t_pump_right);
	ui->cb_9->setChecked(Helicopter->rp_azs.t_sas);
	ui->cb_10->setChecked(Helicopter->rp_azs.t_auto_main);
	ui->cb_11->setChecked(Helicopter->rp_azs.t_auto_fric);
	ui->cb_12->setChecked(Helicopter->rp_azs.t_auto_emufta);
	ui->cb_13->setChecked(Helicopter->rp_azs.t_hidro_main);
	ui->cb_14->setChecked(Helicopter->rp_azs.t_hidro_sec);
	ui->cb_15->setChecked(Helicopter->rp_azs.t_apparel);
	ui->cb_16->setChecked(Helicopter->rp_azs.t_turn_signal);
	ui->cb_17->setChecked(Helicopter->rp_azs.t_spuu_52);
	ui->cb_18->setChecked(Helicopter->rp_azs.t_rentgen);
	ui->cb_19->setChecked(Helicopter->rp_azs.t_ctrl_start);
	ui->cb_20->setChecked(Helicopter->rp_azs.t_close_left);
	ui->cb_21->setChecked(Helicopter->rp_azs.t_close_rght);
	ui->cb_22->setChecked(Helicopter->rp_azs.t_close_bypass);
	ui->cb_23->setChecked(Helicopter->rp_azs.t_ppo_signal);
	ui->cb_24->setChecked(Helicopter->rp_azs.t_ppo_1_auto);
	ui->cb_25->setChecked(Helicopter->rp_azs.t_ppo_1_hand);
	ui->cb_26->setChecked(Helicopter->rp_azs.t_ppo_2_auto);
	ui->cb_27->setChecked(Helicopter->rp_azs.t_ppo_2_hand);
	ui->cb_28->setChecked(Helicopter->rp_azs.t_r_spu);
	ui->cb_29->setChecked(Helicopter->rp_azs.t_r_altimetr);
	ui->cb_30->setChecked(Helicopter->rp_azs.t_r_comand_osnv);
	//ui->cb_31->setChecked(Helicopter->rp_azs.t_r_ils);
	ui->cb_32->setChecked(Helicopter->rp_azs.t_r_empty);
	ui->cb_33->setChecked(Helicopter->rp_azs.t_r_compas_sv);
	ui->cb_34->setChecked(Helicopter->rp_azs.t_r_compas_ukv);
	ui->cb_35->setChecked(Helicopter->rp_azs.t_pust_01);
	ui->cb_36->setChecked(Helicopter->rp_azs.t_r_rls);
	ui->cb_37->setChecked(Helicopter->rp_azs.t_far_left_ctrl);
	ui->cb_38->setChecked(Helicopter->rp_azs.t_far_left_flash);
	ui->cb_39->setChecked(Helicopter->rp_azs.t_far_right_ctrl);
	ui->cb_40->setChecked(Helicopter->rp_azs.t_far_right_flash);
	ui->cb_41->setChecked(Helicopter->rp_azs.t_far_empty);
	ui->cb_42->setChecked(Helicopter->rp_azs.t_strov_light);
	ui->cb_43->setChecked(Helicopter->rp_azs.t_ano);
	ui->cb_44->setChecked(Helicopter->rp_azs.t_cut_main_lock);
	ui->cb_45->setChecked(Helicopter->rp_azs.t_cut_sec_lock);
	ui->cb_46->setChecked(Helicopter->rp_azs.t_ais_ctrl);
	ui->cb_47->setChecked(Helicopter->rp_azs.t_ais_pzu_left);
	ui->cb_48->setChecked(Helicopter->rp_azs.t_ais_pzu_rght);
	ui->cb_49->setChecked(Helicopter->rp_azs.t_ais_co_121);
	ui->cb_50->setChecked(Helicopter->rp_azs.t_ais_glass);
	ui->cb_51->setChecked(Helicopter->rp_azs.t_ais_wipe_left);
	ui->cb_52->setChecked(Helicopter->rp_azs.t_ais_wipe_rght);
	ui->cb_53->setChecked(Helicopter->rp_azs.t_l166_81a);
	ui->cb_54->setChecked(Helicopter->rp_azs.t_ko_50);

	// электрик пульт
	ui->t_akkum_1->setChecked(Helicopter->ep_elp.t_akkum_1);
	ui->t_akkum_2->setChecked(Helicopter->ep_elp.t_akkum_2);
	ui->t_gen_1->setChecked(Helicopter->ep_elp.t_gen_1);
	ui->t_gen_2->setChecked(Helicopter->ep_elp.t_gen_2);
	ui->t_res_gen->setChecked(Helicopter->ep_elp.t_res_gen );
	ui->t_ACDC_1->setChecked(Helicopter->ep_elp.t_ACDC_1);
	ui->t_ACDC_2->setChecked(Helicopter->ep_elp.t_ACDC_2);
	ui->t_test_sys->setChecked(Helicopter->ep_elp.t_test_sys);
	ui->t_aero_pit_dc->setChecked(Helicopter->ep_elp.t_aero_pit_dc );

	ui->hs_1->setValue(Helicopter->ep_elp.sw_ctrl);
	ui->hs_2->setValue(Helicopter->ep_elp.sw_dc_volt_mode);
	ui->hs_3->setValue(Helicopter->ep_elp.sw_ac_volt_mode);

	if (Helicopter->ep_elp.t_r36_man)
		ui->vs_1->setValue(0);
	else if (Helicopter->ep_elp.t_r36_auto)
		ui->vs_1->setValue(2);
	else
		ui->vs_1->setValue(1);

	if (Helicopter->ep_elp.t_pts_man)
		ui->vs_4->setValue(0);
	else if (Helicopter->ep_elp.t_pts_auto)
		ui->vs_4->setValue(2);
	else
		ui->vs_4->setValue(1);

	ui->vs_2->setValue( Helicopter->ep_elp.t_gen );
	
	// тумблеры гидросистемы
	ui->t_hidro_main->setChecked(Helicopter->cep.t_hidro_main);
	ui->t_hidro_second->setChecked(Helicopter->cep.t_hidro_second);
	// тумблеры управления топливными кранами 
	ui->t_fuel_left_close->setChecked(Helicopter->cep.t_fuel_left_close);
	ui->t_fuel_right_close->setChecked(Helicopter->cep.t_fuel_right_close);
	ui->t_fuel_bypass_close->setChecked(Helicopter->cep.t_fuel_bypass_close);
	ui->t_fuel_perepusk->setChecked(Helicopter->cep.t_fuel_perepusk);
	//тумблеры управления топливными насосами
	ui->t_fpump_consum->setChecked(Helicopter->cep.t_fpump_consum);
	ui->t_fpump_left->setChecked(Helicopter->cep.t_fpump_left);
	ui->t_fpump_right->setChecked(Helicopter->cep.t_fpump_right);
	ui->t_eng_left_brake->setChecked(Helicopter->PC.t_eng_left_brake);
	ui->t_eng_rght_brake->setChecked(Helicopter->PC.t_eng_rght_brake);
	ui->t_NV_brake_on->setChecked(!Helicopter->PC.t_NV_brake_off);

	ui->horizontalSlider_3->setValue(Helicopter->cep.t_contr_sensors ? 0 : 1);
	// пиропатроны I и II
	ui->horizontalSlider_4->setValue(Helicopter->cep.t_1 ? 0 : 1);
	// выбор режим запуска ТА (ложный, прокрутка, запуск)
	if (Helicopter->cep.t_ta_sel_false_start) ui->horizontalSlider_5->setValue(0);
	if (Helicopter->cep.t_ta_sel_scroll) ui->horizontalSlider_5->setValue(1);
	if (Helicopter->cep.t_ta_sel_start) ui->horizontalSlider_5->setValue(2);

	// выбор режима запуска двигателя (прокрутка, запуск)
	if (Helicopter->cep.t_eng_sel_scroll) ui->horizontalSlider_6->setValue(0);
	else if (Helicopter->cep.t_eng_sel_start) ui->horizontalSlider_6->setValue(2);
	else  ui->horizontalSlider_6->setValue(1);

	// выбор двигателя для запуска
	if (Helicopter->cep.t_eng_left) ui->horizontalSlider_7->setValue(0);
	else if (Helicopter->cep.t_eng_rght) ui->horizontalSlider_7->setValue(2);
	else  ui->horizontalSlider_7->setValue(1);

	// проверка зажигания
	if (Helicopter->cep.t_eng_left_ign_check) ui->horizontalSlider_8->setValue(0);
	else if (Helicopter->cep.t_eng_right_ign_check) ui->horizontalSlider_8->setValue(2);
	else  ui->horizontalSlider_8->setValue(1);

	ui->apk->setValue(Helicopter->lpb.t_apk_ukv);
	ui->max_roll->setValue(Helicopter->lpb.t_roll_33);
}

void Helicopter_PanelWindow::on_azs1_clicked( )
{
	ui->cb_1->setChecked( true );
	ui->cb_2->setChecked( true );
	ui->cb_3->setChecked( true );
	ui->cb_4->setChecked( true );
	ui->cb_5->setChecked( true );
	ui->cb_6->setChecked( true );
	ui->cb_7->setChecked( true );
	ui->cb_8->setChecked( true );
	ui->cb_9->setChecked( true );
}

void Helicopter_PanelWindow::on_azs2_clicked( )
{
	ui->cb_10->setChecked( true );
	ui->cb_11->setChecked( true );
	ui->cb_12->setChecked( true );
	ui->cb_13->setChecked( true );
	ui->cb_14->setChecked( true );
	ui->cb_15->setChecked( true );
	ui->cb_16->setChecked( true );
	ui->cb_17->setChecked( true );
	ui->cb_18->setChecked( true );
}

void Helicopter_PanelWindow::on_azs3_clicked( )
{
	ui->cb_19->setChecked( true );
	ui->cb_20->setChecked( true );
	ui->cb_21->setChecked( true );
	ui->cb_22->setChecked( true );
	ui->cb_23->setChecked( true );
	ui->cb_24->setChecked( true );
	ui->cb_25->setChecked( true );
	ui->cb_26->setChecked( true );
	ui->cb_27->setChecked( true );
}

void Helicopter_PanelWindow::on_azs4_clicked( )
{
	ui->cb_28->setChecked( true );
	ui->cb_29->setChecked( true );
	ui->cb_30->setChecked( true );
	//ui->cb_31->setChecked(true);
	ui->cb_32->setChecked( true );
	ui->cb_33->setChecked( true );
	ui->cb_34->setChecked( true );
	ui->cb_35->setChecked( true );
	ui->cb_36->setChecked( true );
}

void Helicopter_PanelWindow::on_azs5_clicked( )
{
	ui->cb_37->setChecked( true );
	ui->cb_38->setChecked( true );
	ui->cb_39->setChecked( true );
	ui->cb_40->setChecked( true );
	ui->cb_41->setChecked( true );
	ui->cb_42->setChecked( true );
	ui->cb_43->setChecked( true );
	ui->cb_44->setChecked( true );
	ui->cb_45->setChecked( true );
}

void Helicopter_PanelWindow::on_azs6_clicked( )
{
	ui->cb_46->setChecked( true );
	ui->cb_47->setChecked( true );
	ui->cb_48->setChecked( true );
	ui->cb_49->setChecked( true );
	ui->cb_50->setChecked( true );
	ui->cb_51->setChecked( true );
	ui->cb_52->setChecked( true );
	ui->cb_53->setChecked( true );
	ui->cb_54->setChecked( true );
}
void Helicopter_PanelWindow::on_bFire_pressed( )
{
	qDebug( ) << __FUNCTION__;
	Helicopter->PC.t_left_fire = true;
}

void Helicopter_PanelWindow::on_bFire_released( )
{
	qDebug( ) << __FUNCTION__;
	Helicopter->PC.t_left_fire = false;
}

void Helicopter_PanelWindow::SetState( bool flag, QWidget* widget, const QString style_true, const QString style_false )
{
	flag ? widget->setStyleSheet( style_true ) : widget->setStyleSheet( style_false );
}

void Helicopter_PanelWindow::keyPressEvent( QKeyEvent* event )
{
	int key = event->key( );

	if ( key == Qt::Key_S )
	{
		Helicopter->PC.f_pitch += 1;
	}
	if ( key == Qt::Key_W )
	{
		Helicopter->PC.f_pitch -= 1;
	}
	Helicopter->PC.f_pitch = qBound( 0.f, Helicopter->PC.f_pitch, 255.f );
	if ( key == Qt::Key_A )
	{
		Helicopter->PC.f_roll -= 1;
	}
	if ( key == Qt::Key_D )
	{
		Helicopter->PC.f_roll += 1;
	}
	Helicopter->PC.f_roll = qBound( 0.f, Helicopter->PC.f_roll, 255.f );
	if ( key == Qt::Key_Q )
	{
		Helicopter->PC.f_yawl -= 1;
	}
	if ( key == Qt::Key_E )
	{
		Helicopter->PC.f_yawl += 1;
	}
	if (key == Qt::Key_R)
	{
		PanelRadio_->UpdateTimer->start();
		PanelRadio_->show();
	}
	Helicopter->PC.f_yawl = qBound( 0.f, Helicopter->PC.f_yawl, 255.f );
}

void Helicopter_PanelWindow::on_ParamsUpdate( )
{
	auto FPar = Helicopter->Model_->GetFPar( );
	auto IPar = Helicopter->Model_->GetIPar( );
	if ( Helicopter->joystick_->JoystickActive( ) == false && Helicopter->controls_->ManualControl( ) ) // если нет джойстика и ручной режим
	{
		//управление
		Helicopter->PC.f_Fosh = ui->p_Fosh->value( );
		Helicopter->PC.f_Ecor = ui->p_korrector->value( );
		Helicopter->PC.f_break = ui->p_break->value( );
	} else // если имеется джойстик, заполняем данные с него
	{
		ui->p_Fosh->setValue( Helicopter->PC.f_Fosh );
		ui->p_korrector->setValue( Helicopter->PC.f_Ecor );
		ui->p_break->setValue( Helicopter->PC.f_break );
		ui->pbAPOff->setDown( Helicopter->PC.t_left_ap_off );
	}
#ifdef AP_AES
	_ControlGrid->setJoystick( Helicopter->PC.get_roll( ), Helicopter->PC.get_pitch( ), Helicopter->AP_->getRollCoords( ), Helicopter->AP_->getPitchCoords( ) );
#else
	_ControlGrid->setJoystick( Helicopter->PC.get_roll( ), Helicopter->PC.get_pitch( ), Helicopter->Model_->GetFPar( )->Par130, Helicopter->Model_->GetFPar( )->Par129 );
#endif
	_ControlGrid->setPedals( Helicopter->PC.get_yawl( ), Helicopter->Model_->GetFPar( )->Par195 );
	_ControlGrid->setGas( Helicopter->PC.f_Fosh, Helicopter->Model_->GetFPar( )->Par194 );
	// читаем все данные для окна, работает во всех режимах, индикация шкал и лампочек
	{
		ui->AP_pitch->setValue( static_cast<int>( Helicopter->ccp.f_get_pitch * 10 ) );
		ui->AP_roll->setValue( static_cast<int>( Helicopter->ccp.f_get_roll * 10 ) );
		ui->lcdAP_pitch->display( Helicopter->ccp.f_get_pitch );
		ui->lcdAP_roll->display( Helicopter->ccp.f_get_roll );
		ui->lcd_DC_Amper_1->display( Helicopter->ep_elp.f_amper_1 );
		ui->lcd_DC_Amper_2->display( Helicopter->ep_elp.f_amper_2 );
		ui->lcd_DC_Amper_3->display( Helicopter->ep_elp.f_amper_3 );
		ui->lcd_DC_Volt->display( Helicopter->ep_elp.f_volt );
		ui->lcd_AC_Amper_1->display( Helicopter->ep_elp.f_ac_amper_1 );
		ui->lcd_AC_Amper_2->display( Helicopter->ep_elp.f_ac_amper_3 );
		ui->lcd_AC_Volt->display( Helicopter->ep_elp.f_ac_volt );
		ui->lcd_speedometr->display( static_cast<int>( Helicopter->lpb.uc_450k.speed( ) ) );
		ui->lcd_altimetr->display( static_cast<int>( Helicopter->lpb.vd_10vk.get_alt( ) ) );
		ui->lcd_variometr->display( static_cast<int>( Helicopter->lpb.vr_30mk.var ) );
		ui->lcd_NV->display( static_cast<int>( Helicopter->lpb.ite_1t.nv ) );
		ui->lcd_eng_1->display( static_cast<int>( Helicopter->lpb.ite_2t.eng1 ) );
		ui->lcd_eng_2->display( static_cast<int>( Helicopter->lpb.ite_2t.eng2 ) );
		ui->lcd_fosh->display( static_cast<int>( Helicopter->lpb.up_21_15.fosh ) );
		ui->lcd_course->display( static_cast<int>( Helicopter->lpb.ugr_4uk.dir( ) ) );
		ui->lcd_temp_eng_1->display( static_cast<int>( Helicopter->lpb.ut_6k.temp1 ) );
		ui->lcd_temp_eng_2->display( static_cast<int>( Helicopter->lpb.ut_6k.temp2 ) );

		ui->lcd_break->display( static_cast<int>( Helicopter->PC.f_break ) );
		ui->lcd_Fosh->display( Helicopter->PC.f_Fosh );
		ui->lcd_korr->display( Helicopter->PC.f_Ecor );
		ui->lcd_pedali->display( Helicopter->PC.get_yawl( ) );
		ui->lcd_Y->display( Helicopter->PC.get_pitch( ) );
		ui->lcd_X->display( Helicopter->PC.get_roll( ) );
		ui->lcd_ark->display( Helicopter->lpb.ugr_4uk.ark() );
		ui->f_fuel->display( Helicopter->rpb.f_fuel );

		ui->f_p_oil_main_gb->display( static_cast<int>( Helicopter->ccp.f_p_oil_main_gb ) );
		ui->f_t_oil_inter_gb->display( static_cast<int>( Helicopter->ccp.f_t_oil_inter_gb ) );
		ui->f_t_oil_tail_gb->display( static_cast<int>( Helicopter->ccp.f_t_oil_tail_gb ) );
		ui->f_t_oil_main_gb->display( static_cast<int>( Helicopter->ccp.f_t_oil_main_gb ) );
		ui->f_p_oil_left_eng->display( static_cast<int>( Helicopter->ccp.f_p_oil_left_eng ) );
		ui->f_t_oil_left_eng->display( static_cast<int>( Helicopter->ccp.f_t_oil_left_eng ) );
		ui->f_p_oil_rght_eng->display( static_cast<int>( Helicopter->ccp.f_p_oil_rght_eng ) );
		ui->f_t_oil_rght_eng->display( static_cast<int>( Helicopter->ccp.f_t_oil_rght_eng ) );
		ui->f_p_hidro_main_2->display( static_cast<int>( Helicopter->cep.f_p_hidro_main ) );
		ui->f_p_hidro_second_2->display( static_cast<int>( Helicopter->cep.f_p_hidro_second ) );
		ui->f_p_ta_2->display( Helicopter->cep.f_p_ta );
		ui->f_t_ta_2->display( Helicopter->cep.f_t_ta );
		ui->f_p_air->display( Helicopter->lsp_elp.f_p_air );
		ui->f_p_brake->display( Helicopter->lsp_elp.f_p_brake );
		ui->f_amper->display( Helicopter->lp_elp.f_ampermetr );
		// вывод произвольного параметра из списка
		ui->FPar->display( FPar->operator[]( ui->sbFPar->value( ) - 1 ) );
		ui->IPar->display( IPar->operator[]( ui->sbIPar->value( ) - 1 ) );

		SetState( Helicopter->cep.i_eng_left_fire, ui->i_eng_left_fire, "background:red", "background:white" );
		SetState( Helicopter->cep.i_eng_rght_fire, ui->i_eng_rght_fire, "background:red", "background:white" );
		SetState( Helicopter->cep.i_ko_50_fire, ui->i_ko_50_fire, "background:red", "background:white" );
		SetState( Helicopter->cep.i_red_vsu_fire, ui->i_red_vsu_fire, "background:red", "background:white" );

		SetState( Helicopter->cep.i_eng_left_1_queue, ui->i_eng_left_1_queue, "background:yellow", "background:white" );
		SetState( Helicopter->cep.i_eng_rght_1_queue, ui->i_eng_rght_1_queue, "background:yellow", "background:white" );
		SetState( Helicopter->cep.i_ko_50_1_queue, ui->i_ko_50_1_queue, "background:yellow", "background:white" );
		SetState( Helicopter->cep.i_red_vsu_1_queue, ui->i_red_vsu_1_queue, "background:yellow", "background:white" );

		SetState( Helicopter->cep.i_eng_left_2_queue, ui->i_eng_left_2_queue, "background:yellow", "background:white" );
		SetState( Helicopter->cep.i_eng_rght_2_queue, ui->i_eng_rght_2_queue, "background:yellow", "background:white" );
		SetState( Helicopter->cep.i_ko_50_2_queue, ui->i_ko_50_2_queue, "background:yellow", "background:white" );
		SetState( Helicopter->cep.i_red_vsu_2_queue, ui->i_red_vsu_2_queue, "background:yellow", "background:white" );

		SetState( Helicopter->ccp.i_ap_dir_on, ui->iAPDir, "background:green", "background:white" );
		SetState( Helicopter->ccp.i_ap_dir_off, ui->iAPDirOff, "background:red", "background:white" );
		//SetState(Helicopter_->_OnBoard->b_autopilot, ui->iAP, "background:green", "background:white");
		SetState( Helicopter->ccp.i_ap_on, ui->iAP, "background:green", "background:white" );
		SetState( Helicopter->ccp.i_ap_alt_on, ui->iAPAlt, "background:green", "background:white" );
		SetState( Helicopter->ccp.i_ap_alt_off, ui->iAPAltOff, "background:red", "background:white" );

		SetState( Helicopter->cep.i_hidro_main, ui->i_hidro_main, "background:green", "background:white" );
		SetState( Helicopter->cep.i_hidro_second, ui->i_hidro_second, "background:red", "background:white" );
		SetState( Helicopter->cep.i_fuel_left_close, ui->i_fuel_left_close, "background:yellow", "background:white" );
		SetState( Helicopter->cep.i_fuel_right_close, ui->i_fuel_right_close, "background:yellow", "background:white" );
		SetState( Helicopter->cep.i_fuel_bypass_close, ui->i_fuel_bypass_close, "background:yellow", "background:white" );
		SetState( Helicopter->cep.i_fpump_consum_off, ui->i_fpump_consum_off, "background:yellow", "background:white" );
		SetState( Helicopter->cep.i_fpump_left_off, ui->i_fpump_left_off, "background:yellow", "background:white" );
		SetState( Helicopter->cep.i_fpump_right_off, ui->i_fpump_right_off, "background:yellow", "background:white" );
		
		SetState( Helicopter->rsp_elp.i_ko50_heater, ui->i_ko50_heater, "background:yellow", "background:white" );
		SetState( Helicopter->rsp_elp.i_ko50_ignition, ui->i_ko50_ignition, "background:yellow", "background:white" );
		SetState( Helicopter->rsp_elp.i_ko50_work, ui->i_ko50_work, "background:green", "background:white" );

		SetState( Helicopter->cep.i_contr_sensors, ui->i_contr_sensors, "background:yellow", "background:white" );
		SetState( Helicopter->cep.i_ta_automat_on, ui->i_ta_automat_on, "background:green", "background:white" );
		SetState( Helicopter->cep.i_p_oil_norm, ui->i_p_oil_norm, "background:green", "background:white" );
		SetState( Helicopter->cep.i_rpm_norm, ui->i_rpm_norm, "background:green", "background:white" );
		SetState( Helicopter->cep.i_rpm_max, ui->i_rpm_max, "background:green", "background:white" );
		SetState( Helicopter->cep.i_eng_automat_on, ui->i_eng_automat_on, "background:green", "background:white" );
		SetState( Helicopter->cep.i_ta_work, ui->i_ta_work, "background:green", "background:white" );

		//SetState(Helicopter->lpb.i_eng_left_turn_off, ui->i_eng_left_turn_off, "background:yellow", "background:white");
		SetState( Helicopter->lpb.i_eng_left_vibr_max, ui->i_eng_left_vibr_max, "background:yellow", "background:white" );
		SetState( Helicopter->lpb.i_eng_left_swarf, ui->i_eng_left_swarf, "background:yellow", "background:white" );
		SetState( Helicopter->lpb.i_eng_left_tf, ui->i_eng_left_tf, "background:yellow", "background:white" );
		SetState( Helicopter->lpb.i_eng_left_max_rpm, ui->i_eng_left_max_rpm, "background:yellow", "background:white" );
		SetState( Helicopter->lpb.i_eng_left_emrg, ui->i_eng_left_emrg, "background:yellow", "background:white" );
		SetState( Helicopter->lpb.i_eng_left_p_oil, ui->i_eng_left_p_oil, "background:yellow", "background:white" );
		SetState( Helicopter->lpb.i_eng_left_erd_off, ui->i_eng_left_erd_off, "background:yellow", "background:white" );
		//SetState(Helicopter->lpb.i_eng_right_turn_off, ui->i_eng_right_turn_off, "background:yellow", "background:white");
		SetState( Helicopter->lpb.i_eng_right_vibr_max, ui->i_eng_right_vibr_max, "background:yellow", "background:white" );
		SetState( Helicopter->lpb.i_eng_right_swarf, ui->i_eng_right_swarf, "background:yellow", "background:white" );
		SetState( Helicopter->lpb.i_eng_right_tf, ui->i_eng_right_tf, "background:yellow", "background:white" );
		SetState( Helicopter->lpb.i_eng_right_max_rpm, ui->i_eng_right_max_rpm, "background:yellow", "background:white" );
		SetState( Helicopter->lpb.i_eng_right_emrg, ui->i_eng_right_emrg, "background:yellow", "background:white" );
		SetState( Helicopter->lpb.i_eng_right_p_oil, ui->i_eng_right_p_oil, "background:yellow", "background:white" );
		SetState( Helicopter->lpb.i_eng_right_erd_off, ui->i_eng_right_erd_off, "background:yellow", "background:white" );

		SetState( Helicopter->lpb.i_main_gear_swarf, ui->i_main_gear_swarf, "background:yellow", "background:white" );
		SetState( Helicopter->lpb.i_inter_gear_swarf, ui->i_inter_gear_swarf, "background:yellow", "background:white" );
		SetState( Helicopter->lpb.i_tail_gear_swarf, ui->i_tail_gear_swarf, "background:yellow", "background:white" );

		// Правая Боковая Панель Электропульта
		SetState( Helicopter->ep_elp.i_vu1_brok, ui->lb_vu_1_off, "background:yellow", "background:white" );
		SetState( Helicopter->ep_elp.i_vu2_brok, ui->lb_vu_2_off, "background:yellow", "background:white" );
		SetState( Helicopter->ep_elp.i_aero_pit_dc, ui->lb_test_sys, "background:yellow", "background:white" );
		SetState( Helicopter->ep_elp.i_gen_1_off, ui->lb_gen_1_off, "background:red", "background:white" );
		SetState( Helicopter->ep_elp.i_gen_2_off, ui->lb_gen_2_off, "background:red", "background:white" );
		SetState( Helicopter->ep_elp.i_reser36, ui->lb_reser36, "background:yellow", "background:white" );
		SetState( Helicopter->ep_elp.i_ptc_on, ui->lb_pt_on, "background:yellow", "background:white" );
		SetState( Helicopter->ep_elp.i_resev_on, ui->lb_resev_on, "background:yellow", "background:white" );
		SetState( Helicopter->ep_elp.i_rap_on, ui->lb_rap_on, "background:yellow", "background:white" );
	}
	// передаем в модель все кнопки из окна если ручной режим еили нет Мохи
	if ( Helicopter->controls_->ManualControl( ) || ( Helicopter->can->BoardCount( ) == 0 ) )
	{
		Helicopter->lsp_elp.t_IV_500 = ui->t_IV_500->isChecked( );
		// Левая АЗС
		Helicopter->lp_elp.t_deicing_auto = ui->t_pos_auto->isChecked( );
		Helicopter->lp_elp.t_deicing_off = ui->t_off->isChecked( );
		Helicopter->lp_elp.t_deicing_left_pzu = ui->t_pzu_left_eng->isChecked( );
		Helicopter->lp_elp.t_deicing_right_pzu = ui->t_pzu_right_eng->isChecked( );
		Helicopter->lp_elp.t_deicing_glass = ui->t_auto_glass->isChecked( );
		Helicopter->lp_elp.sw_consumers = ui->sw_consumers->value( );
		// правый боковой электрощиток электропульта
		Helicopter->rsp_ep.t_pzu_left = ui->t_pzu_left->isChecked( );
		Helicopter->rsp_ep.t_pzu_right = ui->t_pzu_right->isChecked( );
		Helicopter->rsh_elp.t_pvd_heat_contr = ui->t_pvd_heat_contr->isChecked( );
		Helicopter->lsh_elp.t_pvd_heat = ui->t_pvd_heat_contr->isChecked( );
		Helicopter->lsh_elp.t_ri_65 = ui->t_ri_65->isChecked( );
		Helicopter->rsh_elp.t_diss = ui->cb_35->isChecked( );

		// лампочки и груз
		Helicopter->rsp_ep.t_ano = ui->t_ano->isChecked();
		Helicopter->rsp_ep.t_flashlight = ui->t_ano->isChecked();
		Helicopter->rsp_ep.t_beacon_down = ui->t_ano->isChecked();
		Helicopter->rsp_ep.t_light_drill = ui->t_ano->isChecked();
		Helicopter->rsp_ep.t_light_contur = ui->t_ano->isChecked();
		Helicopter->lpb.t_rulejka = ui->t_ano->isChecked();
		Helicopter->lpb.t_fpp = ui->t_ano->isChecked();
		Helicopter->lpb.t_svet_ubran = !ui->t_ano->isChecked();
		Helicopter->rpb.t_fara_close = !ui->t_ano->isChecked();
		Helicopter->PC.t_avar_drop = ui->Drop->isDown();
		Helicopter->PC.t_tact_drop = ui->Drop->isDown();
		// АЗС
		Helicopter->rp_azs.t_ta_start = ui->cb_1->isChecked( );
		Helicopter->rp_azs.t_ta_ign = ui->cb_2->isChecked( );
		Helicopter->rp_azs.t_eng_start = ui->cb_3->isChecked( );
		Helicopter->rp_azs.t_eng_ign = ui->cb_4->isChecked( );
		Helicopter->rp_azs.t_pump_consum = ui->cb_5->isChecked( );
		Helicopter->rp_azs.t_pump_left = ui->cb_6->isChecked( );
		Helicopter->rp_azs.t_pump_right = ui->cb_7->isChecked( );
		Helicopter->rp_azs.t_fuel_gauge = ui->cb_8->isChecked( );
		Helicopter->rp_azs.t_spuu_52 = ui->cb_9->isChecked( );
		Helicopter->rp_azs.t_auto_main = ui->cb_10->isChecked( );
		Helicopter->rp_azs.t_auto_fric = ui->cb_11->isChecked( );
		Helicopter->rp_azs.t_auto_emufta = ui->cb_12->isChecked( );
		Helicopter->rp_azs.t_hidro_main = ui->cb_13->isChecked( );
		Helicopter->rp_azs.t_hidro_sec = ui->cb_14->isChecked( );
		Helicopter->rp_azs.t_sas = ui->cb_15->isChecked( );
		Helicopter->rp_azs.t_nav1 = ui->cb_16->isChecked( );
		Helicopter->rp_azs.t_nav2 = ui->cb_17->isChecked( );
		Helicopter->rp_azs.t_distmeter = ui->cb_18->isChecked( );
		Helicopter->rp_azs.t_n_ctrl = ui->cb_19->isChecked( );
		Helicopter->rp_azs.t_close_left = ui->cb_20->isChecked( );
		Helicopter->rp_azs.t_close_rght = ui->cb_21->isChecked( );
		Helicopter->rp_azs.t_close_bypass = ui->cb_22->isChecked( );
		Helicopter->rp_azs.t_ppo_signal = ui->cb_23->isChecked( );
		Helicopter->rp_azs.t_ppo_1_auto = ui->cb_24->isChecked( );
		Helicopter->rp_azs.t_ppo_1_hand = ui->cb_25->isChecked( );
		Helicopter->rp_azs.t_ppo_2_auto = ui->cb_26->isChecked( );
		Helicopter->rp_azs.t_ppo_2_hand = ui->cb_27->isChecked( );
		Helicopter->rp_azs.t_r_spu = ui->cb_28->isChecked( );
		Helicopter->rp_azs.t_r_altimetr = ui->cb_29->isChecked( );
		Helicopter->rp_azs.t_r_comand_osnv = ui->cb_30->isChecked( );
		Helicopter->rp_azs.t_r_compas_sv = ui->cb_32->isChecked( );
		Helicopter->rp_azs.t_r_compas_ukv = ui->cb_33->isChecked( );
		Helicopter->rp_azs.t_resp_uvd1 = ui->cb_34->isChecked( );
		Helicopter->rp_azs.t_resp_uvd2 = ui->cb_35->isChecked( );
		Helicopter->rp_azs.t_r_rls = ui->cb_36->isChecked( );
		Helicopter->rp_azs.t_far_left_ctrl = ui->cb_37->isChecked( );
		Helicopter->rp_azs.t_far_left_flash = ui->cb_38->isChecked( );
		Helicopter->rp_azs.t_far_right_ctrl = ui->cb_39->isChecked( );
		Helicopter->rp_azs.t_far_right_flash = ui->cb_40->isChecked( );
		Helicopter->rp_azs.t_siren_missle = ui->cb_41->isChecked( );

		Helicopter->rp_azs.t_strov_light = ui->cb_42->isChecked( );
		Helicopter->rp_azs.t_ano = ui->cb_43->isChecked( );
		Helicopter->rp_azs.t_tact_sbros = ui->cb_44->isChecked( );
		Helicopter->rp_azs.t_avar_sbros = ui->cb_45->isChecked( );
		Helicopter->rp_azs.t_ais_ctrl = ui->cb_46->isChecked( );
		Helicopter->rp_azs.t_ais_pzu_left = ui->cb_47->isChecked( );
		Helicopter->rp_azs.t_ais_pzu_rght = ui->cb_48->isChecked( );
		Helicopter->rp_azs.t_ais_co_121 = ui->cb_49->isChecked( );
		Helicopter->rp_azs.t_ais_glass = ui->cb_50->isChecked( );
		Helicopter->rp_azs.t_ais_wipe_left = ui->cb_51->isChecked( );
		Helicopter->rp_azs.t_ais_wipe_rght = ui->cb_52->isChecked( );
		Helicopter->rp_azs.t_p_nv = ui->cb_53->isChecked( );
		Helicopter->rp_azs.t_ko_50 = ui->cb_54->isChecked( );
		
		Helicopter->rsp_elp.t_ko50_ta_start = ui->t_ko50_ta_start->isDown( );
		Helicopter->rsp_elp.t_ko50_auto = ui->t_ko50_auto->isChecked( );
		Helicopter->rsp_elp.t_ko50_man = ui->t_ko50_man->isChecked( );
		Helicopter->rsp_elp.t_ko50_full = ui->t_ko50_full->isChecked( );
		Helicopter->rsp_elp.t_ko50_midl = ui->t_ko50_midl->isChecked( );
		Helicopter->rsp_elp.t_ko50_vent = ui->t_ko50_vent->isChecked( );


			// Правая Боковая Панель Электропульта
		Helicopter->ep_elp.t_akkum_1 = ui->t_akkum_1->isChecked( );
		Helicopter->ep_elp.t_akkum_2 = ui->t_akkum_2->isChecked( );
		Helicopter->ep_elp.t_gen_1 = ui->t_gen_1->isChecked( );
		Helicopter->ep_elp.t_gen_2 = ui->t_gen_2->isChecked( );
		Helicopter->ep_elp.t_res_gen = ui->t_res_gen->isChecked( );
		Helicopter->ep_elp.t_ACDC_1 = ui->t_ACDC_1->isChecked( );
		Helicopter->ep_elp.t_ACDC_2 = ui->t_ACDC_2->isChecked( );
		Helicopter->ep_elp.t_test_sys = ui->t_test_sys->isChecked( );
		Helicopter->ep_elp.t_aero_pit_dc = ui->t_aero_pit_dc->isChecked( );
		// выбор режим TP36
		if ( ui->vs_1->value( ) == 0 )
		{
			Helicopter->ep_elp.t_r36_man = true;
			Helicopter->ep_elp.t_r36_auto = false;
		}
		if ( ui->vs_1->value( ) == 1 )
		{
			Helicopter->ep_elp.t_r36_man = false;
			Helicopter->ep_elp.t_r36_auto = false;
		}
		if ( ui->vs_1->value( ) == 2 )
		{
			Helicopter->ep_elp.t_r36_man = false;
			Helicopter->ep_elp.t_r36_auto = true;
		}
		// выбор режим ПТС
		if ( ui->vs_4->value( ) == 0 )
		{
			Helicopter->ep_elp.t_pts_man = true;
			Helicopter->ep_elp.t_pts_auto = false;
		}
		if ( ui->vs_4->value( ) == 1 )
		{
			Helicopter->ep_elp.t_pts_man = false;
			Helicopter->ep_elp.t_pts_auto = false;
		}
		if ( ui->vs_4->value( ) == 2 )
		{
			Helicopter->ep_elp.t_pts_man = false;
			Helicopter->ep_elp.t_pts_auto = true;
		}
		// выбор АЭР питания
		Helicopter->ep_elp.t_aero_pit_ac = ui->vs_3->value( );  // 0 = false, 1 = true
		Helicopter->ep_elp.t_gen = ui->vs_2->value( );  // 0 = true, 1 = false

		//Helicopter->ep_elp.t_ctrl_res_gen = false;

		Helicopter->ep_elp.sw_ctrl = ui->hs_1->value( );
		if (Helicopter->ep_elp.sw_ctrl == 1) {
			Helicopter->ep_elp.t_ctrl_vu1 = true;
			Helicopter->ep_elp.t_ctrl_vu2 = false;
		}
		if (Helicopter->ep_elp.sw_ctrl == 2) {
			Helicopter->ep_elp.t_ctrl_vu1 = false;
			Helicopter->ep_elp.t_ctrl_vu2 = true;
		}
		Helicopter->ep_elp.sw_dc_volt_mode = ui->hs_2->value( );
		Helicopter->ep_elp.sw_ac_volt_mode = ui->hs_3->value( );

		// средняя панель электропульта
		// контроль-огнетушение
		if ( ui->horizontalSlider_3->value( ) == 0 )
		{
			Helicopter->cep.t_contr_sensors = true;
			Helicopter->cep.t_firefighting = false;
		}
		if ( ui->horizontalSlider_3->value( ) == 1 )
		{
			Helicopter->cep.t_contr_sensors = false;
			Helicopter->cep.t_firefighting = true;
		}
		// пиропатроны I и II
		if ( ui->horizontalSlider_4->value( ) == 0 )
		{
			Helicopter->cep.t_1 = true;
			Helicopter->cep.t_2 = false;
		}
		if ( ui->horizontalSlider_4->value( ) == 1 )
		{
			Helicopter->cep.t_1 = false;
			Helicopter->cep.t_2 = true;
		}
		// выбор режим запуска ТА (ложный, прокрутка, запуск)
		if ( ui->horizontalSlider_5->value( ) == 0 )
		{
			Helicopter->cep.t_ta_sel_false_start = true;
			Helicopter->cep.t_ta_sel_scroll = false;
			Helicopter->cep.t_ta_sel_start = false;
		}
		if ( ui->horizontalSlider_5->value( ) == 1 )
		{
			Helicopter->cep.t_ta_sel_false_start = false;
			Helicopter->cep.t_ta_sel_scroll = true;
			Helicopter->cep.t_ta_sel_start = false;
		}
		if ( ui->horizontalSlider_5->value( ) == 2 )
		{
			Helicopter->cep.t_ta_sel_false_start = false;
			Helicopter->cep.t_ta_sel_scroll = false;
			Helicopter->cep.t_ta_sel_start = true;
		}
		// выбор режима запуска двигателя (прокрутка, запуск)
		if ( ui->horizontalSlider_6->value( ) == 0 )
		{
			Helicopter->cep.t_eng_sel_scroll = true;
			Helicopter->cep.t_eng_sel_start = false;
		}
		if ( ui->horizontalSlider_6->value( ) == 1 )
		{
			Helicopter->cep.t_eng_sel_scroll = false;
			Helicopter->cep.t_eng_sel_start = false;
		}
		if ( ui->horizontalSlider_6->value( ) == 2 )
		{
			Helicopter->cep.t_eng_sel_scroll = false;
			Helicopter->cep.t_eng_sel_start = true;
		}
		// выбор двигателя для запуска
		if ( ui->horizontalSlider_7->value( ) == 0 )
		{
			Helicopter->cep.t_eng_left = true;
			Helicopter->cep.t_eng_rght = false;
		}
		if ( ui->horizontalSlider_7->value( ) == 1 )
		{
			Helicopter->cep.t_eng_left = false;
			Helicopter->cep.t_eng_rght = false;
		}
		if ( ui->horizontalSlider_7->value( ) == 2 )
		{
			Helicopter->cep.t_eng_left = false;
			Helicopter->cep.t_eng_rght = true;
		}
		// проверка зажигания
		if ( ui->horizontalSlider_8->value( ) == 0 )
		{
			Helicopter->cep.t_eng_left_ign_check = true;
			Helicopter->cep.t_eng_right_ign_check = false;
		}
		if ( ui->horizontalSlider_8->value( ) == 1 )
		{
			Helicopter->cep.t_eng_left_ign_check = false;
			Helicopter->cep.t_eng_right_ign_check = false;
		}
		if ( ui->horizontalSlider_8->value( ) == 2 )
		{
			Helicopter->cep.t_eng_left_ign_check = false;
			Helicopter->cep.t_eng_right_ign_check = true;
		}

		Helicopter->cep.sw_contr_sensors = ui->horizontalSlider_9->value( );
		Helicopter->cep.t_eng_left_1_queue = ui->t_eng_left_1_queue->isDown( );
		Helicopter->cep.t_eng_left_2_queue = ui->t_eng_left_2_queue->isDown( );
		Helicopter->cep.t_eng_rght_1_queue = ui->t_eng_rght_1_queue->isDown( );
		Helicopter->cep.t_eng_rght_2_queue = ui->t_eng_rght_2_queue->isDown( );
		Helicopter->cep.t_ko_50_1_queue = ui->t_ko_50_1_queue->isDown( );
		Helicopter->cep.t_ko_50_2_queue = ui->t_ko_50_2_queue->isDown( );
		Helicopter->cep.t_red_vsu_1_queue = ui->t_red_vsu_1_queue->isDown( );
		Helicopter->cep.t_red_vsu_2_queue = ui->t_red_vsu_2_queue->isDown( );

		Helicopter->cep.t_ta_start = ui->pbTAStart->isDown( );
		Helicopter->cep.t_ta_stop = ui->pbTAStop->isDown( );
		// контроль каналов
		Helicopter->cep.get_sw_contr_sensors( ui->horizontalSlider_7->value( ) );
		// тумблеры гидросистемы
		Helicopter->cep.t_hidro_main = ui->t_hidro_main->isChecked( );
		Helicopter->cep.t_hidro_second = ui->t_hidro_second->isChecked( );
		// тумблеры запуска двигателя
		Helicopter->cep.t_eng_start = ui->pbLanch->isDown( );
		Helicopter->cep.t_eng_stop = ui->pbLanchOff->isDown( );
		// тумблеры управления топливными кранами 
		Helicopter->cep.t_fuel_left_close = ui->t_fuel_left_close->isChecked( );
		Helicopter->cep.t_fuel_right_close = ui->t_fuel_right_close->isChecked( );
		Helicopter->cep.t_fuel_bypass_close = ui->t_fuel_bypass_close->isChecked( );
		Helicopter->cep.t_fuel_perepusk = ui->t_fuel_perepusk->isChecked( );
		//тумблеры управления топливными насосами
		Helicopter->cep.t_fpump_refil = ui->t_fpump_refil->isChecked( );
		Helicopter->cep.t_fpump_consum = ui->t_fpump_consum->isChecked( );
		Helicopter->cep.t_fpump_left = ui->t_fpump_left->isChecked( );
		Helicopter->cep.t_fpump_right = ui->t_fpump_right->isChecked( );

		// левая панель приборов
		//Helicopter->lpb.ugr_4uk.f_set_course = ui->f_set_course->value( );
		//Helicopter->lpb.ugr_4uk.d1 = ui->f_set_course->value( );
		//Helicopter_->lpb.vd_10vk.p = ui->f_barometr->value();
				// правая панель приборов
		Helicopter->lpb.t_apk_ukv = ui->apk->value( );
		Helicopter->lpb.t_apk_sv = !Helicopter->lpb.t_apk_ukv;
		Helicopter->lpb.t_max_roll = ui->max_roll->value( );

		Helicopter->PC.t_NV_brake_on = ui->t_NV_brake_on->isChecked( );
		Helicopter->PC.t_NV_brake_off = !Helicopter->PC.t_NV_brake_on;
		Helicopter->PC.t_eng_left_brake = ui->t_eng_left_brake->isChecked( );
		Helicopter->PC.t_eng_rght_brake = ui->t_eng_rght_brake->isChecked( );
		// автопилот
		Helicopter->PC.t_left_ap_off = ui->pbAPOff->isDown( );
		Helicopter->ccp.t_ap_on = ui->iAP->isDown( );
		Helicopter->ccp.t_ap_dir_on = ui->iAPDir->isDown( );
		Helicopter->ccp.t_ap_dir_off = ui->iAPDirOff->isDown( );
		Helicopter->ccp.t_ap_alt_on = ui->iAPAlt->isDown( );
		Helicopter->ccp.t_ap_alt_off = ui->iAPAltOff->isDown( );
		Helicopter->cep.t_off_second_hidro = ui->t_off_second_hidro->isDown( );
		Helicopter->rsp_ep.t_check_lamp = ui->t_check_lamp->isChecked( );

		Helicopter->rpb.diss_15.t_left = ui->left->isDown( );
		Helicopter->rpb.diss_15.t_rght = ui->right->isDown( );
		Helicopter->rpb.diss_15.t_n = ui->back->isDown( );
		Helicopter->rpb.diss_15.t_v = ui->forward->isDown( );
		Helicopter->rpb.diss_15.t_minus = ui->minus->isDown( );
		Helicopter->rpb.diss_15.t_plus = ui->plus->isDown( );
		Helicopter->rpb.diss_15.t_off = ui->off->isDown( );
		Helicopter->rpb.diss_15.t_on = ui->on->isDown( );
		ui->f_side->display( static_cast<int>( Helicopter->rpb.diss_15.get_side( ) ) );
		ui->f_way->display( static_cast<int>( Helicopter->rpb.diss_15.get_way( ) ) );
		ui->f_angle->display( static_cast<int>( Helicopter->rpb.diss_15.f_angle ) );
		SetState( Helicopter->rpb.diss_15.state, ui->i_on, "color:red", "color:black" );

		Helicopter->lsh_elp.t_ag_left = ui->t_ag_left->isChecked( );
		Helicopter->lsh_elp.t_ag_res = ui->t_ag_res->isChecked( );
		Helicopter->rsh_elp.t_ag_right = ui->t_ag_right->isChecked( );
		Helicopter->lsh_elp.t_bkk_18 = ui->t_bkk_18->isChecked( );
		Helicopter->lsh_elp.t_rv = ui->t_rv->isChecked( );
		Helicopter->rsh_elp.t_diss = ui->t_diss->isChecked( );

		Helicopter->rpb.sw_fuel() = ui->sw_fuel->value( );
		Helicopter->PC.t_ap_dir_sopr = ui->t_ap_dir_sopr->isChecked( );
		Helicopter->PC.t_fr_l_fosh = ui->t_fr_l_fosh->isChecked();
	} // конец ручного режима
	// режим работы с тренажером, индикация положения тумблеров из кабины
	if ( !Helicopter->controls_->ManualControl( ) )
	{

		ui->t_IV_500->setChecked( Helicopter->lsp_elp.t_IV_500 );
		ui->p_Fosh->setValue( Helicopter->PC.f_Fosh );
		ui->p_korrector->setValue( Helicopter->PC.f_Ecor );

		//        ui->f_barometr->setValue(Helicopter_->lpb.vd_10vk.p);

		ui->p_break->setValue( Helicopter->PC.f_break );
		ui->pbAPOff->setDown( Helicopter->PC.t_left_ap_off );// || Helicopter_->PC.t_rght_ap_off);

		ui->cb_1->setChecked( Helicopter->rp_azs.t_ta_start );
		ui->cb_2->setChecked( Helicopter->rp_azs.t_ta_ign );
		ui->cb_3->setChecked( Helicopter->rp_azs.t_eng_start );
		ui->cb_4->setChecked( Helicopter->rp_azs.t_eng_ign );
		ui->cb_5->setChecked( Helicopter->rp_azs.t_pump_consum );
		ui->cb_6->setChecked( Helicopter->rp_azs.t_pump_left );
		ui->cb_7->setChecked( Helicopter->rp_azs.t_pump_right );
		ui->cb_8->setChecked( Helicopter->rp_azs.t_fuel_gauge );
		ui->cb_9->setChecked( Helicopter->rp_azs.t_spuu_52 );
		ui->cb_10->setChecked( Helicopter->rp_azs.t_auto_main );
		ui->cb_11->setChecked( Helicopter->rp_azs.t_auto_fric );
		ui->cb_12->setChecked( Helicopter->rp_azs.t_auto_emufta );
		ui->cb_13->setChecked( Helicopter->rp_azs.t_hidro_main );
		ui->cb_14->setChecked( Helicopter->rp_azs.t_hidro_sec );
		ui->cb_15->setChecked( Helicopter->rp_azs.t_sas );
		ui->cb_16->setChecked( Helicopter->rp_azs.t_nav1 );
		ui->cb_17->setChecked( Helicopter->rp_azs.t_nav2 );
		ui->cb_18->setChecked( Helicopter->rp_azs.t_distmeter );
		ui->cb_19->setChecked( Helicopter->rp_azs.t_n_ctrl );
		ui->cb_20->setChecked( Helicopter->rp_azs.t_close_left );
		ui->cb_21->setChecked( Helicopter->rp_azs.t_close_rght );
		ui->cb_22->setChecked( Helicopter->rp_azs.t_close_bypass );
		ui->cb_23->setChecked( Helicopter->rp_azs.t_ppo_signal );
		ui->cb_24->setChecked( Helicopter->rp_azs.t_ppo_1_auto );
		ui->cb_25->setChecked( Helicopter->rp_azs.t_ppo_1_hand );
		ui->cb_26->setChecked( Helicopter->rp_azs.t_ppo_2_auto );
		ui->cb_27->setChecked( Helicopter->rp_azs.t_ppo_2_hand );
		ui->cb_28->setChecked( Helicopter->rp_azs.t_r_spu );
		ui->cb_29->setChecked( Helicopter->rp_azs.t_r_altimetr );
		ui->cb_30->setChecked( Helicopter->rp_azs.t_r_comand_osnv );

		ui->cb_32->setChecked( Helicopter->rp_azs.t_r_compas_sv );
		ui->cb_33->setChecked( Helicopter->rp_azs.t_r_compas_ukv );
		ui->cb_34->setChecked( Helicopter->rp_azs.t_resp_uvd1 );
		ui->cb_35->setChecked( Helicopter->rp_azs.t_resp_uvd2 );
		ui->cb_36->setChecked( Helicopter->rp_azs.t_r_rls );
		ui->cb_37->setChecked( Helicopter->rp_azs.t_far_left_ctrl );
		ui->cb_38->setChecked( Helicopter->rp_azs.t_far_left_flash );
		ui->cb_39->setChecked( Helicopter->rp_azs.t_far_right_ctrl );
		ui->cb_40->setChecked( Helicopter->rp_azs.t_far_right_flash );
		ui->cb_41->setChecked( Helicopter->rp_azs.t_siren_missle );
		ui->cb_42->setChecked( Helicopter->rp_azs.t_strov_light );
		ui->cb_43->setChecked( Helicopter->rp_azs.t_ano );
		ui->cb_44->setChecked( Helicopter->rp_azs.t_tact_sbros );
		ui->cb_45->setChecked( Helicopter->rp_azs.t_avar_sbros );
		ui->cb_46->setChecked( Helicopter->rp_azs.t_ais_ctrl );
		ui->cb_47->setChecked( Helicopter->rp_azs.t_ais_pzu_left );
		ui->cb_48->setChecked( Helicopter->rp_azs.t_ais_pzu_rght );
		ui->cb_49->setChecked( Helicopter->rp_azs.t_ais_co_121 );
		ui->cb_50->setChecked( Helicopter->rp_azs.t_ais_glass );
		ui->cb_51->setChecked( Helicopter->rp_azs.t_ais_wipe_left );
		ui->cb_52->setChecked( Helicopter->rp_azs.t_ais_wipe_rght );
		ui->cb_53->setChecked( Helicopter->rp_azs.t_p_nv );
		ui->cb_54->setChecked( Helicopter->rp_azs.t_ko_50 );

		// электрик пульт
		ui->t_akkum_1->setChecked( Helicopter->ep_elp.t_akkum_1 );
		ui->t_akkum_2->setChecked( Helicopter->ep_elp.t_akkum_2 );
		ui->t_gen_1->setChecked( Helicopter->ep_elp.t_gen_1 );
		ui->t_gen_2->setChecked( Helicopter->ep_elp.t_gen_2 );
		ui->t_res_gen->setChecked( Helicopter->ep_elp.t_res_gen );
		ui->t_ACDC_1->setChecked( Helicopter->ep_elp.t_ACDC_1 );
		ui->t_ACDC_2->setChecked( Helicopter->ep_elp.t_ACDC_2 );
		ui->t_test_sys->setChecked( Helicopter->ep_elp.t_test_sys );
		ui->t_aero_pit_dc->setChecked( Helicopter->ep_elp.t_aero_pit_dc );

		ui->hs_1->setValue( Helicopter->ep_elp.sw_ctrl );
		ui->hs_2->setValue( Helicopter->ep_elp.sw_dc_volt_mode );
		ui->hs_3->setValue( Helicopter->ep_elp.sw_ac_volt_mode );

		if ( Helicopter->ep_elp.t_r36_man )
			ui->vs_1->setValue( 0 );
		else if ( Helicopter->ep_elp.t_r36_auto )
			ui->vs_1->setValue( 2 );
		else
			ui->vs_1->setValue( 1 );

		if ( Helicopter->ep_elp.t_pts_man )
			ui->vs_4->setValue( 0 );
		else if ( Helicopter->ep_elp.t_pts_auto )
			ui->vs_4->setValue( 2 );
		else
			ui->vs_4->setValue( 1 );

		ui->vs_2->setValue( Helicopter->ep_elp.t_gen );

		// тумблеры гидросистемы
		ui->t_hidro_main->setChecked( Helicopter->cep.t_hidro_main );
		ui->t_hidro_second->setChecked( Helicopter->cep.t_hidro_second );
		// тумблеры управления топливными кранами 
		ui->t_fuel_left_close->setChecked( Helicopter->cep.t_fuel_left_close );
		ui->t_fuel_right_close->setChecked( Helicopter->cep.t_fuel_right_close );
		ui->t_fuel_bypass_close->setChecked( Helicopter->cep.t_fuel_bypass_close );
		ui->t_fuel_perepusk->setChecked( Helicopter->cep.t_fuel_perepusk );
		//тумблеры управления топливными насосами
		ui->t_fpump_consum->setChecked( Helicopter->cep.t_fpump_consum );
		ui->t_fpump_left->setChecked( Helicopter->cep.t_fpump_left );
		ui->t_fpump_right->setChecked( Helicopter->cep.t_fpump_right );
		ui->t_eng_left_brake->setChecked( Helicopter->PC.t_eng_left_brake );
		ui->t_eng_rght_brake->setChecked( Helicopter->PC.t_eng_rght_brake );
		ui->t_NV_brake_on->setChecked( !Helicopter->PC.t_NV_brake_off );
		ui->t_ap_dir_sopr->setChecked( Helicopter->PC.t_ap_dir_sopr );
		ui->t_fr_l_fosh->setChecked( Helicopter->PC.t_fr_l_fosh );
		ui->horizontalSlider_3->setValue( Helicopter->cep.t_contr_sensors ? 0 : 1 );
		// пиропатроны I и II
		ui->horizontalSlider_4->setValue( Helicopter->cep.t_1 ? 0 : 1 );
		// выбор режим запуска ТА (ложный, прокрутка, запуск)
		if ( Helicopter->cep.t_ta_sel_false_start ) ui->horizontalSlider_5->setValue( 0 );
		if ( Helicopter->cep.t_ta_sel_scroll ) ui->horizontalSlider_5->setValue( 1 );
		if ( Helicopter->cep.t_ta_sel_start ) ui->horizontalSlider_5->setValue( 2 );

		// выбор режима запуска двигателя (прокрутка, запуск)
		if ( Helicopter->cep.t_eng_sel_scroll ) ui->horizontalSlider_6->setValue( 0 );
		else if ( Helicopter->cep.t_eng_sel_start ) ui->horizontalSlider_6->setValue( 2 );
		else  ui->horizontalSlider_6->setValue( 1 );

		// выбор двигателя для запуска
		if ( Helicopter->cep.t_eng_left ) ui->horizontalSlider_7->setValue( 0 );
		else if ( Helicopter->cep.t_eng_rght ) ui->horizontalSlider_7->setValue( 2 );
		else  ui->horizontalSlider_7->setValue( 1 );

		// проверка зажигания
		if ( Helicopter->cep.t_eng_left_ign_check ) ui->horizontalSlider_8->setValue( 0 );
		else if ( Helicopter->cep.t_eng_right_ign_check ) ui->horizontalSlider_8->setValue( 2 );
		else  ui->horizontalSlider_8->setValue( 1 );

		ui->apk->setValue( Helicopter->lpb.t_apk_ukv );
		ui->max_roll->setValue( Helicopter->lpb.t_max_roll );

		// индицируем положение платформы

	}
	// этот блок выполянется во всех режимах для отладки нужных элеменов
	// что бы можно было вручную подвигать стрелки
	{
		//Helicopter_->lsp_elp.f_p_air = ui->f_p_air->value();
		//Helicopter_->lsp_elp.f_p_brake = ui->f_p_brake->value();
		// качалка платформы из формы
		//Helicopter_->_BoardSystem->set_ms_roll( ui->horizont->value());
		//Helicopter_->_BoardSystem->set_ms_pitch( ui->vertical->value());
		// управление автопилотом с виртуальных кнопок 
		// TODO управление автопилотом с окна программы, пока пульт не вернут в вертолет
		Helicopter->ccp.t_ap_on = ui->iAP->isDown( );
		Helicopter->ccp.t_ap_dir_on = ui->iAPDir->isDown( );
		Helicopter->ccp.t_ap_dir_off = ui->iAPDirOff->isDown( );
		Helicopter->ccp.t_ap_alt_on = ui->iAPAlt->isDown( );
		Helicopter->ccp.t_ap_alt_off = ui->iAPAltOff->isDown( );
	}
}

void Helicopter_PanelWindow::PitchSliderReleased( )
{
	Helicopter->ccp.f_set_pitch = ui->Pitch->value( );
}

void Helicopter_PanelWindow::RollSliderReleased( )
{
	Helicopter->ccp.f_set_roll = ui->Roll->value( );
}

template<typename... Args>
void Helicopter_PanelWindow::SetSlider( QSlider* slider, Args... args )
{
	bool params[] = { args... };    auto index = 0;
	for ( int i = 0; i < sizeof...( args ); i++ )
	{
		if ( params[i] )
		{
			slider->setValue( i );
			break;
		}
	}
}

template<typename... Args>
void Helicopter_PanelWindow::CheckSlider( QSlider* slider, Args... args )
{
	bool params[] = { args... };
	auto index = slider->value( );
	if ( index < sizeof...( args ) )
	{
		params[index] = true;
	}
}
