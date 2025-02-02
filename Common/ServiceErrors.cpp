#include "ServiceErrors.h"

T_ServiceErrors::T_ServiceErrors()
{
    ErrorsTimer = new QTimer(this);
    ErrorsTimer->setInterval(100);
    ErrorsTimer->stop();

    for( auto& Error : Errors)
    {
        Error.Name = "UNDEFINED";
        Error.Time = QTime::currentTime();
        Error.Active = false;
        Error.Send = false;
    }

    for ( auto& FI : FaultInjection )
    {
        FI.Name = "UNDEFINED";
        FI.Active = false;
        FI.Time = QTime::currentTime();
        FI.DeltaT = 0;
    }
}

T_ServiceErrors::~T_ServiceErrors()
{
    delete ErrorsTimer;
}

void T_ServiceErrors::Init()
{
    for ( auto& Error : Errors )
    {
       Error.Time = QTime::currentTime();
       Error.Active = false;
       Error.Send = false;
    }

    for ( auto& FI : FaultInjection )
    {
        FI.Active = false;
        FI.Time = QTime::currentTime();
        FI.DeltaT = 0;
    }
}
