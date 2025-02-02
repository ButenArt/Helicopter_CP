#pragma once
#ifndef Helicopter_JOYSTICK_H
#define Helicopter_JOYSTICK_H

#include <QObject>
#include <QTimer>
#include <QMainWindow>
#include <QDebug>
#include "Common/Joystick/LogiControllerInput.h"
#include "Common/Logging.h"

//using namespace LogitechControllerInput;
//namespace LogitechControllerInput

namespace LCI = LogitechControllerInput;

struct JoystickInput
{
    // оси
    // РУС
    float X = 127.;          // продольный канал
    float Y = 127.;          // поперечный канал
    // ШАГ-ГАЗ
    float CorLEng = 127.;    // коррекция левого двигателя
    float CorREng = 127.;    // коррекция правого двигателя
    float Fosh = 0.;         // общий шаг 
    float Ecor = 255.;       // коррекция оборотов двигателя
    bool ADirOn = false;     // автопилот по курсу
    bool APitchOn = false;   // автопилот по тангажу и крену
    bool AAltOn = false;     // автопилот по высоте
    bool ADirOff = false;    // автопилот по курсу
    bool AAltOff = false;    // автопилот по высоте
    // педали
    float Xp = 127.;         // положение педалей 
    float Break = 255.;      // тормоз на педалях
    //кнопки
    bool Gashetka = false;    // гашетка под указательным пальцем
    bool Fire = false;       // красная кнопка fire
    bool Engine1 = false;     // запуск двигателей TODO: пока все двигатели запускаются одновременно логика запуска не реализована
    bool Engine2 = false;     // запуск двигателей TODO: пока все двигатели запускаются одновременно логика запуска не реализована
    bool APOff = false;       // кнопка S3 переключение работы автопилота (должна только отключать, но пока нечем включить)
    bool Trim = false;        // кнопка S4 переключение работы  согласования автопилота, тример
    bool Save = false;        // кнопка S5 под мизенцем
    JoystickInput() = default;
};


class Helicopter_Joystick : public QObject
{
Q_OBJECT

public:
    Helicopter_Joystick(QMainWindow *mainwindow, QSharedPointer<TLogging> log, QObject* parent = nullptr);
    ~Helicopter_Joystick();
    void Init();
    bool JoystickActive() { return _JoystickActive; }
    void Start();  // запускает таймер с высокой частотой опроса
    void Stop();
    JoystickInput joystickInput;
private:
    QTimer *joystickTimer;
    QSharedPointer<TLogging> _log;
    LCI::ControllerInput *g_controller;
    bool _JoystickActive;
    float cor_x = 127.f; //153.f;
    float cor_y = 127.f; //153.f;
    float cor_z = 127.f;
    bool unblock_x = true;
    bool unblock_y = true;
private slots:
    void slotJoystick();
};

#endif // T72_JOYSTICK_H
