#pragma warning (push)
#pragma warning(disable: 4275)
#include "Helicopter_core.h"
#include "Helicopter_server.h"
#include "Helicopter_can.h"
#include "Helicopter_imager.h"
#include "Helicopter_imager_tcp_client.h"
#include "Helicopter_imager_tcp_client_sfera.h"
#include "Helicopter_controls.h"
#include "Helicopter_joystick.h"
#include "Helicopter_imitator_tcp_client.h"
#include "Helicopter_instructor_tcp_client.h"
#include "Helicopter_OC_tcp_client.h"
#include "Helicopter_script.h"
#include "Helicopter_script_tcp_client.h"
#include "Helicopter_serviceerrors.h"
#include "Helicopter_TargetsManager.h"
#include "AviaMotion_TCP_client.h"
#include "Helicopter_Rita_TCP_Client.h"
#include "Helicopter_SPU_Left_Pilot_TCP_Client.h"
#include "Helicopter_SPU_Right_Pilot_TCP_Client.h"
#include "Helicopter_SPU_Bort_Teh_TCP_Client.h"
#include "FlightRecorder/flightrecorder.h"
#include "FlightRecorder/fr_controls.h"
#include "autopilot.h"
#include "Common/ServerInstructor.h"
#pragma warning (pop)

Helicopter_core::Helicopter_core( QMainWindow* parent ) : QObject( parent )
{
    qDebug() << __FUNCTION__;
    _ini = QSharedPointer<TSettingsFile>(new TSettingsFile);
    log_ = QSharedPointer<TLogging>(new TLogging(_ini));
    log_->WriteLog(LogEvents::Application_start, "");

    AP_ = QSharedPointer<Autopilot>(new Autopilot);
    pj_ = QSharedPointer<Helicopter_Projectors>(new Helicopter_Projectors(_ini));

    audio_ = QSharedPointer<Helicopter_audio>(new Helicopter_audio, &QObject::deleteLater);
    connect( &thread_audio, SIGNAL( started( ) ), audio_.data(), SLOT( run( ) ) );
    audio_->moveToThread(&thread_audio);
    thread_audio.start();
    qDebug() << "Server thread" << thread() << "Audio thread" << audio_->thread( );

    server_ = QSharedPointer<Helicopter_Server>(new Helicopter_Server( this ));  //server_ = new Helicopter_Server();
    server_->Init();
    can = QSharedPointer<Helicopter_CAN>(new Helicopter_CAN(_ini, log_));
    imager = QSharedPointer<Helicopter_Imager>(new Helicopter_Imager);
    instructor = QSharedPointer<Helicopter_Instructor>(new Helicopter_Instructor);
    joystick_ = QSharedPointer<Helicopter_Joystick>(new Helicopter_Joystick(parent, log_));

    errors = QSharedPointer<Helicopter_ServiceErrors>( new Helicopter_ServiceErrors( this ));
    targetsmanager = QSharedPointer<Helicopter_TargetsManager>( new Helicopter_TargetsManager( this ));

    _BoardSystem = new BoardSystem(this); // из-за отладки не удобно испольщовать SharedPtr
    Model_ = QSharedPointer<Helicopter_Model>(new Helicopter_Model(this));    // Model_ = new Helicopter_Model(this);
    connect( &thread_model, SIGNAL( started( ) ), Model_.data(), SLOT( run( ) ) );
    Model_->moveToThread( &thread_model );
    thread_model.start( );
    scriptserver = QSharedPointer<Helicopter_ScriptServer>(new Helicopter_ScriptServer( ));

    tcpclient_imager_ = QSharedPointer<Helicopter_TCP_Client_Imager>(new Helicopter_TCP_Client_Imager(this, 0));
    tcpclient_imager_1 = QSharedPointer<Helicopter_TCP_Client_Imager_Sfera>(new Helicopter_TCP_Client_Imager_Sfera(this, 1));
    tcpclient_imager_2 = QSharedPointer<Helicopter_TCP_Client_Imager_Sfera>(new Helicopter_TCP_Client_Imager_Sfera(this, 2));
    tcpclient_imager_3 = QSharedPointer<Helicopter_TCP_Client_Imager_Sfera>(new Helicopter_TCP_Client_Imager_Sfera(this, 3));
    tcpclient_imager_4 = QSharedPointer<Helicopter_TCP_Client_Imager_Sfera>(new Helicopter_TCP_Client_Imager_Sfera(this, 4));
    tcpclient_imager_5 = QSharedPointer<Helicopter_TCP_Client_Imager_Sfera>(new Helicopter_TCP_Client_Imager_Sfera(this, 5));
    tcpclient_imager_6 = QSharedPointer<Helicopter_TCP_Client_Imager_Sfera>(new Helicopter_TCP_Client_Imager_Sfera(this, 6));
    tcpclient_imager_stend = QSharedPointer<Helicopter_TCP_Client_Imager>(new Helicopter_TCP_Client_Imager(this, 7));

    tcpclient_instructor = QSharedPointer<Helicopter_TCP_Client_Instructor>(new Helicopter_TCP_Client_Instructor(this));
    tcpclient_OU_  = QSharedPointer<Helicopter_OU_TCP_Client>(new Helicopter_OU_TCP_Client(this, 0));   
    tcpclient_LPP_ = QSharedPointer<Helicopter_OU_TCP_Client>(new Helicopter_OU_TCP_Client(this, 1));   
    tcpclient_RPP_ = QSharedPointer<Helicopter_OU_TCP_Client>(new Helicopter_OU_TCP_Client(this, 2));   
    tcpclient_OC_  = QSharedPointer<Helicopter_OC_TCP_Client>(new Helicopter_OC_TCP_Client(this));      
    tcpclient_Rita = QSharedPointer<Helicopter_Rita_TCP_Client>(new Helicopter_Rita_TCP_Client(this));
    tcpclient_SPU_BT = QSharedPointer<Helicopter_SPU_Bort_Teh_TCP_Client>(new Helicopter_SPU_Bort_Teh_TCP_Client(this));
    tcpclient_SPU_LP = QSharedPointer<Helicopter_SPU_Left_Pilot_TCP_Client>(new Helicopter_SPU_Left_Pilot_TCP_Client(this));
    tcpclient_SPU_RP = QSharedPointer<Helicopter_SPU_Right_Pilot_TCP_Client>(new Helicopter_SPU_Right_Pilot_TCP_Client(this));
    tcpclient_scriptserver = QSharedPointer<Helicopter_TCP_Client_ScriptServer>(new Helicopter_TCP_Client_ScriptServer(this));
    controls_ = QSharedPointer<Helicopter_controls>(new Helicopter_controls(this)); // тут же создается OnBoard
    serverInstructor = QSharedPointer<ServerInstructor>( new ServerInstructor( this ) );
    if ( _ini->SimulatorType( ) == 19 )
    {
        qDebug( ) << "listen Instructor port";
        if ( !serverInstructor->listen( QHostAddress::Any, 40002 ) )
        {
            // Если не удается прослушить заданый порт, удасяем локальный сервер инструктора
            // это надо проверить на логику
            qDebug( ) << "error starting server Instructor";
            serverInstructor->deleteLater( );
        }
    }

    // сигнал из модели об завершении расчета
    connect( Model_.data( ), SIGNAL( send2vs( ) ), tcpclient_imager_.data( ), SLOT( On_SendTcpData( ) ), Qt::QueuedConnection );
    connect( Model_.data( ), SIGNAL( send2vs( ) ), tcpclient_imager_1.data( ), SLOT( On_SendTcpData( ) ), Qt::QueuedConnection );
    connect( Model_.data( ), SIGNAL( send2vs( ) ), tcpclient_imager_2.data( ), SLOT( On_SendTcpData( ) ), Qt::QueuedConnection );
    connect( Model_.data( ), SIGNAL( send2vs( ) ), tcpclient_imager_3.data( ), SLOT( On_SendTcpData( ) ), Qt::QueuedConnection );
    connect( Model_.data( ), SIGNAL( send2vs( ) ), tcpclient_imager_4.data( ), SLOT( On_SendTcpData( ) ), Qt::QueuedConnection );
    connect( Model_.data( ), SIGNAL( send2vs( ) ), tcpclient_imager_5.data( ), SLOT( On_SendTcpData( ) ), Qt::QueuedConnection );
    connect( Model_.data( ), SIGNAL( send2vs( ) ), tcpclient_imager_6.data( ), SLOT( On_SendTcpData( ) ), Qt::QueuedConnection );
    connect( Model_.data( ), SIGNAL( send2vs( ) ), tcpclient_imager_stend.data( ), SLOT( On_SendTcpData( ) ), Qt::QueuedConnection );

    connect( Model_.data( ), SIGNAL( send2vs( ) ), tcpclient_OU_.data(),      SLOT( slotSendTcpData( ) ), Qt::QueuedConnection );
    // сигнал о готовности визуалки для инструктора
    connect( tcpclient_imager_.data(), SIGNAL( imagerReady( ) ), tcpclient_instructor.data(), SLOT( On_SendImagerReady( ) ) );

    // TODO разкоментировать после изменения исструктора
        //connect(tcpclient_imager_1.data(), SIGNAL(imagerReady()), tcpclient_instructor.data(), SLOT(On_SendImagerReady()));
        //connect(tcpclient_imager_2.data(), SIGNAL(imagerReady()), tcpclient_instructor.data(), SLOT(On_SendImagerReady()));
        //connect(tcpclient_imager_3.data(), SIGNAL(imagerReady()), tcpclient_instructor.data(), SLOT(On_SendImagerReady()));
        //connect(tcpclient_imager_4.data(), SIGNAL(imagerReady()), tcpclient_instructor.data(), SLOT(On_SendImagerReady()));
        //connect(tcpclient_imager_5.data(), SIGNAL(imagerReady()), tcpclient_instructor.data(), SLOT(On_SendImagerReady()));
        //connect(tcpclient_imager_6.data(), SIGNAL(imagerReady()), tcpclient_instructor.data(), SLOT(On_SendImagerReady()));
        //connect(tcpclient_imager_stend.data(), SIGNAL(imagerReady()), tcpclient_instructor.data(), SLOT(On_SendImagerReady()));
    
    // сигналы об изменении боепримасоы между визуалкой и инструктором
    connect( tcpclient_imager_.data( ),    SIGNAL( AmmoOut( ) ),     tcpclient_instructor.data( ), SLOT( SendArmStateDataPacket( ) ), Qt::QueuedConnection );

        // TODO разкоментировать после измененния инструктора
        //connect(tcpclient_imager_1.data(), SIGNAL(AmmoOut()), tcpclient_instructor.data(), SLOT(SendArmStateDataPacket()), Qt::QueuedConnection);
        //connect(tcpclient_imager_2.data(), SIGNAL(AmmoOut()), tcpclient_instructor.data(), SLOT(SendArmStateDataPacket()), Qt::QueuedConnection);
        //connect(tcpclient_imager_3.data(), SIGNAL(AmmoOut()), tcpclient_instructor.data(), SLOT(SendArmStateDataPacket()), Qt::QueuedConnection);
        //connect(tcpclient_imager_4.data(), SIGNAL(AmmoOut()), tcpclient_instructor.data(), SLOT(SendArmStateDataPacket()), Qt::QueuedConnection);
        //connect(tcpclient_imager_5.data(), SIGNAL(AmmoOut()), tcpclient_instructor.data(), SLOT(SendArmStateDataPacket()), Qt::QueuedConnection);
        //connect(tcpclient_imager_6.data(), SIGNAL(AmmoOut()), tcpclient_instructor.data(), SLOT(SendArmStateDataPacket()), Qt::QueuedConnection);
        //connect(tcpclient_imager_stend.data(), SIGNAL(AmmoOut()), tcpclient_instructor.data(), SLOT(SendArmStateDataPacket()), Qt::QueuedConnection);

    connect( tcpclient_instructor.data( ), SIGNAL( AmmoIn( ) ),      tcpclient_imager_.data( ),    SLOT( SendAmmoDataPacket( ) ), Qt::QueuedConnection );
    connect(tcpclient_instructor.data(), SIGNAL(AmmoIn()), tcpclient_imager_1.data(), SLOT(SendAmmoDataPacket()), Qt::QueuedConnection);
    connect(tcpclient_instructor.data(), SIGNAL(AmmoIn()), tcpclient_imager_2.data(), SLOT(SendAmmoDataPacket()), Qt::QueuedConnection);
    connect(tcpclient_instructor.data(), SIGNAL(AmmoIn()), tcpclient_imager_3.data(), SLOT(SendAmmoDataPacket()), Qt::QueuedConnection);
    connect(tcpclient_instructor.data(), SIGNAL(AmmoIn()), tcpclient_imager_4.data(), SLOT(SendAmmoDataPacket()), Qt::QueuedConnection);
    connect(tcpclient_instructor.data(), SIGNAL(AmmoIn()), tcpclient_imager_5.data(), SLOT(SendAmmoDataPacket()), Qt::QueuedConnection);
    connect(tcpclient_instructor.data(), SIGNAL(AmmoIn()), tcpclient_imager_6.data(), SLOT(SendAmmoDataPacket()), Qt::QueuedConnection);
    connect(tcpclient_instructor.data(), SIGNAL(AmmoIn()), tcpclient_imager_stend.data(), SLOT(SendAmmoDataPacket()), Qt::QueuedConnection);
    // сигналы между инструктором и стендом
    connect( serverInstructor.data(), &ServerInstructor::StandPacket, tcpclient_instructor.data(), &Helicopter_TCP_Client_Instructor::On_StandPacket, Qt::AutoConnection);
    connect( tcpclient_instructor.data(), &Helicopter_TCP_Client_Instructor::InstructorPacket, serverInstructor.data(), &ServerInstructor::write, Qt::AutoConnection);
    // сигналы на управление работой модели
    connect( Model_.data( ), &Helicopter_Model::crash, tcpclient_imager_.data( ), &Helicopter_TCP_Client_Imager::SendExitDataPacket, Qt::QueuedConnection );
    connect(Model_.data(), &Helicopter_Model::crash, tcpclient_imager_1.data(), &Helicopter_TCP_Client_Imager_Sfera::SendExitDataPacket, Qt::QueuedConnection);
    connect(Model_.data(), &Helicopter_Model::crash, tcpclient_imager_2.data(), &Helicopter_TCP_Client_Imager_Sfera::SendExitDataPacket, Qt::QueuedConnection);
    connect(Model_.data(), &Helicopter_Model::crash, tcpclient_imager_3.data(), &Helicopter_TCP_Client_Imager_Sfera::SendExitDataPacket, Qt::QueuedConnection);
    connect(Model_.data(), &Helicopter_Model::crash, tcpclient_imager_4.data(), &Helicopter_TCP_Client_Imager_Sfera::SendExitDataPacket, Qt::QueuedConnection);
    connect(Model_.data(), &Helicopter_Model::crash, tcpclient_imager_5.data(), &Helicopter_TCP_Client_Imager_Sfera::SendExitDataPacket, Qt::QueuedConnection);
    connect(Model_.data(), &Helicopter_Model::crash, tcpclient_imager_6.data(), &Helicopter_TCP_Client_Imager_Sfera::SendExitDataPacket, Qt::QueuedConnection);
    connect(Model_.data(), &Helicopter_Model::crash, tcpclient_imager_stend.data(), &Helicopter_TCP_Client_Imager::SendExitDataPacket, Qt::QueuedConnection);
    // сигналы тестирования
    //connect( tcpclient_instructor.data( ), &Helicopter_TCP_Client_Instructor::RunRecords, tcpclient_imager_.data( ), SLOT( SendAmmoDataPacket( ) ), Qt::QueuedConnection );

    // сигналы управления проигыванием записей
    connect( tcpclient_instructor.data( ), &Helicopter_TCP_Client_Instructor::RunRecords, Model_.data( ), &Helicopter_Model::play, Qt::QueuedConnection );
    connect( tcpclient_instructor.data( ), &Helicopter_TCP_Client_Instructor::PlayPos, Model_.data( ), &Helicopter_Model::playpos, Qt::QueuedConnection );
    connect( tcpclient_instructor.data( ), &Helicopter_TCP_Client_Instructor::PlayMode, Model_.data( ), &Helicopter_Model::playmode, Qt::QueuedConnection );
    connect( tcpclient_instructor.data( ), &Helicopter_TCP_Client_Instructor::PlaySpeed,  Model_.data( ), &Helicopter_Model::playspeed, Qt::QueuedConnection );
    connect( tcpclient_instructor.data( ), &Helicopter_TCP_Client_Instructor::Enviroment,  Model_.data( ), &Helicopter_Model::enviroment, Qt::QueuedConnection );
    connect( tcpclient_instructor.data( ), &Helicopter_TCP_Client_Instructor::Forward,  Model_.data( ), &Helicopter_Model::forward, Qt::QueuedConnection );
    connect( tcpclient_instructor.data( ), &Helicopter_TCP_Client_Instructor::Exercise,  Model_.data( ), &Helicopter_Model::exercise, Qt::QueuedConnection );
    
    if ( can->BoardCount( ) > 0 )
    {
        can->Start( );
    }
}

Helicopter_core::~Helicopter_core()
{
    qDebug() << __FUNCTION__;
    disconnect( SIGNAL( send2vs( ) ) );
    thread_audio.quit();
    thread_audio.wait();
}

// запуск симуляции с началом передачи на всех клиентов полученных данных, запуск таймеров для TCPSend
void Helicopter_core::Start()
{
    qDebug() << __FUNCTION__;

    this->Model_->Start();
    this->controls_->Start();
    this->joystick_->Start();
    this->_BoardSystem->Start();
    this->tcpclient_OU_->Start();
    this->tcpclient_imager_->Start();
    this->tcpclient_imager_1->Start();
    this->tcpclient_imager_2->Start();
    this->tcpclient_imager_3->Start();
    this->tcpclient_imager_4->Start();
    this->tcpclient_imager_5->Start();
    this->tcpclient_imager_6->Start();
    this->tcpclient_imager_stend->Start();
    this->tcpclient_OC_->Start();
    this->tcpclient_LPP_->Start();
    this->tcpclient_RPP_->Start();
    this->tcpclient_Rita->Start();
    this->tcpclient_SPU_BT->Start();
    this->tcpclient_SPU_LP->Start();
    this->tcpclient_SPU_RP->Start();
//    this->FR_->start();
}

void Helicopter_core::Stop()
{
    qDebug() << __FUNCTION__;
    this->Model_->Stop();
    this->controls_->Stop();
    this->joystick_->Stop();
    this->_BoardSystem->Stop();
    this->tcpclient_OU_->Stop();
    this->tcpclient_imager_->Stop();
    this->tcpclient_imager_1->Stop();
    this->tcpclient_imager_2->Stop();
    this->tcpclient_imager_3->Stop();
    this->tcpclient_imager_4->Stop();
    this->tcpclient_imager_5->Stop();
    this->tcpclient_imager_6->Stop();
    this->tcpclient_imager_stend->Stop();
    this->tcpclient_OC_->Stop();
    this->tcpclient_LPP_->Stop();
    this->tcpclient_RPP_->Stop();
    this->tcpclient_Rita->Stop();
    this->tcpclient_SPU_BT->Stop();
    this->tcpclient_SPU_LP->Stop();
    this->tcpclient_SPU_RP->Stop();
//    this->FR_->stop();
}

void Helicopter_core::SetAirPort(const Location& loc)
{
	runway_ = loc;
    instructor->course = static_cast<float>(runway_.course);
    instructor->StartLatitude = runway_.start_lat;
    instructor->StartLongitude = runway_.start_lon;

	_BoardSystem->StartLatitude = runway_.start_lat;
	_BoardSystem->StartLongitude = runway_.start_lon;
	_BoardSystem->Latitude = runway_.start_lat;
	_BoardSystem->Longitude = runway_.start_lon;
}

QSharedPointer<Helicopter_audio> Helicopter_core::GetAudio()
{
    return audio_;
}

void Helicopter_core::ResetCanCrash() {
    can->Reset(PC.reset);
}
