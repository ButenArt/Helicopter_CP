#include "Helicopter_SPU_Left_Pilot_TCP_Client.h"

Helicopter_SPU_Left_Pilot_TCP_Client::Helicopter_SPU_Left_Pilot_TCP_Client(Helicopter_core* Helicopter_)
{
    Helicopter = Helicopter_;

    hostaddr = Helicopter->_ini->IP_Left_Pilot();
    tcp_port = Helicopter->_ini->PORT_Left_Pilot();
    TcpSocket->setSocketOption(QAbstractSocket::LowDelayOption, 1);  // выключаем режим nagle, пакет отправляется сразу, без ожидания дополнительных 

    qDebug() << "Connect Left Pilot to " << hostaddr << ":" << tcp_port;
    ConnectToHost(hostaddr, tcp_port);

    TimerTCPSend->setInterval(TxInterval);
    connect(TimerTCPSend, SIGNAL(timeout()), this, SLOT(On_CheckParams()));

    connect(this, SIGNAL(signalConnected()), this, SLOT(On_Connected()), Qt::QueuedConnection);
    connect(this, SIGNAL(signalDisconnected()), this, SLOT(On_Disconnected()), Qt::QueuedConnection);
    connect(this, SIGNAL(signalError()), this, SLOT(On_Error()), Qt::QueuedConnection);
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Helicopter_SPU_Left_Pilot_TCP_Client::RitaOFFSlot);

    bool* temp_arr[faults_count] = {
        &ri_off,                // 0) Выключение 
        &ri_Hleb,               // 57) Позывной Хлебодаровки i=41
        &ri_Aktobe,             // 58) Позовной Актобе
        &ri_shum,               // 59) Тональный шум
        //&kn53_pozuv             // 60)  позывной ILS
    };

    for (size_t i = 0; i < faults_count; i++)
    {
        faults[i] = temp_arr[i];
        flags[i] = true;
    }
}

Helicopter_SPU_Left_Pilot_TCP_Client::~Helicopter_SPU_Left_Pilot_TCP_Client()
{
}

void Helicopter_SPU_Left_Pilot_TCP_Client::RitaOFFSlot() {
    SendSpuLeftPilotDataPacket(0);
    flag_H = false;
    timer->stop();
}

void Helicopter_SPU_Left_Pilot_TCP_Client::On_CheckParams()
{
    if (Helicopter->lpb.spu.t_radio) {
        if (Helicopter->lpb.spu.t_dr) {
            if (Helicopter->ccp.KN53_pozuv_110) {
                if (!flag_H) {
                    SendSpuLeftPilotDataPacket(60);// 110.5 частота
                    timer->start(9000);
                    flag_H = true;
                }
            }
            else if (Helicopter->ccp.KN53_pozuv_111) {
                if (!flag_H) {
                    SendSpuLeftPilotDataPacket(61);// 111.7 частота
                    timer->start(7000);
                    flag_H = true;
                }
            }
            else if (Helicopter->ccp.KN53_pozuv_113) {
                if (!flag_H) {
                    SendSpuLeftPilotDataPacket(62);// 113.4 частота
                    timer->start(9000);
                    flag_H = true;
                }
            }
            else
                SendSpuLeftPilotDataPacket(0);
        }
        else if (Helicopter->lpb.spu.t_rk1) {
            if (Helicopter->rsp_elp.ark_9.flag_pomeh) {
                if (!flag_H) {
                    SendSpuLeftPilotDataPacket(59); // шум
                    timer->start(2000);
                    flag_H = true;
                }
            }
            if (Helicopter->rsp_elp.ark_9.ritaHleb) {
                if (!flag_H) {
                    SendSpuLeftPilotDataPacket(57); // Хлебодаровка
                    timer->start(8000);
                    flag_H = true;
                }
            }
            else if (Helicopter->rsp_elp.ark_9.ritaAktobe) {
                if (!flag_H) {
                    SendSpuLeftPilotDataPacket(58); // Актобе
                    timer->start(6000);
                    flag_H = true;
                }
            }
            else
                SendSpuLeftPilotDataPacket(0);
        }
        else if (Helicopter->lpb.spu.t_rk2) {
            if (Helicopter->rsp_elp.ark_9.flag_pomeh) {
                if (!flag_H) {
                    SendSpuLeftPilotDataPacket(59); // шум
                    timer->start(2000);
                    flag_H = true;
                }
            }
            else
                SendSpuLeftPilotDataPacket(0);
        }
        else {
            SendSpuLeftPilotDataPacket(0);
        }
    }
    else {
        SendSpuLeftPilotDataPacket(0);
    }
}

void Helicopter_SPU_Left_Pilot_TCP_Client::Start()
{
    TimerTCPSend->start();
}

void Helicopter_SPU_Left_Pilot_TCP_Client::Stop()
{
    TimerTCPSend->stop();
}

void Helicopter_SPU_Left_Pilot_TCP_Client::On_Connected()
{
    qDebug() << "SPU Left Pilot Connected";
}

void Helicopter_SPU_Left_Pilot_TCP_Client::On_Disconnected()
{
    qDebug() << "SPU Left Pilot Disconnected";
}

void Helicopter_SPU_Left_Pilot_TCP_Client::On_Error()
{
}

void Helicopter_SPU_Left_Pilot_TCP_Client::SendSpuLeftPilotDataPacket(const int track_number)
{
    if (TcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        QByteArray data;
        QDataStream data_stream(&data, QIODevice::WriteOnly);
        data_stream.setByteOrder(QDataStream::LittleEndian);
        data_stream << static_cast<quint16>(track_number);
        QByteArray header;
        QDataStream header_stream(&header, QIODevice::WriteOnly);
        header_stream.setByteOrder(QDataStream::LittleEndian);
        header_stream << static_cast<quint32>(10); // Тут номер пакета
        header_stream << static_cast<quint32>(data.size());
        QByteArray packet;
        packet.append(header);
        packet.append(data);
        TcpSocket->write(packet, packet.length());
        TcpSocket->flush();
    }
}
