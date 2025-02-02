#ifndef AVIAMOTION_TCP_CLIENT_H
#define AVIAMOTION_TCP_CLIENT_H

#include <QDataStream>
#include "Common\TCP_client.h"
#include "Helicopter_core.h"

#pragma pack(push, 1)
struct MotionSystemTCPHeader
{
    quint32 MagicNumber;
    quint16 PacketType;
    quint16 DataSize;
};
#pragma pack(pop)

struct MotionSystemStateDataPacket
{
    bool PacketReady;                  // готовность пакета
    double matmodelZ;                  // дистанция по оси Z от стартового положения из мат модели (метры)
    float Roll;                        // крен (градусы -360 .. 360)
    float Pitch;                       // тангаж (градусы -80 .. 80)
    float Course;                      // курс (градусы 0 .. 359)
};

struct MotionSystemFeedbackDataPacket
{
    bool PacketReady;                  // готовность пакета
    double X;                          // дистанция по оси Х от стартового положения из мат модели (метры)
    double Y;                          // дистанция по оси Y от стартового положения из мат модели (метры)
    double Z;                          // дистанция по оси Z от стартового положения из мат модели (метры)
    float Roll;                        // крен (градусы -360 .. 360)
    float Pitch;                       // тангаж (градусы -80 .. 80)
    float Course;                      // курс (градусы 0 .. 359)
};

QDebug operator<<(QDebug out, const MotionSystemFeedbackDataPacket& msfbd);

class T_TCP_Client_AviaMotionSystem : public TCP_Client
{
Q_OBJECT

public:
    T_TCP_Client_AviaMotionSystem(Helicopter_core* mi17);
    ~T_TCP_Client_AviaMotionSystem();
    void Start() {};
    void Stop() {};
    void SetInterval(int i) { assert((i > 20) && (i < 200)); TimerTCPSend->setInterval(i); };

protected:
    Helicopter_core* Mi17;
    void GetAviaMotionFeedbackDataPacket(QDataStream &_stream);
    void SendAviaMotionStateDataPacket();
    void ReadCommonTcpData();
    void SendCommonTcpData();

protected slots:
    void On_SendTcpData();
    void On_ReadTcpData();
    //void On_Error();
    //void On_Connected();
    //void On_Disconnected();
};

#endif // AVIAMOTION_TCP_CLIENT_H
