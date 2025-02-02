#ifndef Helicopter_IMITATOR_TCP_CLIENT_H
#define Helicopter_IMITATOR_TCP_CLIENT_H

#include "Helicopter_core.h"
//#include "Helicopter_instructor.h"
#include "Common/TCP_client.h"
#include "Common/SettingsFile.h"

class Helicopter_OU_TCP_Client : public TCP_Client
{
Q_OBJECT

public:
    Helicopter_OU_TCP_Client(Helicopter_core* Helicopter,int index=0);
    ~Helicopter_OU_TCP_Client();
    void Start();
    void Stop();
protected:
    Helicopter_core* Helicopter = nullptr;
    void ReadCommonTcpData();
    void SendControlsDataPacket();

private slots:
    void On_Error();
    void On_Connected();
    void On_Disconnected();
    void slotReadTcpData();
    void slotSendTcpData();
};

#endif // Helicopter_IMITATOR_TCP_CLIENT_H
