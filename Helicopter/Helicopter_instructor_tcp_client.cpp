#include "Helicopter_instructor_tcp_client.h"
#include "Helicopter_imager_tcp_client.h"
#include "Helicopter_imager_tcp_client_sfera.h"
#include "Helicopter_Model.h"
#include "Common/avia_dictionary.h"
#include "Common/ServerInstructor.h"

Helicopter_TCP_Client_Instructor::Helicopter_TCP_Client_Instructor(Helicopter_core* Helicopter)
{
    Helicopter = Helicopter;

    TxInterval = 100;    //  обновление данных для инструктора 0.1 секунда
    hostaddr = Helicopter->_ini->IP_Instructor();
    tcp_port = Helicopter->_ini->PORT_Instructor();
    qDebug() << "Подключение Instructor к " << hostaddr << ":" << tcp_port;

    ConnectToHost(hostaddr, tcp_port);

    TimerTCPSend->setInterval(TxInterval);
    connect(TimerTCPSend,SIGNAL(timeout()),this, SLOT(On_SendTcpData()));
    TimerTCPSend->start();

    connect(this, SIGNAL(signalConnected()), this, SLOT(On_Connected()), Qt::QueuedConnection);
    connect(this, SIGNAL(signalDisconnected()), this, SLOT(On_Disconnected()), Qt::QueuedConnection);
    connect(this, SIGNAL(signalError()), this, SLOT(On_Error()), Qt::QueuedConnection);
    connect(TcpSocket, SIGNAL(readyRead()), this, SLOT(On_ReadTcpData()));

    // тестовые обработчики, что бы убедиться, что сигналы не обрабатываются даже внутри класса
    //connect( this, SIGNAL( RunRecords( QString ) ),this , SLOT( play( QString ) ), Qt::AutoConnection );
    //connect( this, SIGNAL( PlayPos( quint32 ) )   ,this , SLOT( playpos( quint32 ) ), Qt::AutoConnection );
    //connect( this, SIGNAL( PlayMode( quint8 ) )  ,this , SLOT( playmode( quint8 ) ), Qt::AutoConnection );
    //connect( this, SIGNAL( PlaySpeed( float ) ) ,this , SLOT( playspeed( float ) ), Qt::AutoConnection );
    //connect( this, &Helicopter_TCP_Client_Instructor::RunRecords, this, &Helicopter_TCP_Client_Instructor::play, Qt::QueuedConnection );
    //connect( this, &Helicopter_TCP_Client_Instructor::PlayPos,    this, &Helicopter_TCP_Client_Instructor::playpos, Qt::QueuedConnection );
    //connect( this, &Helicopter_TCP_Client_Instructor::PlayMode,   this, &Helicopter_TCP_Client_Instructor::playmode, Qt::QueuedConnection );
    //connect( this, &Helicopter_TCP_Client_Instructor::PlaySpeed,  this, &Helicopter_TCP_Client_Instructor::playspeed , Qt::QueuedConnection );

}

Helicopter_TCP_Client_Instructor::~Helicopter_TCP_Client_Instructor()
{
    TimerTCPSend->stop();
}

void Helicopter_TCP_Client_Instructor::Start()
{
    TimerTCPSend->start();
}

void Helicopter_TCP_Client_Instructor::Stop()
{
    TimerTCPSend->stop();
}

void Helicopter_TCP_Client_Instructor::On_Connected()
{
    qDebug() << "Instructor Connected";
    _Active = ClientConnected();
    Helicopter->server_->ServerReady->PacketReady = true;

    Helicopter->log_->WriteLANLog(LogEvents::TCP_Instructor_connected, "");
}

void Helicopter_TCP_Client_Instructor::On_Disconnected()
{
    qDebug() << "Instructor Disconnected";
    _Active = ClientConnected();
    Helicopter->server_->ServerReady->PacketReady = true;

    Helicopter->log_->WriteLANLog(LogEvents::TCP_Instructor_disconnected, "");
}

void Helicopter_TCP_Client_Instructor::On_StandPacket( QByteArray data )
{
    //qDebug() << __FUNCTION__;
    if (TcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        TcpSocket->write(data);
        TcpSocket->flush();
    }
}

void Helicopter_TCP_Client_Instructor::On_Error()
{
//    //qDebug() << "Instructor Error";
    _Active = ClientConnected();
    Helicopter->server_->ServerReady->PacketReady = true;

    Helicopter->log_->WriteLANLog(LogEvents::TCP_Instructor_Error, socketErrorString);
}

void Helicopter_TCP_Client_Instructor::On_ReadTcpData()
{
    ReadCommonTcpData();
}

void Helicopter_TCP_Client_Instructor::ReadCommonTcpData()
{
    if (TcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        auto header_length = sizeof(Helicopter->instructor->PacketHeader);
        auto availabeBytes = TcpSocket->bytesAvailable();
        while (availabeBytes >= header_length)
        {
			QByteArray header_data;
			QByteArray packet_data;

            if (!processedHeader)
            {
				header_data = TcpSocket->read( header_length );

                QDataStream header_stream(header_data);
                header_stream.setByteOrder(QDataStream::LittleEndian);
                header_stream.setFloatingPointPrecision(QDataStream::SinglePrecision);

                header_stream >> Helicopter->instructor->PacketHeader.Mark;
                header_stream >> Helicopter->instructor->PacketHeader.PacketType;
                header_stream >> Helicopter->instructor->PacketHeader.DataSize;
                processedHeader = true;
				//availabeBytes = availabeBytes - header_length;
				availabeBytes = TcpSocket->bytesAvailable( );
            }

            if ((processedHeader) && (availabeBytes >= Helicopter->instructor->PacketHeader.DataSize))
            {
				packet_data = TcpSocket->read( Helicopter->instructor->PacketHeader.DataSize );
                QDataStream data_stream(packet_data);
                data_stream.setByteOrder(QDataStream::LittleEndian);
                data_stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
                switch (Helicopter->instructor->PacketHeader.PacketType)
                {
                // TODO добавить обработчики стандартных пакетов
                case static_cast<quint32>(InstructorTCPPacketTypes::IR2SR_FailuresDataPacket):
                    GetFailuresDataPacket(data_stream);
                    break;
                case static_cast<quint32>(InstructorTCPPacketTypes::IR2SR_ScriptDataPacket):
                    GetScriptDataPacket(data_stream);
                    break;
                case static_cast<quint32>(InstructorTCPPacketTypes::IR2SR_RunControlDataPacket):
                    GetRunControlDataPacket(data_stream);
                    break;
                case static_cast<quint32>(InstructorTCPPacketTypes::IR2SR_PlayModeDataPacket):
                    GetPlayModeDataPacket(data_stream);
                    break;
                case static_cast<quint32>(InstructorTCPPacketTypes::IR2SR_PlayPosDataPacket):
                    GetPlayPosDataPacket(data_stream);
                    break;
                case static_cast<quint32>(InstructorTCPPacketTypes::IR2SR_PlaySpeedDataPacket):
                    GetPlaySpeedDataPacket(data_stream);
                    break;
                case static_cast<quint32>(InstructorTCPPacketTypes::IR2SR_SoundCamDataPacket):
                    GetSoundCamDataPacket(data_stream);
                    break;
                case static_cast<quint32>(InstructorTCPPacketTypes::IR2SR_ProjectorDataPacket):
                    GetProjectorDataPacket(data_stream);
                    break;
                case static_cast<quint32>(InstructorTCPPacketTypes::IR2SR_ShutterDataPacket):
                    GetShutterDataPacket(data_stream);
                    break;
                case static_cast<quint32>(InstructorTCPPacketTypes::IR2SR_StepCheckDataPacket):
                    GetStepCheckDataPacket(data_stream);
                    break;
                case static_cast<quint32>(InstructorTCPPacketTypes::IR2SR_EnvironmentDataPacket):
                    GetEnvironmentDataPacket(data_stream); 
                    break;
                case static_cast<quint32>(InstructorTCPPacketTypes::IR2SR_ObjectsDataPacket):
                    GetObjectsDataPacket(data_stream); 
                    break;
                case static_cast<quint32>(InstructorTCPPacketTypes::IR2SR_TargetsDataPacket):
                    GetTargetsDataPacket(data_stream); 
                    break;
                case static_cast<quint32>(InstructorTCPPacketTypes::IR2SR_AmmunitionDataPacket):
                    GetAmmunitionDataPacket(data_stream); 
                    break;
                case static_cast<quint32>(InstructorTCPPacketTypes::IR2SR_ExerciseParametersPacket):
                    GetExerciseParametersPacket(data_stream); 
                    break;
                case static_cast<quint32>(InstructorTCPPacketTypes::IR2SR_StatisticsDataPacket):
                    GetStatisticsDataPacket(data_stream);
                    break;
                case static_cast<quint32>(InstructorTCPPacketTypes::IR2SR_ControlDataPacket):
                    GetControlDataPacket(data_stream);
                    break;
                case static_cast<quint32>(InstructorTCPPacketTypes::IR2SR_CommandsDataPacket ):
                    GetCommandsDataPacket(data_stream);
                    break;
                case static_cast<quint32>(InstructorTCPPacketTypes::IR2SR_MessagesDataPacket ):
                    GetMessagesDataPacket(data_stream);
                    break;
                default:
                    qDebug() << "Unknow(data_stream) type ="<< Helicopter->instructor->PacketHeader.PacketType;
                    Helicopter->log_->WriteLANLog(LogEvents::TCP_Instructor_Unknown_PacketType, QString::number(Helicopter->instructor->PacketHeader.PacketType));
                    break;
                }
                processedHeader = false;

				if (Helicopter->instructor->PacketHeader.PacketType != static_cast<quint32>( InstructorTCPPacketTypes::IR2SR_StatisticsDataPacket ))
				{
                    // сообщаем всем кому интересно, что получили пакет от инструктора, по факту он уходит в Стенд, кроме пакета со статистикой, моточасы нам не интересны 
					QByteArray packet;
					packet.append( header_data );
					packet.append( packet_data );
					emit InstructorPacket( packet );  
            }
			}
            availabeBytes = TcpSocket->bytesAvailable();
        }
    }
}

void Helicopter_TCP_Client_Instructor::GetStatisticsDataPacket(QDataStream& _stream)
{
    // Этот пакет спамит постоянно
    StatisticsDataPacket& Statistics = Helicopter->instructor->Statistics;

    Statistics.PacketReady = true;
    _stream >> Statistics.Odometer;
    _stream >> Statistics.EngineClock;
}

void Helicopter_TCP_Client_Instructor::GetControlDataPacket(QDataStream& _stream)
{
    qDebug() << "--<" << __FUNCTION__;
    ControlDataPacket& Control = Helicopter->instructor->Control;

    Control.PacketReady = true;
    _stream >> Control.CommandText;
    qDebug() << Control;
}

void Helicopter_TCP_Client_Instructor::GetCommandsDataPacket(QDataStream& _stream)
{
    qDebug() << "--<" << __FUNCTION__;
    CommandsDataPacket& Commands = Helicopter->instructor->Commands;

    Commands.PacketReady = true;
    _stream >> Commands.MoveForward;
    Helicopter->instructor->commandMoveForward = true;
    emit Forward( );
    qDebug() << Commands;
}

void Helicopter_TCP_Client_Instructor::GetMessagesDataPacket(QDataStream& _stream)
{
    qDebug() << "--<" << __FUNCTION__;
    MessagesDataPacket& Messages = Helicopter->instructor->Messages;

    Messages.PacketReady = true;
    _stream >> Messages.MessageType;
    _stream >> Messages.MessageText;
    qDebug() << Messages;
}

void Helicopter_TCP_Client_Instructor::GetFailuresDataPacket(QDataStream& _stream)
{
    qDebug() << "--<" << __FUNCTION__;
    FailuresDataPacket& Failures = Helicopter->instructor->Failures;

    Failures.PacketReady = true;
    _stream >> Failures.failureType;
    _stream >> Failures.failureActive;
    qDebug() << Failures;
}

void Helicopter_TCP_Client_Instructor::GetScriptDataPacket(QDataStream& _stream)
{
    qDebug() << "--<" << __FUNCTION__;
    _stream >> Helicopter->instructor->script.type_load;
    _stream >> Helicopter->instructor->script.bombs_weight;
    _stream >> Helicopter->instructor->script.aso2b;
    _stream >> Helicopter->instructor->script.consum_tank;
    _stream >> Helicopter->instructor->script.left_tank;
    _stream >> Helicopter->instructor->script.right_tank;
    _stream >> Helicopter->instructor->script.refill;
    _stream >> Helicopter->instructor->script.empty_weight;
    _stream >> Helicopter->instructor->script.crew_weight;
    _stream >> Helicopter->instructor->script.load_weight;
    _stream >> Helicopter->instructor->script.full_weight;
    _stream >> Helicopter->instructor->script.external_cargo;
    _stream >> Helicopter->instructor->script.cable_length;
    _stream >> Helicopter->instructor->script.cargo_weight;
    _stream >> Helicopter->instructor->script.form_cargo;
    _stream >> Helicopter->instructor->script.latitude;
    _stream >> Helicopter->instructor->script.longitude;
    _stream >> Helicopter->instructor->script.height;
    _stream >> Helicopter->instructor->script.curs;
    _stream >> Helicopter->instructor->script.start_delay_time;    // сек
    _stream >> Helicopter->instructor->script.air_radius;   //  Изменение Авиатрафика Радиус, Км
    _stream >> Helicopter->instructor->script.air_density;  //  Плотность, балл
    _stream >> Helicopter->instructor->script.auto_lat;     //  Широта, град
    _stream >> Helicopter->instructor->script.auto_long;    //  Долгота, град
    _stream >> Helicopter->instructor->script.auto_radius;  //  Изменение Автотрафика Радиус, Км
    _stream >> Helicopter->instructor->script.auto_density; //  Плотность, балл

    Helicopter->instructor->fuel = Helicopter->instructor->script.refill;
    Helicopter->instructor->weight = Helicopter->instructor->script.full_weight;
    if ( Helicopter->instructor->weight < 7500 )
    {
        Helicopter->instructor->weight = 7500;
    }
    qDebug() << Helicopter->instructor->script;
}

void Helicopter_TCP_Client_Instructor::GetExerciseParametersPacket(QDataStream& _stream)
{ 
    qDebug() << "--<" << __FUNCTION__;
    ExerciseParametersPacket& ExerciseParameters = Helicopter->instructor->ExerciseParameters;

    ExerciseParameters.PacketReady = true;
    _stream >> ExerciseParameters.ReStart;
    _stream >> ExerciseParameters.Pause;
    _stream >> ExerciseParameters.Stop;
    _stream >> ExerciseParameters.ExerciseNum;
    _stream >> ExerciseParameters.MapType;
    _stream >> ExerciseParameters.night;
    _stream >> ExerciseParameters.season;
    _stream >> ExerciseParameters.Joystick;
    _stream >> ExerciseParameters.AutoloadingSystem;
    _stream >> ExerciseParameters.Uuid;
    _stream >> ExerciseParameters.directionsNumber;
    _stream >> ExerciseParameters.backfire;
    _stream >> ExerciseParameters.selectedKit;
    _stream >> ExerciseParameters.selectedMode;
    _stream >> ExerciseParameters.distance2screen;
    _stream >> ExerciseParameters.run;
    _stream >> ExerciseParameters.lang;

    qDebug() << ExerciseParameters;

    quint32 mode = 0;
    if (ExerciseParameters.ReStart)
    {
        mode = 1;
        Helicopter->instructor->ExerciseReStart();
    }
    if (ExerciseParameters.Stop)
    {
        mode = 2;
        Helicopter->instructor->ExerciseStop();
        Helicopter->instructor->UnloadAmmo();
    }
    if ( ExerciseParameters.Pause )
    {
        mode = 3;
        Helicopter->instructor->exerciseStarted = false;
    }
    switch ( ExerciseParameters.MapType )
    {
    case 8: // Балхаш
        Magnetic::magneticDeclination = 6.0f;
        break;
    case 9: // Актобе
        Magnetic::magneticDeclination = 11.0f;
        break;
    case 10: // Алматы
        Magnetic::magneticDeclination = 5.0f;
        break;
    case 11: // Астана
        Magnetic::magneticDeclination = 10.0f;
        break;
    default:
        Magnetic::magneticDeclination = 10.0f;
        break;
    }
    emit Exercise( mode );
}

void Helicopter_TCP_Client_Instructor::GetAmmunitionDataPacket(QDataStream& _stream)
{
    qDebug() << "--<" << __FUNCTION__;
    AmmunitionDataPacket& AmmunitionInPack = Helicopter->instructor->AmmunitionInPack;

    AmmunitionInPack.PacketReady = true;
    AmmunitionInPack.AmmunitionList.clear();

    _stream >> AmmunitionInPack.packageCount;
    quint8 packageCount = AmmunitionInPack.packageCount;

    while (packageCount > 0)
    {
        Ammunition ammunition;
        _stream >> ammunition.gunType;
        _stream >> ammunition.ammoType;
        _stream >> ammunition.amount;
        AmmunitionInPack.AmmunitionList << ammunition;
        packageCount--;
    }
    qDebug() << AmmunitionInPack;
    Helicopter->instructor->AmmunitionOutPack.AmmunitionList.clear( );
    for ( const auto& AmmoIn : Helicopter->instructor->AmmunitionInPack.AmmunitionList )
    {
        Helicopter->instructor->AmmunitionOutPack.AmmunitionList << AmmoIn;
    }
    Helicopter->instructor->AmmunitionOutPack.packageCount = Helicopter->instructor->AmmunitionOutPack.AmmunitionList.size( );
    // отправим пакет на инструктор, что все получили
    Helicopter->tcpclient_instructor->SendArmStateDataPacket( );
	emit AmmoIn( );

}

void Helicopter_TCP_Client_Instructor::GetTargetsDataPacket(QDataStream& _stream)
{
    qDebug() << "--<" << __FUNCTION__;
    TargetsDataPacket& Targets = Helicopter->instructor->Targets;

    Targets.PacketReady = true;
    Targets.mapTargetsList.clear();

    _stream >> Targets.mapTargetsCount;
    qint32 mapTargetsCount = Targets.mapTargetsCount;
    while (mapTargetsCount > 0)
    {
        MapTargets mapTargets;
        mapTargets.targetsShowList.clear();
        _stream >> mapTargets.ObjectiveUID;
        _stream >> mapTargets.TargetUID;
        _stream >> mapTargets.TargetType;
        _stream >> mapTargets.TargetColor;
        _stream >> mapTargets.azimuth;
        _stream >> mapTargets.latitude;
        _stream >> mapTargets.longitude;
        _stream >> mapTargets.running;
        _stream >> mapTargets.speed;
        _stream >> mapTargets.moveAzimuth;
        _stream >> mapTargets.moveDistance;
        //_stream >> mapTargets.delay;
        //_stream >> mapTargets.activeTime;
        _stream >> mapTargets.onlyNight;
        _stream >> mapTargets.onlyDay;
        _stream >> mapTargets.dropTarget;
        _stream >> mapTargets.blockTarget;
        _stream >> mapTargets.targetShowNumber;
        for (int j = 0; j < mapTargets.targetShowNumber; j++)
        {
            TargetsShow targetsShow;
            _stream >> targetsShow.startTime;
            _stream >> targetsShow.dayTime;
            _stream >> targetsShow.nightTime;
            mapTargets.targetsShowList << targetsShow;
        }
        Targets.mapTargetsList << mapTargets;
        mapTargetsCount--;
    }
    qDebug() << Targets;
}

void Helicopter_TCP_Client_Instructor::GetObjectsDataPacket(QDataStream& _stream)
{
    qDebug() << "--<" << __FUNCTION__;
    ObjectsDataPacket& Objects = Helicopter->instructor->Objects;

    Objects.PacketReady = true;
    Objects.mapObjList.clear();

    _stream >> Objects.mapObjCount;
    qint32 mapObjCount = Objects.mapObjCount;
    while (mapObjCount > 0)
    {
        MapObjects mapObj;
        _stream >> mapObj.objUID;
        _stream >> mapObj.type;
        _stream >> mapObj.subType;
        _stream >> mapObj.azimuth;
        _stream >> mapObj.latitude;
        _stream >> mapObj.longitude;
        Objects.mapObjList << mapObj;
        mapObjCount--;
    }

    // блок инициализации для кабины
    if ( ( Helicopter->_ini->SimulatorType( ) == 19 ) && ( Objects.mapObjCount > 0 ) )
    {
        Helicopter->instructor->StartLatitude = Objects.mapObjList[0].latitude;
        Helicopter->instructor->StartLongitude = Objects.mapObjList[0].longitude;
        Helicopter->instructor->course = static_cast<float>(Objects.mapObjList[0].azimuth);

        Helicopter->_BoardSystem->StartLatitude = Helicopter->instructor->StartLatitude;
        Helicopter->_BoardSystem->StartLongitude = Helicopter->instructor->StartLongitude;
        Helicopter->_BoardSystem->Latitude = Helicopter->instructor->StartLatitude;
        Helicopter->_BoardSystem->Longitude = Helicopter->instructor->StartLongitude;
    }

    // блок инициализации для подыгрыша
    if ( ( Helicopter->_ini->SimulatorType() == 20 ) && ( Objects.mapObjCount > 1 ) )
    {
        Helicopter->instructor->StartLatitude = Objects.mapObjList[1].latitude;
        Helicopter->instructor->StartLongitude = Objects.mapObjList[1].longitude;
        Helicopter->instructor->course = static_cast<float>(Objects.mapObjList[1].azimuth);

        Helicopter->_BoardSystem->StartLatitude = Helicopter->instructor->StartLatitude;
        Helicopter->_BoardSystem->StartLongitude = Helicopter->instructor->StartLongitude;
        Helicopter->_BoardSystem->Latitude = Helicopter->instructor->StartLatitude;
        Helicopter->_BoardSystem->Longitude = Helicopter->instructor->StartLongitude;
    }

    qDebug() << Objects;
}

void Helicopter_TCP_Client_Instructor::GetEnvironmentDataPacket(QDataStream& _stream)
{
    qDebug() << "--<" << __FUNCTION__;
    EnvironmentDataPacket& Environment = Helicopter->instructor->Environment;

    Environment.PacketReady = true;
    _stream >> Environment.unixtime;
    _stream >> Environment.temperature;
    _stream >> Environment.cloudsGrade;
    _stream >> Environment.bottomCloudsLevel;
    _stream >> Environment.topCloudsLevel;
    _stream >> Environment.moistureLevel;
    _stream >> Environment.windSpeed;
    _stream >> Environment.windDirection;
    _stream >> Environment.visibility;
    _stream >> Environment.verticalWindSpeed;
    _stream >> Environment.windBlows;
    _stream >> Environment.pressure;
    _stream >> Environment.snowLevel;
    _stream >> Environment.camNumber;
    _stream >> Environment.fog;
    _stream >> Environment.amount_precipitation;
    _stream >> Environment.intensity_precipitation;
    _stream >> Environment.intensity_vpp;
    emit Enviroment( );
    qDebug() << Environment;
    Helicopter->instructor->weather.temp = static_cast<float>(Environment.temperature);
    Helicopter->instructor->weather.pressure = Environment.pressure;
    Helicopter->instructor->weather.wind_speed = Environment.windSpeed;
    Helicopter->instructor->weather.wind_dir = Environment.windDirection;
    Helicopter->instructor->weather.vws = Environment.verticalWindSpeed;
    Helicopter->instructor->weather.wb = Environment.windBlows;
    Helicopter->instructor->WindChanged = true;
}

void Helicopter_TCP_Client_Instructor::GetStepCheckDataPacket(QDataStream& /*_stream*/)
{
    qDebug() << "--<" << __FUNCTION__;
    TestVidStepsDataPacket& TestVidStep = Helicopter->instructor->TestVidSteps;
    TestVidStep.PacketReady = true;
    emit TestVid( );
}

void Helicopter_TCP_Client_Instructor::GetShutterDataPacket(QDataStream& /*_stream*/)
{
    qDebug() << "--<" << __FUNCTION__ << "empty";
}

void Helicopter_TCP_Client_Instructor::GetProjectorDataPacket(QDataStream& _stream)
{
    qDebug() << "--<" << __FUNCTION__;
    ProjectorDataPacket& PJ = Helicopter->instructor->PJ;

    PJ.PacketReady = true;
    _stream >> PJ.power_on;

    if (PJ.power_on)
        Helicopter->pj_->powerOn();
    else
        Helicopter->pj_->powerOff();
}

void Helicopter_TCP_Client_Instructor::GetSoundCamDataPacket(QDataStream& /*_stream*/)
{
    //qDebug() << "--<" << __FUNCTION__ << "empty";
}

void Helicopter_TCP_Client_Instructor::GetPlaySpeedDataPacket(QDataStream& _stream)
{
    qDebug() << "--<" << __FUNCTION__;
    float speed;
    _stream >> speed;
    qDebug( ) << "speed:" << speed;
    emit PlaySpeed( speed );
}

void Helicopter_TCP_Client_Instructor::GetPlayPosDataPacket(QDataStream& _stream)
{
    qDebug() << "--<" << __FUNCTION__;
    quint32 pos;
    _stream >> pos;
    qDebug( ) << "pos:" << pos;
    emit PlayPos( pos );
}

void Helicopter_TCP_Client_Instructor::GetPlayModeDataPacket(QDataStream& _stream)
{
    qDebug() << "--<" << __FUNCTION__;
    quint8 mode;
    _stream >> mode;
    qDebug( ) << "mode:" << mode;
    emit PlayMode( mode );
}

void Helicopter_TCP_Client_Instructor::GetRunControlDataPacket(QDataStream& _stream)
{
    qDebug() << "--<" << __FUNCTION__;
    QString run_uuid;
    _stream >> run_uuid;
    qDebug( ) << "run_uuid:" << run_uuid;
    //emit AmmoIn( );
    emit RunRecords( run_uuid );
}

void Helicopter_TCP_Client_Instructor::On_SendImagerReady( )
{
    SendImagerReadyDataPacket( );
}

void Helicopter_TCP_Client_Instructor::On_SendTcpData()
{
//    SendCommonTcpData();
    SendBallisticDataPacket( );
	SendComponentsStatePacket( );
	SendAviaStateDataPacket( );
    SendImagerReadyDataPacket( );
    SendServerReadyDataPacket( );
}

void Helicopter_TCP_Client_Instructor::SendBallisticDataPacket( )
{
    const auto& _Imager = Helicopter->imager;
    if ( !_Imager->Ballistic.PacketReady ) return;
    if ( TcpSocket->state( ) == QAbstractSocket::ConnectedState )
    {
        qDebug( ) << "-->" << __FUNCTION__;
        qDebug( ) << _Imager->Ballistic;
        QByteArray data;
        QDataStream data_stream( &data, QIODevice::WriteOnly );
        data_stream.setByteOrder( QDataStream::LittleEndian );
        data_stream.setFloatingPointPrecision( QDataStream::SinglePrecision );
        data_stream << static_cast<qint32>( _Imager->Ballistic.ObjectiveUID );            // UID цели
        data_stream << static_cast<qint32>( _Imager->Ballistic.TargetUID );               // UID цели
        data_stream << static_cast<quint8>( _Imager->Ballistic.ObjectiveType );           // Тип цели
        data_stream << static_cast<bool>( _Imager->Ballistic.Hit );                       // Попадание
        data_stream << static_cast<float>( _Imager->Ballistic.Radius );                   // Радиус попадания
        data_stream << static_cast<quint8>( _Imager->Ballistic.GunType );                 // Тип оружия
        data_stream << static_cast<bool>( _Imager->Ballistic.TargetState );               // Мишень опущена/поднята
        data_stream << static_cast<quint8>( _Imager->Ballistic.TriggerID );               // ID Триггера
        data_stream << static_cast<quint8>( _Imager->Ballistic.Direction );               // Направление
        data_stream << static_cast<quint8>( _Imager->Ballistic.Hit_X );                   // Координаты пробоины по X (в % от левого края)
        data_stream << static_cast<quint8>( _Imager->Ballistic.Hit_Y );                   // Координаты пробоины по Y (в % от верхнего края)
        QByteArray header;
        QDataStream header_stream( &header, QIODevice::WriteOnly );
        header_stream.setByteOrder( QDataStream::LittleEndian );
        header_stream << static_cast<quint8>( 'S' );
        header_stream << static_cast<quint8>( 't' );
        header_stream << static_cast<quint8>( 'o' );
        header_stream << static_cast<quint8>( 'I' );
        header_stream << static_cast<quint32>( InstructorTCPPacketTypes::SR2IR_BallisticEventsPacket );
        header_stream << static_cast<quint32>( data.size( ) );
        QByteArray packet;
        packet.append( header );
        packet.append( data );
        TcpSocket->write( packet );
        TcpSocket->flush( );
        _Imager->Ballistic.PacketReady = false;
    }
}

void Helicopter_TCP_Client_Instructor::SendImagerReadyDataPacket()
{
    const auto& _Imager = Helicopter->imager;
    if (!_Imager->ImagerReady.PacketReady) return;

    if (TcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        qDebug() << "-->" << __FUNCTION__;
        QByteArray data;
        QDataStream data_stream(&data, QIODevice::WriteOnly);
        data_stream.setByteOrder(QDataStream::LittleEndian);
        data_stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
        data_stream << static_cast<quint8>(_Imager->ImagerReady.simulatorType);          // Тип симулятора (техники)
        data_stream << static_cast<quint8>(_Imager->ImagerReady.ImagerReady);            // Готов к упражнению
        QByteArray header;
        QDataStream header_stream(&header, QIODevice::WriteOnly);
        header_stream.setByteOrder(QDataStream::LittleEndian);
        header_stream << static_cast<quint8>('S');
        header_stream << static_cast<quint8>('t');
        header_stream << static_cast<quint8>('o');
        header_stream << static_cast<quint8>('I');
        header_stream << static_cast<quint32>(InstructorTCPPacketTypes::SR2IR_ImagerReadyStatePacket);
        header_stream << static_cast<quint32>(data.size());
        QByteArray packet;
        packet.append(header);
        packet.append(data);
        TcpSocket->write(packet);
        TcpSocket->flush( );
        _Imager->ImagerReady.PacketReady = false;

        qDebug() << _Imager->ImagerReady;
    }
}

void Helicopter_TCP_Client_Instructor::SendServerReadyDataPacket( )
{
    if ( !Helicopter->server_->ServerReady->PacketReady ) return;
    if ( TcpSocket->state( ) == QAbstractSocket::ConnectedState )
    {
        QByteArray data;
        QDataStream data_stream( &data, QIODevice::WriteOnly );
        data_stream.setByteOrder( QDataStream::LittleEndian );
        data_stream.setFloatingPointPrecision( QDataStream::SinglePrecision );
        data_stream << static_cast<quint8>( Helicopter->_ini->SimulatorType( ) ); // Тип симулятора (техники)
        data_stream << static_cast<quint8>( Helicopter->server_->ServerReady->ServerReady );            // Готов к упражнению
        QByteArray header;
        QDataStream header_stream( &header, QIODevice::WriteOnly );
        header_stream.setByteOrder( QDataStream::LittleEndian );
        header_stream << static_cast<quint8>( 'S' );
        header_stream << static_cast<quint8>( 't' );
        header_stream << static_cast<quint8>( 'o' );
        header_stream << static_cast<quint8>( 'I' );
        header_stream << static_cast<quint32>( InstructorTCPPacketTypes::SR2IR_ServerReadyStatePacket );
        header_stream << static_cast<quint32>( data.size( ) );
        QByteArray packet;
        packet.append( header );
        packet.append( data );
        TcpSocket->write( packet );
        TcpSocket->flush( );
        Helicopter->server_->ServerReady->PacketReady = false;
    }
    else { 
        Helicopter->server_->ServerReady->PacketReady = true; // повторить пакет, при отключении инструктора
    }
}

void Helicopter_TCP_Client_Instructor::SendAviaStateDataPacket()
{
    if (TcpSocket->state() == QAbstractSocket::ConnectedState &&  Helicopter->instructor->Avia.PacketReady)
    {
        //qDebug() << "-->" << __FUNCTION__;
        // этот пакет спамит
        Helicopter->instructor->Avia.PacketReady = false;                  // готовность пакета
        QByteArray data;
        QDataStream data_stream(&data, QIODevice::WriteOnly);
        data_stream.setByteOrder(QDataStream::LittleEndian);
        data_stream.setFloatingPointPrecision(QDataStream::SinglePrecision);

        data_stream << Helicopter->instructor->Avia.Pitch;     // Pitch(Наклон вперед назад) - Тангаж
        data_stream << Helicopter->instructor->Avia.Roll;      // Roll(Крен или наклон вправо влево)
        data_stream << Helicopter->instructor->Avia.Curse;     // Курс
        data_stream << Helicopter->instructor->Avia.Speed;     // Скорость
        data_stream << Helicopter->instructor->Avia.Longitude; // Координата техники х(Долгота)
        data_stream << Helicopter->instructor->Avia.Latitude;  // Координата техники у(Широта)
        data_stream << Helicopter->instructor->Avia.Altitude;  // Координата техники z(Высота)
        data_stream << Helicopter->instructor->Avia.Eng1;      // Запущен ли двигатель 1
        data_stream << Helicopter->instructor->Avia.Eng2;      // Запущен ли двигатель 2
        data_stream << Helicopter->instructor->Avia.Colision;  // Столкновение с обьектом
        data_stream << static_cast<quint8>(Helicopter->instructor->Avia.TechID);    // Идентификатор техники

        QByteArray header;
        QDataStream header_stream(&header, QIODevice::WriteOnly);
        header_stream.setByteOrder(QDataStream::LittleEndian);
        header_stream << static_cast<quint8>('S');
        header_stream << static_cast<quint8>('t');
        header_stream << static_cast<quint8>('o');
        header_stream << static_cast<quint8>('I');
        header_stream << static_cast<quint32>(InstructorTCPPacketTypes::SR2IR_AviaStateDataPacket);
        header_stream << static_cast<quint32>(data.size());
        QByteArray packet;
        packet.append(header);
        packet.append(data);
        TcpSocket->write(packet);
        TcpSocket->flush();
    }
}

// функция отправки состояния компонентов.
// TODO написать реализацию получения реальных данных
//  [ ID=100 Server >> Instructor] Данные о готовности компонентов
void Helicopter_TCP_Client_Instructor::SendComponentsStatePacket()
{
    // в случае реконекта на инструктор перепосылаем пакет компонентов
    if (TcpSocket->state() != QAbstractSocket::ConnectedState)
    {
        Helicopter->instructor->SetComponentStatusChanged();
    }

    if(TcpSocket->state() == QAbstractSocket::ConnectedState && Helicopter->instructor->GetComponentStatusChanged())
    {
        qDebug() << "-->" << __FUNCTION__;
        // этот пакет спамит
        quint8 StaticStructCount = 12;
        QByteArray data;
        QDataStream data_stream(&data, QIODevice::WriteOnly);
        data_stream.setByteOrder(QDataStream::LittleEndian);
        data_stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
		// TODO сделать нормальное время
		data_stream << static_cast<qint32>( 999 );        // время прошедшее с запуска сервера 
		data_stream << static_cast<qint32>( StaticStructCount );         // Число структур теста функционирования

        data_stream << static_cast<quint8>(ElementsTypes::Server);                           // 1
        data_stream << static_cast<quint8>(ElementsStateTypes::Good);
        data_stream << QString("Server");
        data_stream << QString("Server");

        data_stream << static_cast<quint8>(ElementsTypes::CAN);                              // 2
        data_stream << static_cast<quint8>(ElementsStateTypes::Good);
        data_stream << QString("Can");
        data_stream << QString("Система обмена данными");

        data_stream << static_cast<quint8>(6);                               // 5
        data_stream << static_cast<quint8>(ElementsStateTypes::Good);
        data_stream << QString("Visual_Controls");
        data_stream << QString("Визуализация органов управления");

        data_stream << static_cast<quint8>(5);                     // 6
        data_stream << static_cast<quint8>(ElementsStateTypes::Good);
        data_stream << QString("Mi-17V-5_Dedicated_Rep_Server");
        data_stream << QString("Сервер визуализации");

        data_stream << static_cast<quint8>(7);                     // 7
        data_stream << static_cast<quint8>(ElementsStateTypes::Good);
        data_stream << QString("Mi-17V-5_Image_Generator_1");
        data_stream << QString("1 Проектор");

        data_stream << static_cast<quint8>(7);                     // 8
        data_stream << static_cast<quint8>(ElementsStateTypes::Good);
        data_stream << QString("Mi-17V-5_Image_Generator_2");
        data_stream << QString("2 Проектор");

        data_stream << static_cast<quint8>(7);                     // 9
        data_stream << static_cast<quint8>(ElementsStateTypes::Good);
        data_stream << QString("Mi-17V-5_Image_Generator_3");
        data_stream << QString("3 Проектор");

        data_stream << static_cast<quint8>(7);                     // 10
        data_stream << static_cast<quint8>(ElementsStateTypes::Good);
        data_stream << QString("Mi-17V-5_Image_Generator_4");
        data_stream << QString("4 Проектор");

        data_stream << static_cast<quint8>(7);                     // 11
        data_stream << static_cast<quint8>(ElementsStateTypes::Good);
        data_stream << QString("Mi-17V-5_Image_Generator_5");
        data_stream << QString("5 Проектор");

        data_stream << static_cast<quint8>(7);                     // 12
        data_stream << static_cast<quint8>(ElementsStateTypes::Good);
        data_stream << QString("Mi-17V-5_Image_Generator_6");
        data_stream << QString("6 Проектор");

		data_stream << static_cast<quint8>( ElementsTypes::Joystick );                         // 13
        data_stream << static_cast<quint8>(ElementsStateTypes::Good);
        data_stream << QString("Joystick");
        data_stream << QString("Джойстик");

		data_stream << static_cast<quint8>( ElementsTypes::PositionTracking );                         // 14
        data_stream << static_cast<quint8>(ElementsStateTypes::Good);
        data_stream << QString("SOK");
        data_stream << QString("СОК");

        QByteArray header;
        QDataStream header_stream(&header, QIODevice::WriteOnly);
        header_stream.setByteOrder(QDataStream::LittleEndian);
        header_stream << static_cast<quint8>('S');
        header_stream << static_cast<quint8>('t');
        header_stream << static_cast<quint8>('o');
        header_stream << static_cast<quint8>('I');
        header_stream << static_cast<quint32>(InstructorTCPPacketTypes::SR2IR_ComponentsStatePacket);
        header_stream << static_cast<quint32>(data.size());
        QByteArray packet;
        packet.append(header);
        packet.append(data);
        TcpSocket->write(packet);
        TcpSocket->flush();
    }
}

//[ ID=105 # Server >> Instructor ] 
void Helicopter_TCP_Client_Instructor::SendArmStateDataPacket( )
{
    if ( TcpSocket->state( ) == QAbstractSocket::ConnectedState )
    {
        qDebug() << "-->" << __FUNCTION__;
        qDebug( ) << Helicopter->instructor->AmmunitionOutPack;
        QByteArray data;
        QDataStream data_stream( &data, QIODevice::WriteOnly );
        data_stream.setByteOrder( QDataStream::LittleEndian );
        data_stream.setFloatingPointPrecision( QDataStream::SinglePrecision );
//		data_stream << Helicopter->instructor->AmmunitionOutPack;
        data_stream << static_cast<quint8>( Helicopter->instructor->AmmunitionOutPack.packageCount );  // число наборов боеприпасов
        for ( int i = 0; i < Helicopter->instructor->AmmunitionOutPack.packageCount; i++ )
        {
            Ammunition ammunition = static_cast<Ammunition>( Helicopter->instructor->AmmunitionOutPack.AmmunitionList[i] );
            data_stream << static_cast<quint8>( ammunition.gunType );
            data_stream << static_cast<quint8>( ammunition.ammoType );
            data_stream << static_cast<quint32>( ammunition.amount );
            if (ammunition.gunType == 27) {
                Helicopter->lp_azs.f_ammo_count_upk_right_1 = (ammunition.amount / 2) / 100 % 10;
                Helicopter->lp_azs.f_ammo_count_upk_right_2 = (ammunition.amount / 2) % 100;
                Helicopter->lp_azs.f_ammo_count_upk_left_1  = (ammunition.amount / 2) / 100 % 10;
                Helicopter->lp_azs.f_ammo_count_upk_left_2  = (ammunition.amount / 2) % 100;
            }
        }

        QByteArray header;
        QDataStream header_stream( &header, QIODevice::WriteOnly );
        header_stream.setByteOrder( QDataStream::LittleEndian );
        header_stream << static_cast<quint8>( 'S' );
        header_stream << static_cast<quint8>( 't' );
        header_stream << static_cast<quint8>( 'o' );
        header_stream << static_cast<quint8>( 'I' );
        header_stream << static_cast<quint32>( InstructorTCPPacketTypes::SR2IR_ArmsStateDataPacket );
        header_stream << static_cast<quint32>( data.size( ) );
        QByteArray packet;
        packet.append( header );
        packet.append( data );
        TcpSocket->write( packet );
        TcpSocket->flush( );
    }
}
