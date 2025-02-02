#ifndef Helicopter_TARGETSMANAGER_H
#define Helicopter_TARGETSMANAGER_H

#include <QObject>
#include <QTimer>
#include <QDateTime>
#include "Helicopter_core.h"
#include "Helicopter_Imager.h"
#include "Helicopter_Instructor.h"
#include "Common/Logging.h"


class Helicopter_TargetsManager : public QObject
{
    Q_OBJECT
public:
    explicit  Helicopter_TargetsManager( Helicopter_core* Helicopter_, QObject *parent = 0);
    ~Helicopter_TargetsManager();
    bool commandMoveForward;           // команда "Вперед" визуализацией обработана
protected:
    Helicopter_core* Helicopter;
private:
    void TargetsManagerStart();
    void TargetsManagerStop();
    QDateTime startingDateTime;
    QTimer *UpdateTimer;
    QTimer *ManagerTimer;
    int secsFromStart;
private slots:
    void slotUpdateManager();
    void slotTargetsManager();
};


#endif // Helicopter_TARGETSMANAGER_H
