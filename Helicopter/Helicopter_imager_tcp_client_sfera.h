#pragma once
#include "Helicopter_core.h"
#include "Helicopter_Model.h"
#include "Helicopter_server.h"
#include "Helicopter_imager.h"
//#include "Helicopter_instructor.h"
#include "Common/SettingsFile.h"
#include "Common/TCP_client.h"
#include <chrono>

using time_point = std::chrono::time_point<std::chrono::system_clock>;

class Helicopter_TCP_Client_Imager_Sfera : public TCP_Client  // сетевые пакеты стандартной визуалки убраны за ненадобностью
{
	Q_OBJECT

public:
	Helicopter_TCP_Client_Imager_Sfera( Helicopter_core* Helicopter, int index = 0);
	~Helicopter_TCP_Client_Imager_Sfera( );
	void ConnectPort( quint16 port ) { tcp_port = port; DisconnectFromHost( ); };
	quint16 GetPort( ) { return tcp_port; };
	void Start( );
	void Stop( );

private:
	Helicopter_core* Helicopter;
	time_point start;
	bool processedHeader = false;

	void SendHelicopterMovingPacket( );      // пакет №1 из protocols.xls struct HelicopterMovingPacket
	void SendHelicopterStartPacket( );       // пакет json из protocols.xls 
	void SendHelicopterStatPacket( );        // пакет json из protocols.xls 
	void SendHelicopterWeatherPacket( );     // пакет json из protocols.xls 

	void SendExerciseParametersPacket( );    //[ ID=1 # Server >> Visual ] Команда на запуск карты
	void SendObjectsDataPacket( );           //[ ID=2 # Server >> Visual ] Объекты на карте
	void SendEnvironmentDataPacket( );       //[ ID=3 # Server >> Visual ] Обстановка
	void SendTargetsDataPacket( );           //[ ID=5 # Server >> Visual ] Мишени на карте
	void SendCommandsDataPacket( );          //[ ID=13 # Server >> Visual] Вперед
	void SendMessagesDataPacket( );          //[ ID=14 # Server >> Visual] Текстовое сообщение
	void SendTargetsControlDataPacket( );    //[ ID=16 # Server >> Visual] Управление мишенями
	void SendEffectsDataPacket( );           //[ ID=101 # Server >> Visual] Состояние борта 
    void SendHelicopterMovingDataPacket();  //[ ID=102 # Server >> Visual] Координаты техники 

	void GetImagerState( QDataStream& stream );                   // [ ID = 4 # Visual >> Server] Состояние симулятора
	void GetImagerReadyDataPacket( QDataStream& stream );         // [ ID = 6 # Visual >> Server] Готовность визуалки
	void GetImagerElementsStateDataPacket( QDataStream& stream ); // [ ID = 15 # Visual >> Server] Данные о готовности компонентов
	void GetHeightDataPacket( QDataStream& stream );           // [ ID = 305 # Visual >> Server ] Состояние техники на карте - ВЫСОТОМЕР
	void GetBallisticDataPacket( QDataStream& stream );           // [ ID=1001 # Visual >> Server ] Событие изменения состояния мишени
	void GetTriggerDataPacket( QDataStream& stream );             // [ ID=1002 # Visual >> Server ] Событие прохождения препятствия (тригеры)
	void GetMissedEventPacked( QDataStream& stream );             // [ ID=1003 # Visual >> Server ] Координаты промаха
	void GetCargoEventPacked( QDataStream& stream );              // [ ID=1005 # Visual >> Server ] Работа с подвесом
	void GetAmmoEventPacked( QDataStream& stream );               // [ ID=1006 # Visual >> Server ] Остаток боеприпасов
	void ReadTcpDataKlim( );
	void ReadTcpDataAES( );

private slots:
	void On_SendTcpData( );
	void On_SendPeriodicTcpData( );
	void On_ReadTcpData( );
	void On_Error( );
	void On_Connected( );
	void On_Disconnected( );
	void SendFireDataPacket( );              //[ ID=103 # Server >> Visual] Стрельба
	void SendPKTFireDataPacket( bool fire ); //[ ID=103 # Server >> Visual] Стрельба из ПКТ
	void SendCargoControlDataPacket( );      //[ ID=19 # Server >> Visual] Работа с подвесом
public slots:
	void SendAmmoDataPacket( );              //[ ID=18 # Server >> Visual] Cостояние оружии
	void SendExitDataPacket( int cause );
signals:
	void imagerReady( );
	void AmmoOut( );
	void CargoHook( quint32 Load);
};

