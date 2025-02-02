#ifndef R_TCP_CLIENT_H
#define R_TCP_CLIENT_H

#include "Helicopter_core.h"
#include "Helicopter_Model.h"
#include "Common/TCP_client.h"
#include "Helicopter_instructor.h"
//#include "Common/SettingsFile.h"


class Helicopter_Rita_TCP_Client : public TCP_Client
{
    Q_OBJECT

public:
    Helicopter_Rita_TCP_Client(Helicopter_core* Helicopter_);
    ~Helicopter_Rita_TCP_Client();

    void SendRitaDataPacket(const int track_number);

public slots:
    void Start();
    void Stop();

private:
    static const int faults_count = 46;
    Helicopter_core* Helicopter;
    //FParam* FPar;
    //IParam* IPar;
    QTimer *timer;

    bool* isTurnedOn;           

    bool flag_H = false;
    bool ri_off = false;        // 0
    bool danger_h = false;      // 41
    bool danger_v = false;      // 55
    bool NV_low = false;        // 14
    bool NV_high = false;       // 15
    bool min_P_left = false;    // 28
    bool min_P_right = false;   // 29
    bool min_P_red = false;     // 30
    bool danger_fuel = false;   // 21
    bool ERD_left = false;      // 51
    bool ERD_right = false;     // 52
    bool t_overspeed1 = false;  // 11
    bool t_overspeed2 = false;  // 12
    bool vibrLeft = false;      // 17
    bool vibrRight = false;      // 18
    bool emerg_left = false;    // 31
    bool emerg_right = false;   // 32
    bool limited_left = false;  // 53
    bool limited_right = false; // 54
    bool icing = false;         // 22
    bool left_tf = false;       // 43
    bool right_tf = false;      // 44
    bool pump_ras = false;      // 8
    bool pump_left = false;     // 25
    bool pump_right = false;    // 26
    bool check_ri = false;      // 56
    bool ri_Hleb = false;       // 57) Позывной Хлебодаровки
    bool ri_Aktobe = false;     // 58) Позовной Актобе
    bool ri_shum = false;       // 59) Тональный шум
    bool kn53_pozuv = false;    // 60  позывной ILS
        
    bool* faults[faults_count];
    bool flags[faults_count];

    int track_numbers[faults_count] = {
        1,  2,  3,  4,  7,  8,  10, 11, 12,
        14, 15, 17, 18, 19, 20, 21, 22, 25, 
        26, 28, 29, 30, 31, 32, 33, 34, 35, 
        40, 41, 43, 44, 46, 47, 48, 51, 52, 
        53, 54, 55, 56, 0,  57, 58, 59, 60
    };
        //1,2,3,4,7,8,10,11,12,
        //14,15,17,18,19,20,21,22,
        //25,26,28,29,30,31,32,33,
        //34,35,40,41,43,44,46,47,
        //48,51,52,53,54,55,56
private slots:
    void On_CheckParams();
    void RitaOFFSlot();
    void On_Error();
    void On_Connected();
    void On_Disconnected();

};

#endif // R_TCP_CLIENT_H
