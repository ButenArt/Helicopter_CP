#pragma once
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include "../Helicopter/Helicopter_core.h"

class ServerInstructor : public QTcpServer
{
    Q_OBJECT
public:
    explicit ServerInstructor( QObject* parent = 0 );
    bool ClientConnected( );
protected:
    Helicopter_core* Helicopter;
    bool processedHeader = false;
    QTcpSocket* socket = nullptr;
    bool clientConnected = false;

    void incomingConnection( qintptr socketDescriptor ) Q_DECL_OVERRIDE;
public slots:
    void ReadCommonTcpData();
    void write( QByteArray data );
signals:
    void StandPacket(QByteArray data);
};

