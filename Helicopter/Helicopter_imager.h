#pragma once
#include <QObject>
#include <QDebug>
#include <Windows.h>
#include "Common/avia_dictionary.h"
#include "Common/server.h"


struct HelicopterMovingDataPacket
{
    int    PacketType = 1;                  // Тип  пакета
    int    PacketSize = 188;                // Размер пакета
    int    ID = 999;                        // ID вертолета
    double Latitude = 50.2570186;           // Широта
    double Longitude = 57.18863743;         // Долгота
    double BarHeight = 2.52;                // Высота
    float  Roll = 0.;                       // Крен
    float  Pich = 3.;                       // Тангаж
    float  Course = 134.;                   // Курс ВПП Актюбинска
    float  HelixRotationSpeed = 0.;         // Скорость вращения винтов
    float  LeftEngineRotationSpeed = 0;     // Обороты  Левого двигателя
    float  RightEngineRotationSpeed = 0;    // Обороты  Правого двигателя
    double HorizontalSx16 = 0;              // управление прожектором SX16 по горизонтали
    double VerticalSx16 = 0;                // управление прожектором SX16 по вертикали
    double FocusSx16 = 20;                  // управление прожектором SX16 фокус светового пятна
    double LeftFppHorizontal = 0;           // управление левый ФПП по горизонтали
    double LeftFppVertical = 0;             // управление левый ФПП по вертикали
    double RightFppHorizontal = 0;          // управление правый ФПП по горизонтали
    double RightFppVertical = 0;            // управление правый ФПП по вертикали
    UINT64 SampleNr = 0;                    // SampleNr(номер сэмпла)
    UINT64 timestamp = 0;                   // Timestamp ( текущее время )
    double matmodelX = 0;                   // matmodelX   дистанция по оси Х от стартового положения из мат модели в м.
    double matmodelY = 225;                 // matmodelY   дистанция по оси Y от стартового положения из мат модели в м.
    double matmodelZ = 0;                   // matmodelZ   дистанция по оси Z от стартового положения из мат модели в м.
    int    Cargo = 0;                       // похоже, это вес подвеса в кг
    int    un2 = 100;                       // значение не понятно, но оно передается из имитатора в таком виде, чем задается пока не разобрался
    double CableLen = 0.;                   // длинна подвеса
    float  speed;                           // скорость вертолета, горизонтальная
    float  vspeed;                          // вертикальная скорость
    float  sideslip;                        // скольжение
    float  proppitch;                       // шаг винта
    double dbl4;                            // резерв
};

struct HelicopterState
{
    UINT32 PacketID=311;           // ID пакета = 311
    UINT32 PacketSize=0;         //Размер пакета
    UINT32 HelixID=0;            //ID вертолета
    float  Hgeo=0;
    float  sh1=0;             //Высота над землей первой опоры
    float  sh2=0;             //Высота над землей второй опоры
    float  sh3=0;             //Высота над землей третьей опоры
    UINT64 SampleNr=0;        //SampleNr(номер сэмпла)
    UINT64 TimeStamp=0;       //TimeStamp
    // cслужебные поля, не передаются по сети
    bool   PacketReady=false;
    HelicopterState() = default;
};

#pragma pack(push, 1)
struct ImagerTCPHeader
{
    quint32 Mark;
    quint32 PacketType;
    quint32 DataSize;
};
#pragma pack(pop)

struct Wheels
{
    float wheelSpeed;                  // Значение скорости колеса (км/ч). Последовательно с переда на зад справа налево.
};

struct Trajectories
{
    qint32 WeaponID = 0;                   // номер оружия
    qint32 WeaponType = 0;                 // тип оружия
    qint32 ProjectileType = 0;             // тип пули
    qint32 ProjectileSubType = 0;          // подтип пули
    float InitialVelocity = 0;             // начальная скорость пули
    qint32 ShotID = 0;                     // Номер выстрела
    float time = 0;                        // Время
    float pos_offset = 0;                  // Смещение
    float pos_height = 0;                  // Высота
    float pos_distance = 0;                // Дистанция
    float rise_angle = 0;                  // Угол вылета снаряда
};

struct TargetControl
{
    qint32 ObjectiveUID = 0;               // UID цели
    qint32 TargetUID = 0;                  // UID мишени
    bool TargetState = 0;                  // Мишень опущена/поднята
};


struct ImagerStateDataPacket
{
    bool PacketReady = 0;                  // готовность пакета
    float X = 0;                           // координата X (метры)
    float Y = 0;                           // координата Y (метры)
    float Z = 0;                           // высота (метры)
    float Roll = 0;                        // крен (градусы -360 .. 360)
    float Pitch = 0;                       // тангаж (градусы -80 .. 80)
    float Course = 0;                      // курс (градусы 0 .. 359)
    bool Sail = 0;                         // плывем
    quint8 Surface = 0;                    // поверхность
    float Speed = 0;                       // скорость
    qint32 WheelsCount = 0;                // количество колес
    QList<Wheels> WheelsList;          // список параметров колес
    float EngineTorque = 0;                // противомомент двигателя
    float RPM = 0;                         // обороты двигателя
    bool  AmmoLoad = false;                // Боеприпасы загружены
};

struct BallisticDataPacket
{
    bool PacketReady = 0;                  // готовность пакета
    qint32 ObjectiveUID = 0;               // UID цели
    qint32 TargetUID = 0;                  // UID мишени
    quint8 ObjectiveType = 0;              // Тип цели
    bool Hit = 0;                          // Попадание
    float Radius = 0;                      // Радиус попадания
    quint8 GunType = 0;                    // Тип оружия
    bool TargetState = 0;                  // Мишень опущена/поднята
    quint8 TriggerID = 0;                  // ID Триггера
    quint8 Direction = 0;                  // Направление
    quint8 Hit_X = 0;                      // Координаты пробоины по X (в % от левого края)
    quint8 Hit_Y = 0;                      // Координаты пробоины по Y (в % от верхнего края)
};

struct ImagerReadyDataPacket
{
    bool PacketReady = 0;                   // готовность пакета
    SimulatorTypes simulatorType = SimulatorTypes::Helicopter;           // тип техники
    bool ImagerReady = 0;                   // Готов
    ImagerReadyDataPacket& operator=( const ImagerReadyDataPacket& rhs );
};

struct TriggerDataPacket
{
    bool PacketReady = 0;                  // готовность пакета
    qint32 ObstacleUID = 0;                // UID препятствия
    quint8 ObstacleType = 0;               // Тип препятствия
    bool ObstacleIn = 0;                   // Въезд на препятствие
    bool ObstacleOut = 0;                  // Выезд из препятствия
    quint8 Trigger = 0;                    // Триггер
};

struct MissedDataPacket
{
    bool PacketReady = 0;                  // готовность пакета
    float X = 0;                           // X координата промаха
    float Y = 0;                           // Y координата промаха
    bool Missed = 0;                       // Въезд на препятствие
};

struct CargoDataPacket
{
    bool PacketReady = 0;                  // готовность пакета
    qint32 Load = 0;                       // Вес подвеса
};

struct TargetsControlDataPacket
{
    bool PacketReady = 0;                  // готовность пакета
    qint32 mapTargetsCount = 0;            // Число мишеней
    QList<TargetControl> targetControlList;
};

struct ProjectileDataPacket
{
    bool PacketReady = 0;                  // готовность пакета
    qint32 TrajectoriesCount = 0;          // количество наборов траекторий
    QList<Trajectories> TrajectoriesList; // Наборы траекторий (структура отрезка траектории снаряда)
};

struct ImagerElementsStateDataPacket
{
    bool PacketReady = 0;                  // готовность пакета
    qint32 ElementsCount = 0;              // Число структур теста функционирования
    QList<Elements> ElementsStateList; // Структура теста функционирования
    quint8 OfflineCounter = 0;             // Счетчик отсутствия соединения
};

struct ExitstateDataPacked
{
    CauseTypes Cause;       // причина остановки симуляции
};

QDebug operator<<( QDebug out, const Elements& ep );
QDebug operator<<( QDebug out, const Wheels& ep );
QDebug operator<<( QDebug out, const BallisticDataPacket& ep );
QDebug operator<<( QDebug out, const ImagerElementsStateDataPacket& ep );
QDebug operator<<( QDebug out, const ImagerReadyDataPacket& ep );
QDebug operator<<( QDebug out, const ImagerStateDataPacket& ep );
QDebug operator<<( QDebug out, const MissedDataPacket& ep );
QDebug operator<<( QDebug out, const CargoDataPacket& ep );
QDebug operator<<( QDebug out, const ProjectileDataPacket& ep );
QDebug operator<<( QDebug out, const TargetControl& ep );
QDebug operator<<( QDebug out, const TargetsControlDataPacket& ep );
QDebug operator<<( QDebug out, const Trajectories& ep );
QDebug operator<<( QDebug out, const TriggerDataPacket& ep );
QDebug operator<<( QDebug out, const ExitstateDataPacked& ep );

bool operator==( const ImagerReadyDataPacket& lhs, const ImagerReadyDataPacket& rhs );
bool operator!=( const ImagerReadyDataPacket& lhs, const ImagerReadyDataPacket& rhs );

class Helicopter_Imager : public QObject
{
    Q_OBJECT
public:
    Helicopter_Imager( QObject* parent = nullptr);
    ~Helicopter_Imager();

    bool ImagerActive_ = false;
    bool ImagerWork = false;
    // данные передаваемые в visual
    HelicopterMovingDataPacket HelixMove;
    //параметры вертолета возвращаемые из visual. храним последние полученные данные
    HelicopterState HelixState;

    ImagerTCPHeader PacketHeader;
    TargetsControlDataPacket TargetsControl;
    ImagerStateDataPacket ImagerState;
    ImagerReadyDataPacket ImagerReady;
    BallisticDataPacket Ballistic;
    TriggerDataPacket Triggers;
    MissedDataPacket Missed;
    CargoDataPacket Cargo;
    ImagerElementsStateDataPacket ImagerElementsState;

    void Init();                       // инициализация параметров
    HelicopterMovingDataPacket& GetHM() { return this->HelixMove; }
    HelicopterState& GetHS() { return this->HelixState; }
    void ImagerStatePacket(HelicopterState packet);
};
