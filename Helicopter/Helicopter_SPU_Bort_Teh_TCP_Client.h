#ifndef BT_TCP_CLIENT_H
#define BT_TCP_CLIENT_H

#include "Helicopter_core.h"
#include "Helicopter_Model.h"
#include "Common/TCP_client.h"
#include "Helicopter_instructor.h"


class Helicopter_SPU_Bort_Teh_TCP_Client : public TCP_Client
{
    Q_OBJECT

public:
    Helicopter_SPU_Bort_Teh_TCP_Client(Helicopter_core* Helicopter_);
    ~Helicopter_SPU_Bort_Teh_TCP_Client();

    void SendSpuBortTehDataPacket(const int track_number);

public slots:
    void Start();
    void Stop();

private:
    static const int faults_count = 5;
    Helicopter_core* Helicopter;
    QTimer *timer;      

    bool flag_H = false;
    bool ri_off = false;        // 0
    bool ri_Hleb = false;       // 57) Позывной Хлебодаровки
    bool ri_Aktobe = false;     // 58) Позовной Актобе
    bool ri_shum = false;       // 59) Тональный шум
    bool kn53_pozuv = false;    // 60  позывной ILS
        
    bool* faults[faults_count];
    bool flags[faults_count];

    int track_numbers[faults_count] = {
        0,  57, 58, 59, 60
    };

private slots:
    void On_CheckParams();
    void On_Error();
    void On_Connected();
    void On_Disconnected();
    void RitaOFFSlot();
};

#endif // LP_TCP_CLIENT_H
