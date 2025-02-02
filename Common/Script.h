#ifndef SCRIPT_H
#define SCRIPT_H

#include <QObject>
#include <Windows.h>

#pragma pack(push, 1)
typedef struct
{
    quint32 Mark;
    quint32 PacketType;
    quint32 DataSize;
} ScriptServerTCPHeader;
#pragma pack(pop)

class T_ScriptServer : public QObject
{
    Q_OBJECT

public:
    bool ScriptServerActive;
    //заголовок пакетов//
    ScriptServerTCPHeader PacketHeader;

    explicit T_ScriptServer(QObject *parent = 0);
    ~T_ScriptServer();
    virtual void Init();
};

#endif // SCRIPT_H
