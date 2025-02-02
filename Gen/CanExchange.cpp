#include "../Helicopter/Helicopter_controls.h"
#include "../Helicopter/Helicopter_BoardSystem.h"
void Helicopter_controls::CanExchange()
{
// class.property unsupported type // commentarry
// 1 АЗС. unsupported  // 
	Helicopter->rp_azs.t_ta_start = GetGPIOINBitValue( 0 );	//Турбоагрегат запуск
	Helicopter->rp_azs.t_ta_ign = GetGPIOINBitValue( 1 );	//Турбоагрегат зажигание
	Helicopter->rp_azs.t_eng_start = GetGPIOINBitValue( 2 );	//Двигатели запуск
	Helicopter->rp_azs.t_eng_ign = GetGPIOINBitValue( 3 );	//Двигатели зажигание
	Helicopter->rp_azs.t_pump_consum = GetGPIOINBitValue( 4 );	//Топливная система - насосы топливн баков - расход
	Helicopter->rp_azs.t_pump_left = GetGPIOINBitValue( 5 );	//Топливная система - насосы топливн баков - левого
	Helicopter->rp_azs.t_pump_right = GetGPIOINBitValue( 6 );	//Топливная система - насосы топливн баков - правого
	Helicopter->rp_azs.t_fuel_gauge = GetGPIOINBitValue( 7 );	//Топливная система - топливомер
	Helicopter->rp_azs.t_spuu_52 = GetGPIOINBitValue( 8 );	//СПУУ - 52
	Helicopter->rp_azs.t_auto_main = GetGPIOINBitValue( 9 );	//Автопилот - основн
	Helicopter->rp_azs.t_auto_fric = GetGPIOINBitValue( 10 );	//Автопилот - фрикц
	Helicopter->rp_azs.t_auto_emufta = GetGPIOINBitValue( 11 );	//Автопилот - электромуфта
	Helicopter->rp_azs.t_hidro_main = GetGPIOINBitValue( 12 );	//Гидросист - основн
	Helicopter->rp_azs.t_hidro_sec = GetGPIOINBitValue( 13 );	//Гидросист - дублир
	Helicopter->rp_azs.t_sas = GetGPIOINBitValue( 14 );	//САС
	Helicopter->rp_azs.t_nav1 = GetGPIOINBitValue( 15 );	//НАВ1
	Helicopter->rp_azs.t_nav2 = GetGPIOINBitValue( 16 );	//НАВ2
	Helicopter->rp_azs.t_distmeter = GetGPIOINBitValue( 17 );	//Дальномер
	Helicopter->rp_azs.t_n_ctrl = GetGPIOINBitValue( 18 );	//Управление оборотами
	Helicopter->rp_azs.t_close_left = GetGPIOINBitValue( 19 );	//Перекрывающие краны - левый
	Helicopter->rp_azs.t_close_rght = GetGPIOINBitValue( 20 );	//Перекрывающие краны - правый
	Helicopter->rp_azs.t_close_bypass = GetGPIOINBitValue( 21 );	//Кран перепус
	Helicopter->rp_azs.t_ppo_signal = GetGPIOINBitValue( 22 );	//Тумблер СИГНАЛИЗАЦ
	Helicopter->rp_azs.t_ppo_1_auto = GetGPIOINBitValue( 23 );	//Тумблер I ОЧЕРЕДЬ АВТОМ
	Helicopter->rp_azs.t_ppo_1_hand = GetGPIOINBitValue( 24 );	//Тумблер I ОЧЕРЕДЬ РУЧН
	Helicopter->rp_azs.t_ppo_2_auto = GetGPIOINBitValue( 25 );	//Тумблер 2 ОЧЕРЕДЬ АВТОМ(РУЧНАЯ-строка закомментированна)
	Helicopter->rp_azs.t_ppo_2_hand = GetGPIOINBitValue( 26 );	//Тумблер 2 ОЧЕРЕДЬ РУЧНАЯ
	Helicopter->rp_azs.t_r_spu = GetGPIOINBitValue( 27 );	//Радио - СПУ
	Helicopter->rp_azs.t_r_altimetr = GetGPIOINBitValue( 28 );	//Радио - Высотомер
	Helicopter->rp_azs.t_r_comand_osnv = GetGPIOINBitValue( 29 );	//Радио - Команд основ
	Helicopter->rp_azs.t_pust_01 = GetGPIOINBitValue( 30 );	//Тумблер - Пустой
	Helicopter->rp_azs.t_r_compas_sv = GetGPIOINBitValue( 31 );	//Радио - Компас СВ
	Helicopter->rp_azs.t_r_compas_ukv = GetGPIOINBitValue( 32 );	//Радио - Компас УКВ
	Helicopter->rp_azs.t_resp_uvd1 = GetGPIOINBitValue( 33 );	//Радио - Ответчик УВД №1
	Helicopter->rp_azs.t_resp_uvd2 = GetGPIOINBitValue( 34 );	//Радио - Ответчик УВД №2
	Helicopter->rp_azs.t_r_rls = GetGPIOINBitValue( 35 );	//Радио - РЛС
	Helicopter->rp_azs.t_far_left_ctrl = GetGPIOINBitValue( 36 );	//Фары - Левая - Управл
	Helicopter->rp_azs.t_far_left_flash = GetGPIOINBitValue( 37 );	//Фары - Левая - Свет
	Helicopter->rp_azs.t_far_right_ctrl = GetGPIOINBitValue( 38 );	//Фары - Правая - Управл
	Helicopter->rp_azs.t_far_right_flash = GetGPIOINBitValue( 39 );	//Фары - Правая - Свет
	Helicopter->rp_azs.t_siren_missle = GetGPIOINBitValue( 40 );	//Ракеты Сирена
	Helicopter->rp_azs.t_strov_light = GetGPIOINBitValue( 41 );	//Строев Огни
	Helicopter->rp_azs.t_ano = GetGPIOINBitValue( 42 );	//АНО
	Helicopter->rp_azs.t_tact_sbros = GetGPIOINBitValue( 43 );	//Такт сброс груза
	Helicopter->rp_azs.t_avar_sbros = GetGPIOINBitValue( 44 );	//Авар сброс груза
	Helicopter->rp_azs.t_ais_ctrl = GetGPIOINBitValue( 45 );	//Противообледенительная система - Управление
	Helicopter->rp_azs.t_ais_pzu_left = GetGPIOINBitValue( 46 );	//Противообледенительная система - ПЗУ Левого Двиг
	Helicopter->rp_azs.t_ais_pzu_rght = GetGPIOINBitValue( 47 );	//Противообледенительная система - ПЗУ Правого Двиг
	Helicopter->rp_azs.t_ais_co_121 = GetGPIOINBitValue( 48 );	//Противообледенительная система - СО - 121
	Helicopter->rp_azs.t_ais_glass = GetGPIOINBitValue( 49 );	//Противообледенительная система - Стекол
	Helicopter->rp_azs.t_ais_wipe_left = GetGPIOINBitValue( 50 );	//Стеклоочиститель - левый
	Helicopter->rp_azs.t_ais_wipe_rght = GetGPIOINBitValue( 51 );	//Стеклоочиститель - правый
	Helicopter->rp_azs.t_p_nv = GetGPIOINBitValue( 52 );	//П НВ
	Helicopter->rp_azs.t_ko_50 = GetGPIOINBitValue( 53 );	//КО - 50
// 2 Левая боковая панель электропульта. unsupported  // 
// 2.1 Контроль. unsupported  // 
	Helicopter->lsp_elp.t_IV_500 = GetGPIOINBitValue( 54 );	//Кнопка ИВ-500Е
	Helicopter->lsp_elp.t_ground = GetGPIOINBitValue( 55 );	//Кнопка ЗЕМЛЯ
	Helicopter->lsp_elp.t_air = GetGPIOINBitValue( 56 );	//Кнопка ВОЗДУХ
// Helicopter->lsp_elp.t_rt_left unsupported  // РТ двигателя левого(нет на панели и вертолете)
// Helicopter->lsp_elp.t_rt_rght unsupported  // РТ двигателя правого(нет на панели и вертолете)
	Helicopter->lsp_elp.t_bur = GetGPIOINBitValue( 57 );	//ОТКЛ БУР(invers t_tbn)
	Helicopter->lsp_elp.t_tbn = GetGPIOINBitValue( 58 );	//ТБН
// 2.2 Пульт управления ракетами ЭП-662. unsupported  // 
	Helicopter->lsp_elp.t_tumblr_1 = GetGPIOINBitValue( 59 );	//Тумблер 1
	Helicopter->lsp_elp.t_button_1_1 = GetGPIOINBitValue( 60 );	//Кнопка 1
	Helicopter->lsp_elp.t_button_1_2 = GetGPIOINBitValue( 61 );	//Кнопка 2(не указано слово)
	Helicopter->lsp_elp.t_button_1_3 = GetGPIOINBitValue( 62 );	//Кнопка 3
	Helicopter->lsp_elp.t_button_1_4 = GetGPIOINBitValue( 63 );	//Кнопка 4
	Helicopter->lsp_elp.t_tumblr_2 = GetGPIOINBitValue( 64 );	//Тумблер 2
	Helicopter->lsp_elp.t_button_2_1 = GetGPIOINBitValue( 65 );	//Кнопка 1
	Helicopter->lsp_elp.t_button_2_2 = GetGPIOINBitValue( 66 );	//Кнопка 2
	Helicopter->lsp_elp.t_button_2_3 = GetGPIOINBitValue( 67 );	//Кнопка 3
	Helicopter->lsp_elp.t_button_2_4 = GetGPIOINBitValue( 68 );	//Кнопка 4
// 2.3 Пульт дистанционного управления аппаратурой РИ-65Б. unsupported  // 
	Helicopter->lsp_elp.t_ri_off = GetGPIOINBitValue( 69 );	//Кнопка ОТКЛ
	Helicopter->lsp_elp.t_ri_check = GetGPIOINBitValue( 70 );	//Кнопка ПРОВЕРКА
	Helicopter->lsp_elp.t_ri_rep = GetGPIOINBitValue( 71 );	//Кнопка ПОВТ
// Helicopter->lsp_elp.t_ri_gain unsupported  // Тумблер УСИЛ(отсутствует в вертолете)
	//SetGPIOOUTBitValue( Helicopter->lsp_elp.i_hv_tral || Helicopter->rsp_ep.t_check_lamp, 0 );	//Лампа ХВ ТРАЛ ОТКРЫТ
	SetGPIOOUTBitValue(Helicopter->lsp_elp.i_ogr_nTr_Left || Helicopter->rsp_ep.t_check_lamp, 0 );// Лампа ОГРnTr LEFT
	SetGPIOOUTBitValue(Helicopter->lsp_elp.i_ogr_nTr_Right || Helicopter->rsp_ep.t_check_lamp, 194);// Лампа ОГРnTr RIGHT
	SetVIDSteps( 0, Helicopter->lsp_elp.f_p_air );	//Манометр МВУ-10К(0-255,100-344)на верте стоит справа, адрес 1004(3)
	SetVIDSteps( 1, Helicopter->lsp_elp.f_p_brake );	//Манометр МА-60К(0-355,60-3521)на верте стоит слева, адрес 1004(2)
	Helicopter->lsp_elp.t_ano = GetGPIOINBitValue( 72 );	//Кнопка КОД-АНО
	Helicopter->lsp_elp.t_siren = GetGPIOINBitValue( 73 );	//Кнопка СИРЕНА
	SetGPIOOUTBitValue( Helicopter->lsp_elp.i_siren_on || Helicopter->rsp_ep.t_check_lamp, 1 );	//Индикатор СИРЕНА ВКЛЮЧЕНА
	Helicopter->lsp_elp.t_ext_lock = GetGPIOINBitValue( 74 );	//Тумблер ВНЕШНЯЯ ПОДВЕСКА
	SetGPIOOUTBitValue( Helicopter->lsp_elp.i_cargo_dump || Helicopter->rsp_ep.t_check_lamp, 2 );	//Индикатор ГРУЗ СБРОШЕН
	Helicopter->lsp_elp.t_sublight_1 = GetGPIOINBitValue( 75 );	//Красный подсвет группа 1
	Helicopter->lsp_elp.t_sublight_2 = GetGPIOINBitValue( 76 );	//Красный подсвет группа 2
// Helicopter->lsp_elp.t_fan unsupported  // Вентилятор(выведен на прямую)
	Helicopter->lsp_elp.t_cargo_dump = GetGPIOINBitValue( 77 );	//Тумблер аварийный сброс груза лебедка / внешняя подвеска
// 2.4 Пульт управления магнитофоном П503Б. unsupported  // 
	Helicopter->lsp_elp.t_recorder = GetGPIOINBitValue( 78 );	//Тумблер вкл / выкл
	Helicopter->lsp_elp.t_autostart = GetGPIOINBitValue( 79 );	//Тумблер автоспуск / непрерывная работа
	Helicopter->lsp_elp.t_spu_lar = GetGPIOINBitValue( 80 );	//Тумблер СПУ / ЛАР
// Helicopter->lsp_elp.f_sublight unsupported  // Подсвет
	SetGPIOOUTBitValue( Helicopter->lsp_elp.i_record || Helicopter->rsp_ep.t_check_lamp || Helicopter->lsp_elp.t_recorder, 3 );	//Индикатор ЗАПИСЬ up
// 2.5 Остальные элементы. unsupported  // 
	Helicopter->lsp_elp.t_disasters = GetGPIOINBitValue( 81 );	//Тумблер бедств
	Helicopter->lsp_elp.t_stiran = GetGPIOINBitValue( 82 );	//Тумблер стиран
	Helicopter->lsp_elp.t_smoke = GetGPIOINBitValue( 83 );	//Тумблер не курить 1-вверх
	Helicopter->lsp_elp.t_belts = GetGPIOINBitValue( 84 );	//Тумблер не курить пристегнуть ремни 8-низ
	Helicopter->lsp_elp.t_exit_tab = GetGPIOINBitValue( 85 );	//Тумблер табло выход
	Helicopter->lsp_elp.t_ri = GetGPIOINBitValue( 86 );	//Тумблер речевой регистратор
	SetGPIOOUTBitValue( Helicopter->lsp_elp.i_falut_rr || Helicopter->rsp_ep.t_check_lamp, 4 );	//Лампа ОТКАЗ РР
	SetGPIOOUTBitValue( Helicopter->lsp_elp.i_vkl_zapas || Helicopter->rsp_ep.t_check_lamp, 5 );	//Лампа ВКЛЮЧИ ЗАПАСНОЙ
// 5 Левая Панель АЗС. unsupported  // 
	SetGPIOOUTBitValue( Helicopter->lp_azs.i_weapon_chain || Helicopter->rsp_ep.t_check_lamp, 6 );	//Светосигнальное табло ЦЕПИ ВООРУЖ ОТКЛ
	SetGPIOOUTBitValue( Helicopter->lp_azs.i_ab || Helicopter->rsp_ep.t_check_lamp, 7 );	//Светосигнальное табло АБ(красная)
	SetGPIOOUTBitValue( Helicopter->lp_azs.i_explosion || Helicopter->rsp_ep.t_check_lamp, 8 );	//Светосигнальное табло ВЗРЫВ(красная)
	SetGPIOOUTBitValue( Helicopter->lp_azs.i_pus_bd1 || Helicopter->rsp_ep.t_check_lamp, 9 );	//Светосигнальное табло ПУС ВЗВЕДН БД1 БД6(желтая)
	SetGPIOOUTBitValue( Helicopter->lp_azs.i_nps || Helicopter->rsp_ep.t_check_lamp, 10 );	//Светосигнальное табло НПС(желтая)
	SetGPIOOUTBitValue( Helicopter->lp_azs.i_pus_bd2 || Helicopter->rsp_ep.t_check_lamp, 11 );	//Светосигнальное табло ПУС ВЗВЕДН БД2 БД5(желтая)
	SetGPIOOUTBitValue( Helicopter->lp_azs.i_upk || Helicopter->rsp_ep.t_check_lamp, 12 );	//Светосигнальное табло УПК(зеленая)
	SetGPIOOUTBitValue( Helicopter->lp_azs.i_bd1_load || Helicopter->rsp_ep.t_check_lamp, 13 );	//Светосигнальное табло БД 1 ЗАГРУЖ(желтая)
	SetGPIOOUTBitValue( Helicopter->lp_azs.i_bd2_load || Helicopter->rsp_ep.t_check_lamp, 14 );	//Светосигнальное табло БД 2 ЗАГРУЖ(желтая)
	SetGPIOOUTBitValue( Helicopter->lp_azs.i_bd3_load || Helicopter->rsp_ep.t_check_lamp, 15 );	//Светосигнальное табло БД 3 ЗАГРУЖ(желтая)
	SetGPIOOUTBitValue( Helicopter->lp_azs.i_bd4_load || Helicopter->rsp_ep.t_check_lamp, 16 );	//Светосигнальное табло БД 4 ЗАГРУЖ(желтая)
	SetGPIOOUTBitValue( Helicopter->lp_azs.i_bd5_load || Helicopter->rsp_ep.t_check_lamp, 17 );	//Светосигнальное табло БД 5 ЗАГРУЖ(желтая)
	SetGPIOOUTBitValue( Helicopter->lp_azs.i_bd6_load || Helicopter->rsp_ep.t_check_lamp, 18 );	//Светосигнальное табло БД 6 ЗАГРУЖ(желтая)
	Helicopter->lp_azs.t_pus_cocking = GetGPIOINBitValue( 87 );	//КНОПКА ВЗВЕДЕНИЕ ПУС
	Helicopter->lp_azs.t_main_switch_on = GetGPIOINBitValue( 88 );	//Тумблер ГЛАВНЫЙ ВЫКЛЮЧАТЕЛЬ ВКЛ
	Helicopter->lp_azs.t_main_switch_off = GetGPIOINBitValue( 89 );	//Тумблер ГЛАВНЫЙ ВЫКЛЮЧАТЕЛЬ ВЫКЛ
	Helicopter->lp_azs.t_explosion = GetGPIOINBitValue( 90 );	//Тумблер ВЗРЫВ - НЕВЗРЫВ
	Helicopter->lp_azs.t_emerg_dump = GetGPIOINBitValue( 91 );	//Тумблер АВАРИЙНЫЙ СБРОС БОМБ Б8 УПК
	Helicopter->lp_azs.t_sns_nav = GetGPIOINBitValue( 92 );	//Тумблер СНС НАВ
	Helicopter->lp_azs.t_uvd_count = GetGPIOINBitValue( 93 );	//Тумблер СЧЕТЧИК УВД
	Helicopter->lp_azs.t_distmeter = GetGPIOINBitValue( 94 );	//Тумблер ДАЛЬНОМЕР
	Helicopter->lp_azs.t_sublight_pkv = GetGPIOINBitValue( 95 );	//Тумблер ПОДСВЕТ ПКВ
	Helicopter->lp_azs.t_power_onv_left = GetGPIOINBitValue( 96 );	//Тумблер ПИТАНИЕ ОНВ ЛЕВЫЙ
	Helicopter->lp_azs.t_power_onv_board = GetGPIOINBitValue( 97 );	//Тумблер ПИТАНИЕ ОНВ БОРТ - ТЕХНИК
	Helicopter->lp_azs.t_power_onv_right = GetGPIOINBitValue( 98 );	//Тумблер ПИТАНИЕ ОНВ ПРАВЫЙ
// Helicopter->lp_azs.f_sublight_pkv unsupported  // КРУТИЛКА ПОДСВЕТ ПКВ
	SetGPIOOUTBitValue( Helicopter->lp_azs.i_upk_right || Helicopter->rsp_ep.t_check_lamp, 19 );	//Лампа над правым УПК
	SetGPIOOUTBitValue( Helicopter->lp_azs.i_upk_right, 20 );	//Счетчик снарядов для правого УПК №1 (вычитатель)
// Helicopter->lp_azs.f_ammo_count_upk_right_2 unsupported  // Счетчик снарядов для правого УПК №2 (вычитатель)
	SetGPIOOUTBitValue( Helicopter->lp_azs.i_upk_left || Helicopter->rsp_ep.t_check_lamp, 21 );	//Лампа над левым УПК
	SetGPIOOUTBitValue( Helicopter->lp_azs.i_upk_left, 22 );	//Счетчик снарядов для левого УПК №1 (вычитатель)
// Helicopter->lp_azs.f_ammo_count_upk_left_2 unsupported  // Счетчик снарядов для левого УПК №2 (вычитатель)
// 6 ЛЕВЫЙ ЩИТОК ЭЛЕКТРОПУЛЬТА. unsupported  // 
// Helicopter->lsh_elp.sw_glass_wiper unsupported  // Переключатель СТЕКЛООЧИСТИТЕЛЬ(unit-1: 40-2скор,38-пуск,36-сброс,35-1скор)
	Helicopter->lsh_elp.t_rv = GetGPIOINBitValue( 99 );	//Тумблер выключатель радиовысотомера
	Helicopter->lsh_elp.t_spuu_52 = GetGPIOINBitValue( 100 );	//Тумблер Выключатель СПУУ-52
// Helicopter->lsh_elp.t_plafon unsupported  // Тумблер ПЛАФОН(подключен на прямую)
	Helicopter->lsh_elp.t_ag_left = GetGPIOINBitValue( 101 );	//Тумблер АГ ЛЕВ
	Helicopter->lsh_elp.t_ag_res = GetGPIOINBitValue( 102 );	//Тумблер АГ РЕЗЕРВ
	Helicopter->lsh_elp.t_bkk_18 = GetGPIOINBitValue( 103 );	//Тумблер БКК-18
	Helicopter->lsh_elp.t_ri_65 = GetGPIOINBitValue( 104 );	//Тумблер РИ-65
	Helicopter->lsh_elp.t_bkk_contr_up = GetGPIOINBitValue( 105 );	//Тумблер I КОНТР БКК
	Helicopter->lsh_elp.t_bkk_contr_down = GetGPIOINBitValue( 106 );	//Тумблер II КОНТР БКК
	Helicopter->lsh_elp.t_pvd_heat = GetGPIOINBitValue( 107 );	//Кнопка КОНТРОЛЬ ОБОГРЕВА ПВД
	SetGPIOOUTBitValue( Helicopter->lsh_elp.i_bkk_ok || Helicopter->rsp_ep.t_check_lamp, 23 );	//Индикатор ИСПРАВН. БКК
	SetGPIOOUTBitValue( Helicopter->lsh_elp.i_ri_65 || Helicopter->rsp_ep.t_check_lamp, 24 );	//Индикатор ВКЛЮЧИ РИ-65
	SetGPIOOUTBitValue( Helicopter->lsh_elp.i_pvd_heat_ok || Helicopter->rsp_ep.t_check_lamp, 25 );	//Индикатор ОБОГРЕВ ИСПРАВЕН
// 7 ЛЕВАЯ ПАНЕЛЬ ЭЛЕКТРОПУЛЬТА. unsupported  // 
	Helicopter->lp_elp.t_deicing_auto = GetGPIOINBitValue( 108 );	//Тумблер(ПРОТИВООБЛЕДЕНИТЕЛЬНАЯ СИСТЕМА) Автомат
	Helicopter->lp_elp.t_deicing_off = GetGPIOINBitValue( 109 );	//Кнопка(ПРОТИВООБЛЕДЕНИТЕЛЬНАЯ СИСТЕМА) ОТКЛ
	Helicopter->lp_elp.t_deicing_left_pzu = GetGPIOINBitValue( 110 );	//Тумблер(ПРОТИВООБЛЕДЕНИТЕЛЬНАЯ СИСТЕМА) ДВИГ ПЗУ ЛЕВ
	Helicopter->lp_elp.t_deicing_right_pzu = GetGPIOINBitValue( 111 );	//Тумблер(ПРОТИВООБЛЕДЕНИТЕЛЬНАЯ СИСТЕМА) ДВИГ ПЗУ ПРАВ
	Helicopter->lp_elp.t_deicing_glass = GetGPIOINBitValue( 112 );	//Тумблер(ПРОТИВООБЛЕДЕНИТЕЛЬНАЯ СИСТЕМА) ОБОГРЕВ 
// Helicopter->lp_elp.sw_consumers unsupported  // Галетный Переключатель ТОК ПОТРЕБИТЕЛЕЙ
	Helicopter->lp_elp.t_A_offret = GetGPIOINBitValue( 113 );	//ВЫК
	Helicopter->lp_elp.t_A_blade_1 = GetGPIOINBitValue( 114 );	//1
	Helicopter->lp_elp.t_A_blade_2 = GetGPIOINBitValue( 115 );	//2
	Helicopter->lp_elp.t_A_blade_3 = GetGPIOINBitValue( 116 );	//3
	Helicopter->lp_elp.t_A_blade_4 = GetGPIOINBitValue( 117 );	//4
	Helicopter->lp_elp.t_A_blade_5 = GetGPIOINBitValue( 118 );	//5
	Helicopter->lp_elp.t_A_blade_tail = GetGPIOINBitValue( 119 );	//Хвост винт
	Helicopter->lp_elp.t_A_glass = GetGPIOINBitValue( 120 );	//Стекол
	Helicopter->lp_elp.t_A_PZU_l = GetGPIOINBitValue( 121 );	//ПЗУ ЛЕВ
	Helicopter->lp_elp.t_A_PZU_r = GetGPIOINBitValue( 122 );	//ПЗУ ПРАВ
	SetGPIOOUTBitValue( Helicopter->lp_elp.i_icing || Helicopter->rsp_ep.t_check_lamp, 26 );	//Индикатор ОБЛЕДЕН.
	SetGPIOOUTBitValue( Helicopter->lp_elp.i_pos_on || Helicopter->rsp_ep.t_check_lamp, 27 );	//Светосигнальное табло ПОС ВКЛЮЧЕНА
	SetGPIOOUTBitValue( Helicopter->lp_elp.i_fault_pos || Helicopter->rsp_ep.t_check_lamp, 28 );	//Светосигнальное табло ОТКАЗ ПОС НВ
	SetGPIOOUTBitValue( Helicopter->lp_elp.i_heat_left || Helicopter->rsp_ep.t_check_lamp, 29 );	//Светосигнальное табло ОБОГРЕВ ДВ.ЛЕВ
	SetGPIOOUTBitValue( Helicopter->lp_elp.i_heat_right || Helicopter->rsp_ep.t_check_lamp, 30 );	//Светосигнальное табло ОБОГРЕВ ДВ.ПРАВ.
	SetGPIOOUTBitValue( Helicopter->lp_elp.i_left_pzu_front || Helicopter->rsp_ep.t_check_lamp, 31 );	//Светосигнальное табло ЛЕВ.ПЗУ.ПЕРЕДН.
	SetGPIOOUTBitValue( Helicopter->lp_elp.i_right_pzu_front || Helicopter->rsp_ep.t_check_lamp, 32 );	//Светосигнальное табло ПРАВ.ПЗУ.ПЕРЕДН
	SetGPIOOUTBitValue( Helicopter->lp_elp.i_left_pzu_back || Helicopter->rsp_ep.t_check_lamp, 33 );	//Светосигнальное табло ЛЕВ.ПЗУ.ЗАДН.
	SetGPIOOUTBitValue( Helicopter->lp_elp.i_right_pzu_back || Helicopter->rsp_ep.t_check_lamp, 34 );	//Светосигнальное табло ПРАВ.ПЗУ.ЗАДН
	SetGPIOOUTBitValue( Helicopter->lp_elp.i_so_121_prop || Helicopter->rsp_ep.t_check_lamp, 35 );	//Светосигнальное табло СО - 121 ИСПРАВЕН
	SetGPIOOUTBitValue( Helicopter->lp_elp.i_flight || Helicopter->rsp_ep.t_check_lamp, 36 );	//Светосигнальное табло ПОЛЕТ
	SetGPIOOUTBitValue( Helicopter->lp_elp.i_parking || Helicopter->rsp_ep.t_check_lamp, 37 );	//Светосигнальное табло СТОЯНКА
	SetVIDSteps( 2, Helicopter->lp_elp.f_ampermetr );	//Амперметр - Стрелка(3-345, 15-3040), адрес 1044(3)
	SetGPIOOUTBitValue( Helicopter->lp_elp.i_section_1 || Helicopter->rsp_ep.t_check_lamp, 38 );	//Светосигнальное табло 1 СЕКЦИЯ
	SetGPIOOUTBitValue( Helicopter->lp_elp.i_section_2 || Helicopter->rsp_ep.t_check_lamp, 39 );	//Светосигнальное табло 2 СЕКЦИЯ
	SetGPIOOUTBitValue( Helicopter->lp_elp.i_section_3 || Helicopter->rsp_ep.t_check_lamp, 40 );	//Светосигнальное табло 3 СЕКЦИЯ
	SetGPIOOUTBitValue( Helicopter->lp_elp.i_section_4 || Helicopter->rsp_ep.t_check_lamp, 41 );	//Светосигнальное табло 4 СЕКЦИЯ
	SetLEDINDICATORValue( Helicopter->lp_elp.freq, 0 );	//Радио Баклан
	Helicopter->lp_elp.f_radio_baklan20_1 = POTNormalValue(1);  // Вращалка Радиостанция Баклан 20 №1
	Helicopter->lp_elp.f_radio_baklan20_2 = POTNormalValue(2);  // Вращалка Радиостанция Баклан 20 №2
	Helicopter->lp_elp.e_radio_baklan20_1 = GetENCValue(6);
	Helicopter->lp_elp.e_radio_baklan20_2 = GetENCValue(7);
	Helicopter->lp_elp.t_radio_baklan20_1 = GetGPIOINBitValue( 123 );	//Тубмлер Радиостанция Баклан 20 №1 ПШ
	Helicopter->lp_elp.t_radio_baklan20_2 = GetGPIOINBitValue( 124 );	//Тубмлер Радиостанция Баклан 20 №2 АП
	Helicopter->lp_elp.t_bark_eng_left_on = GetGPIOINBitValue( 125 );	//Тумблер БАРК ДВИГ ЛЕВАЯ Питание
	Helicopter->lp_elp.t_bark_eng_left_wep = GetGPIOINBitValue( 126 );	//Тумблер БАРК ДВИГ ЛЕВАЯ ЧР
	Helicopter->lp_elp.t_bark_eng_left_ct1 = GetGPIOINBitValue( 127 );	//Тумблер БАРК ДВИГ ЛЕВАЯ СТ1(верх?)
	Helicopter->lp_elp.t_bark_eng_left_ct2 = GetGPIOINBitValue( 128 );	//Тумблер БАРК ДВИГ ЛЕВАЯ СТ2
	Helicopter->lp_elp.t_bark_eng_tg_left = GetGPIOINBitValue( 129 );	//Тумблер БАРК ДВИГ ТГ левый
	Helicopter->lp_elp.t_bark_eng_tg_right = GetGPIOINBitValue( 130 );	//Тумблер БАРК ДВИГ ТГ правый
	Helicopter->lp_elp.t_bark_eng_tk_left = GetGPIOINBitValue( 131 );	//Тумблер БАРК ДВИГ ТК левая
	Helicopter->lp_elp.t_bark_eng_tk_right = GetGPIOINBitValue( 132 );	//Тумблер БАРК ДВИГ ТК првая
	Helicopter->lp_elp.t_bark_eng_right_ct1 = GetGPIOINBitValue( 133 );	//Тумблер БАРК ДВИГ ПРАВАЯ СТ1(верх?)
	Helicopter->lp_elp.t_bark_eng_right_ct2 = GetGPIOINBitValue( 134 );	//Тумблер БАРК ДВИГ ПРАВАЯ СТ2
	Helicopter->lp_elp.t_bark_eng_right_wep = GetGPIOINBitValue( 135 );	//Тумблер БАРК ДВИГ ПРАВАЯ ЧР
	Helicopter->lp_elp.t_bark_eng_right_on = GetGPIOINBitValue( 136 );	//Тумблер БАРК ДВИГ ПРАВАЯ ПИТАНИЕ
// 8 Средняя Панель Электропульта. unsupported  // 
	SetGPIOOUTBitValue( Helicopter->cep.i_eng_left_fire || Helicopter->rsp_ep.t_check_lamp, 42 );	//Индикатор ПОЖАР ЛЕВ ДВ
	SetGPIOOUTBitValue( Helicopter->cep.i_eng_left_1_queue || Helicopter->rsp_ep.t_check_lamp, 43 );	//Индикатор ЛЕВ ДВ I ОЧЕРЕДЬ
	SetGPIOOUTBitValue( Helicopter->cep.i_eng_left_2_queue || Helicopter->rsp_ep.t_check_lamp, 44 );	//Индикатор ЛЕВ ДВ II ОЧЕРЕДЬ
	Helicopter->cep.t_eng_left_1_queue = GetGPIOINBitValue( 137 );	//Кнопка I ОЧЕРЕДЬ ЛЕВ ДВ
	Helicopter->cep.t_eng_left_2_queue = GetGPIOINBitValue( 138 );	//Кнопка II ОЧЕРЕДЬ ЛЕВ ДВ
	SetGPIOOUTBitValue( Helicopter->cep.i_eng_rght_fire || Helicopter->rsp_ep.t_check_lamp, 45 );	//Индикатор ПОЖАР ПРАВ ДВ
	SetGPIOOUTBitValue( Helicopter->cep.i_eng_rght_1_queue || Helicopter->rsp_ep.t_check_lamp, 46 );	//Индикатор ПРАВ ДВ I ОЧЕРЕДЬ
	SetGPIOOUTBitValue( Helicopter->cep.i_eng_rght_2_queue || Helicopter->rsp_ep.t_check_lamp, 47 );	//Индикатор ПРАВ ДВ II ОЧЕРЕДЬ
	Helicopter->cep.t_eng_rght_1_queue = GetGPIOINBitValue( 139 );	//Кнопка I ОЧЕРЕДЬ ПРАВ ДВ
	Helicopter->cep.t_eng_rght_2_queue = GetGPIOINBitValue( 140 );	//Кнопка II ОЧЕРЕДЬ ПРАВ ДВ
	SetGPIOOUTBitValue( Helicopter->cep.i_ko_50_fire || Helicopter->rsp_ep.t_check_lamp, 48 );	//Индикатор ПОЖАР КО-50
	SetGPIOOUTBitValue( Helicopter->cep.i_ko_50_1_queue || Helicopter->rsp_ep.t_check_lamp, 49 );	//Индикатор КО-50 I ОЧЕРЕДЬ
	SetGPIOOUTBitValue( Helicopter->cep.i_ko_50_2_queue || Helicopter->rsp_ep.t_check_lamp, 50 );	//Индикатор КО-50 II ОЧЕРЕДЬ
	Helicopter->cep.t_ko_50_1_queue = GetGPIOINBitValue( 141 );	//Кнопка I ОЧЕРЕДЬ КО-50
	Helicopter->cep.t_ko_50_2_queue = GetGPIOINBitValue( 142 );	//Кнопка II ОЧЕРЕДЬ КО-50
	SetGPIOOUTBitValue( Helicopter->cep.i_red_vsu_fire || Helicopter->rsp_ep.t_check_lamp, 51 );	//Индикатор ПОЖАР РЕД. ВСУ
	SetGPIOOUTBitValue( Helicopter->cep.i_red_vsu_1_queue || Helicopter->rsp_ep.t_check_lamp, 52 );	//Индикатор РЕД ВСУ I ОЧЕРЕДЬ
	SetGPIOOUTBitValue( Helicopter->cep.i_red_vsu_2_queue || Helicopter->rsp_ep.t_check_lamp, 53 );	//Индикатор РЕД ВСУ II ОЧЕРЕДЬ
	Helicopter->cep.t_red_vsu_1_queue = GetGPIOINBitValue( 143 );	//Кнопка I ОЧЕРЕДЬ РЕД ВСУ
	Helicopter->cep.t_red_vsu_2_queue = GetGPIOINBitValue( 144 );	//Кнопка II ОЧЕРЕДЬ РЕД ВСУ
	Helicopter->cep.t_contr_sensors = GetGPIOINBitValue( 145 );	//Тумблер КОНТРОЛЬ ДАТЧИКОВ true
	Helicopter->cep.t_firefighting = GetGPIOINBitValue( 146 );	//Тумблер ОГНЕТУШЕНИЕ(inverse t_contr_sensors)
	Helicopter->cep.t_1 = GetGPIOINBitValue( 147 );	//Тумблер КОНТРОЛЬ ПИРОПАТРОНОВ true
	Helicopter->cep.t_2 = GetGPIOINBitValue( 148 );	//Тумблер II(inverse t_1)
// Helicopter->cep.sw_contr_sensors unsupported  // Переключатель КОНТРОЛЬ ДАТЧИКОВ 
	Helicopter->cep.t_contr_sensors_0 = GetGPIOINBitValue( 149 );	//ВЫК
	Helicopter->cep.t_contr_sensors_1 = GetGPIOINBitValue( 150 );	//контроль
	Helicopter->cep.t_contr_sensors_2 = GetGPIOINBitValue( 151 );	//1-К
	Helicopter->cep.t_contr_sensors_3 = GetGPIOINBitValue( 152 );	//2-А
	Helicopter->cep.t_contr_sensors_4 = GetGPIOINBitValue( 153 );	//3-Н
	Helicopter->cep.t_contr_sensors_5 = GetGPIOINBitValue( 154 );	//4-А
	Helicopter->cep.t_contr_sensors_6 = GetGPIOINBitValue( 155 );	//5-Л
	Helicopter->cep.t_contr_sensors_7 = GetGPIOINBitValue( 156 );	//6-Ы
	SetGPIOOUTBitValue( Helicopter->cep.i_contr_sensors || Helicopter->rsp_ep.t_check_lamp, 54 );	//Индикатор КОНТРОЛЬ ДАТЧИКОВ
	SetVIDSteps( 3, Helicopter->cep.f_p_hidro_main );	//Левая шкала-стрелка (0-267,10-1242), адрес 1034(4)
	SetVIDSteps( 4, Helicopter->cep.f_p_hidro_second );	//Правая шкала-стрелка (0-323,100-1140), адрес 1034(1)
	Helicopter->cep.t_hidro_main = GetGPIOINBitValue( 157 );	//Тумблер ГИДРОСИСТЕМА ОСНОВН true=вкл
	SetGPIOOUTBitValue( Helicopter->cep.i_hidro_main || Helicopter->rsp_ep.t_check_lamp, 55 );	//Индикатор ОСНОВН ВКЛЮЧЕНА
	Helicopter->cep.t_hidro_second = GetGPIOINBitValue( 158 );	//Тумблер ДУБЛИР
	SetGPIOOUTBitValue( Helicopter->cep.i_hidro_second || Helicopter->rsp_ep.t_check_lamp, 56 );	//Индикатор ДУБЛИР ВКЛЮЧЕНА
	Helicopter->cep.t_off_second_hidro = GetGPIOINBitValue( 159 );	//Кнопка ОТКЛ ДУБЛИР
	Helicopter->cep.t_ta_sel_false_start = GetGPIOINBitValue( 160 );	//Тумблер ПРОКРУТ bool
	Helicopter->cep.t_ta_sel_start = GetGPIOINBitValue( 161 );	//Тумблер ЗАПУСК
	Helicopter->cep.t_ta_start = GetGPIOINBitValue( 162 );	//Кнопка ЗАПУСК ТУРБОАГРЕГАТА
	Helicopter->cep.t_ta_stop = GetGPIOINBitValue( 163 );	//Кнопка ВЫКЛ ВСУ АИ-9В
	SetGPIOOUTBitValue( Helicopter->cep.i_ta_automat_on || Helicopter->rsp_ep.t_check_lamp, 57 );	//Индикатор АВТОМАТ ВКЛЮЧЕН
	SetGPIOOUTBitValue( Helicopter->cep.i_p_oil_norm || Helicopter->rsp_ep.t_check_lamp, 58 );	//Индикатор зеленый ДАВ МАСЛА НОРМ(12 на 11 слове загорается красным)
	SetGPIOOUTBitValue( Helicopter->cep.i_rpm_norm || Helicopter->rsp_ep.t_check_lamp, 59 );	//Индикатор зеленый ОБОРОТЫ НОРМА(13 на 10 слове загорается красным)
	SetGPIOOUTBitValue( Helicopter->cep.i_rpm_max || Helicopter->rsp_ep.t_check_lamp, 60 );	//Индикатор ОБОРОТЫ ПРЕДЕЛ
	SetGPIOOUTBitValue( Helicopter->cep.i_p_oil_warn || Helicopter->rsp_ep.t_check_lamp, 61 );	//Индикатор желтый ДАВ МАСЛА (12 на 11 слове загорается красным)
	SetGPIOOUTBitValue( Helicopter->cep.i_TF || Helicopter->rsp_ep.t_check_lamp, 62 );	//Индикатор желтый Топливный фильтр(13 на 10 слове загорается красным)
	SetGPIOOUTBitValue( Helicopter->cep.i_ta_brok || Helicopter->rsp_ep.t_check_lamp, 63 );	//Индикатор ВСУ отказ
	Helicopter->cep.t_eng_start = GetGPIOINBitValue( 164 );	//Кнопка ЗАПУСК ДВИГАТЕЛЕЙ
	SetGPIOOUTBitValue( Helicopter->cep.i_eng_automat_on || Helicopter->rsp_ep.t_check_lamp, 64 );	//Индикатор АВТОМАТ ВКЛЮЧЕН
	Helicopter->cep.t_eng_stop = GetGPIOINBitValue( 165 );	//Кнопка ПРЕКРАЩ ЗАПУСКА ДВИГАТЕЛЕЙ
	Helicopter->cep.t_eng_left = GetGPIOINBitValue( 166 );	//Тумблер ЗАПУСК ЛЕВ
	Helicopter->cep.t_eng_rght = GetGPIOINBitValue( 167 );	//Тумблер ЗАПУСК ПРАВ
	Helicopter->cep.t_eng_sel_start = GetGPIOINBitValue( 168 );	//Тумблер ЗАПУСК
	Helicopter->cep.t_eng_sel_scroll = GetGPIOINBitValue( 169 );	//Тумблер ПРОКРУТ
	SetGPIOOUTBitValue( Helicopter->cep.i_ta_work || Helicopter->rsp_ep.t_check_lamp, 65 );	//Индикатор СТАРТЕР РАБОТАЕТ
	Helicopter->cep.t_eng_left_ign_check = GetGPIOINBitValue( 170 );	//Тумблер ПРОВЕРКА ЗАЖИГ ЛЕВ
	Helicopter->cep.t_eng_right_ign_check = GetGPIOINBitValue( 171 );	//Тумблер ПРОВЕРКА ЗАЖИГ ПРАВ
	SetVIDSteps( 5, Helicopter->cep.f_t_ta );	//Шкала температуры (0-300,9-2636), адрес 1034(2)
	SetVIDSteps( 6, Helicopter->cep.f_p_ta );	//Шкала давления (0-84, 3-1032), адрес 1034(3)
	Helicopter->cep.t_fuel_left_close = GetGPIOINBitValue( 172 );	//Тумблер АИ-9В ПЕРЕКР ЛЕВ КРАН true=откр
	Helicopter->cep.t_fuel_right_close = GetGPIOINBitValue( 173 );	//Тумблер АИ-9В ПЕРЕКР ПРАВ КРАН true=откр
	Helicopter->cep.t_fuel_bypass_close = GetGPIOINBitValue( 174 );	//Тумблер АИ-9В ПЕРЕКР КОЛЬЦЕВ БАКОВ true=откр
	Helicopter->cep.t_fuel_perepusk = GetGPIOINBitValue( 175 );	//Тумблер ПЕРЕПУСК
	SetGPIOOUTBitValue( Helicopter->cep.i_fuel_left_close || Helicopter->rsp_ep.t_check_lamp, 66 );	//Индикатор ЛЕВЫЙ ЗАКРЫТ
	SetGPIOOUTBitValue( Helicopter->cep.i_fuel_right_close || Helicopter->rsp_ep.t_check_lamp, 67 );	//Индикатор ПРАВЫЙ ЗАКРЫТ
	SetGPIOOUTBitValue( Helicopter->cep.i_fuel_bypass_close || Helicopter->rsp_ep.t_check_lamp, 68 );	//Индикатор КОЛЬЦЕВ. ОТКЛ
	Helicopter->cep.t_fpump_refil = GetGPIOINBitValue( 176 );	//Тумблер АИ-9В  КОНТР true=низ
	Helicopter->cep.t_refil = GetGPIOINBitValue( 177 );	//Тумблер АИ-9В ЗАПРАВ  true=вверх(новая переменная)
	Helicopter->cep.t_fpump_consum = GetGPIOINBitValue( 178 );	//Тумблер АИ-9В  РАСХОД true=вверх
	Helicopter->cep.t_fpump_left = GetGPIOINBitValue( 179 );	//Тумблер АИ-9В ЛЕВЫЙ true=вверх
	Helicopter->cep.t_fpump_right = GetGPIOINBitValue( 180 );	//Тумблер АИ-9В ПРАВЫЙ true=вверх
	SetGPIOOUTBitValue( Helicopter->cep.i_fpump_consum_off || Helicopter->rsp_ep.t_check_lamp, 69 );	//Индикатор РАСХОД НЕ РАБОТ
	SetGPIOOUTBitValue( Helicopter->cep.i_fpump_left_off || Helicopter->rsp_ep.t_check_lamp, 70 );	//Индикатор ЛЕВЫЙ НЕ РАБОТ
	SetGPIOOUTBitValue( Helicopter->cep.i_fpump_right_off || Helicopter->rsp_ep.t_check_lamp, 71 );	//Индикатор ПРАВЫЙ НЕ РАБОТ
// 9 Правая Панель Электропульта. unsupported  // 
// Helicopter->rsp_elp.ark_9.f_rk_vol unsupported  // Вращалка Громкость
	Helicopter->rsp_elp.ark_9.t_rk_tlf = GetGPIOINBitValue( 181 );	//Тумблер ТЛФ  false   
	Helicopter->rsp_elp.ark_9.t_rk_tlg = GetGPIOINBitValue( 182 );	//Тумблер ТЛГ(inverse t_rk_vol)
	Helicopter->rsp_elp.ark_9.t_rk_ramka = GetGPIOINBitValue( 183 );	//Кнопка РАМКА
	Helicopter->rsp_elp.ark_9.t_rk_ctrl = GetGPIOINBitValue( 184 );	//Кнопка УПР
	Helicopter->rsp_elp.ark_9.t_rk_chanel = GetGPIOINBitValue( 185 );	//Тумблер КАНАЛ
// Helicopter->rsp_elp.ark_9.sw_rk_mode unsupported  // Галетный Переключатель
	Helicopter->rsp_elp.ark_9.t_rk_off = GetGPIOINBitValue( 186 );	//ВЫК
	Helicopter->rsp_elp.ark_9.t_rk_com = GetGPIOINBitValue( 187 );	//КОМ
	Helicopter->rsp_elp.ark_9.t_rk_ant = GetGPIOINBitValue( 188 );	//АНТ
	Helicopter->rsp_elp.ark_9.t_rk_ram = GetGPIOINBitValue( 189 );	//РАМ
// Helicopter->rsp_elp.f_t_ko50 unsupported  // Крутилка ЗАДАТЧИК Т КО-50
	Helicopter->rsp_elp.ark_ud.t_rkud_sens = GetGPIOINBitValue( 190 );	//Тумблер ЧУВСТВ
	Helicopter->rsp_elp.ark_ud.t_rkud_ukv = GetGPIOINBitValue( 191 );	//Тумблер УКВ-ДЦБ true-ДЦБ
// Helicopter->rsp_elp.ark_ud.f_rkud_vol unsupported  // Вращалка ГРОМКОСТЬ 
	SetGPIOOUTBitValue( Helicopter->rsp_elp.ark_ud.i_rkud_1 || Helicopter->rsp_ep.t_check_lamp, 72 );	//Лампа УП 
	SetGPIOOUTBitValue( Helicopter->rsp_elp.ark_ud.i_rkud_2 || Helicopter->rsp_ep.t_check_lamp, 73 );	//Лампа ШП
	SetGPIOOUTBitValue( Helicopter->rsp_elp.ark_ud.i_rkud_3 || Helicopter->rsp_ep.t_check_lamp, 74 );	//Лампа И
// Helicopter->rsp_elp.ark_ud.sw_rkud_mode unsupported  // Галетный Переключатель АРК-УД uint8 
	Helicopter->rsp_elp.ark_ud.t_rkud_off = GetGPIOINBitValue( 192 );	//0-ВЫК
	Helicopter->rsp_elp.ark_ud.t_rkud_up = GetGPIOINBitValue( 193 );	//1-УП
	Helicopter->rsp_elp.ark_ud.t_rkud_shp = GetGPIOINBitValue( 194 );	//2-ШП
	Helicopter->rsp_elp.ark_ud.t_rkud_i = GetGPIOINBitValue( 195 );	//3-И
	Helicopter->rsp_elp.ark_ud.t_rkud_rpk = GetGPIOINBitValue( 196 );	//4-РПК
// Helicopter->rsp_elp.ark_ud.sw_rkud_chanel unsupported  // Галетный переключатель КАНАЛЫ  uint8 
	Helicopter->rsp_elp.ark_ud.t_rkud_1 = GetGPIOINBitValue( 197 );	//1-канал
	Helicopter->rsp_elp.ark_ud.t_rkud_2 = GetGPIOINBitValue( 198 );	//2-канал
	Helicopter->rsp_elp.ark_ud.t_rkud_3 = GetGPIOINBitValue( 199 );	//3-канал
	Helicopter->rsp_elp.ark_ud.t_rkud_4 = GetGPIOINBitValue( 200 );	//4-канал
	Helicopter->rsp_elp.ark_ud.t_rkud_5 = GetGPIOINBitValue( 201 );	//5-канал
	Helicopter->rsp_elp.ark_ud.t_rkud_6 = GetGPIOINBitValue( 202 );	//6-канал
	Helicopter->rsp_elp.ark_ud.t_contr = GetGPIOINBitValue( 203 );	//Кнопка КОНТР
	Helicopter->rsp_elp.ark_ud.t_ant_l = GetGPIOINBitValue( 204 );	//Кнопка АНТ Л
	Helicopter->rsp_elp.ark_ud.t_ant_r = GetGPIOINBitValue( 205 );	//Кнопка АНТ П
	SetGPIOOUTBitValue( Helicopter->rsp_elp.i_ko50_heater || Helicopter->rsp_ep.t_check_lamp, 75 );	//Индикатор ПОДОГРЕВАТЕЛЬ
	SetGPIOOUTBitValue( Helicopter->rsp_elp.i_ko50_ignition || Helicopter->rsp_ep.t_check_lamp, 76 );	//Индикатор ЗАЖИГАНИЕ
	SetGPIOOUTBitValue( Helicopter->rsp_elp.i_ko50_work || Helicopter->rsp_ep.t_check_lamp, 77 );	//Индикатор КО-50 РАБОТАЕТ
	Helicopter->rsp_elp.t_ko50_ta_start = GetGPIOINBitValue( 206 );	//Кнопка ЗАПУСК ТУРБОАГРЕГАТА
	Helicopter->rsp_elp.t_ko50_auto = GetGPIOINBitValue( 207 );	//Тумблер КО50-АВТ 
	Helicopter->rsp_elp.t_ko50_man = GetGPIOINBitValue( 208 );	//Тумблер КО50-РУЧН
	Helicopter->rsp_elp.t_ko50_fill = GetGPIOINBitValue( 209 );	//Тумблер КО50-ЗАЛИВКА
	Helicopter->rsp_elp.t_ko50_midl = GetGPIOINBitValue( 210 );	//Тумблер КО50-СРЕДН РЕЖИМ
	Helicopter->rsp_elp.t_ko50_vent = GetGPIOINBitValue( 211 );	//Тумблер Вентил
	Helicopter->rsp_elp.t_N = GetGPIOINBitValue( 212 );	//Тумблер СЕВ
	Helicopter->rsp_elp.t_S = GetGPIOINBitValue( 213 );	//Тумблер ЮГ
	Helicopter->rsp_elp.gmk.t_contr300 = GetGPIOINBitValue( 214 );	//Тумблер КОНТР 300
	Helicopter->rsp_elp.gmk.t_contr0 = GetGPIOINBitValue( 215 );	//Тумблер КОНТР 0
// Helicopter->rsp_elp.f_latitude unsupported  // Вращалка ШИРОТА 0:01:00
	Helicopter->rsp_elp.t_mk = GetGPIOINBitValue( 216 );	//Тумблер МК
	//Helicopter->rsp_elp.t_mk = GetGPIOINBitValue( 217 );	//Тумблер ГК
	Helicopter->rsp_elp.t_zk_l = GetGPIOINBitValue( 218 );	//Тумблер ЗК ЛЕВ
	Helicopter->rsp_elp.t_zk_r = GetGPIOINBitValue( 219 );	//Тумблер ЗК ПРАВ
	Helicopter->rsp_elp.t_dubl_osnv = GetGPIOINBitValue( 220 );	//Тумблер ДУБЛИР 
	Helicopter->rsp_elp.t_osnv = GetGPIOINBitValue( 221 );	//Тумблер ОСНОВ(Новое добавление)
	SetGPIOOUTBitValue( Helicopter->rsp_elp.i_dubl || Helicopter->rsp_ep.t_check_lamp, 78 );	//Индикатор ДУБЛИР(Новое добавление)
	SetGPIOOUTBitValue( Helicopter->rsp_elp.i_osnv || Helicopter->rsp_ep.t_check_lamp, 79 );	//Индикатор ОСНОВ(Новое добавление)
	SetGPIOOUTBitValue( Helicopter->rsp_elp.akb_hight_temp_1 || Helicopter->rsp_ep.t_check_lamp, 195 ); //индикатор высокая температура акб 1
	SetGPIOOUTBitValue( Helicopter->rsp_elp.akb_hight_temp_2 || Helicopter->rsp_ep.t_check_lamp, 196 );	//индикатор высокая температура акб 2
// 10 Правый Щиток Электропульта. unsupported  // 
	Helicopter->rsh_elp.t_pvd_heat_contr = GetGPIOINBitValue( 222 );	//КОНТРОЛЬ ОБОГРЕВА ПВД
	SetGPIOOUTBitValue( Helicopter->rsh_elp.i_pvd_heat_ok || Helicopter->rsp_ep.t_check_lamp, 80 );	//Индикатор ОБОГРЕВ ИСПРАВЕН
	Helicopter->rsh_elp.t_diss = GetGPIOINBitValue( 223 );	//Тумблер ДИСС
	Helicopter->rsh_elp.t_rs = GetGPIOINBitValue( 224 );	//Тумблер СВЯЗНАЯ РС
	Helicopter->rsh_elp.t_gmk = GetGPIOINBitValue( 225 );	//Тумблер ГМК-1
	Helicopter->rsh_elp.t_sgu35 = GetGPIOINBitValue( 226 );	//Тумблер ВК-53 (СГУ35)
	Helicopter->rsh_elp.t_ag_right = GetGPIOINBitValue( 227 );	//Тумблер ПРАВЫЙ АГ
// Helicopter->rsh_elp.t_plafon unsupported  // Тумблер ПЛАФОН(на прямую подключен)
// Helicopter->rsh_elp.sw_wiper_mode unsupported  // Переключатель СТЕКЛООЧИСТИТЕЛЬ(юнит 3: 22-пуск,23-сброс,19-2скор,27-1скор)
	Helicopter->rsh_elp.t_rkud = GetGPIOINBitValue( 228 );	//Тумблер АРК-УД
// 11 Правая Боковая Панель Электропульта. unsupported  // 
	Helicopter->rsp_ep.t_check_lamp = GetGPIOINBitValue( 229 );	//Тумблер ПРОВЕРКА ЛАМП
	Helicopter->rsp_ep.t_main_second = GetGPIOINBitValue( 230 );	//Тумблер ОСНОВ
	Helicopter->rsp_ep.t_main_reserv = GetGPIOINBitValue( 459 );	//Тумблер РЕЗЕРВ
// Helicopter->rsp_ep.f_board unsupported  // Вращалка ТАБЛО
	Helicopter->rsp_ep.t_day = GetGPIOINBitValue( 231 );	//Тумблер ДЕНЬ-true, НОЧЬ-false
	Helicopter->rsp_ep.t_night = GetGPIOINBitValue( 232 );	//Тумблер НОЧЬ-false
// Helicopter->rsp_ep.f_gain unsupported  // Вращалка ГРОМКОСТЬ СКАН(пустышка)
// Helicopter->rsp_ep.sw_scan unsupported  // Переключатель НАБОР СКАН(пустышка)
// Helicopter->rsp_ep.t_disable unsupported  // ОТКЛ 
// Helicopter->rsp_ep.t_PrDU unsupported  // ПрДУ
// Helicopter->rsp_ep.t_ZU unsupported  // ЗУ
// Helicopter->rsp_ep.t_MSH unsupported  // МЩ
// Helicopter->rsp_ep.t_NU unsupported  // НУ
// Helicopter->rsp_ep.t_instal unsupported  // УСТАН
// Helicopter->rsp_ep.t_nabor unsupported  // НАБОР
// Helicopter->rsp_ep.t_enter unsupported  // ВВОД
	Helicopter->rsp_ep.t_light_emeg = GetGPIOINBitValue( 233 );	//Тумблер ДЕЖУРНОЕ ОСВЕЩЕНИЕ
	Helicopter->rsp_ep.t_light_generl = GetGPIOINBitValue( 234 );	//Тумблер ОБЩЕЕ ОСВЕЩЕНИЕ
	Helicopter->rsp_ep.t_ano = GetGPIOINBitValue( 235 );	//Тумблер АНО
	Helicopter->rsp_ep.t_light_drill = GetGPIOINBitValue( 236 );	//Тумблер ОГНИ СТРОЕВ
	Helicopter->rsp_ep.t_light_contur = GetGPIOINBitValue( 237 );	//Тумблер ОГНИ КОНТУР
	Helicopter->rsp_ep.t_flashlight = GetGPIOINBitValue( 238 );	//Тумблер МИГАЛКА
	Helicopter->rsp_ep.t_beacon_down = GetGPIOINBitValue( 239 );	//Тумблер МАЯК НИЗ
	Helicopter->rsp_ep.t_pzu_left = GetGPIOINBitValue( 240 );	//Тумблер ПЗУ ДВИГАТ ЛЕВ
	Helicopter->rsp_ep.t_pzu_right = GetGPIOINBitValue( 241 );	//Тумблер ПЗУ ДВИГАТ ПРАВ
	Helicopter->rsp_ep.t_pvd_heat_left = GetGPIOINBitValue( 242 );	//Тумблер ОБОГРЕВ ПВД ЛЕВ
	Helicopter->rsp_ep.t_pvd_heat_right = GetGPIOINBitValue( 243 );	//Тумблер ОБОГРЕВ ПВД ПРАВ
	Helicopter->rsp_ep.t_clock_heat = GetGPIOINBitValue( 244 );	//Тумблер ОБОГРЕВ ЧАСОВ
// Helicopter->rsp_ep.t_heat_accum unsupported  // Тумблер ОБОГРЕВ АККУМ
	Helicopter->rsp_ep.t_sublight_1 = GetGPIOINBitValue( 245 );	//Группа 1
	Helicopter->rsp_ep.t_sublight_2 = GetGPIOINBitValue( 246 );	//Группа 2
// Helicopter->rsp_ep.f_day_night unsupported  // Ночь - День
	SetGPIOOUTBitValue( Helicopter->rsp_ep.i_pzu_left_on || Helicopter->rsp_ep.t_check_lamp, 81 );	//Индикатор ЛЕВ.ПЗУ ВКЛЮЧЕН
	SetGPIOOUTBitValue( Helicopter->rsp_ep.i_pzu_right_on || Helicopter->rsp_ep.t_check_lamp, 82 );	//Индикатор ПРАВ.ПЗУ ВКЛЮЧЕН
// 12 Электрощиток электропульта. unsupported  // 
	SetVIDSteps( 7, Helicopter->ep_elp.f_amper_1 );	//Шкала ПОСТОЯННЫЙ ТОК ЛЕВ, адрес 1064(2)
	SetVIDSteps( 8, Helicopter->ep_elp.f_amper_2 );	//Шкала ПОСТОЯННЫЙ ТОК ЦЕНТР, адрес 1064(1)
	SetVIDSteps( 9, Helicopter->ep_elp.f_amper_3 );	//Шкала ПОСТОЯННЫЙ ТОК ПРАВ, адрес 1064(4)
	Helicopter->ep_elp.t_akkum_1 = GetGPIOINBitValue( 247 );	//Тумблер АККУМУЛЯТОРЫ 1
	Helicopter->ep_elp.t_akkum_2 = GetGPIOINBitValue( 248 );	//Тумблер АККУМУЛЯТОРЫ 2
// Helicopter->ep_elp.get_ctrl() unsupported  // Переключатель КОНТРОЛЬ ТОКА
	Helicopter->ep_elp.t_ctrl_vu1 = GetGPIOINBitValue( 249 );	//ВУ 1 
	Helicopter->ep_elp.t_ctrl_vu2 = GetGPIOINBitValue( 250 );	//ВУ 2
// Helicopter->ep_elp.get_dc_volt_mode() unsupported  // Переключатель ЦЕНТР
	Helicopter->ep_elp.t_dc_volt_mode_0 = GetGPIOINBitValue( 251 );	//ОТКЛ 1
	Helicopter->ep_elp.t_dc_volt_mode_1 = GetGPIOINBitValue( 252 );	//АККУМУЛЯТ 1
	Helicopter->ep_elp.t_dc_volt_mode_2 = GetGPIOINBitValue( 253 );	//АККУМУЛЯТ 2
	Helicopter->ep_elp.t_dc_volt_mode_3 = GetGPIOINBitValue( 254 );	//ВУ 1
	Helicopter->ep_elp.t_dc_volt_mode_4 = GetGPIOINBitValue( 255 );	//ВУ 2
	Helicopter->ep_elp.t_dc_volt_mode_5 = GetGPIOINBitValue( 256 );	//ШИНА ВУ КАН1
	Helicopter->ep_elp.t_dc_volt_mode_6 = GetGPIOINBitValue( 257 );	//ШИНА ВУ КАН2
	Helicopter->ep_elp.t_dc_volt_mode_7 = GetGPIOINBitValue( 258 );	//ШИНА ВС-ВСУ
	Helicopter->ep_elp.t_dc_volt_mode_8 = GetGPIOINBitValue( 259 );	//РЕЗЕРВ ГЕНЕР
	Helicopter->ep_elp.t_dc_volt_mode_9 = GetGPIOINBitValue( 260 );	//ОТКЛ 2
// Helicopter->ep_elp.f_volt_reg unsupported  // Крутилка РЕГУЛИРОВКА НАПРЯЖЕНИЯ
	Helicopter->ep_elp.t_ACDC_1 = GetGPIOINBitValue( 261 );	//Тумблер ВЫПРЯМИТЕЛИ 1
	Helicopter->ep_elp.t_ACDC_2 = GetGPIOINBitValue( 262 );	//Тумблер ВЫПРЯМИТЕЛИ 2
	Helicopter->ep_elp.t_aero_pit_dc = GetGPIOINBitValue( 263 );	//Тумблер АЭР ПИТАН (РЕЗЕРВ ГЕНЕР)
	Helicopter->ep_elp.t_test_sys = GetGPIOINBitValue( 264 );	//Тумблер ПРОВЕРКА ОБОРУД
	SetVIDSteps( 10, Helicopter->ep_elp.f_volt );	//Вольтметр, адрес 1064(3)
	SetGPIOOUTBitValue( Helicopter->ep_elp.i_vu1_brok || Helicopter->rsp_ep.t_check_lamp, 83 );	//Индикатор ВУ1 НЕ РАБОТ
	SetGPIOOUTBitValue( Helicopter->ep_elp.i_vu2_brok || Helicopter->rsp_ep.t_check_lamp, 84 );	//Индикатор ВУ2 НЕ РАБОТ
	SetGPIOOUTBitValue( Helicopter->ep_elp.i_aero_pit_dc || Helicopter->rsp_ep.t_check_lamp, 85 );	//Индикатор АЭР ПИТ 
	SetVIDSteps( 11, Helicopter->ep_elp.f_ac_amper_1 );	//Переменный ток-амперметр лев, адрес 1074(1)
	SetVIDSteps( 14, Helicopter->ep_elp.f_ac_amper_2 );	//Переменный ток-амперметр центр, адрес 1074(3)
	SetVIDSteps( 13, Helicopter->ep_elp.f_ac_volt );	//Переменный ток-вольтметр, адрес 1074(4)
	SetVIDSteps( 12, Helicopter->ep_elp.f_ac_amper_3 );	//Переменный ток-амперметр прав, адрес 1074(2)
	SetGPIOOUTBitValue( Helicopter->ep_elp.i_gen_1_off || Helicopter->rsp_ep.t_check_lamp, 86 );	//Индикатор ГЕН. 1 ОТКЛ
	SetGPIOOUTBitValue( Helicopter->ep_elp.i_gen_2_off || Helicopter->rsp_ep.t_check_lamp, 87 );	//Индикатор ГЕН. 2 ОТКЛ
	SetGPIOOUTBitValue( Helicopter->ep_elp.t_res_gen || Helicopter->rsp_ep.t_check_lamp, 88 );	//Индикатор ГЕН ВСУ РАБОТ
	Helicopter->ep_elp.t_gen_1 = GetGPIOINBitValue( 265 );	//Тумблер ГЕНЕРАТОРЫ 1
	Helicopter->ep_elp.t_gen_2 = GetGPIOINBitValue( 266 );	//Тумблер ГЕНЕРАТОРЫ 2
	Helicopter->ep_elp.t_gen_1_contr = GetGPIOINBitValue( 461 );	//Тумблер ГЕНЕРАТОРЫ 1 Контроль напряжения
	Helicopter->ep_elp.t_gen_2_contr = GetGPIOINBitValue( 462 );	//Тумблер ГЕНЕРАТОРЫ 2 Контроль напряжения
	Helicopter->ep_elp.t_res_gen = GetGPIOINBitValue( 267 );	//Тумблер ВСУ?
	Helicopter->ep_elp.t_r36_auto = GetGPIOINBitValue( 268 );	//Тумблер ТР-Р36В АВТОМАТ
	Helicopter->ep_elp.t_r36_man = GetGPIOINBitValue( 269 );	//Тумблер ТР-Р36В РУЧНОЕ
	Helicopter->ep_elp.t_pts_auto = GetGPIOINBitValue( 270 );	//Тумблер ПТС АВТОМАТ
	Helicopter->ep_elp.t_pts_man = GetGPIOINBitValue( 271 );	//Тумблер ПТС РУЧНОЕ
	Helicopter->ep_elp.t_aero_pit_ac = GetGPIOINBitValue( 272 );	//Тумблер АЭРОД ПИТАН
	Helicopter->ep_elp.t_gen = GetGPIOINBitValue( 273 );	//Тумблер ПСТ АЭР ПИТАН(10-низ)
	Helicopter->ep_elp.t_gen_pts_contr = GetGPIOINBitValue(505); //Тумблер ПСТ АЭР ПИТАН(3-вверх)
// Helicopter->ep_elp.get_ac_volt_mode() unsupported  // Переключатель КОНТРОЛЬ НАПРЯЖЕНИЯ
	Helicopter->ep_elp.t_ac_volt_mode_pts_gen1a = GetGPIOINBitValue( 274 );	//ПТС ГЕНЕРАТОР А
	Helicopter->ep_elp.t_ac_volt_mode_pts_gen1b = GetGPIOINBitValue( 275 );	//ПТС ГЕНЕРАТОР В
	Helicopter->ep_elp.t_ac_volt_mode_pts_gen1c = GetGPIOINBitValue( 276 );	//ПТС ГЕНЕРАТОР С
	Helicopter->ep_elp.t_ac_volt_mode_aer_gen1a = GetGPIOINBitValue( 277 );	//АЗС ПИТАН ГЕНЕРАТОР 2 А
	Helicopter->ep_elp.t_ac_volt_mode_aer_gen1b = GetGPIOINBitValue( 278 );	//АЗС ПИТАН ГЕНЕРАТОР 2 В
	Helicopter->ep_elp.t_ac_volt_mode_aer_gen1c = GetGPIOINBitValue( 279 );	//АЗС ПИТАН ГЕНЕРАТОР 2 С
	SetGPIOOUTBitValue( Helicopter->ep_elp.i_reser36 || Helicopter->rsp_ep.t_check_lamp, 89 );	//Индикатор -38В РЕЗЕРВ
	SetGPIOOUTBitValue( Helicopter->ep_elp.i_ptc_on || Helicopter->rsp_ep.t_check_lamp, 90 );	//Индикатор ПТ ВКЛЮЧЕН
	SetGPIOOUTBitValue( Helicopter->ep_elp.i_resev_on || Helicopter->rsp_ep.t_check_lamp, 91 );	//Индикатор РЕЗЕРВ ЛИНИЯ ВКЛ
	SetGPIOOUTBitValue( Helicopter->ep_elp.i_rap_on || Helicopter->rsp_ep.t_check_lamp, 92 );	//Индикатор АЭР ПИТ ВКЛЮЧ
// 13 Левая Приборная Панель. unsupported  // 
// Helicopter->lpb.t_rulejka unsupported  // Тумблер РУЛЕЖ(нет в вертолете)
	Helicopter->lpb.t_svet_ubran = GetGPIOINBitValue( 280 );	//Тумблер СВЕТ Убрана
	Helicopter->lpb.t_svet = GetGPIOINBitValue( 281 );	//Тумблер СВЕТ Свет
	Helicopter->lpb.t_fpp = GetGPIOINBitValue( 282 );	//Тумблер УПРАВЛ ФПП
	Helicopter->lpb.t_project = GetGPIOINBitValue( 283 );	//Тумблер УПРАВЛ ПРОЖЕКТ
	Helicopter->lpb.t_dnmr = GetGPIOINBitValue(503);        // тумблер НАВ/ДНМР
// Helicopter->lpb.sw_PVD unsupported  // Переключатель СТАТИЧЕСКАЯ СИСТЕМА ПВД 0-левая, 1-объединенная, 2-правая
	Helicopter->lpb.t_pvd_left = GetGPIOINBitValue( 284 );	//ПВД ЛЕВАЯ
	Helicopter->lpb.t_pvd_unitd = GetGPIOINBitValue( 285 );	//ПВД ОБЪЕДИНЕННАЯ
	Helicopter->lpb.t_pvd_right = GetGPIOINBitValue( 286 );	//ПВЖ ПРАВАЯ
	Helicopter->lpb.t_roll_33 = GetGPIOINBitValue( 287 );	//
	SetVIDSteps( 15, Helicopter->lpb.rv_5.alt( ) );	//ВЫСОТОМЕР-Стрелка 1134(2) 0=40, 100=1910, 700=3670
	Helicopter->lpb.rv_5.t_test = GetGPIOINBitValue( 288 );	//ВЫСОТОМЕР-Кнопка ТЕСТ
	Helicopter->lpb.rv_5.encoder = GetENCValue(0);	//ВЫСОТОМЕР-Крутилка индекса (600,)
	SetGPIOOUTBitValue(Helicopter->lpb.rv_5.t_triangle || Helicopter->rsp_ep.t_check_lamp, 184 );// Лампа в крутилке индексом (Новое добавление)
// Helicopter->lpb.rv_5.down unsupported  // 
	SetVIDSteps( 16, Helicopter->lpb.rv_5.index );	//Высотомер-индекс 1134(1)
	SetVIDSteps( 17, Helicopter->lpb.rv_5.blank );	//Высотомер-бленкер 1134(3)
	Helicopter->lpb.t_apk_sv = GetGPIOINBitValue( 289 );	//Тумблер АРК АРК СВ
	Helicopter->lpb.t_apk_ukv = GetGPIOINBitValue( 290 );	//Тумблер АРК АРК УКВ
	SetVIDSteps( 18, Helicopter->lpb.ut_6k.get_temp_1()/*Helicopter->lpb.ut_6k.temp1 */);	//Термометр-стрелка-ЛЕВ ВЕРХ  244(2)
	SetVIDSteps( 19, Helicopter->lpb.ut_6k.get_temp_2()/*Helicopter->lpb.ut_6k.temp2*/ );	//Термометр-стрелка-ПРАВ ВЕРХ 244(3)
	SetCLOCKSteps( 0, Helicopter->lpb.ut_6k.get_t100_1()/* Helicopter->lpb.ut_6k.get_temp1_fine() */ );	//Термометр-стрелка-ЛЕВ НИЗ 254(3)  (0=160)
	SetCLOCKSteps( 1, Helicopter->lpb.ut_6k.get_t100_2()/* Helicopter->lpb.ut_6k.get_temp2_fine()  */);	//Термометр-стрелка-ПРАВ НИЗ 254(4) (0=123)
	SetVIDSteps( 20, Helicopter->lpb.uc_450k.get_speed() );	//Спидометр-стрелка 1144(2) 40=0 150=935
	SetVIDSteps( 21, Helicopter->lpb.vd_10vk.get_km() );	//Высотометр (КМ)-стрелка 1144(1) 0km=0 7km=3078
	SetCLOCKSteps( 2, Helicopter->lpb.vd_10vk.get_m()  );	//Высотометр (М)-стрелка 1154(4) 0=2035
// Helicopter->lpb.vd_10vk.p unsupported  // Высотометр (М)-давление 0:10:00
	Helicopter->lpb.vd_10vk.f_barometr = 671.f + MABNormalValue(14);  // Высотометр (М)-крутилуа 0:10:00
	SetVIDSteps( 22, Helicopter->lpb.up_21_15.fosh );	//Шаг винта стрелка 1:15:00  234(1) (1-296,15-2633)
	SetVIDSteps( 23, Helicopter->lpb.ir_117.index );	//ИЗМЕРИТЕЛЬ РЕЖИМОВ центр 234(2)
	SetVIDSteps( 24, Helicopter->lpb.ir_117.eng1 );	//ИЗМЕРИТЕЛЬ РЕЖИМОВ-Левый указатель 234(4)
	SetVIDSteps( 25, Helicopter->lpb.ir_117.eng2 );	//ИЗМЕРИТЕЛЬ РЕЖИМОВ-Правый указатель 234(3)
	SetVIDSteps( 26, Helicopter->lpb.avg_left.roll );	//АВИАГОРИЗОНТ ЛЕВ-крен 0  крен-roll 1214(2)
	SetVIDSteps( 27, Helicopter->lpb.avg_left.getPitch() );	//АВИАГОРИЗОНТ ЛЕВ-тангаж 0 тангаж 1204(4)
	SetVIDSteps( 28, Helicopter->lpb.avg_left.yawl );	//АВИАГОРИЗОНТ ЛЕВ-рысканье 964(4) 0  крен-roll 
// Helicopter->lpb.avg_left.index unsupported  // АВИАГОРИЗОНТ ЛЕВ-индекс 0,125 стрелка на ободе
// Helicopter->lpb.avg_left.shift_pitch unsupported  // АВИАГОРИЗОНТ ЛЕВ-центровка шкалы тангажа крутилка со стрелками
	SetVIDSteps( 29, Helicopter->lpb.avg_left.blank );	//АВИАГОРИЗОНТ ЛЕВ:Флажок отс. Питания 1214(1)
// Helicopter->lpb.avg_left.alignment unsupported  // АВИАГОРИЗОНТ крутилка юстировка
	Helicopter->lpb.avg_left.encoder = GetENCValue(1);	//
// Helicopter->lpb.ugr_4uk.dir() unsupported  // УГР - текущий курс
	SetCLOCKStepsUGR( 3, Helicopter->lpb.ugr_4uk.dir()  );	//ПНП-текущий курс  284(3)  0=4224
// Helicopter->lpb.ugr_4uk.d1 unsupported  // УГР - тослстая стрелка
	SetCLOCKSteps( 4, Helicopter->lpb.ugr_4uk.ark()  );	//УГР - тонкая стрелка  284(4)  0=1604
// Helicopter->lpb.ugr_4uk.corrector unsupported  // УГР - вращалка юстировки
	SetGPIOOUTBitValue( Helicopter->lpb.ss.i_ss || Helicopter->rsp_ep.t_check_lamp, 93 );	//Индикатор МС - лампа ВЫК
	SetVIDSteps( 30, Helicopter->lpb.ss.speed_x );	//Индикатор МС-вперёд 934(3)
	SetVIDSteps( 31, Helicopter->lpb.ss.speed_x_down );	//Индикатор МС-назад 934(4)
	SetVIDSteps( 32, Helicopter->lpb.ss.speed_z_left );	//Индикатор МС-влево 944(2)
	SetVIDSteps( 33, Helicopter->lpb.ss.speed_z );	//Индикатор МС-вправо 944(3)
	SetVIDSteps( 34, Helicopter->lpb.ss.speed_y );	//Индикатор МС-вверх-вниз 934(1)
	SetVIDSteps( 35, Helicopter->lpb.vr_30mk.var );	//Подъем/спуск стрелка 1144(3) +25=90  0=2015 -25=3925
	SetVIDSteps( 36, Helicopter->lpb.ite_1t.nv );	//Несущий винт обороты - стрелка  934(2)
	SetVIDSteps( 37, Helicopter->lpb.avg_res.roll );	//АВИАГОРИЗОНТ ПРАВ - ROLL  1224(1)
	SetVIDSteps( 38, Helicopter->lpb.avg_res.getPitch() );	//АВИАГОРИЗОНТ ПРАВ - PITCH  1214(3)
	SetVIDSteps( 39, Helicopter->lpb.avg_res.yawl );	//АВИАГОРИЗОНТ ПРАВ - YAWL 964(2)
// Helicopter->lpb.avg_res.index unsupported  // АВИАГОРИЗОНТ ПРАВ - индекс 0,125 стрелка на ободе
// Helicopter->lpb.avg_res.shift_pitch unsupported  // АВИАГОРИЗОНТ ПРАВ-центровка шкалы тангажа крутилка со стрелками
	SetVIDSteps( 40, Helicopter->lpb.avg_res.blank );	//АВИАГОРИЗОНТ ПРАВ : Флажок отс. Питания  1214(4)
// Helicopter->lpb.avg_res.alignment unsupported  // АВИАГОРИЗОНТ ПРАВ : Крутилка юстировки
	Helicopter->lpb.avg_res.encoder = GetENCValue(2);	//
	SetVIDSteps( 41, Helicopter->lpb.ite_2t.eng1 );	//Двигатели 1 обороты - стрелка (нет значений)  964(3) 
	SetVIDSteps( 42, Helicopter->lpb.ite_2t.eng2 );	//Двигатели 2 обороты - стрелка (нет значений) 964(1)
	SetGPIOOUTBitValue( Helicopter->lpb.i_warning_earth || Helicopter->rsp_ep.t_check_lamp, 94 );	//Индикатор ВНИМЕНИЕ ЗЕМЛЯ
	SetGPIOOUTBitValue( Helicopter->lpb.i_danger_earth || Helicopter->rsp_ep.t_check_lamp, 95 );	//Индикатор ОПАСНО ЗЕМЛЯ
	SetGPIOOUTBitValue( Helicopter->lpb.i_roll_max || Helicopter->rsp_ep.t_check_lamp, 96 );	//Индикатор КРЕН ВЕЛИК
	Helicopter->lpb.t_cso1_yellow = GetGPIOINBitValue( 291 );	//Кнопка ЦСО1 желтый
	Helicopter->lpb.t_cso1_red = GetGPIOINBitValue( 292 );	//Кнопка ЦСО1 красный   
	SetGPIOOUTBitValue( Helicopter->lpb.i_cso1_yellow || Helicopter->rsp_ep.t_check_lamp, 97 );	//Индикатор ЦСО1 желтый
	SetGPIOOUTBitValue( Helicopter->lpb.i_cso1_red || Helicopter->rsp_ep.t_check_lamp, 98 );	//Индикатор ЦСО1 красный   
	SetGPIOOUTBitValue( Helicopter->lpb.i_ag_net_kontr || Helicopter->rsp_ep.t_check_lamp, 99 );	//Индикатор НЕТ КОНТР АГ
	SetGPIOOUTBitValue( Helicopter->lpb.i_ag_left_brok || Helicopter->rsp_ep.t_check_lamp, 100 );	//Индикатор ОТКАЗ АГ ЛЕВ
	SetGPIOOUTBitValue( Helicopter->lpb.i_ag_res_brok || Helicopter->rsp_ep.t_check_lamp, 101 );	//Индикатор ОТКАЗ АГ РЕЗЕРВ
	SetGPIOOUTBitValue( Helicopter->lpb.i_main_gear_swarf || Helicopter->rsp_ep.t_check_lamp, 102 );	//Индикатор СТРУЖКА ГЛ. РЕДУК.
	SetGPIOOUTBitValue( Helicopter->lpb.i_inter_gear_swarf || Helicopter->rsp_ep.t_check_lamp, 103 );	//Индикатор СТРУЖКА ПРОМ. РЕД.
	SetGPIOOUTBitValue( Helicopter->lpb.i_tail_gear_swarf || Helicopter->rsp_ep.t_check_lamp, 104 );	//Индикатор СТРУЖКА ХВ. РЕДУК.
	SetGPIOOUTBitValue( Helicopter->lpb.i_eng_left_vibr_dang || Helicopter->rsp_ep.t_check_lamp, 105 );	//Индикатор ВИБР. ОПАС. ЛЕВ. ДВИГ.
	SetGPIOOUTBitValue( Helicopter->lpb.i_eng_left_swarf || Helicopter->rsp_ep.t_check_lamp, 106 );	//Индикатор СТРУЖКА ЛЕВ.ДВИГ.
	SetGPIOOUTBitValue( Helicopter->lpb.i_eng_left_p_oil || Helicopter->rsp_ep.t_check_lamp, 107 );	//Индикатор МАЛО. РМ. ЛЕВ. ДВИГ.
	SetGPIOOUTBitValue( Helicopter->lpb.i_eng_left_emrg || Helicopter->rsp_ep.t_check_lamp, 108 );	//Индикатор Ч.Р.ЛЕВ.ДВИГ.
	SetGPIOOUTBitValue( Helicopter->lpb.i_nv_max || Helicopter->rsp_ep.t_check_lamp, 109 );	//Индикатор п.Н.В ВЫСОКИЕ
	SetGPIOOUTBitValue( Helicopter->lpb.i_eng_left_tf || Helicopter->rsp_ep.t_check_lamp, 110 );	//Индикатор ЗАСОР. ТФ. ЛЕВ. ДВИГ.
	SetGPIOOUTBitValue( Helicopter->lpb.i_eng_left_vibr_max || Helicopter->rsp_ep.t_check_lamp, 111 );	//Индикатор ВИБР. ПОВ. ЛЕВ. ДВИГ.
	SetGPIOOUTBitValue( Helicopter->lpb.i_eng_left_max_rpm || Helicopter->rsp_ep.t_check_lamp, 112 );	//Индикатор Превышение оборотов свободной турбины левого двигателя.
	SetGPIOOUTBitValue( Helicopter->lpb.i_eng_left_erd_off || Helicopter->rsp_ep.t_check_lamp, 113 );	//Индикатор ОТКЛ. ЭРД. ЛЕВ. ДВИГ.
	SetGPIOOUTBitValue( Helicopter->lpb.i_nv_min || Helicopter->rsp_ep.t_check_lamp, 114 );	//Индикатор п.Н.В НИЗКИЕ
	SetGPIOOUTBitValue( Helicopter->lpb.i_eng_right_vibr_dang || Helicopter->rsp_ep.t_check_lamp, 115 );	//Индикатор ВИБР ОПАС ПРАВ ДВИГ
	SetGPIOOUTBitValue( Helicopter->lpb.i_eng_right_swarf || Helicopter->rsp_ep.t_check_lamp, 116 );	//Индикатор СТРУЖКА ПРАВ.ДВИГ.
	SetGPIOOUTBitValue( Helicopter->lpb.i_eng_right_p_oil || Helicopter->rsp_ep.t_check_lamp, 117 );	//Индикатор МАЛО РМ. ПРАВ. ДВИГ.
	SetGPIOOUTBitValue( Helicopter->lpb.i_eng_right_emrg || Helicopter->rsp_ep.t_check_lamp, 118 );	//Индикатор Ч.Р.ПРАВ.ДВИГ.
	SetGPIOOUTBitValue( Helicopter->lpb.i_main_gear_p_oil || Helicopter->rsp_ep.t_check_lamp, 119 );	//Индикатор МАЛО РМ ГЛ РЕДУК
	SetGPIOOUTBitValue( Helicopter->lpb.i_eng_right_tf || Helicopter->rsp_ep.t_check_lamp, 120 );	//Индикатор ЗАСОР. ТФ. ПРАВ. ДВИГ.
	SetGPIOOUTBitValue( Helicopter->lpb.i_eng_right_vibr_max || Helicopter->rsp_ep.t_check_lamp, 121 );	//Индикатор ВИБР. ПОВ. ПРАВ. ДВИГ.
	SetGPIOOUTBitValue( Helicopter->lpb.i_eng_right_max_rpm || Helicopter->rsp_ep.t_check_lamp, 122 );	//Индикатор Превышение оборотов свободной турбины правого двигателя
	SetGPIOOUTBitValue( Helicopter->lpb.i_eng_right_erd_off || Helicopter->rsp_ep.t_check_lamp, 123 );	//Индикатор ОТКЛ. ЭРД. ПРАВ. ДВИГ.
	SetGPIOOUTBitValue( Helicopter->lpb.i_eng_left_limit || Helicopter->rsp_ep.t_check_lamp, 124 );	//Индикатор ограничения режима левого двигателя
	SetGPIOOUTBitValue( Helicopter->lpb.i_eng_right_limit || Helicopter->rsp_ep.t_check_lamp, 125 );	//Индикатор ограничения режима правого двигателя
// Helicopter->lpb.ki_206.course() unsupported  // KI206 подвижная шкала(компаса) добавить управление желтыми треугольниками
	Helicopter->lpb.ki_206.set_radial( MABNormalValue( 0 ) );	//Ручка задатчика 313(4) N=1371
	Helicopter->rpb.ki_206.set_radial(MABNormalValue(0));	//Ручка задатчика 313(4) N=1371
	SetVIDSteps( 43, Helicopter->lpb.ki_206.i_from_to );	//НАВИГАТОР Индикатор FROM - TO  1234(3) TO=3492 FROM=118
	SetVIDSteps( 44, Helicopter->lpb.ki_206.i_gs );	//Навигатор флажок GS  1244(1) 1=1450 0=3107
	SetVIDSteps( 45, Helicopter->lpb.ki_206.i_nav );	//НАВИГАТОР Флажок NAV  1244(2) 1=1438 0=2767
	SetVIDSteps( 46, Helicopter->lpb.ki_206.dir );	//НАВИГАТОР - линия вертикали 1234(1) 0=2164 1373-2936
	SetVIDSteps( 47, Helicopter->lpb.ki_206.glis );	//НАВИГАТОР - линия горизонтали  1234(2) 0=2159  1275-3164
// Helicopter->lpb.ki_206.roll unsupported  // крен
// 14 Центральный Пульт. unsupported  // 
	SetVIDSteps( 48, Helicopter->ccp.f_t_oil_inter_gb );	//Шкала T МАСЛА ПРОМ. РЕД  914(1) (-50-207,150-2130)
	SetVIDSteps( 49, Helicopter->ccp.f_t_oil_tail_gb );	//Шкала T МАСЛА ХВОСТ. РЕД  904(2) -50=165 150=1738
	SetVIDSteps( 50, Helicopter->ccp.f_p_oil_main_gb );	//Шкала P МАСЛА ГЛ. РЕДУК. 914(4) (0-202,8-1798)
	SetVIDSteps( 51, Helicopter->ccp.f_t_oil_main_gb );	//Шкала T МАСЛА ГЛ. РЕДУК. 904(1) -50=243 150=1571
	SetVIDSteps( 52, Helicopter->ccp.f_p_oil_left_eng );	//Шкала ЛЕВЫЙ ДВИГАТЕЛЬ ДАВЛЕНИЕ  914(2) (0=226,8=1883)
	SetVIDSteps( 53, Helicopter->ccp.f_t_oil_left_eng );	//Шкала ЛЕВЫЙ ДВИГАТЕЛЬ T МАСЛА  914(3)(-50-157,150-1740)
	SetVIDSteps( 54, Helicopter->ccp.f_p_oil_rght_eng );	//Шкала ПРАВЫЙ ДВИГАТЕЛЬ ДАВЛЕНИЕ  904(3) (0-289,8-1911)
	SetVIDSteps( 55, Helicopter->ccp.f_t_oil_rght_eng );	//Шкала ПРАВЫЙ ДВИГАТЕЛЬ T МАСЛА  904(4)  (-50=195, 150=1786)
	Helicopter->ccp.t_ap_speed = GetGPIOINBitValue( 293 );	//Тумблер СТАБ V
	SetGPIOOUTBitValue( Helicopter->ccp.i_ap_speed || Helicopter->rsp_ep.t_check_lamp, 126 );	//Индикатор СТАБ V
//  unsupported  // Вращалка ПУЛЬТ управления БУРТ-2Ж
// Helicopter->ccp.t_zbn_on unsupported  // Тумблер ЗБН ВКЛ
	Helicopter->ccp.t_bur_on = GetGPIOINBitValue( 294 );	//Тумблер БУР ВКЛ
	Helicopter->ccp.t_kontr_lamp = GetGPIOINBitValue( 295 );	//Кнопка КОНТР ЛАМП(КОНТР ИНД)
	SetGPIOOUTBitValue( Helicopter->ccp.i_otkaz_bur || Helicopter->ccp.t_kontr_lamp || Helicopter->rsp_ep.t_check_lamp, 127 );	//Индикатор ОТКАЗ БУР-1
	SetGPIOOUTBitValue( Helicopter->ccp.i_otkaz_en || Helicopter->ccp.t_kontr_lamp || Helicopter->rsp_ep.t_check_lamp, 128 );	//Индикатор ОТКАЗ ЭН
// Helicopter->ccp.t_day_night unsupported  // Тумблер ДЕНЬ-НОЧЬ
// Helicopter->ccp.t_kontr_vvoda unsupported  // Кнопка КОНТР ВВОДА
	Helicopter->ccp.t_vvod = GetGPIOINBitValue( 296 );	//Кнопка ВВОД
// Helicopter->ccp.t_sdvig unsupported  // Кнопка СДВИГ
	Helicopter->ccp.t_cbros = GetGPIOINBitValue( 297 );	//Кнопка СБРОС
	Helicopter->ccp.t_ust = GetGPIOINBitValue( 298 );	//Кнопка УСТ
	Helicopter->ccp.t_od = GetGPIOINBitValue( 299 );	//Кнопка ОД
	SetLEDINDICATORValue(Helicopter->ccp.led_bur, 3);	//БУР ЛЕД
// Helicopter->ccp.f_1 unsupported  // Лампочка
// Helicopter->ccp.f_2 unsupported  // Лампочка
// Helicopter->ccp.f_3 unsupported  // Лампочка
	SetVIDSteps( 56, Helicopter->ccp.f_get_curs );	//Планка НАПРАВЛЕНИЕ 204(1) 10-410-830
	SetVIDSteps( 57, Helicopter->ccp.f_get_roll );	//Планка КРЕН  214(3) 380-890-1385 45
	SetVIDSteps( 58, Helicopter->ccp.f_get_pitch );	//Планка ТАНГАЖ  204(4)  30-390-710
	SetVIDSteps( 59, Helicopter->ccp.f_get_alt );	//Планка ВЫСОТА 204(2) 70-400-730
	SetVIDSteps( 60, Helicopter->ccp.f_spu52_ind );	//Планка СПУ52 204(3) 110-320-710
	Helicopter->ccp.t_spu52_off = GetGPIOINBitValue( 300 );	//Кнопка КОНТРОЛЬ ОТКЛ(out 5-17)
	SetGPIOOUTBitValue( Helicopter->ccp.i_spu52_off || Helicopter->ccp.t_spu52_off || Helicopter->rsp_ep.t_check_lamp, 129 );	//Индикатор КОНТРОЛЬ ОТКЛ(out 5-17)
	Helicopter->ccp.f_spu52 = POTNormalValue( 0 );	//Вращалка КОНТРОЛЬ 0:01:00
	Helicopter->ccp.t_spu52_p = GetGPIOINBitValue( 301 );	//Тумблер КОНТРОЛЬ P
	Helicopter->ccp.t_spu52_t = GetGPIOINBitValue( 302 );	//Тумблер КОНТРОЛЬ t
	Helicopter->ccp.t_sublight_ki_13 = GetGPIOINBitValue( 303 );	//Тумблер ПОДСВЕТ КИ-13
	Helicopter->ccp.t_sublight_azs = GetGPIOINBitValue( 304 );	//Тумблер ПОДСВЕТ ПАНЕЛИ АЗС
//  unsupported  // Тумблер ИК
//  unsupported  // Тумблер НАСТРОЙКА
	Helicopter->ccp.f_set_curs = MABNormalValue( 1 ) /*- 5.*/;	//АВТОПИЛОТ - Вращалка НАПРАВЛЕНИЕ  323(3)
	Helicopter->ccp.f_set_roll = MABNormalValue( 2 ) /*- 5.*/;	//АВТОПИЛОТ - Вращалка КРЕН  323(2)
	Helicopter->ccp.f_set_pitch = MABNormalValue( 3 )/* - 5.*/;	//АВТОПИЛОТ - Вращалка ТАНГАЖ 323(1)
	Helicopter->ccp.t_set_curs = GetGPIOINBitValue( 305 );	//АВТОПИЛОТ - кнопка на  НАПРАВЛЕНИЕ  323(3)
	Helicopter->ccp.t_set_roll = GetGPIOINBitValue( 306 );	//АВТОПИЛОТ - кнопка на КРЕН  323(2)
	Helicopter->ccp.t_set_pitch = GetGPIOINBitValue( 307 );	//АВТОПИЛОТ - кнопка ТАНГАЖ 323(1)
	Helicopter->ccp.t_control = GetGPIOINBitValue( 308 );	//Контроль в панели под крышкой
	SetGPIOOUTBitValue( Helicopter->ccp.i_ap_dir_off || Helicopter->ccp.t_ap_dir_off || Helicopter->rsp_ep.t_check_lamp, 130 );	//АВТОПИЛОТ - Лампочка НАПРАВЛЕНИЕ ОТКЛ
	SetGPIOOUTBitValue( Helicopter->ccp.i_ap_dir_on || Helicopter->rsp_ep.t_check_lamp, 131 );	//АВТОПИЛОТ - Лампочка НАПРАВЛЕНИЕ ВКЛ
	SetGPIOOUTBitValue( Helicopter->ccp.i_ap_on || Helicopter->rsp_ep.t_check_lamp, 132 );	//АВТОПИЛОТ - Лампочка средняя ВКЛ
	SetGPIOOUTBitValue( Helicopter->ccp.i_ap_alt_on || Helicopter->rsp_ep.t_check_lamp, 133 );	//Лампочка ВЫСОТА ВКЛ
	SetGPIOOUTBitValue( Helicopter->ccp.i_ap_alt_off || Helicopter->ccp.t_ap_alt_off || Helicopter->rsp_ep.t_check_lamp, 134 );	//Лампочка ВЫСОТА ОТКЛ
	Helicopter->ccp.t_ap_dir_off = GetGPIOINBitValue( 309 );	//АВТОПИЛОТ - Кнопка НАПРАВЛЕНИЕ ОТКЛ
	Helicopter->ccp.t_ap_dir_on = GetGPIOINBitValue( 310 );	//АВТОПИЛОТ - Кнопка НАПРАВЛЕНИЕ ВКЛ
	Helicopter->ccp.t_ap_on = GetGPIOINBitValue( 311 );	//АВТОПИЛОТ - Кнопка средняя ВКЛ
	Helicopter->ccp.t_ap_alt_on = GetGPIOINBitValue( 312 );	//АВТОПИЛОТ - Кнопка ВЫСОТА ВКЛ
	Helicopter->ccp.t_ap_alt_off = GetGPIOINBitValue( 313 );	//АВТОПИЛОТ - Кнопка ВЫСОТА ОТКЛ
	Helicopter->ccp.t_main_off_ab = GetGPIOINBitValue( 314 );	//Тумблер ГЛАВЫЙ ВЫКЛ АБ
	Helicopter->ccp.t_emerg_drop = GetGPIOINBitValue( 315 );	//Тумблер АВРИЙНЫЙ СБРОС
	Helicopter->ccp.t_explosion = GetGPIOINBitValue( 316 );	//Тумблер ВЗРЫВ
	Helicopter->ccp.t_po1 = GetGPIOINBitValue( 317 );	//Тумблер ПО1
	Helicopter->ccp.t_po2 = GetGPIOINBitValue( 318 );	//Тумблер ПО2
	SetGPIOOUTBitValue( Helicopter->ccp.i_ab || Helicopter->rsp_ep.t_check_lamp, 135 );	//Лампа АБ
	SetGPIOOUTBitValue( Helicopter->ccp.i_explosion || Helicopter->rsp_ep.t_check_lamp, 136 );	//Лампа ВЗРЫВ
	Helicopter->ccp.t_cargo_cabin = GetGPIOINBitValue( 319 );	//Тумблер ГРУЗ КАБИНА
	Helicopter->ccp.t_crew = GetGPIOINBitValue( 320 );	//Тумблер ЭКИПАЖ
	Helicopter->ccp.t_day = GetGPIOINBitValue( 321 );	//Тумблер ДЕНЬ
	Helicopter->ccp.t_night = GetGPIOINBitValue( 322 );	//Тумблер НОЧЬ
	SetGPIOOUTBitValue( Helicopter->ccp.i_power || Helicopter->rsp_ep.t_check_lamp, 137 );	//Лампа ПИТАНИЕ
	SetGPIOOUTBitValue( Helicopter->ccp.i_brace || Helicopter->rsp_ep.t_check_lamp, 138 );	//Лампа РАСЧАЛКА
	SetGPIOOUTBitValue( Helicopter->ccp.i_removed || Helicopter->rsp_ep.t_check_lamp, 139 );	//Лампа УБРАНА
	SetGPIOOUTBitValue( Helicopter->ccp.i_work || Helicopter->rsp_ep.t_check_lamp, 140 );	//Лампа РАБОТА
	SetGPIOOUTBitValue( Helicopter->ccp.i_dropped || Helicopter->rsp_ep.t_check_lamp, 141 );	//Лампа ВЫПУЩЕНА
	SetGPIOOUTBitValue( Helicopter->ccp.i_EL || Helicopter->rsp_ep.t_check_lamp || Helicopter->ccp.t_control, 142 );	//EL
	SetGPIOOUTBitValue( Helicopter->ccp.i_NO || Helicopter->rsp_ep.t_check_lamp || Helicopter->ccp.t_control, 143 );	//NO
	SetGPIOOUTBitValue( Helicopter->ccp.i_VT || Helicopter->rsp_ep.t_check_lamp || Helicopter->ccp.t_control, 144 );	//VT
	SetGPIOOUTBitValue( Helicopter->ccp.i_PO || Helicopter->rsp_ep.t_check_lamp || Helicopter->ccp.t_control, 145 );	//PO
	SetGPIOOUTBitValue( Helicopter->ccp.i_TZ || Helicopter->rsp_ep.t_check_lamp || Helicopter->ccp.t_control, 146 );	//TZ
	SetGPIOOUTBitValue( Helicopter->ccp.i_TO || Helicopter->rsp_ep.t_check_lamp || Helicopter->ccp.t_control, 147 );	//TO
	SetGPIOOUTBitValue( Helicopter->ccp.i_SG || Helicopter->rsp_ep.t_check_lamp || Helicopter->ccp.t_control, 148 );	//SG
	SetGPIOOUTBitValue( Helicopter->ccp.i_OV || Helicopter->rsp_ep.t_check_lamp || Helicopter->ccp.t_control, 149 );	//OV
	SetGPIOOUTBitValue( Helicopter->ccp.i_ZV || Helicopter->rsp_ep.t_check_lamp || Helicopter->ccp.t_control, 150 );	//ZV
	Helicopter->ccp.t_ap_alt_up = GetGPIOINBitValue( 323 );	//АВТОПИЛОТ - Кнопка КОНТРОЛЬ ВЫСОТЫ вверх
	Helicopter->ccp.t_ap_alt_down = GetGPIOINBitValue( 324 );	//АВТОПИЛОТ - Кнопка КОНТРОЛЬ ВЫСОТЫ вниз
	//KT76
	SetLEDINDICATORValue(Helicopter->ccp.KT76_LCD, 6);
	Helicopter->ccp.KT76_t_IDT = GetGPIOINBitValue( 441 );
	Helicopter->ccp.KT76_t_0 = GetGPIOINBitValue( 442 );
	Helicopter->ccp.KT76_t_1 = GetGPIOINBitValue( 443 );
	Helicopter->ccp.KT76_t_2 = GetGPIOINBitValue( 444 );
	Helicopter->ccp.KT76_t_3 = GetGPIOINBitValue( 445 );
	Helicopter->ccp.KT76_t_4 = GetGPIOINBitValue( 446 );
	Helicopter->ccp.KT76_t_5 = GetGPIOINBitValue( 447 );
	Helicopter->ccp.KT76_t_6 = GetGPIOINBitValue( 448 );
	Helicopter->ccp.KT76_t_7 = GetGPIOINBitValue( 449 );
	Helicopter->ccp.KT76_t_CLR = GetGPIOINBitValue( 450 );
	Helicopter->ccp.KT76_t_VFR = GetGPIOINBitValue( 451 );
	Helicopter->ccp.KT76_gal_OFF = GetGPIOINBitValue( 452 );
	Helicopter->ccp.KT76_gal_SBY = GetGPIOINBitValue( 453 );
	Helicopter->ccp.KT76_gal_TST = GetGPIOINBitValue( 454 );
	Helicopter->ccp.KT76_gal_ON  = GetGPIOINBitValue( 455 );
	Helicopter->ccp.KT76_gal_ALT = GetGPIOINBitValue( 456 );
	SetGPIOOUTBitValue( Helicopter->ccp.KT76_i_FL, 179);
	SetGPIOOUTBitValue( Helicopter->ccp.KT76_i_ALT, 180);
	SetGPIOOUTBitValue( Helicopter->ccp.KT76_i_ON, 181);
	SetGPIOOUTBitValue( Helicopter->ccp.KT76_i_R , 182);
	SetGPIOOUTBitValue( Helicopter->ccp.KT76_i_SBY, 183);
	//KN53
	SetLEDINDICATORValue(Helicopter->ccp.KN53_LCD_16, 7);
	SetLEDINDICATORValue(Helicopter->ccp.KN53_LCD, 8);
	Helicopter->ccp.KN53_str = GetGPIOINBitValue(457);
	Helicopter->ccp.KN53_on = GetGPIOINBitValue(458);
	Helicopter->ccp.KN53_freqM = GetENCValue(4);  // енкодер переключения МГц частот
	Helicopter->ccp.KN53_freqK = GetENCValue(5);  // енкодер переключения КГц частот
	Helicopter->ccp.KN53_pozuv = GetGPIOINBitValue(469); // прослушивание позывных на КН53
// 15 Правая Приборная Панель. unsupported  // 
	Helicopter->rpb.t_dnmr = GetGPIOINBitValue(504);        // тумблер НАВ/ДНМР
	Helicopter->rpb.t_fara_close = GetGPIOINBitValue( 325 );	//Тумблер СВЕТ Убрана
	Helicopter->rpb.t_fara_on = GetGPIOINBitValue( 326 );	//Тумблер СВЕТ Свет
// Helicopter->rpb.t_projector unsupported  // Тумблер УПРАВЛ Свет
	SetVIDSteps( 61, Helicopter->rpb.f_fuel );	//ТОПЛИВОМЕР  0 964(4)
	SetGPIOOUTBitValue( Helicopter->rpb.i_fuel_270 || Helicopter->rsp_ep.t_check_lamp, 151 );	//Индикатор ОСТАЛОСЬ 270 Л
	SetGPIOOUTBitValue( Helicopter->rpb.i_dptb_left || Helicopter->rsp_ep.t_check_lamp, 152 );	//Индикатор ДПТБ ПУСТ ЛЕВ
	SetGPIOOUTBitValue( Helicopter->rpb.i_dptb_right || Helicopter->rsp_ep.t_check_lamp, 153 );	//Индикатор ДПТБ ПУСТ ПРАВ
// Helicopter->rpb.get_sw_fuel() unsupported  // Переключатель топливомера(ОДИН ЛИШНИЙ?)
	Helicopter->rpb.t_fuel_0 = GetGPIOINBitValue( 327 );	//0 - ВЫКЛ
	Helicopter->rpb.t_fuel_1 = GetGPIOINBitValue( 328 );	//1 - СУММА - суммарный запас топлива
	Helicopter->rpb.t_fuel_2 = GetGPIOINBitValue( 329 );	//2 - Пл - левый подвесной
	Helicopter->rpb.t_fuel_3 = GetGPIOINBitValue( 330 );	//3 - Ппр - правый подвесной
	Helicopter->rpb.t_fuel_4 = GetGPIOINBitValue( 331 );	//4 - РАСХ. - расходный
	Helicopter->rpb.t_fuel_5 = GetGPIOINBitValue( 332 );	//5 - Д - правый дополнительный
// Helicopter->rpb.get_dop_fuel() unsupported  // Переключатель доп баков
	Helicopter->rpb.t_fuel_low_left = GetGPIOINBitValue( 333 );	//ВНУТР ЛЕВ
	Helicopter->rpb.t_fuel_low_right = GetGPIOINBitValue( 334 );	//ВНУТР ПРАВ
	Helicopter->rpb.t_fuel_hi_left = GetGPIOINBitValue( 335 );	//ВНЕШ ЛЕВ
	Helicopter->rpb.t_fuel_hi_right = GetGPIOINBitValue( 336 );	//ВНЕШ ПРАВ
// Helicopter->rpb.kdi_572.sw_switch unsupported  // Переключатель KDI-572 -OFF(новое добавление индикаторов)
	Helicopter->rpb.kdi_572.t_N1 = GetGPIOINBitValue( 337 );	//N1
	Helicopter->rpb.kdi_572.t_HLD = GetGPIOINBitValue( 338 );	//HLD
	Helicopter->rpb.kdi_572.t_N2 = GetGPIOINBitValue( 339 );	//N2
	SetGPIOOUTBitValue( Helicopter->rpb.kdi_572.i_N1 || Helicopter->rsp_ep.t_check_lamp, 154 );	//Индикатор N1
	SetGPIOOUTBitValue( Helicopter->rpb.kdi_572.i_H || Helicopter->rsp_ep.t_check_lamp, 155 );	//Индикатор H
	SetGPIOOUTBitValue( Helicopter->rpb.kdi_572.i_N2 || Helicopter->rsp_ep.t_check_lamp, 156 );	//Индикатор N2
// Helicopter->rpb.kdi_572.i_HM unsupported  // Индикатор HM
// Helicopter->rpb.kdi_572.i_RNV unsupported  // Индикатор RNV
// Helicopter->rpb.kdi_572.i_KT unsupported  // Индикатор KT
// Helicopter->rpb.kdi_572.i_MIN unsupported  // Индикатор MIN
	SetLEDINDICATORValue( Helicopter->rpb.kdi_572.led, 1 );	//комбинированный индикатор
// Helicopter->rpb.kdi_572.f_1 unsupported  // Индикатор KDI-572 1-скорость
// Helicopter->rpb.kdi_572.f_2 unsupported  // Индикатор KDI-572 2-растояние
// Helicopter->rpb.kdi_572.f_3 unsupported  // Индикатор KDI-572 3-время
	Helicopter->rpb.t_alarm = GetGPIOINBitValue( 340 );	//Тумблер СИГНАЛИЗАЦИЯ
	SetVIDSteps( 62, Helicopter->rpb.uc_450k.get_speed() );	//Спидометр-стрелка p1 914(2) 0=104
	SetVIDSteps( 63, Helicopter->rpb.vd_10vk.get_km() );	//Высотометр (КМ)-стрелка 984(3) 0=2053
	SetCLOCKSteps( 5, Helicopter->rpb.vd_10vk.get_m()  );	//Высотометр (М)-стрелка  984(4) 0=2424
// Helicopter->rpb.vd_10vk.p unsupported  // Высотометр давление 670:790
	Helicopter->rpb.vd_10vk.f_barometr = 670.f + MABNormalValue(15);  // Высотометр (М)-крутилуа 0:10:00
// Helicopter->rpb.vd_10vk.corrector unsupported  // 
	SetVIDSteps( 64, Helicopter->rpb.avg.roll );	//АВИАГОРИЗОНТ ПРАВ-КРЕН 0p 1204(3)  0=2001
	SetVIDSteps( 65, Helicopter->rpb.avg.getPitch() );	//АВИАГОРИЗОНТ ПРАВ-ТАНГАЖ 0p 1204(1)
	SetVIDSteps( 66, Helicopter->rpb.avg.yawl );	//АВИАГОРИЗОНТ ПРАВ-РЫСКАНИЕ 1p 904(1) 0=399
// Helicopter->rpb.avg.index unsupported  // АВИАГОРИЗОНТ ПРАВ-индекс :180
// Helicopter->rpb.avg.shift_pitch unsupported  // АВИАГОРИЗОНТ ПРАВ-центровка шкалы тангажа-180:180
	SetVIDSteps( 67, Helicopter->rpb.avg.blank );	//АВИАГОРИЗОНТ ПРАВ:Флажок отс. Питания 0p 1204(2)
// Helicopter->rpb.avg.alignment unsupported  // АВИАГОРИЗОНТ ПРАВ крутилка юстировки
	Helicopter->rpb.avg.encoder = GetENCValue(3);	//600 7-8 
// Helicopter->rpb.avg.up unsupported  // 
// Helicopter->rpb.avg.down unsupported  // 
// Helicopter->rpb.ugr_4uk.dir() unsupported  // УГР - Текущий курс
	SetCLOCKStepsUGR( 6, Helicopter->rpb.ugr_4uk.dir()  );	//ПНП-текущий курс  954(3)  0=
// Helicopter->rpb.ugr_4uk.d1 unsupported  // УГР - Толстая стрелка
	SetCLOCKSteps( 7, Helicopter->rpb.ugr_4uk.ark()  );	//УГР - Тонкая стрелка  p1 284(3)  0=
// Helicopter->rpb.ugr_4uk.f_set_course unsupported  // УГР - Центровка
// Helicopter->rpb.ugr_4uk.corrector unsupported  // 
	SetVIDSteps( 68, Helicopter->rpb.snos.angle );	//УГОЛ СНОСА 964(1) (0 середина — 2144)  -40=3462,40875
// Helicopter->rpb.snos.speed unsupported  // DISS Спидометр — км/час
	SetGPIOOUTBitValue( Helicopter->rpb.snos.i_angle || Helicopter->rsp_ep.t_check_lamp, 157 );	//Угол сноса 
	Helicopter->rpb.snos.t_pk = GetGPIOINBitValue( 341 );	//УГОЛ СНОСА - Переключатель Р-К true-К 
	Helicopter->rpb.snos.t_cm = GetGPIOINBitValue( 342 );	//УГОЛ СНОСА - Переключатель С-М true = М 
	SetVIDSteps( 69, Helicopter->rpb.vr_30mk.var );	//Подъем/спуск стрелка  974(2) 0=104
	SetVIDSteps( 70, Helicopter->rpb.ite_1t.nv );	//Несущий винт обороты - стрелка 904(2) (0=0, 105=4320)
	SetVIDSteps( 71, Helicopter->rpb.ite_2t.eng1 );	//Двигатели 1 обороты - стрелка 904(4) 0=745
	SetVIDSteps( 72, Helicopter->rpb.ite_2t.eng2 );	//Двигатели 2 обороты - стрелка 914(3) 0=4120
	Helicopter->rpb.diss_15.t_rght = GetGPIOINBitValue( 343 );	//Кнопка ВПР
	Helicopter->rpb.diss_15.t_left = GetGPIOINBitValue( 344 );	//Кнопка ВЛ
// Helicopter->rpb.diss_15.sw_side unsupported  // БОКОВОЕ УКЛОНЕНИЕ ВЛЕВО-ВПРАВО
// Helicopter->rpb.diss_15.get_side() unsupported  // Боковое уклонение-значение
	Helicopter->rpb.diss_15.t_v = GetGPIOINBitValue( 345 );	//Кнопка В
	Helicopter->rpb.diss_15.t_n = GetGPIOINBitValue( 346 );	//Кнопка Н
// Helicopter->rpb.diss_15.get_way() unsupported  // Путь-значение   TODO : исправить ДИСС !!!
// Helicopter->rpb.diss_15.sw_way unsupported  // ПУТЬ ВПЕРЕД-НАЗАД
	Helicopter->rpb.diss_15.t_plus = GetGPIOINBitValue( 347 );	//Кнопка +
	Helicopter->rpb.diss_15.t_minus = GetGPIOINBitValue( 348 );	//Кнопка -
// Helicopter->rpb.diss_15.f_angle unsupported  // Угол карты-значение TODO кажется тут не совпадают типы
	Helicopter->rpb.diss_15.t_on = GetGPIOINBitValue( 349 );	//Кнопка ВКЛ
	Helicopter->rpb.diss_15.t_off = GetGPIOINBitValue( 350 );	//Кнопка ОТКЛ
// Helicopter->rpb.diss_15.i_on unsupported  // ДИСС - лампа ВКЛЮЧЕНО
// !Helicopter->rpb.diss_15.i_on unsupported  // ДИСС - лампа ОТКЛ (инверсия от ВКЛ)
	SetGPIOOUTBitValue( Helicopter->rpb.i_diss_brok || Helicopter->rsp_ep.t_check_lamp, 158 );	//Индикатор ДИСС ОТКАЗ
	SetGPIOOUTBitValue( Helicopter->rpb.i_ag_brok || Helicopter->rsp_ep.t_check_lamp, 159 );	//Индикатор ОТКАЗ АГ ПРАВ
	SetGPIOOUTBitValue( Helicopter->rpb.i_warning_earth || Helicopter->rsp_ep.t_check_lamp, 160 );	//Индикатор ВНИМЕНИЕ ЗЕМЛЯ
	SetGPIOOUTBitValue( Helicopter->rpb.i_danger_earth || Helicopter->rsp_ep.t_check_lamp, 161 );	//Индикатор ОПАСНО ЗЕМЛЯ
	SetGPIOOUTBitValue( Helicopter->rpb.i_pitch_max || Helicopter->rsp_ep.t_check_lamp, 162 );	//Индикатор ТАНГАЖ ВЕЛИК
	SetGPIOOUTBitValue( Helicopter->rpb.i_roll_max || Helicopter->rsp_ep.t_check_lamp, 163 );	//Индикатор КРЕН ВЕЛИК bool IP->Par022
// Helicopter->rpb.ki_206.course() unsupported  // KI206 - текущий курс
	SetVIDSteps( 73, Helicopter->rpb.ki_206.i_from_to );	//KI206 Индикатор FROM TO 1 1264(3) fr=1082, to=3224
	SetVIDSteps( 74, Helicopter->rpb.ki_206.i_gs );	//KI206 флажок GS
	SetVIDSteps( 75, Helicopter->rpb.ki_206.i_nav );	//KI206 Флажок NAV
	SetVIDSteps( 76, Helicopter->rpb.ki_206.dir );	//KI206 - линия вертикали 1 1264(1) 1373-2936
	SetVIDSteps( 77, Helicopter->rpb.ki_206.glis );	//KI206 - линия горизонтали 1 1264(2) 1275-3164
	SetVIDSteps( 78, Helicopter->rpb.f_temp );	//Температура в пассажирском отсеке 904(3) -60=207,602338
// 16 Центральный пульт(наклонная часть). unsupported  // 
// Helicopter->cpc.f_kt74_left unsupported  // Вращалка КТ-74 левая
// Helicopter->cpc.t_msg unsupported  // Кнопка MSG
// Helicopter->cpc.t_obs unsupported  // Кнопка OBS
// Helicopter->cpc.t_alt unsupported  // Кнопка ALT
// Helicopter->cpc.t_nrst unsupported  // Кнопка NPRST
// Helicopter->cpc.t_d unsupported  // Кнопка D
// Helicopter->cpc.t_clr unsupported  // Кнопка CLR
// Helicopter->cpc.t_ent unsupported  // Кнопка ENT
// Helicopter->cpc.f_kt74_right unsupported  // Вращалка КТ-74 правая
// Helicopter->cpc.i_kn62_use unsupported  // Индикатор KN-62 USE
// Helicopter->cpc.i_kn62_stby unsupported  // Индикатор KN-62 STBY
// Helicopter->cpc.t_kt74_ident unsupported  // KT-74 Кнопка IDENT
// Helicopter->cpc.t_kt74_vfr unsupported  // KT-74 Кнопка VFR
// Helicopter->cpc.t_kt74_0 unsupported  // KT-74 Кнопка 0
// Helicopter->cpc.t_kt74_1 unsupported  // KT-74 Кнопка 1
// Helicopter->cpc.t_kt74_2 unsupported  // KT-74 Кнопка 2
// Helicopter->cpc.t_kt74_3 unsupported  // KT-74 Кнопка 3
// Helicopter->cpc.t_kt74_4 unsupported  // KT-74 Кнопка 4
// Helicopter->cpc.t_kt74_5 unsupported  // KT-74 Кнопка 5
// Helicopter->cpc.t_kt74_6 unsupported  // KT-74 Кнопка 6
// Helicopter->cpc.t_kt74_7 unsupported  // KT-74 Кнопка 7
// Helicopter->cpc.t_kt74_ent unsupported  // KT-74 Кнопка ENT
// Helicopter->cpc.t_kt74_func unsupported  // KT-74 Кнопка FUNC
// Helicopter->cpc.f_kt74 unsupported  // КТ-74 Вращалка
// Helicopter->cpc.sw_kn62a unsupported  // KN-62A ВЕРХНИЙ ПЕРЕКЛЮЧАТЕЛЬ
// Helicopter->cpc.t_kn62a_off unsupported  // KN-62A ВЫКЛЮЧАТЕЛЬ
// Helicopter->cpc.f_kn62 unsupported  // KN-62A Вращалка
// 17 Рычаги и педали. unsupported  // 
	Helicopter->PC.f_roll = MABNormalValue( 4 );	//Штурвал ВЛЕВО-ВПРАВО 353(1) 1569-645  расчет 691-1491
	Helicopter->PC.f_pitch = MABNormalValue( 5 );	//Штурвал ВПЕРЕД-НАЗАД 353(3) 2803-3432  расчет 2596-3438
// Helicopter->PC.t_right_spu1 unsupported  // Лев штурвал - Кнопка СПУ РАДИО(напрямую)
// Helicopter->PC.t_left_trim unsupported  // Лев штурвал - Кнопка ЭЛЕКТРОМАГНИТНЫЙ ТОРМОЗ(напрямую)
	Helicopter->PC.t_left_fire = GetGPIOINBitValue( 351 );	//Лев штурвал - Левая красная кнопка
	Helicopter->PC.t_left_ap_off = GetGPIOINBitValue( 352 );	//Лев штурвал - Кнопка ОТКЛЮЧЕНИЕ АВТОПИЛОТА
// Helicopter->PC.t_right_spu2 unsupported  // Прав штурвал -Кнопка СПУ РАДИО 0-ВЫКЛ, 1-КАБИНА, 2- ДИСПЕТЧЕР(напрямую)
// Helicopter->PC.t_rght_centr unsupported  // Прав штурвал - Кнопка ЭЛЕКТРОМАГНИТНЫЙ ТОРМОЗ(напрямую)
	Helicopter->PC.t_right_fire = GetGPIOINBitValue( 353 );	//Прав штурвал-Левая красная кнопка
	Helicopter->PC.t_right_ap_off = GetGPIOINBitValue( 354 );	//Прав штурвал - Кнопка ОТКЛЮЧЕНИЕ АВТОПИЛОТА
// Helicopter->PC.t_fric unsupported  // Барашек фрикциона 0:1,0-от себя, 1-на себя
	Helicopter->PC.t_fpp_l_up = GetGPIOINBitValue( 355 );	//Кнопка управления фарой вверх
	Helicopter->PC.t_fpp_l_down = GetGPIOINBitValue( 356 );	//Кнопка управления фарой вниз
	Helicopter->PC.t_fpp_l_left = GetGPIOINBitValue( 357 );	//Кнопка управления фарой влево
	Helicopter->PC.t_fpp_l_right = GetGPIOINBitValue( 358 );	//Кнопка управления фарой вправо
	Helicopter->PC.t_avar_drop = GetGPIOINBitValue( 359 );	//Кнопка Аварийного сброса груза
	Helicopter->PC.t_tact_drop = GetGPIOINBitValue( 360 );	//Кнопка тактического сброса груза
	Helicopter->PC.t_fr_l_fosh = GetGPIOINBitValue( 437 );  // Кнопка фрикциона
	Helicopter->PC.t_rpm_up = GetGPIOINBitValue( 361 );	//Тумблер
	Helicopter->PC.t_rpm_down = GetGPIOINBitValue( 362 );	//Тумблер
	Helicopter->PC.t_rpm_right_up = GetGPIOINBitValue( 363 );	//Тумблер
	Helicopter->PC.t_rpm_right_down = GetGPIOINBitValue( 364 );	//Тумблер
	Helicopter->PC.t_fpp_r_up = GetGPIOINBitValue( 365 );	//Кнопка управления фарой вверх
	Helicopter->PC.t_fpp_r_down = GetGPIOINBitValue( 366 );	//Кнопка управления фарой вниз 
	Helicopter->PC.t_fpp_r_left = GetGPIOINBitValue( 367 );	//Кнопка управления фарой влево
	Helicopter->PC.t_fpp_r_right = GetGPIOINBitValue( 368 );	//Кнопка управления фарой вправо
	Helicopter->PC.t_tact_drop = GetGPIOINBitValue( 369 );	//Кнопка тактического сброса груза
// Helicopter->PC.t_fr_l_fosh unsupported  // Кнопка фрикциона bool она одна на всех
	Helicopter->PC.f_eng1 = MABNormalValue( 6 );	//Руд левый  343(3) 1180-230 1697-225
	Helicopter->PC.f_eng2 = MABNormalValue( 7 );	//Руд правый 343(4) 2627-962  2058-1233
	Helicopter->PC.f_Fosh = MABNormalValue( 8 );	//Рукоятка шаг-газ 343(1) 1698-962
	Helicopter->PC.f_Ecor = MABNormalValue( 9 );	//Ручка коррекции 353(1) Б=3450 М=3784
	Helicopter->PC.f_yawl = MABNormalValue( 10 );	//Педали Лев Пилот 343(2)  23-1053
	Helicopter->PC.f_break = MABNormalValue( 11 );	//Тормоз колес 323(4)  1496-2424
	Helicopter->PC.t_NV_brake_on = GetGPIOINBitValue( 370 );	//Рычаг тормоза винта
	Helicopter->PC.t_NV_brake_off = GetGPIOINBitValue( 371 );	//Рычаг тормоза винта
// Helicopter->PC.f_nv_brake unsupported  // Рычаг тормоза винта кнопка
	Helicopter->PC.t_eng_rght_brake = GetGPIOINBitValue( 372 );	//Остановка двигателя ПРАВ
	Helicopter->PC.t_eng_left_brake = GetGPIOINBitValue( 373 );	//Остановка двигателя ЛЕВ
	Helicopter->PC.t_safety_lock = GetGPIOINBitValue( 374 );	//Предохранительный выключатель вооружения
	SetGPIOOUTBitValue( Helicopter->PC.i_weapon_warning || Helicopter->rsp_ep.t_check_lamp, 164 );	//Лампа ОСТОРОЖНО ЦЕПИ ВООРУЖЕНИЯ ПОД ТОКОМ
	Helicopter->PC.t_bomb_drop_left = GetGPIOINBitValue( 375 );	//Кнопка сброса бомб
	Helicopter->PC.t_bomb_drop_right = GetGPIOINBitValue( 376 );	//Кнопка сброса бомб
	SetCLOCKSteps( 8, Helicopter->PC.f_compass  );	//Магнитный компас
	SetGPIOOUTBitValue( Helicopter->PC.reset, 165 );	//Сброс питания
	SetGPIOOUTBitValue( Helicopter->PC.vibro, 166 );	//Включение вибромотора 0-1-0
	Helicopter->PC.t_ap_dir_sopr = GetGPIOINBitValue( 377 ) || GetGPIOINBitValue(438) || GetGPIOINBitValue(439) || GetGPIOINBitValue(440);	//блокировка автопилота на педалях
	SetVIDSteps( 79, Helicopter->PC.f_vibro );	//Задание частоты
	Helicopter->PC.pkt_fire = GetGPIOINBitValue( 378 );	//стрельба из пулемета
	Helicopter->PC.pkt_load = GetGPIOINBitValue( 379 );	//взвод
	Helicopter->PC.pkt_x = MABNormalValue( 12 );	//наводка пулемета по горизонтели
	Helicopter->PC.pkt_y = MABNormalValue( 13 );	//наводка пулемета по вертикали
// 18 Щиток вооружения командира. unsupported  // 
// Helicopter->_BoardSystem->get_sw_weapon_type() unsupported  // Галетный переключатель
// Helicopter->WeaponPult.t_MV unsupported  // MV
	Helicopter->WeaponPult.t_PKT = GetGPIOINBitValue( 380 );	//PKT
	Helicopter->WeaponPult.t_FAB = GetGPIOINBitValue( 381 );	//FAB
	Helicopter->WeaponPult.t_NAR = GetGPIOINBitValue( 382 );	//NAR
	Helicopter->WeaponPult.t_UPK23 = GetGPIOINBitValue( 383 );	//UPK23
// Helicopter->WeaponPult.t_AGS17 unsupported  // AGS17
	Helicopter->WeaponPult.t_fire_short = GetGPIOINBitValue( 384 );	//Тумблер ОЧЕРЕДЬ МРС КОРОТКАЯ
// Helicopter->WeaponPult.t_fire_long unsupported  // Тумблер ОЧЕРЕДЬ МРС ДЛИННАЯ(среднее положение тумблера)
	Helicopter->WeaponPult.t_fire_midle = GetGPIOINBitValue( 385 );	//Тумблер ОЧЕРЕДЬ МРС СРЕДНЯЯ
	Helicopter->WeaponPult.t_pod_ext = GetGPIOINBitValue( 386 );	//Тумблер ПОДВЕСКА ВНЕШН
// Helicopter->WeaponPult.t_pod_all unsupported  // Тумблер ПОДВЕСКА ВСЕ(среднее положение тумблера)
	Helicopter->WeaponPult.t_pod_inter = GetGPIOINBitValue( 387 );	//Тумблер ПОДВЕСКА 
// 19 Пульт Контроля ДИСС. unsupported  // 
	SetGPIOOUTBitValue( Helicopter->pc_diss.i_m || Helicopter->rsp_ep.t_check_lamp, 167 );	//Индикатор М
	SetGPIOOUTBitValue( Helicopter->pc_diss.i_contr || Helicopter->rsp_ep.t_check_lamp, 168 );	//Индикатор КОНТР
	SetGPIOOUTBitValue( Helicopter->pc_diss.i_work || Helicopter->rsp_ep.t_check_lamp, 169 );	//Индикатор РАБОТА
	SetGPIOOUTBitValue( Helicopter->pc_diss.i_b || Helicopter->rsp_ep.t_check_lamp, 170 );	//Индикатор В
// Helicopter->pc_diss.sw_mode unsupported  // Галетный Переключатель
	Helicopter->pc_diss.t_p1 = GetGPIOINBitValue( 388 );	//
	Helicopter->pc_diss.t_p2 = GetGPIOINBitValue( 389 );	//
	Helicopter->pc_diss.t_p3 = GetGPIOINBitValue( 390 );	//
	Helicopter->pc_diss.t_p4 = GetGPIOINBitValue( 391 );	//
	Helicopter->pc_diss.t_p5 = GetGPIOINBitValue( 392 );	//
	/*********ДИСС-32**********/
	Helicopter->pc_diss.t_contr = GetGPIOINBitValue( 463 );
	Helicopter->pc_diss.t_poisk = GetGPIOINBitValue( 464 );
	Helicopter->pc_diss.t_up_left = GetGPIOINBitValue(465);
	Helicopter->pc_diss.t_back_right = GetGPIOINBitValue(466);
	Helicopter->pc_diss.t_spedd127 = GetGPIOINBitValue( 467 );
	Helicopter->pc_diss.t_speed258 = GetGPIOINBitValue( 468 );
	SetGPIOOUTBitValue( Helicopter->pc_diss.i_vss || Helicopter->rsp_ep.t_check_lamp, 188);
	SetGPIOOUTBitValue( Helicopter->pc_diss.i_vch || Helicopter->rsp_ep.t_check_lamp, 189 );
	SetGPIOOUTBitValue( Helicopter->pc_diss.i_bvk || Helicopter->rsp_ep.t_check_lamp, 190 );
	SetGPIOOUTBitValue( Helicopter->pc_diss.i_flight || Helicopter->rsp_ep.t_check_lamp, 191);
	SetGPIOOUTBitValue( Helicopter->pc_diss.i_ispr || Helicopter->rsp_ep.t_check_lamp, 192);
	SetGPIOOUTBitValue( Helicopter->pc_diss.i_search || Helicopter->rsp_ep.t_check_lamp, 193);
	SetGPIOOUTBitValue(Helicopter->pc_diss.t_contr_flag || Helicopter->rsp_ep.t_check_lamp, 197);
	SetGPIOOUTBitValue(Helicopter->pc_diss.t_poisk_flag || Helicopter->rsp_ep.t_check_lamp, 198);
	SetGPIOOUTBitValue(Helicopter->pc_diss.t_up_left ||  Helicopter->rsp_ep.t_check_lamp, 199);
	SetGPIOOUTBitValue(Helicopter->pc_diss.t_back_right || Helicopter->rsp_ep.t_check_lamp, 200);
	SetGPIOOUTBitValue(Helicopter->pc_diss.t_spedd127 || Helicopter->rsp_ep.t_check_lamp, 201);
	SetGPIOOUTBitValue(Helicopter->pc_diss.t_speed258 || Helicopter->rsp_ep.t_check_lamp, 202);
// 20 Щиток электросрабатывателя. unsupported  // 
	SetGPIOOUTBitValue( Helicopter->WeaponPult.i_bd_1_load || Helicopter->rsp_ep.t_check_lamp, 171 );	//Индикатор БД1 ЗАГРУЖ
	SetGPIOOUTBitValue( Helicopter->WeaponPult.i_bd_2_load || Helicopter->rsp_ep.t_check_lamp, 172 );	//Индикатор БД2 ЗАГРУЖ
	SetGPIOOUTBitValue( Helicopter->WeaponPult.i_bd_3_load || Helicopter->rsp_ep.t_check_lamp, 173 );	//Индикатор БД3 ЗАГРУЖ
	SetGPIOOUTBitValue( Helicopter->WeaponPult.i_bd_4_load || Helicopter->rsp_ep.t_check_lamp, 174 );	//Индикатор БД4 ЗАГРУЖ
	SetGPIOOUTBitValue( Helicopter->WeaponPult.i_vzriv || Helicopter->rsp_ep.t_check_lamp, 175 );	//Индикатор АВАР ВЗРЫВ
	SetGPIOOUTBitValue( Helicopter->WeaponPult.i_weapon_line || Helicopter->rsp_ep.t_check_lamp, 176 );	//Индикатор СЕТЬ БВ ВКЛЮЧ
	Helicopter->WeaponPult.t_pus_load_PO1 = GetGPIOINBitValue( 393 );	//Тумблер ПО1
	Helicopter->WeaponPult.t_pus_load_PO2 = GetGPIOINBitValue( 394 );	//Тумблер ПО2(новая переменная)
	Helicopter->WeaponPult.t_main_switch = GetGPIOINBitValue( 395 );	//Тумблер ГЛАВНЫЙ ВЫКЛЮЧ
	Helicopter->WeaponPult.t_vzriv = GetGPIOINBitValue( 396 );	//Тумблер ВЗРЫВ
	Helicopter->WeaponPult.t_sbros = GetGPIOINBitValue( 397 );	//Тумблер СБРОС БОМБ
// Х АРК селектор частоты. unsupported  // 
	Helicopter->rsp_elp.ark_9.freq_selector1.t_uni[0] = GetGPIOINBitValue( 398 );	//
	Helicopter->rsp_elp.ark_9.freq_selector1.t_uni[1] = GetGPIOINBitValue( 399 );	//
	Helicopter->rsp_elp.ark_9.freq_selector1.t_uni[2] = GetGPIOINBitValue( 400 );	//
	Helicopter->rsp_elp.ark_9.freq_selector1.t_uni[3] = GetGPIOINBitValue( 401 );	//
	Helicopter->rsp_elp.ark_9.freq_selector1.t_uni[4] = GetGPIOINBitValue( 402 );	//
	Helicopter->rsp_elp.ark_9.freq_selector1.t_dec[0] = GetGPIOINBitValue( 403 );	//
	Helicopter->rsp_elp.ark_9.freq_selector1.t_dec[1] = GetGPIOINBitValue( 404 );	//
	Helicopter->rsp_elp.ark_9.freq_selector1.t_dec[2] = GetGPIOINBitValue( 405 );	//
	Helicopter->rsp_elp.ark_9.freq_selector1.t_dec[3] = GetGPIOINBitValue( 406 );	//
	Helicopter->rsp_elp.ark_9.freq_selector1.t_dec[4] = GetGPIOINBitValue( 407 );	//
	Helicopter->rsp_elp.ark_9.freq_selector1.t_hndr[0] = GetGPIOINBitValue( 408 );	//
	Helicopter->rsp_elp.ark_9.freq_selector1.t_hndr[1] = GetGPIOINBitValue( 409 );	//
	Helicopter->rsp_elp.ark_9.freq_selector1.t_hndr[2] = GetGPIOINBitValue( 410 );	//
	Helicopter->rsp_elp.ark_9.freq_selector1.t_hndr[3] = GetGPIOINBitValue( 411 );	//
	Helicopter->rsp_elp.ark_9.freq_selector1.t_hndr[4] = GetGPIOINBitValue( 412 );	//
	Helicopter->rsp_elp.ark_9.freq_selector2.t_uni[0] = GetGPIOINBitValue( 413 );	//
	Helicopter->rsp_elp.ark_9.freq_selector2.t_uni[1] = GetGPIOINBitValue( 414 );	//
	Helicopter->rsp_elp.ark_9.freq_selector2.t_uni[2] = GetGPIOINBitValue( 415 );	//
	Helicopter->rsp_elp.ark_9.freq_selector2.t_uni[3] = GetGPIOINBitValue( 416 );	//
	Helicopter->rsp_elp.ark_9.freq_selector2.t_uni[4] = GetGPIOINBitValue( 417 );	//
	Helicopter->rsp_elp.ark_9.freq_selector2.t_dec[0] = GetGPIOINBitValue( 418 );	//
	Helicopter->rsp_elp.ark_9.freq_selector2.t_dec[1] = GetGPIOINBitValue( 419 );	//
	Helicopter->rsp_elp.ark_9.freq_selector2.t_dec[2] = GetGPIOINBitValue( 420 );	//
	Helicopter->rsp_elp.ark_9.freq_selector2.t_dec[3] = GetGPIOINBitValue( 421 );	//
	Helicopter->rsp_elp.ark_9.freq_selector2.t_dec[4] = GetGPIOINBitValue( 422 );	//
	Helicopter->rsp_elp.ark_9.freq_selector2.t_hndr[0] = GetGPIOINBitValue(423);	//
	Helicopter->rsp_elp.ark_9.freq_selector2.t_hndr[1] = GetGPIOINBitValue( 424 );	//
	Helicopter->rsp_elp.ark_9.freq_selector2.t_hndr[2] = GetGPIOINBitValue( 425 );	//
	Helicopter->rsp_elp.ark_9.freq_selector2.t_hndr[3] = GetGPIOINBitValue( 426 );	//
	Helicopter->rsp_elp.ark_9.freq_selector2.t_hndr[4] = GetGPIOINBitValue( 427 );	//
// Пульт управления ловушками. unsupported  // 
// Helicopter->UV26.tablo unsupported  // 
	SetGPIOOUTBitValue( Helicopter->UV26.i_left || Helicopter->rsp_ep.t_check_lamp, 177 );	//
	SetGPIOOUTBitValue( Helicopter->UV26.i_right || Helicopter->rsp_ep.t_check_lamp, 178 );	//
	Helicopter->UV26.t_left = GetGPIOINBitValue( 428 );	//
	Helicopter->UV26.t_right = GetGPIOINBitValue( 429 );	//
	Helicopter->UV26.t_prog = GetGPIOINBitValue( 430 );	//
	Helicopter->UV26.t_seria = GetGPIOINBitValue( 431 );	//
	Helicopter->UV26.t_zalp = GetGPIOINBitValue( 432 );	//
	Helicopter->UV26.t_stop = GetGPIOINBitValue( 433 ) || GetGPIOINBitValue( 507 );	//
	Helicopter->UV26.t_interval = GetGPIOINBitValue( 434 );	//
	Helicopter->UV26.t_reset = GetGPIOINBitValue( 435 );	//
	Helicopter->UV26.t_pusk = GetGPIOINBitValue( 436 ) || GetGPIOINBitValue( 506 );	//
	SetLEDINDICATORValue(Helicopter->_BoardSystem->tablo, 2 );	//сборное табло для DISS и УВ26
// Пульт имитации обледенения 
	SetGPIOOUTBitValue( Helicopter->lp_elp.i_imitation_hot, 185);
	SetGPIOOUTBitValue( Helicopter->lp_elp.i_imitation_cold, 186);
	Helicopter->lp_elp.t_imitation = GetGPIOINBitValue(460);
// СПУ-7 ЛЕВОГО ПМЛОТА
	Helicopter->lpb.spu.t_ukr = GetGPIOINBitValue( 470 );         // Галетник УКР
	Helicopter->lpb.spu.t_sr = GetGPIOINBitValue( 471 );          // Галетник СР
	Helicopter->lpb.spu.t_kr = GetGPIOINBitValue( 472 );          // Галетник КР
	Helicopter->lpb.spu.t_dr = GetGPIOINBitValue( 473 );          // Галетник ДР
	Helicopter->lpb.spu.t_rk1 = GetGPIOINBitValue( 474 );         // Галетник РК1
	Helicopter->lpb.spu.t_rk2 = GetGPIOINBitValue( 475 );         // Галетник РК2
	Helicopter->lpb.spu.t_set_1 = GetGPIOINBitValue( 478 );      // Тумблер сеть 1
	Helicopter->lpb.spu.t_set_2 = GetGPIOINBitValue( 479 );      // Тумблер сеть 2
	Helicopter->lpb.spu.k_cv = GetGPIOINBitValue( 480 );          // Кнопка ЦВ
	Helicopter->lpb.spu.t_spu = GetGPIOINBitValue( 476 );         // Тумблер СПУ
	Helicopter->lpb.spu.t_radio = GetGPIOINBitValue( 477 );       // Тумблер РАДИО
// СПУ-7 правого ПМЛОТА
	Helicopter->rpb.spu.t_ukr = GetGPIOINBitValue(481);         // Галетник УКР
	Helicopter->rpb.spu.t_sr = GetGPIOINBitValue(482);          // Галетник СР
	Helicopter->rpb.spu.t_kr = GetGPIOINBitValue(483);          // Галетник КР
	Helicopter->rpb.spu.t_dr = GetGPIOINBitValue(484);          // Галетник ДР
	Helicopter->rpb.spu.t_rk1 = GetGPIOINBitValue(485);         // Галетник РК1
	Helicopter->rpb.spu.t_rk2 = GetGPIOINBitValue(486);         // Галетник РК2
	Helicopter->rpb.spu.t_set_1 = GetGPIOINBitValue(489);      // Тумблер сеть 1
	Helicopter->rpb.spu.t_set_2 = GetGPIOINBitValue(490);      // Тумблер сеть 2
	Helicopter->rpb.spu.k_cv = GetGPIOINBitValue(491);          // Кнопка ЦВ
	Helicopter->rpb.spu.t_spu = GetGPIOINBitValue(487);         // Тумблер СПУ
	Helicopter->rpb.spu.t_radio = GetGPIOINBitValue(488);       // Тумблер РАДИО
// СПУ-7 борт техника
	Helicopter->ep_elp.spu.t_ukr = GetGPIOINBitValue(492);        // Галетник УКР
	Helicopter->ep_elp.spu.t_sr = GetGPIOINBitValue(493);         // Галетник СР
	Helicopter->ep_elp.spu.t_kr = GetGPIOINBitValue(494);         // Галетник КР
	Helicopter->ep_elp.spu.t_dr = GetGPIOINBitValue(495);         // Галетник ДР
	Helicopter->ep_elp.spu.t_rk1 = GetGPIOINBitValue(496);        // Галетник РК1
	Helicopter->ep_elp.spu.t_rk2 = GetGPIOINBitValue(497);        // Галетник РК2
	Helicopter->ep_elp.spu.t_set_1 = GetGPIOINBitValue(500);     // Тумблер сеть 1
	Helicopter->ep_elp.spu.t_set_2 = GetGPIOINBitValue(501);     // Тумблер сеть 2
	Helicopter->ep_elp.spu.k_cv = GetGPIOINBitValue(502);         // Кнопка ЦВ
	Helicopter->ep_elp.spu.t_spu = GetGPIOINBitValue(498);        // Тумблер СПУ
	Helicopter->ep_elp.spu.t_radio = GetGPIOINBitValue(499);      // Тумблер РАДИО
}
