#ifndef Helicopter_OC_TCP_CLIENT_H
#define Helicopter_OC_TCP_CLIENT_H

#include "Helicopter_core.h"
#include "Common/TCP_client.h"


class Helicopter_OC_TCP_Client : public TCP_Client
{
Q_OBJECT

public:
    Helicopter_OC_TCP_Client(Helicopter_core* Helicopter);
    ~Helicopter_OC_TCP_Client();
    void Start();
    void Stop();
    bool _Active;
private:
    Helicopter_core* Helicopter;
    void ReadCommonTcpData();
    void SendOCDataPacket();

private slots:
    void On_Error();
    void On_Connected();
    void On_Disconnected();
    void slotReadTcpData();
    void slotSendTcpData();
};

#endif // Helicopter_OC_TCP_CLIENT_H
