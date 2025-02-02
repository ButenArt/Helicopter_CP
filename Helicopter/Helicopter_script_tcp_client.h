#ifndef Helicopter_SCRIPT_TCP_CLIENT_H
#define Helicopter_SCRIPT_TCP_CLIENT_H

#include "Helicopter_core.h"
#include "Helicopter_instructor.h"
#include "Helicopter_script.h"
#include "Common/SettingsFile.h"
#include "Common/TCP_client.h"
#include "Common/Replay.h"

class Helicopter_TCP_Client_ScriptServer : public TCP_Client
{
Q_OBJECT

public:
    Helicopter_TCP_Client_ScriptServer(Helicopter_core *Helicopter);
    ~Helicopter_TCP_Client_ScriptServer();
private:
    Helicopter_core* Helicopter;
    void SendControlDataPacket();
    void ReadCommonTcpData();
private slots:
    void slotError();
    void slotReadTcpData();
    void slotSendTcpData();
};

#endif // Helicopter_SCRIPT_TCP_CLIENT_H
