#include "Helicopter_TargetsManager.h"
#include <QDebug>

Helicopter_TargetsManager::Helicopter_TargetsManager( Helicopter_core* Helicopter_, QObject *parent) : QObject(parent)
{
    Helicopter = Helicopter_;

    startingDateTime = QDateTime::currentDateTimeUtc();
    secsFromStart = 0;

    commandMoveForward = false;

    UpdateTimer = new QTimer(this); // по этому таймеру ТМ проверяет запущено ли упражнение для начала работы
    UpdateTimer->setInterval(500);  // 20
    connect(UpdateTimer,SIGNAL(timeout()),this, SLOT(slotUpdateManager()));
    UpdateTimer->start();

    ManagerTimer = new QTimer(this); // таймер обработки 
    ManagerTimer->setInterval(100);
    connect(ManagerTimer,SIGNAL(timeout()),this, SLOT(slotTargetsManager()));
    ManagerTimer->stop();
}

Helicopter_TargetsManager::~Helicopter_TargetsManager()
{
    delete ManagerTimer;
    delete UpdateTimer;
}

void Helicopter_TargetsManager::TargetsManagerStart()
{
    qDebug( ) << __FUNCTION__;
    ManagerTimer->stop();
    startingDateTime = QDateTime::currentDateTimeUtc();
    secsFromStart = 0;
    ManagerTimer->start();
}

void Helicopter_TargetsManager::TargetsManagerStop()
{
    //qDebug( ) << __FUNCTION__;
    ManagerTimer->stop();
    secsFromStart = 0;
}

void Helicopter_TargetsManager::slotUpdateManager()
{
    if( Helicopter->instructor->exerciseStarted )
	{
        if ( Helicopter->instructor->commandMoveForward )
        {
            TargetsManagerStart( );
            Helicopter->instructor->commandMoveForward = false;
        }
	}
    else 
        TargetsManagerStop();
}

void Helicopter_TargetsManager::slotTargetsManager()
{
    secsFromStart = startingDateTime.secsTo(QDateTime::currentDateTimeUtc());
    TargetsControlDataPacket TargetsControlData;
    TargetsControlData.PacketReady = true;
    TargetsControlData.targetControlList.clear();
    for ( auto& mapTargets : Helicopter->instructor->Targets.mapTargetsList )
    {
        TargetControl targetControl;

        for ( int j = 0; j < mapTargets.targetShowNumber; j++ )
        {
            if ( mapTargets.targetsShowList[j].startTime == secsFromStart )
            {
                targetControl.ObjectiveUID = mapTargets.ObjectiveUID;
                targetControl.TargetUID = mapTargets.TargetUID;
                targetControl.TargetState = true;
                TargetsControlData.targetControlList << targetControl;
            } else if ( mapTargets.onlyDay && ( mapTargets.targetsShowList[j].startTime + mapTargets.targetsShowList[j].dayTime == secsFromStart ) )
            {
                targetControl.ObjectiveUID = mapTargets.ObjectiveUID;
                targetControl.TargetUID = mapTargets.TargetUID;
                targetControl.TargetState = false;
                TargetsControlData.targetControlList << targetControl;
            } else if ( mapTargets.onlyNight && ( mapTargets.targetsShowList[j].startTime + mapTargets.targetsShowList[j].nightTime == secsFromStart ) )
            {
                targetControl.ObjectiveUID = mapTargets.ObjectiveUID;
                targetControl.TargetUID = mapTargets.TargetUID;
                targetControl.TargetState = false;
                TargetsControlData.targetControlList << targetControl;
            }
        }
    }

    TargetsControlData.mapTargetsCount = TargetsControlData.targetControlList.size( );
    if ( TargetsControlData.mapTargetsCount > 0 )
    {
        Helicopter->imager->TargetsControl = TargetsControlData;
        qDebug( ) << TargetsControlData;
    }
}
