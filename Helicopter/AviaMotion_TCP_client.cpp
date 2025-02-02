#include "AviaMotion_TCP_client.h"
#include "Helicopter_Model.h"


T_TCP_Client_AviaMotionSystem::T_TCP_Client_AviaMotionSystem(Helicopter_core* mi17)
{
    Mi17 = mi17;
    TxInterval = Mi17->_ini->LanTxInterval();
    hostaddr = Mi17->_ini->IP_MotionSystemGunner();
    tcp_port = Mi17->_ini->PORT_MotionSystemGunner();

    ConnectToHost(hostaddr, tcp_port);
    TxInterval = Mi17->_BoardSystem->platform_period;  // TODO проверим этот темп, он по умолчанияю в платформе
    TimerTCPSend->setInterval(TxInterval);
    connect(TimerTCPSend, SIGNAL(timeout()), this, SLOT(On_SendTcpData()));
    TimerTCPSend->start();

    //connect(this, SIGNAL(signalConnected()), this, SLOT(slotConnected()), Qt::QueuedConnection);
    //connect(this, SIGNAL(signalDisconnected()), this, SLOT(slotDisconnected()), Qt::QueuedConnection);
    //connect(this, SIGNAL(signalError()), this, SLOT(slotError()), Qt::QueuedConnection);
    connect(TcpSocket, SIGNAL(readyRead()), this, SLOT(On_ReadTcpData()));
}

T_TCP_Client_AviaMotionSystem::~T_TCP_Client_AviaMotionSystem()
{
    qDebug() << __FUNCTION__;

}

//void T_TCP_Client_AviaMotionSystem::slotConnected()
//{
////    _MotionSystem->GunnerMotionSystemActive = ClientConnected();
//    Mi17_->log_->WriteLANLog(LogEvents::TCP_GunnerMotionSystem_connected, "");
//}
//
//void T_TCP_Client_AviaMotionSystem::slotDisconnected()
//{
////    _MotionSystem->GunnerMotionSystemActive = ClientConnected();
//    Mi17_->log_->WriteLANLog(LogEvents::TCP_GunnerMotionSystem_disconnected, "");
//}
//
//void T_TCP_Client_AviaMotionSystem::slotError()
//{
////    _MotionSystem->GunnerMotionSystemActive = ClientConnected();
//    Mi17_->log_->WriteLANLog(LogEvents::TCP_GunnerMotionSystem_Error, socketErrorString);
//}

void T_TCP_Client_AviaMotionSystem::On_SendTcpData()
{
    SendAviaMotionStateDataPacket();
}

void T_TCP_Client_AviaMotionSystem::SendAviaMotionStateDataPacket()
{
//    double MatmodelZ = 0; // Mi17_->_BoardSystem->matmodelY;
    float Roll = Mi17->_BoardSystem->get_ms_roll();       //    / _ini->Divider_MotionSystemGunner();
    float Pitch = Mi17->_BoardSystem->get_ms_pitch();     //    / _ini->Divider_MotionSystemGunner();
    float Course = Mi17->_BoardSystem->get_ms_course();   //    / _ini->Divider_MotionSystemGunner();
    
    auto FP = Mi17->Model_->GetFPar();
    float Ax = FP->Ax;        //  
    float Ay = FP->Ay;        //  
    float Az = FP->Az;        //  
    float Wx = FP->Wx;        //  
    float Wy = FP->Wy;        //  
    float Wz = FP->Wz;        //  
    //float Nx = FP->Nx;        //
    //float Ny = FP->Ny;        //
    //float Nz = FP->Nz;        //
    //Ax = Nx * 9.8f;
    //Ay = Ny * 9.8f;
    //Az = Nz * 9.8f;

    if(TcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        QByteArray data;
        QDataStream data_stream(&data, QIODevice::WriteOnly);
        data_stream.setByteOrder(QDataStream::LittleEndian);
        data_stream << static_cast<double>(0);
        data_stream << static_cast<double>(0);
        data_stream << static_cast<double>(0);    // дистанция по оси Z от стартового положения из мат модели (метры)
        data_stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
        data_stream << static_cast<float>(Roll);          // крен (градусы)
        data_stream << static_cast<float>(Pitch);         // тангаж (градусы)
        data_stream << static_cast<float>(Course);        // направление (градусы)
        data_stream << static_cast<float>(Ax);
        data_stream << static_cast<float>(Az);
        data_stream << static_cast<float>(Ay);
        data_stream << static_cast<float>(Wx);
        data_stream << static_cast<float>(Wz);
        data_stream << static_cast<float>(Wy);
        data_stream << static_cast<quint32>(0);
        QByteArray header;
        QDataStream header_stream(&header, QIODevice::WriteOnly);
        header_stream.setByteOrder(QDataStream::LittleEndian);
        header_stream << static_cast<quint32>(0x7AADF00D);
        header_stream << static_cast<quint16>(MotionSystemTCPPacketTypes::SR2MS_MotionSystemStateDataPacket);
        header_stream << static_cast<quint16>(data.size());
        QByteArray packet;
        packet.append(header);
        packet.append(data);
        TcpSocket->write(packet, packet.length());
        TcpSocket->flush();

//        if(_Instructor->exerciseStarted) _replay->WritePacketData(_Instructor->ExerciseParameters->Uuid,ClientPacketTypes::MotionSystemPacket,quint32(MotionSystemTCPPacketTypes::SR2MS_MotionSystemStateDataPacket),packet);
    }
}

void T_TCP_Client_AviaMotionSystem::On_ReadTcpData()
{
    ReadCommonTcpData();
}

void T_TCP_Client_AviaMotionSystem::ReadCommonTcpData()
{
    bool processedHeader = false;
    MotionSystemTCPHeader MotionAviaHeader;

    if(TcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        quint8 header_length = sizeof(MotionSystemTCPHeader); // 32 + 16 + 16 бит
        quint32 availabeBytes = TcpSocket->bytesAvailable();
        while(availabeBytes >= header_length)
        {
            if(!processedHeader)
            {
                QByteArray header_data = TcpSocket->read(header_length);

                QDataStream header_stream(header_data);
                header_stream.setByteOrder(QDataStream::LittleEndian);
                header_stream.setFloatingPointPrecision(QDataStream::SinglePrecision);

                header_stream >> MotionAviaHeader.MagicNumber;
                header_stream >> MotionAviaHeader.PacketType;
                header_stream >> MotionAviaHeader.DataSize;
                processedHeader = true;
                availabeBytes = availabeBytes - header_length;
            }

            if((processedHeader) && (availabeBytes >= MotionAviaHeader.DataSize))
            {
                QByteArray packet_data = TcpSocket->read(MotionAviaHeader.DataSize);
                QDataStream data_stream(packet_data);
                data_stream.setByteOrder(QDataStream::LittleEndian);
                data_stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
                switch(MotionAviaHeader.PacketType)
                {
                  case static_cast<quint32>(MotionSystemTCPPacketTypes::MS2SR_MotionSystemFeedbackDataPacket):
                     GetAviaMotionFeedbackDataPacket(data_stream);
                  break;
                  default:
                     Mi17->log_->WriteLANLog(LogEvents::TCP_GunnerMotionSystem_Unknown_PacketType, QString::number(MotionAviaHeader.PacketType));
                  break;
                }
            }
            availabeBytes = TcpSocket->bytesAvailable();
            processedHeader = false;
        }
    }
}

void T_TCP_Client_AviaMotionSystem::GetAviaMotionFeedbackDataPacket(QDataStream &_stream)
{
    MotionSystemFeedbackDataPacket AviaMotionFeedback;

    AviaMotionFeedback.PacketReady = true;
    _stream >> AviaMotionFeedback.X;
    _stream >> AviaMotionFeedback.Y;
    _stream >> AviaMotionFeedback.Z;
    _stream >> AviaMotionFeedback.Roll;
    _stream >> AviaMotionFeedback.Pitch;
    _stream >> AviaMotionFeedback.Course;
    //if(int(AviaMotionFeedback.Roll) !=0 || int(AviaMotionFeedback.Pitch)!=0 || int(AviaMotionFeedback.Course)!=0)
    {
        qDebug() << AviaMotionFeedback;
    }
    // тут можно будет сравнить отправленное значение с фактическим, что бы узнать погрешность и задержку
    // 
    //_MotionSystem->GunnerMotionFeedbackEnqueuePacket(GunnerMotionFeedback);
}

QDebug operator<<(QDebug out, const MotionSystemFeedbackDataPacket& AviaMotionFeedback) {
    out << fixed << qSetRealNumberPrecision(3) << "{ "
        << AviaMotionFeedback.X
        << AviaMotionFeedback.Y
        << AviaMotionFeedback.Z
        << AviaMotionFeedback.Roll
        << AviaMotionFeedback.Pitch
        << AviaMotionFeedback.Course
        << "}";
    return out;
}
