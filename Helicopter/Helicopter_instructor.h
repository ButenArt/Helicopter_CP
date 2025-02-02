#ifndef Helicopter_INSTRUCTOR_H
#define Helicopter_INSTRUCTOR_H
#include <QObject>
#include <QDebug>
#include <QDataStream>

//#include "Common/Instructor.h"

#pragma pack(push, 1)
struct InstructorTCPHeader
{
    quint32 Mark;
    quint32 PacketType;
    quint32 DataSize;
};



struct MapObjects
{
    qint32 objUID = 0;                     // UID объекта
    quint8 type = 0;                       // Тип объекта
    quint8 subType = 0;                    // Подтип объекта
    qint32 azimuth = 0;                    // Азимут
    float latitude = 0.f;                    // Широта
    float longitude = 0.f;                   // Долгота
};

struct TargetsShow
{
    quint32 startTime = 0;                 // Время старта
    quint32 dayTime = 0;                   // Время показа днем
    quint32 nightTime = 0;                 // Время показа ночью
};

struct MapTargets
{
    qint32 ObjectiveUID = 0;               // UID цели
    qint32 TargetUID = 0;                  // UID мишени
    quint8 TargetType = 0;                 // Тип мишени
    quint8 TargetColor = 0;                // Цвет мишени
    qint32 azimuth = 0;                    // Азимут
    float latitude = 0.f;                    // Широта
    float longitude = 0.f;                   // Долгота
    bool running = false;                      // Дижущаяся или нет
    float speed = 0.f;                       // Скорость движения (м/с)
    qint32 moveAzimuth = 0;                // Азимут движения
    qint32 moveDistance = 0;               // Расстояние движения
    bool onlyNight = false;                    // Показывать только ночью
    bool onlyDay = false;                      // Показывать только днем
    bool dropTarget = false;                   // Выключать (опускать при поражении)
    bool blockTarget = false;                  // Блокировать при поражении
    quint8 targetShowNumber = 0;           // Количество показов
    QList<TargetsShow> targetsShowList;
    //qint32 delay;                      // Задержка перед появлением (сек)
    //qint32 activeTime;                 // Время показа (сек)
};

struct Ammunition
{
    quint8 gunType = 0;                    // Тип оружия
    quint8 ammoType = 0;                   // Тип снарядов
    quint32 amount = 0;                    // Количество
};
#pragma pack(pop)

//QDataStream& operator<<( QDataStream& out, const Ammunition& ammo );
//QDataStream& operator>>( QDataStream& in, Ammunition& ammo );

struct ExerciseParametersPacket
{
    bool PacketReady = false;           // готовность пакета
    bool ReStart = false;               // Старт/Перезапуск упражнения
    bool Pause = false;                 // Пауза (Визуализация замирает)
    bool Stop = false;                  // Стоп (Прервать упражнение)
    quint8 ExerciseNum = 0;             // Номер упражнения
    quint8 MapType = 0;                 // Тип карты
    bool night = false;                 // Ночь
    quint8 season = 0;                  // Сезон
    bool Joystick = false;              // Джойстик
    bool AutoloadingSystem = false;     // Автомат заряжания
    QByteArray Uuid;                       // GUID карты и упражнения
    quint8 directionsNumber;            // Количество направлений
    bool backfire = false;              // Ответный огонь
    quint8 selectedKit = 0;             // Выбранный комплект
    quint8 selectedMode = 0;            // Выбранный режим
    quint8 distance2screen = 0;         // Расстояние до экрана
    QString run;                        // guid для записи и проигрывания полета
    QString lang;                       // Язык интерфейса
};


struct ObjectsDataPacket
{
    bool PacketReady = false;           // готовность пакета
    qint32 mapObjCount = 0;             // Число объектов
    QList<MapObjects> mapObjList;
};

struct TargetsDataPacket
{
    bool PacketReady = false;           // готовность пакета
    qint32 mapTargetsCount = 0;         // Число мишеней
    QList<MapTargets> mapTargetsList;
};

struct EnvironmentDataPacket
{
    bool PacketReady = false;           // готовность пакета
    qint32 unixtime = 0;                // Дата и время
    qint32 temperature = 0;             // Температура (градусы цельсия)
    qint32 cloudsGrade = 0;             // Бальность облаков
    float bottomCloudsLevel = 0.f;      // Нижняя кромка облаков [m]
    float topCloudsLevel = 0.f;         // Верхняя кромка облаков [m]
    bool moistureLevel;                 // Наличие осадков (дождь,снег)
    float windSpeed = 0.f;              // Скорость ветра [м/c]
    float windDirection = 0.f;          // Направление ветра градусы
    float visibility = 0.f;             // Метеовидимость
    float verticalWindSpeed = 0.f;      // Скорость ветра вертикальная [м/c]
    float windBlows = 0.f;              // Порывы ветра [м/c]
    float pressure = 0.f;               // Атмосферное давление
    qint32 snowLevel = 0;               // Заснеженность в баллах
    qint32 camNumber = 0;               // Номер камеры
    bool fog;                           // Туман
    qint32 amount_precipitation;
    qint32 intensity_precipitation;
    qint32 intensity_vpp;
};

struct AmmunitionDataPacket
{
    bool PacketReady = false;           // готовность пакета
    quint8 packageCount = 0;            // Число наборов
    QList<Ammunition> AmmunitionList;
};
struct CameraDataPacket
{
    bool PacketReady = false;           // готовность пакета
    quint8 CamNum = 0;                  // номер камеры
};

struct ControlDataPacket
{
    bool PacketReady = false;           // готовность пакета
    //quint32 CommandSize = 0;            // длина команды
    QString CommandText;                // текст команды
};

struct FailuresDataPacket
{
    bool PacketReady = false;           // готовность пакета
    quint8 failureType = 0;             // Тип отказа
    bool failureActive = false;         // Отказ включен (активирован)
};

struct CommandsDataPacket
{
    bool PacketReady = false;           // готовность пакета
    bool MoveForward = false;           // команда "Вперед"
};

struct MessagesDataPacket
{
    bool PacketReady = false;           // готовность пакета
    quint8 MessageType = 0;             // тип сообщения
//    qint32 MessageSize = 0;             // длина сообщения
    QString MessageText = 0;            // текст сообщения
};

struct AutoLoadSystemTestDataPacket
{
    bool PacketReady = false;           // готовность пакета
    bool MakeTest = false;              // тестировать работу автомата заряжания
};

struct AutoLoadSystemStatePacket
{
    bool PacketReady = false;           // готовность пакета
    bool TestRequired = false;          // требуется тестирование работы автомата заряжания
    bool TestSuccessful = false;        // работа автомата заряжания протестирована успешно
};

struct StatisticsDataPacket
{
    bool PacketReady;                   // готовность пакета
    quint32 Odometer = 0;               // Показание одометра (в метрах)
    quint32 EngineClock = 0;            // Моточасы двигателя (в секундах)
};

struct ScriptDataPacket
{
    bool PacketReady = false;           // готовность пакета
    quint8 type_load = 0;
    quint8 bombs_weight = 0;
    quint8 aso2b = 0;
    float  consum_tank = 0.f;
    float  left_tank = 0.f;
    float  right_tank = 0.f;
    float  refill = 0.f;
    float  empty_weight = 0.f;
    float  crew_weight = 0.f;
    float  load_weight = 0.f;
    float  full_weight = 0.f;
    bool   external_cargo = false;
    float  cable_length = 0.f;
    float  cargo_weight = 0.f;
    quint8 form_cargo = 0;
    float  latitude = 0.f;
    float  longitude = 0.f;
    float  height = 0.f;
    float  curs = 0.f;
    quint32  start_delay_time = 0;      // сек
    float  dir_wind = 0.f;              // Направление ветра, град(от 0 до 360)
    float  speed_wind = 0.f;            // Скорость ветра, м / с(от 0 до 40)
    float  vspeed_wind = 0.f;           // Вертикальная скорость ветра, м / с(от 0 до 40)
    float  gusts_wind = 0.f;            // Порывы ветра, м / с(от 0 до 40)
    float  air_temp = 0.f;              // Температура воздуха, град(от - 50 до 50)
    float  ait_press = 0.f;             // Давление воздуха, мм.рт.ст(от 670 до 800)
    float  visibility = 0.f;            // Метеовидемость, м(от 0  до 100, 000)
    float  cloud = 0.f;                 // Облачность, балл(от 0 до 9)
    float  low_cloud = 0.f;             // Нижняя граница облаков, м(от 0 до 10, 000)
    float  hi_cloud = 0.f;              // Верхняя граница облаков, м(от 0 до 10, 000)
    float  fallout = 0.f;               // Осадки, балл(от 0 до 9)
    float  snow_cover = 0.f;            // Заснеженность(поверхности) (от 0 до 9)
    float  air_radius = 0.f;            // Изменение Авиатрафика Радиус, Км
    quint32 air_density = 0;            // Плотность, балл
    float  auto_lat = 0.f;              // Широта, град
    float  auto_long = 0.f;             // Долгота, град
    float  auto_radius = 0.f;           // Изменение Автотрафика Радиус, Км
    quint32 auto_density = 0;           // Плотность, балл
};

struct AviaDataPacket
{
    bool PacketReady = false;           // готовность пакета

    float Pitch = 0;                    // Pitch(Наклон вперед назад) - Тангаж
    float Roll = 0;                     // Roll(Крен или наклон вправо влево)
    float Curse = 0;                    // Курс
    float Speed = 0;                    // Скорость
    float Longitude = 0;                // Координата техники х(Долгота)
    float Latitude = 0;                 // Координата техники у(Широта)
    float Altitude = 0;                 // Координата техники z(Высота)
    bool  Eng1 = false;                 // Запущен ли двигатель 1
    bool  Eng2 = false;                 // Запущен ли двигатель 2
    bool  Colision = false;             // Столкновение с обьектом
    quint8 TechID = 0;                  // Идентификатор техники
};

struct Weapon
{
    bool PacketReady = false;           // готовность пакета

};
struct TestVidStepsDataPacket
{
    bool PacketReady = false;
};

struct ProjectorDataPacket
{
    bool PacketReady = false;   // готовность пакета
    bool power_on = false;        // вкл/выкл 
};

struct Weather
{
    int     day = 0;
    int     month = 0;
    int     year = 0;
    int     hour = 0;
    int     min = 0;
    float   temp = 0.f;
    int     cloud = 0;
    float   cloud_bot = 0.f;
    float   cloud_top = 0.f;
    int     rain = 0;
    float   wind_speed = 0.f;
    float   wind_dir = 0.f;
    float   visibility = 0.f;
    float   vws = 0.f;
    float   wb = 0.f;
    float   pressure = 0.f;
    int     snow = 0;
    int     cam = 0;
    Weather(int i = 0);
    QString json();
};

struct Fault
{
    bool Fire = false;                  // Пожар на вертолете
    bool Fire_R = false;                // Пожар на вертолете
    bool Fire_KO = false;               // Пожар на вертолете
    bool Fire_VSU = false;              // Пожар на вертолете
    bool Engine1 = false;               // Отказ первого двигателя
    bool Engine2 = false;               // Отказ второго двигателя
    bool Gear = false;                  // Неисправность редуктора
    bool Gen1 = false;                  // неисправность первого генератора
    bool Gen2 = false;                  // неисправность второго генератора
    bool VU1 = false;                   // неисправность первого выпрямителя
    bool VU2 = false;                   // неисправность второго выпрямителя
    bool RV_contrl = false;             // неисправноять путевого управления
    bool RV_gear = false;               // отказ трансмисии путевого управления
    bool HidroMain = false;             // отказ основной гидросистемы
    bool HidroSecond = false;           // отказ резервной гидросистемы
    bool HidroDiafrag = false;          // Прорыв диафрагмы гидроаккумулятора
    bool Vibration_left = false;        // Повышенная вибрация левого двигателя
    bool Vibration_right = false;       // Повышенная вибрация правого двигателя 
    bool Vibration_Max_left = false;    // Опасная вибрация левого двигателя
    bool Vibration_Max_right = false;   // Опасная вибрация правого двигателя
    bool TF_right = false;              // Засорение топливного фильтра правого двигателя
    bool TF_left = false;               // Засорение топливного фильтра левого двигателя
    bool Swarf_right = false;           // стружка правого двигателя
    bool Swarf_left = false;            // стружка левого двигателя
    bool Autopilot = false;             // Отказ автопилота всех каналов
    bool Autopilot_curs = false;             // Отказ автопилота по курсу
    bool Autopilot_pitch = false;             // Отказ автопилота по крену и тангажу
    bool Autopilot_alt = false;             // Отказ автопилота по высоте
    bool AviaHorizont = false;          // отказ авиагоризонта
    bool AviaHorizont_two = false;          // отказ авиагоризонта
    bool Curs = false;                  // отказ курсовой системы
    bool Barometer = false;             // отказ барометрического высотомера
    bool Speed = false;                 // отказ левого указателя скорости
    bool radio = false;                 // отказ радиосвязи
    bool ARK = false;                   // отказ АРК
    bool spuu = false;                  // отказ СПУУ
    bool BKK18 = false;                 // отказ BKK-18
    bool BUR = false;                   // отказ БУР
    bool FreqNV = false;                // Выход частоты вращения несущего винта за допустимые пределы
    bool EngP_minLeft = false;          // Падение давления масла в левом двигателе
    bool EngP_minRight = false;          // Падение давления масла в правом двигателе
    bool EngLeftTempGas = false;        // Выход левого двигателя на режим ограничения по температуре газов перед турбиной
    bool EngRightTempGas = false;        // Выход праовго двигателя на режим ограничения по температуре газов перед турбиной
    bool PompashLeftEng = false;        // Помпаж левого двигателя
    bool FuelPumpRash = false;          // Отказ топливной системы расходный насос
    bool FuelPumpPereLeft = false;      // Отказ топлив системы перекачивающего левого насоса
    bool FuelPumpPereTWO = false;       // отказ топливн системы обоих перекачивающих насосов
    bool Fuel270 = false;               // отработка остака топлива неисправность поплавка
    bool LeftCursNoCNTRL = false;       // Самопроизвольное вращение вертолета влево при взлете или посадке
    bool AkbTempHight_1 = false;        // Загорание светового табло "АККУМ 1 t° ВЫСОКА"
    bool pos_vint = false;              // Неисправности ПОС винтов
    bool pos_pzu = false;                 // Неисправности ПОС двигателей ПЗУ
    bool pos_glass = false;                 // Неисправности ПОС стекол
    bool pos_pvd = false;                 // Неисправности ПОС обогрева пвд
    bool aero_pts = false;              // аэродромное питание
};

QDebug operator<<( QDebug out, const ExerciseParametersPacket& ep );
QDebug operator<<( QDebug out, const EnvironmentDataPacket& ep );
QDebug operator<<( QDebug out, const TargetsDataPacket& ep );
QDebug operator<<( QDebug out, const MapTargets& ep );
QDebug operator<<( QDebug out, const AmmunitionDataPacket& ep );
QDebug operator<<( QDebug out, const Ammunition& ep );
QDebug operator<<( QDebug out, const ScriptDataPacket& ep );
QDebug operator<<( QDebug out, const ObjectsDataPacket& ep );
QDebug operator<<( QDebug out, const MessagesDataPacket& ep );
QDebug operator<<( QDebug out, const FailuresDataPacket& ep );
QDebug operator<<( QDebug out, const CommandsDataPacket& ep );
QDebug operator<<( QDebug out, const ControlDataPacket& ep );

QDataStream& operator<<( QDataStream& out, const TargetsShow& ep );
QDataStream& operator>>( QDataStream& in, TargetsShow& ep );
QDataStream& operator<<( QDataStream& out, const MapObjects& ep );
QDataStream& operator>>( QDataStream& in, MapObjects& ep );

QDataStream& operator<<( QDataStream& out, const ExerciseParametersPacket& ep );
QDataStream& operator<<( QDataStream& out, const EnvironmentDataPacket& ep );
QDataStream& operator<<( QDataStream& out, const TargetsDataPacket& ep );
QDataStream& operator<<( QDataStream& out, const MapTargets& ep );
QDataStream& operator<<( QDataStream& out, const AmmunitionDataPacket& ep );
QDataStream& operator<<( QDataStream& out, const Ammunition& ep );
QDataStream& operator<<( QDataStream& out, const ScriptDataPacket& ep );
QDataStream& operator<<( QDataStream& out, const ObjectsDataPacket& ep );
//QDataStream& operator<<( QDataStream& out, const MessagesDataPacket& ep );
//QDataStream& operator<<( QDataStream& out, const FailuresDataPacket& ep );
//QDataStream& operator<<( QDataStream& out, const CommandsDataPacket& ep );
//QDataStream& operator<<( QDataStream& out, const ControlDataPacket& ep );
QDataStream& operator>>( QDataStream& out, ExerciseParametersPacket& ep );
QDataStream& operator>>( QDataStream& out, EnvironmentDataPacket& ep );
QDataStream& operator>>( QDataStream& out, TargetsDataPacket& ep );
QDataStream& operator>>( QDataStream& out, MapTargets& ep );
QDataStream& operator>>( QDataStream& out, AmmunitionDataPacket& ep );
QDataStream& operator>>( QDataStream& out, Ammunition& ep );
QDataStream& operator>>( QDataStream& out, ScriptDataPacket& ep );
QDataStream& operator>>( QDataStream& out, ObjectsDataPacket& ep );
//QDataStream& operator>>( QDataStream& out, MessagesDataPacket& ep );
//QDataStream& operator>>( QDataStream& out, FailuresDataPacket& ep );
//QDataStream& operator>>( QDataStream& out, CommandsDataPacket& ep );
//QDataStream& operator>>( QDataStream& out, ControlDataPacket& ep );

bool operator==( const ExerciseParametersPacket& lhs, const ExerciseParametersPacket& rhs );
bool operator==( const EnvironmentDataPacket& lhs, const EnvironmentDataPacket& rhs );
bool operator==( const AmmunitionDataPacket& lhs, const AmmunitionDataPacket& rhs );
bool operator==( const TargetsDataPacket& lhs, const TargetsDataPacket& rhs );
bool operator==( const ScriptDataPacket& lhs, const ScriptDataPacket& rhs );
bool operator==( const ObjectsDataPacket& lhs, const ObjectsDataPacket& rhs );

class Helicopter_Instructor : public QObject
{
    Q_OBJECT
public:
    //служебные параметры упражнения//
    bool exerciseReInit = false;        // переинициализировать все параметры имитатора
    bool exerciseStarted = false;       // упражнение работает
    bool exerciseStop = false;          // упражнение завершено
    qint32 exerciseTime = 0;            // прошедшее время с момента старта для текущего упражнения (сек)
    qint32 exerciseTimeOld = 0;         // прошедшее время с момента старта для предыдущего упражнения (сек)
    bool commandMoveForward = false;    // команда "Вперед" визуализацией обработана

    InstructorTCPHeader PacketHeader;               //заголовок пакетов//
    ExerciseParametersPacket ExerciseParameters;    //параметры упражнения//
    ObjectsDataPacket Objects;          //объекты на карте//
    TargetsDataPacket Targets;          //мишени на карте//
    EnvironmentDataPacket Environment;  //параметры окружающей среды//
    //боеприпасы//
    AmmunitionDataPacket AmmunitionInPack;   // начальное состояние
    AmmunitionDataPacket AmmunitionOutPack;  // текущее состояние
    CameraDataPacket Camera;            //переключение вида//
    ControlDataPacket Control;          //управление комплексом//
    FailuresDataPacket Failures;        //ввод отказов//
    CommandsDataPacket Commands;        //команды инструктора//
    MessagesDataPacket Messages;        //текстовые сообщения//
    StatisticsDataPacket Statistics;    //статистика//
    ScriptDataPacket script;            // авиацинные параметры упражнеения
    AviaDataPacket Avia;                // Состояние техники на карте (пешки)
    TestVidStepsDataPacket TestVidSteps; // команда на тестирование стрелочных приборов
    ProjectorDataPacket PJ;             // Состояние проекторов

    // параметры передаваемые во модель полета
    float   weight = 8100.f;    // 8100 это снаряженный вес без топлива, пока нет инструктора
    float   fuel = 1500.f;
    float   ct = 0.f;
    double  StartLatitude;       // Широта
    double  StartLongitude;      // Долгота
    float   course = 0.f;
    bool    WindChanged = false;
    Weather weather;            // параметры погоды для модели и карты

    //отказы
    Fault fault;

    explicit Helicopter_Instructor( QObject* parent = nullptr );
    ~Helicopter_Instructor();
    void Init();                       // инициализация параметров
    void ExerciseReStart();
    void ExerciseStop();
    void UnloadAmmo();
    // работа с исправностью компонентов, они сами регистрируют изменения своего сосояния
    void SetComponentStatusChanged() { ComponentStatusChanged = true; }
    bool GetComponentStatusChanged() { auto ret = ComponentStatusChanged; ComponentStatusChanged = false; return ret; }
protected:
    bool ComponentStatusChanged = true;    // изменение состояни одного из компонетов, фиксируется самими компонентами
    QTimer* TimerExerciseTime;

};

#endif // Helicopter_INSTRUCTOR_H
