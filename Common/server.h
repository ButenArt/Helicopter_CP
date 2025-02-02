#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTimer>
#include <QDateTime>
#include "avia_dictionary.h"
#include <QDebug>

struct Elements
{
    quint8 ElementType;                            // Элемент тренажера
    quint8 ElementState;                           // Неисправность элемента тренажера
    qint32 ElementNameSize;                        // Длина названия
    QString ElementName;                           // Название
    qint32 CommentSize;                            // Длина описания
    QString Comment;                               // Описание
};

class T_Server : public QObject
{
    Q_OBJECT
public:
    explicit T_Server(QObject *parent = 0);
    ~T_Server();
    //готов к упражнению//
    struct ServerReadyDataPacket;
    ServerReadyDataPacket *ServerReady;
    Elements elements;                             // структура состояния элементов тренажера
    QString CANBusStateMsg;                        // сообщение о состоянии CAN блоков тренажера
    ElementsStateTypes CANBusState;                // Состояние CAN блоков тренажера
    ElementsStateTypes ResidentCabinState;         // Состояние резидента механика-водителя
    ElementsStateTypes JoystickState;              // Состояние джойстика
    qint32 clockGenerator;                         // тактовый генератор в милисекундах с момента запуска сервера
    bool ResetPower;                               // общий сброс питания тренажера
    quint16 ResetPowerTickCounter;                 // количество отработанных итераций после сброса питания
    virtual void Init();
private:
    QDateTime startingDateTime;
    QTimer *clockGeneratorTimer;
private slots:
    void slotClockGenerator();
};


struct T_Server::ServerReadyDataPacket
{
    bool PacketReady;                  // готовность пакета
    SimulatorTypes simulatorType;      // Тип симулятора (техники)
    bool ServerReady;                  // Готов
};

QDebug operator<<( QDebug out, const T_Server::ServerReadyDataPacket& ep );


#endif // SERVER_H
