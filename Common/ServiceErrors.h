#ifndef SERVICEERRORS_H
#define SERVICEERRORS_H

#include <QObject>
#include <QTime>
#include <QTimer>
#include <array>


constexpr int N_ERRORS = 259;
constexpr int N_FAULTS = 259;

struct TErrors
{
    QString Name;     // название ошибки
    bool Active;      // состояние ошибки
    uint DeltaT;      // время в секундах в течении которого ошибка не фиксируется
    QTime Time;       // время фиксации ошибки
    bool Send;        // отправлять сообщение об ошибке
};

struct TFaultInjection
{
    QString Name;     // название отказа
    bool Active;      // активность неисправности
    QTime Time;       // время введенния руководителем неисправностей
    uint DeltaT;      // время в секундах действия неисправностей
};

class T_ServiceErrors : public QObject
{

  Q_OBJECT

  public:
    T_ServiceErrors();
    ~T_ServiceErrors();
    std::array < TErrors, N_ERRORS> Errors;
    std::array<TFaultInjection, N_FAULTS> FaultInjection;
    void Init();
  protected:
    QTimer *ErrorsTimer;

};

#endif // SERVICEERRORS_H
