#include "Helicopter_server.h"
#include <QDebug>

Helicopter_Server::Helicopter_Server( Helicopter_core* Helicopter, QObject* parent ) : T_Server( parent )
{
    Q_ASSERT( Helicopter );
    if ( Helicopter->_ini->SimulatorType( ) == 19 )
    {
        ServerReady->simulatorType = SimulatorTypes::Helicopter;
    }
    if ( Helicopter->_ini->SimulatorType( ) == 20 )
    {
        ServerReady->simulatorType = SimulatorTypes::Helicopterpod;
    }
}

Helicopter_Server::~Helicopter_Server()
{
    qDebug() << __FUNCTION__;
}

void Helicopter_Server::Init()
{
    T_Server::Init();
    //ServerReady->simulatorType = SimulatorTypes::Helicopter;        // тип техники
    ServerReady->ServerReady = true;                           // Готов
    ServerReady->PacketReady = true;
}
