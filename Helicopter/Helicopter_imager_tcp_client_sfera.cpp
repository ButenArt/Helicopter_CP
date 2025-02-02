#include "Helicopter_imager_tcp_client_sfera.h"
#include "Helicopter_instructor.h"
#include "Helicopter_instructor_tcp_client.h"

int64_t cycle_time();

Helicopter_TCP_Client_Imager_Sfera::Helicopter_TCP_Client_Imager_Sfera(Helicopter_core* Helicopter, int index)
{
    Helicopter = Helicopter;

    //TxInterval = Helicopter_->_ini->LanTxInterval();
    TxInterval = 100;    //               1/60 секунды для 60 FPS
    hostaddr = Helicopter->_ini->IP_Imager(index);
    tcp_port = Helicopter->_ini->PORT_Imager(index);
    qDebug() << "Подключение VS" << index << " к " << hostaddr << ":" << tcp_port;

    ConnectToHost(hostaddr, tcp_port);
    TimerTCPSend->setInterval(TxInterval);
    connect(TimerTCPSend, SIGNAL(timeout()), this, SLOT(On_SendPeriodicTcpData()));
    TimerTCPSend->start();

    connect(this,SIGNAL(signalConnected()),this,SLOT(On_Connected()),Qt::QueuedConnection);
    connect(this,SIGNAL(signalDisconnected()),this,SLOT(On_Disconnected()),Qt::QueuedConnection);
    connect(this,SIGNAL(signalError()),this,SLOT(On_Error()),Qt::QueuedConnection);
    connect(TcpSocket, SIGNAL(readyRead()), this, SLOT(On_ReadTcpData()));


    start = std::chrono::system_clock::now();
}

Helicopter_TCP_Client_Imager_Sfera::~Helicopter_TCP_Client_Imager_Sfera()
{
}

void Helicopter_TCP_Client_Imager_Sfera::Start()
{
    if (tcp_port == 10000) { // для визуалки AES
        return;
    }
    else
    {
        SendHelicopterStartPacket(); // отправляем пакет инициализации графики
        SendHelicopterWeatherPacket();
    }
}

void Helicopter_TCP_Client_Imager_Sfera::Stop()
{
}

void Helicopter_TCP_Client_Imager_Sfera::On_Connected()
{
    TcpSocket->setSocketOption(QAbstractSocket::LowDelayOption, 1);  // выключаем режим nagle, пакет отправляется сразу, без ожидания дополнительных данных

    //qDebug() << "Imager Connected";
    Helicopter->imager->ImagerActive_ = ClientConnected();
    Helicopter->log_->WriteLANLog(LogEvents::TCP_Imager_connected, "");
}

void Helicopter_TCP_Client_Imager_Sfera::On_Disconnected()
{
    //qDebug() << "Imager Disconnected";
    Helicopter->imager->ImagerActive_ = ClientConnected();
    Helicopter->log_->WriteLANLog(LogEvents::TCP_Imager_disconnected, "");
}

void Helicopter_TCP_Client_Imager_Sfera::On_Error()
{
    Helicopter->imager->ImagerActive_ = ClientConnected();
    Helicopter->log_->WriteLANLog(LogEvents::TCP_Imager_Error, socketErrorString);
}
void Helicopter_TCP_Client_Imager_Sfera::On_ReadTcpData()
{
    if ( tcp_port == 8001 )
        ReadTcpDataKlim();
    else {
        return;
        //ReadTcpDataAES();
    }
}

void Helicopter_TCP_Client_Imager_Sfera::ReadTcpDataKlim()
{

    if (TcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        auto FP = Helicopter->Model_->GetFPar();
        auto hs = Helicopter->imager->GetHS();
        auto header_length = 8;
        auto availabeBytes = TcpSocket->bytesAvailable();
        while (availabeBytes >= header_length)
        {
            if (!processedHeader)
            {
                QByteArray header_data = TcpSocket->read(header_length);

                QDataStream header_stream(header_data);
                header_stream.setByteOrder(QDataStream::LittleEndian);

                header_stream >> hs.PacketID;
                header_stream >> hs.PacketSize;
                processedHeader = true;
                availabeBytes = TcpSocket->bytesAvailable();
            }

            if ((processedHeader) && (availabeBytes >= hs.PacketSize))
            {
                QByteArray packet_data = TcpSocket->read(hs.PacketSize);
                QDataStream data_stream(packet_data);
                data_stream.setByteOrder(QDataStream::LittleEndian);
                data_stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
                switch (hs.PacketID)
                {
                    case static_cast<quint32>(311) : // Новая визуалка
                        hs.PacketReady = true;
                        data_stream >> hs.HelixID;            //ID вертолета
                        data_stream >> hs.Hgeo;
                        data_stream >> hs.sh1;             //Высота над землей первой опоры
                        data_stream >> hs.sh2;             //Высота над землей второй опоры
                        data_stream >> hs.sh3;             //Высота над землей третьей опоры
                        data_stream >> hs.SampleNr;        //SampleNr(номер сэмпла)
                        data_stream >> hs.TimeStamp;       //TimeStamp

                        if (hs.Hgeo < 2.4f) {
                            qDebug() << "Height visual" << hs.Hgeo;
                            hs.Hgeo = 2.4f;
                        }
                        FP->Hgeo = hs.Hgeo;
                        break;
                    case static_cast<quint32>(51) : // Старая визуалка
                        double Hsea;
                        double Hgeo;
                        double Hsh1;
                        double Hsh2;
                        double Hsh3;
                        double UD;
                        int    ui;
                        hs.PacketReady = true;
                        data_stream >> hs.HelixID;                  //ID вертолета
                        data_stream.setFloatingPointPrecision(QDataStream::DoublePrecision);
                        data_stream >> Hsea;                        // высота над уровнем моря?
                        data_stream >> Hgeo; hs.Hgeo = float(Hgeo);  // высота ЦТ над поверхностью, земля, дома, вода
                        data_stream >> Hsh1; hs.sh1 = float(Hsh1);   // Высота над землей первой опоры
                        data_stream >> Hsh2; hs.sh2 = float(Hsh2);   // Высота над землей второй опоры
                        data_stream >> Hsh3; hs.sh3 = float(Hsh3);             //Высота над землей третьей опоры
                        data_stream >> ui;
                        data_stream >> hs.SampleNr;        //SampleNr(номер сэмпла)
                        data_stream >> UD;

//                      //qDebug() << hs.Hgeo << hs.sh1 << hs.sh2 << hs.sh3;
                        
                        if ( hs.Hgeo > 2.4 ) Helicopter->_BoardSystem->GeoHeight = hs.Hgeo;
                        //qDebug( ) << QTime::currentTime( ) << "Alt" << hs.Hgeo << *FP;

                        break;
                    default:
                        //qDebug() << "LogEvents::TCP_Imager_Unknown_PacketType" << hs.PacketID;
                       break;
                }
                processedHeader = false;
            }

            availabeBytes = TcpSocket->bytesAvailable();
        }
    }
}

void Helicopter_TCP_Client_Imager_Sfera::ReadTcpDataAES()
{
    bool processedHeader = false;

    static quint32 old_packet;
    if (TcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        auto _Imager = Helicopter->imager.data();
        auto header_length = sizeof(_Imager->PacketHeader);
        auto availabeBytes = TcpSocket->bytesAvailable();
        while (availabeBytes >= header_length)
        {
            if (!processedHeader)
            {
                QByteArray header_data = TcpSocket->read(header_length);

                QDataStream header_stream(header_data);
                header_stream.setByteOrder(QDataStream::LittleEndian);
                header_stream.setFloatingPointPrecision(QDataStream::SinglePrecision);

                header_stream >> _Imager->PacketHeader.Mark;
                header_stream >> _Imager->PacketHeader.PacketType;
                header_stream >> _Imager->PacketHeader.DataSize;
                processedHeader = true;
                availabeBytes = TcpSocket->bytesAvailable( );
            }

            if ((processedHeader) && (availabeBytes >= _Imager->PacketHeader.DataSize))
            {
                QByteArray packet_data = TcpSocket->read(_Imager->PacketHeader.DataSize);
                QDataStream data_stream(packet_data);
                data_stream.setByteOrder(QDataStream::LittleEndian);
                data_stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
                switch (_Imager->PacketHeader.PacketType)
                {
                case static_cast<quint32>(ImagerTCPPacketTypes::IG2SR_ImagerStateDataPacket):
                    GetImagerState(data_stream);
                    break;
                case static_cast<quint32>(ImagerTCPPacketTypes::IG2SR_ImagerReadyDataPacket):
                    GetImagerReadyDataPacket(data_stream);
                    break;
                case static_cast<quint32>(ImagerTCPPacketTypes::IG2SR_ImagerElementsStateDataPacket):
                    GetImagerElementsStateDataPacket(data_stream);
                    break;
                case static_cast<quint32>(ImagerTCPPacketTypes::IG2SR_SensorsDataPacket):
                    GetHeightDataPacket(data_stream);
                    break;
                case static_cast<quint32>(ImagerTCPPacketTypes::IG2SR_BallisticEventsPacket):
                    GetBallisticDataPacket(data_stream);
                    break;
                case static_cast<quint32>(ImagerTCPPacketTypes::IG2SR_TriggerEventsPacket):
                    GetTriggerDataPacket(data_stream);
                    break;
                case static_cast<quint32>(ImagerTCPPacketTypes::IG2SR_MissedEventsPacket):
                    GetMissedEventPacked(data_stream);
                    break;
                case static_cast<quint32>(ImagerTCPPacketTypes::IG2SR_CargoEventsPacket ):
                    GetCargoEventPacked(data_stream);
                    break;
                case static_cast<quint32>(ImagerTCPPacketTypes::IG2SR_AmmoEventsPacket ):
                    GetAmmoEventPacked(data_stream);
                    break;
                default:
                    qDebug() << "LogEvents::TCP_Imager_Unknown_PacketType" << _Imager->PacketHeader.PacketType;
                    //qDebug() << "previous package" << old_packet;
                    break;
                }
            }
            availabeBytes = TcpSocket->bytesAvailable();
            processedHeader = false;
            old_packet = _Imager->PacketHeader.PacketType;
        }
    }
}

void Helicopter_TCP_Client_Imager_Sfera::On_SendTcpData()
{
    if (tcp_port != 8001) // для визуалки AES
    {
        SendHelicopterMovingDataPacket();
    }
    else   // для визуалки Клименко
    {
        SendHelicopterStatPacket();
        SendHelicopterMovingPacket();
    }
}

void Helicopter_TCP_Client_Imager_Sfera::On_SendPeriodicTcpData()
{
    if (tcp_port == 10000)// для визуалки AES
    {
        SendExerciseParametersPacket();     // 1
        SendObjectsDataPacket();            // 2
        SendEnvironmentDataPacket();        // 3
        SendTargetsDataPacket();            // 5
        SendCommandsDataPacket();           // 13
        SendMessagesDataPacket();           // 14
        SendTargetsControlDataPacket();     // 16
        //SendWeaponDataPacket();             // 18 эмитится из control
        SendEffectsDataPacket();            // 101
        // 102 эммитится из модели 
        //SendFireDataPacket();               // 103 то же эмиттеть из модели
    }
}

//[ ID=1 # Server >> Visual ] Команда на запуск карты
void Helicopter_TCP_Client_Imager_Sfera::SendExerciseParametersPacket()
{

    if (!Helicopter->instructor->ExerciseParameters.PacketReady) return;

    if (TcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        qDebug() << "<--" << __FUNCTION__;
        const auto& _Instructor = Helicopter->instructor.data();
        QByteArray data;
        QDataStream data_stream(&data, QIODevice::WriteOnly);
        data_stream.setByteOrder(QDataStream::LittleEndian);
        data_stream << static_cast<bool>(_Instructor->ExerciseParameters.ReStart);
        data_stream << static_cast<bool>(_Instructor->ExerciseParameters.Pause);
        data_stream << static_cast<bool>(_Instructor->ExerciseParameters.Stop);
        data_stream << static_cast<quint8>(_Instructor->ExerciseParameters.ExerciseNum);
        data_stream << static_cast<quint8>(_Instructor->ExerciseParameters.MapType);
        data_stream << static_cast<bool>(_Instructor->ExerciseParameters.night);
        data_stream << static_cast<quint8>(_Instructor->ExerciseParameters.season);
        data_stream << static_cast<bool>(_Instructor->ExerciseParameters.Joystick);
        data_stream << static_cast<bool>( false ); // АВтомат заряжания
        data_stream << static_cast<qint8>(_Instructor->ExerciseParameters.directionsNumber);
        data_stream << static_cast<bool>(_Instructor->ExerciseParameters.backfire);
        data_stream << static_cast<quint8>(_Instructor->ExerciseParameters.selectedKit);
        data_stream << static_cast<quint8>(_Instructor->ExerciseParameters.selectedMode);
        data_stream << static_cast<qint8>(_Instructor->ExerciseParameters.distance2screen);
        data_stream << static_cast<QString>(_Instructor->ExerciseParameters.run );
        data_stream << static_cast<QString>(_Instructor->ExerciseParameters.lang );
        QByteArray header;
        QDataStream header_stream(&header, QIODevice::WriteOnly);
        header_stream.setByteOrder(QDataStream::LittleEndian);
        header_stream << static_cast<quint8>('S');
        header_stream << static_cast<quint8>('t');
        header_stream << static_cast<quint8>('o');
        header_stream << static_cast<quint8>('V');
        header_stream << static_cast<quint32>(ImagerTCPPacketTypes::SR2IG_ExerciseParametersPacket);
        header_stream << static_cast<quint32>(data.size());
        QByteArray packet;
        packet.append(header);
        packet.append(data);
        TcpSocket->write(packet, packet.length());
        TcpSocket->flush();

        Helicopter->instructor->ExerciseParameters.PacketReady = false;
        if ( _Instructor->ExerciseParameters.Stop == true )
        {
            Helicopter->imager->ImagerWork = false;
            //qDebug() << "Helicopter->imager_->ImagerWork" << Helicopter->imager->ImagerWork;
        }
        qDebug() << _Instructor->ExerciseParameters;
    }
}

//[ ID=2 # Server >> Visual ] Объекты на карте
void Helicopter_TCP_Client_Imager_Sfera::SendObjectsDataPacket()
{
    if (!Helicopter->instructor->Objects.PacketReady) return;

    if (TcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        qDebug() << "<--" << __FUNCTION__;
        const auto& _Instructor = Helicopter->instructor.data();
        QByteArray data;
        QDataStream data_stream(&data, QIODevice::WriteOnly);
        data_stream.setByteOrder(QDataStream::LittleEndian);
        data_stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
        data_stream << static_cast<qint32>(_Instructor->Objects.mapObjCount);
        for (int i = 0; i < _Instructor->Objects.mapObjCount; i++)
        {
            MapObjects mapObj = static_cast<MapObjects>(_Instructor->Objects.mapObjList[i]);
            data_stream << static_cast<qint32>(mapObj.objUID);
            data_stream << static_cast<quint8>(mapObj.type);
            data_stream << static_cast<quint8>(mapObj.subType);
            data_stream << static_cast<qint32>(mapObj.azimuth);
            data_stream << static_cast<float>(mapObj.latitude);
            data_stream << static_cast<float>(mapObj.longitude);
        }

        QByteArray header;
        QDataStream header_stream(&header, QIODevice::WriteOnly);
        header_stream.setByteOrder(QDataStream::LittleEndian);
        header_stream << static_cast<quint8>('S');
        header_stream << static_cast<quint8>('t');
        header_stream << static_cast<quint8>('o');
        header_stream << static_cast<quint8>('V');
        header_stream << static_cast<quint32>(ImagerTCPPacketTypes::SR2IG_ObjectsDataPacket);
        header_stream << static_cast<quint32>(data.size());
        QByteArray packet;
        packet.append(header);
        packet.append(data);
        TcpSocket->write(packet);
        TcpSocket->flush();
        Helicopter->instructor->Objects.PacketReady = false;

        qDebug() << _Instructor->Objects;
    }
}

//[ ID=3 # Server >> Visual  ] Обстановка
void Helicopter_TCP_Client_Imager_Sfera::SendEnvironmentDataPacket()
{
    if (!Helicopter->instructor->Environment.PacketReady) return;

    if (TcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        qDebug() << "<--" << __FUNCTION__;
        const auto& _Instructor = Helicopter->instructor.data();
        QByteArray data;
        QDataStream data_stream(&data, QIODevice::WriteOnly);
        data_stream.setByteOrder(QDataStream::LittleEndian);
        data_stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
        data_stream << static_cast<quint32>(_Instructor->Environment.unixtime);
        data_stream << static_cast<qint32>(_Instructor->Environment.temperature);
        data_stream << static_cast<qint32>(_Instructor->Environment.cloudsGrade);
        data_stream << static_cast<float>(_Instructor->Environment.bottomCloudsLevel);
        data_stream << static_cast<float>(_Instructor->Environment.topCloudsLevel);
        data_stream << static_cast<bool>(_Instructor->Environment.moistureLevel);
        data_stream << static_cast<float>(_Instructor->Environment.windSpeed);
        data_stream << static_cast<float>(_Instructor->Environment.windDirection);
        data_stream << static_cast<float>(_Instructor->Environment.visibility);
        data_stream << static_cast<float>(_Instructor->Environment.verticalWindSpeed);
        data_stream << static_cast<float>(_Instructor->Environment.windBlows);
        data_stream << static_cast<float>(_Instructor->Environment.pressure);
        data_stream << static_cast<qint32>(_Instructor->Environment.snowLevel);
        data_stream << static_cast<qint32>(_Instructor->Environment.camNumber);
        data_stream << static_cast<bool>(_Instructor->Environment.fog);
        data_stream << static_cast<qint32>(_Instructor->Environment.amount_precipitation);
        data_stream << static_cast<qint32>(_Instructor->Environment.intensity_precipitation);
        data_stream << static_cast<qint32>(_Instructor->Environment.intensity_vpp);

        QByteArray header;
        QDataStream header_stream(&header, QIODevice::WriteOnly);
        header_stream.setByteOrder(QDataStream::LittleEndian);
        header_stream << static_cast<quint8>('S');
        header_stream << static_cast<quint8>('t');
        header_stream << static_cast<quint8>('o');
        header_stream << static_cast<quint8>('V');
        header_stream << static_cast<quint32>(ImagerTCPPacketTypes::SR2IG_EnvironmentDataPacket);
        header_stream << static_cast<quint32>(data.size());
        QByteArray packet;
        packet.append(header);
        packet.append(data);
        TcpSocket->write(packet);
        TcpSocket->flush();
        Helicopter->instructor->Environment.PacketReady = false;

        qDebug() << _Instructor->Environment;
    }
}

//[ ID=5 # Server >> Visual ] Мишени на карте
void Helicopter_TCP_Client_Imager_Sfera::SendTargetsDataPacket()
{
    if (!Helicopter->instructor->Targets.PacketReady) return;

    if (TcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        qDebug() << "<--" << __FUNCTION__;
        const auto& _Instructor = Helicopter->instructor.data();
        QByteArray data;
        QDataStream data_stream(&data, QIODevice::WriteOnly);
        data_stream.setByteOrder(QDataStream::LittleEndian);
        data_stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
        data_stream << static_cast<qint32>(_Instructor->Targets.mapTargetsCount);
        for (int i = 0; i < _Instructor->Targets.mapTargetsCount; i++)
        {
            MapTargets mapTargets = static_cast<MapTargets>(_Instructor->Targets.mapTargetsList[i]);
            data_stream << static_cast<qint32>(mapTargets.ObjectiveUID);
            data_stream << static_cast<qint32>(mapTargets.TargetUID);
            data_stream << static_cast<quint8>(mapTargets.TargetType);
            data_stream << static_cast<quint8>(mapTargets.TargetColor);
            data_stream << static_cast<qint32>(mapTargets.azimuth);
            data_stream << static_cast<float>(mapTargets.latitude);
            data_stream << static_cast<float>(mapTargets.longitude);
            data_stream << static_cast<bool>(mapTargets.running);
            data_stream << static_cast<float>(mapTargets.speed);
            data_stream << static_cast<qint32>(mapTargets.moveAzimuth);
            data_stream << static_cast<qint32>(mapTargets.moveDistance);
            //data_stream << static_cast<qint32>(mapTargets.delay);
            //data_stream << static_cast<qint32>(mapTargets.activeTime);
            data_stream << static_cast<bool>(mapTargets.onlyNight);
            data_stream << static_cast<bool>(mapTargets.onlyDay);
            data_stream << static_cast<bool>(mapTargets.dropTarget);
            data_stream << static_cast<bool>(mapTargets.blockTarget);
            data_stream << static_cast<quint8>(mapTargets.targetShowNumber);
            for (int j = 0; j < mapTargets.targetShowNumber; j++)
            {
                TargetsShow targetsShow = static_cast<TargetsShow>(_Instructor->Targets.mapTargetsList[i].targetsShowList[j]);
                data_stream << static_cast<qint32>(targetsShow.startTime);
                data_stream << static_cast<qint32>(targetsShow.dayTime);
                data_stream << static_cast<qint32>(targetsShow.nightTime);
            }
        }

        QByteArray header;
        QDataStream header_stream(&header, QIODevice::WriteOnly);
        header_stream.setByteOrder(QDataStream::LittleEndian);
        header_stream << static_cast<quint8>('S');
        header_stream << static_cast<quint8>('t');
        header_stream << static_cast<quint8>('o');
        header_stream << static_cast<quint8>('V');
        header_stream << static_cast<quint32>(ImagerTCPPacketTypes::SR2IG_TargetsDataPacket);
        header_stream << static_cast<quint32>(data.size());
        QByteArray packet;
        packet.append(header);
        packet.append(data);
        TcpSocket->write(packet);
        TcpSocket->flush();
        Helicopter->instructor->Targets.PacketReady = false;

        qDebug() << _Instructor->Targets;
    }
}

//[ ID=13 # Server >> Visual ] Вперед
void Helicopter_TCP_Client_Imager_Sfera::SendCommandsDataPacket()
{
    if (!Helicopter->instructor->Commands.PacketReady) return;

    if (TcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        qDebug() << "<--" << __FUNCTION__;
        const auto& _Instructor = Helicopter->instructor.data();
        QByteArray data;
        QDataStream data_stream(&data, QIODevice::WriteOnly);
        data_stream.setByteOrder(QDataStream::LittleEndian);
        data_stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
        data_stream << static_cast<quint8>(_Instructor->Commands.MoveForward);

        QByteArray header;
        QDataStream header_stream(&header, QIODevice::WriteOnly);
        header_stream.setByteOrder(QDataStream::LittleEndian);
        header_stream << static_cast<quint8>('S');
        header_stream << static_cast<quint8>('t');
        header_stream << static_cast<quint8>('o');
        header_stream << static_cast<quint8>('V');
        header_stream << static_cast<quint32>(ImagerTCPPacketTypes::SR2IG_CommandsDataPacket);
        header_stream << static_cast<quint32>(data.size());
        QByteArray packet;
        packet.append(header);
        packet.append(data);
        TcpSocket->write(packet);
        TcpSocket->flush();
        Helicopter->instructor->Commands.PacketReady = false;

        qDebug() << _Instructor->Commands;
    }
}

//[ ID=14 # Server >> Visual ] Текстовое сообщение
void Helicopter_TCP_Client_Imager_Sfera::SendMessagesDataPacket()
{
    if (!Helicopter->instructor->Messages.PacketReady) return;

    if (TcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        qDebug() << "<--" << __FUNCTION__;
        const auto& _Instructor = Helicopter->instructor.data();
        QByteArray data;
        QDataStream data_stream(&data, QIODevice::WriteOnly);
        data_stream.setByteOrder(QDataStream::LittleEndian);
        data_stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
        data_stream << static_cast<quint8>(_Instructor->Messages.MessageType);
        data_stream << static_cast<QString>(_Instructor->Messages.MessageText);

        QByteArray header;
        QDataStream header_stream(&header, QIODevice::WriteOnly);
        header_stream.setByteOrder(QDataStream::LittleEndian);
        header_stream << static_cast<quint8>('S');
        header_stream << static_cast<quint8>('t');
        header_stream << static_cast<quint8>('o');
        header_stream << static_cast<quint8>('V');
        header_stream << static_cast<quint32>(ImagerTCPPacketTypes::SR2IG_MessagesDataPacket);
        header_stream << static_cast<quint32>(data.size());
        QByteArray packet;
        packet.append(header);
        packet.append(data);
        TcpSocket->write(packet);
        TcpSocket->flush();
        Helicopter->instructor->Messages.PacketReady = false;

        qDebug() << _Instructor->Messages;
    }
}

//[ ID=16 # Server >> Visual] Управление мишенями
void Helicopter_TCP_Client_Imager_Sfera::SendTargetsControlDataPacket()
{
    if (!Helicopter->imager->TargetsControl.PacketReady) return;

    if (TcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        qDebug() << "<--" << __FUNCTION__;
        const auto& _Imager = Helicopter->imager.data();
        QByteArray data;
        QDataStream data_stream(&data, QIODevice::WriteOnly);
        data_stream.setByteOrder(QDataStream::LittleEndian);
        data_stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
        data_stream << static_cast<qint32>(_Imager->TargetsControl.mapTargetsCount);
        for (int i = 0; i < _Imager->TargetsControl.mapTargetsCount; i++)
        {
            data_stream << static_cast<qint32>(_Imager->TargetsControl.targetControlList[i].ObjectiveUID);
            data_stream << static_cast<qint32>(_Imager->TargetsControl.targetControlList[i].TargetUID);
            data_stream << static_cast<quint8>(_Imager->TargetsControl.targetControlList[i].TargetState);
        }

        QByteArray header;
        QDataStream header_stream(&header, QIODevice::WriteOnly);
        header_stream.setByteOrder(QDataStream::LittleEndian);
        header_stream << static_cast<quint8>('S');
        header_stream << static_cast<quint8>('t');
        header_stream << static_cast<quint8>('o');
        header_stream << static_cast<quint8>('V');
        header_stream << static_cast<quint32>(ImagerTCPPacketTypes::SR2IG_TargetsControlDataPacket);
        header_stream << static_cast<quint32>(data.size());
        QByteArray packet;
        packet.append(header);
        packet.append(data);
        TcpSocket->write(packet);
        TcpSocket->flush();
        Helicopter->imager->TargetsControl.PacketReady = false;

        qDebug() << _Imager->TargetsControl;
    }
}

void Helicopter_TCP_Client_Imager_Sfera::SendCargoControlDataPacket( )
{
    if ( TcpSocket->state( ) == QAbstractSocket::ConnectedState )
    {
        qDebug( ) << "<--" << __FUNCTION__;
        //const auto& _Instructor = Helicopter->instructor.data( );
        QByteArray data;
        QDataStream data_stream( &data, QIODevice::WriteOnly );
        data_stream.setByteOrder( QDataStream::LittleEndian );
        data_stream.setFloatingPointPrecision( QDataStream::SinglePrecision );

        data_stream << static_cast<qint32>( 1 );

        QByteArray header;
        QDataStream header_stream( &header, QIODevice::WriteOnly );
        header_stream.setByteOrder( QDataStream::LittleEndian );
        header_stream << static_cast<quint8>( 'S' );
        header_stream << static_cast<quint8>( 't' );
        header_stream << static_cast<quint8>( 'o' );
        header_stream << static_cast<quint8>( 'V' );
        header_stream << static_cast<quint32>( ImagerTCPPacketTypes::SR2IG_CargoDataPacket );
        header_stream << static_cast<quint32>( data.size( ) );
        QByteArray packet;
        packet.append( header );
        packet.append( data );
        TcpSocket->write( packet );
        TcpSocket->flush( );
    }
}

void Helicopter_TCP_Client_Imager_Sfera::SendExitDataPacket( int cause)
{
    qDebug( ) << "<--" << __FUNCTION__;
    if ( TcpSocket->state( ) == QAbstractSocket::ConnectedState )
    {
        QByteArray data;
        QDataStream data_stream( &data, QIODevice::WriteOnly );
        data_stream.setByteOrder( QDataStream::LittleEndian );
        data_stream.setFloatingPointPrecision( QDataStream::SinglePrecision );

        data_stream << static_cast<qint8>( cause );

        QByteArray header;
        QDataStream header_stream( &header, QIODevice::WriteOnly );
        header_stream.setByteOrder( QDataStream::LittleEndian );
        header_stream << static_cast<quint8>( 'S' );
        header_stream << static_cast<quint8>( 't' );
        header_stream << static_cast<quint8>( 'o' );
        header_stream << static_cast<quint8>( 'V' );
        header_stream << static_cast<quint32>( ImagerTCPPacketTypes::SR2IG_ExitStateDataPacket );
        header_stream << static_cast<quint32>( data.size( ) );
        QByteArray packet;
        packet.append( header );
        packet.append( data );
        TcpSocket->write( packet );
        TcpSocket->flush( );
    }
}

//[ ID=18 # Server >> Visual] Cостояние оружии
void Helicopter_TCP_Client_Imager_Sfera::SendAmmoDataPacket()
{
//    if (!Helicopter->instructor->AmmunitionInPack.PacketReady) return;

    if (TcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        qDebug() << "<--" << __FUNCTION__;
        const auto& _Instructor = Helicopter->instructor.data();
        QByteArray data;
        QDataStream data_stream(&data, QIODevice::WriteOnly);
        data_stream.setByteOrder(QDataStream::LittleEndian);
        data_stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
        data_stream << _Instructor->AmmunitionInPack;

        QByteArray header;
        QDataStream header_stream(&header, QIODevice::WriteOnly);
        header_stream.setByteOrder(QDataStream::LittleEndian);
        header_stream << static_cast<quint8>('S');
        header_stream << static_cast<quint8>('t');
        header_stream << static_cast<quint8>('o');
        header_stream << static_cast<quint8>('V');
        header_stream << static_cast<quint32>(ImagerTCPPacketTypes::SR2IG_WeaponDataPacket);
        header_stream << static_cast<quint32>(data.size());
        QByteArray packet;
        packet.append(header);
        packet.append(data);
        TcpSocket->write(packet);
        TcpSocket->flush();
        Helicopter->instructor->AmmunitionInPack.PacketReady = false;

        qDebug() << _Instructor->AmmunitionInPack;
    }
}

//[ ID=101 # Server >> Visual] Состояние борта - лампочки, винты, термические ловушки
void Helicopter_TCP_Client_Imager_Sfera::SendEffectsDataPacket()
{
    static UINT64 count;
    //if (!Helicopter->_BoardSystem->changes) return;

    if (TcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        //qDebug() << "<--" << __FUNCTION__;
        QByteArray data;
        QDataStream data_stream(&data, QIODevice::WriteOnly);
        data_stream.setByteOrder(QDataStream::LittleEndian);
        data_stream.setFloatingPointPrecision(QDataStream::SinglePrecision); // одинарная точность float
        data_stream << Helicopter->_BoardSystem->sx16_horiz;              // управление прожектором SX16 по горизонтали
        data_stream << Helicopter->_BoardSystem->sx16_vert;               // управление прожектором SX16 по вертикали
        data_stream << Helicopter->_BoardSystem->sx16_focus;              // управление прожектором SX16 фокус светового пятна
        data_stream << Helicopter->_BoardSystem->fpp_l_horiz;			    // управление левый ФПП по горизонтали
        data_stream << Helicopter->_BoardSystem->fpp_l_vert;			    // управление левый ФПП по вертикали
        data_stream << Helicopter->_BoardSystem->fpp_r_horiz;			    // управление правый ФПП по горизонтали
        data_stream << Helicopter->_BoardSystem->fpp_r_vert;              // управление правый ФПП по вертикали
        data_stream << count++;                                     // SampleNr (номер сэмпла)
        data_stream << Helicopter->_BoardSystem->ANO;                     // Включение ламп АНО
        data_stream << Helicopter->_BoardSystem->beacon_up;               // Включение верхнего проблескового маяка
        data_stream << Helicopter->_BoardSystem->beacon_down;             // Включение нижнего проблескового маяка
        data_stream << Helicopter->_BoardSystem->light_drill;             // Включение строевых огней
        data_stream << Helicopter->_BoardSystem->light_contur;            // Включение контурных огней
        data_stream << Helicopter->_BoardSystem->light_taxiing;           // Включение рулежных огней
        data_stream << Helicopter->_BoardSystem->FPP_l;                   // Включение левой фары ФПП
        data_stream << Helicopter->_BoardSystem->FPP_r;                   // Включение правой фары ФПП
        data_stream << Helicopter->_BoardSystem->SX16;                    // Включение прожектора SX-16
        data_stream << Helicopter->_BoardSystem->thermal_trap_l;          // Отстрел тепловых ловушек по правому борту
        data_stream << Helicopter->_BoardSystem->thermal_trap_r;          // Отстрел тепловых ловушек по левому борту
        data_stream << Helicopter->PC.pkt_x;                              // прицеливание из ПКТ по оси X
        data_stream << Helicopter->PC.pkt_y;                              // прицеливание из ПКТ по оси Y
        
        Helicopter->_BoardSystem->changes = false;

        QByteArray header;
        QDataStream header_stream(&header, QIODevice::WriteOnly);
        header_stream.setByteOrder(QDataStream::LittleEndian);
        header_stream << static_cast<quint8>('S');
        header_stream << static_cast<quint8>('t');
        header_stream << static_cast<quint8>('o');
        header_stream << static_cast<quint8>('V');
        header_stream << static_cast<quint32>(ImagerTCPPacketTypes::SR2IG_DriverStateDataPacket);
        header_stream << static_cast<quint32>(data.size());
        QByteArray packet;
        packet.append(header);
        packet.append(data);
        TcpSocket->write(packet);

        TcpSocket->flush();
    }
}



//[ ID=102 # Server >> Visual] Координаты техники, положение вертолета
void Helicopter_TCP_Client_Imager_Sfera::SendHelicopterMovingDataPacket()
{
    //static bool first = true;
    //if ( first )
    //{
    //    first = false;
    //    //qDebug() << "<--" << __FUNCTION__;
    //}
    //qDebug( ) << "cycle_time =" <<cycle_time( );
    if (TcpSocket->state() == QAbstractSocket::ConnectedState && Helicopter->imager->ImagerWork == true)
    {
        const auto& FPar = Helicopter->Model_->GetFPar();
        QByteArray data;
        QDataStream data_stream(&data, QIODevice::WriteOnly);
        data_stream.setByteOrder(QDataStream::LittleEndian);
        data_stream.setFloatingPointPrecision(QDataStream::DoublePrecision); // двойная точность double
        data_stream << Helicopter->_BoardSystem->Latitude;            // Широта
        data_stream << Helicopter->_BoardSystem->Longitude;           // Долгота
        data_stream << Helicopter->_BoardSystem->BarHeight; -2;           // Высота визуальной модели 2.83, математической 2.53
        data_stream.setFloatingPointPrecision(QDataStream::SinglePrecision); // одинарная точность float
        data_stream << Helicopter->_BoardSystem->get_vis_roll();      // Крен
        data_stream << Helicopter->_BoardSystem->get_vis_pitch( ) - 3.35f;  // Тангаж, в визуальной модели 0 угол равен стояночном 3,35
        data_stream << Helicopter->_BoardSystem->get_vis_course();    // Курс
        data_stream << FPar->Vpr * MS2KM;					    // скорость вертолета, горизонтальная
        data_stream << FPar->Par095;						    // вертикальная скорость	
        data_stream << FPar->Par148;						    // скольжение	
        data_stream << FPar->Par141 * 180.f / PI;               // шаг винта
        data_stream << Helicopter->_BoardSystem->NV;                      // Скорость вращения винтов
        data_stream << Helicopter->_BoardSystem->_Engine_1->eng;          // Обороты Левого двигателя
        data_stream << Helicopter->_BoardSystem->_Engine_2->eng;          // Обороты Правого двигателя

        QByteArray header;
        QDataStream header_stream(&header, QIODevice::WriteOnly);
        header_stream.setByteOrder(QDataStream::LittleEndian);
        header_stream << static_cast<quint8>('S');
        header_stream << static_cast<quint8>('t');
        header_stream << static_cast<quint8>('o');
        header_stream << static_cast<quint8>('V');
        header_stream << static_cast<quint32>(ImagerTCPPacketTypes::SR2IG_CommanderStateDataPacket);
        header_stream << static_cast<quint32>(data.size());
        QByteArray packet;
        packet.append(header);
        packet.append(data);
        TcpSocket->write(packet);
        TcpSocket->flush();
        ////qDebug() << "Lat:" << Helicopter->_BoardSystem->Latitude << "Long:" << Helicopter->_BoardSystem->Longitude
        //    << "Alt:" << Helicopter->_BoardSystem->BarHeight << "\n[" << Helicopter->_BoardSystem->get_vis_roll()
        //    << Helicopter->_BoardSystem->get_vis_pitch() << Helicopter->_BoardSystem->get_vis_course() << "]";
    }
    else
    {
        Helicopter->imager->ImagerWork = false;
    }
}

enum e_weapon_type { wp_unknown = 0, wp_nrs = 17, wp_fab = 18, wp_mine = 19, wp_gsh23 = 20 };
enum e_wp_mode { mod_unknown = 0, mod_single, mod_short, mod_mid, mod_long };

//[ ID=103 # Server >> Visual] Стрельба 
// TODO стрельбу лучше сделать эмиттером сигнала, а не постоянным опросом как было у головина
void Helicopter_TCP_Client_Imager_Sfera::SendFireDataPacket()
{
//    if (!Helicopter->_BoardSystem->b_fire) return;

    qDebug() << "<--" << __FUNCTION__;
    // TODO тут сделать пакет-структуру для выстрела
    if (TcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        QByteArray data;
        QDataStream data_stream(&data, QIODevice::WriteOnly);
        data_stream.setByteOrder(QDataStream::LittleEndian);
        data_stream << static_cast<bool>(Helicopter->_BoardSystem->b_fire);   // Стрельба	
        data_stream << static_cast<quint8>( Helicopter->WeaponPult.sw_weapon_type( ) );	    // Тип оружия 12-15 - РС ФАБ Мины УПК23
        data_stream << static_cast<quint8>(Helicopter->WeaponPult.sw_burst());      // Режим стрельбы 
        data_stream << static_cast<quint8>( Helicopter->WeaponPult.sw_weapon_type( ) );// ( 0 );      // Тип пусковой установки

        QByteArray header;
        QDataStream header_stream(&header, QIODevice::WriteOnly);
        header_stream.setByteOrder(QDataStream::LittleEndian);
        header_stream << static_cast<quint8>('S');
        header_stream << static_cast<quint8>('t');
        header_stream << static_cast<quint8>('o');
        header_stream << static_cast<quint8>('V');
        header_stream << static_cast<quint32>(ImagerTCPPacketTypes::SR2IG_GunnerStateDataPacket);
        header_stream << static_cast<quint32>(data.size());
        QByteArray packet;
        packet.append(header);
        packet.append(data);
        TcpSocket->write(packet);
        TcpSocket->flush();
        qDebug() << static_cast<bool>(Helicopter->_BoardSystem->b_fire)
            << static_cast<quint8>( Helicopter->WeaponPult.sw_weapon_type( ) )
            << static_cast<quint8>( Helicopter->WeaponPult.sw_burst( ) );
    }
}

//[ ID=103 # Server >> Visual] Стрельба 
// TODO стрельбу лучше сделать эмиттером сигнала, а не постоянным опросом как было у головина
void Helicopter_TCP_Client_Imager_Sfera::SendPKTFireDataPacket( bool fire )
{
    qDebug() << "<--" << __FUNCTION__;
    if (TcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        QByteArray data;
        QDataStream data_stream(&data, QIODevice::WriteOnly);
        data_stream.setByteOrder(QDataStream::LittleEndian);
        data_stream << static_cast<bool>( fire );       // Стрельба	
        data_stream << static_cast<quint8>( 2 );        // Тип оружия ПКТ
        data_stream << static_cast<quint8>( 3 );        // Режим стрельбы очередь 
        data_stream << static_cast<quint8>( 2 );        // Тип пусковой установки ПКТ

        QByteArray header;
        QDataStream header_stream(&header, QIODevice::WriteOnly);
        header_stream.setByteOrder(QDataStream::LittleEndian);
        header_stream << static_cast<quint8>('S');
        header_stream << static_cast<quint8>('t');
        header_stream << static_cast<quint8>('o');
        header_stream << static_cast<quint8>('V');
        header_stream << static_cast<quint32>(ImagerTCPPacketTypes::SR2IG_GunnerStateDataPacket);
        header_stream << static_cast<quint32>(data.size());
        QByteArray packet;
        packet.append(header);
        packet.append(data);
        TcpSocket->write(packet);
        TcpSocket->flush();
        qDebug() << static_cast<bool>(Helicopter->_BoardSystem->b_fire)
            << static_cast<quint8>( Helicopter->WeaponPult.sw_weapon_type( ) )
            << static_cast<quint8>( Helicopter->WeaponPult.sw_burst( ) );
    }
}

// [ ID=4 # Visual >> Server] Состояние симулятора
// это сообщение от визуалки постоянно спамит не важными для вертолета состоянием.
void Helicopter_TCP_Client_Imager_Sfera::GetImagerState(QDataStream& _stream)
{
    // //qDebug() << ">--" << __FUNCTION__; // это сообщенеи спамится очень часто
    const auto& _Imager = Helicopter->imager.data();
    qint32 WheelsCount = 0;

    _Imager->ImagerState.PacketReady = true;
    _stream >> _Imager->ImagerState.X;
    _stream >> _Imager->ImagerState.Y;
    _stream >> _Imager->ImagerState.Z;
    _stream >> _Imager->ImagerState.Roll;
    _stream >> _Imager->ImagerState.Pitch;
    _stream >> _Imager->ImagerState.Course;
    _stream >> _Imager->ImagerState.Sail;
    _stream >> _Imager->ImagerState.Surface;
    _stream >> _Imager->ImagerState.Speed;
    _stream >> _Imager->ImagerState.WheelsCount;
    _Imager->ImagerState.WheelsList.clear();
    WheelsCount = _Imager->ImagerState.WheelsCount;
    while (WheelsCount > 0)
    {
        Wheels wheel;
        _stream >> wheel.wheelSpeed;
        _Imager->ImagerState.WheelsList << wheel;
        WheelsCount--;
    }
    _stream >> _Imager->ImagerState.EngineTorque;
    _stream >> _Imager->ImagerState.RPM;
    _stream >> _Imager->ImagerState.AmmoLoad;
    ////qDebug() << _Imager->ImagerState;
}

// [ID = 6 # Visual >> Server] Готовность визуалки
void Helicopter_TCP_Client_Imager_Sfera::GetImagerReadyDataPacket(QDataStream& _stream)
{
	qDebug( ) << ">--" << __FUNCTION__;
    const auto& _Imager = Helicopter->imager;
    quint8 simulatorType = 0;
    _stream >> simulatorType;
    _Imager->ImagerReady.simulatorType = static_cast<SimulatorTypes>(simulatorType);
    _stream >> _Imager->ImagerReady.ImagerReady;
	Helicopter->imager->ImagerWork = _Imager->ImagerReady.ImagerReady;
    _Imager->ImagerReady.PacketReady = true;
    qDebug( ) << _Imager->ImagerReady;
	emit imagerReady( );
}

// [ID = 15 # Visual >> Server] Данные о готовности компонентов
void Helicopter_TCP_Client_Imager_Sfera::GetImagerElementsStateDataPacket(QDataStream& _stream)
{
    //qDebug() << ">--" << __FUNCTION__;
    const auto& _Imager = Helicopter->imager.data();
    qint32 ElementsCount = 0;

    qint32 ElementNameSize = 0;
    QByteArray ElementName = 0;
    qint32 CommentSize = 0;
    QByteArray Comment = 0;
    quint8 ElementNameDetail = 0;
    quint8 CommentDetail = 0;

    _Imager->ImagerElementsState.PacketReady = true;
    _stream >> _Imager->ImagerElementsState.ElementsCount;
    _Imager->ImagerElementsState.ElementsStateList.clear();

    ElementsCount = _Imager->ImagerElementsState.ElementsCount;
    while (ElementsCount > 0)
    {
        Elements elements;
        _stream >> elements.ElementType;
        _stream >> elements.ElementState;
        _stream >> elements.ElementNameSize;
        ElementNameSize = elements.ElementNameSize;
        while (ElementNameSize > 0)
        {
            _stream >> ElementNameDetail;
            ElementName.append(ElementNameDetail);
            ElementNameSize--;
        }
        elements.ElementName = QTextCodec::codecForMib(1015)->toUnicode(ElementName);
        ElementName = 0;
        _stream >> elements.CommentSize;
        CommentSize = elements.CommentSize;
        while (CommentSize > 0)
        {
            _stream >> CommentDetail;
            Comment.append(CommentDetail);
            CommentSize--;
        }
        elements.Comment = QTextCodec::codecForMib(1015)->toUnicode(Comment);
        Comment = 0;
        _Imager->ImagerElementsState.ElementsStateList << elements;
        ElementsCount--;
    }
    //qDebug() << _Imager->ImagerElementsState;
}

// [ ID=305 # Visual >> Server ] Состояние техники на карте - ВЫСОТОМЕР
void Helicopter_TCP_Client_Imager_Sfera::GetHeightDataPacket(QDataStream& _stream)
{
    //    //qDebug() << ">--" << __FUNCTION__;
    float gear1,gear2,gear3;
    float alt;
    //int id;
    //auto FP = Helicopter->Model_->GetFPar();
    _stream >> alt; // Helicopter->_BoardSystem->GeoHeight;
    _stream >> gear1;
    _stream >> gear2;
    _stream >> gear3;  // высота стоек шасси, пока не нужна
    
    // корректруем высоту от карты, так как она показывает высоту до шасси, а модели нужно до центра масс
    // 2.649 взята из первого расчета модели для веса в 8,4 тонны, это не совсем правильно.
    // Удовенко говорил, что за ~20см до касания земли он начинал игнорировать показания внешней высоты от визуалки и считал ее самомстоятельно.
    Helicopter->_BoardSystem->GeoHeight = alt;

//#ifdef TRACE
    //qDebug() << QTime::currentTime() << "Imager Alt" << alt;
//#endif
}

// [ ID=1001 # Visual >> Server ] Событие изменения состояния мишени
void Helicopter_TCP_Client_Imager_Sfera::GetBallisticDataPacket(QDataStream& _stream)
{
    //qDebug() << "-->" << __FUNCTION__;
    const auto& _Imager = Helicopter->imager.data();
    _Imager->Ballistic.PacketReady = true;
    _stream >> _Imager->Ballistic.ObjectiveUID;
    _stream >> _Imager->Ballistic.TargetUID;
    _stream >> _Imager->Ballistic.ObjectiveType;
    _stream >> _Imager->Ballistic.Hit;
    _stream >> _Imager->Ballistic.Radius;
    _stream >> _Imager->Ballistic.GunType;
    _stream >> _Imager->Ballistic.TargetState;
    _stream >> _Imager->Ballistic.TriggerID;
    _stream >> _Imager->Ballistic.Direction;
    _stream >> _Imager->Ballistic.Hit_X;
    _stream >> _Imager->Ballistic.Hit_Y;
    qDebug() << _Imager->Ballistic;
}

// [ ID=1002 # Visual >> Server ] Событие прохождения препятствия (тригеры)
void Helicopter_TCP_Client_Imager_Sfera::GetTriggerDataPacket(QDataStream& _stream)
{
    qDebug() << "-->" << __FUNCTION__;
    const auto& _Imager = Helicopter->imager.data();
    _Imager->Triggers.PacketReady = true;
    _stream >> _Imager->Triggers.ObstacleUID;
    _stream >> _Imager->Triggers.ObstacleType;
    _stream >> _Imager->Triggers.ObstacleIn;
    _stream >> _Imager->Triggers.ObstacleOut;
    _stream >> _Imager->Triggers.Trigger;
    qDebug() << _Imager->Triggers;
}

// [ ID=1003 # Visual >> Server ] Координаты промаха
void Helicopter_TCP_Client_Imager_Sfera::GetMissedEventPacked(QDataStream& _stream)
{
    qDebug() << "-->" << __FUNCTION__;
    const auto& _Imager = Helicopter->imager.data();
    _Imager->Missed.PacketReady = true;
    _stream >> _Imager->Missed.X;
    _stream >> _Imager->Missed.Y;
    _stream >> _Imager->Missed.Missed;
    qDebug() << _Imager->Missed;
}

// [ID = 1005 # Visual >> Server] Работа с подвесом
void Helicopter_TCP_Client_Imager_Sfera::GetCargoEventPacked( QDataStream& _stream )
{
    qDebug( ) << "-->" << __FUNCTION__;
    const auto& _Imager = Helicopter->imager.data( );
    _Imager->Cargo.PacketReady = true;
    _stream >> _Imager->Cargo.Load;
    emit CargoHook( _Imager->Cargo.Load );
    qDebug( ) << _Imager->Cargo;
}

void Helicopter_TCP_Client_Imager_Sfera::GetAmmoEventPacked( QDataStream& _stream )
{
    qDebug( ) << "-->" << __FUNCTION__;
    const auto& _Instructor = Helicopter->instructor.data( );

    _Instructor->AmmunitionOutPack.PacketReady = true;
    _Instructor->AmmunitionOutPack.AmmunitionList.clear( );
    _stream >> _Instructor->AmmunitionOutPack;
    emit AmmoOut( );
    qDebug( ) << _Instructor->AmmunitionOutPack;
}



/******************Для Клименкто*****************************************************************/

void Helicopter_TCP_Client_Imager_Sfera::SendHelicopterMovingPacket()
{
    if (TcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        if (tcp_port == 10000) { // для визуалки AES
            //qDebug() << "Ошибка, данный пакет для визуалки 122";
            return;
        }
        static UINT64 count;
        using namespace std::chrono;

        auto& hp = Helicopter->imager->GetHM();
        auto FPar = Helicopter->Model_->GetFPar();

        hp.Latitude  = Helicopter->_BoardSystem->Latitude;  // направлен на север
        hp.Longitude = Helicopter->_BoardSystem->Longitude; // направлен на восток
        hp.BarHeight = Helicopter->_BoardSystem->BarHeight;
        hp.Roll = Helicopter->_BoardSystem->get_vis_roll();     //FPar->Roll * 180 / PI;
        hp.Pich = Helicopter->_BoardSystem->get_vis_pitch();    // FPar->Pitch * 180 / PI;
        hp.Course = Helicopter->_BoardSystem->get_vis_course(); //(2.f * PI - FPar->Yaw) * 180.f / PI;
        hp.HelixRotationSpeed = Helicopter->_BoardSystem->NV;  // FPar->RPMnv;
        hp.LeftEngineRotationSpeed = Helicopter->_BoardSystem->_Engine_1->eng;   //FPar->Par152;
        hp.RightEngineRotationSpeed = Helicopter->_BoardSystem->_Engine_2->eng;  // FPar->Par153;
        hp.HorizontalSx16 = Helicopter->_BoardSystem->sx16_horiz;                // управление прожектором SX16 по горизонтали
        hp.VerticalSx16 = Helicopter->_BoardSystem->sx16_vert;                   // управление прожектором SX16 по вертикали
        hp.FocusSx16 = Helicopter->_BoardSystem->sx16_focus;                     // управление прожектором SX16 фокус светового пятна
        hp.LeftFppHorizontal = Helicopter->_BoardSystem->fpp_l_horiz;            // управление левый ФПП по горизонтали
        hp.LeftFppVertical = Helicopter->_BoardSystem->fpp_l_vert;               // управление левый ФПП по вертикали
        hp.RightFppHorizontal = Helicopter->_BoardSystem->fpp_r_horiz;           // управление правый ФПП по горизонтали
        hp.RightFppVertical = Helicopter->_BoardSystem->fpp_r_vert;              // управление правый ФПП по вертикали
        hp.SampleNr++;// = count++;
        hp.timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(system_clock::now() - start).count();
        hp.matmodelX = 0;                   // matmodelX   дистанция по оси Х от стартового положения из мат модели в м.
        hp.matmodelY = 225;                 // matmodelY   дистанция по оси Y от стартового положения из мат модели в м.
        hp.matmodelZ = 0;                   // matmodelZ   дистанция по оси Z от стартового положения из мат модели в м.
        hp.Cargo = 0;                       // похоже, это вес подвеса в кг
        hp.un2 = 100;                       // значение не понятно, но оно передается из имитатора в таком виде, чем задается пока не разобрался
        hp.CableLen = 0.;                   // длинна подвеса
        hp.speed = FPar->Vpr*MS2KM;               // скорость вертолета, горизонтальная
        hp.vspeed = FPar->Par095;               // вертикальная скорость
        hp.sideslip = FPar->Par148;             // скольжение
        hp.proppitch = FPar->Par141 * 180.f / PI; // шаг винта
        hp.dbl4 = 0;

        QByteArray data;
        QDataStream data_stream(&data, QIODevice::WriteOnly);
        data_stream.setByteOrder(QDataStream::LittleEndian);
        data_stream.setFloatingPointPrecision(QDataStream::DoublePrecision);
        // двойная точность double
        data_stream << hp.PacketType;                   //Тип  пакета
        data_stream << hp.PacketSize;                   //Размер пакета
        data_stream << hp.ID;                           //ID вертолета
        data_stream << hp.Latitude;                     //Широта
        data_stream << hp.Longitude;                    //Долгота
        data_stream << hp.BarHeight;                    //Высота
        data_stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
        // одинарная точность float
        data_stream << hp.Roll;                         //Крен
        data_stream << hp.Pich;                         //Тангаж
        data_stream << hp.Course;                       //Курс
        data_stream << hp.HelixRotationSpeed;           //Скорость вращения винтов
        data_stream << hp.LeftEngineRotationSpeed;      //Обороты  Левого двигателя
        data_stream << hp.RightEngineRotationSpeed;     //Обороты  Правого двигателя
        data_stream.setFloatingPointPrecision(QDataStream::DoublePrecision);
        // двойная точность double
        data_stream << hp.HorizontalSx16;               //управление прожектором SX16 по горизонтали
        data_stream << hp.VerticalSx16;                 //управление прожектором SX16 по вертикали
        data_stream << hp.FocusSx16;                    //управление прожектором SX16 фокус светового пятна
        data_stream << hp.LeftFppHorizontal;            //управление левый ФПП по горизонтали
        data_stream << hp.LeftFppVertical;              //управление левый ФПП по вертикали
        data_stream << hp.RightFppHorizontal;           //управление правый ФПП по горизонтали
        data_stream << hp.RightFppVertical;             //управление правый ФПП по вертикали
        data_stream << hp.SampleNr;                     //SampleNr(номер сэмпла)
        data_stream << hp.timestamp;                    // Timestamp ( текущее время )
        data_stream << hp.matmodelX;                    //matmodelX   дистанция по оси Х от стартового положения из мат модели в м.
        data_stream << hp.matmodelY;                    //matmodelY   дистанция по оси Y от стартового положения из мат модели в м.
        data_stream << hp.matmodelZ;                    //matmodelZ   дистанция по оси Z от стартового положения из мат модели в м.
        data_stream << hp.Cargo;                        // похоже, это вес подвеса в кг
        data_stream << hp.un2;                          // значение не понятно, но оно передается из имитатора в таком виде, чем задается пока не разобрался
        data_stream << hp.CableLen;                     // длинна подвеса
        data_stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
        data_stream << hp.speed;					    // скорость вертолета, горизонтальная
        data_stream << hp.vspeed;						// вертикальная скорость	
        data_stream << hp.sideslip;						// скольжение	
        data_stream << hp.proppitch;                    // шаг винта
        data_stream.setFloatingPointPrecision(QDataStream::DoublePrecision);
        data_stream << hp.dbl4;                         // назначение неизвестно

        TcpSocket->write(data);
        TcpSocket->flush();
        ////qDebug() << "Helicopter->_BoardSystem->BarHeight =" << Helicopter->_BoardSystem->BarHeight;
    }
}

void Helicopter_TCP_Client_Imager_Sfera::SendHelicopterStartPacket()
{
    if (TcpSocket->state() == QAbstractSocket::ConnectedState)
	{
        QByteArray data;
		QDataStream data_stream(&data, QIODevice::WriteOnly);
		data_stream.setByteOrder(QDataStream::LittleEndian);
		data_stream << 4u;                   //Тип  пакета
		data_stream << Helicopter->runway_.json_start.toUtf8();  //json string длинна ужеесть в QString

		TcpSocket->write(data);
		TcpSocket->flush();
	}
}

void Helicopter_TCP_Client_Imager_Sfera::SendHelicopterStatPacket()
{
    if (TcpSocket->state() == QAbstractSocket::ConnectedState && Helicopter->_BoardSystem->StateChanged())
	{
        QByteArray data;
		QDataStream data_stream(&data, QIODevice::WriteOnly);
		data_stream.setByteOrder(QDataStream::LittleEndian);
		data_stream << 2u;                   //Тип  пакета
		data_stream << Helicopter->_BoardSystem->json().toUtf8();  //json string длинна ужеесть в QString

		TcpSocket->write(data);
		TcpSocket->flush();
		//qDebug() << Helicopter->_BoardSystem->json();
	}
}

void Helicopter_TCP_Client_Imager_Sfera::SendHelicopterWeatherPacket()
{
    if (TcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        QByteArray data;
        QDataStream data_stream(&data, QIODevice::WriteOnly);
        data_stream.setByteOrder(QDataStream::LittleEndian);
        data_stream << 3u;                   //Тип  пакета
        data_stream << Helicopter->instructor->weather.json().toUtf8();  //json string длинна ужеесть в QString

        TcpSocket->write(data);
        TcpSocket->flush();
        //qDebug() << Helicopter->instructor->weather.json().toUtf8();
    }
}


