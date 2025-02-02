#ifndef REPLAY_H
#define REPLAY_H

#include <QObject>
#include <QtSql>
#include "SettingsFile.h"
#include "Logging.h"

const WORD COMMIT_ROWS = 100;

typedef struct _ReplayPacket_STRUCT
{
    QString guid;
    quint8 clientType;
    quint32 packetType;
    QByteArray packetData;
} TReplayPacket, *PTReplayPacket;

class TReplay : public QThread
{
Q_OBJECT

public:
    TReplay(TSettingsFile *ini, TLogging *log);
    ~TReplay();
    void ClearReplay();
    int GetReplayListCount();
    void WritePacketData(QString guid, ClientPacketTypes clientType, quint32 packetType, QByteArray packetData);

private:
    QSqlDatabase *db;
    TSettingsFile *_ini;
    TLogging *_log;
    bool ReplayEnabled;
    bool dbOpen;
    quint8 insertCounter;
    TReplayPacket replayPacket;
    QList<TReplayPacket> replayPacketList;
    void CreateReplayTable();
protected:
    void run();

signals:
    void signalWritePacketData();

private slots:
    void slotInsertPacketData();

};

#endif // REPLAY_H
