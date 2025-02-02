#pragma once

#include "Helicopter_core.h"
#include "Helicopter_Model.h"
#include "Helicopter_OC_tcp_client.h"
#include "Common/SettingsFile.h"
#include <sstream>
#include <iomanip>

// TODO изначально, пакеты для приборов обновлялись раз в секеунду, но они переставали обновляться если не работает модель
// проблема в том, что при работе плеера, модель не работает. Я переделал работу на сигналы от модели, но они 60 раз в секунду
// надо убедится, что это не перегрузит сеть и что то придумать, главное об этом не забыть, поэтому и написал тут
// сиграл конектится в Core

Helicopter_OC_TCP_Client::Helicopter_OC_TCP_Client(Helicopter_core* Helicopter)
{
    Helicopter = Helicopter;

    TxInterval = 1000; // пакеты на сервер отправляются раз в 1 секунду, без вариантов
    hostaddr = Helicopter->_ini->IP_ObjectivControl();
    tcp_port = Helicopter->_ini->PORT_ObjectivControl();
    qDebug() << "Подключение OC к " << hostaddr << ":" << tcp_port;
    ConnectToHost(hostaddr, tcp_port);

    //TimerTCPSend->setInterval(TxInterval);
    //connect(TimerTCPSend,SIGNAL(timeout()),this, SLOT(slotSendTcpData()));
    connect(TimerTCPSend,SIGNAL(timeout()),this, SLOT(slotSendTcpData()));
    connect(this, SIGNAL(signalConnected()), this, SLOT(On_Connected()), Qt::QueuedConnection);
    connect(this, SIGNAL(signalDisconnected()), this, SLOT(On_Disconnected()), Qt::QueuedConnection);
    connect(this, SIGNAL(signalError()), this, SLOT(On_Error()), Qt::QueuedConnection);
    connect(TcpSocket, SIGNAL(readyRead()), this, SLOT(slotReadTcpData()));
}

Helicopter_OC_TCP_Client::~Helicopter_OC_TCP_Client()
{
    //TimerTCPSend->stop();
}

void Helicopter_OC_TCP_Client::Start()
{
    //TimerTCPSend->start();
}

void Helicopter_OC_TCP_Client::Stop()
{
//    TimerTCPSend->stop();
}

void Helicopter_OC_TCP_Client::slotReadTcpData()
{
    ReadCommonTcpData();
}

void Helicopter_OC_TCP_Client::slotSendTcpData()
{
    SendOCDataPacket();
}


void Helicopter_OC_TCP_Client::On_Connected()
{
    qDebug() << "OC Connected";
    _Active = ClientConnected();
    Helicopter->log_->WriteLANLog(LogEvents::TCP_OC_connected, "");
}

void Helicopter_OC_TCP_Client::On_Disconnected()
{
    qDebug() << "OC Disconnected";
    _Active = ClientConnected();
    Helicopter->log_->WriteLANLog(LogEvents::TCP_OC_disconnected, "");
}

void Helicopter_OC_TCP_Client::On_Error()
{
//    qDebug() << "OC Error";
    _Active = ClientConnected();
    Helicopter->log_->WriteLANLog(LogEvents::TCP_OC_Error, socketErrorString);
}

/*
QByteArray block;
QDataStream sendStream(&block, QIODevice::ReadWrite);
sendStream << quint16(0) << str;
sendStream.device()->seek(0);
sendStream << (quint16)(block.size() - sizeof(quint16));
m_ptcpSocket->write(block);
*/

/*
data_stream << static_cast<bool>();
data_stream << static_cast<float>();
data_stream << static_cast<UINT8>();
*/

void Helicopter_OC_TCP_Client::SendOCDataPacket()
{
//    qDebug() << __FUNCTION__;
    if(TcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        QByteArray data;
        QDataStream data_stream(&data, QIODevice::WriteOnly);
        data_stream.setByteOrder(QDataStream::LittleEndian);
        data_stream.setFloatingPointPrecision(QDataStream::SinglePrecision);

        auto FP = Helicopter->Model_->GetFPar();
        auto IP = Helicopter->Model_->GetIPar();

        std::stringstream ss;
        ss << std::setw(10) << std::left << static_cast<int>(FP->Time) << "\t";		// Время [T][c][0]
        ss << std::fixed << std::setprecision(1);
        ss << std::setw(7) << std::right << static_cast<float>(FP->Hgeo-2.48) << "\t";	//Высота геометрическая [Hг][м][0]
        ss << std::setw(6) << std::right << static_cast<float>(FP->Vpr * 3.6) << "\t";	//Скорость приборная [Vпр][км/ч][0]
        ss << std::fixed << std::setprecision(2);
        ss << std::setw(6) << std::right << static_cast<float>(360. - FP->Yaw * Ra) << "\t";	//Гиромагнитный курс [Курс][град][1]
        ss << std::setw(6) << std::right << static_cast<float>(FP->Roll * Ra) << "\t";	//Угол крена [Крен][град][1]
        ss << std::setw(6) << std::right << static_cast<float>(FP->Pitch * Ra) << "\t";	//Угол тангажа [Танг][град][1]
        ss << std::setw(6) << std::right << static_cast<float>(FP->Ny) << "\t";		//Вертикальная перегрузка [Ny][g][1]
        ss << std::setw(6) << std::right << static_cast<float>(FP->Par141 * Ra) << "\t";	//Положение РУ общего шага НВ [РУош][град][1]
        ss << std::setw(6) << std::right << static_cast<float>(FP->RPMnv) << "\t";	//Частота вращения несущего винта[Nнв][%][1]
        ss << std::setw(6) << std::right << static_cast<float>(FP->Par152) << "\t";	//Частота вращения ТК лев.двигателя[Nтк.1][%][1]
        ss << std::setw(6) << std::right << static_cast<float>(FP->Par153) << "\t";	//Частота вращения ТК прав.двигателя[Nтк.2][%][1]
        ss << std::setw(6) << std::right << static_cast<float>(25.61) << "\t";	//Напряжение постоянного тока на акк.шине[U27][В][1]
        ss << static_cast<bool>(FP->Hgeo < 2.52 ? 1 : 0) << "\t";	//Обжатие опор шасси[Ш.обж]
        ss << static_cast<bool>(FP->Hgeo < 8 ? 1 : 0) << "\t";	//Опасная высота с РВ[ВПР]
        ss << static_cast<bool>(0) << "\t";	//Нажатие кнопки выхода на внешнюю радиосвязь[ВРСВ]
        ss << static_cast<bool>(IP->Par029) << "\t";	//Минимальный остаток топлива(270 л)[ТопМин!]
        ss << static_cast<bool>(IP->Par034) << "\t";	//Чрезвычайный режим лев.двигателя[ЧР1]
        ss << static_cast<bool>(IP->Par035) << "\t";	//Чрезвычайный режим прав.двигателя[ЧР2]
        ss << static_cast<bool>(IP->Par048) << "\t";	//Включение каналов крена и тангажа АП - 34Б[АПкртг]
        ss << static_cast<bool>(IP->Par022) << "\t";	//Крен велик[Крен > ]
        ss << static_cast<bool>(0);		//Сброс груза с внешней повески[Сбр.Гр]
        qDebug() << ss.str().c_str();
        data_stream << QString(ss.str().c_str()).toUtf8();


        QByteArray packet;
        packet.append(data);
        TcpSocket->write(packet);
        TcpSocket->flush();

        //if(_Instructor->exerciseStarted) _replay->WritePacketData(_Instructor->ExerciseParameters->Uuid,ClientPacketTypes::ControlsImitatorPacket,quint32(ControlsImitatorTCPPacketTypes::SR2CI_CommanderControlsDataPacket),packet);
    }
}

// Тут ничего не должно приходить, если что то получено, то это мусор, который надо прочитать из сокета и удалить?
void Helicopter_OC_TCP_Client::ReadCommonTcpData()
{
//    bool processedHeader = false;
    if (TcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        auto availabeBytes = TcpSocket->bytesAvailable();
        while (availabeBytes >= 0)
        {
             TcpSocket->read(availabeBytes);
        }
    }
}
