#include "Helicopter_MainWindow.h"
#include "Helicopter_core.h"
#include "Helicopter_can.h"
#include "Helicopter_imager.h"
#include "Helicopter_server.h"
#include "Helicopter_imager_tcp_client.h"
#include "Helicopter_imager_tcp_client_sfera.h"
#include "Helicopter_instructor_tcp_client.h"
#include "Helicopter_script_tcp_client.h"
#include "Helicopter_imitator_tcp_client.h"
#include "Helicopter_OC_tcp_client.h"
#include "Common/ServerInstructor.h"
#include "Helicopter_BoardSystem.h"
#include "FlightRecorder/flightrecorder.h"
//#include "FlightRecorder/fr_controls.h"
#include "ui_Helicopter_MainWindow.h"
#include <QInputDialog>
#include <QSpinBox>
#include <QDebug>
#include "Common/log_duration.h"

Helicopter_MainWindow::Helicopter_MainWindow( QWidget* parent ) : QMainWindow( parent ), ui( new Ui::Helicopter_MainWindow )
{
	ui->setupUi( this );
	this->setWindowFlags( Qt::Window | Qt::WindowCloseButtonHint );
	this->setFixedSize( this->width( ), this->height( ) );
	this->setWindowTitle( "Mi-171 Control Program" );

	SystemTray_ = new SystemTray( );
	SystemTray_->setToolTip( this->windowTitle( ) );

	Helicopter = new Helicopter_core( this );

	NetStatTimer_ = new QTimer( this );
	NetStatTimer_->setInterval( 1000 );
	connect( NetStatTimer_, SIGNAL( timeout( ) ), this, SLOT( slotUpdateNetStat( ) ) );
	NetStatTimer_->start( );

	if ( Helicopter->can->BoardCount( ) > 0 )
	{
		ui->labelMsg->setText( "Moxa CAN Card OK !" );
		ui->labelMsg->setStyleSheet( "background-color: green" );
	}
	else
	{
		ui->labelMsg->setText( "There is no Moxa CAN Card.........!" );
		ui->labelMsg->setStyleSheet( "background-color: red" );
	}

	connect( this, SIGNAL( signalPlaceToTray( ) ), this, SLOT( slotPlaceToTray( ) ), Qt::QueuedConnection );
	connect( SystemTray_, SIGNAL( signalShow( ) ), this, SLOT( slotShowMainWindow( ) ), Qt::QueuedConnection );
	connect( SystemTray_, SIGNAL( signalQuit( ) ), this, SLOT( slotApplicationQuit( ) ), Qt::QueuedConnection );

	Helicopter->instructor->weight = ui->Weight->value( );
	Helicopter->instructor->fuel = ui->Fuel->value( );
	Helicopter->instructor->weather.wind_speed = ui->Wind->value( );
	Helicopter->instructor->weather.wind_dir = ui->WindDir->value( );
	Helicopter->instructor->weather.temp = ui->Temp->value( );
	Helicopter->instructor->weather.pressure = ui->Press->value( );
	emit ui->weather->activated( 0 );

	connect( ui->Weight, QOverload<int>::of( &QSpinBox::valueChanged ),
			 [=] ( int i ) { Helicopter->instructor->weight = i; } );

	connect( ui->Fuel, QOverload<int>::of( &QSpinBox::valueChanged ),
			 [=] ( int i ) { Helicopter->instructor->fuel = i; } );

	connect( ui->Wind, QOverload<int>::of( &QSpinBox::valueChanged ),
			 [=] ( int i ) { Helicopter->instructor->weather.wind_speed = i; Helicopter->instructor->WindChanged = true; } );

	connect( ui->WindDir, QOverload<int>::of( &QSpinBox::valueChanged ),
			 [=] ( int i ) { Helicopter->instructor->weather.wind_dir = i; Helicopter->instructor->WindChanged = true; } );

	connect( ui->Temp, QOverload<int>::of( &QSpinBox::valueChanged ),
			 [=] ( int i ) { Helicopter->instructor->weather.temp = i; } );

	connect( ui->Press, QOverload<int>::of( &QSpinBox::valueChanged ),
			 [=] ( int i ) { Helicopter->instructor->weather.pressure = i; } );

	connect( ui->Alt, QOverload<int>::of( &QSpinBox::valueChanged ),
			 [=] ( int i )
			 {
				 Helicopter->instructor->weather.temp = 15 - ( i * 0.0062f );
				 ui->Temp->setValue( Helicopter->instructor->weather.temp );
				 Helicopter->instructor->weather.pressure = 760 - ( i * 0.07f );
				 ui->Press->setValue( Helicopter->instructor->weather.pressure );
			 } );

	Helicopter->PC.dead_zone = ui->DZ->value( );
	Helicopter->PC.dead_zone_pd = 0;
	Helicopter->_BoardSystem->platform_period = 50;

	connect( ui->DZ, QOverload<int>::of( &QSpinBox::valueChanged ),
			 [=] ( int i ) { Helicopter->PC.dead_zone = i; } );

	ui->lblImagerStatus->setMask( QRegion( 0, 0, 14, 14, QRegion::Ellipse ) );
	ui->lblInstructorStatus->setMask( QRegion( 0, 0, 14, 14, QRegion::Ellipse ) );

	//ui->lblMotionSystemStatus->setMask(QRegion(0,0,14,14, QRegion::Ellipse));
	ui->lblOUStatus->setMask( QRegion( 0, 0, 14, 14, QRegion::Ellipse ) );
	ui->lblScriptServerStatus->setMask( QRegion( 0, 0, 14, 14, QRegion::Ellipse ) );
	ui->lblScreenItemsStatus->setMask( QRegion( 0, 0, 14, 14, QRegion::Ellipse ) );
	ui->lblJoystickStatus->setMask( QRegion( 0, 0, 14, 14, QRegion::Ellipse ) );
	ui->lblObjControlStatus->setMask( QRegion( 0, 0, 14, 14, QRegion::Ellipse ) );
	ui->lblModelStatus->setMask( QRegion( 0, 0, 14, 14, QRegion::Ellipse ) );
	ui->lblLPPStatus->setMask( QRegion( 0, 0, 14, 14, QRegion::Ellipse ) );
	ui->lblRPPStatus->setMask( QRegion( 0, 0, 14, 14, QRegion::Ellipse ) );
    ui->lblStand->setMask(QRegion(0, 0, 14, 14, QRegion::Ellipse));

	CanWindow_ = new Helicopter_CANWindow( this, Helicopter->controls_ );
	CanWindow_->hide( );

	FaultWindow_ = new Helicopter_Faults( Helicopter, this );
	FaultWindow_->hide( );
	/*
		errorswindow = new Helicopter_ErrorsWindow;
		errorswindow->_Helicopter_manager = Helicopter_;
		errorswindow->hide();
	*/

	PilotWindow_ = new Helicopter_PanelWindow( Helicopter, this );
	WeaponWindow_ = new Helicopter_WeaponWindow( Helicopter, this );

	//FR_ = new FlightRecorder( Helicopter->Model_->GetFPar( ), Helicopter->Model_->GetIPar( ), this );
	//FR_->start( );
	//FR_C = new FR_Controls( 500, 400, FR_ );

	if ( Helicopter->tcpclient_imager_->GetPort( ) == 10000 )
	{
		ui->rb10000->setChecked( true );
		ui->ExStartButton_1->setEnabled( true );
		ui->ExStartButton_2->setEnabled( false );
		ui->ExStartButton_3->setEnabled( false );
		ui->ExStartButton_4->setEnabled( true );
    }
    else
	{
		ui->rb8001->setChecked( true );
		ui->ExStartButton_1->setEnabled( false );
		ui->ExStartButton_2->setEnabled( true );
		ui->ExStartButton_3->setEnabled( true );
		ui->ExStartButton_4->setEnabled( false );
	}
    ui->ExStopButton->setEnabled( false );
}

Helicopter_MainWindow::~Helicopter_MainWindow( )
{
	qDebug( ) << __FUNCTION__;
	//    delete PilotWindow_;
	//    delete errorswindow;
	delete CanWindow_;
	//    delete msform;
	delete NetStatTimer_;
	delete Helicopter;
	delete SystemTray_;
	delete ui;
}

void Helicopter_MainWindow::keyPressEvent( QKeyEvent* event )
{
	int key = event->key( );

	if ( key == Qt::Key_9 )
	{
		CanWindow_->UpdateTimer->start( );
		CanWindow_->show( );
	}
	if ( key == Qt::Key_1 )
	{
		PilotWindow_->UpdateTimer->start( );
		PilotWindow_->show( );
	}
	if ( key == Qt::Key_2 )
	{
		//WeaponWindow_->UpdateTimer->start( );
		WeaponWindow_->show( );
	}
	if ( key == Qt::Key_3 )
	{
		FaultWindow_->show( );
	}
	if ( key == Qt::Key_4 )
	{
		//FR_->show( );
		//FR_->start( );
	}
	//if(key==Qt::Key_3)
	//{
	//    errorswindow->UpdateTimer->start();
	//    errorswindow->show();
	//}
}

void Helicopter_MainWindow::closeEvent( QCloseEvent* /*event*/ )
{
	qDebug( ) << __FUNCTION__;
	Helicopter->Stop( );
	//    event->ignore();
	//    emit signalPlaceToTray();
}

void Helicopter_MainWindow::slotPlaceToTray( )
{
	SystemTray_->showIconTray( );
	this->hide( );
}

void Helicopter_MainWindow::slotShowMainWindow( )
{
	SystemTray_->showIconTray( );
	this->show( );
}

void Helicopter_MainWindow::slotApplicationQuit( )
{
	SystemTray_->hideIconTray( );
	Helicopter->log_->WriteLog( LogEvents::Application_close, "" );
	QApplication::quit( );
}

void Helicopter_MainWindow::ExStart( quint8 maptype )
{
	qDebug( ) << __FUNCTION__;
	Helicopter->instructor->ExerciseParameters.ReStart = true;
	Helicopter->instructor->ExerciseParameters.Pause = false;
	Helicopter->instructor->ExerciseParameters.Stop = false;
	Helicopter->instructor->ExerciseParameters.ExerciseNum = 0;
	Helicopter->instructor->ExerciseParameters.MapType = maptype;  // AKTOBE
	Helicopter->instructor->ExerciseParameters.night = false;
	Helicopter->instructor->ExerciseParameters.season = static_cast<quint8>( Seasons::Summer );
	Helicopter->instructor->ExerciseParameters.AutoloadingSystem = false;
	Helicopter->instructor->ExerciseParameters.Uuid = QUuid::createUuid( ).toByteArray( );
	Helicopter->instructor->ExerciseParameters.directionsNumber = 0;
	Helicopter->instructor->ExerciseParameters.backfire = false;
	Helicopter->instructor->ExerciseParameters.selectedKit = 0;
	Helicopter->instructor->ExerciseParameters.selectedMode = 0;
	Helicopter->instructor->ExerciseParameters.distance2screen = 0;
	Helicopter->instructor->ExerciseParameters.PacketReady = true;
	Helicopter->instructor->ExerciseReStart( );
	Helicopter->instructor->UnloadAmmo( );
	
	Helicopter->instructor->Environment.unixtime = 1669359535;
	Helicopter->instructor->Environment.temperature = Helicopter->instructor->weather.temp;
	Helicopter->instructor->Environment.cloudsGrade = Helicopter->instructor->weather.cloud;
	Helicopter->instructor->Environment.bottomCloudsLevel = 800;
	Helicopter->instructor->Environment.topCloudsLevel = 1200;
	Helicopter->instructor->Environment.moistureLevel = false;
	Helicopter->instructor->Environment.windSpeed = Helicopter->instructor->weather.wind_speed;
	Helicopter->instructor->Environment.windDirection = Helicopter->instructor->weather.wind_dir;
	Helicopter->instructor->Environment.visibility = Helicopter->instructor->weather.visibility;
	Helicopter->instructor->Environment.verticalWindSpeed = Helicopter->instructor->weather.vws;
	Helicopter->instructor->Environment.windBlows = Helicopter->instructor->weather.wb;
	Helicopter->instructor->Environment.pressure = Helicopter->instructor->weather.pressure;
	Helicopter->instructor->Environment.snowLevel = Helicopter->instructor->weather.snow;
	Helicopter->instructor->Environment.camNumber = Helicopter->instructor->weather.cam;
	Helicopter->instructor->Environment.fog = 0;
	Helicopter->instructor->Environment.amount_precipitation = 0;
	Helicopter->instructor->Environment.intensity_precipitation = 0;
	Helicopter->instructor->Environment.intensity_vpp = 50;
	Helicopter->instructor->Environment.PacketReady = true;
}

void Helicopter_MainWindow::ExStop( )
{
	qDebug( ) << __FUNCTION__;
	Helicopter->instructor->ExerciseParameters.ReStart = false;
	Helicopter->instructor->ExerciseParameters.Pause = false;
	Helicopter->instructor->ExerciseParameters.Stop = true;
	Helicopter->instructor->ExerciseParameters.ExerciseNum = 0;
	Helicopter->instructor->ExerciseParameters.MapType = 0;
	Helicopter->instructor->ExerciseParameters.night = false;
	Helicopter->instructor->ExerciseParameters.season = static_cast<quint8>( Seasons::Summer );
	Helicopter->instructor->ExerciseParameters.AutoloadingSystem = false;
	Helicopter->instructor->ExerciseParameters.PacketReady = true;
	Helicopter->instructor->ExerciseStop( );
	Helicopter->instructor->UnloadAmmo( );
}
void Helicopter_MainWindow::on_rb8001_clicked( bool checked )
{
	if ( checked )
	{
		Helicopter->tcpclient_imager_->ConnectPort( 8001 );
		ui->ExStartButton_1->setEnabled( false );
		ui->ExStartButton_2->setEnabled( true );
		ui->ExStartButton_3->setEnabled( true );
		ui->ExStartButton_4->setEnabled( false );
	}
}
void Helicopter_MainWindow::on_rb10000_clicked( bool checked )
{
	if ( checked )
	{
		Helicopter->tcpclient_imager_->ConnectPort( 10000 );
		Helicopter->tcpclient_imager_1->ConnectPort(10000);
		Helicopter->tcpclient_imager_2->ConnectPort(10000);
		Helicopter->tcpclient_imager_3->ConnectPort(10000);
		Helicopter->tcpclient_imager_4->ConnectPort(10000);
		Helicopter->tcpclient_imager_5->ConnectPort(10000);
		Helicopter->tcpclient_imager_6->ConnectPort(10000);
		Helicopter->tcpclient_imager_stend->ConnectPort(10000);
		ui->ExStartButton_1->setEnabled( true );
		ui->ExStartButton_2->setEnabled( false );
		ui->ExStartButton_3->setEnabled( false );
		ui->ExStartButton_4->setEnabled( true );
	}
}

void Helicopter_MainWindow::on_ExStartButton_1_clicked( )
{
	qDebug( ) << "Запуск упражнения в ручном режиме, Актобе";
    //Helicopter->Stop();
	Helicopter->SetAirPort( Helicopter->Aktube );
    //Helicopter->lpb.ki_206.freq( 113.4f );
    //Helicopter->rpb.ki_206.freq( 113.4f );
	ExStart( 9 );
	Helicopter->imager->ImagerWork = true;
    ui->ExStartButton_1->setEnabled( false );
    ui->ExStartButton_2->setEnabled( false );
    ui->ExStartButton_3->setEnabled( false );
    ui->ExStartButton_4->setEnabled( false );
    ui->ExStopButton->setEnabled( true );
    ui->rb8001->setEnabled( false );
    ui->rb10000->setEnabled( false );
}

void Helicopter_MainWindow::on_ExStartButton_2_clicked( )
{
	qDebug( ) << "Запуск упражнения в ручном режиме, Алматы";
    //Helicopter->Stop();
	Helicopter->SetAirPort( Helicopter->Almaty );
    Helicopter->lpb.ki_206.freq( 112.3f );
    Helicopter->rpb.ki_206.freq( 112.3f );
	ExStart( 10 );
	Helicopter->imager->ImagerWork = true;
    ui->ExStartButton_1->setEnabled( false );
    ui->ExStartButton_2->setEnabled( false );
    ui->ExStartButton_3->setEnabled( false );
    ui->ExStartButton_4->setEnabled( false );
    ui->ExStopButton->setEnabled( true );
    ui->rb8001->setEnabled( false );
    ui->rb10000->setEnabled( false );
}

void Helicopter_MainWindow::on_ExStartButton_3_clicked( )
{
	qDebug( ) << "Запуск упражнения в ручном режиме, Боралдай";
	Helicopter->SetAirPort( Helicopter->Boralday );
	ExStart( 10 );
	Helicopter->imager->ImagerWork = true;
    ui->ExStartButton_1->setEnabled( false );
    ui->ExStartButton_2->setEnabled( false );
    ui->ExStartButton_3->setEnabled( false );
    ui->ExStartButton_4->setEnabled( false );
    ui->ExStopButton->setEnabled( true );
    ui->rb8001->setEnabled( false );
    ui->rb10000->setEnabled( false );
}

void Helicopter_MainWindow::on_ExStartButton_4_clicked( )
{
	qDebug( ) << "Запуск упражнения в ручном режиме, Хлебодар";
	//Helicopter->SetAirPort( Helicopter->Hlebodar );
	Helicopter->SetAirPort( Helicopter->Balkhash );
	Helicopter->imager->ImagerWork = true;
	ExStart( 8 );
    ui->ExStartButton_1->setEnabled( false );
    ui->ExStartButton_2->setEnabled( false );
    ui->ExStartButton_3->setEnabled( false );
    ui->ExStartButton_4->setEnabled( false );
    ui->ExStopButton->setEnabled( true );
    ui->rb8001->setEnabled( false );
    ui->rb10000->setEnabled( false );
}

void Helicopter_MainWindow::on_ExStopButton_clicked( )
{
	qDebug( ) << "Остановка упражнения в ручном режиме";
	//Helicopter->Stop( );
	ExStop( );
    if ( Helicopter->tcpclient_imager_->GetPort( ) == 10000 )
    {
        ui->rb10000->setChecked( true );
        ui->ExStartButton_1->setEnabled( true );
        ui->ExStartButton_2->setEnabled( false );
        ui->ExStartButton_3->setEnabled( false );
        ui->ExStartButton_4->setEnabled( true );
    }
    else
    {
        ui->rb8001->setChecked( true );
        ui->ExStartButton_1->setEnabled( false );
        ui->ExStartButton_2->setEnabled( true );
        ui->ExStartButton_3->setEnabled( true );
        ui->ExStartButton_4->setEnabled( false );
    }
    ui->ExStopButton->setEnabled( false );
    ui->rb8001->setEnabled( true );
    ui->rb10000->setEnabled( true );
}

void Helicopter_MainWindow::on_Save_clicked( )
{
	Helicopter->_BoardSystem->b_save = ui->Save->isChecked( );
	if ( Helicopter->_BoardSystem->b_save == true )
		ui->Save->setText( "Stop save" );
	else
		ui->Save->setText( "Save" );
}


/* 
список пресетов по погоде
лето день ясно
лето день облачно 5
лето ночь ясно
зима день облачно 10
*/

void Helicopter_MainWindow::on_weather_activated( int i )
{
	Weather _weather( i );
	Helicopter->instructor->weather = _weather;

	ui->Temp->setValue( Helicopter->instructor->weather.temp );
	ui->Press->setValue( Helicopter->instructor->weather.pressure );

	//    Helicopter_->json_weather = Helicopter_->instructor->weather_.json();
}

void Helicopter_MainWindow::on_Projector_ON_clicked( )
{
	Helicopter->pj_->powerOn( );

}
void Helicopter_MainWindow::on_Projector_OFF_clicked( )
{
	Helicopter->pj_->powerOff( );
}

void Helicopter_MainWindow::slotUpdateNetStat( )
{
	if ( Helicopter->can->CAN_Ready( ) )
	{
		if ( Helicopter->can->ReadBuffer( 0 ) > 0 )
		{
			ui->label_CANbuffer_0->setStyleSheet( "color: red" );
		} 
		else
		{
			ui->label_CANbuffer_0->setStyleSheet( "color: black" );
		}
		ui->label_CANbuffer_0->setText( "Buffer 0: " + QString::number( Helicopter->can->ReadBuffer( 0 ) ) );
        //ui->label_CANbuffer_0->adjustSize();
		if ( Helicopter->can->ReadBuffer( 1 ) > 0 )
		{
			ui->label_CANbuffer_1->setStyleSheet( "color: red" );
		} 
		else
		{
			ui->label_CANbuffer_1->setStyleSheet( "color: black" );
		}
		ui->label_CANbuffer_1->setText( "Buffer 1: " + QString::number( Helicopter->can->ReadBuffer( 1 ) ) );
        //ui->label_CANbuffer_1->adjustSize();
		ui->label_TXcnt->setStyleSheet( "color: black" );
		ui->label_TXcnt->setText( "TX count: " + QString::number( Helicopter->can->TXpacktsCNT( ) ) );
        //ui->label_TXcnt->adjustSize();
		ui->label_RXcnt->setStyleSheet( "color: black" );
		ui->label_RXcnt->setText( "RX count: " + QString::number( Helicopter->can->RXpacktsCNT( ) ) );
        //ui->label_RXcnt->adjustSize();
		ui->label_TXpps->setStyleSheet( "color: black" );
		ui->label_TXpps->setText( "TX pps: " + QString::number( Helicopter->can->TXpacktsPPS( ) ) );
        //ui->label_TXpps->adjustSize();
		ui->label_RXpps->setStyleSheet( "color: black" );
		ui->label_RXpps->setText( "RX pps: " + QString::number( Helicopter->can->RXpacktsPPS( ) ) );
        //ui->label_RXpps->adjustSize();

        auto mean_durations = LogDuration<>::mean_duration( );
        ui->lblTx->setStyleSheet( "color: black" );
        ui->lblRx->setStyleSheet( "color: black" );
        for ( const auto& [id, duration] : mean_durations ) {
            if ( id == "TX" )
            {
                ui->lblTx->setText( QString( "%1: %2 us" ).arg( id ).arg( QString::number( duration, 'f', 3 ) ) );
            }
            if ( id == "RX" )
            {
                ui->lblRx->setText( QString( "%1: %2 us" ).arg( id ).arg( QString::number( duration, 'f', 3 ) ) );
            }
        }

        
		if ( Helicopter->can->RXpacktsPPS( ) != 0 )
		{
            ui->NetStatGroupBox->setTitle("CAN : Conection OK");
			ui->NetStatGroupBox->setStyleSheet( "color: green" );
		} 
		else
		{
            ui->NetStatGroupBox->setTitle("CAN : Conection Err");
			ui->NetStatGroupBox->setStyleSheet( "color: red" );
		}
	}
	ui->labelX->setText( "X: " + QString::number( Helicopter->PC.f_pitch ) );
	ui->labelY->setText( "Y: " + QString::number( Helicopter->PC.f_roll ) );
	ui->labelCorLEng->setText( "Cor L Eng: " + QString::number( Helicopter->PC.f_eng1 ) );
	ui->labelCorREng->setText( "Cor R Eng: " + QString::number( Helicopter->PC.f_eng2 ) );
	ui->labelFosh->setText( "Fosh: " + QString::number( Helicopter->PC.f_Fosh ) );
	ui->labelEcor->setText( "Ecor: " + QString::number( Helicopter->PC.f_Ecor ) );
	ui->labelXp->setText( "Pedals: " + QString::number( Helicopter->PC.f_yawl ) );
	ui->labelBreak->setText( "Break: " + QString::number( Helicopter->PC.f_break ) );

	if ( Helicopter->tcpclient_imager_->ClientConnected( ) )
		ui->lblImagerStatus->setStyleSheet( "background-color: green" );
	else
		ui->lblImagerStatus->setStyleSheet( "background-color: red" );
	if (Helicopter->tcpclient_imager_1->ClientConnected())
		ui->lblImagerStatus_1->setStyleSheet("background-color: green");
	else
		ui->lblImagerStatus_1->setStyleSheet("background-color: red");
	if (Helicopter->tcpclient_imager_2->ClientConnected())
		ui->lblImagerStatus_2->setStyleSheet("background-color: green");
	else
		ui->lblImagerStatus_2->setStyleSheet("background-color: red");
	if (Helicopter->tcpclient_imager_3->ClientConnected())
		ui->lblImagerStatus_3->setStyleSheet("background-color: green");
	else
		ui->lblImagerStatus_3->setStyleSheet("background-color: red");
	if (Helicopter->tcpclient_imager_4->ClientConnected())
		ui->lblImagerStatus_4->setStyleSheet("background-color: green");
	else
		ui->lblImagerStatus_4->setStyleSheet("background-color: red");
	if (Helicopter->tcpclient_imager_5->ClientConnected())
		ui->lblImagerStatus_5->setStyleSheet("background-color: green");
	else
		ui->lblImagerStatus_5->setStyleSheet("background-color: red");
	if (Helicopter->tcpclient_imager_6->ClientConnected())
		ui->lblImagerStatus_6->setStyleSheet("background-color: green");
	else
		ui->lblImagerStatus_6->setStyleSheet("background-color: red");
	if (Helicopter->tcpclient_imager_stend->ClientConnected())
		ui->lblImagerStatus_stend->setStyleSheet("background-color: green");
	else
		ui->lblImagerStatus_stend->setStyleSheet("background-color: red");

	if ( Helicopter->tcpclient_instructor->ClientConnected( ) )
		ui->lblInstructorStatus->setStyleSheet( "background-color: green" );
	else
		ui->lblInstructorStatus->setStyleSheet( "background-color: red" );

	if(Helicopter->tcpclient_scriptserver->ClientConnected()) 
	   ui->lblScriptServerStatus->setStyleSheet("background-color: green");
	else 
	   ui->lblScriptServerStatus->setStyleSheet("background-color: red");

	if ( Helicopter->tcpclient_OU_->ClientConnected( ) )
		ui->lblOUStatus->setStyleSheet( "background-color: green" );
	else
		ui->lblOUStatus->setStyleSheet( "background-color: red" );

	if ( Helicopter->tcpclient_LPP_->ClientConnected( ) )
		ui->lblLPPStatus->setStyleSheet( "background-color: green" );
	else
		ui->lblLPPStatus->setStyleSheet( "background-color: red" );

	if ( Helicopter->tcpclient_RPP_->ClientConnected( ) )
		ui->lblRPPStatus->setStyleSheet( "background-color: green" );
	else
		ui->lblRPPStatus->setStyleSheet( "background-color: red" );
    if (Helicopter->serverInstructor->ClientConnected())
        ui->lblStand->setStyleSheet("background-color: green");
    else
        ui->lblStand->setStyleSheet("background-color: red");
	//   if(Helicopter_->tcpclient_screenitems->ClientConnected()) 
	//ui->lblScreenItemsStatus->setStyleSheet("background-color: green");
 //   else 
	//ui->lblScreenItemsStatus->setStyleSheet("background-color: red");

	if ( Helicopter->joystick_->JoystickActive( ) )
		ui->lblJoystickStatus->setStyleSheet( "background-color: green" );
	else
		ui->lblJoystickStatus->setStyleSheet( "background-color: red" );

	if ( Helicopter->tcpclient_OC_->ClientConnected( ) )
		ui->lblObjControlStatus->setStyleSheet( "background-color: green" );
	else
		ui->lblObjControlStatus->setStyleSheet( "background-color: red" );

	if ( Helicopter->Model_->Inited( ) )
		ui->lblModelStatus->setStyleSheet( "background-color: green" );
	else
		ui->lblModelStatus->setStyleSheet( "background-color: red" );

	ui->lst_Fault->clear( );
	ui->lst_Fault->addItem( Helicopter->server_->CANBusStateMsg );

	//ui->lWeight->setNum(Helicopter_->instructor->weight);
	//ui->lFuel->setNum(Helicopter_->instructor->fuel);
	//ui->lCT->setNum(Helicopter_->instructor->ct);
    using namespace std::literals;
    auto mean_durations = LogDuration<>::mean_duration( );
    QString statusText;
    for ( const auto& [id, duration] : mean_durations ) {
        statusText += QString( "%1: %2 us | " ).arg( id ).arg( QString::number( duration, 'f', 3 ) );
    }
    statusBar( )->showMessage( statusText );
	LogDuration<>::resetAll( );
}
