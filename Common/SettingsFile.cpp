#include "SettingsFile.h"
#include <QDebug>

TSettingsFile::TSettingsFile()
{
    _WindowMinimize = false;
    _CanLoggingEnabled = false;
    _LanLoggingEnabled = false;
    _SoundsEnabled = false;
    _ManualMode = false;
    _Board = "MOXA";
    //_ReplayEnabled = false;

    _CanTxInterval = 20;
    _CanRxInterval = 10;

	for ( int i = 0; i < N_UNITS; ++i )
	{
		_MAB[i].Addr = 0x300 + i * 0x10;   // 300 310 320 330 ...
		_MAB[i].Port = 0;
		_MAB[i].Enable = false;
	}

	// VID
	for ( int i = 0; i < N_UNITS; ++i )
	{
		_VID[i].Addr = 0x200 + i * 0x10;   // 
		_VID[i].Port = 0;
		_VID[i].Enable = false;
	}
    for ( int i = 0; i < 10; ++i )
    {
        _VID[i + 10].Addr = 0x900 + i * 0x10;   // 
        _VID[i + 10].Port = 0;
        _VID[i + 10].Enable = false;
    }
    for ( int i = 0; i < 10; ++i )
    {
        _VID[i + 10*2].Addr = 0x1000 + i * 0x10;   // 
        _VID[i + 10*2].Port = 0;
        _VID[i + 10*2].Enable = false;
    }
    for ( int i = 0; i < 10; ++i )
    {
        _VID[i + 10*3].Addr = 0x1100 + i * 0x10;   // 
        _VID[i + 10*3].Port = 0;
        _VID[i + 10*3].Enable = false;
    }
    for ( int i = 0; i < 10; ++i )
    {
        _VID[i + 10*4].Addr = 0x1200 + i * 0x10;   // 
        _VID[i + 10*4].Port = 0;
        _VID[i + 10*4].Enable = false;
    }

	// GPIN
	for ( int i = 0; i < N_UNITS; ++i )
	{
		_GPIOIN[i].Addr = 0x500 + i * 0x10;   // 
		_GPIOIN[i].Port = 0;
		_GPIOIN[i].Enable = false;
	}

	// GPOUT
	for ( int i = 0; i < N_UNITS; ++i )
	{
		_GPIOOUT[i].Addr = 0x700 + i * 0x10;   // 
		_GPIOOUT[i].Port = 0;
		_GPIOOUT[i].Enable = false;
	}

    // ENCODER
	for ( int i = 0; i < N_UNITS; ++i )
	{
		_ENC[i].Addr = 0x600 + i * 0x10;   // 
		_ENC[i].Port = 0;
		_ENC[i].Enable = false;
	}

	// POTENTIOMETER
	for ( int i = 0; i < N_UNITS; ++i )
	{
		_POT[i].Addr = 0x700 + i * 0x10;   // 
		_POT[i].Port = 0;
		_POT[i].Enable = false;
	}

    _LanTxInterval = 20;
    _LanRxInterval = 20;
      _IP_Imager[0] = "127.0.0.1";
    _PORT_Imager[0] = 10000;
      _IP_Imager[1] = "127.0.0.2";
    _PORT_Imager[1] = 10000;
      _IP_Imager[2] = "127.0.0.3";
    _PORT_Imager[2] = 10000;
      _IP_Imager[3] = "127.0.0.4";
    _PORT_Imager[3]  = 10000;
      _IP_Imager[4] = "127.0.0.5";
    _PORT_Imager[4]  = 10000;
      _IP_Imager[5] = "127.0.0.6";
    _PORT_Imager[5]  = 10000;
      _IP_Imager[6] = "127.0.0.7";
    _PORT_Imager[6]  = 10000;
      _IP_Imager[7] = "127.0.0.8";
    _PORT_Imager[7]  = 10000;

    _IP_Instructor = "127.0.0.1";
    _PORT_Instructor = 40000;
    _IP_ScriptServer = "127.0.0.1";
    _PORT_ScriptServer = 31414;
    _IP_OU[0] = "127.0.0.1";
    _PORT_OU[0] = 20000;
    _IP_OU[1] = "127.0.0.1";
    _PORT_OU[1] = 20001;
    _IP_OU[2] = "127.0.0.1";
    _PORT_OU[2] = 20002;
    _IP_CARCOMP = "127.0.0.1";
    _PORT_CARCOMP = 10010;
    _IP_ScreenItems = "127.0.0.1";
    _PORT_ScreenItems = 20001;
    _IP_AviaMotionSystem = "127.0.0.1";
    _PORT_AviaMotionSystem = 30303;
    ReadConfigINI(QCoreApplication::applicationDirPath().append("/config.ini"));
}

TSettingsFile::~TSettingsFile()
{

}

qint64 TSettingsFile::ReadVerificationRegistryKey(QString key, bool rewrite)
{
    qint64 timestamp = QDateTime::currentDateTimeUtc().toSecsSinceEpoch();
    QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\VisualStudio", QSettings::NativeFormat);
    if(settings.childKeys().contains(key, Qt::CaseInsensitive) && !rewrite)
    {
        timestamp = settings.value(key).toInt();
    }
    else
    {
        settings.setValue(key, QDateTime::currentDateTimeUtc().toSecsSinceEpoch());
    }
    return timestamp;
}

void TSettingsFile::ReadConfigINI(QString INIfilename)
{
    QSettings *ini = new QSettings(INIfilename, QSettings::IniFormat);

    if(QFileInfo(INIfilename).exists())
    {
        // MAIN //
        ini->beginGroup("MAIN");
        {
            _WindowMinimize = ini->value("START_MINIMIZED", 0).toBool();
            _SoundsEnabled  = ini->value("SOUNDS", 0).toBool();
            _ManualMode     = ini->value("MANUAL_MODE", 0).toBool();
            _Board = ini->value( "BOARD", "MOXA" ).toString( );
            _SimulatorType     = ini->value("SIMULATOR_TYPE", 19).toInt();
        }
        ini->endGroup();

        // CAN_CONFIG //
        ini->beginGroup("CAN_CONFIG");
        {
            _CanLoggingEnabled  = ini->value("LOGGING", 0).toBool();
            _CanTxInterval  = ini->value("TXInterval", 20).toInt();
            _CanRxInterval  = ini->value("RXInterval", 10).toInt();

            QString sADDR;
			QString sUNIT;
			int iADDR;
			bool ok;

            // MAB
            for ( int i = 0; i < N_UNITS; ++i )
            {
                sUNIT = "MAB" + QString::number( i ) + "UNIT_";
                sADDR = ini->value( sUNIT + "ADDR" ).toString( );
                iADDR = sADDR.toInt( &ok, 16 );

                _MAB[i].Addr = ok ? iADDR : 0x300 + i * 0x10;   // 300 310 320 330 ...
                _MAB[i].Port = ini->value( sUNIT + "PORT", 0 ).toInt( );
                _MAB[i].Enable = ini->value( sUNIT + "ENABLE", 0 ).toBool( );
                qDebug( ) << sUNIT << hex << iADDR << _MAB[i].Addr << _MAB[i].Port << _MAB[i].Enable;
            }

            // VID
            for ( int i = 0; i < N_V_UNITS; ++i )
            {
                sUNIT = "VID" + QString::number( i ) + "UNIT_";
                sADDR = ini->value( sUNIT + "ADDR" ).toString( );
                iADDR = sADDR.toInt( &ok, 16 );

                _VID[i].Addr = ok ? iADDR : _VID[i].Addr;   // инициализированно по умолчанию в конструкторе
                _VID[i].Port = ini->value( sUNIT + "PORT", 0 ).toInt( );
                _VID[i].Enable = ini->value( sUNIT + "ENABLE", 0 ).toBool( );
                qDebug( ) << sUNIT << hex << iADDR << _VID[i].Addr << _VID[i].Port << _VID[i].Enable;
            }


            // GPOIN
            for ( int i = 0; i < N_UNITS; ++i )
            {
                sUNIT = "GPIOIN" + QString::number( i ) + "UNIT_";
                sADDR = ini->value( sUNIT + "ADDR" ).toString( );
                iADDR = sADDR.toInt( &ok, 16 );

                _GPIOIN[i].Addr = ok ? iADDR : 0x500 + i * 0x10;   // 
                _GPIOIN[i].Port = ini->value( sUNIT + "PORT", 0 ).toInt( );
                _GPIOIN[i].Enable = ini->value( sUNIT + "ENABLE", 0 ).toBool( );
                qDebug( ) << sUNIT << hex << iADDR << _GPIOIN[i].Addr << _GPIOIN[i].Port << _GPIOIN[i].Enable;
            }

            // GPOUT
            for ( int i = 0; i < N_UNITS; ++i )
            {
                sUNIT = "GPIOOUT" + QString::number( i ) + "UNIT_";
                sADDR = ini->value( sUNIT + "ADDR" ).toString( );
                iADDR = sADDR.toInt( &ok, 16 );

                _GPIOOUT[i].Addr = ok ? iADDR : 0x700 + i * 0x10;   // 
                _GPIOOUT[i].Port = ini->value( sUNIT + "PORT", 0 ).toInt( );
                _GPIOOUT[i].Enable = ini->value( sUNIT + "ENABLE", 0 ).toBool( );
                qDebug( ) << sUNIT << hex << iADDR << _GPIOOUT[i].Addr << _GPIOOUT[i].Port << _GPIOOUT[i].Enable;
            }

            // POTENTIOMETER
            for ( int i = 0; i < N_UNITS; ++i )
            {
                sUNIT = "POT" + QString::number( i ) + "UNIT_";
                sADDR = ini->value( sUNIT + "ADDR" ).toString( );
                iADDR = sADDR.toInt( &ok, 16 );

                _POT[i].Addr = ok ? iADDR : 0x700 + i * 0x10;   // 
                _POT[i].Port = ini->value( sUNIT + "PORT", 0 ).toInt( );
                _POT[i].Enable = ini->value( sUNIT + "ENABLE", 0 ).toBool( );
                qDebug( ) << sUNIT << hex << iADDR << _POT[i].Addr << _POT[i].Port << _POT[i].Enable;
            }

            // ENCODER
            for ( int i = 0; i < 16; ++i )
            {
                sUNIT = "ENC" + QString::number( i ) + "UNIT_";
                sADDR = ini->value( sUNIT + "ADDR" ).toString( );
                iADDR = sADDR.toInt( &ok, 16 );

                _ENC[i].Addr = ok ? iADDR : 0x600 + i * 0x10;   // 
                _ENC[i].Port = ini->value( sUNIT + "PORT", 0 ).toInt( );
                _ENC[i].Enable = ini->value( sUNIT + "ENABLE", 0 ).toBool( );
                qDebug( ) << sUNIT << hex << iADDR << _ENC[i].Addr << _ENC[i].Port << _ENC[i].Enable;
            }
        }
        ini->endGroup();

        // LAN_CONFIG //
        ini->beginGroup("LAN_CONFIG");
        {
            _LanLoggingEnabled = ini->value("LOGGING", 0).toBool();
            //_ReplayEnabled = ini->value("REPLAY", 0).toBool();
            _LanTxInterval = ini->value("TXInterval", 20).toInt();
            _LanRxInterval = ini->value("RXInterval", 20).toInt();
            
            for (int i = 0; i < 8; ++i) {   // 0-инструктор, 1-6-сфера, 7-стенд
                _IP_Imager[i] = ini->value("IP_IMAGER_" + QString::number(i), "127.0.0." + QString::number(1+i)).toString();
                _PORT_Imager[i] = ini->value("PORT_IMAGER_", 10000).toInt();
            }
            
            _IP_Instructor = ini->value("IP_INSTRUCTOR", "127.0.0.1").toString();
            _PORT_Instructor = ini->value("PORT_INSTRUCTOR", 40000).toInt();
            
            _IP_ObjectivControl = ini->value("IP_OBJECTIVCONTROL", "127.0.0.1").toString();
            _PORT_ObjectivControl = ini->value("PORT_OBJECTIVCONTROL", 40001).toInt();

            _IP_ScriptServer = ini->value("IP_SCRIPTSERVER", "127.0.0.1").toString();
            _PORT_ScriptServer = ini->value("PORT_SCRIPTSERVER", 31414).toInt();
            
            _IP_OU[0] = ini->value("IP_CONTROLSIMITATOR", "192.168.7.108").toString();
            _PORT_OU[0] = ini->value("PORT_CONTROLSIMITATOR", 20000).toInt();
            for (int i = 1; i < 5; ++i) {
                qDebug() << "IP_CONTROLSIMITATOR" + QString::number(i);
                _IP_OU[i] = ini->value("IP_CONTROLSIMITATOR" +QString::number(i), "127.0.0.2").toString();
                _PORT_OU[i] = ini->value("PORT_CONTROLSIMITATOR" + QString::number(i), 20000+i).toInt();
            }

            _IP_Rita = ini->value("IP_RITA", "127.0.0.1").toString();
            _PORT_Rita = ini->value("PORT_RITA", 8006).toInt();

            _IP_Bort_Teh      = ini->value("IP_BORT_TEH", "127.0.0.1").toString();
            _PORT_Bort_Teh    = ini->value("PORT_BORT_TEH", 8006).toInt();
            _IP_Left_Pilot    = ini->value("IP_LEFT_PILOT", "127.0.0.1").toString();
            _PORT_Left_Pilot  = ini->value("PORT_LEFT_PILOT", 8006).toInt();
            _IP_Right_Pilot   = ini->value("IP_RIGHT_PILOT", "127.0.0.1").toString();
            _PORT_Right_Pilot = ini->value("PORT_RIGHT_PILOT", 8006).toInt();

            _IP_CARCOMP = ini->value("IP_CARCOMP", "127.0.0.1").toString();
            _PORT_CARCOMP = ini->value("PORT_CARCOMP", 10010).toInt();

            _IP_ScreenItems = ini->value("IP_SCREEN_ITEMS", "127.0.0.1").toString();
            _PORT_ScreenItems = ini->value("PORT_SCREEN_ITEMS", 20001).toInt();
            
            _IP_AviaMotionSystem = ini->value("IP_AVIA_MOTIONSYSTEM", "127.0.0.1").toString();
            _PORT_AviaMotionSystem = ini->value("PORT_AVIA_MOTIONSYSTEM", 30303).toInt();
        }
        ini->endGroup();
        ini->beginGroup("PROJECTORS");
        {
            _IP_PROJECTORS = ini->value("IP").toString();
            _PASS_PROJECTORS = ini->value("PASSWORD").toString();
            _PORT_PROJECTORS = ini->value("PORT", 4352 ).toInt();
            _TIME_PROJECTORS = ini->value("TIME", 60000 ).toInt();
        }
        ini->endGroup();

        ini->beginGroup("POSITION");
        {
            _FR_x = ini->value("fr_x").toInt();
            _FR_y = ini->value("fr_y").toInt();
        }
        ini->endGroup();
    }
    ini->destroyed();
}
