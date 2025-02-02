#ifndef FCC_H
#define FCC_H

#include <QObject>
#include <QQueue>

#pragma pack(push, 1)
typedef struct
{
    quint32 Mark;
    quint32 PacketType;
    quint32 DataSize;
} FCCTCPHeader;
#pragma pack(pop)

class T_FCC : public QObject
{
    Q_OBJECT
public:
    bool FCC_Active;
    //макимальная длина очереди//
    const quint8 QueueMaxLength = 5;
    //заголовок пакетов//
    FCCTCPHeader PacketHeader;
    explicit T_FCC(QObject *parent = 0);
    ~T_FCC();
    virtual void Init();
    virtual int GetQueueBuffer();
    virtual void ClearQueueBuffer();
};

#endif // FCC_H
