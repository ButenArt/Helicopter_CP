#ifndef Helicopter_PROJECTORS
#define Helicopter_PROJECTORS

#include <QTcpSocket>
#include <QAbstractSocket>
#include <QDebug>
#include <QByteArray>
#include <QObject>
#include <QCryptographicHash>
#include <QTimer>
#include "Common/SettingsFile.h"
//#include <QVector>
#include <vector>

class Projector : public QObject
{
    Q_OBJECT

public:
    Projector( QString ip_, QString password_ = "", int port_ = 4352, QObject* parent = nullptr ) 
        : ip(ip_)
        , password( password_)
        , port( port_)
        , QObject( parent )
    {
        id = ++idGen;

        socket = new QTcpSocket(this);
        socket->setSocketOption(QAbstractSocket::KeepAliveOption, 1);

        QObject::connect(socket, SIGNAL(readyRead()), SLOT(readSocketData()), Qt::UniqueConnection);
    }

    ~Projector() { delete socket; }

    void command(QByteArray msg)
    {
        connectToHost();
        pendingCommand = msg;
    }

    void powerOn() { command("%1POWR 1\r"); }
    void powerOff() { command("%1POWR 0\r"); }
    void AVmuteOn() { command("%1AVMT 31\r"); }
    void AVmuteOff() { command("%1AVMT 30\r"); }
    void getPowerStatus() { command("%1POWR ?\r"); }
    void getAVmuteStatus() { command("%1POWR ?\r"); }

    bool power_on = false;

private:
    static int idGen;
    QString ip;
    QString password;
    int port = 4352; // всегда для PJLINK
    unsigned int id;
    QTcpSocket* socket;
    QByteArray pendingCommand;

    void connectToHost() 
    { 
        if (!(QAbstractSocket::ConnectedState == socket->state()))
            socket->connectToHost(ip, port, QIODevice::ReadWrite); 
    }

    void response(QByteArray msg)
    {
        if (msg.contains("PJLINK 1"))
        {
            // Remove PJLINK 1 - 9 bits
            auto random_number = msg.remove(0, 9);
            random_number.chop(1);
            random_number.append(password.toUtf8());
            auto hashedMessage = md5hash(random_number);
            hashedMessage.append(pendingCommand);
            if (socket->isWritable())
                socket->write(hashedMessage);
            pendingCommand.clear();
        }
        else if (msg.contains("PJLINK 0"))
        {
            if (socket->isWritable())
                socket->write(pendingCommand);
            pendingCommand.clear();
        }
        else if (msg.contains("POWR") || msg.contains("AVMT"))
        {
            if (msg.contains("POWR=1"))
                power_on = true;
            else
                power_on = false;
            socket->close();    // закрываем соединение когда получим ответ на запрос.
        }
    }

    QByteArray md5hash(QByteArray message)
    {
        return QCryptographicHash::hash(message, QCryptographicHash::Md5).toHex();
    }

private slots:

    void readSocketData()
    {
        QByteArray receivedData;
        while (socket->bytesAvailable())
        {
            receivedData += socket->readAll();
        }
        response( receivedData );
    }
};

class Helicopter_Projectors : public QObject
{
    Q_OBJECT

public:

    using Iterator = Projector;
    using ConstIterator = const Projector*;

    explicit Helicopter_Projectors(QSharedPointer<TSettingsFile> ini);
    ~Helicopter_Projectors();

    void powerOn();
    void powerOff();
    void AVmuteOn();
    void AVmuteOff();
    void getPowerStatus();
    void getAVmuteStatus();
    
    auto begin( )
    {
        return projectors.begin();
    };

    auto end( )
    {
        return projectors.end( );
    };
    auto cbegin( ) const
    {
        return projectors.cbegin( );
    };
    auto cend( ) const
    {
        return projectors.cend( );
    };
private:
    std::vector<Projector*> projectors;

private slots:
    void getPowerStatusSlot();

};

#endif // Helicopter_PROJECTORS
