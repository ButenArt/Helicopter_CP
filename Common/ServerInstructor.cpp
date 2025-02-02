#include "Common/ServerInstructor.h"
#include "../Helicopter/Helicopter_instructor_tcp_client.h"

ServerInstructor::ServerInstructor( QObject* parent )
    : QTcpServer( parent )
{
	Helicopter = qobject_cast<Helicopter_core*>( parent );
}

bool ServerInstructor::ClientConnected( )
{
	return clientConnected;
}

void ServerInstructor::write( QByteArray data )
{
	//qDebug( ) << __FUNCTION__;
	//qDebug( ) << "Sending data:" << data;
	if ( clientConnected )
	{
		socket->write( data );
	}
}

void ServerInstructor::incomingConnection( qintptr socketDescriptor )
{
	socket = new QTcpSocket( this );
	if ( !socket->setSocketDescriptor( socketDescriptor ) ) {
		qDebug( ) << "error creating socket";
		delete socket;
		return;
	}
	clientConnected = true;
	qDebug( ) << "new connection from:" << socket->peerAddress( ).toString( );
	//connect( socket, &QTcpSocket::readyRead, [=]{
	//	QByteArray data = socket->readAll( );
	//	qDebug( ) << "received data:" << data;
	//	} );
    
    connect(socket, &QTcpSocket::readyRead, this, &ServerInstructor::ReadCommonTcpData);

	connect( socket, &QTcpSocket::disconnected, [=]{
		qDebug( ) << "disconnected from:" << socket->peerAddress( ).toString( );
		clientConnected = false;
		socket->deleteLater( );
		} );
}


// принимаем пакеты со стенда в сторону инструктора
// передаем на инструктор только 100 пакет с местоположением стенда, остальное выкидываем
void ServerInstructor::ReadCommonTcpData()
{
    if (socket->state() == QAbstractSocket::ConnectedState)
    {
        auto header_length = sizeof(Helicopter->instructor->PacketHeader);
        auto availabeBytes = socket->bytesAvailable();
        while (availabeBytes >= header_length)
        {
            QByteArray header_data;
            QByteArray packet_data;
            QByteArray packet;

            if (!processedHeader)
            {
                header_data = socket->read(header_length);

                QDataStream header_stream(header_data);
                header_stream.setByteOrder(QDataStream::LittleEndian);
                header_stream.setFloatingPointPrecision(QDataStream::SinglePrecision);

                header_stream >> Helicopter->instructor->PacketHeader.Mark;
                header_stream >> Helicopter->instructor->PacketHeader.PacketType;
                header_stream >> Helicopter->instructor->PacketHeader.DataSize;
                processedHeader = true;
                availabeBytes = socket->bytesAvailable();
            }

            if ((processedHeader) && (availabeBytes >= Helicopter->instructor->PacketHeader.DataSize))
            {
                packet_data = socket->read(Helicopter->instructor->PacketHeader.DataSize);
                QDataStream data_stream(packet_data);
                data_stream.setByteOrder(QDataStream::LittleEndian);
                data_stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
				switch (Helicopter->instructor->PacketHeader.PacketType)
				{
                // пакет с местоположением стенда
				case static_cast<quint32>(InstructorTCPPacketTypes::SR2IR_AviaStateDataPacket):
                    packet.append(header_data);
                    packet.append(packet_data);
                    emit StandPacket(packet);
                    break;
				default:
                    qDebug() << "Skiping message =" << Helicopter->instructor->PacketHeader.PacketType;
                    break;
                }
                processedHeader = false;
            }
            availabeBytes = socket->bytesAvailable();
        }
    }
}
