 #include "Helicopter_Projectors.h"

Helicopter_Projectors::Helicopter_Projectors(QSharedPointer<TSettingsFile> ini)
{
    QStringList IPList;

    IPList << (ini->IP_PROJECTORS()).split(" ");

    for ( auto& ip : IPList )
    {
        projectors.push_back( new Projector ( ip, ini->PASS_PROJECTOR( ), ini->PORT_PROJECTORS( ) ) );
    }
    getPowerStatus( );
}

Helicopter_Projectors::~Helicopter_Projectors()
{
    projectors.clear();
}

void Helicopter_Projectors::powerOn()
{
    for (auto& projector : projectors )
        projector->powerOn();
}

void Helicopter_Projectors::powerOff()
{
    for ( auto& projector : projectors )
        projector->powerOff( );
}

void Helicopter_Projectors::AVmuteOn()
{
    for ( auto& projector : projectors )
        projector->AVmuteOn();
}

void Helicopter_Projectors::AVmuteOff()
{
    for ( auto& projector : projectors )
        projector->AVmuteOff();
}

void Helicopter_Projectors::getPowerStatus()
{
    for ( auto& projector : projectors )
        projector->getPowerStatus();
}

void Helicopter_Projectors::getAVmuteStatus()
{
    for ( auto& projector : projectors )
        projector->getAVmuteStatus();
}

void Helicopter_Projectors::getPowerStatusSlot()
{
    for ( auto& projector : projectors )
        projector->getPowerStatus();
}

int Projector::idGen = -1;
