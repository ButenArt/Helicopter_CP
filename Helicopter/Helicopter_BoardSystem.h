#ifndef Helicopter_BOARD_SYSTEM_H
#define Helicopter_BOARD_SYSTEM_H

#include <Windows.h>
#include <QObject>
#include <QTimer>
#include <QDebug>
#include <array>
#include <cmath>
#include "encoder.h"
#include "Common/avia_dictionary.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdint>
#include <ctime>
//#include "Helicopter_core.h"

class Helicopter_core;

constexpr int COMPSTEP = 100;  // расчет в бортовых системах каждые 100мс

#ifndef M_PI
const double M_PI = std::acos( -1.0 );
#endif // M_PI

/**
 * @brief The _NAME_ class
 * Класс отвечает за бортовые системы вертолета
 * Compute() это автомат работы стартера
 *
class _NAME_ : public QObject
{

  Q_OBJECT

  public:
    _NAME_(Helicopter_core *Helicopter);
    ~_NAME_();
    void Compute(float dT);
  private:

  private slots:
};
**/

class Magnetic{
protected:
    /*static*/ [[nodiscard]] float norm360( float angle );
    /*static*/ [[nodiscard]] float norm180( float angle );
public:
    static float magneticDeclination;
};

/**
 * @brief The AVG class
 * Класс прибора авиагоризонт
 */
class AVG {
public:
    float roll = 0.f;             // АВИАГОРИЗОНТ ЛЕВ-КРЕН
    float pitch = 0.f;            // АВИАГОРИЗОНТ ЛЕВ-ТАНГАЖ
    float yawl = 0.f;             // АВИАГОРИЗОНТ ЛЕВ-РЫСКАНИЕ
    float index = 0.f;            // АВИАГОРИЗОНТ ЛЕВ-индекс
    float shift_pitch = 0.f;      // АВИАГОРИЗОНТ ЛЕВ-центровка шкалы тангажа
    bool  blank = false;          // АВИАГОРИЗОНТ ЛЕВ:Флажок отс. питания
    int   encoder = 0;          // ручка энкодера +-1 для пересчета alignment
    bool  up = 0;               // ручка энкодера как кнопка +-1 для пересчета alignment
    bool  down = 0;             // ручка энкодера как кнопка +-1 для пересчета alignment
    bool  t_appetitr = false;
    float getPitch( )const { return (pitch + shift_pitch); };
};

/**
 * @brief The RV_3 class
 * Класс прибора радиовысотомер
 */
class RV_5 {
    float alt_;                // ВЫСОТОМЕР-Стрелка
    float test_alt = 0;
public:
    bool azs_rv = false;
    bool blank = false;       // Бленкер
    bool t_test = false;      // ВЫСОТОМЕР-Кнопка ТЕСТ
    bool t_triangle = false;  // ВЫСОТОМЕР-индикатор с треугольником
	float f_index = 0.f;	  //ВЫСОТОМЕР-Крутилка индекса на ОУ
    int  encoder = 0;         // крутилка индекса 
    bool  up = 0;               // ручка энкодера как кнопка +-1 для пересчета alignment
    bool  down = 0;             // ручка энкодера как кнопка +-1 для пересчета alignment
    float index = 20.f;       // индекс
    bool flag_t_rv = false;   // флаг срабатывания тумблера РВ на правой боковой панели
    float alt( )
    { 
        auto alt = alt_;
        if (t_test && azs_rv && alt_ <= 4) {
            test_alt += 0.1f;
            if (test_alt > 17)
                test_alt = 17;
            return test_alt;
        }
        else if (test_alt > 1) {
            if (flag_t_rv)
                test_alt -= 0.0002f;
            return test_alt;
        }
        else {
            test_alt = 0.;
        }

        if ( alt > 750 )
        {
            alt = 750; blank = true;
        };
        return alt;
    }
    void set_alt( float alt )
    {
        alt_ = alt;
    };
    void set_test_alt(float alt) {
        test_alt = alt;
    }
    float get_test_alt() { return test_alt; }
};

/**
 * @brief The UT_6K class
 * Класс прибора Указатель 2УТ-6К температуры газов
 * двигателя ТВ3-117ВМ
 */
class UT_6K {
public:
    float temp1 = 0.f; //Термометр-стрелка-ЛЕВ ВЕРХ левый двигатель
    float temp2 = 0.f; //Термометр-стрелка-ПРАВ ВЕРХ правый двигатель
    float get_temp1_fine() { return remainder(temp1, 100.f); };
    float get_temp2_fine() { return remainder(temp2, 100.f); };
    float temp_1 = 0.f; 
    float t100_1 = 0.f; 
    float t_1 = 0.f;  
    float temp_2 = 0.f;
    float t100_2 = 0.f;
    float t_2 = 0.f;
public:
    void set_temp_1(float te) { this->temp_1 = te; t100_1 = te / 100.f;  t_1 = t_1 - static_cast<int>(t100_1); };
    float get_temp_1() { return temp_1; };
    float get_t100_1() { if (temp_1 < 100) return temp_1; else return temp_1; };
    float get_t_1() { return t_1; };
    void set_temp_2(float te) { this->temp_2 = te; t100_2 = te / 100.f;  t_2 = t_2 - static_cast<int>(t100_2); };
    float get_temp_2() { return temp_2; };
    float get_t100_2() { if (temp_2 < 100) return temp_2; else return temp_2; };
    float get_t_2() { return t_2; };
};

/**
 * @brief The VD_10VK class
 * Класс прибора Высотомер ВД-10ВК
 *
 */
class VD_10VK {
    float alt = 0.f; // высота
    float km = 0.f; //Высотометр (КМ)-стрелка
    float m = 0.f;  //Высотометр (М)-стрелка
public:
    void set_alt(float alt) { this->alt = alt; km = alt / 1000.f;  m = alt - static_cast<int>(km); };
    float get_alt() { return alt; };
    float get_km() { return km; };
    float get_m() { return m; };
    float p = 0.f;  //Высотомер-давление
    float f_barometr = 671; //Высотомер-крутилка юстировки
    int   encoder = 0;
};

/**
 * @brief The IR_117 class
 * Класс прибора Указатель режимов ИР-117
 *
 */
class IR_117 {
public:
    float index = 0.f; //ИЗМЕРИТЕЛЬ РЕЖИМОВ-Центральный индекс
    float eng1 = 0.f; //ИЗМЕРИТЕЛЬ РЕЖИМОВ-Левый указатель
    float eng2 = 0.f; //ИЗМЕРИТЕЛЬ РЕЖИМОВ-Правый указатель
};
//
///**
// * @brief The UGR_4UK class
// * Класс прибора УГР-4УК
// *
// */
//class UGR_4UK {
//    float dir_ = 0.f;         // текущий курс
//public:
//    float d1 = 0.f;           // толстая стрелка
//    float ark = 0.f;           // тонкая стрелка
//    float f_set_course = 0.f; //  - вращалка счётчика заданного курса
//    int   corrector = 0;
//
//    // TODO: требуется сделать расчет склонения от местоположения, можно по номеру карты
//    float dir() { return (dir_ - 5); };     // текущий курс магнитный с учетом склонения в Актюбинске
//    void set_dir(float dir) { dir_ = dir; };// текущий курс
//    float& set_dir( ) { return dir_; };// текущий курс для записи типа set_dir( ) = MAB(1);
//};

// {43.33855555555556, 77.01205555555556} // ILS LOC 23L III / E / 2  108.1 MHZ
const double MAX_DEVIATION = 15.0; // Максимальное отклонение в градусах
// код класса Antena сгенерирован GPT
// Класс, представляющий антенну ILS
class Antenna {
public:
    Antenna( double latitude, double longitude, double direction )
        : latitude_( latitude ), longitude_( longitude ), direction_( direction ) {}

    double getDeviation( double aircraftLatitude, double aircraftLongitude ) const {
        double angle = calculateAngle( aircraftLatitude, aircraftLongitude );
        double deviation = angle - direction_;
        deviation = normalizeAngle180( deviation );
        return deviation;
    }

    double getGlide( double aircraftLatitude, double aircraftLongitude, double alt ) const {
        double distance = 1000 * calculateDistance( aircraftLatitude, aircraftLongitude );
        double angle = std::atan2( alt, distance ) * 180.f / M_PI;
        double deviation = 3. - angle;
        deviation = normalizeAngle180( deviation );
        return deviation;
    }

    bool isInAngle( double aircraftLatitude, double aircraftLongitude, double angleThreshold ) const {
        double angle = calculateAngle( aircraftLatitude, aircraftLongitude );
        double deviation = std::abs( normalizeAngle180( angle - direction_ ) );
        return deviation < angleThreshold;
    }

    bool isInRange( double aircraftLatitude, double aircraftLongitude ) const {
        double distance = calculateDistance( aircraftLatitude, aircraftLongitude );
        return distance <= range_;
    }

    // относительный расчет дальности, учитывает трапецию, но не считает в КМ, а градусах. Умножить на 111.321 для км.
    double calculateDistance( double aircraftLatitude, double aircraftLongitude ) const {
        double distance = std::hypot( aircraftLatitude - latitude_, ( ( aircraftLongitude - longitude_ ) * cos( latitude_ * M_PI / 180. ) ) ) * 111.321;
        //PRINT_CHANGED_VAR( distance );
        return distance;
    }

private:
    double latitude_;
    double longitude_;
    double direction_;
    double range_ = 45.0;

    // возвращает угол места ЛА относительно антены
    double calculateAngle( double aircraftLatitude, double aircraftLongitude ) const {
        double latitude = aircraftLatitude - latitude_;
        double longitude = ( aircraftLongitude - longitude_ ) * cos( latitude_ * M_PI / 180. );

        double angle = std::atan2( longitude, latitude ) * 180.f / M_PI;
        angle = normalizeAngle360( angle );
        //PRINT_CHANGED_VAR( angle );
        return angle;
    }


    double normalizeAngle360( double angle ) const {
        angle = std::fmod( angle + 360.0, 360.0 );
        return angle;
    }

    double normalizeAngle180( double angle ) const {
        angle = normalizeAngle360( angle );
        if ( angle > 180.0 )
            angle -= 360.0;
        return angle;
    }
};

/**
 * @brief The PNP_72 class
 * Класс прибора ПНП-72
 *
 */
class PNP_72 : public Magnetic {
    std::vector<Antenna> antDir_;
    std::vector<Antenna> antGlide_;
    float dir_;//ПНП-текущий курс
    float ark_ = 0.f; //ПНП-тонкая стрелка
    float d1_ = 0.f; //ПНП-толстая стрелка
    float curse_ = 0.f; //ПНП-шкала отклонения омввт курсового маяка (вертикальная)
    float glide_ = 0.f; //ПНП-линия вертикали (сноса)
public:
    PNP_72( );
    float dir( ) const { return ( dir_ - Magnetic::magneticDeclination ); };//ПНП-текущий курс + Magnetic::magneticDeclination
    float ark( ) const { return ( ark_ /*- Magnetic::magneticDeclination*/ ); }; //+ Magnetic::magneticDeclination
    float d1( ) const { return ( d1_ - Magnetic::magneticDeclination ); }; //+ Magnetic::magneticDeclination
    void  set_ark( float a ); //+ Magnetic::magneticDeclination
    void  set_dir( float dir );//ПНП-текущий курс
    void  set_D1( int encoder );//ПНП-толстая стрелка
    void  set_curse( double latitude, double longitude );
    void  set_glide( double latitude, double longitude, double alt );
    float  glide( );
    float  curse( );
    bool  b1 = false; //ПНП - Бленкер отказа датчика и указателя текущ курса
    bool  bk = false; //ПНП Флажок К боковой
    bool  bg = false; //ПНП Флажок Г боковой
    bool  br = false;  //ПНП - Бленкер счётчика дальности
    bool  bdir = false; //ПНП - Бленкер счётчика заданного курса
    int   n_range = 0; //ПНП - счётчик дальности
    int   n_csc = 0;//ПНП - счётчик заданного курса
    bool  test = false;//ПНП - кнопка ТЕСТ
    //float f_set_course = 0.f;//ПНП-вращалка счётчика заданного курса
    int   encoder = 0;//ПНП-вращалка счётчика заданного курса
    friend QDebug operator<<( QDebug out, const PNP_72& p ){
        out << p.dir_ << p.ark_ << p.dir( ) << p.ark( ) << p.n_range << p.n_csc;
        return out;
    };
};

/**
 * @brief The SS class
 * Класс индикатор малых скоростей
 *
 */
class SS {
public:
    bool i_ss = false; //Индикатор МС - лампа ВЫК
    float speed_x = 0.f; //Индикатор МС-вперёд
    float speed_x_down = 0.f; //Индикатор МС-назад
    float speed_z_left = 0.f; //Индикатор МС-влево
    float speed_z = 0.f; //Индикатор МС-вправо
    float speed_y = 0.f; //Индикатор МС-вверх-вниз
};

/**
 * @brief The UP_21_15 class
 * Класс прибора Индикатор УП-21-15 указателя шага несущего винта
 *
 */
class UP_21_15 {
public:
    float fosh = 0.f; //Fош [град], угол общего шага НВ
};

/**
 * @brief The UC_450K class
 * Класс прибора Указатель скорости УС-450К
 *
 */
class UC_450K {
    float speed_ = 0.f; // скорость
public:
    float& speed( ) { return speed_; };
    float get_speed( ) { return speed_ < 40 ? 40 : speed_; }; // скорость
};

/**
 * @brief The VR_30MK class
 * Класс прибора Вариометр ВР-30МК
 *
 */
class VR_30MK {
public:
    float var = 0.f; //вариаометр
    float var4clock = 0.f; //вариаометр
    float variometr( float v ) { var = v; var4clock = v < 0 ? v + 60 : v; return var; }; //вариаометр
};

/**
 * @brief The ITE_1T class
 * Класс прибора Указатель ИТЭ-1Т числа оборотов НВ
 *
 */
class ITE_1T {
public:
    float nv = 0.f;
    void simulate() { nv += nv < 30.f ? 0.02f : 0.f; };
    void simulationState(bool state) { simulated = state; };
    bool isSimulated( ) { return simulated; };
private:
    bool simulated = false;
};

/**
 * @brief The ITE_2T class
 * Класс прибора Двухстрелочный указатель ИТЭ-2Т числа оборотов двигателя ТВЗ-117ВМ
 *
 */
class ITE_2T {
public:
    float eng1 = 0.f;
    float eng2 = 0.f;
};


/**
 * @brief The UGR_4UK class
 * Класс Указатель УГР-4УК курсовой системы ГМК-1А
 *
 */
class UGR_4UK : public Magnetic {
    float dir_ = 0.f;         // текущий курс
    float ark_ = 0.f;         // тонкая стрелка
    float d1_ = 0.f;          // толстая стрелка
public:
    void set_D1( int encoder );
    void set_ark( float ark );
    void set_dir( float dir ) { 
        if (dir > 359)
            dir = 0;
        if (dir < 0)
            dir = 359;
        dir_ = dir; 
    };  // текущий курс
    int   encoder = 0;
    float magnetic = 10;
    float d1( ) { return ( d1_ - Magnetic::magneticDeclination ); };     // текущий курс магнитный с учетом склонения // + Magnetic::magneticDeclination
    float dir( ) { return (dir_/* - Magnetic::magneticDeclination*/); };     // текущий курс магнитный с учетом склонения // + Magnetic::magneticDeclination
    float ark( ) { return ( float(int(ark_)) /*- Magnetic::magneticDeclination*/ ); };     // текущий курс магнитный с учетом склонения // + Magnetic::magneticDeclination
};

 /**
  * @brief The SNOS class
  * Класс Индикатор путевой скорости и угла сноса аппаратуры ДИСС-15
  *
  */
class SNOS {
public:
    float angle = 0.f;// УГОЛ СНОСА прибор R169
    float speed = 0.f;// Спидометр - км/час R198*3.6
    bool i_angle = false;// УГОЛ СНОСА - индикатор
    bool t_pk = false;// УГОЛ СНОСА - Переключатель Р-К TODO: что это такое на сносе
    bool t_cm = false;// УГОЛ СНОСА - Переключатель С-М
};


struct Point {
	float latitude;
	float longitude;
};

struct Line {
	Point p; // точка на прямой, кординаты VOR
	float alpha; // угол наклона прямой в радианах
};

const double kEarthRadiusKM = 6378.0;

/**
 * @brief The KI_204 class
 * Класс KI-204 навигатор
 *
 */
class KI_204 : public Magnetic {
    float course_ = 0;// НАВИГАТОР-текущий курс
    float radial_ = 0;
    float freq_ = 108.3f;//112.3f; // по умолчанию частота Алматинского VOR
    float DME_ = 0.f;  // растояние до VOR

    Point  OBS_geo = { 0.f,0.f };

    struct station {
        int id;
        float freq;
        Point pos;
    };

    //float norm( float& alpha );
    float distance( Point position );  // дистанция указаной точки до заданого радиала
    void fromto( ); // расчет флагов FROM и TO

public:
    KI_204( );
    auto find();
    float course( ) const { return ( course_ - Magnetic::magneticDeclination ); };// НАВИГАТОР-текущий курс // + Magnetic::magneticDeclination
    void set_course( float course ) { course_ = course + Magnetic::magneticDeclination; };  //  для начальной инициализации по магитному курсу
    //	float radial( ) const { return radial_; };  // это свойство нужно только внутри для расчета располежение ЛА относительно VOR
    float freq( ) const  { return freq_; };		// вывод настроенной в приемникечастоты

    void  set_radial( int encoder );			// установка заданного радиала вращением OBS
    void  set_radial( float rad );			// установка заданного радиала вращением радиала

    float freq( float freq );					// задание частоты приемника VOR

    void  Calc( float lat, float lon );			// основной метод расчета показаний прибора
    auto VOR( ) -> Point;						// сервисный метод вывода коорддинат VOR на который настроен прибор


    std::vector<station> vs;
    int encoder = 0;
    float glis = 0;// НАВИГАТОР-линия горизонтали
    float dir = 0;// НАВИГАТОР-линия вертикали курса
    float roll = 0;// Навигатор-крен
    bool  i_fr = false;// НАВИГАТОР Индикатор FR
    bool  i_to = false;// НАВИГАТОР Индикатор TO
    bool  i_from_to = false;// НАВИГАТОР Индикатор TO
    bool  i_gs = false;// Навигатор флажок GS
    bool  i_nav = false;// НАВИГАТОР Флажок NAV
    friend std::ostream& operator<<( std::ostream& out, const KI_204& obj );
};
/**
 * @brief The DISS_15 class
 * Класс прибора ДИСС-15
 *
 */
class DISS_15 {
private:
    // координаты при включении ДИСС
    float x_start = 0.f;
    float z_start = 0.f;

    // текущие координаты
    float x_cur = 0.f;
    float z_cur = 0.f;

    // заданный путь, курс и уклонение
    float set_way = 0;
    float set_angle = 0;
    float set_side = 0;

    float f_side = 0.f;   // Боковое уклонение-значение в метрах
    float f_way = 0.f;    // Путь-значение в метрах
    float way_cur = 0.f;  // пройденый маршрут после изменения курса
    float way_prev = 0.f; // пройденый маршрут до изменения курса
    float step_ = 100.f;
    float step_a_ = 0.01f;
    int   pressed_ = 0;
public:
    bool t_rght = false;    // Кнопка ВПР
    bool t_left = false;    // Кнопка ВЛ
    quint8 sw_side = 0;       // БОКОВОЕ УКЛОНЕНИЕ ВЛЕВО-ВПРАВО
    bool t_v = false;       // Кнопка В
    bool t_n = false;       // Кнопка Н
    quint8 sw_way = 0;        // ПУТЬ ВПЕРЕД-НАЗАД
    bool t_plus = false;    // Кнопка +
    bool t_minus = false;   // Кнопка -
    float f_angle = 0.f;  // Угол карты-значение
    bool t_on = false;      // Кнопка ВКЛ
    bool t_off = false;     // Кнопка ОТКЛ
    bool t_prev = false;
    bool i_on = false;      // ДИСС - лампа ВКЛЮЧЕНО

    void off();
    float get_side() { return f_side * 0.001f; };
    float get_way() { return f_way * 0.001f; };
    void set_test_way(float x) { f_way = x; }
    void set_test_side(float x) { f_side = x; }
    void fill();
    void setX(float x) { x_cur = x; }
    void setZ(float z) { z_cur = z; }
    void setX_start(float xs) { x_start = xs; }
    void setZ_start(float zs) { z_start = zs; }
    void button_proc();

    bool state = false;

    // debug
    float getXc() const { return x_cur; }
    float getZc() const { return z_cur; }
    float getSetWay() const { return set_way; }
    float getSetAngle() const { return set_angle; }
};

class FreqSelector {
public:
    FreqSelector( const std::string& filename ) {
        loadTablesFromFile( filename );
    }

    int getSelectedFrequency( const std::vector<int>& hundredsInputs,
        const std::vector<int>& tensInputs,
        const std::vector<int>& onesInputs ) {
        int hundreds = getRowNumber( hundredsTable_, hundredsInputs );
        int tens = getRowNumber( tensTable_, tensInputs );
        int ones = getRowNumber( onesTable_, onesInputs );

        return ( hundreds * 100 ) + ( tens * 10 ) + ones;
    }

    int getSelectedFrequency( ) {
        int hundreds = getRowNumber( hundredsTable_, t_hndr );
        int tens = getRowNumber( tensTable_, t_dec );
        int ones = getRowNumber( onesTable_, t_uni );

        return ( hundreds * 100 ) + ( tens * 10 ) + ones;
    }

    std::vector<int> t_uni = std::vector<int>( 5 );
    std::vector<int> t_dec = std::vector<int>( 5 );
    std::vector<int> t_hndr = std::vector<int>( 5 );
private:
    void loadTablesFromFile( const std::string& filename ) {
        std::ifstream file( filename );
        if ( file.is_open( ) ) {
            loadTable( file, hundredsTable_ );
            loadTable( file, tensTable_ );
            loadTable( file, onesTable_ );
            file.close( );
        }
        else {
            std::cerr << "Error: Failed to open file " << filename << std::endl;
        }
    }

    void loadTable( std::ifstream& file, std::vector<std::vector<int>>& table ) {
        size_t rows, cols;
        file >> rows >> cols;
        table.resize( rows, std::vector<int>( cols ) );

        for ( size_t i = 0; i < rows; ++i ) {
            for ( size_t j = 0; j < cols; ++j ) {
                file >> table[i][j];
            }
        }
    }

    int getRowNumber( const std::vector<std::vector<int>>& truthTable, const std::vector<int>& inputs ) {
        for ( size_t i = 0; i < truthTable.size( ); ++i ) {
            const std::vector<int>& row = truthTable[i];
            if ( row == inputs ) {
                return i;
            }
        }

        return 0;  // Входные параметры не соответствуют ни одной строки таблицы. было -1
    }

private:
    std::vector<std::vector<int>> hundredsTable_;
    std::vector<std::vector<int>> tensTable_;
    std::vector<std::vector<int>> onesTable_;
};

struct station {
    float freq;
    float lat;
    float lon;
    QString id;
};

class Stantions
{
    std::vector<station> vs;
public:

    Stantions( const std::string& filename ){
        loadBeaconFromFile( filename );
    }
    std::size_t size( ) const
    {
        return vs.size( );
    }

    const station& operator[]( std::size_t index ) const
    {
        return vs[index];
    }

    station& operator[]( std::size_t index )
    {
        return vs[index];
    }

    std::vector<station>::const_iterator begin( ) const
    {
        return vs.begin( );
    }

    std::vector<station>::const_iterator end( ) const
    {
        return vs.end( );
    }

    std::vector<station>::iterator begin( )
    {
        return vs.begin( );
    }

    std::vector<station>::iterator end( )
    {
        return vs.end( );
    }
private:
    bool loadBeaconFromFile( const std::string& filename );
};

/*
    Класс приборов АРК-9

    Для использования АРК-9 необходимо задать курс, координаты вертолета,
    получить данные о частоте (сотни, десятки, единицы).
    Пример описан в debug()
    Для использования АРК-УД необходимо получить данные о выбраном
    канале
*/
class ARK_9 : public Magnetic
{
public:
    ARK_9();

    // Считает направление тонкой стрелки АРК
    float get_curs( double latitude, double longitude, double course );
    float get_freq() { return freq; }
    float f_rk_vol = 0;           // Вращалка 1 Ручка "ГРОМК." для регулировки громкости сигналов
    bool t_rk_tlg = false;      // Тумблер ТЛГ
    bool t_rk_tlf = false;      // Тумблер ТЛФ
    bool flag_pomeh = false;    // Тональный шум 
    bool t_rk_ramka = false;    // Кнопка РАМКА
    bool t_rk_ctrl = false;     // Кнопка УПР
    bool t_rk_chanel = false;   // Тумблер КАНАЛ "Б – Д" для переключения наборных устройств
    bool freqAkt_left = false;  // Левый галетник определеная частота
    bool freqAkt_right = false;  // Левый галетник определеная частота
    bool freqHleb_left = false;  // Левый галетник определеная частота
    bool freqHleb_right = false;  // Левый галетник определеная частота
    bool ritaAktobe = false;
    bool ritaHleb = false;

    // Пульт управления
    float f_signal_power = 0.f;   // Индикатор мощности сигнала 

    FreqSelector freq_selector1 = FreqSelector( "ARK1.txt" );     // набор бит при работе командного устройства
    FreqSelector freq_selector2 = FreqSelector( "ARK2.txt" );     // набор бит при работе командного устройства

    quint8 sw_rk_mode( );        // Галетный Переключатель
    bool t_rk_off = false;
    bool t_rk_com = false;
    bool t_rk_ant = false;
    bool t_rk_ram = false;

    float f_rk_left_top = 0;    // ЛЕВ КГЦ-ВРАШАЛКА (ВЕРХНЯЯ) для имитатора приборов
    float f_rk_left_mid = 0;    // ЛЕВ КГЦ-ВРАШАЛКА (СРЕДНЯЯ) для имитатора приборов
    float f_rk_left_bot = 0;    // ЛЕВ КГЦ-ВРАШАЛКА (НИЖНЯЯ) для имитатора приборов
    float f_rk_right_top = 0;   // ПРАВ КГЦ-ВРАШАЛКА (ВЕРХНЯЯ) для имитатора приборов
    float f_rk_right_mid = 0;   // ПРАВ КГЦ-ВРАШАЛКА (СРЕДНЯЯ) для имитатора приборов
    float f_rk_right_bot = 0;   // ПРАВ КГЦ-ВРАШАЛКА (НИЖНЯЯ) для имитатора приборов
    float f_rk_radio1 = 0.f;	// результирующая частота на левом КП
    float f_rk_radio2 = 0.f;	// результирующая частота на правом КП

private:
    // АРК-9
    Stantions vs = Stantions( "NDB.txt" );
    //std::vector<station> vs;
    //std::vector<station> loadBeaconFromFile( const std::string& filename );
    bool t_sv = false;
    size_t t_chanel = 0;
    float course = 0.f;           // Текущий курс, град.
    float freq = 0.f;             // Выбранныя частота
    const float freq_diff = 10.f;       // Максимальное отклонение от заданной частоты
    float x = 0.f;                // Координаты вертолета
    float y = 0.f;
    float x_station = 0.f;        // Координаты выбранной станции
    float y_station = 0.f;

    //void choose_station();  // Выбор станции 
    float calc_dist( );      // Расстояние до станции
};

/*
    Класс приборов АРК-УД

    Для использования АРК-УД необходимо задать курс, координаты вертолета,
    Для использования АРК-УД необходимо получить данные о выбраном
    канале
    Пример описан в debug()
*/
class ARK_UD : public Magnetic
{
public:
    ARK_UD( );

    // Считает направление тонкой стрелки АРК
    float get_curs( double latitude, double longitude, double course );

    bool t_rkud_sens = false;     // Тумблер ЧУВСТВ
    bool t_rkud_ukv = false;      // Тумблер УКВ-ДЦБ
    float f_rkud_vol = 0;         // громкость
    bool i_rkud_1 = false;        // Индикатор УП
    bool i_rkud_2 = false;        // Индикатор ШП
    bool i_rkud_3 = false;        // Индикатор И

    bool t_contr = false;		  // Кнопка Контроль
    bool t_ant_l = false;         // Кнопка АНТ Л
    bool t_ant_r = false;         // Кнопка АНТ П

    bool t_ukv_dcv = false;         // Переключатель "УКВ-ДЦВ" АРК-УД
    quint8 sw_rkud_mode( );        // Галетный Переключатель
    bool t_rkud_off = false;
    bool t_rkud_up = false;
    bool t_rkud_shp = false;
    bool t_rkud_i = false;
    bool t_rkud_rpk = false;
    quint8 sw_rkud_chanel( );      // Галетный переключатель КАНАЛЫ
    bool t_rkud_1 = false;
    bool t_rkud_2 = false;
    bool t_rkud_3 = false;
    bool t_rkud_4 = false;
    bool t_rkud_5 = false;
    bool t_rkud_6 = false;

private:
    // АРК-УД
    // Фиксированные частоты по каналам на ПУ
    std::array<float, 7> ukv_fixed_freqs = { 0.0f, 114166.f, 114333.f, 114583.f, 121500.f, 123100.f, 124100.f };
    //std::vector<station> vs;
    Stantions vs = Stantions( "UHF.txt" );
    float course = 0.f;           // Текущий курс, град.
    float freq = 0.f;             // Выбранныя частота
    const float freq_diff = 10.f;       // Максимальное отклонение от заданной частоты
    float x = 0.f;                // Координаты вертолета
    float y = 0.f;
    float x_station = 0.f;        // Координаты станции
    float y_station = 0.f;
};

/**
 * @brief The class KDI_572
 * Класс дальномера KDI_572
 *
 */
class KDI_572
{

public:
    quint8 sw_switch = 0;   // Переключатель KDI-572
    bool  t_N1 = false;     // переключатель N1
    bool  t_HLD = false;
    bool  t_N2 = false;

    quint32 i_S = 0;          // Индикатор KDI-572 №1
    quint32 i_S_R = 0;          // Индикатор KDI-572 №1
    quint32 i_Speed = 0;          // Индикатор KDI-572 №2
    quint32 i_Time = 0;          // Индикатор KDI-572 №3
    bool freq_ok = false;
    float freq_HLD = 0;
    float freq_ready = 0.0f;
    float freq_reserv = 0.0f;
    float f_1 = 0.f;
    float f_2 = 0.f;
    float f_3 = 0.f;
    bool freq_HLD_flag = false;
    float s_1;                // Индикатор строковый KDI-572 №1
    float s_2;                // Индикатор строковый KDI-572 №2
    float s_3;                // Индикатор строковый KDI-572 №3
    QVector<BYTE> led = {0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12};
    bool  i_N1 = false;
    bool  i_H = false;
    bool  i_N2 = false;
    bool  i_HM = false;
    bool  i_RNV = false;
    bool  i_KT = false;
    bool  i_MIN = false;
};
/**
 * @brief The class PRIMA
 * Класс Радиостанция "Прима-КВ"
 *
 */
class PRIMA
{
public:
    QVector<BYTE> led_up_left = { 0x12,0x12,0x12,0x12,0x12 }; //экран с первой строкой 12 - ти разрядных
    QVector<BYTE> led_down_left = { 0x12,0x12,0x12,0x12,0x12 }; //экран со второй строкой 12-ти разрядных
    QVector<BYTE> led_up_right = { 0x12,0x12,0x12,0x12,0x12,0x12,0x12 }; //экран с первой строкой 12 - ти разрядных
    QVector<BYTE> led_down_right = { 0x12,0x12,0x12,0x12,0x12,0x12,0x12 }; //экран со второй строкой 12-ти разрядных

    bool on_psh = false;            //сдвоенный поворотный переключатель ГРОМК-ПШ-ОТКЛ
    bool str_smen_freq = false;     //кнопка ↕- для быстрой смены 

    bool on_prima = true;           // ручка ОТКЛ
    bool PrdU = false;              // ручка ПРДУ
    bool ZU = false;                // ручка ЗУ
    bool Msh = false;               // ручка Мщ
    bool NU = false;                // ручка НУ
    bool Ust = false;               // ручка УСТАН
    bool vvod = false;              // кнопка ВВОД
    bool vvod_pred = false;         // ручка ОТКЛ-
    quint8 freq_nabor = 0;          // ручка НАБОР дает +-1
};
/**
 * @brief The class UV_26
 * Класс пульта отстрела ловушек УВ-26
 *
 */
class UV_26
{
public:
    uint seria_ = 1;                // сколько серий при запуске программмы
    uint zalp_ = 1;                 // сколько ловушек в каждой серии
    uint seria_start_ = seria_;     // сколько серий при запуске программмы
    uint interval_ = 0;             // интервал между залпами
    uint left_zar = 64;
    uint right_zar = 64;
    quint32    tablo = 0;              // табло 3 символьное
    bool    i_left = false;         // индикатор левого борта
    bool    i_right = false;        // индикатор правого борта
    bool    t_left = false;         // кнопка выбора левого борта
    bool    t_right = false;        // кнопка выбора правого борта
    bool    t_prog = false;         // переключатель выбора режима програмирования
    bool    t_seria = false;        // задание количества серий
    bool    t_seria_pred = false;   // flag задание количества сери
    bool    t_zalp = false;         // задание количеста выстрелов в залпе
    bool    t_zalp_pred = false;    // flag задание количеста выстрелов в залпе
    bool    t_stop = false;         // остановка отстрела ловушек
    bool    t_interval = false;     // задание размера интервала между отстрелами ловишек в серии
    bool    t_interval_pred = false;//  flag задание размера интервала между отстрелами ловишек в серии
    bool    t_reset = false;        // сброс на 110 однократный сброс по одной ловушки по выбранным бортам
	bool    t_pusk = false;         // отстрел ловушек по заданой программе
    bool    t_pusk_pred = false;         // отстрел ловушек по заданой программе
};

/*
    Класс приборов ГМК
*/
class GMK
{
public:
    bool t_contr0 = false;        // Кнопка КОНТР 0
    bool t_contr300 = false;      // Кнопка КОНТР 300
};

/**
 * @brief The LeftPanelAZS class
 * Класс Левая панель АЗС
 *
 */
class LeftPanelAZS : public QObject {
    Q_OBJECT
public:
    bool i_weapon_chain = false;            // Светосигнальное табло ЦЕПИ ВООРУЖ ОТКЛ
    bool i_ab = false;                      // Светосигнальное табло АБ(желтая)
    bool i_explosion = false;               // Светосигнальное табло ВЗРЫВ(красная)
    bool i_pus_bd1 = false;                 // Светосигнальное табло ПУС ВЗВЕДН БД1 БД6(желтая)
    bool i_nps = false;                     // Светосигнальное табло НРС
    bool i_pus_bd2 = false;                 // Светосигнальное табло ПУС ВЗВЕДН БД2 БД5(желтая)
    bool i_upk = false;                     // Светосигнальное табло УПК
    bool i_bd1_load = false;                // Светосигнальное табло БД 1 ЗАГРУЖ(желтая)
    bool i_bd2_load = false;                // Светосигнальное табло БД 2 ЗАГРУЖ(желтая)
    bool i_bd3_load = false;                // Светосигнальное табло БД 3 ЗАГРУЖ(желтая)
    bool i_bd4_load = false;                // Светосигнальное табло БД 4 ЗАГРУЖ(желтая)
    bool i_bd5_load = false;                // Светосигнальное табло БД 5 ЗАГРУЖ(желтая)
    bool i_bd6_load = false;                // Светосигнальное табло БД 6 ЗАГРУЖ(желтая)
    bool t_pus_cocking = false;             // КНОПКА ВЗВЕДЕНИЕ ПУС
    bool t_main_switch_on = false;          // Тумблер ГЛАВНЫЙ ВЫКЛЮЧАТЕЛЬ ВКЛ
    bool t_main_switch_off = false;         // Тумблер ГЛАВНЫЙ ВЫКЛЮЧАТЕЛЬ ВЫКЛ
    bool t_explosion = false;               // Тумблер ВЗРЫВ - НЕВЗРЫВ
    bool t_emerg_dump = false;              // Тумблер АВАРИЙНЫЙ СБРОС БОМБ Б8 УПК
    bool t_sns_nav = false;                 // Тумблер СНС НАВ
    bool t_uvd_count = false;               // Тумблер СЧЕТЧИК УВД
    bool t_distmeter = false;               // Тумблер ДАЛЬНОМЕР
    bool t_sublight_pkv = false;            // Тумблер ПОДСВЕТ ПКВ
    bool t_power_onv_left = false;          // Тумблер ПИТАНИЕ ОНВ ЛЕВЫЙ
    bool t_power_onv_board = false;         // Тумблер ПИТАНИЕ ОНВ БОРТ - ТЕХНИК
    bool t_power_onv_right = false;         // Тумблер ПИТАНИЕ ОНВ ПРАВЫЙ
    float f_sublight_pkv = 0.f;             // КРУТИЛКА ПОДСВЕТ ПКВ
    bool i_upk_right = false;               // Лампа над правым УПК
    bool i_upk_left = false;                // Лампа над левым УПК
    float f_ammo_count_upk_right_1 = 0.f;   // Счетчик снарядов для правого УПК №1
    float f_ammo_count_upk_right_2 = 0.f;   // Счетчик снарядов для правого УПК №2
    float f_ammo_count_upk_left_1 = 0.f;    // Счетчик снарядов для левого УПК №1
    float f_ammo_count_upk_left_2 = 0.f;    // Счетчик снарядов для левого УПК №2
};

/**
 * @brief The LeftSidePAnelElectropult class
 * Класс Левая боковая панель электропульта
 *
 */
class LeftSidePanelElectropult : public QObject {
    Q_OBJECT
public:
    // 2.1 Контроль
    bool t_IV_500 = false;      // ИВ - 500Е
    bool t_ground = false;      // Кнопка ЗЕМЛЯ
    bool t_air = false;         // Кнопка ВОЗДУХ
    bool t_rt_left = false;     // Кнопка РТ ЛЕВОГО ДВИГАТЕЛЯ
    bool t_rt_rght = false;     // Кнопка РТ ПРАВОГО ДВИГАТЕЛЯ
    bool t_bur = false;         // БУР
    bool t_tbn = false;         // ТБН
// 2.2.Пульт управления ракетами ЭП - 662
    bool t_tumblr_1 = false;    // Тумблер 1
    bool t_button_1_1 = false;  // Кнопка 1
    bool t_button_1_2 = false;  // Кнопка 2
    bool t_button_1_3 = false;  // Кнопка 3
    bool t_button_1_4 = false;  // Кнопка 4
    bool t_tumblr_2 = false;    // Тумблер 2
    bool t_button_2_1 = false;  // Кнопка 1
    bool t_button_2_2 = false;  // Кнопка 2
    bool t_button_2_3 = false;  // Кнопка 3
    bool t_button_2_4 = false;  // Кнопка 4
// 2.3.Пульт дистанционного управления аппаратурой РИ - 65Б
    bool t_ri_off = false;      // Тумблер РИ ОТКЛ
    bool t_ri_check = false;    // Тумблер РИ ПРОВЕРКА    
    bool t_ri_rep = false;      // Тумблер РИ ПОВТОР
    bool t_ri_gain = false;     // Тумблер РИ УСИЛ
    bool i_hv_tral = false;     // Лампа ХВ ТРАЛ ОТКРЫТ
    bool i_ogr_nTr_Left = false;// Лампа ОГРnTr LEFT
    bool i_ogr_nTr_Right = false;// Лампа ОГРnTr RIGHT
    float f_p_air = 0.f;        // Манометр МВУ-10К, контролирующий давление воздуха в воздушной системе
    float f_p_brake = 0.f;      // Манометр МА-60К, контролирующий давление воздуха в тормозной системе колес шасси
    bool t_ano = false;         // Кнопка КОД-АНО
    bool t_siren = false;       // Кнопка СИРЕНА
    bool i_siren_on = false;    // Индикатор СИРЕНА ВКЛЮЧЕНА
    bool t_ext_lock = false;    // Тумблер ВНЕШНЯЯ ПОДВЕСКА
    bool i_cargo_dump = false;  // Индикатор ГРУЗ СБРОШЕН
    bool t_sublight_1 = false;  // Красный подсвет группа 1
    bool t_sublight_2 = false;  // Красный подсвет группа 2
    bool t_fan = false;         // Вентилятор
    bool t_cargo_dump = false;  // Тумблер аварийный сброс груза лебедка / внешняя подвеска
// 2.4.Пульт управления магнитофоном П503Б
    bool t_recorder = false;    // Тумблер вкл / выкл
    bool t_autostart = false;   // Тумблер автоспуск / непрерывная работа
    bool t_spu_lar = false;       // Тумблер СПУ / ЛАР
    float f_sublight = 0.f;     // Подсвет
    bool i_record = false;      // Индикатор ЗАПИСЬ up
// 2.5.Остальные элементы
    bool t_disasters = false;   // Тумблер бедств
    bool t_stiran = false;      // Тумблер стиран
    bool t_smoke = false;       // Тумблер не курить / не курить пристегнуть ремни
    bool t_belts = false;       // Тумблер пристегнуть ремни
    bool t_exit_tab = false;    // Тумблер табло выход
    bool t_ri = false;          // Тумблер речевой регистратор
    bool i_falut_rr = false;    // Лампа ОТКАЗ РР
    bool i_vkl_zapas = false;   // Лампа ВКЛЮЧИ ЗАПАСНОЙ
};

/**
 * @brief The LeftShildElectroPult class
 * Класс ЛЕВЫЙ ЩИТОК ЭЛЕКТРОПУЛЬТА
 *
 */
class LeftShildElectroPult : public QObject {
    Q_OBJECT
public:
    quint8 sw_glass_wiper = 0;  // Переключатель СТЕКЛООЧИСТИТЕЛЬ
    bool t_rv = false;          // Тумблер выключатель радиовысотомера
    bool t_spuu_52 = false;     // Тумблер Выключатель СПУУ-52
    bool t_plafon = false;      // Тумблер ПЛАФОН
    bool t_ag_left = false;     // Тумблер АГ ЛЕВ
    bool t_ag_res = false;      // Тумблер АГ РЕЗЕРВ
    bool t_bkk_18 = false;      // Тумблер БКК-18
    bool t_ri_65 = false;       // Тумблер РИ-65
    bool t_bkk_contr_up = false;   // Тумблер I КОНТР БКК
    bool t_bkk_contr_down = false; // Тумблер II КОНТР БКК
    bool t_pvd_heat = false;    // Кнопка КОНТРОЛЬ ОБОГРЕВА ПВД
    bool i_bkk_ok = false;      // Индикатор ИСПРАВН. БКК
    bool i_ri_65 = false;       // Индикатор ВКЛЮЧИ РИ-65
    bool i_pvd_heat_ok = false; // Индикатор ОБОГРЕВ ИСПРАВЕН
private:

private slots:
};

/**
 * @brief The SPU_Pult class
 * Класс Пульта управления связью
 *
 */
class SPU_Pult: public QObject {
    Q_OBJECT
public:
    float f_prosl = 0.f;        // крутилка ПРОСЛ
    float f_obsh = 0.f;         // кротилка Общпая
    bool t_ukr = false;         // Галетник УКР
    bool t_sr = false;          // Галетник СР
    bool t_kr = false;          // Галетник КР
    bool t_dr = false;          // Галетник ДР
    bool t_rk1 = false;         // Галетник РК1
    bool t_rk2 = false;         // Галетник РК2
    bool t_set_1 = false;       // Тумблер сеть 1
    bool t_set_2 = false;       // Тумблер сеть 2
    bool k_cv = false;          // Кнопка ЦВ
    bool t_spu = false;         // Тумблер СПУ
    bool t_radio = false;       // Тумблер РАДИО
    bool flag_on_spu = false;   // Флаг включения в индикаторах
    quint8 gal() {
        if (t_ukr)
            return 0;
        if (t_sr)
            return 1;
        if (t_kr)
            return 2;
        if (t_dr)
            return 3;
        if (t_rk1)
            return 4;
        if (t_rk2)
            return 5;
        return 0;
    }
private:

private slots:
};
/**
 * @brief The LeftPanelElectroPult class
 * Класс ЛЕВАЯ ПАНЕЛЬ ЭЛЕКТРОПУЛЬТА
 *
 */
class LeftPanelElectroPult : public QObject {
    Q_OBJECT
public:
    bool t_deicing_auto = false;        // Тумблер(ПРОТИВООБЛЕДЕНИТЕЛЬНАЯ СИСТЕМА) Автомат
    bool t_deicing_off = false;         // Кнопка(ПРОТИВООБЛЕДЕНИТЕЛЬНАЯ СИСТЕМА) ОТКЛ
    bool flag_deicing_off = false;         // Кнопка(ПРОТИВООБЛЕДЕНИТЕЛЬНАЯ СИСТЕМА) ОТКЛ
    bool t_deicing_left_pzu = false;    // Тумблер(ПРОТИВООБЛЕДЕНИТЕЛЬНАЯ СИСТЕМА) ДВИГ ПЗУ ЛЕВ
    bool t_deicing_right_pzu = false;   // Тумблер(ПРОТИВООБЛЕДЕНИТЕЛЬНАЯ СИСТЕМА) ДВИГ ПЗУ ПРАВ
    bool t_deicing_glass = false;       // Тумблер(ПРОТИВООБЛЕДЕНИТЕЛЬНАЯ СИСТЕМА) ОБОГРЕВ СТЕКОЛ

    quint8 sw_consumers = 0;    // Галетный Переключатель ТОК ПОТРЕБИТЕЛЕЙ
    bool t_A_offret = false;    // ВЫКЛ
    bool t_A_blade_1 = false;   // ЛОПАСТИ 1 - 5
    bool t_A_blade_2 = false;
    bool t_A_blade_3 = false;
    bool t_A_blade_4 = false;
    bool t_A_blade_5 = false;
    bool t_A_blade_tail = false;// ХВ
    bool t_A_glass = false;     // СТЕКЛА
    bool t_A_PZU_l = false;     // ПЗУ ПРАВ
    bool t_A_PZU_r = false;     // ПЗУ ЛЕВ
    quint8 get_consumers();

    //пульт проверки ПОС за правым пилотом
    bool i_imitation_hot = false;       // Индикатор обогрев
    bool i_imitation_cold = false;      // Индикатор обледенение
    bool t_imitation = false;           // Кнопка имитация под колпачком
    /*************************************/
    bool i_icing = false;               // Индикатор ОБЛЕДЕН.
    bool i_pos_on = false;              // Светосигнальное табло ПОС ВКЛЮЧЕНА
    bool i_fault_pos = false;           // Светосигнальное табло ОТКАЗ ПОС НВ
    bool i_heat_left = false;           // Светосигнальное табло ОБОГРЕВ ДВ.ЛЕВ
    bool i_heat_right = false;          // Светосигнальное табло ОБОГРЕВ ДВ.ПРАВ.
    bool i_left_pzu_front = false;      // Светосигнальное табло ЛЕВ.ПЗУ.ПЕРЕДН.
    bool i_right_pzu_front = false;     // Светосигнальное табло ПРАВ.ПЗУ.ПЕРЕДН
    bool i_left_pzu_back = false;       // Светосигнальное табло ЛЕВ.ПЗУ.ЗАДН.
    bool i_right_pzu_back = false;      // Светосигнальное табло ПРАВ.ПЗУ.ЗАДН
    bool i_so_121_prop = false;         // Светосигнальное табло СО - 121 ИСПРАВЕН
    bool i_flight = false;              // Светосигнальное табло ПОЛЕТ
    bool i_parking = false;             // Светосигнальное табло СТОЯНКА
    float f_ampermetr = false;          // Амперметр - Стрелка
    const std::array<float, 10> amper = { 0,70,70,70,70,70,140,60,70,70 };  // токи потребления системы ПОС
    bool i_section_1 = false;           // Светосигнальное табло 1 СЕКЦИЯ
    bool i_section_2 = false;           // Светосигнальное табло 2 СЕКЦИЯ
    bool i_section_3 = false;           // Светосигнальное табло 3 СЕКЦИЯ
    bool i_section_4 = false;           // Светосигнальное табло 4 СЕКЦИЯ
    qint32 freq = 0;                      // табло радиостанции
    qint32 left_freq = 122;            // табло радиостанции MГц
    qint32 right_freq = 648;            // табло радиостанции MГц
    float f_radio_baklan20_1 = 0.f;   // Вращалка Радиостанция Баклан 20 №1
    float f_radio_baklan20_2 = 0.f;   // Вращалка Радиостанция Баклан 20 №2
    qint32 e_radio_baklan20_1 = 0;   // Вращалка Радиостанция Баклан 20 №1
    qint32 e_radio_baklan20_2 = 0;   // Вращалка Радиостанция Баклан 20 №2
    bool t_radio_baklan20_1 = false;    // Тубмлер Радиостанция Баклан 20 №1
    bool t_radio_baklan20_2 = false;    // Тубмлер Радиостанция Баклан 20 №2
    bool t_bark_eng_left_on = true;    // Тумблер БАРК ДВИГ ЛЕВАЯ Питание
    bool t_bark_eng_left_wep = false;   // Тумблер БАРК ДВИГ ЛЕВАЯ ЧР
    bool t_bark_eng_left_ct1 = false;   // Тумблер БАРК ДВИГ ЛЕВАЯ СТ1
    bool t_bark_eng_left_ct2 = false;   // Тумблер БАРК ДВИГ ЛЕВАЯ СТ2
    bool t_bark_eng_tg_left = false;    // Тумблер БАРК ДВИГ ТГ левый
    bool t_bark_eng_tg_right = false;   // Тумблер БАРК ДВИГ ТГ правый
    bool t_bark_eng_tk_left = false;    // Тумблер БАРК ДВИГ ТК левая
    bool t_bark_eng_tk_right = false;   // Тумблер БАРК ДВИГ ТК првая
    bool t_bark_eng_right_ct1 = false;  // Тумблер БАРК ДВИГ ПРАВАЯ СТ1
    bool t_bark_eng_right_ct2 = false;  // Тумблер БАРК ДВИГ ПРАВАЯ СТ2
    bool t_bark_eng_right_wep = false;// Тумблер БАРК ДВИГ ПРАВАЯ ЧР
    bool t_bark_eng_right_on = true;// Тумблер БАРК ДВИГ ПРАВАЯ ПИТАНИЕ
private:

private slots:
};

/**
 * @brief The PultControlDISS class
 * Класс Пульт Контроля ДИСС
 *
 */
class PultControlDISS : public QObject {
    Q_OBJECT
public:
    //DISS-32//
    bool t_contr = false;     
    bool t_poisk = false;
    bool t_contr_flag = false;
    bool t_poisk_flag = false;
    bool t_contr_pred = false;
    bool t_poisk_pred = false;
    bool t_up_left = false;
    bool t_back_right = false;
    bool t_spedd127 = false;
    bool t_speed258 = false;
    bool i_vss = false;
    bool i_vch = false;
    bool i_bvk = false;
    bool i_flight = false;
    bool i_ispr = false;
    bool i_search = false;
    /***************/
    bool i_m = false; // Индикатор М
    bool i_contr = false; // Индикатор КОНТР
    bool i_work = false; // Индикатор РАБОТА
    bool i_b = false; // Индикатор В
    quint8 sw_mode = 0; // Галетный Переключатель
    bool t_p1 = false;
    bool t_p2 = false;
    bool t_p3 = false;
    bool t_p4 = false;
    bool t_p5 = false;
    quint8  get_sw_mode();
private:

private slots:
};

/**
 * @brief The LeftPilotBoard class
 * Класс отвечает за левую доску пилота вертолета
 * Compute() это автомат работы
 */
class LeftPilotBoard : public QObject
{
    Q_OBJECT
public:
    bool t_rulejka = false;    // Тумблер РУЛЕЖ
    bool t_svet_ubran = false;    // Тумблер СВЕТ
    bool t_svet = false;    // Тумблер СВЕТ
    bool t_fpp = false;    // Тумблер УПРАВЛ ФПП
    bool t_project = false;    // Тумблер УПРАВЛ ПРОЖЕКТ
    quint8 sw_PVD = 0;        // Переключатель СТАТИЧЕСКАЯ СИСТЕМА ПВД
    bool t_pvd_left = false;    // ПВД ЛЕВАЯ
	bool t_pvd_unitd = true;    // ПВД ОБЪЕДИНЕННАЯ
    bool t_pvd_right = false;    // ПВЖ ПРАВАЯ
    bool t_roll_33 = false;    // предел крена
    bool t_apk_sv = false;    // Тумблер АРК СВ
    bool t_apk_ukv = false;    // Тумблер АРК УКВ
    bool t_max_roll = false;    // Тумблер ПРЕДЕЛЬН КРЕН
    bool t_dnmr = false;        // тумблер НАВ/ДНМР

    RV_5    rv_5;       // Указатель УВ-5М радиовысотомера РВ-5
    UT_6K ut_6k;        // Указатель 2УТ-6К температуры газов
    UC_450K uc_450k;    // Спидометр-стрелка Указатель скорости УС-450К
    VD_10VK vd_10vk;    // Высотомер ВД-10ВК
    UP_21_15 up_21_15;  // Шаг винта в градусах стрелка Индикатор УП-21-15 указателя шага несущего винта
    IR_117 ir_117;      // Указатель режимов ИР-117
    AVG avg_left;       // авиагоризонт левый
    UGR_4UK ugr_4uk;    // Прибор УГР-4
    SS ss;              // Индикатор висения и малых скоростей аппаратуры ДИСС-15
    VR_30MK vr_30mk;    // Подъем-спуск-Стрелка Вариометр ВР-30МК
    ITE_1T ite_1t;      // Несущий винт обороты - стрелка Указатель ИТЭ-1Т числа оборотов НВ
    AVG avg_res;        // авиагоризонт резервный
    ITE_2T ite_2t;      // Двухстрелочный указатель ИТЭ-2Т числа оборотов двигателя ТВЗ-117ВМ
    KI_204 ki_206;      // Внешний индикатор КI-206
    SPU_Pult spu;


    bool i_warning_earth = false;       // Индикатор ВНИМЕНИЕ ЗЕМЛЯ
    bool i_danger_earth = false;        // Индикатор ОПАСНО ЗЕМЛЯ
    bool i_roll_max = false;            // Индикатор КРЕН ВЕЛИК
    bool t_cso1_yellow = false;         // Кнопка ЦСО1 желтый
    bool t_cso1_red = false;            // Кнопка ЦСО1 красный   
    bool i_cso1_yellow = false;         // Индикатор ЦСО1 желтый
    bool i_cso1_red = false;            // Индикатор ЦСО1 красный   
    bool i_ag_net_kontr = false;        // Индикатор НЕТ КОНТР АГ
    bool i_ag_left_brok = false;        // Индикатор ЛЕВ АГ РЕЗЕРВ
    bool i_ag_res_brok = false;         // Индикатор ОТКАЗ АГ РЕЗЕРВ

    bool i_main_gear_p_oil = false;     // Индикатор МАЛО РМ ГЛ РЕДУК
    bool i_main_gear_swarf = false;     // Индикатор СТРУЖКА ГЛ. РЕДУК.
    bool i_inter_gear_swarf = false;    // Индикатор СТРУЖКА ПРОМ. РЕД.
    bool i_tail_gear_swarf = false;     // Индикатор СТРУЖКА ХВ. РЕДУК.
    bool i_eng_left_vibr_dang = false;  // Индикатор ВИБР. ОПАС. ЛЕВ. ДВИГ.
    bool i_eng_left_swarf = false;      // Индикатор СТРУЖКА ЛЕВ.ДВИГ.
    bool i_eng_left_p_oil = false;      // Индикатор МАЛО. РМ. ЛЕВ. ДВИГ.
    bool i_eng_left_emrg = false;       // Индикатор Ч.Р.ЛЕВ.ДВИГ.
    bool i_eng_left_tf = false;         // Индикатор ЗАСОР. ТФ. ЛЕВ. ДВИГ.
    bool i_eng_left_vibr_max = false;   // Индикатор ВИБР. ПОВ. ЛЕВ. ДВИГ.
    bool i_eng_left_max_rpm = false;    // Индикатор Превышение оборотов свободной турбины левого двигателя.
    bool i_eng_left_erd_off = false;    // Индикатор ОТКЛ. ЭРД. ЛЕВ. ДВИГ.
    bool i_nv_max = false;              // Индикатор п.Н.В ВЫСОКИЕ
    bool i_eng_right_vibr_dang = false; // Индикатор ВИБР ОПАС ЛЕВ ДВИГ
    bool i_eng_right_swarf = false;     // Индикатор СТРУЖКА ПРАВ.ДВИГ.
    bool i_eng_right_p_oil = false;     // Индикатор МАЛО РМ. ПРАВ. ДВИГ.
    bool i_eng_right_emrg = false;      // Индикатор Ч.Р.ПРАВ.ДВИГ.
    bool i_nv_min = false;              // Индикатор п.Н.В НИЗКИЕ
    bool i_eng_right_tf = false;        // Индикатор ЗАСОР. ТФ. ПРАВ. ДВИГ.
    bool i_eng_right_vibr_max = false;  // Индикатор ВИБР. ПОВ. ПРАВ. ДВИГ.
    bool i_eng_right_max_rpm = false;   // Индикатор Превышение оборотов свободной турбины правого двигателя
    bool i_eng_right_erd_off = false;   // Индикатор ОТКЛ. ЭРД. ПРАВ. ДВИГ.
    bool i_eng_left_limit = false;      // Индикатор ЛЕВ. ДВИГ. ПРЕДЕЛ
    bool i_eng_right_limit = false;     // Индикатор ПРАВ. ДВИГ. ПРЕДЕЛ

    quint8  get_sw_pvd();
private:

private slots:
};

/***************************************************************************/
/**
 * @brief The CentralElectricPult class
 * Класс отвечает за средняя панель электропульта (запуск двигателей) вертолета
 * Compute() это автомат работы
 */
class CentralElectricPult : public QObject
{
    Q_OBJECT
public:
	void StartAll( );
    //    void  Compute(float dT) {};
    bool  i_eng_left_fire = false;      // Индикатор ПОЖАР ЛЕВ ДВ
    bool  i_eng_left_1_queue = false;   // Индикатор ЛЕВ ДВ I ОЧЕРЕДЬ
    bool  i_eng_left_2_queue = false;   // Индикатор ЛЕВ ДВ II ОЧЕРЕДЬ
    bool  t_eng_left_1_queue = false;   // Кнопка I ОЧЕРЕДЬ ЛЕВ ДВ
    bool  t_eng_left_2_queue = false;   // Кнопка II ОЧЕРЕДЬ ЛЕВ ДВ
    bool  i_eng_rght_fire = false;      // Индикатор ПОЖАР ПРАВ ДВ
    bool  i_eng_rght_1_queue = false;   // Индикатор ПРАВ ДВ I ОЧЕРЕДЬ
    bool  i_eng_rght_2_queue = false;   // Индикатор ПРАВ ДВ II ОЧЕРЕДЬ
    bool  t_eng_rght_1_queue = false;   // Кнопка I ОЧЕРЕДЬ ПРАВ ДВ
    bool  t_eng_rght_2_queue = false;   // Кнопка II ОЧЕРЕДЬ ПРАВ ДВ
    bool  i_ko_50_fire = false;         // Индикатор ПОЖАР КО-50
    bool  i_ko_50_1_queue = false;      // Индикатор КО-50 I ОЧЕРЕДЬ
    bool  i_ko_50_2_queue = false;      // Индикатор КО-50 II ОЧЕРЕДЬ
    bool  t_ko_50_1_queue = false;      // Кнопка I ОЧЕРЕДЬ КО-50
    bool  t_ko_50_2_queue = false;      // Кнопка II ОЧЕРЕДЬ КО-50
    bool  i_red_vsu_fire = false;       // Индикатор ПОЖАР РЕД. ВСУ
    bool  i_red_vsu_1_queue = false;    // Индикатор РЕД ВСУ I ОЧЕРЕДЬ
    bool  i_red_vsu_2_queue = false;    // Индикатор РЕД ВСУ II ОЧЕРЕДЬ
    bool  t_red_vsu_1_queue = false;    // Кнопка I ОЧЕРЕДЬ РЕД ВСУ
    bool  t_red_vsu_2_queue = false;    // Кнопка II ОЧЕРЕДЬ РЕД ВСУ
    bool  t_contr_sensors = false;      // Тумблер КОНТРОЛЬ ДАТЧИКОВ
    bool  t_firefighting = false;       // Тумблер ОГНЕТУШЕНИЕ
    bool  t_1 = false;                  // Тумблер КОНТРОЛЬ ПИРОПАТРОНОВ
    bool  t_2 = false;                  // Тумблер II
    quint8 sw_contr_sensors = 0;        // Переключатель КОНТРОЛЬ ДАТЧИКОВ
    bool  t_contr_sensors_0 = false;
    bool  t_contr_sensors_1 = false;
    bool  t_contr_sensors_2 = false;
    bool  t_contr_sensors_3 = false;
    bool  t_contr_sensors_4 = false;
    bool  t_contr_sensors_5 = false;
    bool  t_contr_sensors_6 = false;
    bool  t_contr_sensors_7 = false;
    bool  i_contr_sensors = false;      // Индикатор КОНТРОЛЬ ДАТЧИКОВ
    float f_p_hidro_main = 0.f;         // основная гидросистема
    float f_p_hidro_second = 0.f;       // дублирующая гидросистема
    bool t_second_hidro = false;        // Тумблер ДУБЛИР
    bool  t_off_second_hidro = false;   // Кнопка ОТКЛ ДУБЛИР
	bool  t_hidro_main = true;       //  Тумблер ГИДРОСИСТЕМА ОСНОВН
    bool  i_hidro_main = false;         // Индикатор ОСНОВН ВКЛЮЧЕНА
	bool  t_hidro_second = true;     //  Тумблер ГИДРОСИСТЕМА ДУБЛИР
    bool  i_hidro_second = false;       // Индикатор ДУБЛИР ВКЛЮЧЕНА

    bool  t_ta_sel_false_start = false; // Тумблер ТА ПРОКРУТ ЛОЖН ЗАПУСК
    bool  t_ta_sel_scroll = false;      // Тумблер ТА ПРОКРУТ 
    bool  t_ta_sel_start = false;       // Тумблер ТА ПРОКРУТ ЗАПУСК
    bool  t_ta_start = false;           // Кнопка ЗАПУСК ТУРБОАГРЕГАТА
    bool  i_ta_working = false;         // Индикатор ВСУ РАБОТАЕТ
    bool  i_ta_automat_on = false;      // Индикатор АВТОМАТ ВКЛЮЧЕН
    bool  t_ta_stop = false;            // Кнопка ВЫКЛ АИ-9В
    bool  i_p_oil_norm = false;         // Индикатор ДАВ. МАСЛА НОРМА аи9
    bool  i_rpm_norm = false;           // Индикатор ОБОРОТЫ НОРМА аи9
    bool  i_rpm_max = false;            // Индикатор ОБОРОТЫ ПРЕДЕЛ аи9
    bool  i_p_oil_warn = false;         // Индикатор МАСЛО сафир
    bool  i_TF = false;                 // Индикатор Топливный фильтр
    bool  i_ta_brok = false;            // Индикатор ВСУ отказ
    bool  t_eng_start = false;          // Кнопка ЗАПУСК ДВИГАТЕЛЕЙ
    bool  i_eng_automat_on = false;     // Индикатор АВТОМАТ ВКЛЮЧЕН
    bool  t_eng_stop = false;           // Кнопка ПРЕКРАЩ ЗАПУСКА ДВИГАТЕЛЕЙ
    bool  t_eng_left = false;           // Тумблер ЗАПУСК ДВИГАТЕЛЕЙ ЛЕВ
    bool  t_eng_rght = false;           // Тумблер ЗАПУСК ДВИГАТЕЛЕЙ ПРАВ
    bool  t_eng_sel_start = false;      // Тумблер ДВ положении запуск
    bool  t_eng_sel_scroll = false;     // Тумблер ДВ в положении ПРОКРУТ
    bool  i_ta_work = false;            // Индикатор СТАРТЕР РАБОТАЕТ
    bool  t_eng_left_ign_check = false; // Тумблер ПРОВЕРКА ЗАЖИГ ЛЕВ
    bool  t_eng_right_ign_check = false;// Тумблер ПРОВЕРКА ЗАЖИГ ПРАВ
    bool flag_eng1 = false;
    bool flag_eng2 = false; 
    float f_t_ta = 0.f;                 // Шкала температуры компрессора ТА
    float f_p_ta = 0.f;                 // Шкала давления компрессора ТА
	bool  t_fuel_left_close = true;     //  Тумблер ПЕРЕКР ЛЕВ ТОПЛИВНЫЙ КРАН
	bool  t_fuel_right_close = true;    //  Тумблер ПЕРЕКР ПРАВ ТОПЛИВНЫЙ КРАН
	bool  t_fuel_bypass_close = true;   //  Тумблер ПЕРЕКР ОБХОДНОЙ БАКОВ
	bool  t_fuel_perepusk = true;       //  Тумблер ПЕРЕПУСК
	bool  i_fuel_left_close = true;     //  Индикатор ПЕРЕКР ЛЕВ ТОПЛИВНЫЙ КРАН
	bool  i_fuel_right_close = true;    //  Индикатор ПЕРЕКР ПРАВ ТОПЛИВНЫЙ КРАН
	bool  i_fuel_bypass_close = true;   //  Индикатор ПЕРЕКР ОБХОДНОЙ БАКОВ
	bool  t_fpump_refil = true;         //  Тумблер НАСОС ЗАПРАВ
    bool  t_refil = false;              //Тумблер АИ-9В  КОНТР true=низ
	bool  t_fpump_contr = true;         //  Тумблер НАСОС КОНТР
	bool  t_fpump_consum = true;        //  Тумблер АИ-9В НАСОС РАСХОД
	bool  t_fpump_left = true;          //  Тумблер АИ-9В НАСОС ЛЕВЫЙ
	bool  t_fpump_right = true;         //  Тумблер АИ-9В НАСОС ПРАВЫЙ
	bool  i_fpump_consum_off = true;    //  Индикатор НАСОС РАСХОД НЕ РАБОТ
	bool  i_fpump_left_off = true;      //  Индикатор НАСОС ЛЕВЫЙ НЕ РАБОТ
	bool  i_fpump_right_off = true;     //  Индикатор НАСОС ПРАВЫЙ НЕ РАБОТ

    quint8  get_sw_contr_sensors(); //  Переключатель КОНТРОЛЬ ДАТЧИКОВ
    void    clr_sw_contr_sensors(); // сброс переменных
    void    get_sw_contr_sensors(int val); // сброс переменных
private:
private slots:
};



/********************************************************************************************
 * @brief The CentralControlPanel class
 * Класс отвечает за центральный пульт управления вертолета
 * Compute() это автомат работы
 ********************************************************************************************/
class CentralControlPanel : public QObject
{
    Q_OBJECT
public:
    float f_t_oil_inter_gb = 0.f;   // Шкала T МАСЛА ПРОМЕЖУТОЧНОГО РЕДУКТОРА
    float f_t_oil_tail_gb = 0.f;    // Шкала T МАСЛА В ХВОСТОВОМ РЕДУКТОРЕ
    float f_p_oil_main_gb = 0.f;    // Шкала P МАСЛА ГЛ.РЕДУК.
    float f_t_oil_main_gb = 0.f;    // Шкала T МАСЛА ГЛ.РЕДУК.
    float f_p_oil_left_eng = 0.f;   // Шкала ЛЕВЫЙ ДВИГАТЕЛЬ ДАВЛЕНИЕ
    float f_t_oil_left_eng = 0.f;   // Шкала ЛЕВЫЙ ДВИГАТЕЛЬ T МАСЛА
    float f_p_oil_rght_eng = 0.f;   // Шкала ПРАВЫЙ ДВИГАТЕЛЬ ДАВЛЕНИЕ
    float f_t_oil_rght_eng = 0.f;   // Шкала ПРАВЫЙ ДВИГАТЕЛЬ T МАСЛА
    bool  t_searchlight = false;    // Тумблер ПРОЖЕКТОР
    bool  t_ignition = false;       // Кнопка ПОДЖИГ
    bool  t_focus = false;          // Кнопка ФОКУС
    float f_1 = 0.f;                // Вращалка 1
    float f_2 = 0.f;                // Вращалка 2
    float f_3 = 0.f;                // Вращалка 3
    float f_get_curs = 0.f;         // Планка НАПРАВЛЕНИЕ
    float f_get_curs_math = 0.f;         // Планка НАПРАВЛЕНИЕ предполетная проверка
    float f_get_roll = 0.f;         // Планка КРЕН
    float f_get_pitch = 0.f;        // Планка ТАНГАЖ
    float f_get_alt = 0.f;          // Планка ВЫСОТА
    float f_spu52_ind = 0.f;        // Шкала СПУ52
    bool  t_spu52_off = false;      // Кнопка КОНТРОЛЬ ОТКЛ
    bool  i_spu52_off = false;      // индикатор КОНТРОЛЬ ОТКЛ
    float f_spu52 = 0.f;            // Вращалка КОНТРОЛЬ
    QVector<float> f_spu52_pot = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }; // Среднее для POT МГц
    bool  t_spu52_p = false;        // Тумблер КОНТРОЛЬ P
    bool  t_spu52_t = false;        // Тумблер КОНТРОЛЬ t
    bool  t_glass_wash = false;     // Тумблер ОМЫВ СТЕКОЛ
    bool  t_cmd_zu = false;         // Тумблер КОМАНД РС ЗУ
    bool  t_cmd_nu = false;         // Тубмлер КОМАНД РС НУ
    bool  t_cmd_psh = false;        // Тумблер ПШ
    float f_set_curs = 0.f;         // АВТОПИЛОТ - Вращалка НАПРАВЛЕНИЕ
    float f_set_roll = 0.f;         // АВТОПИЛОТ - Вращалка КРЕН
    float f_set_pitch = 0.f;        // АВТОПИЛОТ - Вращалка ТАНГАЖ
    float pred_set_curs = 0.f;         // АВТОПИЛОТ - Вращалка НАПРАВЛЕНИЕ
    float pred_set_roll = 0.f;         // АВТОПИЛОТ - Вращалка КРЕН
    float pred_set_pitch = 0.f;        // АВТОПИЛОТ - Вращалка ТАНГАЖ
    int res_f_set_curs = 0;         // АВТОПИЛОТ - Вращалка НАПРАВЛЕНИЕ
    int res_f_set_roll = 0;         // АВТОПИЛОТ - Вращалка КРЕН
    int res_f_set_pitch = 0;        // АВТОПИЛОТ - Вращалка ТАНГАЖ
    bool res_flag_curs = false;         // АВТОПИЛОТ - Вращалка НАПРАВЛЕНИЕ
    bool res_flag_roll = false;         // АВТОПИЛОТ - Вращалка КРЕН
    bool res_flag_pitch = false;        // АВТОПИЛОТ - Вращалка ТАНГАЖ
    bool  t_set_curs = false;       // АВТОПИЛОТ - Кнопка НАПРАВЛЕНИЕ
    bool  t_set_roll = false;       // АВТОПИЛОТ - Кнопка КРЕН
    bool  t_set_pitch = false;      // АВТОПИЛОТ - Кнопка ТАНГАЖ
    //bool  t_ap_alt_up = false;      // АВТОПИЛОТ - Тумблер Контроль
    //bool  t_ap_alt_down = false;    // АВТОПИЛОТ - Тумблер Контроль
    bool  t_ap_dir_off = false;     // АВТОПИЛОТ - Кнопка НАПРАВЛЕНИЕ ОТКЛ
    bool  t_ap_dir_on = false;      // АВТОПИЛОТ - Кнопка НАПРАВЛЕНИЕ ВКЛ
    bool  t_ap_on = false;          // АВТОПИЛОТ - Кнопка СРЕДНЯЯ ВКЛ
    bool  t_ap_alt_on = false;      // АВТОПИЛОТ - Кнопка ВЫСОТА ВКЛ
    bool  t_ap_alt_off = false;     // АВТОПИЛОТ - Кнопка ВЫСОТА ОТКЛ
    bool  t_ap_alt_up = false;      // АВТОПИЛОТ - Кнопка ВЫСОТА UP
    bool  t_ap_alt_down = false;    // АВТОПИЛОТ - Кнопка ВЫСОТА DOWN
    bool  t_ap_speed = false;       // АВТОПИЛОТ - Тумблер СКОРОСТЬ 
    bool  i_ap_dir_off = false;     // АВТОПИЛОТ - индикатор НАПРАВЛЕНИЕ ОТКЛ
    bool  i_ap_dir_on = false;      // АВТОПИЛОТ - индикатор НАПРАВЛЕНИЕ ВКЛ
    bool  i_ap_on = false;          // АВТОПИЛОТ - индикатор СРЕДНЯЯ ВКЛ
    bool  i_ap_alt_on = false;      // АВТОПИЛОТ - индикатор ВЫСОТА ВКЛ
    bool  i_ap_alt_off = false;     // АВТОПИЛОТ - индикатор ВЫСОТА ОТКЛ
    bool  i_ap_speed = false;       // АВТОПИЛОТ - индикатор СКОРОСТЬ 
    bool  t_trans_main = false;     // Тумблер ТРАНСФ ДИМ ОСНОВ
    bool  t_trans_reserv = false;   // Тубмлер ТРАНСФ ДИМ ЗАПАСН
    bool  t_1 = false;              // Кнопка АП - РК - ПШ
    bool  t_ap = false;             // Тумблер АП
    bool  t_rk = false;             // Тумблер РК
    bool  t_psh = false;            // Тумблер ПШ
    float f_ap_rk_psh = 0.f;        // Вращалка АП - РК - ПШ
    float f_chanel = 0.f;           // КРУТИЛКА КАНАЛ
    bool  i_apparel_up = false;     // Индикатор АППАРЕЛЬ УБРАНА
    bool  i_apparel_down = false;   // Индикатор АППАРЕЛЬ ВЫПУЩЕНА

    bool t_main_off_ab = false;     // Тумблер ГЛАВЫЙ ВЫКЛ АБ
    bool t_emerg_drop = false;      // Тумблер АВРИЙНЫЙ СБРОС
    bool t_explosion = false;       // Тумблер ВЗРЫВ
    bool t_po1 = false;             // Тумблер ПО1
    bool t_po2 = false;             // Тумблер ПО2
    bool i_ab = false;              // Лампа АБ
    bool i_explosion = false;       // Лампа 
    bool t_cargo_cabin = false;     // Тумблер ГРУЗ КАБИНА
    bool t_crew = false;            // Тумблер ЭКИПАЖ
    bool t_day = false;             // Тумблер ДЕНЬ
    bool t_night = false;           // Тумблер НОЧЬ
    bool i_power = false;           // Лампа ПИТАНИЕ
    bool i_brace = false;           // Лампа РАСЧАЛКА
    bool i_removed = false;         // Лампа УБРАНА
    bool i_work = false;            // Лампа РАБОТА
    bool i_dropped = false;         // Лампа ВЫПУЩЕНА

    bool t_sublight_ki_13 = false;  // Тумблер ПОДСВЕТ КИ-13
    bool t_sublight_azs = false;    // Тумблер ПОДСВЕТ ПАНЕЛИ АЗС

    bool t_zbn_on = false;      // Тумблер ЗБН ВКЛ
    bool t_bur_on = false;      // Тумблер БУР ВКЛ
    bool t_kontr_lamp = false;  // Кнопка КОНТР ЛАМП
    bool i_otkaz_bur = false;   // Индикатор ОТКАЗ БУР-1
    bool i_otkaz_en = false;    // Индикатор ОТКАЗ ЭН
    bool t_day_night = false;   // Тумблер ДЕНЬ-НОЧЬ
    bool t_kontr_vvoda = false; // Кнопка КОНТР ВВОДА
    bool t_vvod = false;        // Кнопка ВВОД
    bool t_vvod_pred = false;      // Кнопка ОД предыдущ состояние
    bool t_sdvig = false;       // Кнопка СДВИГ
    bool t_cbros = false;       // Кнопка СБРОС
    bool t_ust = false;         // Кнопка УСТ
    bool t_ust_pred = false;      // Кнопка ОД предыдущ состояние
    bool t_od = false;          // Кнопка ОД
    bool t_od_pred = false;      // Кнопка ОД предыдущ состояние    
    QVector<QVector<BYTE>> bur = { {0x1,0x2,0x1,0x2,0x1,0x2, 0x0, 0x0},
                                    {0x1,0x2,0x1,0x2,0x1,0x2, 0x1, 0x1},
                                    {0x12,0x12,0x1,0x3,0x1,0x3, 0x2, 0x2},
                                    {0x12,0x12,0x12,0x3,0x1,0x3, 0x3, 0x3},
                                    {0x12,0x12,0x12,0x3,0x1,0x3, 0x4, 0x4} }; // Создаем QVector размером 8 на 5 и заполняем его нулями
    //Helicopter->ccp.current_time = std::time(nullptr);
    //Helicopter->ccp.local_time = std::localtime(&Helicopter->ccp.current_time);
    qint32 save_time = 0;
    qint32 save_date = 0;
    qint32 save_reis = 0;
    qint32 save_centr = 0;
    qint32 save_mass = 1300; 
    //std::time_t current_time;
    //std::tm* local_time;
    QVector<BYTE> led_bur = { 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8 };
    int shet_t_od = 0;          // контроль нажатий на код ОД
    int time_on = 0;            // контроль времени вкл
    qint32 led_left = 0xffff;        // Левое табло БУР 6 значений
    qint8 led_OD = 0;            // Правое табло кода БУР 1 значение
    qint8 led_UST = 0;            // значение в Led_vvod тоесть значение на одном из digits
    qint8 led_VVOD = 0;            // какой digits используется

    bool i_EL = false;  // ОТКАЗ ЭЛЕКТРОНИКИ 
    bool i_NO = false;  // ПРЕВЫШЕНИЕ max n
    bool i_VT = false;  // ПРЕВЫШЕНИЕ t С ВГ
    bool i_PO = false;  // n МАЛЫ
    bool i_TZ = false;  // t c ВГ МАЛА
    bool i_TO = false;  // Р МАСЛА МАЛО
    bool i_SG = false;  // УРОВЕНЬ СИГНАЛА ДАТЧИКА N МАЛ
    bool i_OV = false;  // ОТКАЗ ТОПЛ. СЕРВОКЛ.
    bool i_ZV = false;  // ОТКАЗ 1-ГО ДАТЧИКА N
    bool t_control = false; // Кнопка КОНТРОЛЬ
    float f_get_roll_grad();         // Планка КРЕН

    qint32 KT76_LCD = 0; // ТАбло прибора КТ76
    qint32 KT76_BORT = 0; // ТАбло прибора КТ76
    //qint8 KT76_RAZRYAD = 0;
    bool KT76_t_IDT = false;
    bool KT76_t_0 = false;
    bool KT76_t_1 = false;
    bool KT76_t_2 = false;
    bool KT76_t_3 = false;
    bool KT76_t_4 = false;
    bool KT76_t_5 = false;
    bool KT76_t_6 = false;
    bool KT76_t_7 = false;
    bool KT76_t_CLR = false;
    bool KT76_t_VFR = false;
    bool KT76_gal_OFF = true;
    bool KT76_gal_SBY = false;
    bool KT76_gal_TST = false;
    bool KT76_gal_ON = false;
    bool KT76_gal_ALT = false;
    quint8 kt76_gal() {
        if (KT76_gal_OFF) return 0;
        if (KT76_gal_SBY) return 1;
        if (KT76_gal_TST) return 2;
        if (KT76_gal_ON ) return 3;
        if (KT76_gal_ALT) return 4;
        return 0;
    }
    bool KT76_i_FL = false;
    bool KT76_i_ALT = false;
    bool KT76_i_ON = false;
    bool KT76_i_R = false;
    bool KT76_i_SBY = false;
    bool KT76_t_0_pred = false;
    bool KT76_t_1_pred = false;
    bool KT76_t_2_pred = false;
    bool KT76_t_3_pred = false;
    bool KT76_t_4_pred = false;
    bool KT76_t_5_pred = false;
    bool KT76_t_6_pred = false;
    bool KT76_t_7_pred = false;

    qint32 KN53_LCD_leftM = 11; // ТАбло прибора КН53 2,3,4,5 цифра 8-17 * 10000
    qint32 KN53_LCD_leftK = 70; // ТАбло прибора КН53 2,3,4,5 цифра 0-95 * 100
    qint32 KN53_LCD_rightM = 10; // ТАбло прибора КН53 7,8,9,10 цифра 8-11 
    qint32 KN53_LCD_rightK = 50; // ТАбло прибора КН53 7,8,9,10 цифра 0-95 
    qint32 KN53_LCD = 0; // ТАбло прибора 
    qint32 KN53_LCD_16 = 0; // ТАбло прибора КН53 1 и 6 цифра

    bool KN53_pozuv = false;
    bool KN53_pozuv_110 = false;
    bool KN53_pozuv_111 = false;
    bool KN53_pozuv_113 = false;
    bool KN53_str = false;      // кнопка стрелки
    bool KN53_str_pred = false;
    size_t KN53_str_freq = 1;
    bool KN53_on = false;       // вращалка включения
    qint32 KN53_freqM = false;            // вращалка настройки частот
    qint32 KN53_freqK = false;            // вращалка настройки частот
};

// Центральный Пульт Управления
class CentralPultContr : public QObject
{
    Q_OBJECT
public:
    bool t_msg = false;         // Кнопка MSG
    bool t_obs = false;         // Кнопка OBS
    bool t_alt = false;         // Кнопка ALT
    bool t_nrst = false;        // Кнопка NPRST
    bool t_d = false;           // Кнопка D
    bool t_clr = false;         // Кнопка CLR
    bool t_ent = false;         // Кнопка ENT
    float f_kt74_left = 0.f;    // Вращалка КТ-74 левая
    float f_kt74_right = 0.f;   // Вращалка КТ-74 правая
    bool i_kn62_use = false;    // Индикатор KN-62 USE
    bool i_kn62_stby = false;   // Индикатор KN-62 STBY
    bool t_kt74_ident = false;  // KT-74 Кнопка IDENT
    bool t_kt74_vfr = false;    // KT-74 Кнопка VFR
    bool t_kt74_0 = false;      // KT-74 Кнопка 0
    bool t_kt74_1 = false;      // KT-74 Кнопка 1
    bool t_kt74_2 = false;      // KT-74 Кнопка 2
    bool t_kt74_3 = false;      // KT-74 Кнопка 3
    bool t_kt74_4 = false;      // KT-74 Кнопка 4
    bool t_kt74_5 = false;      // KT-74 Кнопка 5
    bool t_kt74_6 = false;      // KT-74 Кнопка 6
    bool t_kt74_7 = false;      // KT-74 Кнопка 7
    bool t_kt74_ent = false;    // KT-74 Кнопка ENT
    bool t_kt74_func = false;   // KT-74 Кнопка FUNC

    float f_kt74 = 0.f;         // КТ-74 ращалка
    quint8 sw_kn62a = 0;        // KN-62A ВЕРХНИЙ ПЕРЕКЛЮЧАТЕЛЬ
    bool t_kn62a_off = false;   // KN-62A ВЫКЛЮЧАТЕЛЬ
    float f_kn62 = 0.f;         // KN-62A Вращалка
};


// Панель Выбора Режима
class PanelSelectMode : public QObject
{
    Q_OBJECT
public:
    bool t_night = false;
    bool t_day = false;
    bool t_marsh = false;
    bool t_posad = false;
    bool t_ils = false;
    bool t_sp50 = false;
    bool i_K1 = false;
    bool i_G1 = false;
    bool i_K2 = false;
    bool i_G2 = false;
    bool i_K = false;
    bool i_G = false;
    float f_gaint = 0.f;
};
// Панель Выбора Курса
class PanelSelectCourse : public QObject
{
    Q_OBJECT
public:
    float f_course = 0.f;
    float f_set_course = 0.f;
};

//
/**
 * @brief The RghtPanelAZS class
 * Класс отвечает за АЗС панель вертолета
 * Compute() это автомат работы азс
 */
class RghtPanelAZS : public QObject
{
    Q_OBJECT
public:
	//RghtPanelAZS( ) = default;
	//~RghtPanelAZS( ) {};
	void StartAll( );
    //    void Compute(float dT) {};

	bool i_peash = true;         // Индикатор ПЭАШ
	bool i_pevu = true;          // Индикатор ПЭВУ
	bool i_vis_otstr = true;     // Индикатор ВИЗУАЛИЗАЦИЯ ОТСТРЕЛА
	bool i_ash1 = true;          // Индикатор АШ-1
	bool i_ash2 = true;          // Индикатор АШ-2
	bool i_shakk_vsu = true;     // Индикатор ШАКК ВСУ
	bool i_shvu_vsu = true;      // Индикатор ШВУ ВСУ
	bool i_shvu_1k = true;       // Индикатор ШВУ 1К
	bool i_shvu_2k = true;       // Индикатор ШВУ 2К

    bool t_nav1 = true;            // Тумблер НАВ1
    bool t_nav2 = true;            // Тумблер НАВ2
    bool t_distmeter = true;       // Тумблер ДАЛЬНОМЕР
	bool t_ta_start = true;      // Тумблер ТУРБОАГРЕГАТ ЗАПУСК
	bool t_ta_ign = true;        // Тумблер ТУРБОАГРЕГАТ ЗАЖИГ
	bool t_eng_start = true;    // Тумблер ДВИГАТЕЛИ ЗАПУСК
	bool t_eng_ign = true;       // Тумблер ДВИГАТЕЛИ ЗАЖИГ
	bool t_fuel_gauge = true;    // Тумблер ТОПЛИВОМЕР
	bool t_pump_consum = true;   // Тумблер НАСОСЫ ТОПЛИВН БАКОВ РАСХОД
	bool t_pump_left = true;     // Тумблер НАСОСЫ ТОПЛИВН БАКОВ  ЛЕВОГО
	bool t_pump_right = true;    // Тумблер НАСОСЫ ТОПЛИВН БАКОВ  ПРАВОГО
	bool t_sas = true;           // Тумблер САС
	bool t_auto_main = true;     // Тумблер АВТОПИЛОТ ОСНОВН
	bool t_auto_fric = true;     // Тумблер АВТОПИЛОТ ФРИКЦ
	bool t_auto_emufta = true;   // Тумблер АВТОПИЛОТ ЭЛЕКТРОМУФТА
	bool t_hidro_main = true;    // Тумблер ГИДРОСИСТ ОСНОВН
	bool t_hidro_sec = true;     // Тумблер ГИДРОСИСТ ДУБЛИР
	bool t_apparel = true;       // Тумблер УПРАВЛ АППАРЕЛЬ
	bool t_turn_signal = true;   // Тумблер УКАЗАТ ПОВОРОТ
	bool t_spuu_52 = true;       // Тумблер СПУУ-52
	bool t_rentgen = true;       // Тумблер РЕНТГЕНОМЕТР
    bool t_n_ctrl = true;          // Тумблер УПРАЛ ОБОРОТАМИ
    bool t_ctrl_start = true;    // Тумблер УПРАВЛ ЗАПУСК
	bool t_close_left = true;    // Тумблер ПЕРЕКРЫВ КРАНЫ ЛЕВЫЙ
	bool t_close_rght = true;    // Тумблер ПЕРЕКРЫВ КРАНЫ ПРАВЫЙ
	bool t_close_bypass = true;  // Тумблер КРАН ПЕРЕПУС
	bool t_ppo_signal = true;    // Тумблер ППО СИГНАЛИЗАЦ
	bool t_ppo_1_auto = true;    // Тумблер I ОЧЕРЕДЬ АВТОМ
	bool t_ppo_1_hand = true;    // Тумблер I ОЧЕРЕДЬ РУЧН
	bool t_ppo_2_auto = true;    // Тумблер 2 ОЧЕРЕДЬ АВТОМ
	bool t_ppo_2_hand = true;    // Тумблер 2 ОЧЕРЕДЬ РУЧНАЯ
	bool t_r_spu = true;         // Тумблер СПУ
	bool t_r_altimetr = true;    // Тумблер ВЫСОТОМЕР
    bool t_r_comand_osnv = true;   // Тумблер КОМАНД РС
    bool t_pust_01 = true;
	bool t_r_ils = true;         // Тумблер HAB ILS
	bool t_r_empty = true;       // Тумблер РАДИО - ПУСТО
	bool t_r_compas_sv = true;   // Тумблер КОМПАС СВ
	bool t_r_compas_ukv = true;  // Тумблер КОМПАС УКВ
    bool t_resp_uvd1 = true;       // Ответчик УВД 1
    bool t_resp_uvd2 = true;       // Ответчик УВД 2
	bool t_r_rls = true;         // Тумблер РЛС
	bool t_far_left_ctrl = true; // Тумблер ЛЕВ ФАРА УПРАВЛ
	bool t_far_left_flash = true;// Тумблер ЛЕВ ФАРА СВЕТ
	bool t_far_right_ctrl = true;// Тумблер ПРАВ ФАРА УПРАВЛ
	bool t_far_right_flash = true;// Тумблер ПРАВ ФАРА СВЕТ
	bool t_far_empty = true;     // Тумблер ПУСТО
	bool t_strov_light = true;   // Тумблер СТРОЕВ ОГНИ
    bool t_ano = true;             // Тумблер АНО
    bool t_cut_main_lock = true;   // Тумблер ОБРУБ ТРОС СПГ УПРАВЛ ЗАМКА ОСНОВН
    bool t_cut_sec_lock = true;    // Тумблер ОБРУБ ТРОС СПГ УПРАВЛ ЗАМКА ДУБЛИР
    bool t_siren_missle = true;    // Ракеты Сирена
    bool t_avar_sbros = true;      // Кнопка Аварийного сброса груза
    bool t_tact_sbros = true;      // Кнопка Аварийного сброса груза
	bool t_ais_ctrl = true;      // Тумблер ПРОТИВООБЛЕД СИСТЕМА УПРАВЛЕНИЕ
	bool t_ais_pzu_left = true;  // Тумблер ПРОТИВООБЛЕД СИСТЕМА ПЗУ ЛЕВ ДВИГАТ
	bool t_ais_pzu_rght = true;  // Тумблер ПРОТИВООБЛЕД СИСТЕМА ПЗУ ПРАВ ДВИГАТ
	bool t_ais_co_121 = true;    // Тумблер ПРОТИВООБЛЕД СИСТЕМА СО-121
	bool t_ais_glass = true;     // Тумблер ПРОТИВООБЛЕД СИСТЕМА СТЕКОЛ
	bool t_ais_wipe_left = true; // Тумблер СТЕКЛООЧИСТ ЛЕВЫЙ
	bool t_ais_wipe_rght = true; // Тумблер СТЕКЛООЧИСТ ПРАВЫЙ
	bool t_l166_81a = true;      // Тумблер Л166-81A
    bool t_p_nv = true;            // Тумблер П НВ
	bool t_ko_50 = true;         // Тумблер КО-50  

private:

private slots:
};


// СПУ-7 ПРАВ Пилота
// нет

/**
 * @brief RightPanelElectroPult The  class
 * Класс Правая Панель Электропульта
 * радиокомпасы и КО50
 */
class RightPanelElectroPult : public QObject {
    Q_OBJECT
public:
    ARK_9 ark_9;				  // Автоматический радио компас 9
    ARK_UD ark_ud;				  // Автоматический радио компас УКВ
    GMK gmk;					  // ГМК
    float f_t_ko50 = 0;           // Крутилка ЗАДАТЧИК Т КО-50
    bool i_ko50_heater = false;   // Индикатор ПОДОГРЕВАТЕЛЬ
    bool i_ko50_ignition = false; // Индикатор ЗАЖИГАНИЕ
    bool i_ko50_work = false;     // Индикатор КО-50 РАБОТАЕТ
    bool t_ko50_ta_start = false; // Кнопка ЗАПУСК ТУРБОАГРЕГАТА
    bool t_ko50_man = false;      // Тумблер КО50-РУЧН
    bool t_ko50_auto = false;     // Тумблер КО50-АВТ
    bool t_ko50_fill = false;     // Тумблер КО50-ЗАЛИВКА
    bool t_ko50_full = false;     // Тумблер КО50-ПОЛНАЯ
    bool t_ko50_midl = false;     // Тумблер КО50-СРЕДНЯЯ
    bool t_ko50_vent = false;     // Тумблер Вентил
    bool t_N = false;             // Тумблер СЕВ ЮГ
    bool t_S = false;             // Тумблер СЕВ СЕВЕР
    //bool t_kontr = false;         // Тумблер КОНТР 300
    //bool t_kontr_0 = false;       // Тумблер КОНТР 0
    float f_latitude = 0;         // широта
    bool t_mk = false;            // Тумблер МК ГК
    bool t_zk_l = false;          // Тумблер ЗК ЛЕВ
    bool t_zk_r = false;          // Тумблер ЗК ПРАВ
    bool t_dubl_osnv = false;   // Тумблер ДУБЛИР ОСНОВ
    bool t_osnv = false;        //Тумблер ОСНОВ(Новое добавление)
    bool i_dubl = false;        // Индикатор ДУБЛИР
    bool i_osnv = false;        // Индикатор ОСНОВ
    bool akb_hight_temp_1 = false; //индикатор высокая температура акб 1
    bool akb_hight_temp_2 = false; //индикатор высокая температура акб 2

    quint8 get_rk_mode();
    quint8 get_rkud_chanel();

private:

private slots:
};

/**
 * @brief RightSidePanelElectroPult The  class
 * Класс Правая боковая Панель Электропульта
 * правая длинная с ПЗУ и подсветкой
 */
class RightSidePanelElectroPult : public QObject {
    Q_OBJECT
public:
    bool t_check_lamp = false;      // Тумблер ПРОВЕРКА ЛАМП
    bool t_main_second = false;     // Тумблер ОСНОВ
    bool t_main_reserv = false;     // Тумблер РЕЗЕРВ
    float f_board = 0.f;            // Вращалка ТАБЛО
    bool t_day = false;             // Тумблер ДЕНЬ
    bool t_night = false;           // Тумблер НОЧЬ
    float f_gain = 0.f;             // Вращалка ГРОМКОСТЬ ОБЩАЯ
    quint8 sw_scan = 0;             // Вращалка НАБОР СКАН
    bool t_light_emeg = false;      // Тумблер ДЕЖУРНОЕ ОСВЕЩЕНИЕ
    bool t_light_generl = false;    // Тумблер ОБЩЕЕ ОСВЕЩЕНИЕ
    bool t_contr = false;           // Кнопка КОНТРОЛЬ
    bool i_contr = false;           // Лампа КОНТРОЛЬ
    bool i_low_lock = false;        // Индикатор НИЖНИЙ ЗАМОК ОТКРЫТ
    bool t_ano = false;             // Тумблер 
    bool t_light_drill = false;     // Тумблер ОГНИ СТРОЕВ
    bool t_light_contur = false;    // Тумблер ОГНИ КОНТУР
    bool t_flashlight = false;      // Тумблер МАЯК ВЕРХ
    bool t_beacon_down = false;     // Тумблер МАЯК НИЗ
    bool t_pzu_left = false;        // Тумблер ПЗУ ДВИГАТ ЛЕВ
    bool t_pzu_right = false;       // Тумблер ПЗУ ДВИГАТ ПРАВ
    bool flag_pzu_left = false;        // Флаг ПЗУ ДВИГАТ ЛЕВ
    bool flag_pzu_right = false;       // Флаг ПЗУ ДВИГАТ ПРАВ
    bool t_pvd_heat_left = false;   // Тумблер ОБОГРЕВ ПВД ЛЕВ
    bool t_pvd_heat_right = false;  // Тумблер ОБОГРЕВ ПВД ПРАВ
    bool t_clock_heat = false;      // Тумблер ОБОГРЕВ ЧАСОВ
    //bool t_heat_accum = false;    // Тумблер ОБОГРЕВ АККУМ
    //bool t_flashlight = false;    // Тумблер МИГАЛКА
    bool t_sublight_1 = false;      // Тумблер ПОДСВЕТ ГРУППА1
    bool t_sublight_2 = false;      // Тумблер ПОДСВЕТ ГРУППА2
    float f_day_night = 0.f;        // Ночь - День
    bool t_fan = false;             // Тумблер вентилятор
    bool i_pzu_left_on = false;     // Индикатор ЛЕВ.ПЗУ ВКЛЮЧЕН
    bool i_pzu_right_on = false;    // Индикатор ПРАВ.ПЗУ ВКЛЮЧЕН
    PRIMA prima;
};

/**
 * @brief The RightShildElectroPilt class
 * Класс Правый Щиток Электропульта
 * треугольный пульт включатели ДИСС АВГ
 */
class RightShildElectroPilt : public QObject {
    Q_OBJECT
public:
    bool t_pvd_heat_contr = false;  // КОНТРОЛЬ ОБОГРЕВА ПВД
    bool i_pvd_heat_ok = false;     // Индикатор ОБОГРЕВ ИСПРАВЕН
	bool t_diss = true;// Тумблер ДИСС
    bool i_diss_on = false;         // Индикатор ДИСС ВКЛЮЧЕН
    bool t_rs = true;              // Тумблер СВЯЗНАЯ РС
    bool t_gmk = true;             // Тумблер ГМК-1
    bool t_sgu35 = true;           // Тумблер ВК-53 (СГУ35)
    bool i_sgu35_on = true;        // Индикатор СГУ-35 ВКЛЮЧЕН
    bool t_ag_right = true;        // Тумблер ПРАВЫЙ АГ
    bool t_plafon = true;          // Тумблер ПЛАФОН
    quint8 sw_wiper_mode;           // Переключатель СТЕКЛООЧИСТИТЕЛЬ
    bool t_laring = true;          // Тумблер ЛАРИНГ 
    bool t_rkud = true;            // Тумблер АРК-УД

private:
private slots:
};

/**
 * @brief The RightElectroPanelElectroPult class
 * Класс Правый Электрощиток Электропульта
 * доска с вольметрами и амперметрами, включатели АКК, ВУ и Ген
 */
class ElectroPanelElectroPult : public QObject {
    Q_OBJECT
public:
    float f_amper_1 = 0.f;          // Шкала ПОСТОЯННЫЙ ТОК ЛЕВ
    float f_amper_2 = 0.f;          // Шкала ПОСТОЯННЫЙ ТОК ЦЕНТР
    float f_amper_3 = 0.f;          // Шкала ПОСТОЯННЫЙ ТОК ПРАВ
	bool t_akkum_1 = true;         // Тумблер АККУМУЛЯТОРЫ 1
	bool t_akkum_2 = true;         // Тумблер АККУМУЛЯТОРЫ 2
    quint8 sw_ctrl = 0;             // Переключ КОНТРОЛЬ ТОКА
    bool t_ctrl_vu1 = false;        // Переключ КОНТРОЛЬ ТОКА
    bool t_ctrl_vu2 = false;        // Переключ КОНТРОЛЬ ТОКА
    quint8 sw_dc_volt_mode = 0;     // Переключатель ЦЕНТР
    bool t_dc_volt_mode_0 = false;  // Переключ КОНТРОЛЬ откл
    bool t_dc_volt_mode_1 = false;  // Переключ КОНТРОЛЬ акк1
    bool t_dc_volt_mode_2 = false;  // Переключ КОНТРОЛЬ акк2
    bool t_dc_volt_mode_3 = false;  // Переключ КОНТРОЛЬ вып1
    bool t_dc_volt_mode_4 = false;  // Переключ КОНТРОЛЬ вып2
    bool t_dc_volt_mode_5 = false;  // Переключ КОНТРОЛЬ шина ву1
    bool t_dc_volt_mode_6 = false;  // Переключ КОНТРОЛЬ шина ву2
    bool t_dc_volt_mode_7 = false;  // Переключ КОНТРОЛЬ шина ву всу
    bool t_dc_volt_mode_8 = false;  // Переключ КОНТРОЛЬ резервный ген
    bool t_dc_volt_mode_9 = false;  // Переключ КОНТРОЛЬ откл
    float f_volt_reg = 0.f;         // Крутилка РЕГУЛИР НАПРЯЖ
	bool t_ACDC_1 = true;           // Тумблер ВЫПРЯМИТЕЛИ 1
	bool t_ACDC_2 = true;           // Тумблер ВЫПРЯМИТЕЛИ 2
    bool t_test_sys = false;        // Тумблер ПРОВЕРКА ОБОРУД подключение преобразователя ПО800 27-> ~115
    bool t_aero_pit_dc = false;     // Тумблер АЭР ПИТ потоянный
    float f_volt = 0.f;             // Вольтметр
    bool i_vu1_brok = false;        // Индикатор ВУ1 НЕ РАБОТ
    bool i_vu2_brok = false;        // Индикатор ВУ2 НЕ РАБОТ
    bool i_aero_pit_dc = false;     // Индикатор АЭР питание включено
    float f_ac_amper_1 = 0.f;       // Переменный ток-амперметр лев
    float f_ac_amper_2 = 0.f;       // Переменный ток-амперметр прав
    float f_ac_amper_3 = 0.f;       // Переменный ток-амперметр средний
    float f_ac_volt = 0.f;          // Переменный ток-вольтметр
    bool i_gen_1_off = false;       // Индикатор ГЕН. 1 ОТКЛ
    bool i_gen_2_off = false;       // Индикатор ГЕН. 2 ОТКЛ
	bool t_gen_1 = true;            // Тумблер ГЕНЕРАТОРЫ 1
	bool t_gen_1_contr = false;     // Тумблер ГЕНЕРАТОРЫ 1 Контроль
	bool t_gen_2 = true;            // Тумблер ГЕНЕРАТОРЫ 2
	bool t_gen_2_contr = false;     // Тумблер ГЕНЕРАТОРЫ 2 Контроль
    bool t_res_gen = false;         // Тумблер ВСУ
    bool t_r36_auto = false;        // Тумблер ТР-Р36В АВТОМАТ
    bool t_r36_man = false;         // Тумблер ТР-Р36В РУЧНОЕ
	bool t_pts_auto = true;         // Тумблер ПТС АВТОМАТ
    bool t_pts_man = false;         // Тумблер ПТС РУЧНОЕ
    bool t_aero_pit_ac = false;     // Тумблер АЭР ПИТАН
    bool t_gen = false;             // Тумблер ПТС АЭР ПИТАН
    bool t_gen_pts_contr = false;   // �������� ���������� ��� ��� �����
    quint8 sw_ac_volt_mode = 0;     // Переключатель КОНТРОЛЬ НАПРЯЖЕНИЯ
    bool t_ac_volt_mode_pts_gen1a = false;  // Переключатель ЛТС ген1
    bool t_ac_volt_mode_pts_gen1b = false;  // Переключатель ген1
    bool t_ac_volt_mode_pts_gen1c = false;  // Переключатель ген1
    bool t_ac_volt_mode_aer_gen1a = false;  // Переключатель аэродромное питание ген1
    bool t_ac_volt_mode_aer_gen1b = false;  // Переключатель ген1
    bool t_ac_volt_mode_aer_gen1c = false;  // Переключатель ген1
  
    bool i_reser36 = false;         // Индикатор -38В РЕЗЕРВ
    bool i_ptc_on = false;           // Индикатор ПТC ВКЛЮЧЕН
    bool i_resev_on = false;        // Индикатор РЕЗЕРВ ЛИНИЯ ВКЛ
    bool i_rap_on = false;          // Индикатор АЭР ПОДКЛ

    SPU_Pult spu;                   // СПУ-7 для борт-техника
    quint8 get_ctrl();
    quint8 get_dc_volt_mode();
    quint8 get_ac_volt_mode();
private:

private slots:
};

/**
 * @brief The RightPilotBoard class
 * Класс Правая Приборная Панель
 *
 */
class RightPilotBoard : public QObject {
    Q_OBJECT
public:
    bool t_fara_on = false;         // Тумблер ФАРА СВЕТ
    bool t_fara_close = false;      // Тумблер ФАРА УБРАНА
    bool t_projector = false;       // Тумблер УПРАВЛ ПРОЖЕКТОР
    float f_fuel = 0.f;             // ТОПЛИВОМЕР
    bool i_fuel_270 = false;        // Индикатор ОСТАЛОСЬ 270 Л
    bool i_dptb_left = false;       // Индикатор ДПТБ ПУСТ ЛЕВ
    bool i_dptb_right = false;      // Индикатор ДПТБ ПУСТ ПРАВ
    bool t_alarm = false;           // Тумблер СИГНАЛИЗАЦИЯ
    bool i_warning_earth = false;   // Индикатор ВНИМЕНИЕ ЗЕМЛЯ
    bool i_danger_earth = false;    // Индикатор ОПАСНО ЗЕМЛЯ
    bool t_dnmr = false;            // Тумблер НАВ/ДНМР
    bool t_fuel_0 = false;
    bool t_fuel_1 = false;
    bool t_fuel_2 = false;
    bool t_fuel_3 = false;
    bool t_fuel_4 = false;
    bool t_fuel_5 = false;
    bool t_fuel_6 = false;

    VD_10VK vd_10vk;    // Высотометр (КМ)-стрелка
    UC_450K uc_450k;    // Спидометр-стрелка
    KI_204 ki_206;      // система навигации ki-204
    AVG avg;            // авиагоризонт
    VR_30MK vr_30mk;    // Подъем-спуск-Стрелка Вариометр ВР-30МК
    UGR_4UK ugr_4uk;    // Указатель УГР-4УК курсовой системы ГМК-1А
    SNOS snos;          // Индикатор путевой скорости и угла сноса аппаратуры ДИСС-15
    ITE_1T ite_1t;      // Несущий винт обороты - стрелка
    ITE_2T ite_2t;      // Двигатели обороты - стрелка
    DISS_15 diss_15;    // Индикатор координат аппаратуры ДИСС-15
    KDI_572 kdi_572;    // Дальномер
    float f_temp = 0.f; // Температура в пассажирском отсеке(новая переменная)
    bool i_diss_brok = false;   // Индикатор ДИСС ОТКАЗ
    bool i_ag_brok = false;     // Индикатор ОТКАЗ АГ ПРАВ
    bool i_roll_max = false;    // Индикатор КРЕН ВЕЛИК
    bool i_pitch_max = false;   // Индикатор ТАНГАЖ ВЕЛИК

    SPU_Pult spu;                   // СПУ-7 для борт-техника
    quint8 sw_dop_fuel;         // Галетный Переключатель ДОПОЛН БАКИ
    bool t_fuel_low_left = false;
    bool t_fuel_low_right = false;
    bool t_fuel_hi_left = false;
    bool t_fuel_hi_right = false;

    quint8 get_dop_fuel();
    quint8&  sw_fuel();  // Переключатель БАКОВ
    quint8  sw_fuel_mm( );  // Переключатель БАКОВ для модели
private:
    quint8 sw_fuel_ = 0; // Галетный Переключатель

private slots:
};


/**
 * @brief The PilotControl class
 * Класс Рычаги и педали
 *
 */
class PilotControl : public QObject {
    Q_OBJECT
public:
    float& roll( ) { return f_roll; };

    float dead_zone = 3.f;                 // мертвая зона ручки
    float dead_zone_pd = 9.f;              // мертвая зона педалей
    float f_roll = 127.f;          // Штурвал ВЛЕВО-ВПРАВО
    float f_pitch = 127.f;         // Штурвал ВПЕРЕД-НАЗАД
    float f_yawl = 127.f;          // Педали Лев Пилот
    float f_ap_roll = 127.f;          // автопилотное Штурвал ВЛЕВО-ВПРАВО
    float f_ap_pitch = 127.f;         // автопилотное Штурвал ВПЕРЕД-НАЗАД
    float f_ap_yawl = 127.f;          // автопилотное Педали Лев Пилот
    float f_ap_Fosh = 0.f;             // автопилотное Рукоятка шаг-газ
    float f_trg_roll = 127.f;          // Штурвал ВЛЕВО-ВПРАВО
    float f_trg_pitch = 127.f;         // Штурвал ВПЕРЕД-НАЗАД
    float f_trg_yawl = 127.f;          // Педали Лев Пилот
    void set_trim();
    float get_roll();          // Штурвал ВЛЕВО-ВПРАВО
    float get_pitch();         // Штурвал ВПЕРЕД-НАЗАД
    float get_yawl();         // Педали ВЛЕВО-ВПРАВО
    void set_roll(float x) { f_roll = x; };          // Штурвал ВЛЕВО-ВПРАВО
    void set_pitch(float x) { f_pitch = x; };         // Штурвал ВПЕРЕД-НАЗАД
    void set_yawl(float x) { f_yawl = x; };         // Педали ВЛЕВО-ВПРАВО

    bool t_eng_left_brake = false; // Остановка двигателя ЛЕВ
    bool t_eng_rght_brake = false; // Остановка двигателя ПРАВ
    bool t_bomb_drop_left = false; // Сброс бомб левым пилотом
    bool t_bomb_drop_right = false; // Сброс бомб правым пилотом
    bool t_right_fire = false;      // Прав штурвал - Левая красная кнопка
    bool t_right_ap_off = false;    // Прав штурвал - Кнопка ОТКЛЮЧЕНИЕ АВТОПИЛОТА
    bool t_right_ap_off_prev = false; // триггер
//    bool t_rght_centr=false;     // Прав штурвал - Кнопка ЭЛЕКТРОМАГНИТНЫЙ ТОРМОЗ
    bool t_right_spu1 = false;       // Прав штурвал -Кнопка СПУ РАДИО
    bool t_right_spu2 = false;      // Прав штурвал -Кнопка СПУ РАДИО
    bool t_left_fire_prev = false; // Лев штурвал-Левая красная кнопка предыдущее значение для тригера
    bool t_left_fire = false;      // Лев штурвал-Левая красная кнопка
    bool t_left_ap_off = false;    // Лев штурвал - Кнопка ОТКЛЮЧЕНИЕ АВТОПИЛОТА
    bool t_left_trim = false;    // Лев штурвал - Кнопка ЭЛЕКТРОМАГНИТНЫЙ ТОРМОЗ
//    quint8 sw_left_spu;       // Лев штурвал - Кнопка СПУ РАДИО
//    float f_nv_brake;         // Рычаг тормоза винта
    bool t_NV_brake_on = false;    // тормоз НВ включен
	bool t_NV_brake_off = true;   // тормоз НВ выключен
    float f_eng1 = 127.f;          // Руд левый
    float f_eng2 = 127.f;          // Руд правый
    float f_Fosh = 0.f;          // Рукоятка шаг-газ
    float f_Ecor = 255.f;          // Ручка коррекции
    float f_break = 0.f;         // тормоз колес
    bool t_ap_dir_sopr = false;    // блокировка автопилота на педалях
    quint16 shet_ap_dir = 0;
	bool t_avar_drop = false;     // Кнопка Аварийного сброса груза
    bool t_fpp_l_up = false;         // Кнопка управления фарой вверх
    bool t_fpp_l_down = false;       // Кнопка управления фарой вниз
    bool t_fpp_l_left = false;       // Кнопка управления фарой влево
    bool t_fpp_l_right = false;      // Кнопка управления фарой вправо
	bool t_tact_drop = false;     // Кнопка тактического сброса груза
//    bool t_fric=false; // Кнопка фрикциона
    bool t_rpm_up = false; // Тумблер повышения оборотов НВ
    bool t_rpm_down = false; // Тумблер понижения оборотов НВ
    bool t_rpm_right_up = false; // Тумблер повышения оборотов НВ
    bool t_rpm_right_down = false; // Тумблер понижения оборотов НВ
    bool t_fpp_r_up = false; // Кнопка управления фарой вверх правого прилота
    bool t_fpp_r_down = false; // Кнопка управления фарой вниз
    bool t_fpp_r_left = false; // Кнопка управления фарой влево
    bool t_fpp_r_right = false; // Кнопка управления фарой вправо
    bool t_fr_l_fosh = false; // Кнопка фрикциона рув левого пилота


    bool t_bomb_drop = false;       // Кнопка сброса бомб
    float f_compass = 0.f;          // Магнитный Компас 
    bool reset = false;             // сброс реле питания СОД
    bool vibro = true;              // включение вибрации спадающим фронтом
    float f_vibro = 0.f;            // управелние частотой вибромотора
    bool pkt_fire = false;          // стрельба из пулемета
    bool pkt_fire_prev = false;     // предыдущее состояние спусквого крючка для тригера
    bool pkt_loaded = false;        // ПКТ взведен
    bool pkt_load = true;           // рычаг взвода
    float pkt_x = 0.f;              // наводка пулемета по горизонтели
    float pkt_y = 0.f;              // наводка пулемета по вертикали
    bool t_safety_lock = false;     // тумблер включения вооружения над головой левого пилота
    bool i_weapon_warning = false;  // Лампа ОСТОРОЖНО ЦЕПИ ВООРУЖЕНИЯ ПОД ТОКОМ

private:

private slots:
};

/**
 * @brief The  class AllWaeponPult
 * Класс сборный всех пультов вооружения
 *
 */
class AllWeaponPult : public QObject {
    Q_OBJECT
public:
    // Левый АЗС
    bool i_weapon_line = false;
    bool i_AB = false;
    bool i_vzriv = false;
    bool i_pus_bd_1_6 = false;
    bool i_npc = false;
    bool i_pus_bd_2_5 = false;
    bool i_upk = false;
    bool i_bd_1_load = false;
    bool i_bd_2_load = false;
    bool i_bd_3_load = false;
    bool i_bd_4_load = false;
    bool i_bd_5_load = false;
    bool i_bd_6_load = false;
    int  c_upk_left = 0;
    int  c_upk_right = 0;
    bool t_pus_load = false;
    bool t_pus_load_PO1 = false;    //Тумблер ПО1
    bool t_pus_load_PO2 = false;    //Тумблер ПО2(новая переменная
    bool t_main_switch = false;     //Тумблер ГЛАВНЫЙ ВЫКЛЮЧ
    bool t_vzriv = false;           //Тумблер ВЗРЫВ
    bool t_sbros = false;           //Тумблер СБРОС БОМБ
    // пульт левой приборной доски
    bool t_MV = false;      // минирование?
    bool t_PKT = false;     // РПК
    bool t_FAB = false;     // бомбы
    bool t_NAR = false;     // неуправляемые ракеты
    bool t_UPK23 = false;   // пушки 23х2
    bool t_AGS17 = false;   // гранатомет
    qint8 sw_weapon_type_ = 31; // РС по умолчанию
    // Выбор вооружения по кнопкам для визуалки
    quint8 sw_weapon_type( );
    // Выбор вооружения индикатор для органов управления
    quint8 get_sw_weapon_type( );

    quint8 sw_burst();
    bool t_fire_short = true;
    bool t_fire_midle = false;
    bool t_fire_long = false;

    bool t_pod_ext = false;
    bool t_pod_all = true;
    bool t_pod_inter = false;

    //// пульт у правой приборной доски

    // пульт минирования
    float f_counter1 = 0.f;
    float f_counter2 = 0.f;
    float f_speed_1 = 0.f;
    float f_speed_2 = 0.f;
};


/**
 * @brief The BS class
 * Базовый класс для агрегатов вертолета
 *
 */

class BS : public QObject
{
private:
    virtual void reset() {}
public:
    void Start() { reset(); }
    void Stop() { reset(); }
};

/**
 * @brief The VSU class
 * Класс отвечает за АИ9 вертолета
 * Compute() это автомат работы стартера
 */
class VSU : public BS /*QObject*/
{
    Q_OBJECT
private:
    const float temp_0 = 15;    // TODO переделать на чтиемое с инструктора
    const float temp_max = 880;
    const float temp_normal = 630;
    const float presure_0 = 0.f;
    const float presure_normal = 1.9f;
    const int stage_1 = 6000;  // 6 секунд от старта
    const int stage_2 = 9000; // 8 секунд от старта
    const int stage_3 = 12000; // 12 секунд от старта
    const int stage_4 = 25000; // 15 секунд от старта
    const int time_scroll = 20000; // время работы прокрутки и ложного запуска 20 сек - TODO уточнить
    const int d_time = COMPSTEP;    // шаг итерирования в мс
    float dT = 10.f;
    float dP = 0.013f;
    float ind_sim_time = 0.0f;
    QTimer* OnBoardTimer;
    int time_ = 0;
public:
    void reset();
    enum class State
    {
        OFF,            // остановлен
        STARTING,       // запускается
        START_FALSE,    // ложный запуск
        SCROLL,         // прокрутка
        WORK,           // работает
        STOPING,        // останавливается
    };
    State state_ = State::OFF;

    VSU(Helicopter_core* Helicopter);
    ~VSU() {};

    Helicopter_core* Helicopter;
    float temp_ = 0.f;      // температура газов
    float temp_comp = 0.f;  // температура газов расчетная, кто больше
    float presure_ = 0.f;   // давление за турбиной
    bool ta_automat_on = false; // работает автомат стартера
    bool ta_p_oil_norm = false;
    bool ta_rpm_norm = false;
    bool ta_rpm_max = false;

    float temp() { return temp_; };
    float presure() { return presure_; }
    void start_TA();
    void false_start_TA();
    void scroll_TA();
    void stop_TA();
    bool is_working() { return state_ == State::WORK ? true : false; }
    bool is_stoping() { return state_ == State::STOPING ? true : false; }
    bool is_off() { return state_ == State::OFF ? true : false; }
    bool is_starting() { return (state_ == State::STARTING || state_ == State::START_FALSE || state_ == State::SCROLL) ? true : false; }
    State get_state() { return state_; };
private:
    void starting();
    void stoping();
private slots:
    void on_Compute();
};
QDebug operator<<(QDebug out, const VSU::State& state_);


/**
*@brief The Electrics class
* Класс отвечает за электрические системы вертолета
* Compute() это автомат работы стартера
*/
class Electrics : public BS
{
    Q_OBJECT
private:
    Helicopter_core* Helicopter;
    QTimer* OnBoardTimer = nullptr;
    void reset();
public:
    Electrics(Helicopter_core* Helicopter);
    ~Electrics() {};
    bool Is27V() { return ((Uakk > 20) || t_akk); };
    bool Is208V() { return Ugen204 > 200; };
    bool Is28V() { return ((Uvu1 > 27) || (Uvu2 > 27));}

    float Uakk1 = 0.;  // шина постоянного питание с аккумулятор 1
    float Uakk2 = 0.;  // шина постоянного питание с аккумулятор 2
    float Iakk1 = 0.;  // Ток аккумулятора 1
    float Iakk2 = 0.;  // Ток аккумулятора 2
    float Uakk = 0.;  // шина постоянного питание с аккумуляторов
    bool t_akk = false; // включенные АКБ на электрощитке

    float Uvu1 = 0.;   // постоянного питание выпрямитель 1
    float Uvu2 = 0.;   // постоянного питание выпрямитель 2
    float Ivu1 = 0.;   // Ток выпрямителя 1
    float Ivu2 = 0.;   // Ток выпрямителя 2
    float Uvu = 0.;   // шина постоянного питание выпрямителей

    float Usgu = 0.;    // резервный генератор на 29 вольт
    float Isgu = 0.;   // Ток резевного генератора

    float Ugen36 = 0.;    // шина переменного трехфазного питания 36в 400Гц
    float Ugen36r = 0.;    // шина переменного трехфазного питания резервная 36в 400Гц
    float Ugen115 = 0.;   // шина переменного однофазного питяния 115в 400Гц
    float Ugen115r = 0.;   // шина переменного однофазного питяния резервная 115в 400Гц
    float Ugen204 = 0.;   // шина переменного трехфазного питяния 204в 400Гц
    float Udim = 0.;    // шина переменного однофазного питяния 36в 400Гц

    float Igen1 = 0.;    // Ток генератора 1
    float Igen2 = 0.;    // Ток генератора 2

    int Hgeo = 0; //высота вертолета для контроля
private slots:
    void on_Compute();
};

/**
*@brief The Hydro class
* Класс отвечает за электрические системы вертолета
* Compute() это автомат работы стартера
*/
class Hydraulic : public BS
{
    Q_OBJECT
private:
    Helicopter_core* Helicopter;
    const float min_pres = 45.f;    // давление включения насоса
    const float max_pres = 67.f;    // давление выключения насоса
    const float second_off = 40.f;  // выключение резервного
    const float switch_pres = 35.f; // переключение на резервный
    const float air_pres = 30.f;    // давление азота
    const float dP = 1.8f;           // производительность насоса TODO надо узнать
    QTimer* OnBoardTimer = nullptr;
    int time_ = 0;
    void reset();
public:
    Hydraulic(Helicopter_core* Helicopter);
    ~Hydraulic() {};
    bool isWork( ) { return (main > 1 || second > 1); };

    float main = 0.f;    // давление в главной системе
    bool main_pump = false;
    float second = 0.f;  // давление в резервной системе
    bool second_pump = false;
    const float rashod = 0.5f;         // скорость падение давления в системе
    bool  broken_main = false;   // выход из строя насоса основной системы
    bool  broken_second = false;   // выход из строя насоса резервной системы
    bool  broken_akk = false;   // выход из строя аккумулятора системы

private slots:
    void on_Compute();
};

/**
 * @brief The Engine class
 * Класс отвечает за двигатели вертолета
 * Compute() это автомат работы стартера
 */
class Engine : public BS
{
    Q_OBJECT
private:
    Helicopter_core* Helicopter;
    QTimer* OnBoardTimer;
    int time_ = 0;
public:
    void reset();
    enum class State
    {
        WORK,         // работает
        OFF,            // остановлен
        STARTING,       // запускается
        STARTING_1,     // подача топлива
        START_FALSE,    // ложный запуск
        SCROLL,         // прокрутка
        STOPING,        // останавливается
    };
public:
    Engine(Helicopter_core* Helicopter);
    ~Engine() {};
    State state_ = State::OFF;
    float eng = 0.;     // обороты двишателя в процентах

    void start_eng();   // включили стартер на ФМ идет прокрутка
    void fuel_eng();    // подали топливо на фм идет - двигатель включен, переход в WORK по 55% 
    void scroll_eng();
    void false_start_eng();
    void stop_eng();
    int  mode_eng();  // значение для FM 0 - работает; 1 - выключен; 2 - нормальная работа; 3 - прокрутка; 4 - ложный запуск
    bool is_working() { return state_ == State::WORK; }
    bool is_stoping() { return state_ == State::STOPING; }
    bool is_off() { return state_ == State::OFF ? true : false; }
    bool is_starting() { return (state_ == State::STARTING || state_ == State::STARTING_1); }
    bool is_scrolling() { return (state_ == State::START_FALSE || state_ == State::SCROLL); }

private slots:
    void on_Compute();
};

QDebug operator<<(QDebug out, const Engine::State& state_);

/**
*@brief The Pneumatic  class
* Класс отвечает за электрические системы вертолета
* Compute() это автомат работы стартера
*/
class Pneumatic : public BS
{
    Q_OBJECT
private:
    Helicopter_core* Helicopter;
    const float min_pres = 40.f;
    const float max_pres = 50.f;
    const float max_break = 30.5f;
    QTimer* OnBoardTimer = nullptr;
    int time_ = 0;
    void reset() { _main = max_pres; _break = 0.f; };
    float prev_br = 255.f;
public:
    Pneumatic(Helicopter_core* Helicopter);
    ~Pneumatic() {};
    void Start() {};
    void Stop() {};
    void Push(float br) {
        if (br < 100.f)
            _break = 30.5f;
        else
            _break = (255.f - br) * (max_break / 255.f);
        // ниже код "сбрасывает" давление на момент открытия клапана тормозов, так сделано в DCS Mi-8
        // но пилот говорит, что маенометр показывает реальное давление в системе, да и стрелки шаговиков не успевают
        //_main = br < prev_br ? 0.f : max_pres; 
        //prev_br = br;
    };

    float _main = max_pres;    // давление в главной системе
    float _break = 0.f;  // давление в тормозной системе

private slots:
    void on_Compute() {};
};

/**
 * @brief The BoardSystem class
 * Класс для внутренней логики дополнительные переменные, мигание, тригеры и тд
 *
 */
class BoardSystem : public QObject {
    Q_OBJECT
private:
    Helicopter_core* Helicopter;
    void reset();

    float f_roll = 0.f;         // полный крен ЛА из модели
    float f_pitch = 0.f;        // полный тангаж ЛА
    float f_course = 0.f;       // направлене ЛА
    float matmodelY = 0.f;      // высота
    float f_ms_roll = 0.f;      // крен системы подвжности
    float f_ms_pitch = 0.f;     // тангаж системы подвижности
    float f_ms_course = 0.f;    // направлене системы подвижности
    bool  platform = false;     // крены за счет платформы
    QTimer* timer_05;           // полсекундный счетчик
    uint counter38_5 = 0;           // счетчик на 38,5 секунды
public:
    BoardSystem(Helicopter_core* Helicopter);
    ~BoardSystem();
    void Start();
    void Stop();
    QString json();
    bool StateChanged();
	void StartAll( ); // запуск всех систем в автоматическом режиме для отладки

    int  platform_period = 50;
    bool b_light_cab = false;     // подсветка приборов и кабины при включении аккумулятора
    bool b_blink = false;         // магалка для приборов, будет менять свое состояние раз в полсекунды
    // 
    bool b_autopilot = false;         // автопилот включен
    bool b_autopilot_dir = false;     // автопилот включен направления
    bool b_autopilot_alt = false;     // автопилот включен высоты
    bool b_autopilot_speed = false; // автопилот включен скорости

    bool b_POS = false;
    bool b_icing = false;
    uint u_pos_switch = 0;
    bool b_fire = false;        // Стрельба


    bool b_save = false;
    bool   manual_mode = false;		// режим ручного перемещения по карте (задание начальных координат)
    double StartLatitude = 0.;       // Широта
    double StartLongitude = 0.;      // Долгота
    double Latitude = 0.;            // Широта
    double Longitude = 0.;           // Долгота
    double BarHeight = 0.;          // Высота бараметрическая
    double GeoHeight = 0.f;         // Высота до земли
    //double VisHeight = 0.f;       // Высота визуалки

    float Temp = 0.f;           // температура за бортом
    float NV = 0.;              // обороты несущего винта в %
    bool  changes = false;      // ппроизошли изменения, которые надо отправить на визуалку.
    bool  ANO = false;          // Включение ламп АНО
    bool  beacon_up = false;    // Включение верхнего проблескового маяка
    bool  beacon_down = false;  // Включение нижнего проблескового маяка
    bool  light_drill = false;  // Включение строевых огней
    bool  light_contur = false; // Включение контурных огней
    bool  light_taxiing = false; // Включение рулежных огней
    bool  FPP_l = false;        // Включение левой фары ФПП
    bool  FPP_r = false;        // Включение правой фары ФПП
    bool  SX16 = false;         // Включение прожектора SX-16
    bool  thermal_trap_l = false; // Отстрел тепловых ловушек по левому борту
    bool  thermal_trap_r = false; // Отстрел тепловых ловушек по правому борту

    float sx16_vert = 0.f;
    float sx16_horiz = 0.f;
    float sx16_focus = 0.f;
    float fpp_l_vert = 0.f;
    float fpp_l_horiz = 0.f;
    float fpp_r_vert = 0.f;
    float fpp_r_horiz = 0.f;

    bool b_break_press = false;
    bool b_break_release = true;
    Encoder* enc_course;
    Encoder* enc_roll;
    Encoder* enc_pitch;
    Encoder* enc_orlan_1;
    Encoder* enc_orlan_2;
    Encoder* enc_spuu52;

    double deg2rad(double deg) {
        return (deg * M_PI / 180.0);
    }
    double CalcLat(float Xg);
    double CalcLon(float Xg);
    double CalcAlt(float Xg);
    double CalcDistance(double lat1, double lon1, double alt1, double lat2, double lon2);
    float CalcGlis(float Hgeo, float Sgeo);

    void set_roll(float r);
    void set_pitch(float p);
    void set_course(float c);

    void set_ms_roll(float r);
    void set_ms_pitch(float p);

    float get_roll() const;
    float get_pitch() const;
    float get_course() const;

    float get_vis_roll() const;
    float get_vis_pitch() const;
    float get_vis_course() const;

    float get_ms_roll() const;         // крен системы подвжности
    float get_ms_pitch() const;        // тангаж системы подвижности
    float get_ms_course() const;       // направлене системы подвижности
    bool Is27V() { return _Electrics->Is27V(); };
    bool Is208V() { return _Electrics->Is208V(); };
    bool Is28V() { return _Electrics->Is28V(); };
    bool eng_automat_on = false; // работает автомат двигателя
    bool ta_starter_on = false; // стартер работает

    //std::array<bool, 10> vibration = { false };   // массив частот на вибрацию, должна быть установлена не более одной

    VSU* _VSU;                          // Класс отвечает за АИ9 вертолета
    Pneumatic* _Pneumatic;
    Hydraulic* _Hydraulic;
    Electrics* _Electrics;        // Класс отвечает за электрические системы вертолета
    Engine* _Engine_1;         // Класс отвечает за двигатели вертолета
    Engine* _Engine_2;         // Класс отвечает за двигатели вертолета
    bool pps[2] = { 1,1 };
    //float f_set_course=0.f;   // заданный курс левая доска
    //float f_barometr=670.f;
    int   tablo = 0;                // комбинированая строка для вывода ДИСС и УВ26
    //float f_ki13 = 0;               // магнитный компас
private slots:
    void updateTimer();
};

#endif // Helicopter_BOARD_SYSTEM_H
