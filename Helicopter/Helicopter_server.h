#pragma once
#include "Common/server.h"
#include "Helicopter_core.h"

class Helicopter_Server : public T_Server
{
    Q_OBJECT
public:
    Helicopter_Server( Helicopter_core* Helicopter, QObject* parent = nullptr );
    ~Helicopter_Server();
    void Init();                       // инициализация параметров
};
