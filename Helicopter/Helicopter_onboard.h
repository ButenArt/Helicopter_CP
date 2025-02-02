#ifndef Helicopter_ONBOARD_H
#define Helicopter_ONBOARD_H

//#include <Windows.h>
#include <QObject>
#include <QTimer>
#include <QThread>
#include "Helicopter_audio.h"
#include "Helicopter_core.h"
#include "Helicopter_Prima.h"
#include "Common/Logging.h"
//#include "Helicopter_Prima.h"


/**
 * @brief The Helicopter_Onboard class
 * Класс отвечает за бортовые системы вертолета
 * Onboard_Compute() это автомат работы бортовых систем
 */
class Helicopter_Onboard : public QObject
{
    Q_OBJECT
private:
    const float FPP_UP =     0.f;   // диапазоны отклонения посадочной фары TODO уточнить
    const float FPP_DOWN = -120.f;
    const float FPP_LEFT = -180.f;
    const float FPP_RIGHT = 180.f;
    std::chrono::steady_clock::time_point KO_start;
    std::chrono::steady_clock::time_point PZUL_start;
    std::chrono::steady_clock::time_point PZUR_start;
public:
    Helicopter_Onboard(Helicopter_core *Helicopter);
    ~Helicopter_Onboard();
    void Onboard_Compute(float dT);
    QSharedPointer<Helicopter_audio> _audio;
private:
    void CalcVibraion();
    void CalcCoordinate();
    void CalcLighting();
    void CalcHidravlic();
    void CalcElectric();
    void CalcFiring();
    void CalcPnevmo();
    void CalcAutoPilot();
    void CalcVSU();
    void CalcEngine();
    void CalcPPS();
    void CalcIcing();
    void CalcDISSContr( );
    void CalcARK( );
    void CalcARK_15( );
    void CalcGMK();
    void CalcVOR();
    void CalcCargo();
    void CalcKO50( );
    void CalcSPUU52( );
    void CalcPZU( );
    void Calc( );
    void CalcBUR();
    void CalcOrlan();
    void CalcKT76C();
    void CalcKN53();
    void CalcKDI();
    //void CalcPrima();
    void CalcUV26();
    void StartEngineProc();
    void StartTAProc();
    Helicopter_core *Helicopter;
    QTimer* FlashTimer;
    bool Flash;                         // мигание

    Helicopter_Prima *pr;
    QTimer *ImitatioColdTimer;
    QTimer *UV26Timer;
    int counter = 0;
    bool flag = false;
    void startCounter() { counter = 1; flag = false; }
    int getCheckCondition() { return counter; }
signals:
    void SendFire();
    void prima_signal();
    void SendPKTFire( bool fire );
    void CargoDrop();
    void PuskUV26Signal(bool left, bool right, size_t s, size_t z);
private slots:
    void on_CargoHooked( quint32 Load );
    void checkCondition() { counter++; }
    void PuskUV26(bool left, bool right, size_t s, size_t z);
    void PuskUV26Slot();
protected:
    [[nodiscard]] float norm360(float angle)
    {
        angle = static_cast<float>(std::fmod(angle + 360.0, 360.0));
        return angle;
    }
};

#endif // Helicopter_ONBOARD_H
