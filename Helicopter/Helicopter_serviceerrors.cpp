#include "Helicopter_ServiceErrors.h"
#include "Helicopter_instructor.h"
#include "Helicopter_core.h"
#include "Helicopter_Model.h"

Helicopter_ServiceErrors::Helicopter_ServiceErrors( Helicopter_core* Helicopter_ )
{
    Helicopter = Helicopter_;

    FaultInjection[0].Name = "Unknown";
    FaultInjection[1].Name = "Пожар на вертолете";
    FaultInjection[2].Name = "Отказ одного двигателя";
    FaultInjection[3].Name = "Отказ двух двигателей";
    FaultInjection[4].Name = "Неисправности редукторов";
    FaultInjection[5].Name = "Неисправность системы автоматического регулирования (САР) двигателя";
    FaultInjection[6].Name = "Выход частоты вращения несущего винта за допустимые пределы";
    FaultInjection[7].Name = "Повышенная (опасная) вибрация двигателей";
    FaultInjection[8].Name = "Загорание светового табло ЗАСОР ТФ ЛЕВ (ПРАВ) ДВИГ";
    FaultInjection[9].Name = "Падение давления масла в двигателе";
    FaultInjection[10].Name = "Выход двигателя на режим ограничений по температуре газов перед турбиной или по частоте вращения ротора турбокомпрессора";
    FaultInjection[11].Name = "Загорание светового табло СТРУЖКА ЛЕВ (ПРАВ) ДВИГ";
    FaultInjection[12].Name = "Неустойчивая работа двигателя (помпаж)";
    FaultInjection[13].Name = "Отказ топливной системы (подкачивающего и перекачивающих насосов)";
    FaultInjection[14].Name = "Загорание светового табло ОСТАЛОСЬ 270л";
    FaultInjection[15].Name = "Отказ путевого управления";
    FaultInjection[16].Name = "Отказ гидросистем";
    FaultInjection[17].Name = "Порыв диафрагмы одного из гидроаккумулятора основной гидросистемы";
    FaultInjection[18].Name = "Непреднамеренное превышение максимально допустимой скорости полета";
    FaultInjection[19].Name = "Непреднамеренное уменьшение скорости полета ниже минимально допустимой";
    FaultInjection[20].Name = "Самопроизвольное вращение вертолета влево при взлете или посадке";
    FaultInjection[21].Name = "Вихревое кольцо";
    FaultInjection[22].Name = "Появление низкочастотных колебаний в полете";
    FaultInjection[23].Name = "Потеря пространственной ориентировки";
    FaultInjection[24].Name = "Отказ генератора переменного тока и выпрямительных устройств";
    FaultInjection[25].Name = "Загорание светового табло АККУМ 1 (2) t ВЫСОКА";
    FaultInjection[26].Name = "Отказ автопилота";
    FaultInjection[27].Name = "Отказ авиагоризонта (авиагоризонтов)";
    FaultInjection[28].Name = "Отказ курсовой системы";
    FaultInjection[29].Name = "Отказ барометрического высотометра";
    FaultInjection[30].Name = "Отказ левого указателя скорости УС-450К";
    FaultInjection[31].Name = "Отказ радиосвязи";
    FaultInjection[32].Name = "Отказ радиокомпаса АРК-15М";
    FaultInjection[33].Name = "Отказ системы подвижных упоров управления";
    FaultInjection[34].Name = "Неисправности ПОС";
    FaultInjection[35].Name = "Вынужденное покидание вертолета в полете";
    FaultInjection[36].Name = "Рекомендации по действиям экипажа при вынужденном покидании приводнившегося вертолета";
    FaultInjection[37].Name = "Правила вынужденного покидания вертолета на земле";
    FaultInjection[38].Name = "Действия экипажа при возникновении особых случаев в полете с применением очков ГЕО-ОНВ1-01";
    FaultInjection[39].Name = "Отказ очков ГЕО-ОНВ1-01";
    FaultInjection[40].Name = "Усложнение условий полета с применением очков ГЕО-ОНВ1-01";
    FaultInjection[41].Name = "Отказ БКК-18";
    FaultInjection[42].Name = "Отказ БУР-1-2 сер 2";
	FaultInjection[43].Name = "Отказ обоих генераторов переменного тока и выпрямительных устройств";

    connect(ErrorsTimer,SIGNAL(timeout()),this, SLOT(slotCheckErrors()));
    ErrorsTimer->start();
}

void Helicopter_ServiceErrors::slotCheckErrors()
{
	if ( Helicopter->instructor->Failures.PacketReady )
	{
		if ( Helicopter->instructor->Failures.failureType < N_FAULTS || (Helicopter->instructor->Failures.failureType == 250))
		{
			FaultInjection[Helicopter->instructor->Failures.failureType].Active = Helicopter->instructor->Failures.failureActive;
            Helicopter->instructor->Failures.PacketReady = false;
			switch ( Helicopter->instructor->Failures.failureType )
			{
			case F_FIRE:             // Пожар на вертолете
				Helicopter->instructor->fault.Fire = Helicopter->instructor->Failures.failureActive;
				break;
			case F_FIRE_R:             // Пожар на вертолете
				Helicopter->instructor->fault.Fire_R = Helicopter->instructor->Failures.failureActive;
				break;
			case F_FIRE_KO:             // Пожар на вертолете
				Helicopter->instructor->fault.Fire_KO = Helicopter->instructor->Failures.failureActive;
				break;
			case F_FIRE_VSU:             // Пожар на вертолете
				Helicopter->instructor->fault.Fire_VSU = Helicopter->instructor->Failures.failureActive;
				break;
			case F_ENGINE1:          // Отказ одного двигателя
				Helicopter->instructor->fault.Engine1 = Helicopter->instructor->Failures.failureActive;
				break;
			case F_ENGINE2:          // Отказ двух двигателей
				Helicopter->instructor->fault.Engine1 = Helicopter->instructor->Failures.failureActive;
				Helicopter->instructor->fault.Engine2 = Helicopter->instructor->Failures.failureActive;
				break;
			case F_GEAR:             // Неисправности редукторов
				Helicopter->instructor->fault.Gear = Helicopter->instructor->Failures.failureActive;
				break;
			case F_SAR:              // Неисправность системы автоматического регулирования( САР ) двигателя, удален из бд инструктора так как стоит сапфир
				break;
			case F_NV:               // Выход частоты вращения несущего винта за допустимые пределы
				Helicopter->instructor->fault.FreqNV = Helicopter->instructor->Failures.failureActive;
				break;
			case F_VIBR:             // Повышенная вибрация левого двигателя
				Helicopter->instructor->fault.Vibration_left = Helicopter->instructor->Failures.failureActive;
				Helicopter->instructor->fault.Vibration_Max_left = Helicopter->instructor->Failures.failureActive;
				break;
			case F_VIBR_R:             // Повышенная вибрация левого двигателя
				Helicopter->instructor->fault.Vibration_right = Helicopter->instructor->Failures.failureActive;
				Helicopter->instructor->fault.Vibration_Max_right = Helicopter->instructor->Failures.failureActive;
				break;
			case F_TF:               // Загорание светового табло ЗАСОР ТФ ЛЕВ( ПРАВ ) ДВИГ
				Helicopter->instructor->fault.TF_right = Helicopter->instructor->Failures.failureActive;
				Helicopter->instructor->fault.TF_left = Helicopter->instructor->Failures.failureActive;
				break;
			case F_POIL:             // Падение давления масла в Левого двигателя
				Helicopter->instructor->fault.EngP_minLeft = Helicopter->instructor->Failures.failureActive;
				break;
			case F_POIL_Right:             // Падение давления масла в Левого двигателя
				Helicopter->instructor->fault.EngP_minRight = Helicopter->instructor->Failures.failureActive;
				break;
			case F_ENG_TEMP:        // Выход лекого двигателя на режим ограничений по температуре газов перед турбиной или по частоте вращения ротора турбокомпрессора
				Helicopter->instructor->fault.EngLeftTempGas = Helicopter->instructor->Failures.failureActive;
				break;
			case F_ENG_TEMP_R:        // Выход правого двигателя на режим ограничений по температуре газов перед турбиной или по частоте вращения ротора турбокомпрессора
				Helicopter->instructor->fault.EngRightTempGas = Helicopter->instructor->Failures.failureActive;
				break;
			case F_ENG_SWARF:       // Загорание светового табло СТРУЖКА ЛЕВ ДВИГ
				Helicopter->instructor->fault.Swarf_left = Helicopter->instructor->Failures.failureActive;
				break;
			case F_ENG_SWARF_R:       // Загорание светового табло СТРУЖКА ПРАВ ДВИГ
				Helicopter->instructor->fault.Swarf_right = Helicopter->instructor->Failures.failureActive;
				break;
			case F_SURGE:           // Неустойчивая работа двигателя( помпаж левого )
				Helicopter->instructor->fault.PompashLeftEng = Helicopter->instructor->Failures.failureActive;
				break;
			case F_FUEL_PUMP:       // Отказ топливной системы( подкачивающего асосов )
				Helicopter->instructor->fault.FuelPumpRash = Helicopter->instructor->Failures.failureActive;
				break;
			case F_FUEL_PUMP_L:       // Отказ топливной системы( левого перекачивающих насосов )
				Helicopter->instructor->fault.FuelPumpPereLeft = Helicopter->instructor->Failures.failureActive;
				break;
			case F_FUEL_PUMP_TWO:       // Отказ топливной системы( обоих перекачивающих насосов )
				Helicopter->instructor->fault.FuelPumpPereTWO = Helicopter->instructor->Failures.failureActive;
				break;
			case F_FUEL_270:        // Загорание светового табло ОСТАЛОСЬ 270л
				Helicopter->instructor->fault.Fuel270 = Helicopter->instructor->Failures.failureActive;
				break;
			case F_CURS_CTRL:       // Отказ путевого управления
				Helicopter->instructor->fault.RV_contrl = Helicopter->instructor->Failures.failureActive;
				break;
			case F_CURS_GEAR:       // Отказ путевого управления
				Helicopter->instructor->fault.RV_gear = Helicopter->instructor->Failures.failureActive;
				break;
			case F_HYDRO:           // Отказ гидросистем
				Helicopter->instructor->fault.HidroMain = Helicopter->instructor->Failures.failureActive;
				break;
			case F_HIDRO_AKUM:      // Порыв диафрагмы одного из гидроаккумулятора основной гидросистемы
				Helicopter->instructor->fault.HidroDiafrag = Helicopter->instructor->Failures.failureActive;
				break;
			case F_MAX_SPEED:       // Непреднамеренное превышение максимально допустимой скорости полета
				break;
			case F_MIN_SPEED:       // Непреднамеренное уменьшение скорости полета ниже минимально допустимой
				break;
			case F_ROTATION:        // Самопроизвольное вращение вертолета влево при взлете или посадке
				Helicopter->instructor->fault.LeftCursNoCNTRL = Helicopter->instructor->Failures.failureActive;
				break;
			case F_RING:            // "Вихревое кольцо"
				break;
			case F_LOW_M:           // Появление низкочастотных колебаний в полете
				break;
			case F_LOST_SA:         // Потеря пространственной ориентировки
				break;
			case F_VOLT:            // Отказ генераторов переменного тока и выпрямительных устройств
				Helicopter->instructor->fault.Gen1 = Helicopter->instructor->Failures.failureActive;
				break;
			case F_VOLT_TWO:            // Отказ генераторов переменного тока и выпрямительных устройств
				Helicopter->instructor->fault.Gen1 = Helicopter->instructor->Failures.failureActive;
				Helicopter->instructor->fault.Gen2 = Helicopter->instructor->Failures.failureActive;
				break;
			case F_VU_TWO:            // Отказ обоих выпрямительных устройств
				Helicopter->instructor->fault.VU1 = Helicopter->instructor->Failures.failureActive;
				Helicopter->instructor->fault.VU2 = Helicopter->instructor->Failures.failureActive;
				break;
			case F_VU_1:            // Отказ одного выпрямительного устройства
				Helicopter->instructor->fault.VU1 = Helicopter->instructor->Failures.failureActive;
				break;
			case F_AKKUM_T:         // Загорание светового табло АККУМ 1 ( 2 ) t ВЫСОКА
				Helicopter->instructor->fault.AkbTempHight_1 = Helicopter->instructor->Failures.failureActive;
				break;
			case F_AUTOPILOT:       // Отказ автопилота всех каналов
				Helicopter->instructor->fault.Autopilot = Helicopter->instructor->Failures.failureActive;
				break;
			case F_AUTOPILOT_CURS:       // Отказ автопилота по курсу
				Helicopter->instructor->fault.Autopilot_curs = Helicopter->instructor->Failures.failureActive;
				break;
			case F_AUTOPILOT_PITCH:       // Отказ автопилота по крену и тангажу
				Helicopter->instructor->fault.Autopilot_pitch = Helicopter->instructor->Failures.failureActive;
				break;
			case F_AUTOPILOT_ALT:       // Отказ автопилота по высоте
				Helicopter->instructor->fault.Autopilot_alt = Helicopter->instructor->Failures.failureActive;
				break;
			case F_AGR:             // Отказ авиагоризонта( авиагоризонтов )
				Helicopter->instructor->fault.AviaHorizont = Helicopter->instructor->Failures.failureActive;
				break;
			case F_AGR_TWO:             // Отказ авиагоризонта( авиагоризонтов )
				Helicopter->instructor->fault.AviaHorizont_two = Helicopter->instructor->Failures.failureActive;
				break;
			case F_GMK:             // Отказ курсовой системы
				Helicopter->instructor->fault.Curs = Helicopter->instructor->Failures.failureActive;
				break;
			case F_BAR:             // Отказ барометрического высотометра
				Helicopter->instructor->fault.Barometer = Helicopter->instructor->Failures.failureActive;
				break;
			case F_SPEED:           // Отказ левого указателя скорости УС - 450К
				Helicopter->instructor->fault.Speed = Helicopter->instructor->Failures.failureActive;
				break;
			case F_RADIO:           // Отказ радиосвязи
				Helicopter->instructor->fault.radio = Helicopter->instructor->Failures.failureActive;
				break;
			case F_ARK:             // Отказ радиокомпаса АРК - 15М
				Helicopter->instructor->fault.ARK = Helicopter->instructor->Failures.failureActive;
				break;
			case F_SPPU:            // Отказ системы подвижных упоров управления
				Helicopter->instructor->fault.spuu = Helicopter->instructor->Failures.failureActive;
				break;
			case F_POS_V:             // Неисправности ПОС винтов
				Helicopter->instructor->fault.pos_vint = Helicopter->instructor->Failures.failureActive;
				break;
			case F_POS_PZU:             // Неисправности ПОС двигателей пзу
				Helicopter->instructor->fault.pos_pzu = Helicopter->instructor->Failures.failureActive;
				break;
			case F_POS_GLASS:             // Неисправности ПОС стекол
				Helicopter->instructor->fault.pos_glass = Helicopter->instructor->Failures.failureActive;
				break;
			case F_POS_PVD:             // Неисправности ПОС обогрева пвд
				Helicopter->instructor->fault.pos_pvd = Helicopter->instructor->Failures.failureActive;
				break;
			case F_ESC:             // Вынужденное покидание вертолета в полете
				break;
				break;
			case F_INSTR_ESC:       // Рекомендации по действиям экипажа при вынужденном покидании приводнившегося вертолета
				break;
			case F_LAND_ESC:        // Правила вынужденного покидания вертолета на земле
				break;
			case F_GEO:             // Действия экипажа при возникновении особых случаев в полете с применением очков ГЕО - ОНВ1 - 01
				break;
			case F_GEO_FAULT:       // Отказ очков ГЕО - ОНВ1 - 01
				break;
			case F_GEO_:            // Усложнение условий полета с применением очков ГЕО - ОНВ1 - 01
				break;
			case F_BKK18:           // Отказ БКК - 18
				Helicopter->instructor->fault.BKK18 = Helicopter->instructor->Failures.failureActive;
				break;
			case F_BUR:              // Отказ БУР - 1 - 2 сер 2
				Helicopter->instructor->fault.BUR = Helicopter->instructor->Failures.failureActive;
				break;
			case F_AERO_PTS:
				Helicopter->instructor->fault.aero_pts = Helicopter->instructor->Failures.failureActive;
			default:
				break;
			}
		}
	}
}
