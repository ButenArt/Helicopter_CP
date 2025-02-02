#ifndef AUTOPILOT_H
#define AUTOPILOT_H

#include <QObject>
#include <QTimer>
#include <QElapsedTimer>
#include <QDebug>
#include <QSettings>
#include <math.h>
#include "TransferFunction.h"

struct DeltaParamAP
{
    bool flagOldAP = true;             // чей АП использовать true(Дедовский), false(Новый Амира) 
    float d_roll_AP = 1.0;            // дельта для вращалки по крену
    float d_pitch_AP = 1.0;           // дельта для вращалки по тангажу
    float d_yaw_AP = 1.0;             // дельта для вращалки по направлению
    float range_roll_AP = 100;        // коридор при котором не отключается АП по крену
    float range_pitch_AP = 100;       // коридор при котором не отключается АП по тангажу
    float range_yaw_AP = 100;         // коридор при котором не отключается АП по направлению
    float range_height_AP = 100;      // коридор при котором не отключается АП по высоте
    float k_newAP = 1.0;              // Коэффициент для нового АП
};

class Autopilot: public QObject
{
    Q_OBJECT

public:
    Autopilot(QObject *parent = nullptr);
    ~Autopilot();

    // функции вкл/выкл каналов
    void turnOn_roll_pitch();
    void turnOff_roll_pitch();
    void turnOn_yaw(); 
    void turnOff_yaw();
    void turnOn_speed();
    void turnOff_speed();
    void turnOn_height();
    void turnOff_height();

    // функции задания параметров расчета
    void setPitchParams(const float jy, const float pitch, const float w_theta, const float pitch_set);
    void setSpeedParams(const float speed);
    void setRollParams(const float jx, const float roll, const float w_gamma, const float roll_set);
    void setYawParams(const float px, const float yaw, const float w_psi, const float yaw_set, const bool pd);
    void setHeightParams(const float fosh, const float height);

    // сигналы для модели
    float getPitchCoords()const;
    float getRollCoords()const;
    float getYawCoords()const;
    float getHeightCoords()const;
    
    // состояние работы автопилота
    bool getRollPitchState()    const { return ap_roll_pitch; }
    bool getYawState()          const { return ap_yaw; }
    bool getSpeedState()        const { return ap_speed; }
    bool getHeightState()       const { return ap_height; }

    // разница сигналов летчика и АП
    float getRollDifference()   const;
    float getPitchDifference()  const;
    float getYawDifference()    const;
    float getHeightDifference() const;

    void calculate_delta();

    float getFlagAP() const { return dParamAP.flagOldAP;       }
    float getDeltaRoll() const { return dParamAP.d_roll_AP;       }
    float getDeltaPitch() const { return dParamAP.d_pitch_AP;      }
    float getDeltaYaw() const { return dParamAP.d_yaw_AP;        }
    float getRangeRoll() const { return dParamAP.range_roll_AP;   }
    float getRangePitch() const { return dParamAP.range_pitch_AP;  }
    float getRangeYaw() const { return dParamAP.range_yaw_AP;    }
    float getRangeHeght() const { return dParamAP.range_height_AP; }
    float getNewAPdelta() const { return dParamAP.k_newAP;         }

    friend QDebug operator<<(QDebug out, const Autopilot& ap);
    DeltaParamAP dParamAP;

private:
    QSettings* ini;

    // общие параметры каналов курса и крена
    struct Params
    {
        float k;            // k = screw step (delta) / angle
        float k_pilot;      // k = screw step (delta) / pilot given angle
        float kw;           // kw = screw step (delta) / angular velocity (w)
        float d_pilot;      // сигналы с органов управления
        float angle;        // реальный угол
        float set_angle;    // заданный угол
        float w;            // угловая скорость
        float set;          // сигнал с рукоятки задания
    };

    Params roll_params;
    Params yaw_params;
    Params pitch_params;
    
    float k_speed;
    float set_speed;
    float speed;

    struct
    {
        float k;
        float height;
        float set_height;
        float set;
        float d_pilot;
    } height_params; // канал высоты

    // вкл/выкл каналы
    bool ap_roll_pitch;
    bool ap_yaw;
    bool pedal; // состояние нажатия на педали
    bool ap_speed;
    bool ap_height;

    // значения сигналов для модели
    float delta_roll = 0.f;
    float delta_roll_calc = 0.f;
    float delta_pitch = 0.f;
    float delta_pitch_calc = 0.f;
    float delta_yaw = 0.f;
    float delta_height = 0.f;

    // разница сигналов летчика и АП
    float diff_roll = 0.f;
    float diff_pitch = 0.f;
    float diff_yaw = 0.f;
    float diff_height = 0.f;

    // функции расчета
    void calc_roll();
    void calc_pitch();
    void calc_yaw();
    void calc_height();
    float saturation(const float ap_input, const float p_input, const float limit);

    // фильтры и доп. математика
    // 
    // для получения производной
    //    s
    //  -----
    //  s + 1
    TransferFunction derivative = TransferFunction(-1, 1, -1, 1);   

    // сглаживает угловую скорость по тангажу
    //    Ts
    //  ------
    //  Ts + 1
    TransferFunction pitch_filter = TransferFunction(-2, 1, -2, 1);

    // сглаживает сигнал РУ по крену
    //        Ts         1
    //  d - ------ d = ------ d
    //      Ts + 1     Ts + 1
    TransferFunction roll_filter = TransferFunction(-2, 1, 2, 0);   

    // сглаживает угловую скорость по рысканию
    //    Ts
    //  ------
    //  Ts + 1
    TransferFunction yaw_filter = TransferFunction(-2, 1, -2, 1); // Работает криво (не использовать)

};

#endif // AUTOPILOT_H
