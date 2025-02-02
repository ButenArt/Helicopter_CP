#include "Helicopter_Rita_TCP_Client.h"

Helicopter_Rita_TCP_Client::Helicopter_Rita_TCP_Client(Helicopter_core* Helicopter_)
{
    Helicopter = Helicopter_;

    hostaddr = Helicopter->_ini->IP_Rita();
    tcp_port = Helicopter->_ini->PORT_Rita();
    TcpSocket->setSocketOption(QAbstractSocket::LowDelayOption, 1);  // выключаем режим nagle, пакет отправляется сразу, без ожидания дополнительных 

    qDebug( ) << "Connect RI 65 to " << hostaddr << ":" << tcp_port;
    ConnectToHost(hostaddr, tcp_port);

    TimerTCPSend->setInterval( TxInterval );
    connect( TimerTCPSend, SIGNAL(timeout()), this, SLOT(On_CheckParams()));

    connect(this, SIGNAL(signalConnected()), this, SLOT(On_Connected()), Qt::QueuedConnection);
    connect(this, SIGNAL(signalDisconnected()), this, SLOT(On_Disconnected()), Qt::QueuedConnection);
    connect(this, SIGNAL(signalError()), this, SLOT(On_Error()), Qt::QueuedConnection);
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Helicopter_Rita_TCP_Client::RitaOFFSlot);

    bool* temp_arr[faults_count] = {
        &Helicopter->cep.i_eng_left_fire,                     // 1)  Пожар в отсеке левого двигателя
        &Helicopter->cep.i_eng_rght_fire,                     // 2)  Пожар в отсеке правого двигателя
        &Helicopter->cep.i_ko_50_fire,                        // 3)  Пожар в отсеке главного редуктора
        &Helicopter->cep.i_red_vsu_fire,                      // 4)  Пожар в отсеке обогревателя
        &Helicopter->instructor->fault.HidroMain,             // 7)  Отказала основная гидросистема
        &pump_ras,                                      // 8)  Отказал насос расходного топливного бака
        &Helicopter->instructor->fault.HidroSecond,           // 10) Отказала дублирующая гидросистема
        &t_overspeed1,                                  // 11) Превышение оборотов свободной турбины левого двигателя
        &t_overspeed2,                                  // 12) Превышение оборотов свободной турбины правого двигателя
        &NV_low,                                        // 14) Низкие обороты несущего винта
        &NV_high,                                       // 15) Высокие обороты несущего винта 
        &vibrLeft,                                      // 17) Опасная вибрация левого двигателя
        &vibrRight,                                     // 18) Опасная вибрация правого двигателя
        &Helicopter->instructor->fault.Gen1,                  // 19) Отказал первый генератор
        &Helicopter->instructor->fault.Gen2,                  // 20) Отказал второй генератор
        &danger_fuel,                                   // 21) Аварийный остаток топлива
        &icing,                                         // 22) Обледенение
        &pump_left,                                     // 25) Отказал насос правого топливного бака
        &pump_right,                                    // 26) Отказал насос левого топливного бака
        &min_P_left,                                    // 28) Минимальное давление масла в левом двигателе
        &min_P_right,                                   // 29) Минимальное давление масла в правом двигателе
        &min_P_red,                                     // 30) Минимальное давление масла в главном редукторе 
        &emerg_left,                                    // 31) Чрезвычайний режим левого двигателя
        &emerg_right,                                   // 32) Чрезвычайний режим правого двигателя
        &Helicopter->rpb.i_ag_brok,                           // 33) Отказ правого АГ, отключи АП
        &Helicopter->lpb.i_ag_left_brok,                      // 34) Отказ левого АГ
        &Helicopter->lpb.i_ag_res_brok,                       // 35) Отказ резервоного АГ
        &Helicopter->instructor->fault.pos_vint,              // 40) Отказала обледенительная система НВ (не моделируется)
        &danger_h,                                      // 41) Опасная высота i = 28
        &left_tf,                                       // 43) Засорение топливного фильтра правого двигателя
        &right_tf,                                      // 44) Засорение топливного фильтра левого двигателя
        &Helicopter->instructor->fault.VU1,                   // 46) Отказ первого выпрямителя
        &Helicopter->instructor->fault.VU2,                   // 47) Отказ второго выпрямителя
        nullptr,                                        // 48) Предельные обороты ВСУ (не моделируется)
        &ERD_left,                                      // 51) Отказ электронного регулятора левого двигателя
        &ERD_right,                                     // 52) Отказ электронного регулятора правого двигателя
        &limited_left,                                  // 53) Ограничение режима левого двигателя
        &limited_right,                                 // 54) Ограничение режима правого двигателя
        &danger_v,                                      // 55) Скорость велика
        &check_ri,                                      // 56) Речевой информатор исправен
        &ri_off,                                        // 0) Выключение РИ
        &Helicopter->rsp_elp.ark_9.ritaHleb,                  // 57) Позывной Хлебодаровки i=41
        &Helicopter->rsp_elp.ark_9.ritaAktobe,                // 58) Позовной Актобе
        &Helicopter->rsp_elp.ark_9.flag_pomeh,                                       // 59) Тональный шум
        &kn53_pozuv                                     // 60  позывной ILS
    };

    for (size_t i = 0; i < faults_count; i++)
    {
        faults[i] = temp_arr[i];
        flags[i] = true;
    }

    isTurnedOn = &Helicopter->lsh_elp.t_ri_65;
}

Helicopter_Rita_TCP_Client::~Helicopter_Rita_TCP_Client()
{
}

void Helicopter_Rita_TCP_Client::RitaOFFSlot() {
    SendRitaDataPacket(0);
    flag_H = false;
    timer->stop();
}

void Helicopter_Rita_TCP_Client::On_CheckParams()
{
    auto FPar = Helicopter->Model_->GetFPar( );
    auto IPar = Helicopter->Model_->GetIPar( );

    vibrLeft = Helicopter->instructor->fault.Vibration_Max_left || Helicopter->instructor->fault.Vibration_left || Helicopter->lpb.i_eng_left_vibr_max;
    vibrRight = Helicopter->instructor->fault.Vibration_Max_right || Helicopter->instructor->fault.Vibration_right || Helicopter->lpb.i_eng_right_vibr_max;
    danger_v = FPar->Vpr > 250 ? true : false;
    NV_low = !!IPar->Par041;
    NV_high = !!IPar->Par040 || Helicopter->instructor->fault.FreqNV || Helicopter->rpb.ite_1t.nv > 100;
    min_P_left = !!IPar->Par038;
    min_P_right = !!IPar->Par039;
    danger_fuel = !!IPar->Par029;
    ERD_left = !!IPar->Par050;
    ERD_right = !!IPar->Par050;
    t_overspeed1 = FPar->Par152 > 100 ? true : false;
    t_overspeed2 = FPar->Par153 > 100 ? true : false;
    emerg_left = IPar->Par034;
    emerg_right = IPar->Par035;
    limited_left = IPar->Par036;
    limited_right = IPar->Par037;
    icing = Helicopter->lp_elp.i_icing;
    min_P_red = Helicopter->instructor->fault.Gear;
    left_tf = Helicopter->lpb.i_eng_left_tf;
    right_tf = Helicopter->lpb.i_eng_right_tf;
    pump_ras = Helicopter->cep.i_fpump_consum_off;
    pump_left = Helicopter->cep.i_fpump_left_off;
    pump_right = Helicopter->cep.i_fpump_right_off;
    check_ri = Helicopter->lsp_elp.t_ri_check;
    if (FPar->Hgeo > 3)
        danger_h = (FPar->Hgeo < Helicopter->lpb.rv_5.index) ? true : false;
    else if (((Helicopter->lpb.rv_5.alt() > 5) && (Helicopter->lpb.rv_5.alt() < Helicopter->lpb.rv_5.index))) {
        if (!flag_H) {
            SendRitaDataPacket(41);
            timer->start(2000);
            flag_H = true;
        }
    }

    if (Helicopter->lsh_elp.t_ri_65 && !Helicopter->lsp_elp.t_ri_off)
        ri_off = false;
    else {
        ri_off = true;
        SendRitaDataPacket(0);
    }
    /*
    if (Helicopter->rsp_elp.ark_9.ritaHleb) {
        ri_Hleb = true;
    }

    if (Helicopter->rsp_elp.ark_9.ritaAktobe) {
        ri_Aktobe = true;
    }

    if (Helicopter->rsp_elp.ark_9.flag_pomeh) {
        ri_shum = true;
        flags[43] = true;
    }
    else {
        if(flags[43])
            ri_off = true;
        ri_shum = false;
        flags[43] = false;
    }*/

    //if (Helicopter->ccp.KN53_pozuv_flag) {
    //    kn53_pozuv = true;    // 60  позывной ILS
    //}

    for (int i = 0; i < faults_count; i++)
    {
        if (faults[i] && *faults[i] && flags[i]) // проверка на nullptr && true/false
        {
            //if (*isTurnedOn)
            //{
                SendRitaDataPacket(track_numbers[i]);
                SendRitaDataPacket(track_numbers[i]);
            //}
            flags[i] = false;
        }
        else if(faults[i] && !(*faults[i]))
        {
            flags[i] = true;
        }
    }
}

void Helicopter_Rita_TCP_Client::Start()
{
    TimerTCPSend->start( );
}

void Helicopter_Rita_TCP_Client::Stop()
{
    TimerTCPSend->stop( );
}

void Helicopter_Rita_TCP_Client::On_Connected()
{
    qDebug() << "Rita Connected";
}

void Helicopter_Rita_TCP_Client::On_Disconnected()
{
    qDebug() << "Rita Disconnected";
}

void Helicopter_Rita_TCP_Client::On_Error()
{
    //qDebug() << "Rita" << socketErrorString;
}

void Helicopter_Rita_TCP_Client::SendRitaDataPacket(const int track_number)
{
    if (*isTurnedOn)
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
}
