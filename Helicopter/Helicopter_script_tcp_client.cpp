#include "Helicopter_script_tcp_client.h"


Helicopter_TCP_Client_ScriptServer::Helicopter_TCP_Client_ScriptServer(Helicopter_core* Helicopter_)
{
    Helicopter = Helicopter_;

    TxInterval = Helicopter->_ini->LanTxInterval();
    hostaddr = Helicopter->_ini->IP_ScriptServer();
    tcp_port = Helicopter->_ini->PORT_ScriptServer();
    qDebug( ) << "Connected to ScriptServer " << hostaddr << ":" << tcp_port;

    ConnectToHost(hostaddr, tcp_port);

    TimerTCPSend->setInterval(TxInterval);
    connect(TimerTCPSend,SIGNAL(timeout()),this, SLOT(slotSendTcpData()));
    TimerTCPSend->start();

    connect(this,SIGNAL(signalConnected()),this,SLOT(slotConnected()),Qt::QueuedConnection);
    connect(this,SIGNAL(signalDisconnected()),this,SLOT(slotDisconnected()),Qt::QueuedConnection);
    connect(this,SIGNAL(signalError()),this,SLOT( slotError()),Qt::QueuedConnection);
    connect(TcpSocket, SIGNAL(readyRead()), this, SLOT(slotReadTcpData()));
}

Helicopter_TCP_Client_ScriptServer::~Helicopter_TCP_Client_ScriptServer()
{
    TimerTCPSend->stop();
}

void Helicopter_TCP_Client_ScriptServer::slotError()
{
    Helicopter->scriptserver->ScriptServerActive = ClientConnected();
}

void Helicopter_TCP_Client_ScriptServer::slotReadTcpData()
{
    ReadCommonTcpData();
}

void Helicopter_TCP_Client_ScriptServer::ReadCommonTcpData()
{
    bool processedHeader = false;
    if ( TcpSocket->state() == QAbstractSocket::ConnectedState )
    {
        auto header_length = sizeof( Helicopter->scriptserver->PacketHeader );
        auto availabeBytes = TcpSocket->bytesAvailable();
        while ( availabeBytes >= header_length )
        {
            if ( !processedHeader )
            {
                QByteArray header_data = TcpSocket->read( header_length );

                QDataStream header_stream( header_data );
                header_stream.setByteOrder( QDataStream::LittleEndian );
                header_stream.setFloatingPointPrecision( QDataStream::SinglePrecision );

                header_stream >> Helicopter->scriptserver->PacketHeader.Mark;
                header_stream >> Helicopter->scriptserver->PacketHeader.PacketType;
                header_stream >> Helicopter->scriptserver->PacketHeader.DataSize;
                processedHeader = true;
                availabeBytes = availabeBytes - header_length;
            }

            if ( (processedHeader) && (availabeBytes >= Helicopter->scriptserver->PacketHeader.DataSize) )
            {
                QByteArray packet_data = TcpSocket->read( Helicopter->scriptserver->PacketHeader.DataSize );
                QDataStream data_stream( packet_data );
                data_stream.setByteOrder( QDataStream::LittleEndian );
                data_stream.setFloatingPointPrecision( QDataStream::SinglePrecision );
                //switch ( Helicopter->scriptserver->PacketHeader.PacketType )
                //{
                //default:
                //    //_log->WriteLANLog( LogEvents::TCP_ScriptServer_Unknown_PacketType, QString::number( _ScriptServer->PacketHeader.PacketType ) );
                //    break;
                //}
            }
            availabeBytes = TcpSocket->bytesAvailable();
            processedHeader = false;
        }
    }
}

void Helicopter_TCP_Client_ScriptServer::slotSendTcpData()
{
    SendControlDataPacket();
}

void Helicopter_TCP_Client_ScriptServer::SendControlDataPacket()
{
    if ( !Helicopter->instructor->Control.PacketReady ) return;
    if ( TcpSocket->state() == QAbstractSocket::ConnectedState )
    {
        QByteArray data;
        QDataStream data_stream( &data, QIODevice::WriteOnly );
        data_stream.setByteOrder( QDataStream::LittleEndian );
        data_stream.setFloatingPointPrecision( QDataStream::SinglePrecision );
        data_stream << static_cast<QString>( Helicopter->instructor->Control.CommandText);        // текст команды
        QByteArray header;
        QDataStream header_stream( &header, QIODevice::WriteOnly );
        header_stream.setByteOrder( QDataStream::LittleEndian );
        header_stream << static_cast<quint8>('S');
        header_stream << static_cast<quint8>('t');
        header_stream << static_cast<quint8>('o');
        header_stream << static_cast<quint8>('C');
        header_stream << static_cast<quint32>(ScriptServerTCPPacketTypes::SR2ST_ControlDataPacket);
        header_stream << static_cast<quint32>(data.size());
        QByteArray packet;
        packet.append( header );
        packet.append( data );
        TcpSocket->write( packet );
        TcpSocket->flush( );
        Helicopter->instructor->Control.PacketReady = false;
    }
}
