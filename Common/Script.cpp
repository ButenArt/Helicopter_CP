#include "Script.h"

T_ScriptServer::T_ScriptServer(QObject *parent) : QObject(parent)
{
    ScriptServerActive = false;

    PacketHeader.Mark = 0;
    PacketHeader.PacketType = 0;
    PacketHeader.DataSize = 0;

}

T_ScriptServer::~T_ScriptServer()
{

}

void T_ScriptServer::Init()
{
    ScriptServerActive = false;

    PacketHeader.Mark = 0;
    PacketHeader.PacketType = 0;
    PacketHeader.DataSize = 0;

}
