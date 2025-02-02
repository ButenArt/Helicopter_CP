#ifndef SETTINGSFILE_H
#define SETTINGSFILE_H

#include <QObject>
#include <QCoreApplication>
#include <QFile>
#include <QFileInfo>
#include <QSettings>
#include <QDateTime>
#include <Windows.h>
#include <array>

const auto N_UNITS = 16;
const auto N_V_UNITS = 100;

class TSettingsFile : public QObject
{
  Q_OBJECT

  public:
    TSettingsFile();
    ~TSettingsFile();
    bool WindowMinimize() const { return _WindowMinimize; }
    bool CanLoggingEnabled() const { return _CanLoggingEnabled; }
    bool LanLoggingEnabled() const { return _LanLoggingEnabled; }
    bool SoundsEnabled() const { return _SoundsEnabled; }
    bool ManualMode() const { return _ManualMode; }
    WORD CanTxInterval() const { return _CanTxInterval; }
    WORD CanRxInterval() const { return _CanRxInterval; }
    QString Board( ) const { return _Board; }
    int SimulatorType() const { return _SimulatorType; }

    // TODO: Внимание! тут дополнительно захардкожены параметры конфигурации, количество элементов! Будте внимательны!
    WORD MABUnitAddr(int port) const { assert(port >= 0 && port < N_UNITS ); return _MAB[port].Addr; }
    BYTE MABUnitPort(int port) const { assert(port >= 0 && port < N_UNITS ); return _MAB[port].Port; }
    bool MABUnitEnable(int port) const { assert(port >= 0 && port < N_UNITS ); return _MAB[port].Enable; }

    WORD VIDUnitAddr(int port) const { assert(port >= 0 && port < N_V_UNITS ); return _VID[port].Addr; }
    BYTE VIDUnitPort(int port) const { assert(port >= 0 && port < N_V_UNITS ); return _VID[port].Port; }
    bool VIDUnitEnable(int port) const { assert(port >= 0 && port < N_V_UNITS ); return _VID[port].Enable; }

    WORD GPIOINUnitAddr(int port) const { assert(port >= 0 && port < N_UNITS ); return _GPIOIN[port].Addr; }
    BYTE GPIOINUnitPort(int port) const { assert(port >= 0 && port < N_UNITS ); return _GPIOIN[port].Port; }
    bool GPIOINUnitEnable(int port) const { assert(port >= 0 && port < N_UNITS ); return _GPIOIN[port].Enable; }

    WORD GPIOOUTUnitAddr(int port) const { assert(port >= 0 && port < N_UNITS ); return _GPIOOUT[port].Addr; }
    BYTE GPIOOUTUnitPort(int port) const { assert(port >= 0 && port < N_UNITS ); return _GPIOOUT[port].Port; }
    bool GPIOOUTUnitEnable(int port) const { assert(port >= 0 && port < N_UNITS ); return _GPIOOUT[port].Enable; }

    WORD POTUnitAddr(int port) const { assert(port >= 0 && port < N_UNITS ); return _POT[port].Addr; }
    BYTE POTUnitPort(int port) const { assert(port >= 0 && port < N_UNITS ); return _POT[port].Port; }
    bool POTUnitEnable(int port) const { assert(port >= 0 && port < N_UNITS ); return _POT[port].Enable; }

    WORD ENCUnitAddr( int port ) const { assert( port >= 0 && port < N_UNITS ); return _ENC[port].Addr; }
    BYTE ENCUnitPort( int port ) const { assert( port >= 0 && port < N_UNITS ); return _ENC[port].Port; }
    bool ENCUnitEnable( int port ) const { assert( port >= 0 && port < N_UNITS ); return _ENC[port].Enable; }

    // LED находятся по адресам GPIO_IN
    //WORD LEDUnitAddr( int port ) const { assert( port >= 0 && port < N_UNITS ); return _LED[port].Addr; }
    //BYTE LEDUnitPort( int port ) const { assert( port >= 0 && port < N_UNITS ); return _LED[port].Port; }
    //bool LEDUnitEnable( int port ) const { assert( port >= 0 && port < N_UNITS ); return _LED[port].Enable; }

    // LСD находятся по адресам MAB
    //WORD LCDUnitAddr( int port ) const { assert( port >= 0 && port < N_UNITS ); return _LCD[port].Addr; }
    //BYTE LCDUnitPort( int port ) const { assert( port >= 0 && port < N_UNITS ); return _LCD[port].Port; }
    //bool LCDUnitEnable( int port ) const { assert( port >= 0 && port < N_UNITS ); return _LCD[port].Enable; }

    WORD LanTxInterval() const { return _LanTxInterval; }
    WORD LanRxInterval() const { return _LanRxInterval; }
    QString IP_Imager(int ind=0) const { return _IP_Imager[ind]; }
    WORD PORT_Imager(int ind=0) const { return _PORT_Imager[ind]; }
    QString IP_Instructor() const { return _IP_Instructor; }
    WORD PORT_Instructor() const { return _PORT_Instructor; }
    QString IP_ObjectivControl() const { return _IP_ObjectivControl; }
    WORD PORT_ObjectivControl() const { return _PORT_ObjectivControl; }
    QString IP_ScriptServer() const { return _IP_ScriptServer; }
    WORD PORT_ScriptServer() const { return _PORT_ScriptServer; }
    QString IP_OU(int ind=0) const { return _IP_OU[ind]; }
    WORD PORT_OU(int ind=0) const { return _PORT_OU[ind]; }

    QString IP_Rita() const { return _IP_Rita; }
    WORD PORT_Rita() const { return _PORT_Rita; }
    QString IP_Bort_Teh() const { return _IP_Bort_Teh; }
    WORD PORT_Bort_Teh() const { return _PORT_Bort_Teh; }
    QString IP_Left_Pilot() const { return _IP_Left_Pilot; }
    WORD PORT_Left_Pilot() const { return _PORT_Left_Pilot; }
    QString IP_Right_Pilot() const { return _IP_Right_Pilot; }
    WORD PORT_Right_Pilot() const { return _PORT_Right_Pilot; }
    QString IP_CARCOMP() const { return _IP_CARCOMP; }
    WORD PORT_CARCOMP() const { return _PORT_CARCOMP; }
    QString IP_ScreenItems() const { return _IP_ScreenItems; }
    WORD PORT_ScreenItems() const { return _PORT_ScreenItems; }
    QString IP_MotionSystemGunner() const { return _IP_AviaMotionSystem; }
    WORD PORT_MotionSystemGunner() const { return _PORT_AviaMotionSystem; }

    int FR_x() const { return _FR_x; }
    int FR_y() const { return _FR_y; }

    QString IP_ControllerServo() { return _IP_ControllerServo; }
    WORD PORT_ControllerServo() { return _PORT_ControllerServo; }

    QString IP_UPU() { return _IP_UPU; }
    WORD PORT_UPU() { return _PORT_UPU; }
    QString IP_MFI( ) { return _IP_MFI; }
    WORD PORT_MFI( ) { return _PORT_MFI; }
    QString IP_OPI() { return _IP_OPI; }
    WORD PORT_OPI() { return _PORT_OPI; }
    QString IP_OC() { return _IP_OC; }
    WORD PORT_OC() { return _PORT_OC; }
    QString IP_RI() { return _IP_RI; }
    WORD PORT_RI() { return _PORT_RI; }


    QString IP_PROJECTORS() { return _IP_PROJECTORS; }
    QString PASS_PROJECTOR() { return _PASS_PROJECTORS; }
    WORD PORT_PROJECTORS() { return _PORT_PROJECTORS; }
    WORD TIME_PROJECTORS() { return _TIME_PROJECTORS; }

    qint64 ReadVerificationRegistryKey(QString key, bool rewrite);

  private:
    bool _WindowMinimize;          // сворачивать основное окно приложения
    bool _CanLoggingEnabled;       // флаг активации журналирования CAN
    bool _LanLoggingEnabled;       // флаг активации журналирования LAN
    bool _SoundsEnabled;           // флаг активации звуков
    bool _ManualMode;              // флаг режима работы сервера - ручное управление или CAN
    int _SimulatorType;            // тип тренажера, 19 кабина, 20 ведомый
    WORD _CanTxInterval;           // интервал в мс между отправкой CAN пакетов
    WORD _CanRxInterval;           // интервал в мс между приемом CAN пакетов
    QString _Board;                // строка с названием САН платы MOXA PCAN VIRT

    struct CAN {                   // CAN agent
        WORD Addr;                 // адрес CAN блока 
        bool Enable;               // активность CAN блока
        BYTE Port;                 // порт CAN блока
    };

    std::array<CAN, N_UNITS>  _MAB;
    std::array<CAN, N_V_UNITS>  _VID;
    std::array<CAN, N_UNITS>  _GPIOIN;
    std::array<CAN, N_UNITS>  _GPIOOUT;
    std::array<CAN, N_UNITS>  _POT;
    std::array<CAN, N_UNITS>  _ENC;
    //std::array<CAN, N_UNITS>  _LED;
    //std::array<CAN, N_UNITS>  _LCD;
    WORD _LanTxInterval;           // интервал в мс отправки LAN пакетов
    WORD _LanRxInterval;           // интервал в мс приема LAN пакетов
    QString _IP_Imager[8];            // IP адрес TCP сервера визуализации
    WORD _PORT_Imager[8];             // порт TCP сервера визуализации

    QString _IP_Instructor;        // IP адрес TCP сервера иструктора
    WORD _PORT_Instructor;         // порт TCP сервера иструктора
    QString _IP_ScriptServer;      // IP адрес TCP сервера сценариев
    WORD _PORT_ScriptServer;       // порт TCP сервера сценариев

    QString _IP_OU[5];  // IP адрес TCP имитатора органов управления
    WORD _PORT_OU[5];   // порт TCP имитатора органов управления
    
    QString _IP_Rita;
    WORD _PORT_Rita;
    QString _IP_Bort_Teh;
    WORD    _PORT_Bort_Teh;
    QString _IP_Left_Pilot;
    WORD    _PORT_Left_Pilot;
    QString _IP_Right_Pilot;
    WORD    _PORT_Right_Pilot;

    QString _IP_ObjectivControl;   // IP адрес ObjectivControl
    WORD _PORT_ObjectivControl;    // порт TCP ObjectivControl
    QString _IP_CARCOMP;           // IP адрес TCP имитатора бортового компьютера автомобиля
    WORD _PORT_CARCOMP;            // порт TCP имитатора бортового компьютера автомобиля
    QString _IP_ScreenItems;       // IP адрес TCP имитатора приборов на экране
    WORD _PORT_ScreenItems;        // порт TCP имитатора приборов на экране
    QString _IP_ControllerServo;   // IP адрес ADS контроллера сервопривода
    WORD _PORT_ControllerServo;    // порт ADS контроллера сервопривода
    QString _IP_AviaMotionSystem;// IP адрес TCP сервера системы подвижности стрелка-наводчика
    WORD _PORT_AviaMotionSystem; // порт TCP сервера системы подвижности стрелка-наводчика

    QString _IP_UPU;               // IP адрес УПУ
    WORD _PORT_UPU;                // порт УПУ
    QString _IP_MFI;               // IP МФИ
    WORD _PORT_MFI;                // порт МФИ
    QString _IP_OPI;               // IP ОПИ
    WORD _PORT_OPI;                // порт ОПИ
    QString _IP_OC;               // IP OC
    WORD _PORT_OC;                // порт OC
    QString _IP_RI;               // IP RI
    WORD _PORT_RI;                // порт RI

    QString _IP_PROJECTORS;       // IP проекторов
    QString _PASS_PROJECTORS;     // пароль проекторов
    WORD _PORT_PROJECTORS;        // порт проекторов
    WORD _TIME_PROJECTORS;        // время опроса проекторов

    int _FR_x;  // позиционирование СОК
    int _FR_y;  

    void ReadConfigINI(QString INIfilename);
};

#endif // SETTINGSFILE_H
