#ifndef Helicopter_TCP_CLIENT_INSTRUCTOR_H
#define Helicopter_TCP_CLIENT_INSTRUCTOR_H

//#include "Common/Instructor_TCP_client.h"
#include "Helicopter_core.h"
#include "Helicopter_server.h"
#include "Helicopter_imager.h"
#include "Helicopter_instructor.h"
#include "Common/SettingsFile.h"
#include "Common/TCP_client.h"

class Helicopter_TCP_Client_Instructor : public TCP_Client // 
{
Q_OBJECT

public:
    Helicopter_TCP_Client_Instructor(Helicopter_core* Helicopter);
    ~Helicopter_TCP_Client_Instructor();
    void Start();
    void Stop();
    bool _Active;

private:
    Helicopter_core* Helicopter;
    bool processedHeader = false;
    void SendAviaStateDataPacket();     // 100
    void SendComponentsStatePacket();   // 200
    void SendImagerReadyDataPacket();   // 103
    void SendServerReadyDataPacket( );  // 106
    void SendBallisticDataPacket( );
    void ReadCommonTcpData();
    void GetFailuresDataPacket(QDataStream& _stream); //12
    void GetScriptDataPacket(QDataStream& _stream);
    void GetExerciseParametersPacket(QDataStream& _stream);
    void GetAmmunitionDataPacket(QDataStream& _stream);
    void GetTargetsDataPacket(QDataStream& _stream);
    void GetObjectsDataPacket(QDataStream& _stream);
    void GetEnvironmentDataPacket(QDataStream& _stream);
    void GetStepCheckDataPacket(QDataStream& _stream);
    void GetShutterDataPacket(QDataStream& _stream);
    void GetProjectorDataPacket(QDataStream& _stream);
    void GetSoundCamDataPacket(QDataStream& _stream);
    void GetPlaySpeedDataPacket(QDataStream& _stream);
    void GetPlayPosDataPacket(QDataStream& _stream);
    void GetPlayModeDataPacket(QDataStream& _stream);
    void GetRunControlDataPacket(QDataStream& _stream);
    void GetStatisticsDataPacket(QDataStream& _stream);
    void GetControlDataPacket(QDataStream& _stream);
    void GetCommandsDataPacket(QDataStream& _stream);
    void GetMessagesDataPacket(QDataStream& _stream);


private slots:
    void On_SendImagerReady( );
    void On_SendTcpData();
    void On_ReadTcpData();
    void On_Error();
    void On_Connected();
    void On_Disconnected();
public slots:
    void SendArmStateDataPacket( );             // 105
    void On_StandPacket( QByteArray data );     // пакет 100 с местположением стенда для инструктора

signals:
    // сигналы о приходе соответствущих пакетов от инструктора, потихоньку переделываю на сигналбную структуру вместо опроса по таймеру, привет Головину
    void InstructorPacket( QByteArray data);
    void AmmoIn( );
    void RunRecords( QString run_uuid );
    void PlayMode( quint8 mode );
    void PlayPos( quint32 mode );
    void PlaySpeed( float mode );
    void Forward( );
    void Enviroment( );
    void Exercise( quint32 );
    void TestVid( );
};


#endif // Helicopter_TCP_CLIENT_INSTRUCTOR_H
