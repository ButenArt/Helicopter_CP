#include "FCC.h"

T_FCC::T_FCC(QObject *parent) : QObject(parent)
{
    FCC_Active = false;

    PacketHeader.Mark = 0;
    PacketHeader.PacketType = 0;
    PacketHeader.DataSize = 0;
}

T_FCC::~T_FCC()
{

}

void T_FCC::Init()
{
    FCC_Active = false;

    PacketHeader.Mark = 0;
    PacketHeader.PacketType = 0;
    PacketHeader.DataSize = 0;
}

int T_FCC::GetQueueBuffer()
{
    int buffer = 0;
    return buffer;
}

void T_FCC::ClearQueueBuffer()
{

}
