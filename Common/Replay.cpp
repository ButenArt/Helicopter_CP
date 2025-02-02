#include "Replay.h"

TReplay::TReplay(TSettingsFile *ini, TLogging *log)
{
    _ini = ini;
    _log = log;
    //ReplayEnabled = _ini->ReplayEnabled();
    replayPacket.guid = "{00000000-0000-0000-0000-000000000000}";
    replayPacket.clientType = 0;
    replayPacket.packetType = 0;
    replayPacket.packetData = 0;
    replayPacketList.clear();
    insertCounter = 0;
    QString dbdir = QCoreApplication::applicationDirPath().append("/data/");
    QString dbpath = QCoreApplication::applicationDirPath().append("/data/Replay.db");
    db = new QSqlDatabase;
    *db = QSqlDatabase::addDatabase("QSQLITE");
    db->setHostName("localhost");
    db->setDatabaseName(dbpath);
    db->setPassword("");
    db->setUserName("");
    if(!QDir().exists(dbdir)) QDir().mkdir(dbdir);
    if(!QFile::exists(dbpath)) CreateReplayTable();
    if(!db->open())
    {
        _log->WriteLANLog(LogEvents::DB_Replay_Open_Error, db->lastError().text());
        dbOpen = false;
    }
    else
    {
        dbOpen = true;
        db->exec("PRAGMA synchronous = OFF");
        //db->exec("PRAGMA journal_mode = MEMORY");
        db->transaction();
    }
    connect(this,SIGNAL(signalWritePacketData()),this,SLOT(slotInsertPacketData()));
}

TReplay::~TReplay()
{
    db->commit();
    db->close();
    delete db;
    quit();
    wait();
}

void TReplay::WritePacketData(QString guid, ClientPacketTypes clientType, quint32 packetType, QByteArray packetData)
{
    if(!dbOpen) return;
    if(!ReplayEnabled) return;
    replayPacket.guid = guid;
    replayPacket.clientType = quint8(clientType);
    replayPacket.packetType = packetType;
    replayPacket.packetData = packetData;
    replayPacketList.append((TReplayPacket)replayPacket);
    emit signalWritePacketData();
}

int TReplay::GetReplayListCount()
{
    return replayPacketList.count();
}

void TReplay::CreateReplayTable()
{
    db->open();
    QSqlQuery query;
    query.clear();
    query.prepare("CREATE TABLE Replay (PacketGuid GUID NOT NULL,"
                  "ClientType NUMBER NOT NULL,PacketType NUMBER NOT NULL,"
                  "PacketDT TIMESTAMP NOT NULL,PacketData BLOB NOT NULL)");
    query.exec();
    //qDebug() << query.lastError().text();
    db->close();
}

void TReplay::ClearReplay()
{
    if(!dbOpen) return;
    QSqlQuery query;
    query.clear();
    query.prepare("DELETE FROM Replay;");
    query.exec();
    db->commit();
    db->transaction();
    //qDebug() << query.lastError().text();
}

void TReplay::slotInsertPacketData()
{
    while(!replayPacketList.isEmpty())
    {
        TReplayPacket tmpReplayPacket;
        tmpReplayPacket = replayPacketList.takeFirst();
        QSqlQuery query;
        query.clear();
        query.prepare("INSERT INTO Replay (PacketGuid, ClientType, PacketType, PacketDT, PacketData) VALUES (:guid, :client, :type, :dt, :packet);");
        query.bindValue(":guid", tmpReplayPacket.guid);
        query.bindValue(":client", tmpReplayPacket.clientType);
        query.bindValue(":type", tmpReplayPacket.packetType);
        query.bindValue(":dt", QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm:ss:zzz"));
        query.bindValue(":packet", tmpReplayPacket.packetData);
        query.exec();
        insertCounter++;
        if(insertCounter >= COMMIT_ROWS)
        {
            db->commit();
            insertCounter = 0;
            db->transaction();
        }
        //qDebug() << query.lastError().text();
    }
}

void TReplay::run()
{
    exec();
}
