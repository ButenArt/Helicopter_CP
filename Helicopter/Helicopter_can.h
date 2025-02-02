#ifndef Helicopter_CAN_H
#define Helicopter_CAN_H

#include <QCanBus>
#include <QTime>
#include "Common/SettingsFile.h"

const auto N_MAB_UNITS = 4;  // LCD
const auto N_VID_UNITS = 100;
const auto N_IN_UNITS =  11; // LED
const auto N_OUT_UNITS = 6;
const auto N_POT_UNITS = 6;
const auto N_ENC_UNITS = 1;
const auto N_LED_UNITS = 4;
const auto N_LCD_UNITS = 0;

#include "Common/CAN_common.h"

class Helicopter_CAN : public CANBusComm
{
  Q_OBJECT

public:
    Helicopter_CAN( QSharedPointer<TSettingsFile> ini, QSharedPointer<TLogging> log, QObject* parent = nullptr );
    ~Helicopter_CAN();
    void Start();
    void Stop();
    void Reset( bool &reset );  // ссылка не переменную, которая завязана на сброс СОДа
    void HeartBeatEnable();
    void HeartBeatDisable();
//    T_UNITS UNITS;
private:
    QSharedPointer<TSettingsFile> _ini;
    WORD _TxInterval;
    WORD _RxInterval;
    bool _canHeartBeat;
    void ReadCAN(BYTE port);
    QTimer* TimerTX0;   // OTher Port All
    //QTimer* TimerTX1;   // VID первая половина
    //QTimer* TimerTX2;   // VID вторая половина
    //QTimer* TimerTX3;   // VID вторая половина
    std::vector<TVIDData> VIDenable;
private slots:
    //void slotTXVID1();
    //void slotTXVID2();
    //void slotTXVID3();
    //void slotTXother();
    void slotTX();
    void slotRX();
    void slotHeartBeat();
};

#endif //Helicopter_CAN_H