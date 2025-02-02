#pragma once
#include <QObject>
#include <QQueue>
#include <Windows.h>

#pragma pack(push, 1)
typedef struct
{
    quint32 MagicNumber;
    quint16 PacketType;
    quint16 DataSize;
} MotionSystemTCPHeader;
#pragma pack(pop)

typedef struct
{
    bool PacketReady;                  // готовность пакета
    double matmodelZ;                  // дистанция по оси Z от стартового положения из мат модели (метры)
    float Roll;                        // крен (градусы -360 .. 360)
    float Pitch;                       // тангаж (градусы -80 .. 80)
    float Course;                      // курс (градусы 0 .. 359)
} MotionSystemStateDataPacket;

typedef struct
{
    bool PacketReady;                  // готовность пакета
    double X;                          // дистанция по оси Х от стартового положения из мат модели (метры)
    double Y;                          // дистанция по оси Y от стартового положения из мат модели (метры)
    double Z;                          // дистанция по оси Z от стартового положения из мат модели (метры)
    float Roll;                        // крен (градусы -360 .. 360)
    float Pitch;                       // тангаж (градусы -80 .. 80)
    float Course;                      // курс (градусы 0 .. 359)
} MotionSystemFeedbackDataPacket;


class T_MotionSystem : public QObject
{
    Q_OBJECT
public:
    bool AviaMotionSystemActive;
    //макимальная длина очереди//
    const quint8 QueueMaxLength = 5;
    //заголовок пакетов//
    MotionSystemTCPHeader AviaPacketHeader;
    //заданное положение платформы//
    MotionSystemStateDataPacket *AviaMotionState;
    QQueue<MotionSystemStateDataPacket> AviaMotionStateQueue;
    //возвращенное состояние платформы//
    MotionSystemFeedbackDataPacket *AviaMotionFeedback;
    QQueue<MotionSystemFeedbackDataPacket> AviaMotionFeedbackQueue;

    explicit T_MotionSystem(QObject *parent = 0);
    ~T_MotionSystem();
    virtual void Init();
    virtual int GetAviaQueueBuffer();
    virtual void ClearAviaQueueBuffer();
    void AviaMotionStateEnqueuePacket(MotionSystemStateDataPacket packet);
    void AviaMotionStateDequeuePacket();
    void AviaMotionFeedbackEnqueuePacket(MotionSystemFeedbackDataPacket packet);
    void AviaMotionFeedbackDequeuePacket();
};
