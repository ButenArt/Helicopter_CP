#ifndef Helicopter_SCRIPT_H
#define Helicopter_SCRIPT_H

#include "Common/Script.h"

class Helicopter_ScriptServer : public T_ScriptServer
{
    Q_OBJECT
public:
    Helicopter_ScriptServer();
    ~Helicopter_ScriptServer();
    void Init();                       // инициализация параметров
};

#endif // Helicopter_SCRIPT_H
