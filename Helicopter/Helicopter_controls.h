#pragma once
#include "Helicopter_core.h"
#include "Helicopter_FM.h"
#include "Helicopter_can.h"
#include "Common/Controls.h"
#include "Helicopter_joystick.h"
#include "Helicopter_onboard.h"
#include "Gen\Helicopter_SOD.h"
// таблица с максимальным количеством объектов из файла Logic_IN_OUT.ini

class Helicopter_controls : public T_Controls
{
	Q_OBJECT
public:
	Helicopter_controls( Helicopter_core* Helicopter );
	~Helicopter_controls( );
	void    Start( ); // при запуске упражнения
	void    Stop( ); // при остановке упражнения
	QSharedPointer<Helicopter_audio> GetAudio( ) { return OnBoard_->_audio; };
	T_UNITS& GetUnitsData( );
    ElementsStateTypes CANBusState = ElementsStateTypes::Good;
    QString CANBusStateMsg = "";
private:
    QTimer* LogTimer;
    bool saving = false;
    bool creating = true;
    bool closing = false;
    QFile *out;
    QTextStream *stream;

	Helicopter_core* Helicopter;
	Helicopter_Onboard* OnBoard_;
	void    ReadLogicINI( QString INIfilename );
	bool    TestVidSteps( );
	void    CanHeartBeat( );

	void    ComputeParameters( );
	void    ValidateParameters( );
	void    SubstitutionParameters( );
	void    PrintParametrs( );
	void    PrintTumblers( );
	void    CanExchange( );

private slots:
	void    slotValidateParameters( );
    void    slotLogsSave();
public slots:

};

