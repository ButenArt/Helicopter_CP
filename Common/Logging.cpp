#include <QCoreApplication>
#include <QFileInfo>
#include <QDateTime>
#include "Logging.h"

TLogging::TLogging(QSharedPointer<TSettingsFile> ini)
{
    _ini = ini;
    _lan_logging = _ini->LanLoggingEnabled();
    _can_logging = _ini->CanLoggingEnabled();
    _can_data_logging = _ini->CanLoggingEnabled();
    _srv_logging = true;
    _ballistics_logging = true;
    lan_logfile = new QFile();
    lan_logfile->setFileName(QCoreApplication::applicationDirPath().append("/Lan.log"));
    if(_lan_logging) lan_logfile->open(QIODevice::Append);
    can_logfile = new QFile();
    can_logfile->setFileName(QCoreApplication::applicationDirPath().append("/Can.log"));
    if(_can_logging) can_logfile->open(QIODevice::Append);
    can_data_logfile = new QFile();
    can_data_logfile->setFileName(QCoreApplication::applicationDirPath().append("/CanData.log"));
    if(_can_data_logging) can_data_logfile->open(QIODevice::Append);
    srv_logfile = new QFile();
    srv_logfile->setFileName(QCoreApplication::applicationDirPath().append("/Server.log"));
    if(_srv_logging) srv_logfile->open(QIODevice::Append);
    ballistics_logfile = new QFile();
    ballistics_logfile->setFileName(QCoreApplication::applicationDirPath().append("/Ballistics.log"));
    if(_ballistics_logging) ballistics_logfile->open(QIODevice::Append);
}

TLogging::~TLogging()
{
    if(lan_logfile->isOpen()) lan_logfile->close();
    delete lan_logfile;
    if(can_logfile->isOpen()) can_logfile->close();
    delete can_logfile;
    if(can_data_logfile->isOpen()) can_data_logfile->close();
    delete can_data_logfile;
    if(srv_logfile->isOpen()) srv_logfile->close();
    delete srv_logfile;
    if(ballistics_logfile->isOpen()) ballistics_logfile->close();
    delete ballistics_logfile;
}

void TLogging::WriteLog(LogEvents event, QString externalstr)
{
    if(!_srv_logging) return;
    QString logstr = "";
    switch(event)
    {
        case LogEvents::Application_start:
          logstr = "\r\nServer start [" + QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss") + "]";
        break;
        case LogEvents::Application_close:
          logstr = "\r\nServer close [" + QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss") + "]\r\n";
        break;
        default:
        break;
    }
    if(srv_logfile->isOpen())
    {
        srv_logfile->write(logstr.toUtf8());
        srv_logfile->flush();
    }
}

void TLogging::WriteBallisticsLog(QString externalstr)
{
    if(!_ballistics_logging) return;
    QString logstr = "\r\n[" + QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss") + "] " + externalstr;

    if(ballistics_logfile->isOpen())
    {
        ballistics_logfile->write(logstr.toUtf8());
        ballistics_logfile->flush();
    }
}

void TLogging::WriteLANLog(LogEvents event, QString externalstr)
{
    if(!_lan_logging) return;
    QString logstr = "";
    switch(event)
    {
        case LogEvents::TCP_Imager_connected:
          logstr = "\r\n[" + QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss") + "] Imager TCP Socket received the connected() signal";
        break;
        case LogEvents::TCP_Imager_disconnected:
          logstr = "\r\n[" + QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss") + "] Imager TCP Socket received the disconnected() signal";
        break;
        case LogEvents::TCP_Imager_Error:
          logstr = "\r\n[" + QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss") + "] Imager TCP Socket ERROR: " + externalstr;
        break;
        case LogEvents::TCP_Imager_Unknown_PacketType:
          logstr = "\r\n[" + QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss") + "] Imager TCP Unknown PacketType: " + externalstr;
        break;
        case LogEvents::TCP_Instructor_connected:
          logstr = "\r\n[" + QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss") + "] Instructor TCP Socket received the connected() signal";
        break;
        case LogEvents::TCP_Instructor_disconnected:
          logstr = "\r\n[" + QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss") + "] Instructor TCP Socket received the disconnected() signal";
        break;
        case LogEvents::TCP_Instructor_Error:
          logstr = "\r\n[" + QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss") + "] Instructor TCP Socket ERROR: " + externalstr;
        break;
        case LogEvents::TCP_DriverMotionSystem_connected:
          logstr = "\r\n[" + QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss") + "] Driver Motion System TCP Socket received the connected() signal";
        break;
        case LogEvents::TCP_DriverMotionSystem_disconnected:
          logstr = "\r\n[" + QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss") + "] Driver Motion System TCP Socket received the disconnected() signal";
        break;
        case LogEvents::TCP_DriverMotionSystem_Error:
          logstr = "\r\n[" + QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss") + "] Driver Motion System TCP Socket ERROR: " + externalstr;
        break;
        case LogEvents::TCP_GunnerMotionSystem_connected:
          logstr = "\r\n[" + QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss") + "] Gunner Motion System TCP Socket received the connected() signal";
        break;
        case LogEvents::TCP_GunnerMotionSystem_disconnected:
          logstr = "\r\n[" + QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss") + "] Gunner Motion System TCP Socket received the disconnected() signal";
        break;
        case LogEvents::TCP_GunnerMotionSystem_Error:
          logstr = "\r\n[" + QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss") + "] Gunner Motion System TCP Socket ERROR: " + externalstr;
        break;
        case LogEvents::TCP_ScriptServer_connected:
          logstr = "\r\n[" + QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss") + "] Script Server TCP Socket received the connected() signal";
        break;
        case LogEvents::TCP_ScriptServer_disconnected:
          logstr = "\r\n[" + QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss") + "] Script Server TCP Socket received the disconnected() signal";
        break;
        case LogEvents::TCP_ScriptServer_Error:
          logstr = "\r\n[" + QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss") + "] Script Server TCP Socket ERROR: " + externalstr;
        break;
        case LogEvents::TCP_ControlsImitator_connected:
          logstr = "\r\n[" + QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss") + "] Controls Imitator TCP Socket received the connected() signal";
        break;
        case LogEvents::TCP_ControlsImitator_disconnected:
          logstr = "\r\n[" + QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss") + "] Controls Imitator TCP Socket received the disconnected() signal";
        break;
        case LogEvents::TCP_ControlsImitator_Error:
          logstr = "\r\n[" + QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss") + "] Controls Imitator TCP Socket ERROR: " + externalstr;
        break;
        case LogEvents::TCP_FCC_connected:
          logstr = "\r\n[" + QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss") + "] Fire Control Computer TCP Socket received the connected() signal";
        break;
        case LogEvents::TCP_FCC_disconnected:
          logstr = "\r\n[" + QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss") + "] Fire Control Computer TCP Socket received the disconnected() signal";
        break;
        case LogEvents::TCP_FCC_Error:
          logstr = "\r\n[" + QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss") + "] Fire Control Computer TCP Socket ERROR: " + externalstr;
        break;
        case LogEvents::DB_Replay_Open_Error:
          logstr = "\r\n[" + QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss") + "] Replay Database Open ERROR: " + externalstr;
        break;
        default:
        break;
    }
    if(lan_logfile->isOpen())
    {
        lan_logfile->write(logstr.toUtf8());
        lan_logfile->flush();
    }
}

void TLogging::WriteMOXALog(int error, QString externalstr)
{
    if(!_can_logging) return;
    QString logstr = "";
    QString errstr = "";
    switch(error)
    {
        case 0:
            errstr = "INVALID_HANDLE_VALUE";
        break;
        case -1:
            errstr = "E_ACCESS_DEVICE_FAILED";
        break;
        case -2:
            errstr = "E_TX_FAILED";
        break;
        case -3:
            errstr = "E_RX_FAILED";
        break;
        case -4:
            errstr = "E_TX_TIMEOUT";
        break;
        case -5:
            errstr = "E_RX_TIMEOUT";
        break;
        case -6:
            errstr = "E_INVALID_ACC_ID_CODE_VALUE";
        break;
        case -7:
            errstr = "E_INVALID_ACC_ID_MASK_VALUE";
        break;
        case -8:
            errstr = "E_INVALID_SIZE_VALUE";
        break;
        case -9:
            errstr = "E_INVALID_OPERATION";
        break;
        case -10:
            errstr = "E_WIN32_FAILED";
        break;
        default:
            errstr = "UNKNOWN_ERROR";
        break;
    }

    logstr = "\r\n[" + QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss") + "] ERROR: " + externalstr + " (" + QString::number(error) + ")" + errstr;
    if(can_logfile->isOpen())
    {
        can_logfile->write(logstr.toUtf8());
        can_logfile->flush();
    }
}

void TLogging::WriteCANLog(int event, QString externalstr)
{
    if(!_can_data_logging) return;
    QString logstr = "";
    QString eventstr = "";
    switch(event)
    {
        case 1:
            eventstr = "CNIO_SUCCESS";
        break;
        case 0:
            eventstr = "INVALID_HANDLE_VALUE";
        break;
        case -1:
            eventstr = "E_ACCESS_DEVICE_FAILED";
        break;
        case -2:
            eventstr = "E_TX_FAILED";
        break;
        case -3:
            eventstr = "E_RX_FAILED";
        break;
        case -4:
            eventstr = "E_TX_TIMEOUT";
        break;
        case -5:
            eventstr = "E_RX_TIMEOUT";
        break;
        case -6:
            eventstr = "E_INVALID_ACC_ID_CODE_VALUE";
        break;
        case -7:
            eventstr = "E_INVALID_ACC_ID_MASK_VALUE";
        break;
        case -8:
            eventstr = "E_INVALID_SIZE_VALUE";
        break;
        case -9:
            eventstr = "E_INVALID_OPERATION";
        break;
        case -10:
            eventstr = "E_WIN32_FAILED";
        break;
        default:
            eventstr = "UNKNOWN_EVENT";
        break;
    }

    logstr = "\r\n[" + QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss.zzz") + "] EVENT: " + externalstr + " (" + QString::number(event) + ")" + eventstr;
    if(can_data_logfile->isOpen())
    {
        can_data_logfile->write(logstr.toUtf8());
        can_data_logfile->flush();
    }
}

