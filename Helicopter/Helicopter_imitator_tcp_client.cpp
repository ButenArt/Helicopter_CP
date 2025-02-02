#pragma once
#pragma warning (push)
#pragma warning(disable: 4275)
#include "Helicopter_core.h"
#include "Helicopter_Model.h"
#include "Helicopter_imitator_tcp_client.h"
#pragma warning(pop)

Helicopter_OU_TCP_Client::Helicopter_OU_TCP_Client(Helicopter_core* Helicopter, int index)
{
    Helicopter = Helicopter;

    TxInterval = 20; // Helicopter_->_ini->LanTxInterval();
    hostaddr = Helicopter->_ini->IP_OU(index);
    tcp_port = Helicopter->_ini->PORT_OU(index);
    qDebug() << "Подключение OU" << index << " к " << hostaddr << ":" << tcp_port;
    TcpSocket->setSocketOption(QAbstractSocket::LowDelayOption, 1);  // выключаем режим nagle, пакет отправляется сразу, без ожидания дополнительных данных

    ConnectToHost(hostaddr, tcp_port);
    TimerTCPSend->setInterval(TxInterval);
    connect(TimerTCPSend, SIGNAL(timeout()), this, SLOT(slotSendTcpData()), Qt::QueuedConnection);
    connect(this, SIGNAL(signalConnected()), this, SLOT(On_Connected()), Qt::QueuedConnection);
    connect(this, SIGNAL(signalDisconnected()), this, SLOT(On_Disconnected()), Qt::QueuedConnection);
    connect(this, SIGNAL(signalError()), this, SLOT(On_Error()), Qt::QueuedConnection);
    connect(TcpSocket, SIGNAL(readyRead()), this, SLOT(slotReadTcpData()));
}

Helicopter_OU_TCP_Client::~Helicopter_OU_TCP_Client()
{
    TimerTCPSend->stop();
}

void Helicopter_OU_TCP_Client::Start()
{
    TimerTCPSend->start();
}

void Helicopter_OU_TCP_Client::Stop()
{
    TimerTCPSend->stop();
}

void Helicopter_OU_TCP_Client::slotReadTcpData()
{
    //ReadCommonTcpData();
}

void Helicopter_OU_TCP_Client::slotSendTcpData()
{
    SendControlsDataPacket();
}


void Helicopter_OU_TCP_Client::On_Connected()
{
    //    Helicopter_->controlsimitator_->ControlsImitatorActive = ClientConnected();
    Helicopter->log_->WriteLANLog(LogEvents::TCP_ControlsImitator_connected, "");
}

void Helicopter_OU_TCP_Client::On_Disconnected()
{
    //    Helicopter_->controlsimitator_->ControlsImitatorActive = ClientConnected();
    Helicopter->log_->WriteLANLog(LogEvents::TCP_ControlsImitator_disconnected, "");
}

void Helicopter_OU_TCP_Client::On_Error()
{
    //    Helicopter_->controlsimitator_->ControlsImitatorActive = ClientConnected();
    Helicopter->log_->WriteLANLog(LogEvents::TCP_ControlsImitator_Error, socketErrorString);
}

void Helicopter_OU_TCP_Client::SendControlsDataPacket()
{
    //    qDebug() << tcp_port;
    if (TcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        auto FP = Helicopter->Model_->GetFPar();
        //        auto IP = Helicopter_->Model_->GetIPar();

        QByteArray data;
        QDataStream data_stream(&data, QIODevice::WriteOnly);
        data_stream.setByteOrder(QDataStream::LittleEndian);
        data_stream.setFloatingPointPrecision(QDataStream::SinglePrecision);

        //  1 АЗС
        data_stream << static_cast<bool>(Helicopter->rp_azs.t_ta_start);         // Турбоагрегат запуск
        data_stream << static_cast<bool>(Helicopter->rp_azs.t_ta_ign);           // Турбоагрегат зажигание
        data_stream << static_cast<bool>(Helicopter->rp_azs.t_eng_start);        // Двигатели запуск
        data_stream << static_cast<bool>(Helicopter->rp_azs.t_eng_ign);          // Двигатели зажигание
        data_stream << static_cast<bool>(Helicopter->rp_azs.t_pump_consum);      // Топливная система - насосы топливн баков - расход
        data_stream << static_cast<bool>(Helicopter->rp_azs.t_pump_left);        // Топливная система - насосы топливн баков - левого
        data_stream << static_cast<bool>(Helicopter->rp_azs.t_pump_right);       // Топливная система - насосы топливн баков - правого
        data_stream << static_cast<bool>(Helicopter->rp_azs.t_fuel_gauge);       // Топливная система - топливомер
        data_stream << static_cast<bool>(Helicopter->rp_azs.t_spuu_52);          // СПУУ - 52                                                                             
        data_stream << static_cast<bool>(Helicopter->rp_azs.t_auto_main);        // Автопилот - основн
        data_stream << static_cast<bool>(Helicopter->rp_azs.t_auto_fric);        // Автопилот - фрикц
        data_stream << static_cast<bool>(Helicopter->rp_azs.t_auto_emufta);      // Автопилот - электромуфта
        data_stream << static_cast<bool>(Helicopter->rp_azs.t_hidro_main);       // Гидросист - основн
        data_stream << static_cast<bool>(Helicopter->rp_azs.t_hidro_sec);        // Гидросист - дублир
        data_stream << static_cast<bool>(Helicopter->rp_azs.t_sas);              // САС
        data_stream << static_cast<bool>(Helicopter->rp_azs.t_nav1);             // НАВ1
        data_stream << static_cast<bool>(Helicopter->rp_azs.t_nav1);             // НАВ2
        data_stream << static_cast<bool>(Helicopter->rp_azs.t_distmeter);        // Дальномер                                                                           
        data_stream << static_cast<bool>(Helicopter->rp_azs.t_n_ctrl);           // Управление оборотами
        data_stream << static_cast<bool>(Helicopter->rp_azs.t_close_left);       // Перекрывающие краны - левый
        data_stream << static_cast<bool>(Helicopter->rp_azs.t_close_rght);       // Перекрывающие краны - правый
        data_stream << static_cast<bool>(Helicopter->rp_azs.t_close_bypass);     // Кран перепус
        data_stream << static_cast<bool>(Helicopter->rp_azs.t_ppo_signal);       // Тумблер СИГНАЛИЗАЦ
        data_stream << static_cast<bool>(Helicopter->rp_azs.t_ppo_1_auto);       // Тумблер I ОЧЕРЕДЬ АВТОМ
        data_stream << static_cast<bool>(Helicopter->rp_azs.t_ppo_1_hand);       // Тумблер I ОЧЕРЕДЬ РУЧН
        //data_stream << static_cast<bool>(Helicopter->rp_azs.t_ppo_2_auto);       // Тумблер 2 ОЧЕРЕДЬ АВТОМ
        data_stream << static_cast<bool>(Helicopter->rp_azs.t_ppo_2_hand);       // Тумблер 2 ОЧЕРЕДЬ РУЧНАЯ                                                                            
        data_stream << static_cast<bool>(Helicopter->rp_azs.t_r_spu);            // Радио - СПУ
        data_stream << static_cast<bool>(Helicopter->rp_azs.t_r_altimetr);       // Радио - Высотомер 
        data_stream << static_cast<bool>(Helicopter->rp_azs.t_r_comand_osnv);    // Радио - Команд основ
        data_stream << static_cast<bool>(Helicopter->rp_azs.t_r_compas_sv);      // Радио - Компас СВ
        data_stream << static_cast<bool>(Helicopter->rp_azs.t_r_compas_ukv);     // Радио - Компас УКВ
        data_stream << static_cast<bool>(Helicopter->rp_azs.t_resp_uvd1);        // Радио - Ответчик УВД №1
        data_stream << static_cast<bool>(Helicopter->rp_azs.t_resp_uvd2);        // Радио - Ответчик УВД №2
        data_stream << static_cast<bool>(Helicopter->rp_azs.t_r_rls);            // Радио - РЛС
        data_stream << static_cast<bool>(Helicopter->rp_azs.t_far_left_ctrl);    // Фары - Левая - Управл
        data_stream << static_cast<bool>(Helicopter->rp_azs.t_far_left_flash);   // Фары - Левая - Свет
        data_stream << static_cast<bool>(Helicopter->rp_azs.t_far_right_ctrl);   // Фары - Правая - Управл
        data_stream << static_cast<bool>(Helicopter->rp_azs.t_far_right_flash);  // Фары - Правая - Свет
        data_stream << static_cast<bool>(Helicopter->rp_azs.t_siren_missle);     // Ракеты Сирена
        data_stream << static_cast<bool>(Helicopter->rp_azs.t_strov_light);      // Строев Огни
        data_stream << static_cast<bool>(Helicopter->rp_azs.t_ano);              // АНО
        data_stream << static_cast<bool>(Helicopter->rp_azs.t_tact_sbros);       // Такт сброс груза
        data_stream << static_cast<bool>(Helicopter->rp_azs.t_avar_sbros);       // Авар сброс груза                                                                             
        data_stream << static_cast<bool>(Helicopter->rp_azs.t_ais_ctrl);         // Противообледенительная система - Управление
        data_stream << static_cast<bool>(Helicopter->rp_azs.t_ais_pzu_left);     // Противообледенительная система - ПЗУ Левого Двиг
        data_stream << static_cast<bool>(Helicopter->rp_azs.t_ais_pzu_rght);     // Противообледенительная система - ПЗУ Правого Двиг
        data_stream << static_cast<bool>(Helicopter->rp_azs.t_ais_co_121);       // Противообледенительная система - СО - 121
        data_stream << static_cast<bool>(Helicopter->rp_azs.t_ais_glass);        // Противообледенительная система - Стекол
        data_stream << static_cast<bool>(Helicopter->rp_azs.t_ais_wipe_left);    // Стеклоочиститель - левый
        data_stream << static_cast<bool>(Helicopter->rp_azs.t_ais_wipe_rght);    // Стеклоочиститель - правый
        data_stream << static_cast<bool>(Helicopter->rp_azs.t_p_nv);             // П НВ
        data_stream << static_cast<bool>(Helicopter->rp_azs.t_ko_50);            // КО - 50

//  2 Левая боковая панель электропульта
//  2.1. Контроль
        data_stream << static_cast<bool>(Helicopter->lsp_elp.t_IV_500);      // Кнопка НВ-500Е
        data_stream << static_cast<bool>(Helicopter->lsp_elp.t_ground);      // Кнопка ЗЕМЛЯ
        data_stream << static_cast<bool>(Helicopter->lsp_elp.t_air);         // Кнопка ВОЗДУХ
        data_stream << static_cast<bool>(Helicopter->lsp_elp.t_rt_left);     // РТ двигателя левого
        data_stream << static_cast<bool>(Helicopter->lsp_elp.t_rt_rght);     // РТ двигателя правого
        data_stream << static_cast<bool>(!Helicopter->lsp_elp.t_bur);         // БУР
        data_stream << static_cast<bool>(Helicopter->lsp_elp.t_tbn);         // ТБН
//  2.2. Пульт управления ракетами ЭП-662
        data_stream << static_cast<bool>(Helicopter->lsp_elp.t_tumblr_1);    // Тумблер 1
        data_stream << static_cast<bool>(Helicopter->lsp_elp.t_button_1_1);  // Кнопка 1
        data_stream << static_cast<bool>(Helicopter->lsp_elp.t_button_1_2);  // Кнопка 2
        data_stream << static_cast<bool>(Helicopter->lsp_elp.t_button_1_3);  // Кнопка 3
        data_stream << static_cast<bool>(Helicopter->lsp_elp.t_button_1_4);  // Кнопка 4
        data_stream << static_cast<bool>(Helicopter->lsp_elp.t_tumblr_2);    // Тумблер 2
        data_stream << static_cast<bool>(Helicopter->lsp_elp.t_button_2_1);  // Кнопка 1
        data_stream << static_cast<bool>(Helicopter->lsp_elp.t_button_2_2);  // Кнопка 2
        data_stream << static_cast<bool>(Helicopter->lsp_elp.t_button_2_3);  // Кнопка 3
        data_stream << static_cast<bool>(Helicopter->lsp_elp.t_button_2_4);  // Кнопка 4
//  2.3. Пульт дистанционного управления аппаратурой РИ-65Б
        data_stream << static_cast<bool>(Helicopter->lsp_elp.t_ri_off);      // Кнопка ОТКЛ
        data_stream << static_cast<bool>(Helicopter->lsp_elp.t_ri_check);    // Кнопка ПРОВЕРКА
        data_stream << static_cast<bool>(Helicopter->lsp_elp.t_ri_rep);      // Кнопка ПОВТ
        /*не отображается при новых ОУ*/data_stream << static_cast<bool>(Helicopter->lsp_elp.t_ri_gain);     // Тумблер УСИЛ
        data_stream << static_cast<bool>(Helicopter->lsp_elp.i_hv_tral || Helicopter->rsp_ep.t_check_lamp);     // Лампа ХВ ТРАЛ ОТКРЫТ
        data_stream << static_cast<bool>(Helicopter->lsp_elp.i_ogr_nTr_Left || Helicopter->rsp_ep.t_check_lamp);// Лампа ОГРп ЛЕВЫЙ
        data_stream << static_cast<bool>(Helicopter->lsp_elp.i_ogr_nTr_Right || Helicopter->rsp_ep.t_check_lamp);// Лампа ОГРп ПРАВЫЙ
        data_stream << static_cast<float>(Helicopter->lsp_elp.f_p_air);      // Манометр МВУ-10К
        data_stream << static_cast<float>(Helicopter->lsp_elp.f_p_brake);    // Манометр МА-60К
        data_stream << static_cast<bool>(Helicopter->lsp_elp.t_ano);         // Кнопка КОД-АНО
        data_stream << static_cast<bool>(Helicopter->lsp_elp.t_siren);       // Кнопка СИРЕНА
        data_stream << static_cast<bool>(Helicopter->lsp_elp.i_siren_on || Helicopter->rsp_ep.t_check_lamp);//Индикатор СИРЕНА ВКЛЮЧЕНА
        data_stream << static_cast<bool>(Helicopter->lsp_elp.t_ext_lock);    // Тумблер ВНЕШНЯЯ ПОДВЕСКА
        data_stream << static_cast<bool>(Helicopter->lsp_elp.i_cargo_dump || Helicopter->rsp_ep.t_check_lamp);  // Индикатор ГРУЗ СБРОШЕН
        data_stream << static_cast<bool>(Helicopter->lsp_elp.t_sublight_1);  // Красный подсвет группа 1
        data_stream << static_cast<bool>(Helicopter->lsp_elp.t_sublight_2);  // Красный подсвет группа 2
        data_stream << static_cast<bool>(Helicopter->lsp_elp.t_fan);         // Вентилятор
        data_stream << static_cast<bool>(Helicopter->lsp_elp.t_cargo_dump);  // Тумблер аварийный сброс груза лебедка / внешняя подвеска
//  2.4. Пульт управления магнитофоном П503Б
        data_stream << static_cast<bool>(Helicopter->lsp_elp.t_recorder);    // Тумблер вкл / выкл
        data_stream << static_cast<bool>(Helicopter->lsp_elp.t_autostart);   // Тумблер автоспуск / непрерывная работа
        data_stream << static_cast<bool>(Helicopter->lsp_elp.t_spu_lar);       // Тумблер СПУ / ЛАР
        data_stream << static_cast<float>(Helicopter->lsp_elp.f_sublight);   // Подсвет
        data_stream << static_cast<bool>(Helicopter->lsp_elp.i_record || Helicopter->rsp_ep.t_check_lamp);      // Индикатор ЗАПИСЬ up
//  2.5. Остальные элементы
        data_stream << static_cast<bool>(Helicopter->lsp_elp.t_disasters);   // Тумблер бедств
        data_stream << static_cast<bool>(Helicopter->lsp_elp.t_stiran);      // Тумблер стиран
        data_stream << static_cast<bool>(Helicopter->lsp_elp.t_smoke);       // Тумблер не курить / не курить пристегнуть ремни
        data_stream << static_cast<bool>(Helicopter->lsp_elp.t_exit_tab);    // Тумблер табло выход
        data_stream << static_cast<bool>(Helicopter->lsp_elp.t_ri);          // Тумблер речевой регистратор
        data_stream << static_cast<bool>(Helicopter->lsp_elp.i_falut_rr || Helicopter->rsp_ep.t_check_lamp);    // Лампа ОТКАЗ РР
        data_stream << static_cast<bool>(Helicopter->lsp_elp.i_vkl_zapas || Helicopter->rsp_ep.t_check_lamp);   // Лампа ВКЛЮЧИ ЗАПАСНОЙ

//  3 СПУ-7 Лев Пилота
        data_stream << static_cast<float>(Helicopter->lpb.spu.f_prosl);//Вращалка ГРОМКОСТЬ ПРОСЛ	float
        data_stream << static_cast<float>(Helicopter->lpb.spu.f_obsh);//Вращалка ГРОМКОСТЬ ОБЩАЯ	float
        data_stream << static_cast<bool>(Helicopter->lpb.spu.t_set_1);//Тумблер СЕТЬ 1	bool
        data_stream << static_cast<bool>(Helicopter->lpb.spu.t_set_2);//Тумблер СЕТЬ 2	bool
        data_stream << static_cast<bool>(Helicopter->lpb.spu.t_spu);//Тумблер СПУ	bool
        data_stream << static_cast<bool>(Helicopter->lpb.spu.t_radio);//Тумблер РАДИО	bool
        data_stream << static_cast<quint8>(Helicopter->lpb.spu.gal());//Галетный Переключатель	uint8
//  4 СПУ-7 Прав Пилота
        data_stream << static_cast<float>(Helicopter->rpb.spu.f_prosl);// Вращалка ГРОМКОСТЬ ПРОСЛ	float
        data_stream << static_cast<float>(Helicopter->rpb.spu.f_obsh);//Вращалка ГРОМКОСТЬ ОБЩАЯ	float
        data_stream << static_cast<bool>(Helicopter->rpb.spu.t_set_1); // Тумблер СЕТЬ 1	bool
        data_stream << static_cast<bool>(Helicopter->rpb.spu.t_set_2);// Тумблер СЕТЬ 2	bool
        data_stream << static_cast<bool>(Helicopter->rpb.spu.t_spu); //Тумблер СПУ	bool
        data_stream << static_cast<bool>(Helicopter->rpb.spu.t_radio); // Тумблер РАДИО	bool
        data_stream << static_cast<quint8>(Helicopter->rpb.spu.gal());///Галетный Переключатель	uint8

//  5 Левая Панель АЗС
        data_stream << static_cast<bool>(Helicopter->lp_azs.i_weapon_chain || Helicopter->rsp_ep.t_check_lamp);// Светосигнальное табло ЦЕПИ ВООРУЖ ОТКЛ
        data_stream << static_cast<bool>(Helicopter->lp_azs.i_ab || Helicopter->rsp_ep.t_check_lamp);          // Светосигнальное табло АБ(желтая)
        data_stream << static_cast<bool>(Helicopter->lp_azs.i_explosion || Helicopter->rsp_ep.t_check_lamp);   // Светосигнальное табло ВЗРЫВ(красная)
        data_stream << static_cast<bool>(Helicopter->lp_azs.i_pus_bd1 || Helicopter->rsp_ep.t_check_lamp);     // Светосигнальное табло ПУС ВЗВЕДН БД1 БД6(желтая)
        data_stream << static_cast<bool>(Helicopter->lp_azs.i_nps || Helicopter->rsp_ep.t_check_lamp);         // Светосигнальное табло НРС
        data_stream << static_cast<bool>(Helicopter->lp_azs.i_pus_bd2 || Helicopter->rsp_ep.t_check_lamp);     // Светосигнальное табло ПУС ВЗВЕДН БД2 БД5(желтая)
        data_stream << static_cast<bool>(Helicopter->lp_azs.i_upk || Helicopter->rsp_ep.t_check_lamp);         // Светосигнальное табло УПК
        data_stream << static_cast<bool>(Helicopter->lp_azs.i_bd1_load || Helicopter->rsp_ep.t_check_lamp);    // Светосигнальное табло БД 1 ЗАГРУЖ(желтая)
        data_stream << static_cast<bool>(Helicopter->lp_azs.i_bd2_load || Helicopter->rsp_ep.t_check_lamp);    // Светосигнальное табло БД 2 ЗАГРУЖ(желтая)
        data_stream << static_cast<bool>(Helicopter->lp_azs.i_bd3_load || Helicopter->rsp_ep.t_check_lamp);    // Светосигнальное табло БД 3 ЗАГРУЖ(желтая)
        data_stream << static_cast<bool>(Helicopter->lp_azs.i_bd4_load || Helicopter->rsp_ep.t_check_lamp);    // Светосигнальное табло БД 4 ЗАГРУЖ(желтая)
        data_stream << static_cast<bool>(Helicopter->lp_azs.i_bd5_load || Helicopter->rsp_ep.t_check_lamp);    // Светосигнальное табло БД 5 ЗАГРУЖ(желтая)
        data_stream << static_cast<bool>(Helicopter->lp_azs.i_bd6_load || Helicopter->rsp_ep.t_check_lamp);    // Светосигнальное табло БД 6 ЗАГРУЖ(желтая)
        data_stream << static_cast<bool>(Helicopter->lp_azs.t_pus_cocking);              // КНОПКА ВЗВЕДЕНИЕ ПУС
        data_stream << static_cast<bool>(Helicopter->lp_azs.t_main_switch_off);           // Тумблер ГЛАВНЫЙ ВЫКЛЮЧАТЕЛЬ ВКЛ
        data_stream << static_cast<bool>(Helicopter->lp_azs.t_main_switch_on);          // Тумблер ГЛАВНЫЙ ВЫКЛЮЧАТЕЛЬ ВЫКЛ
        data_stream << static_cast<bool>(Helicopter->lp_azs.t_explosion);                // Тумблер ВЗРЫВ - НЕВЗРЫВ
        data_stream << static_cast<bool>(!Helicopter->lp_azs.t_emerg_dump);               // Тумблер АВАРИЙНЫЙ СБРОС БОМБ Б8 УПК
        data_stream << static_cast<bool>(Helicopter->lp_azs.t_sns_nav);                  // Тумблер СНС НАВ
        data_stream << static_cast<bool>(Helicopter->lp_azs.t_uvd_count);                // Тумблер СЧЕТЧИК УВД
        data_stream << static_cast<bool>(Helicopter->lp_azs.t_distmeter);                // Тумблер ДАЛЬНОМЕР
        data_stream << static_cast<bool>(Helicopter->lp_azs.t_sublight_pkv);             // Тумблер ПОДСВЕТ ПКВ
        data_stream << static_cast<bool>(Helicopter->lp_azs.t_power_onv_left);           // Тумблер ПИТАНИЕ ОНВ ЛЕВЫЙ
        data_stream << static_cast<bool>(Helicopter->lp_azs.t_power_onv_board);          // Тумблер ПИТАНИЕ ОНВ БОРТ - ТЕХНИК
        data_stream << static_cast<bool>(Helicopter->lp_azs.t_power_onv_right);          // Тумблер ПИТАНИЕ ОНВ ПРАВЫЙ
        data_stream << static_cast<float>(Helicopter->lp_azs.f_sublight_pkv);            // КРУТИЛКА ПОДСВЕТ ПКВ
        data_stream << static_cast<float>(Helicopter->lp_azs.f_ammo_count_upk_right_1);  // Счетчик снарядов для правого УПК №1
        data_stream << static_cast<float>(Helicopter->lp_azs.f_ammo_count_upk_right_2);  // Счетчик снарядов для правого УПК №2
        data_stream << static_cast<float>(Helicopter->lp_azs.f_ammo_count_upk_left_1);   // Счетчик снарядов для левого УПК №1
        data_stream << static_cast<float>(Helicopter->lp_azs.f_ammo_count_upk_left_2);   // Счетчик снарядов для левого УПК №2

//  6 ЛЕВЫЙ ЩИТОК ЭЛЕКТРОПУЛЬТА
        data_stream << static_cast<quint8>(Helicopter->lsh_elp.sw_glass_wiper);   // Переключатель СТЕКЛООЧИСТИТЕЛЬ
        data_stream << static_cast<bool>(Helicopter->lsh_elp.t_rv);               // Тумблер выключатель радиовысотомера
        data_stream << static_cast<bool>(Helicopter->lsh_elp.t_spuu_52);          // Тумблер Выключатель СПУУ-52
        data_stream << static_cast<bool>(Helicopter->lsh_elp.t_plafon);           // Тумблер ПЛАФОН
        data_stream << static_cast<bool>(Helicopter->lsh_elp.t_ag_left);          // Тумблер АГ ЛЕВ
        data_stream << static_cast<bool>(Helicopter->lsh_elp.t_ag_res);           // Тумблер АГ РЕЗЕРВ
        data_stream << static_cast<bool>(Helicopter->lsh_elp.t_bkk_18);           // Тумблер БКК-18
        data_stream << static_cast<bool>(Helicopter->lsh_elp.t_bkk_contr_down);     // Тумблер I КОНТР БКК
        data_stream << static_cast<bool>(Helicopter->lsh_elp.t_bkk_contr_up);   // Тумблер II КОНТР БКК
        data_stream << static_cast<bool>(Helicopter->lsh_elp.t_ri_65);            // Тумблер РИ-65
        data_stream << static_cast<bool>(Helicopter->lsh_elp.t_pvd_heat);         // Кнопка КОНТРОЛЬ ОБОГРЕВА ПВД
        data_stream << static_cast<bool>(Helicopter->lsh_elp.i_bkk_ok || Helicopter->rsp_ep.t_check_lamp);      // Индикатор ИСПРАВН. БКК
        data_stream << static_cast<bool>(Helicopter->lsh_elp.i_ri_65 || Helicopter->rsp_ep.t_check_lamp);       // Индикатор ВКЛЮЧИ РИ-65
        data_stream << static_cast<bool>(Helicopter->lsh_elp.i_pvd_heat_ok || Helicopter->rsp_ep.t_check_lamp); // Индикатор ОБОГРЕВ ИСПРАВЕН

//  7 ЛЕВАЯ ПАНЕЛЬ ЭЛЕКТРОПУЛЬТА
        data_stream << static_cast<bool>(!Helicopter->lp_elp.t_deicing_auto);         // Тумблер(ПРОТИВООБЛЕДЕНИТЕЛЬНАЯ СИСТЕМА) Автомат
        data_stream << static_cast<bool>(Helicopter->lp_elp.t_deicing_off);          // Кнопка(ПРОТИВООБЛЕДЕНИТЕЛЬНАЯ СИСТЕМА) ОТКЛ
        data_stream << static_cast<bool>(Helicopter->lp_elp.t_deicing_left_pzu);     // Тумблер(ПРОТИВООБЛЕДЕНИТЕЛЬНАЯ СИСТЕМА) ДВИГ ПЗУ ЛЕВ
        data_stream << static_cast<bool>(Helicopter->lp_elp.t_deicing_right_pzu);    // Тумблер(ПРОТИВООБЛЕДЕНИТЕЛЬНАЯ СИСТЕМА) ДВИГ ПЗУ ПРАВ
        data_stream << static_cast<bool>(!Helicopter->lp_elp.t_deicing_glass);        // Тумблер(ПРОТИВООБЛЕДЕНИТЕЛЬНАЯ СИСТЕМА) ОБОГРЕВ 
        data_stream << static_cast<quint8>(Helicopter->lp_elp.get_consumers());      // Галетный Переключатель ТОК ПОТРЕБИТЕЛЕЙ
        data_stream << static_cast<bool>(Helicopter->lp_elp.i_icing || Helicopter->rsp_ep.t_check_lamp);              // Индикатор ОБЛЕДЕН.
        data_stream << static_cast<bool>(Helicopter->lp_elp.i_pos_on || Helicopter->rsp_ep.t_check_lamp);             // Светосигнальное табло ПОС ВКЛЮЧЕНА
        data_stream << static_cast<bool>(Helicopter->lp_elp.i_fault_pos || Helicopter->rsp_ep.t_check_lamp);          // Светосигнальное табло ОТКАЗ ПОС НВ
        data_stream << static_cast<bool>(Helicopter->lp_elp.i_heat_left || Helicopter->rsp_ep.t_check_lamp);          // Светосигнальное табло ОБОГРЕВ ДВ.ЛЕВ
        data_stream << static_cast<bool>(Helicopter->lp_elp.i_heat_right || Helicopter->rsp_ep.t_check_lamp);         // Светосигнальное табло ОБОГРЕВ ДВ.ПРАВ.
        data_stream << static_cast<bool>(Helicopter->lp_elp.i_left_pzu_front || Helicopter->rsp_ep.t_check_lamp);     // Светосигнальное табло ЛЕВ.ПЗУ.ПЕРЕДН.
        data_stream << static_cast<bool>(Helicopter->lp_elp.i_right_pzu_front || Helicopter->rsp_ep.t_check_lamp);    // Светосигнальное табло ПРАВ.ПЗУ.ПЕРЕДН
        data_stream << static_cast<bool>(Helicopter->lp_elp.i_left_pzu_back || Helicopter->rsp_ep.t_check_lamp);      // Светосигнальное табло ЛЕВ.ПЗУ.ЗАДН.
        data_stream << static_cast<bool>(Helicopter->lp_elp.i_right_pzu_back || Helicopter->rsp_ep.t_check_lamp);     // Светосигнальное табло ПРАВ.ПЗУ.ЗАДН
        data_stream << static_cast<bool>(Helicopter->lp_elp.i_so_121_prop || Helicopter->rsp_ep.t_check_lamp);        // Светосигнальное табло СО - 121 ИСПРАВЕН
        data_stream << static_cast<bool>(Helicopter->lp_elp.i_flight || Helicopter->rsp_ep.t_check_lamp);             // Светосигнальное табло ПОЛЕТ
        data_stream << static_cast<bool>(Helicopter->lp_elp.i_parking || Helicopter->rsp_ep.t_check_lamp);            // Светосигнальное табло СТОЯНКА
        data_stream << static_cast<float>(Helicopter->lp_elp.f_ampermetr);                                       // Альтиметр - Стрелка
        data_stream << static_cast<bool>(Helicopter->lp_elp.i_section_1 || Helicopter->rsp_ep.t_check_lamp);          // Светосигнальное табло 1 СЕКЦИЯ
        data_stream << static_cast<bool>(Helicopter->lp_elp.i_section_2 || Helicopter->rsp_ep.t_check_lamp);          // Светосигнальное табло 2 СЕКЦИЯ
        data_stream << static_cast<bool>(Helicopter->lp_elp.i_section_3 || Helicopter->rsp_ep.t_check_lamp);          // Светосигнальное табло 3 СЕКЦИЯ
        data_stream << static_cast<bool>(Helicopter->lp_elp.i_section_4 || Helicopter->rsp_ep.t_check_lamp);          // Светосигнальное табло 4 СЕКЦИЯ
        data_stream << static_cast<float>(Helicopter->lp_elp.e_radio_baklan20_1);    // Вращалка Радиостанция Баклан 20 №1
        data_stream << static_cast<float>(Helicopter->lp_elp.e_radio_baklan20_2);    // Вращалка Радиостанция Баклан 20 №2
        data_stream << static_cast<bool>(Helicopter->lp_elp.t_radio_baklan20_1);     // Тубмлер Радиостанция Баклан 20 №1
        data_stream << static_cast<bool>(Helicopter->lp_elp.t_radio_baklan20_2);     // Тубмлер Радиостанция Баклан 20 №2
        data_stream << static_cast<float>(static_cast<float>(Helicopter->lp_elp.freq / 100) / 1000);          // Табло Баклан
        data_stream << static_cast<bool>(Helicopter->lp_elp.t_bark_eng_left_on);     // Тумблер БАРК ДВИГ ЛЕВАЯ Питание
        data_stream << static_cast<bool>(Helicopter->lp_elp.t_bark_eng_left_wep);    // Тумблер БАРК ДВИГ ЛЕВАЯ ЧР
        data_stream << static_cast<bool>(Helicopter->lp_elp.t_bark_eng_left_ct1);    // Тумблер БАРК ДВИГ ЛЕВАЯ СТ1
        data_stream << static_cast<bool>(Helicopter->lp_elp.t_bark_eng_left_ct2);    // Тумблер БАРК ДВИГ ЛЕВАЯ СТ2
        data_stream << static_cast<bool>(Helicopter->lp_elp.t_bark_eng_tg_left);     // Тумблер БАРК ДВИГ ТГ левый
        data_stream << static_cast<bool>(Helicopter->lp_elp.t_bark_eng_tg_right);    // Тумблер БАРК ДВИГ ТГ правый
        data_stream << static_cast<bool>(Helicopter->lp_elp.t_bark_eng_tk_left);     // Тумблер БАРК ДВИГ ТК левая
        data_stream << static_cast<bool>(Helicopter->lp_elp.t_bark_eng_tk_right);    // Тумблер БАРК ДВИГ ТК првая
        data_stream << static_cast<bool>(Helicopter->lp_elp.t_bark_eng_right_ct1);   // Тумблер БАРК ДВИГ ПРАВАЯ СТ1
        data_stream << static_cast<bool>(Helicopter->lp_elp.t_bark_eng_right_ct2);   // Тумблер БАРК ДВИГ ПРАВАЯ СТ2
        data_stream << static_cast<bool>(Helicopter->lp_elp.t_bark_eng_right_wep);   // Тумблер БАРК ДВИГ ПРАВАЯ ЧР
        data_stream << static_cast<bool>(Helicopter->lp_elp.t_bark_eng_right_on);    // Тумблер БАРК ДВИГ ПРАВАЯ ПИТАНИЕ
        data_stream << static_cast<bool>(0);          // Подсвет Лимба ПКВ
//  8 Средняя Панель Электропульта
        data_stream << static_cast<bool>(Helicopter->cep.i_eng_left_fire || Helicopter->rsp_ep.t_check_lamp);     // Индикатор ПОЖАР ЛЕВ ДВ	bool
        data_stream << static_cast<bool>(Helicopter->cep.i_eng_left_1_queue || Helicopter->rsp_ep.t_check_lamp);  // Индикатор ЛЕВ ДВ I ОЧЕРЕДЬ	bool
        data_stream << static_cast<bool>(Helicopter->cep.i_eng_left_2_queue || Helicopter->rsp_ep.t_check_lamp);  // Индикатор ЛЕВ ДВ II ОЧЕРЕДЬ	bool
        data_stream << static_cast<bool>(Helicopter->cep.t_eng_left_1_queue);                               // Кнопка I ОЧЕРЕДЬ ЛЕВ ДВ	bool
        data_stream << static_cast<bool>(Helicopter->cep.t_eng_left_2_queue);                               // Кнопка II ОЧЕРЕДЬ ЛЕВ ДВ	bool
        data_stream << static_cast<bool>(Helicopter->cep.i_eng_rght_fire || Helicopter->rsp_ep.t_check_lamp);     // Индикатор ПОЖАР ПРАВ ДВ	bool
        data_stream << static_cast<bool>(Helicopter->cep.i_eng_rght_1_queue || Helicopter->rsp_ep.t_check_lamp);  // Индикатор ПРАВ ДВ I ОЧЕРЕДЬ	bool
        data_stream << static_cast<bool>(Helicopter->cep.i_eng_rght_2_queue || Helicopter->rsp_ep.t_check_lamp);  // Индикатор ПРАВ ДВ II ОЧЕРЕДЬ	bool
        data_stream << static_cast<bool>(Helicopter->cep.t_eng_rght_1_queue);                               // Кнопка I ОЧЕРЕДЬ ПРАВ ДВ	bool
        data_stream << static_cast<bool>(Helicopter->cep.t_eng_rght_2_queue);                               // Кнопка II ОЧЕРЕДЬ ПРАВ ДВ	bool
        data_stream << static_cast<bool>(Helicopter->cep.i_ko_50_fire || Helicopter->rsp_ep.t_check_lamp);        // Индикатор ПОЖАР КО-50	bool
        data_stream << static_cast<bool>(Helicopter->cep.i_ko_50_1_queue || Helicopter->rsp_ep.t_check_lamp);     // Индикатор КО-50 I ОЧЕРЕДЬ	bool
        data_stream << static_cast<bool>(Helicopter->cep.i_ko_50_2_queue || Helicopter->rsp_ep.t_check_lamp);     // Индикатор КО-50 II ОЧЕРЕДЬ	bool
        data_stream << static_cast<bool>(Helicopter->cep.t_ko_50_1_queue);                                  // Кнопка I ОЧЕРЕДЬ КО-50	bool
        data_stream << static_cast<bool>(Helicopter->cep.t_ko_50_2_queue);                                  // Кнопка II ОЧЕРЕДЬ КО-50	bool
        data_stream << static_cast<bool>(Helicopter->cep.i_red_vsu_fire || Helicopter->rsp_ep.t_check_lamp);      // Индикатор ПОЖАР РЕД. ВСУ	bool
        data_stream << static_cast<bool>(Helicopter->cep.i_red_vsu_1_queue || Helicopter->rsp_ep.t_check_lamp);   // Индикатор РЕД ВСУ I ОЧЕРЕДЬ	bool
        data_stream << static_cast<bool>(Helicopter->cep.i_red_vsu_2_queue || Helicopter->rsp_ep.t_check_lamp);   // Индикатор РЕД ВСУ II ОЧЕРЕДЬ	bool
        data_stream << static_cast<bool>(Helicopter->cep.t_red_vsu_1_queue);                                // Кнопка I ОЧЕРЕДЬ РЕД ВСУ	bool
        data_stream << static_cast<bool>(Helicopter->cep.t_red_vsu_2_queue);                                // Кнопка II ОЧЕРЕДЬ РЕД ВСУ	bool
        data_stream << static_cast<bool>(Helicopter->cep.t_contr_sensors);                                  // Тумблер КОНТРОЛЬ ДАТЧИКОВ	bool
        data_stream << static_cast<bool>(Helicopter->cep.t_firefighting);                                   // Тумблер ОГНЕТУШЕНИЕ	bool
        data_stream << static_cast<bool>(Helicopter->cep.t_1);                                              // Тумблер КОНТРОЛЬ ПИРОПАТРОНОВ	bool
        data_stream << static_cast<bool>(Helicopter->cep.t_2);                                              // Тумблер II	bool
        data_stream << static_cast<quint8>(Helicopter->cep.sw_contr_sensors);                               // Переключатель КОНТРОЛЬ ДАТЧИКОВ	uint8	0-вык,1-контроль, 2-7:1-6 каналы
        data_stream << static_cast<bool>(Helicopter->cep.i_contr_sensors || Helicopter->rsp_ep.t_check_lamp);     // Индикатор КОНТРОЛЬ ДАТЧИКОВ	bool
        data_stream << static_cast<float>(Helicopter->cep.f_p_hidro_main);                                  // Левая шкала-стрелка	float	0,069444
        data_stream << static_cast<float>(Helicopter->cep.f_p_hidro_second);                                // Правая шкала-стрелка	float	0,069444
        data_stream << static_cast<bool>(Helicopter->cep.t_hidro_main);                                     // Тумблер ГИДРОСИСТЕМА ОСНОВН	bool	true=вкл
        data_stream << static_cast<bool>(Helicopter->cep.i_hidro_main || Helicopter->rsp_ep.t_check_lamp);        // Индикатор ОСНОВН ВКЛЮЧЕНА	bool
        data_stream << static_cast<bool>(Helicopter->cep.t_hidro_second);                                   // Тумблер ДУБЛИР
        data_stream << static_cast<bool>(Helicopter->cep.i_hidro_second || Helicopter->rsp_ep.t_check_lamp);      // Индикатор ДУБЛИР ВКЛЮЧЕНА	bool
        data_stream << static_cast<bool>(Helicopter->cep.t_off_second_hidro);                               // Кнопка ОТКЛ ДУБЛИР
        data_stream << static_cast<bool>(Helicopter->cep.t_ta_sel_false_start);                             // Тумблер ПРОКРУТ bool
        data_stream << static_cast<bool>(Helicopter->cep.t_ta_sel_start);                                   // Тумблер ЗАПУСК	bool
        data_stream << static_cast<bool>(Helicopter->cep.t_ta_start);                                       // Кнопка ЗАПУСК ТУРБОАГРЕГАТА	bool
        data_stream << static_cast<bool>(Helicopter->cep.t_ta_stop);                                        // Кнопка ВЫКЛ АИ-9В	bool
        data_stream << static_cast<bool>(Helicopter->cep.i_ta_automat_on || Helicopter->rsp_ep.t_check_lamp);     // Индикатор АВТОМАТ ВКЛЮЧЕН	bool
        data_stream << static_cast<bool>(Helicopter->cep.i_p_oil_warn || Helicopter->rsp_ep.t_check_lamp);        // Индикатор ДАВ МАСЛА НОРМ
        data_stream << static_cast<bool>(Helicopter->cep.i_TF || Helicopter->rsp_ep.t_check_lamp);                // Индикатор Фильтр топлива	    bool
        data_stream << static_cast<bool>(Helicopter->cep.i_ta_brok || Helicopter->rsp_ep.t_check_lamp);           // Индикатор ВСУ Отказ	    bool
        data_stream << static_cast<bool>(Helicopter->cep.t_eng_start);                                      // Кнопка ЗАПУСК ДВИГАТЕЛЕЙ
        data_stream << static_cast<bool>(Helicopter->cep.i_eng_automat_on || Helicopter->rsp_ep.t_check_lamp);    // Индикатор АВТОМАТ ВКЛЮЧЕН	bool
        data_stream << static_cast<bool>(Helicopter->cep.t_eng_stop);                                       // Кнопка ПРЕКРАЩ ЗАПУСКА ДВИГАТЕЛЕЙ	bool
        data_stream << static_cast<bool>(Helicopter->cep.t_eng_left);                                       // Тумблер ЗАПУСК ЛЕВ	bool
        data_stream << static_cast<bool>(Helicopter->cep.t_eng_rght);                                       // Тумблер ЗАПУСК ПРАВ	bool
        data_stream << static_cast<bool>(Helicopter->cep.t_eng_sel_scroll);                                  // Тумблер запуск	bool	true=вкл
        data_stream << static_cast<bool>(Helicopter->cep.t_eng_sel_start);                                 // Тумблер ПРОКРУТ	bool	true=вкл
        data_stream << static_cast<bool>(Helicopter->cep.i_ta_work || Helicopter->rsp_ep.t_check_lamp);           // Индикатор СТАРТЕР РАБОТАЕТ
        data_stream << static_cast<bool>(Helicopter->cep.t_eng_left_ign_check);                             // Тумблер ПРОВЕРКА ЗАЖИГ ЛЕВ	bool
        data_stream << static_cast<bool>(Helicopter->cep.t_eng_right_ign_check);                            // Тумблер ПРОВЕРКА ЗАЖИГ ПРАВ	bool
        data_stream << static_cast<float>(Helicopter->cep.f_t_ta);                                          // Шкала температуры	float	0,625
        data_stream << static_cast<float>(Helicopter->cep.f_p_ta);                                          // Шкала давления	float	0:03
        data_stream << static_cast<bool>(Helicopter->cep.t_fuel_left_close);                                // Тумблер АИ-9В ПЕРЕКР ЛЕВ КРАН	bool	true=откр
        data_stream << static_cast<bool>(Helicopter->cep.t_fuel_right_close);                               // Тумблер АИ-9В ПЕРЕКР ПРАВ КРАН	bool	true=откр
        data_stream << static_cast<bool>(Helicopter->cep.t_fuel_bypass_close);                              // Тумблер АИ-9В ПЕРЕКР КОЛЬЦЕВ БАКОВ	bool	true=откр
        data_stream << static_cast<bool>(Helicopter->cep.t_fuel_perepusk);                                  // Тумблер ПЕРЕПУСК	bool
        data_stream << static_cast<bool>(Helicopter->cep.i_fuel_left_close || Helicopter->rsp_ep.t_check_lamp);   // Индикатор ЛЕВЫЙ ЗАКРЫТ
        data_stream << static_cast<bool>(Helicopter->cep.i_fuel_right_close || Helicopter->rsp_ep.t_check_lamp);  // Индикатор ПРАВЫЙ ЗАКРЫТ
        data_stream << static_cast<bool>(Helicopter->cep.i_fuel_bypass_close || Helicopter->rsp_ep.t_check_lamp); // Индикатор КОЛЬЦЕВ. ОТКЛ
        data_stream << static_cast<bool>(!Helicopter->cep.t_fpump_refil);                                    // Тумблер АИ-9В ЗАПРАВ КОНТР	bool	true=вверх
        data_stream << static_cast<bool>(Helicopter->cep.t_fpump_consum);                                   // Тумблер АИ-9В ЗАПРАВ РАСХОД	bool	true=вверх
        data_stream << static_cast<bool>(Helicopter->cep.t_fpump_left);                                     // Тумблер АИ-9В ЗАПРАВ ЛЕВЫЙ	bool	true=вверх
        data_stream << static_cast<bool>(Helicopter->cep.t_fpump_right);                                    // Тумблер АИ-9В ЗАПРАВ ПРАВЫЙ	bool	true=вверх
        data_stream << static_cast<bool>(Helicopter->cep.i_fpump_consum_off || Helicopter->rsp_ep.t_check_lamp);  // Индикатор РАСХОД НЕ РАБОТ	bool
        data_stream << static_cast<bool>(Helicopter->cep.i_fpump_left_off || Helicopter->rsp_ep.t_check_lamp);    // Индикатор ЛЕВЫЙ НЕ РАБОТ	    bool
        data_stream << static_cast<bool>(Helicopter->cep.i_fpump_right_off || Helicopter->rsp_ep.t_check_lamp);   // Индикатор ПРАВЫЙ НЕ РАБОТ	bool

//  9 Правая Панель Электропульта
        data_stream << static_cast<float>(Helicopter->rsp_elp.ark_9.f_rk_vol);         // Вращалка Громкость
        data_stream << static_cast<bool>(Helicopter->rsp_elp.ark_9.t_rk_tlf);          // Тумблер ТЛФ      
        data_stream << static_cast<bool>(Helicopter->rsp_elp.ark_9.t_rk_tlg);          // Тумблер ТЛГ	    
        data_stream << static_cast<bool>(Helicopter->rsp_elp.ark_9.t_rk_ramka);        // Кнопка РАМКА	
        data_stream << static_cast<bool>(Helicopter->rsp_elp.ark_9.t_rk_ctrl);         // Кнопка УПР	
        data_stream << static_cast<bool>(Helicopter->rsp_elp.ark_9.t_rk_chanel);       // Тумблер КАНАЛ
        data_stream << static_cast<quint8>(Helicopter->rsp_elp.ark_9.sw_rk_mode());    // Галетный Переключатель
        data_stream << static_cast<float>(Helicopter->rsp_elp.ark_9.f_rk_left_top);    // ЛЕВ КГЦ-ВРАШАЛКА (ВЕРХНЯЯ)
        data_stream << static_cast<float>(Helicopter->rsp_elp.ark_9.f_rk_left_mid);    // ЛЕВ КГЦ-ВРАШАЛКА (СРЕДНЯЯ)
        data_stream << static_cast<float>(Helicopter->rsp_elp.ark_9.f_rk_left_bot);    // ЛЕВ КГЦ-ВРАШАЛКА (НИЖНЯЯ)
        data_stream << static_cast<float>(Helicopter->rsp_elp.ark_9.f_rk_right_top);   // ПРАВ КГЦ-ВРАШАЛКА (ВЕРХНЯЯ)
        data_stream << static_cast<float>(Helicopter->rsp_elp.ark_9.f_rk_right_mid);   // ПРАВ КГЦ-ВРАШАЛКА (СРЕДНЯЯ)
        data_stream << static_cast<float>(Helicopter->rsp_elp.ark_9.f_rk_right_bot);   // ПРАВ КГЦ-ВРАШАЛКА (НИЖНЯЯ)
        data_stream << static_cast<float>(Helicopter->rsp_elp.f_t_ko50);             // Крутилка ЗАДАТЧИК Т КО-50
        data_stream << static_cast<bool>(Helicopter->rsp_elp.ark_ud.t_rkud_sens);       // Тумблер ЧУВСТВ
        data_stream << static_cast<bool>(Helicopter->rsp_elp.ark_ud.t_rkud_ukv);        // Тумблер УКВ-ДЦБ	bool	true-ДЦБ
        data_stream << static_cast<float>(Helicopter->rsp_elp.ark_ud.f_rkud_vol);       // Вращалка ГРОМКОСТЬ	float	0:01
        data_stream << static_cast<bool>(Helicopter->rsp_elp.ark_ud.i_rkud_1 || Helicopter->rsp_ep.t_check_lamp);   // Лампа УП	bool
        data_stream << static_cast<bool>(Helicopter->rsp_elp.ark_ud.i_rkud_2 || Helicopter->rsp_ep.t_check_lamp);   // Лампа ШП	bool
        data_stream << static_cast<bool>(Helicopter->rsp_elp.ark_ud.i_rkud_3 || Helicopter->rsp_ep.t_check_lamp);   // Лампа И	    bool
        data_stream << static_cast<quint8>(Helicopter->rsp_elp.ark_ud.sw_rkud_mode());    // Галетный Переключатель	АРК-УД uint8	0-ВЫК,1-УП,2-ШП,3-И,4-РПК
        data_stream << static_cast<quint8>(Helicopter->rsp_elp.ark_ud.sw_rkud_chanel());  // Галетный переключатель КАНАЛЫ	uint8
        data_stream << static_cast<bool>(Helicopter->rsp_elp.ark_ud.t_contr);            // Кнопка КОНТР	bool
        data_stream << static_cast<bool>(Helicopter->rsp_elp.ark_ud.t_ant_l);           // Кнопка АНТ Л	bool
        data_stream << static_cast<bool>(Helicopter->rsp_elp.ark_ud.t_ant_r);           // Кнопка АНТ П	bool
        data_stream << static_cast<bool>(Helicopter->rsp_elp.i_ko50_heater || Helicopter->rsp_ep.t_check_lamp);  // Индикатор ПОДОГРЕВАТЕЛЬ	bool
        data_stream << static_cast<bool>(Helicopter->rsp_elp.i_ko50_ignition || Helicopter->rsp_ep.t_check_lamp);// Индикатор ЗАЖИГАНИЕ	bool
        data_stream << static_cast<bool>(Helicopter->rsp_elp.i_ko50_work || Helicopter->rsp_ep.t_check_lamp);    // Индикатор КО-50 РАБОТАЕТ	bool
        data_stream << static_cast<bool>(Helicopter->rsp_elp.akb_hight_temp_1 || Helicopter->rsp_ep.t_check_lamp);// Индикатор АКБ 1 темп выс
        data_stream << static_cast<bool>(Helicopter->rsp_elp.akb_hight_temp_2 || Helicopter->rsp_ep.t_check_lamp);    // Индикатор АКБ 2 темп выс
        data_stream << static_cast<bool>(Helicopter->rsp_elp.t_ko50_ta_start);   // Кнопка ЗАПУСК ТУРБОАГРЕГАТА	bool
        data_stream << static_cast<bool>(Helicopter->rsp_elp.t_ko50_man);        // Тумблер КО50-РУЧН	bool	true-РУЧН
        data_stream << static_cast<bool>(Helicopter->rsp_elp.t_ko50_auto);       // Тумблер КО50-АВТ	bool	true-РУЧН
        data_stream << static_cast<bool>(Helicopter->rsp_elp.t_ko50_fill);       // Тумблер КО50-ЗАЛИВКА	bool
        data_stream << static_cast<bool>(Helicopter->rsp_elp.t_ko50_midl);       // Тумблер КО50-СРЕДН РЕЖИМ	bool
        data_stream << static_cast<bool>(Helicopter->rsp_elp.t_ko50_vent);       // Тумблер Вентил	bool
        data_stream << static_cast<bool>(!Helicopter->rsp_elp.t_N);               // Тумблер СЕВ ЮГ	bool	true-ЮГ
        data_stream << static_cast<bool>(Helicopter->rsp_elp.gmk.t_contr0);    // Тумблер КОНТР	bool	true-300
        data_stream << static_cast<bool>(Helicopter->rsp_elp.gmk.t_contr300);    // Тумблер КОНТР	bool	true-300
        data_stream << static_cast<float>(Helicopter->rsp_elp.f_latitude);       // Вращалка ШИРОТА	float	0:01
        data_stream << static_cast<bool>(Helicopter->rsp_elp.t_mk);              // Тумблер МК ГК	bool	true-ГК
        data_stream << static_cast<bool>(Helicopter->rsp_elp.t_zk_l);            // Тумблер ЗК ЛЕВ	bool
        data_stream << static_cast<bool>(Helicopter->rsp_elp.t_zk_r);            // Тумблер ЗК ПРАВ	bool
        data_stream << static_cast<bool>(Helicopter->rsp_elp.t_dubl_osnv);       // Тумблер ДУБЛИР ОСНОВ

//  10 Правый Щиток Электропульта
        data_stream << static_cast<bool>(Helicopter->rsh_elp.t_pvd_heat_contr);                    // КОНТРОЛЬ ОБОГРЕВА ПВД
        data_stream << static_cast<bool>(Helicopter->rsh_elp.i_pvd_heat_ok || Helicopter->rsp_ep.t_check_lamp);   // Индикатор ОБОГРЕВ ИСПРАВЕН
        data_stream << static_cast<bool>(Helicopter->rsh_elp.t_diss);                              // Тумблер ДИСС
        data_stream << static_cast<bool>(Helicopter->rsh_elp.t_rs);                                // Тумблер СВЯЗНАЯ РС
        data_stream << static_cast<bool>(Helicopter->rsh_elp.t_gmk);                               // Тумблер ГМК-1
        data_stream << static_cast<bool>(Helicopter->rsh_elp.t_sgu35);                             // Тумблер ВК-53 (СГУ35)
        data_stream << static_cast<bool>(Helicopter->rsh_elp.t_ag_right);                          // Тумблер ПРАВЫЙ АГ
        data_stream << static_cast<bool>(Helicopter->rsh_elp.t_plafon);                            // Тумблер ПЛАФОН
        data_stream << static_cast<quint8>(Helicopter->rsh_elp.sw_wiper_mode);                     // Переключатель СТЕКЛООЧИСТИТЕЛЬ
        data_stream << static_cast<bool>(Helicopter->rsh_elp.t_rkud);                              // Тумблер АРК-УД

//  11 Правая Боковая Панель Электропульта
        data_stream << static_cast<bool>(Helicopter->rsp_ep.t_check_lamp);                         // Тумблер ПРОВЕРКА ЛАМП
        data_stream << static_cast<bool>(Helicopter->rsp_ep.t_main_second);                        // Тумблер ОСНОВ-РЕЗЕРВ
        data_stream << static_cast<bool>(Helicopter->rsp_ep.t_main_reserv);
        data_stream << static_cast<float>(Helicopter->rsp_ep.f_board);                             // Вращалка ТАБЛО
        data_stream << static_cast<bool>(Helicopter->rsp_ep.t_day);                                // Тумблер ДЕНЬ
        data_stream << static_cast<bool>(Helicopter->rsp_ep.t_night);                              // Тумблер НОЧЬ
        data_stream << static_cast<float>(Helicopter->rsp_ep.f_gain);                              // Вращалка ГРОМКОСТЬ СКАН
        data_stream << static_cast<quint8>(Helicopter->rsp_ep.sw_scan);                            // Вращалка НАБОР СКАН
        data_stream << static_cast<bool>(Helicopter->rsp_ep.t_light_emeg);                         // Тумблер ДЕЖУРНОЕ ОСВЕЩЕНИЕ
        data_stream << static_cast<bool>(Helicopter->rsp_ep.t_light_generl);                       // Тумблер ОБЩЕЕ ОСВЕЩЕНИЕ
        data_stream << static_cast<bool>(Helicopter->rsp_ep.t_ano);                                // Тумблер АНО
        data_stream << static_cast<bool>(Helicopter->rsp_ep.t_light_drill);                        // Тумблер ОГНИ СТРОЕВ
        data_stream << static_cast<bool>(Helicopter->rsp_ep.t_light_contur);                       // Тумблер ОГНИ КОНТУР
        data_stream << static_cast<bool>(Helicopter->rsp_ep.t_flashlight );                        // Тумблер МАЯК НИЗ
        data_stream << static_cast<bool>(Helicopter->rsp_ep.t_beacon_down);                        // Тумблер МАЯК НИЗ
        data_stream << static_cast<bool>(Helicopter->rsp_ep.t_pzu_left);                           // Тумблер ПЗУ ДВИГАТ ЛЕВ
        data_stream << static_cast<bool>(Helicopter->rsp_ep.t_pzu_right);                          // Тумблер ПЗУ ДВИГАТ ПРАВ
        data_stream << static_cast<bool>(Helicopter->rsp_ep.t_pvd_heat_left);                      // Тумблер ОБОГРЕВ ПВД ЛЕВ
        data_stream << static_cast<bool>(Helicopter->rsp_ep.t_pvd_heat_right);                     // Тумблер ОБОГРЕВ ПВД ПРАВ
        data_stream << static_cast<bool>(Helicopter->rsp_ep.t_clock_heat);                         // Тумблер ОБОГРЕВ ЧАСОВ
        data_stream << static_cast<bool>(Helicopter->rsp_ep.t_sublight_1);                         // Тумблер МИГАЛКА
        data_stream << static_cast<bool>(Helicopter->rsp_ep.t_sublight_2);                         // Тумблер МИГАЛКА
        data_stream << static_cast<float>(Helicopter->rsp_ep.f_day_night);                         // Ночь - День
        data_stream << static_cast<bool>(Helicopter->rsp_ep.t_fan);                                // Тумблер МИГАЛКА
        data_stream << static_cast<bool>(Helicopter->rsp_ep.i_pzu_right_on || Helicopter->rsp_ep.t_check_lamp);// Индикатор ПРАВ.ПЗУ ВКЛЮЧЕН
        data_stream << static_cast<bool>(Helicopter->rsp_ep.i_pzu_left_on || Helicopter->rsp_ep.t_check_lamp);// Индикатор ЛЕВ.ПЗУ ВКЛЮЧЕН
//  12 Электрощиток электропульта
        data_stream << static_cast<float>(Helicopter->ep_elp.f_amper_1);             // Шкала ПОСТОЯННЫЙ ТОК ЛЕВ
        data_stream << static_cast<float>(Helicopter->ep_elp.f_amper_2);             // Шкала ПОСТОЯННЫЙ ТОК ЦЕНТР
        data_stream << static_cast<float>(Helicopter->ep_elp.f_amper_3);             // Шкала ПОСТОЯННЫЙ ТОК ПРАВ
        data_stream << static_cast<bool>(Helicopter->ep_elp.t_akkum_1);              // Тумблер АККУМУЛЯТОРЫ 1
        data_stream << static_cast<bool>(Helicopter->ep_elp.t_akkum_2);              // Тумблер АККУМУЛЯТОРЫ 2
        data_stream << static_cast<quint8>(Helicopter->ep_elp.get_ctrl());           // Переключатель КОНТРОЛЬ ТОКА
        data_stream << static_cast<quint8>(Helicopter->ep_elp.get_dc_volt_mode());   // Переключатель ЦЕНТР
        data_stream << static_cast<quint8>(Helicopter->ep_elp.f_volt_reg);           // Крутилка РЕГУЛИРОВКА НАПРЯЖЕНИЯ
        data_stream << static_cast<bool>(Helicopter->ep_elp.t_ACDC_1);               // Тумблер ВЫПРЯМИТЕЛИ 1
        data_stream << static_cast<bool>(Helicopter->ep_elp.t_ACDC_2);               // Тумблер ВЫПРЯМИТЕЛИ 2
        data_stream << static_cast<bool>(Helicopter->ep_elp.t_aero_pit_dc);          // Тумблер АЭР ПИТАН (РЕЗЕРВ ГЕНЕР)
        data_stream << static_cast<bool>(Helicopter->ep_elp.t_test_sys);             // Тумблер ПРОВЕРКА ОБОРУД
        data_stream << static_cast<float>(Helicopter->ep_elp.f_volt);                // Вольтметр
        data_stream << static_cast<bool>(Helicopter->ep_elp.i_vu1_brok || Helicopter->rsp_ep.t_check_lamp); // Индикатор ВУ1 НЕ РАБОТ
        data_stream << static_cast<bool>(Helicopter->ep_elp.i_vu2_brok || Helicopter->rsp_ep.t_check_lamp); // Индикатор ВУ2 НЕ РАБОТ
        data_stream << static_cast<bool>(Helicopter->ep_elp.i_aero_pit_dc || Helicopter->rsp_ep.t_check_lamp);  // Индикатор АЭР ПИТ 
        data_stream << static_cast<float>(Helicopter->ep_elp.f_ac_amper_1);          // Переменный ток-амперметр лев
        data_stream << static_cast<float>(Helicopter->ep_elp.f_ac_amper_2);          // Переменный ток-амперметр прав
        data_stream << static_cast<float>(Helicopter->ep_elp.f_ac_amper_3);          // Переменный ток-амперметр центральный
        data_stream << static_cast<float>(Helicopter->ep_elp.f_ac_volt );            // Переменный ток ввольтметр
        data_stream << static_cast<bool>(Helicopter->ep_elp.i_gen_1_off || Helicopter->rsp_ep.t_check_lamp); // Индикатор ГЕН. 1 ОТКЛ
        data_stream << static_cast<bool>(Helicopter->ep_elp.i_gen_2_off || Helicopter->rsp_ep.t_check_lamp); // Индикатор ГЕН. 2 ОТКЛ
        data_stream << static_cast<bool>(Helicopter->ep_elp.t_res_gen || Helicopter->rsp_ep.t_check_lamp); // Индикатор ГЕН ВСУ РАБОТ
        data_stream << static_cast<bool>(Helicopter->ep_elp.t_gen_1);                // Тумблер ГЕНЕРАТОРЫ 1
        data_stream << static_cast<bool>(Helicopter->ep_elp.t_gen_1_contr);          // Тумблер ГЕНЕРАТОРЫ 1
        data_stream << static_cast<bool>(Helicopter->ep_elp.t_gen_2);                // Тумблер ГЕНЕРАТОРЫ 2
        data_stream << static_cast<bool>(Helicopter->ep_elp.t_gen_2_contr);          // Тумблер ГЕНЕРАТОРЫ 2
        data_stream << static_cast<bool>(Helicopter->ep_elp.t_res_gen );             // Тумблер ВСУ
        data_stream << static_cast<bool>(Helicopter->ep_elp.t_r36_auto);             // Тумблер ТР-Р36В АВТОМАТ
        data_stream << static_cast<bool>(Helicopter->ep_elp.t_r36_man);              // Тумблер ТР-Р36В РУЧНОЕ
        data_stream << static_cast<bool>(Helicopter->ep_elp.t_pts_auto);             // Тумблер ПТС АВТОМАТ
        data_stream << static_cast<bool>(Helicopter->ep_elp.t_pts_man);              // Тумблер ПТС РУЧНОЕ
        data_stream << static_cast<bool>(Helicopter->ep_elp.t_aero_pit_ac);             // Тумблер АЭР ПИТАН
        data_stream << static_cast<bool>(Helicopter->ep_elp.t_gen_pts_contr);                  // Тумблер ПТС АЭР ПИТАН вверх
        data_stream << static_cast<bool>(Helicopter->ep_elp.t_gen);                  // Тумблер ПТС АЭР ПИТАН низ
        data_stream << static_cast<quint8>(Helicopter->ep_elp.get_ac_volt_mode());    // Переключатель КОНТРОЛЬ НАПРЯЖЕНИЯ
        data_stream << static_cast<bool>(Helicopter->ep_elp.i_reser36 || Helicopter->rsp_ep.t_check_lamp);    // Индикатор -38В РЕЗЕРВ
        data_stream << static_cast<bool>(Helicopter->ep_elp.i_ptc_on || Helicopter->rsp_ep.t_check_lamp);      // Индикатор ПТ ВКЛЮЧЕН
        data_stream << static_cast<bool>(Helicopter->ep_elp.i_resev_on || Helicopter->rsp_ep.t_check_lamp);   // Индикатор РЕЗЕРВ ЛИНИЯ ВКЛ
        data_stream << static_cast<bool>(Helicopter->ep_elp.i_rap_on || Helicopter->rsp_ep.t_check_lamp);    // Индикатор АЭР ПИТ ВКЛЮЧ

//  13 Левая Приборная Панель
        data_stream << static_cast<bool>(Helicopter->lpb.t_roll_33);                 // Тумблер Предел КРЕНА
        data_stream << static_cast<bool>(Helicopter->lpb.t_svet_ubran);                   // Тумблер СВЕТ Убрана
        data_stream << static_cast<bool>(Helicopter->lpb.t_svet);                   // Тумблер СВЕТ Свет
        data_stream << static_cast<bool>(Helicopter->lpb.t_fpp);                     // Тумблер УПРАВЛ ФПП
        data_stream << static_cast<bool>(Helicopter->lpb.t_project);                 // Тумблер УПРАВЛ ПРОЖЕКТ
        data_stream << static_cast<quint8>(Helicopter->lpb.get_sw_pvd());                  // Переключатель СТАТИЧЕСКАЯ СИСТЕМА ПВД	uint8	0-левая, 1-объединенная, 2-правая

        data_stream << static_cast<float>(Helicopter->lpb.rv_5.alt());                 // ВЫСОТОМЕР-Стрелка	float	0,486111
        data_stream << static_cast<bool>(Helicopter->lpb.rv_5.t_test);               // ВЫСОТОМЕР-Кнопка ТЕСТ
        data_stream << static_cast<float>(Helicopter->lpb.rv_5.encoder);             // ВЫСОТОМЕР-Крутилка индекса
        data_stream << static_cast<float>(Helicopter->lpb.rv_5.index);               // Высотомер-индекс
        data_stream << static_cast<bool>(Helicopter->lpb.rv_5.blank);                // Высотомер-бленкер

        data_stream << static_cast<bool>(Helicopter->lpb.t_apk_sv);                  // Тумблер АРК	bool	АРК СВ
        data_stream << static_cast<bool>(Helicopter->lpb.t_apk_ukv);                 // Тумблер АРК	bool	АРК УКВ
        data_stream << static_cast<float>(Helicopter->lpb.ut_6k.temp1);              // Термометр-стрелка-ЛЕВ ВЕРХ	float	0,833333
        data_stream << static_cast<float>(Helicopter->lpb.ut_6k.temp2);              // Термометр-стрелка-ПРАВ ВЕРХ	float	0,833333
        data_stream << static_cast<float>(Helicopter->lpb.ut_6k.get_temp1_fine());   // Термометр-стрелка-ЛЕВ НИЗ	float	0,069444
        data_stream << static_cast<float>(Helicopter->lpb.ut_6k.get_temp2_fine());   // Термометр-стрелка-ПРАВ НИЗ	float	0,069444
        data_stream << static_cast<float>(Helicopter->lpb.uc_450k.speed());            // Спидометр-стрелка	float	0,3125
        data_stream << static_cast<float>(Helicopter->lpb.vd_10vk.get_km());         // Высотометр (КМ)-стрелка	float	0:10
        data_stream << static_cast<float>(Helicopter->lpb.vd_10vk.get_m());          // Высотометр (М)-стрелка	float	0:10
        data_stream << static_cast<float>(Helicopter->lpb.vd_10vk.p);                // Высотометр (М)-давление	float	0:10
        data_stream << static_cast<float>(Helicopter->lpb.vd_10vk.f_barometr);       // Высотометр (М)-крутилуа	float	0:10
        data_stream << static_cast<float>(Helicopter->lpb.up_21_15.fosh);            // Шаг винта стрелка	float	1:15
        data_stream << static_cast<float>(Helicopter->lpb.ir_117.index);             // ИЗМЕРИТЕЛЬ РЕЖИМОВ центр
        data_stream << static_cast<float>(Helicopter->lpb.ir_117.eng1);              // ИЗМЕРИТЕЛЬ РЕЖИМОВ-Левый указатель
        data_stream << static_cast<float>(Helicopter->lpb.ir_117.eng2);              // ИЗМЕРИТЕЛЬ РЕЖИМОВ-Правый указатель
        data_stream << static_cast<float>(Helicopter->lpb.avg_left.roll);            // АВИАГОРИЗОНТ ЛЕВ-крен	float	0  крен-roll
        data_stream << static_cast<float>(Helicopter->lpb.avg_left.getPitch( ) );           // АВИАГОРИЗОНТ ЛЕВ-тангаж	float	0 тангаж
        data_stream << static_cast<float>(Helicopter->lpb.avg_left.yawl);            // АВИАГОРИЗОНТ ЛЕВ-раысканье	float	0  крен-roll
        data_stream << static_cast<float>(Helicopter->lpb.avg_left.index);           // АВИАГОРИЗОНТ ЛЕВ-индекс	float	0,125 стрелка на ободе
        data_stream << static_cast<float>(Helicopter->lpb.avg_left.shift_pitch);     // АВИАГОРИЗОНТ ЛЕВ-центровка шкалы тангажа	float	0 крутилка со стрелками
        data_stream << static_cast<bool>(Helicopter->lpb.avg_left.blank);            // АВИАГОРИЗОНТ ЛЕВ:Флажок отс. питания	bool
        data_stream << static_cast<float>(Helicopter->lpb.avg_left.shift_pitch );    // АВИАГОРИЗОНТ крутилка юстировка
        data_stream << static_cast<float>(Helicopter->lpb.ugr_4uk.dir());            // УГР - текущий курс
        data_stream << static_cast<float>(Helicopter->lpb.ugr_4uk.d1());             // УГР - тослстая стрелка
        data_stream << static_cast<float>(Helicopter->lpb.ugr_4uk.ark());            // УГР - тонкая стрелка
        data_stream << static_cast<float>(Helicopter->lpb.ugr_4uk.encoder );         // УГР - вращалка юстировки
        data_stream << static_cast<bool>(Helicopter->lpb.ss.i_ss || Helicopter->rsp_ep.t_check_lamp);    // Индикатор МС - лампа ВЫК
        data_stream << static_cast<float>(Helicopter->lpb.ss.speed_x);               // Индикатор МС-вперёд-назад
        data_stream << static_cast<float>(Helicopter->lpb.ss.speed_z);               // Индикатор МС-влево-вправо
        data_stream << static_cast<float>(Helicopter->lpb.ss.speed_y);               // Индикатор МС-вверх-вниз
        data_stream << static_cast<float>(Helicopter->lpb.vr_30mk.var);              // Подъем/спуск стрелка	float	#ЗНАЧ!
        data_stream << static_cast<float>(Helicopter->lpb.ite_1t.nv);                // Несущий винт обороты - стрелка	float	0,069444
        data_stream << static_cast<float>(Helicopter->lpb.avg_res.roll);             // АВИАГОРИЗОНТ ПРАВ - YAW	float	0
        data_stream << static_cast<float>(Helicopter->lpb.avg_res.getPitch( ) );     // АВИАГОРИЗОНТ ПРАВ - PITCH	float	0
        data_stream << static_cast<float>(Helicopter->lpb.avg_res.yawl);             // АВИАГОРИЗОНТ ПРАВ - раысканье	float	0  крен-roll
        data_stream << static_cast<float>(Helicopter->lpb.avg_res.index);            // АВИАГОРИЗОНТ ПРАВ - индекс	float	0,125 стрелка на ободе
        data_stream << static_cast<float>(Helicopter->lpb.avg_res.shift_pitch);      // АВИАГОРИЗОНТ ПРАВ - центровка шкалы тангажа	float	0 крутилка со стрелками
        data_stream << static_cast<bool>(Helicopter->lpb.avg_res.blank);             // АВИАГОРИЗОНТ ПРАВ : Флажок отс. питания	bool
        data_stream << static_cast<float>(Helicopter->lpb.avg_res.shift_pitch );     // АВИАГОРИЗОНТ ПРАВ : Крутилка юстировки
        data_stream << static_cast<float>(Helicopter->lpb.ite_2t.eng1);              // Двигатели 1 обороты - стрелка	float	0,069444
        data_stream << static_cast<float>(Helicopter->lpb.ite_2t.eng2);              // Двигатели 2 обороты - стрелка	float	0,069444
        data_stream << static_cast<bool>(Helicopter->lpb.i_warning_earth || Helicopter->rsp_ep.t_check_lamp); // Индикатор ВНИМЕНИЕ ЗЕМЛЯ
        data_stream << static_cast<bool>(Helicopter->lpb.i_danger_earth || Helicopter->rsp_ep.t_check_lamp);  // Индикатор ОПАСНО ЗЕМЛЯ
        data_stream << static_cast<bool>(Helicopter->lpb.i_roll_max || Helicopter->rsp_ep.t_check_lamp);      // Индикатор КРЕН ВЕЛИК
        data_stream << static_cast<bool>(Helicopter->lpb.i_cso1_yellow || Helicopter->rsp_ep.t_check_lamp);   // Индикатор ЦСО1 желтый
        data_stream << static_cast<bool>(Helicopter->lpb.i_cso1_red || Helicopter->rsp_ep.t_check_lamp);      // Индикатор ЦСО1 красный   
        data_stream << static_cast<bool>(Helicopter->lpb.i_ag_net_kontr || Helicopter->rsp_ep.t_check_lamp);  // Индикатор НЕТ КОНТР АГ
        data_stream << static_cast<bool>(Helicopter->lpb.i_ag_left_brok || Helicopter->rsp_ep.t_check_lamp);  // Индикатор ОТКАЗ АГ ЛЕВ
        data_stream << static_cast<bool>(Helicopter->lpb.i_ag_res_brok || Helicopter->rsp_ep.t_check_lamp);   // Индикатор ОТКАЗ АГ РЕЗЕРВ

        data_stream << static_cast<bool>(Helicopter->lpb.i_main_gear_swarf || Helicopter->rsp_ep.t_check_lamp); // Индикатор СТРУЖКА ГЛ. РЕДУК.
        data_stream << static_cast<bool>(Helicopter->lpb.i_inter_gear_swarf || Helicopter->rsp_ep.t_check_lamp); // Индикатор СТРУЖКА ПРОМ. РЕД.
        data_stream << static_cast<bool>(Helicopter->lpb.i_tail_gear_swarf || Helicopter->rsp_ep.t_check_lamp); // Индикатор СТРУЖКА ХВ. РЕДУК.
        data_stream << static_cast<bool>(Helicopter->lpb.i_eng_left_vibr_dang || Helicopter->rsp_ep.t_check_lamp); // Индикатор ВИБР. ОПАС. ЛЕВ. ДВИГ.
        data_stream << static_cast<bool>(Helicopter->lpb.i_eng_left_swarf || Helicopter->rsp_ep.t_check_lamp); // Индикатор СТРУЖКА ЛЕВ.ДВИГ.
        data_stream << static_cast<bool>(Helicopter->lpb.i_eng_left_p_oil || Helicopter->rsp_ep.t_check_lamp); // Индикатор МАЛО. РМ. ЛЕВ. ДВИГ.
        data_stream << static_cast<bool>(Helicopter->lpb.i_eng_left_emrg || Helicopter->rsp_ep.t_check_lamp); // Индикатор Ч.Р.ЛЕВ.ДВИГ.
        data_stream << static_cast<bool>(Helicopter->lpb.i_nv_max || Helicopter->rsp_ep.t_check_lamp); // Индикатор п.Н.В ВЫСОКИЕ
        data_stream << static_cast<bool>(Helicopter->lpb.i_eng_left_tf || Helicopter->rsp_ep.t_check_lamp); // Индикатор ЗАСОР. ТФ. ЛЕВ. ДВИГ.
        data_stream << static_cast<bool>(Helicopter->lpb.i_eng_left_vibr_max || Helicopter->rsp_ep.t_check_lamp); // Индикатор ВИБР. ПОВ. ЛЕВ. ДВИГ.
        data_stream << static_cast<bool>(Helicopter->lpb.i_eng_left_max_rpm || Helicopter->rsp_ep.t_check_lamp); // Индикатор Превышение оборотов свободной турбины левого двигателя.
        data_stream << static_cast<bool>(Helicopter->lpb.i_eng_left_erd_off || Helicopter->rsp_ep.t_check_lamp); // Индикатор ОТКЛ. ЭРД. ЛЕВ. ДВИГ.
        data_stream << static_cast<bool>(Helicopter->lpb.i_nv_min || Helicopter->rsp_ep.t_check_lamp); // Индикатор п.Н.В НИЗКИЕ
        data_stream << static_cast<bool>(Helicopter->lpb.i_eng_right_vibr_dang || Helicopter->rsp_ep.t_check_lamp); // Индикатор ВИБР ОПАС ПРАВ ДВИГ
        data_stream << static_cast<bool>(Helicopter->lpb.i_eng_right_swarf || Helicopter->rsp_ep.t_check_lamp); // Индикатор СТРУЖКА ПРАВ.ДВИГ.
        data_stream << static_cast<bool>(Helicopter->lpb.i_eng_right_p_oil || Helicopter->rsp_ep.t_check_lamp); // Индикатор МАЛО РМ. ПРАВ. ДВИГ.
        data_stream << static_cast<bool>(Helicopter->lpb.i_eng_right_emrg || Helicopter->rsp_ep.t_check_lamp); // Индикатор Ч.Р.ПРАВ.ДВИГ.
        data_stream << static_cast<bool>(Helicopter->lpb.i_main_gear_p_oil || Helicopter->rsp_ep.t_check_lamp); // Индикатор МАЛО РМ ГЛ РЕДУК
        data_stream << static_cast<bool>(Helicopter->lpb.i_eng_right_tf || Helicopter->rsp_ep.t_check_lamp); // Индикатор ЗАСОР. ТФ. ПРАВ. ДВИГ.
        data_stream << static_cast<bool>(Helicopter->lpb.i_eng_right_vibr_max || Helicopter->rsp_ep.t_check_lamp); // Индикатор ВИБР. ПОВ. ПРАВ. ДВИГ.
        data_stream << static_cast<bool>(Helicopter->lpb.i_eng_right_max_rpm || Helicopter->rsp_ep.t_check_lamp); // Индикатор Превышение оборотов свободной турбины правого двигателя
        data_stream << static_cast<bool>(Helicopter->lpb.i_eng_right_erd_off || Helicopter->rsp_ep.t_check_lamp); // Индикатор ОТКЛ. ЭРД. ПРАВ. ДВИГ.
        
        data_stream << static_cast<bool>(Helicopter->lpb.i_eng_left_limit || Helicopter->rsp_ep.t_check_lamp); // Индикатор ОГР РЕЖ ЛЕВ
        data_stream << static_cast<bool>(Helicopter->lpb.i_eng_right_limit || Helicopter->rsp_ep.t_check_lamp); // Индикатор ОГР РЕЖ ПРАВ

        data_stream << static_cast<float>(Helicopter->lpb.ki_206.course());  // KI206 подвижная шкала(компаса)
        data_stream << static_cast<float>(Helicopter->lpb.ki_206.course());   // Ручка задатчика
        data_stream << static_cast<bool>(Helicopter->lpb.ki_206.i_from_to);  // НАВИГАТОР Индикатор FROM - TO
        data_stream << static_cast<bool>(Helicopter->lpb.ki_206.i_gs);       // Навигатор флажок GS
        data_stream << static_cast<bool>(Helicopter->lpb.ki_206.i_nav);      // НАВИГАТОР Флажок NAV
        data_stream << static_cast<float>(Helicopter->lpb.ki_206.dir);       // НАВИГАТОР - линия вертикали (носа)
        data_stream << static_cast<float>(Helicopter->lpb.ki_206.glis);      // НАВИГАТОР - линия горизонтали
        data_stream << static_cast<bool>(0);                           // Индикатор ОМЫВАТЕЛЬ СТЕКОЛ
        data_stream << static_cast<bool>(Helicopter->UV26.t_pusk);           // УВ-26 ПУСК
        data_stream << static_cast<bool>(Helicopter->UV26.t_stop);           // УВ-26 СТОП
        data_stream << static_cast<bool>(Helicopter->lpb.t_dnmr);            // ОПОЗНАВ НАВ<->ДНМР(ДАЛЬНОМЕР)
        data_stream << static_cast<float>(0);                          // Крутилка ПОДСВЕТ АГ
//  14 Центральный Пульт
        //KDI
        data_stream << static_cast<float>(Helicopter->rpb.kdi_572.i_S);//табло КДИ Лев
        data_stream << static_cast<float>(Helicopter->rpb.kdi_572.led[3] * 100 + Helicopter->rpb.kdi_572.led[4] * 10 + Helicopter->rpb.kdi_572.led[5]);//табло КДИ Центр
        data_stream << static_cast<float>(Helicopter->rpb.kdi_572.i_Time);//табло КДИ Прав
        data_stream << static_cast<quint8>(Helicopter->rpb.kdi_572.sw_switch); //крутилка КДИ(0 - 3)
        //KT76
        data_stream << static_cast<bool>(Helicopter->ccp.KT76_t_IDT);//Кнопка IDT
        data_stream << static_cast<bool>(Helicopter->ccp.KT76_t_0);//Кнопка 0
        data_stream << static_cast<bool>(Helicopter->ccp.KT76_t_1);//Кнопка 1
        data_stream << static_cast<bool>(Helicopter->ccp.KT76_t_2);//Кнопка 2
        data_stream << static_cast<bool>(Helicopter->ccp.KT76_t_3);//Кнопка 3
        data_stream << static_cast<bool>(Helicopter->ccp.KT76_t_4);//Кнопка 4
        data_stream << static_cast<bool>(Helicopter->ccp.KT76_t_5);//Кнопка 5
        data_stream << static_cast<bool>(Helicopter->ccp.KT76_t_6);//Кнопка 6
        data_stream << static_cast<bool>(Helicopter->ccp.KT76_t_7);//Кнопка 7
        data_stream << static_cast<bool>(Helicopter->ccp.KT76_t_CLR);//Кнопка CLR
        data_stream << static_cast<bool>(Helicopter->ccp.KT76_t_VFR);//Кнопка VFR
        data_stream << static_cast<quint8>(Helicopter->ccp.kt76_gal());//Галетник(0 - off и тд 4 - alt)
            qint32 alt = static_cast<qint32>(Helicopter->lpb.vd_10vk.get_alt() - 2.);
            if (alt < 0)
                alt = 0;
        data_stream << static_cast<quint32>(alt);//Табло КТ76 - ЛЕВОЕ
        data_stream << static_cast<quint32>(Helicopter->ccp.KT76_BORT);//Табло КТ76 - ПРАВОЕ
        //KN53
        data_stream << static_cast<bool>(Helicopter->ccp.KN53_str);//Кнопка стрелки
        data_stream << static_cast<bool>(Helicopter->ccp.KN53_on);//вращалка ON(bool так как два положения on и off)
        data_stream << static_cast<float>(Helicopter->ccp.KN53_freqK);//Вращалка частоты
        data_stream << static_cast<quint32>((100 + Helicopter->ccp.KN53_LCD_leftM ) * 100 + Helicopter->ccp.KN53_LCD_leftK);//Табло КН 53 - USE(слева)
        data_stream << static_cast<quint32>((100 + Helicopter->ccp.KN53_LCD_rightM) * 100  + Helicopter->ccp.KN53_LCD_rightK);//Табло КН 53 - STBY(справа)
        data_stream << static_cast<float>(Helicopter->ccp.f_t_oil_inter_gb);         // Шкала T МАСЛА ПРОМ. РЕД	float	#ЗНАЧ!
        data_stream << static_cast<float>(Helicopter->ccp.f_t_oil_tail_gb);          // Шкала T МАСЛА ХВОСТ. РЕД	float	#ЗНАЧ!
        data_stream << static_cast<float>(Helicopter->ccp.f_p_oil_main_gb);          // Шкала P МАСЛА ГЛ. РЕДУК.	float	0:08
        data_stream << static_cast<float>(Helicopter->ccp.f_t_oil_main_gb);          // Шкала T МАСЛА ГЛ. РЕДУК.	float	#ЗНАЧ!
        data_stream << static_cast<float>(Helicopter->ccp.f_p_oil_left_eng);         // Шкала ЛЕВЫЙ ДВИГАТЕЛЬ ДАВЛЕНИЕ	float	0:08
        data_stream << static_cast<float>(Helicopter->ccp.f_t_oil_left_eng);         // Шкала ЛЕВЫЙ ДВИГАТЕЛЬ T МАСЛА	float	#ЗНАЧ!
        data_stream << static_cast<float>(Helicopter->ccp.f_p_oil_rght_eng);         // Шкала ПРАВЫЙ ДВИГАТЕЛЬ ДАВЛЕНИЕ	float	0:08
        data_stream << static_cast<float>(Helicopter->ccp.f_t_oil_rght_eng);         // Шкала ПРАВЫЙ ДВИГАТЕЛЬ T МАСЛА	float	#ЗНАЧ!
        data_stream << static_cast<bool>(Helicopter->ccp.t_ap_speed);                // Тумблер СТАБ V
        data_stream << static_cast<bool>(Helicopter->ccp.i_ap_speed || Helicopter->rsp_ep.t_check_lamp); // Индикатор СТАБ V
        data_stream << static_cast<quint8>(Helicopter->ccp.led_OD);                   // Вращалка ПУЛЬТ управления БУРТ-2Ж
        data_stream << static_cast<bool>(Helicopter->ccp.t_bur_on);                  // Тумблер BUR ВКЛ
        data_stream << static_cast<bool>(Helicopter->ccp.i_otkaz_bur || Helicopter->ccp.t_kontr_lamp || Helicopter->rsp_ep.t_check_lamp);
        data_stream << static_cast<bool>(Helicopter->ccp.i_otkaz_en || Helicopter->ccp.t_kontr_lamp || Helicopter->rsp_ep.t_check_lamp);
        data_stream << static_cast<bool>(Helicopter->ccp.t_kontr_lamp);              // Кнопка КОНТР ЛАМП
        data_stream << static_cast<bool>(Helicopter->ccp.t_cbros);                   // Кнопка СБРОС
        data_stream << static_cast<bool>(Helicopter->ccp.t_vvod);                    // Кнопка ВВОД
        data_stream << static_cast<bool>(Helicopter->ccp.t_ust);             // Кнопка КОНТР ВВОДА
        data_stream << static_cast<bool>(Helicopter->ccp.t_od);                   // Кнопка СДВИГ
        data_stream << static_cast<quint32>(Helicopter->ccp.led_left);
        data_stream << static_cast<float>(Helicopter->ccp.f_1);                      // Вращалка 1	float	0:01
        data_stream << static_cast<float>(Helicopter->ccp.f_2);                      // Вращалка 2	float	0:01
        data_stream << static_cast<float>(Helicopter->ccp.f_3);                      // Вращалка 3	float	0:01
        data_stream << static_cast<float>(Helicopter->ccp.f_get_curs);               // Планка НАПРАВЛЕНИЕ
        data_stream << static_cast<float>(Helicopter->ccp.f_get_roll_grad());        // Планка КРЕН
        data_stream << static_cast<float>(Helicopter->ccp.f_get_pitch);              // Планка ТАНГАЖ
        data_stream << static_cast<float>(Helicopter->ccp.f_get_alt);                // Планка ВЫСОТА
        data_stream << static_cast<float>(Helicopter->ccp.f_spu52_ind );              // Планка КОНТРОЛЬ ВЛЕВО-ВПРАВО
        data_stream << static_cast<bool>(Helicopter->ccp.t_spu52_off);               // Кнопка КОНТРОЛЬ ОТКЛ	bool
        data_stream << static_cast<float>(Helicopter->ccp.f_spu52);                  // Вращалка КОНТРОЛЬ	float	0:01
        data_stream << static_cast<bool>(Helicopter->ccp.t_spu52_p);                 // Тумблер КОНТРОЛЬ P	bool
        data_stream << static_cast<bool>(Helicopter->ccp.t_spu52_t);                 // Тумблер КОНТРОЛЬ t	bool
        data_stream << static_cast<bool>(Helicopter->ccp.t_sublight_ki_13);          // Тумблер ПОДСВЕТ КИ-13
        data_stream << static_cast<bool>(Helicopter->ccp.t_sublight_azs);            // Тумблер ПОДСВЕТ ПАНЕЛИ АЗС
        data_stream << static_cast<bool>(0);                                    // Тумблер ИК
        data_stream << static_cast<bool>(0);                                    // Тумблер НАСТРОЙКА
        data_stream << static_cast<bool>(0);      // Тубмлер Радиостанция Баклан 20 ПШ
        data_stream << static_cast<bool>(0);       // Тубмлер Радиостанция Баклан 20 ПЛ
        data_stream << static_cast<float>(0);    // Радиостанция Баклан 20 Левая вращалка
        data_stream << static_cast<float>(0);   // Радиостанция Баклан 20 Правая вращалка
        data_stream << static_cast<float>(0);     // Радиостанция Баклан 20 Вращалка громкость
        data_stream << static_cast<bool>(0);       // Радиостанция Баклан 20 Кнопка АС
        data_stream << static_cast<float>(Helicopter->ccp.f_set_curs);               // АВТОПИЛОТ - Вращалка НАПРАВЛЕНИЕ	float	0:01
        data_stream << static_cast<float>(Helicopter->ccp.f_set_roll);               // АВТОПИЛОТ - Вращалка КРЕН	float	0:01
        data_stream << static_cast<float>(Helicopter->ccp.f_set_pitch);              // АВТОПИЛОТ - Вращалка ТАНГАЖ	float	0:01
        data_stream << static_cast<bool>(Helicopter->ccp.t_ap_alt_up);               // АВТОПИЛОТ - Тумблер Контроль
        data_stream << static_cast<bool>(Helicopter->ccp.t_ap_alt_down);             // АВТОПИЛОТ - Тумблер Контроль
        data_stream << static_cast<bool>(Helicopter->ccp.i_ap_dir_off);              // АВТОПИЛОТ - Кнопка НАПРАВЛЕНИЕ ОТКЛ	bool
        data_stream << static_cast<bool>(Helicopter->ccp.i_ap_dir_on);               // АВТОПИЛОТ - Кнопка НАПРАВЛЕНИЕ ВКЛ	bool
        data_stream << static_cast<bool>(Helicopter->ccp.i_ap_on);                   // АВТОПИЛОТ - Кнопка средняя ВКЛ	bool
        data_stream << static_cast<bool>(Helicopter->ccp.i_ap_alt_on);               // Кнопка ВЫСОТА ВКЛ	bool
        data_stream << static_cast<bool>(Helicopter->ccp.i_ap_alt_off);              // Кнопка ВЫСОТА ОТКЛ	bool
        data_stream << static_cast<float>(0);              // Шкала секунды
        data_stream << static_cast<float>(0);              // Шкала минуты
        data_stream << static_cast<float>(0);             // Шкала часы
        data_stream << static_cast<float>(0);             // Вращалка часы левая
        data_stream << static_cast<float>(0);            // Вращалка часы правая
        data_stream << static_cast<bool>(Helicopter->ccp.t_main_off_ab);             // Тумблер ГЛАВЫЙ ВЫКЛ АБ
        data_stream << static_cast<bool>(Helicopter->ccp.t_emerg_drop);              // Тумблер АВРИЙНЫЙ СБРОС
        data_stream << static_cast<bool>(Helicopter->ccp.t_explosion);               // Тумблер ВЗРЫВ
        data_stream << static_cast<bool>(Helicopter->ccp.t_po1);                     // Тумблер ПО1
        data_stream << static_cast<bool>(Helicopter->ccp.t_po2);                     // Тумблер ПО2
        data_stream << static_cast<bool>(Helicopter->ccp.i_ab || Helicopter->rsp_ep.t_check_lamp);        // Лампа АБ
        data_stream << static_cast<bool>(Helicopter->ccp.i_explosion || Helicopter->rsp_ep.t_check_lamp); // Лампа ВЗРЫВ
        data_stream << static_cast<bool>(Helicopter->ccp.t_cargo_cabin);             // Тумблер ГРУЗ КАБИНА
        data_stream << static_cast<bool>(Helicopter->ccp.t_crew);                    // Тумблер ЭКИПАЖ
        data_stream << static_cast<bool>(Helicopter->ccp.t_day);                     // Тумблер ДЕНЬ
        data_stream << static_cast<bool>(Helicopter->ccp.t_night);                   // Тумблер НОЧЬ
        data_stream << static_cast<bool>(Helicopter->ccp.i_power || Helicopter->rsp_ep.t_check_lamp);    // Лампа ПИТАНИЕ
        data_stream << static_cast<bool>(Helicopter->ccp.i_brace || Helicopter->rsp_ep.t_check_lamp);    // Лампа РАСЧАЛКА
        data_stream << static_cast<bool>(Helicopter->ccp.i_removed || Helicopter->rsp_ep.t_check_lamp); // Лампа УБРАНА
        data_stream << static_cast<bool>(Helicopter->ccp.i_work || Helicopter->rsp_ep.t_check_lamp);    // Лампа РАБОТА
        data_stream << static_cast<bool>(Helicopter->ccp.i_dropped || Helicopter->rsp_ep.t_check_lamp); // Лампа ВЫПУЩЕНА
        data_stream << static_cast<bool>(Helicopter->ccp.i_EL || Helicopter->rsp_ep.t_check_lamp || Helicopter->ccp.t_control);
        data_stream << static_cast<bool>(Helicopter->ccp.i_NO || Helicopter->rsp_ep.t_check_lamp || Helicopter->ccp.t_control);
        data_stream << static_cast<bool>(Helicopter->ccp.i_VT || Helicopter->rsp_ep.t_check_lamp || Helicopter->ccp.t_control);
        data_stream << static_cast<bool>(Helicopter->ccp.i_PO || Helicopter->rsp_ep.t_check_lamp || Helicopter->ccp.t_control);
        data_stream << static_cast<bool>(Helicopter->ccp.i_TZ || Helicopter->rsp_ep.t_check_lamp || Helicopter->ccp.t_control);
        data_stream << static_cast<bool>(Helicopter->ccp.i_TO || Helicopter->rsp_ep.t_check_lamp || Helicopter->ccp.t_control);
        data_stream << static_cast<bool>(Helicopter->ccp.i_SG || Helicopter->rsp_ep.t_check_lamp || Helicopter->ccp.t_control);
        data_stream << static_cast<bool>(Helicopter->ccp.i_OV || Helicopter->rsp_ep.t_check_lamp || Helicopter->ccp.t_control);
        data_stream << static_cast<bool>(Helicopter->ccp.i_ZV || Helicopter->rsp_ep.t_check_lamp || Helicopter->ccp.t_control);
        data_stream << static_cast<bool>(Helicopter->ccp.t_control );       // Кнопка КОНТРОЛЬ 
        data_stream << static_cast<bool>(0);
        data_stream << static_cast<bool>(0);
        data_stream << static_cast<bool>(0);
//  15 Правая Приборная Панель
        data_stream << static_cast<bool>(Helicopter->rpb.t_fara_close);          // Тумблер СВЕТ	bool	Убрана
        data_stream << static_cast<bool>(Helicopter->rpb.t_fara_on);             // Тумблер СВЕТ	bool	Свет
        data_stream << static_cast<bool>(Helicopter->rpb.t_projector);           // Тумблер УПРАВЛ	bool	Свет
        data_stream << static_cast<float>(Helicopter->rpb.f_fuel);               // ТОПЛИВОМЕР	float	1,944444
        data_stream << static_cast<bool>(Helicopter->rpb.i_fuel_270 || Helicopter->rsp_ep.t_check_lamp);  // Индикатор ОСТАЛОСЬ 270 Л	bool
        data_stream << static_cast<bool>(Helicopter->rpb.i_dptb_left || Helicopter->rsp_ep.t_check_lamp); // Индикатор ДПТБ ПУСТ ЛЕВ
        data_stream << static_cast<bool>(Helicopter->rpb.i_dptb_right || Helicopter->rsp_ep.t_check_lamp);// Индикатор ДПТБ ПУСТ ПРАВ
        data_stream << static_cast<quint8>(Helicopter->rpb.sw_fuel());       // Переключатель топливомера
        data_stream << static_cast<quint8>(Helicopter->rpb.get_dop_fuel());      // Переключатель доп баков
        data_stream << static_cast<quint8>(Helicopter->rpb.kdi_572.sw_switch);   // Переключатель KDI-572
        data_stream << static_cast<float>(Helicopter->rpb.kdi_572.f_1);          // Индикатор KDI-572 1
        data_stream << static_cast<float>(Helicopter->rpb.kdi_572.f_2);          // Индикатор KDI-572 2
        data_stream << static_cast<float>(Helicopter->rpb.kdi_572.f_3);          // Индикатор KDI-572 3
        data_stream << static_cast<bool>(Helicopter->rpb.t_alarm);               // Тумблер СИГНАЛИЗАЦИЯ
        data_stream << static_cast<float>(Helicopter->rpb.uc_450k.speed());      // Спидометр-стрелка	float	0,3125
        data_stream << static_cast<float>(Helicopter->rpb.vd_10vk.get_km());     // Высотометр (КМ)-стрелка	float	0:10
        data_stream << static_cast<float>(Helicopter->rpb.vd_10vk.get_m());      // Высотометр (М)-стрелка	float	0:1000
        data_stream << static_cast<float>(Helicopter->rpb.vd_10vk.p);            // Высотометр давление	float	670:790
        data_stream << static_cast<float>(Helicopter->rpb.vd_10vk.f_barometr);   // Высотометр-вращалка
        data_stream << static_cast<float>(Helicopter->rpb.avg.roll);             // АВИАГОРИЗОНТ ПРАВ-КРЕН	float	-180:180
        data_stream << static_cast<float>(Helicopter->rpb.avg.getPitch( ) );     // АВИАГОРИЗОНТ ПРАВ-ТАНГАЖ	float	-180:180
        data_stream << static_cast<float>(Helicopter->rpb.avg.yawl);             // АВИАГОРИЗОНТ ПРАВ-РЫСКАНИЕ	float	-180:180
        data_stream << static_cast<float>(Helicopter->rpb.avg.index);            // АВИАГОРИЗОНТ ПРАВ-индекс	float	:180
        data_stream << static_cast<float>(Helicopter->rpb.avg.shift_pitch);      // АВИАГОРИЗОНТ ПРАВ-центровка шкалы тангажа	float	-180:180
        data_stream << static_cast<bool>(Helicopter->rpb.avg.blank);             // АВИАГОРИЗОНТ ПРАВ:Флажок отс. питания	bool
        data_stream << static_cast<float>(Helicopter->rpb.avg.shift_pitch );     // АВИАГОРИЗОНТ ПРАВ крутилка юстировки
        data_stream << static_cast<float>(Helicopter->rpb.ugr_4uk.dir());        // УГР - Текущий курс
        data_stream << static_cast<float>(Helicopter->rpb.ugr_4uk.d1());           // УГР - Толстая стрелка
        data_stream << static_cast<float>(Helicopter->rpb.ugr_4uk.ark());          // УГР - Тонкая стрелка
        data_stream << static_cast<float>(Helicopter->rpb.ugr_4uk.encoder );     // УГР - Центровка
        data_stream << static_cast<float>(Helicopter->rpb.snos.angle);           // УГОЛ СНОСА прибор	float	-40:40
        data_stream << static_cast<float>(Helicopter->rpb.snos.speed);           // Спидометр - км/час	float	0:10
        data_stream << static_cast<bool>(Helicopter->rpb.snos.i_angle);          // УГОЛ СНОСА - индикатор	bool	
        data_stream << static_cast<bool>(FP->Par169);                      // УГОЛ СНОСА - Переключатель Р-К	bool	true-К
        data_stream << static_cast<bool>(Helicopter->rpb.snos.t_cm);             // УГОЛ СНОСА - Переключатель С-М	bool	true = М
        data_stream << static_cast<float>(Helicopter->rpb.vr_30mk.var);          // Подъем/спуск стрелка	float	#ЗНАЧ!
        data_stream << static_cast<float>(Helicopter->rpb.ite_1t.nv);            // Несущий винт обороты - стрелка	float	0,069444
        data_stream << static_cast<float>(Helicopter->rpb.ite_2t.eng1);          // Двигатели 1 обороты - стрелка	float	0,069444
        data_stream << static_cast<float>(Helicopter->rpb.ite_2t.eng2);          // Двигатели 2 обороты - стрелка	float	0,069444
        data_stream << static_cast<bool>(Helicopter->rpb.diss_15.t_rght);        // Кнопка ВПР
        data_stream << static_cast<bool>(Helicopter->rpb.diss_15.t_left);        // Кнопка ВЛ
        data_stream << static_cast<quint8>(Helicopter->rpb.diss_15.sw_side);     // БОКОВОЕ УКЛОНЕНИЕ ВЛЕВО-ВПРАВО
        data_stream << static_cast<float>(Helicopter->rpb.diss_15.get_side());   // Боковое уклонение-значение
        data_stream << static_cast<bool>(Helicopter->rpb.diss_15.t_v);           // Кнопка В
        data_stream << static_cast<bool>(Helicopter->rpb.diss_15.t_n);           // Кнопка Н
        data_stream << static_cast<float>(Helicopter->rpb.diss_15.get_way());    // Путь-значение   TODO : исправить ДИСС !!!
        data_stream << static_cast<quint8>(Helicopter->rpb.diss_15.sw_way);      // ПУТЬ ВПЕРЕД-НАЗАД
        data_stream << static_cast<bool>(Helicopter->rpb.diss_15.t_plus);        // Кнопка +
        data_stream << static_cast<bool>(Helicopter->rpb.diss_15.t_minus);       // Кнопка -
        data_stream << static_cast<float>(Helicopter->rpb.diss_15.f_angle);      // Угол карты-значение TODO кажется тут не совпадают типы
        data_stream << static_cast<bool>(Helicopter->rpb.diss_15.t_on);          // Кнопка ВКЛ
        data_stream << static_cast<bool>(Helicopter->rpb.diss_15.t_off);         // Кнопка ОТКЛ
        data_stream << static_cast<bool>(Helicopter->rpb.diss_15.i_on || Helicopter->rsp_ep.t_check_lamp);    // ДИСС - лампа ВКЛЮЧЕНО	bool
        data_stream << static_cast<bool>(!(Helicopter->rpb.diss_15.i_on) || Helicopter->rsp_ep.t_check_lamp); // ДИСС - лампа ОТКЛ
        data_stream << static_cast<bool>(Helicopter->rpb.i_diss_brok || Helicopter->rsp_ep.t_check_lamp);     // Индикатор ДИСС ОТКАЗ	bool
        data_stream << static_cast<bool>(Helicopter->rpb.i_ag_brok || Helicopter->rsp_ep.t_check_lamp);       // Индикатор ОТКАЗ АГ ПРАВ	bool
        data_stream << static_cast<bool>(Helicopter->rpb.i_warning_earth || Helicopter->rsp_ep.t_check_lamp); // Индикатор ВНИМЕНИЕ ЗЕМЛЯ
        data_stream << static_cast<bool>(Helicopter->rpb.i_danger_earth || Helicopter->rsp_ep.t_check_lamp);  // Индикатор ОПАСНО ЗЕМЛЯ
        data_stream << static_cast<bool>(Helicopter->rpb.i_pitch_max || Helicopter->rsp_ep.t_check_lamp);     // Индикатор ТАНГАЖ ВЕЛИК	
        data_stream << static_cast<bool>(Helicopter->rpb.i_roll_max || Helicopter->rsp_ep.t_check_lamp);      // Индикатор КРЕН ВЕЛИК	bool IP->Par022
        data_stream << static_cast<float>(Helicopter->rpb.ki_206.course());      // KI206 - текущий курс
        data_stream << static_cast<bool>(Helicopter->rpb.ki_206.i_from_to);      // KI206 Индикатор FROM TO
        data_stream << static_cast<bool>(Helicopter->rpb.ki_206.i_gs);           // KI206 флажок GS
        data_stream << static_cast<bool>(Helicopter->rpb.ki_206.i_nav);          // KI206 Флажок NAV
        data_stream << static_cast<float>(Helicopter->rpb.ki_206.dir);           // KI206 - линия вертикали (носа)
        data_stream << static_cast<float>(Helicopter->rpb.ki_206.glis);          // KI206 - линия горизонтали
        data_stream << static_cast<float>(Helicopter->rpb.f_temp);               // Термометр ТЕМПЕРАТУРА В ПАССАЖИР. КАБ. 
        data_stream << static_cast<float>(Helicopter->PC.f_compass);             // Показания компаса 
        data_stream << static_cast<float>(0); // Подсвет АГ
        data_stream << static_cast<bool>(Helicopter->rpb.t_dnmr); // НАВ/ДНМР
        data_stream << static_cast<bool>(0); // ОМЫВ СТЕКОЛ
//  16 Центральный пульт (наклонная часть)
    /*Перемещено на центральный пульт 14*/
//  17 Рычаги и педали
        data_stream << static_cast<float>((Helicopter->PC.f_roll / 128.) - 1);   // Штурвал ВЛЕВО-ВПРАВО -1:1,90град:-90град
        data_stream << static_cast<float>(1 - (Helicopter->PC.f_pitch / 128.));  // Штурвал ВПЕРЕД-НАЗАД -1:1,90град:-90град
        data_stream << static_cast<quint8>(0);                              // Лев штурвал - Кнопка СПУ РАДИО
        data_stream << static_cast<bool>(Helicopter->PC.t_left_trim);            // Лев штурвал - Кнопка ЭЛЕКТРОМАГНИТНЫЙ ТОРМОЗ
        data_stream << static_cast<bool>(Helicopter->PC.t_left_fire);            // Лев штурвал - Левая красная кнопка
        data_stream << static_cast<bool>(Helicopter->PC.t_left_ap_off);          // Лев штурвал - Кнопка ОТКЛЮЧЕНИЕ АВТОПИЛОТА
        data_stream << static_cast<quint8>(0);                              // Прав штурвал -Кнопка СПУ РАДИО 0-ВЫКЛ, 1-КАБИНА, 2- ДИСПЕТЧЕР
        data_stream << static_cast<bool>(0);                                // Прав штурвал - Кнопка ЭЛЕКТРОМАГНИТНЫЙ ТОРМОЗ
        data_stream << static_cast<bool>(Helicopter->PC.t_right_fire);           // Прав штурвал-Левая красная кнопка	bool
        data_stream << static_cast<bool>(Helicopter->PC.t_right_ap_off);         // Прав штурвал - Кнопка ОТКЛЮЧЕНИЕ АВТОПИЛОТА	bool
        data_stream << static_cast<float>(0);                               // Барашек фрикциона	float	0:1,0-от себя, 1-на себя
        data_stream << static_cast<bool>(Helicopter->PC.t_fpp_l_up);             // Кнопка управления фарой вверх	bool
        data_stream << static_cast<bool>(Helicopter->PC.t_fpp_l_down);           // Кнопка управления фарой вниз	bool
        data_stream << static_cast<bool>(Helicopter->PC.t_fpp_l_left);           // Кнопка управления фарой влево	bool
        data_stream << static_cast<bool>(Helicopter->PC.t_fpp_l_right);          // Кнопка управления фарой вправо	bool
        data_stream << static_cast<bool>(Helicopter->PC.t_avar_drop);           // Кнопка Аварийного сброса груза	bool
        data_stream << static_cast<bool>(Helicopter->PC.t_tact_drop);           // Кнопка тактического сброса груза	bool
        data_stream << static_cast<bool>(Helicopter->PC.t_fr_l_fosh);            // Кнопка фрикциона	bool
        data_stream << static_cast<bool>(Helicopter->PC.t_rpm_up || Helicopter->PC.t_rpm_down);//Тумблер	bool
        data_stream << static_cast<bool>(Helicopter->PC.t_fpp_r_up);             // Кнопка управления фарой вверх	bool
        data_stream << static_cast<bool>(Helicopter->PC.t_fpp_r_down);           // Кнопка управления фарой вниз 	bool
        data_stream << static_cast<bool>(Helicopter->PC.t_fpp_r_left);           // Кнопка управления фарой влево	bool
        data_stream << static_cast<bool>(Helicopter->PC.t_fpp_r_right);          // Кнопка управления фарой вправо	bool
        data_stream << static_cast<bool>(Helicopter->PC.t_tact_drop);           // Кнопка тактического сброса груза	bool
        data_stream << static_cast<bool>(Helicopter->PC.t_fr_l_fosh);            // Кнопка фрикциона	bool она одна на всех
        data_stream << static_cast<float>(Helicopter->PC.f_eng1 / 255.);         //Руд левый	float	0:1,0-от себя
        data_stream << static_cast<float>(Helicopter->PC.f_eng2 / 255.);         //Руд правый	float	0:1,0-от себя
        data_stream << static_cast<float>(Helicopter->PC.f_Fosh / 255.);         //Рукоятка шаг-газ	float	0:1,0-от себя, 1-на себя
        data_stream << static_cast<float>(Helicopter->PC.f_Ecor / 255.);         //Ручка коррекции	float	0:1,0-от себя
        data_stream << static_cast<float>(( Helicopter->PC.f_yawl / 128.) - 1);      //Педали Лев Пилот
        data_stream << static_cast<float>(Helicopter->PC.t_NV_brake_on ? 1.f : 0.f);//Рычаг тормоза винта
        data_stream << static_cast<bool>(0);                                //Рычаг тормоза винта кнопка
        data_stream << static_cast<float>(Helicopter->PC.t_eng_rght_brake ? 1.f : 0.f);// Остановка двигателя ПРАВ	float
        data_stream << static_cast<float>(Helicopter->PC.t_eng_left_brake ? 1.f : 0.f);// Остановка двигателя ЛЕВ	float
        data_stream << static_cast<bool>( Helicopter->PC.t_safety_lock );             // Предохранительный выключатель вооружения
        data_stream << static_cast<bool>(Helicopter->PC.i_weapon_warning || Helicopter->rsp_ep.t_check_lamp); // Лампа ОСТОРОЖНО ЦЕПИ ВООРУЖЕНИЯ ПОД ТОКОМ
        data_stream << static_cast<bool>(Helicopter->PC.t_bomb_drop);            // Кнопка сброса бомб
        
        data_stream << static_cast<float>(Helicopter->PC.pkt_x*1.6 - 80);            // ПКТ-горизонтальный угол
        data_stream << static_cast<float>(Helicopter->PC.pkt_y*0.7 - 10);            // ПКТ-вертикальный угол
        data_stream << static_cast<bool>(Helicopter->PC.pkt_fire );        // ПКТ-кнопки

//  18 Щиток вооружения командира
        data_stream << static_cast<quint8>(Helicopter->WeaponPult.get_sw_weapon_type());           // Галетный переключатель
        data_stream << static_cast<bool>(Helicopter->WeaponPult.t_fire_short );              // Тумблер ОЧЕРЕДЬ МРС КОРОТКАЯ
        data_stream << static_cast<bool>(Helicopter->WeaponPult.t_fire_long);               // Тумблер ОЧЕРЕДЬ МРС ДЛИННАЯ
        data_stream << static_cast<bool>(Helicopter->WeaponPult.t_fire_midle);              // Тумблер ОЧЕРЕДЬ МРС СРЕДНЯЯ
        data_stream << static_cast<bool>(Helicopter->WeaponPult.t_pod_ext);                 // Тумблер ПОДВЕСКА ВНЕШН
        data_stream << static_cast<bool>(Helicopter->WeaponPult.t_pod_all);                    // Тумблер ПОДВЕСКА ВСЕ
        data_stream << static_cast<bool>(Helicopter->WeaponPult.t_pod_inter);                 // Тумблер ПОДВЕСКА 

//  19 Пульт Контроля ДИСС
        data_stream << static_cast<bool>(Helicopter->pc_diss.i_m || Helicopter->rsp_ep.t_check_lamp);     // Индикатор М	bool
        data_stream << static_cast<bool>(Helicopter->pc_diss.i_contr || Helicopter->rsp_ep.t_check_lamp); // Индикатор КОНТР	bool
        data_stream << static_cast<bool>(Helicopter->pc_diss.i_work || Helicopter->rsp_ep.t_check_lamp);  // Индикатор РАБОТА	bool
        data_stream << static_cast<bool>(Helicopter->pc_diss.i_b || Helicopter->rsp_ep.t_check_lamp);     // Индикатор В	bool
        data_stream << static_cast<quint8>(Helicopter->pc_diss.sw_mode);                             // Галетный Переключатель	uint8
//  20 Щиток электросрабатывателя
        data_stream << static_cast<bool>( Helicopter->WeaponPult.i_bd_1_load || Helicopter->rsp_ep.t_check_lamp ); // Индикатор БД1 ЗАГРУЖ
        data_stream << static_cast<bool>( Helicopter->WeaponPult.i_bd_2_load || Helicopter->rsp_ep.t_check_lamp ); // Индикатор БД2 ЗАГРУЖ
        data_stream << static_cast<bool>( Helicopter->WeaponPult.i_bd_3_load || Helicopter->rsp_ep.t_check_lamp ); // Индикатор БД3 ЗАГРУЖ
        data_stream << static_cast<bool>( Helicopter->WeaponPult.i_bd_4_load || Helicopter->rsp_ep.t_check_lamp ); // Индикатор БД4 ЗАГРУЖ
        data_stream << static_cast<bool>( Helicopter->WeaponPult.i_vzriv || Helicopter->rsp_ep.t_check_lamp ); // Индикатор АВАР ВЗРЫВ
        data_stream << static_cast<bool>( Helicopter->WeaponPult.i_weapon_line || Helicopter->rsp_ep.t_check_lamp ); // Индикатор СЕТЬ БВ ВКЛЮЧ.
        data_stream << static_cast<bool>( Helicopter->WeaponPult.t_sbros); // Тумблер СБРОС БОМБ
        data_stream << static_cast<bool>( Helicopter->WeaponPult.t_vzriv); // Тумблер ВЗРЫВ
        data_stream << static_cast<bool>( Helicopter->WeaponPult.t_main_switch); // Тумблер ГЛАВНЫЙ ВЫКЛЮЧ
        data_stream << static_cast<bool>( Helicopter->WeaponPult.t_pus_load_PO1); // Тумблер ПО1-ПО2
//  21. Пульты управления
    //Пульт УВ - 26
        data_stream << static_cast<quint32>( Helicopter->UV26.tablo );   //Табло
        data_stream << static_cast<bool>( Helicopter->UV26.i_left );   //Индикатор левый борд
        data_stream << static_cast<bool>( Helicopter->UV26.i_right );   //Индикатор правый борд
        data_stream << static_cast<bool>( Helicopter->UV26.t_left );   //Тумблер левый борд
        data_stream << static_cast<bool>( Helicopter->UV26.t_right );   //Тумблер правый борд
        data_stream << static_cast<bool>( Helicopter->UV26.t_prog );   //Тумблер НАЛ - ПРОГ
        data_stream << static_cast<bool>( Helicopter->UV26.t_seria );   //Кнопка СЕРИЯ
        data_stream << static_cast<bool>( Helicopter->UV26.t_interval );   //Кнопка Интервал
        data_stream << static_cast<bool>( Helicopter->UV26.t_zalp );   //Кнопка Залп
        data_stream << static_cast<bool>( Helicopter->UV26.t_reset );   //Кнопка сброс
        data_stream << static_cast<bool>( Helicopter->UV26.t_stop );   //Кнопка Стоп
        data_stream << static_cast<bool>( Helicopter->UV26.t_pusk );   //Кнопка ПУСК
    //ДИСС - 32
        data_stream << static_cast<bool>(Helicopter->pc_diss.i_vss ); //Индикатор Левый край 1 сверху
        data_stream << static_cast<bool>(Helicopter->pc_diss.i_vch ); //Индикатор Левый край 2 сверху
        data_stream << static_cast<bool>(Helicopter->pc_diss.i_bvk ); //Индикатор Левый край 3 сверху
        data_stream << static_cast<bool>(Helicopter->pc_diss.i_flight ); //Индикатор Правы край 1 сверху
        data_stream << static_cast<bool>(Helicopter->pc_diss.i_ispr); //Индикатор Правы край 2 сверху
        data_stream << static_cast<bool>(Helicopter->pc_diss.i_search); //Индикатор Правы край 3сверху
        data_stream << static_cast<bool>(Helicopter->pc_diss.t_contr_flag); //Кнопка 1 вверхний ряд
        data_stream << static_cast<bool>(Helicopter->pc_diss.t_up_left); //Кнопка 2 вверхний ряд
        data_stream << static_cast<bool>(Helicopter->pc_diss.t_back_right); //Кнопка 3 вверхний ряд
        data_stream << static_cast<bool>(Helicopter->pc_diss.t_poisk_flag); //Кнопка 1 нижний ряд
        data_stream << static_cast<bool>(Helicopter->pc_diss.t_spedd127); //Кнопка 2 нижний ряд
        data_stream << static_cast<bool>(Helicopter->pc_diss.t_speed258); //Кнопка 3 нижний ряд
    //КМ - 8 Коррекционный механизм       Helicopter->pc_diss.t_speed258 |
        data_stream << static_cast<float>(Helicopter->PC.f_compass);//Стрелка Черная КУрс
        data_stream << static_cast<float>(Helicopter->PC.f_compass - 5.);//Стрелка Белая Магнитное отклонение
    //Пульт имитации обледенения стоит за правым пилотом
        data_stream << static_cast<bool>(Helicopter->lp_elp.i_imitation_hot);//Индикатор Обогрев
        data_stream << static_cast<bool>(Helicopter->lp_elp.i_imitation_cold);//Индикатор Обледен
        data_stream << static_cast<bool>(Helicopter->lp_elp.t_imitation);//Кнопка Имитации


        QByteArray header;
        QDataStream header_stream(&header, QIODevice::WriteOnly);
        header_stream.setByteOrder(QDataStream::LittleEndian);
        header_stream << static_cast<quint8>('S');
        header_stream << static_cast<quint8>('t');
        header_stream << static_cast<quint8>('o');
        header_stream << static_cast<quint8>('O');
        header_stream << static_cast<quint32>(500);
        header_stream << static_cast<quint32>(data.size());

        QByteArray packet;
        packet.append(header);
        packet.append(data);
        TcpSocket->write(packet);

        //if(_Instructor->exerciseStarted) _replay->WritePacketData(_Instructor->ExerciseParameters->Uuid,ClientPacketTypes::ControlsImitatorPacket,quint32(ControlsImitatorTCPPacketTypes::SR2CI_CommanderControlsDataPacket),packet);
    }
}

// Тут ничего не должно приходить, если что то получено, то это мусор, который надо прочитать из сокета и удалить?
void Helicopter_OU_TCP_Client::ReadCommonTcpData()
{
    //bool processedHeader );
    if (TcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        auto availabeBytes = TcpSocket->bytesAvailable();
        while (availabeBytes >= 0)
        {
            TcpSocket->read(availabeBytes);
        }
    }
}
