#ifndef Helicopter_H
#define Helicopter_H


#pragma warning( push )
#pragma warning( disable: 26812 )
#include <QMainWindow>
#pragma warning( pop )

#include "Helicopter_joystick.h"
#include "Helicopter_audio.h"
#include "Common/Replay.h"
#include "Helicopter_BoardSystem.h"
#include "Helicopter_Projectors.h"
#include <sstream>
#include <iomanip>

class Helicopter_CAN;
class Helicopter_Server;
class TLogging;
class TSettingsFile;
class Helicopter_controls;
class Helicopter_Imager;
class Helicopter_Model;
class Helicopter_TCP_Client_Imager;
class Helicopter_TCP_Client_Imager_Sfera;
class Helicopter_TCP_Client_Instructor;
class Helicopter_Joystick;
class Helicopter_Instructor;
class Helicopter_ControlsImitator;
class Helicopter_OU_TCP_Client;
class Helicopter_OC_TCP_Client;
class Helicopter_LPP_TCP_Client;
class Helicopter_RPP_TCP_Client;
//class T_TCP_Client_AviaMotionSystem;
class TestCanBoard;
class Helicopter_Projector;
class Autopilot;
class Helicopter_ScriptServer;
class Helicopter_TCP_Client_ScriptServer;
class FlightRecorder;
class FR_Controls;
class Helicopter_Rita_TCP_Client;
class Helicopter_SPU_Bort_Teh_TCP_Client;
class Helicopter_SPU_Left_Pilot_TCP_Client;
class Helicopter_SPU_Right_Pilot_TCP_Client;
class Helicopter_ServiceErrors;
class Helicopter_TargetsManager;
class ServerInstructor;

struct RadioBeacon
{
    double lat;
    double lon;
    double freq;
    QString name;
};

constexpr float PI = 3.14159265358f;
constexpr float GtoR = PI / 180.f;
constexpr float Ra = 180.f / PI;
constexpr float MS2KM = 3.6f;
/**
 * @brief The Helicopter class
 * Класс отвечает за создание и инициализацию всех объектов
 * тренажера, является контейнером всех систем. 
 */
class Helicopter_core : public QObject
{
Q_OBJECT

public:
    struct Location {
        //double lat1m = 0.;
        //double lon1m = 0.;
        double start_lat = 0.;
        double start_lon = 0.;
        double course = 0.;
        double alt = 0.;
        double vpp = 0.;
        double base_alt = 0.;
        QString json_start;
    };

    Helicopter_core(QMainWindow * /*mainwindow*/);
    ~Helicopter_core();
    void Start();
    void Stop();
    bool Is27V() { return _BoardSystem->Is27V(); }
    bool Is208V() { return _BoardSystem->Is208V(); }
    bool Is115V() { return _BoardSystem->_Electrics->Ugen115 > 100.f; }
    bool Is115Vr() { return _BoardSystem->_Electrics->Ugen115r > 100.f; }
    void SetAirPort(const Location& loc);
    void ResetCanCrash();

    QSharedPointer<Helicopter_audio> GetAudio();

    QSharedPointer<Helicopter_audio> audio_;
    QSharedPointer<Helicopter_CAN> can;
    QSharedPointer<Helicopter_Server> server_;
    QSharedPointer<TLogging> log_;
    QSharedPointer<TSettingsFile> _ini;
    QSharedPointer<Helicopter_controls> controls_;
    QSharedPointer<Helicopter_Imager> imager;
    QSharedPointer<Helicopter_Instructor> instructor;
    QSharedPointer<Helicopter_Joystick> joystick_;
    QSharedPointer<Helicopter_Model> Model_;
    QSharedPointer<Helicopter_Projectors> pj_;
    QSharedPointer<Autopilot> AP_;
    QSharedPointer<Helicopter_ScriptServer> scriptserver;
    QSharedPointer<Helicopter_ServiceErrors> errors;
    QSharedPointer<Helicopter_TargetsManager> targetsmanager;
    //QSharedPointer<FlightRecorder> FR_;
    //QSharedPointer<FR_Controls> FR_C;
//    Helicopter_ControlsImitator* controlsimitator_;
    Location runway_;

    // бортовые системы вертолета
    LeftPanelAZS lp_azs;                // Левая панель АЗС
    LeftSidePanelElectropult lsp_elp;   // Левая боковая панель электропульта
    LeftShildElectroPult lsh_elp;       // ЛЕВЫЙ ЩИТОК ЭЛЕКТРОПУЛЬТА
    LeftPanelElectroPult lp_elp;        // ПАНЕЛЬ ЭЛЕКТРОПУЛЬТА
    PultControlDISS pc_diss;            // Пульт Контроля ДИСС
    LeftPilotBoard lpb;                 // отвечает за левую доску пилота вертолета
    CentralElectricPult cep;            // отвечает за средняя панель электропульта(запуск двигателей) вертолета
    CentralControlPanel ccp;            // отвечает за центральный пульт управления вертолета
    CentralPultContr cpc;               // Центральный Пульт Управления
    PanelSelectMode psm;                // Панель Выбора Режима
    PanelSelectCourse psc;              // Панель Выбора Курса
    RghtPanelAZS rp_azs;                // отвечает за АЗС панель вертолета
    RightPanelElectroPult rsp_elp;      // Правая Панель Электропульта НАВИГАЦИЯ
    RightShildElectroPilt rsh_elp;      // Правый Щиток Электропульта
    RightSidePanelElectroPult rsp_ep;   // Правая боковая Панель Электропульта
    RightPilotBoard rpb;                // Правая Приборная Панель
    ElectroPanelElectroPult ep_elp;    // Электрощиток Электропульта ВОЛЬТМЕТРЫ
    PilotControl PC;                    // Рычаги и педали
    UV_26 UV26;                         // отстрел ловушек
    AllWeaponPult WeaponPult;           // Все пульты вооружения
    BoardSystem* _BoardSystem;   // дополнительные переменные, состояния, тригеры итд

    QSharedPointer<Helicopter_TCP_Client_Imager> tcpclient_imager_; // instructor
    QSharedPointer<Helicopter_TCP_Client_Imager_Sfera> tcpclient_imager_1;  // Sfera number
    QSharedPointer<Helicopter_TCP_Client_Imager_Sfera> tcpclient_imager_2;  // Sfera number
    QSharedPointer<Helicopter_TCP_Client_Imager_Sfera> tcpclient_imager_3;  // Sfera number
    QSharedPointer<Helicopter_TCP_Client_Imager_Sfera> tcpclient_imager_4;  // Sfera number
    QSharedPointer<Helicopter_TCP_Client_Imager_Sfera> tcpclient_imager_5;  // Sfera number
    QSharedPointer<Helicopter_TCP_Client_Imager_Sfera> tcpclient_imager_6;  // Sfera number
    QSharedPointer<Helicopter_TCP_Client_Imager> tcpclient_imager_stend; // Stend
    QSharedPointer<Helicopter_OU_TCP_Client> tcpclient_OU_;  // органы управления у инструктора
    QSharedPointer<Helicopter_OU_TCP_Client> tcpclient_LPP_;  // левая приборная панель
    QSharedPointer<Helicopter_OU_TCP_Client> tcpclient_RPP_;  // правая приборная панель
    QSharedPointer<Helicopter_OC_TCP_Client> tcpclient_OC_;    // объективный контроль
    QSharedPointer<Helicopter_Rita_TCP_Client> tcpclient_Rita;
    QSharedPointer<Helicopter_SPU_Bort_Teh_TCP_Client> tcpclient_SPU_BT;
    QSharedPointer<Helicopter_SPU_Left_Pilot_TCP_Client> tcpclient_SPU_LP;
    QSharedPointer<Helicopter_SPU_Right_Pilot_TCP_Client> tcpclient_SPU_RP;
    QSharedPointer<Helicopter_TCP_Client_Instructor> tcpclient_instructor;
    QSharedPointer<Helicopter_TCP_Client_ScriptServer> tcpclient_scriptserver;
    QSharedPointer<ServerInstructor> serverInstructor;

// список аэродромов
// пересчет по долготе
// пересчет по широте
// координаты точки появления 
// курс постановки ЛА
// высота аэродрома
// заданный курс ВПП
// базовая высота визуалки, 2.52 для Клименко и 0 для нашей
// дополнительная информация в json
    QString json_start_bor = "{\"helicoptersList\":[{\"id\":999,\"latitude\":43.354218,\"longitude\":76.87879,\"course\":180.0,\"controlType\":1,\"arming\":1}],\"tanksList\":[{\"id\":1002,\"latitude\":43.354573,\"longitude\":76.878711,\"course\":0.0,\"height\":-0.48}],\"fireList\":[{\"id\":1005,\"latitude\":43.357328,\"longitude\":76.87739,\"width\":100.0,\"height\":100.0}]}";
    QString json_start_alm = "{\"helicoptersList\":[{\"id\":999,\"latitude\":43.3442540,\"longitude\":77.01328300,\"course\":220.0,\"controlType\":1,\"arming\":1}],\"tanksList\":[{\"id\":1002,\"latitude\":43.354573,\"longitude\":76.878711,\"course\":0.0,\"height\":-0.48}],\"fireList\":[{\"id\":1005,\"latitude\":43.357328,\"longitude\":76.87739,\"width\":100.0,\"height\":100.0}]}";
    QString json_start_akx = "{\"helicoptersList\":[{\"id\":999,\"latitude\":50.2570186,\"longitude\":57.18863743,\"course\":135.0,\"controlType\":1,\"arming\":1}],\"tanksList\":[{\"id\":1002,\"latitude\":43.354573,\"longitude\":76.878711,\"course\":0.0,\"height\":-0.48}],\"fireList\":[{\"id\":1005,\"latitude\":43.357328,\"longitude\":76.87739,\"width\":100.0,\"height\":100.0}]}";
    QString json_start_blk = "{\"helicoptersList\":[{\"id\":999,\"latitude\":46.94028716,\"longitude\":75.04745930,\"course\":232.0,\"controlType\":1,\"arming\":1}],\"tanksList\":[{\"id\":1002,\"latitude\":43.354573,\"longitude\":76.878711,\"course\":0.0,\"height\":-0.48}],\"fireList\":[{\"id\":1005,\"latitude\":43.357328,\"longitude\":76.87739,\"width\":100.0,\"height\":100.0}]}";

    const Location Boralday = { /*1. / 111095., 1. / 81542.,*/ 43.35612, 76.87902, 180, 710,   0, 2.52, json_start_bor };
    const Location Almaty   = { /*1. / 111095., 1. / 81542.,*/ 43.34172, 77.01841,  56, 680,  56, 2.52, json_start_alm }; // курс 56.4 236.4
    const Location Aktube   = { /*1. / 111231., 1. / 71697.,*/ 50.25112, 57.19784, 135,  75, 135,  0.0, json_start_akx };
    const Location Hlebodar = { /*1. / 111231., 1. / 71697.,*/ 50.25610, 57.18963, 135,  75, 135,  0.0, json_start_akx };
    //const Location Hlebodar   = { /*1. / 111231., 1. / 71697.,*/ 50.54902, 56.85249, 135,  75, 135,  0.0, json_start_akx };
    const Location Balkhash = { /*1. / 111231., 1. / 71697.,*/ 46.90095, 75.01697, 232, 225, 232,  0.0, json_start_blk };

    std::vector<RadioBeacon> ARK_Aktube = { { 50.25, 57.179722, 400, "D" } };
    const RadioBeacon VOR_Aktube = { 50.26341, 57.18185, 113.4, "AKB" };

private:
    QThread thread_audio;
    QThread thread_model;
};
#endif // Helicopter_H