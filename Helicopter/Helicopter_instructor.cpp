#include "Helicopter_instructor.h"
#include <QTimer>
#include <sstream>
#include <iomanip>

Helicopter_Instructor::Helicopter_Instructor( QObject* parent ) : QObject( parent )
{
    qDebug() << __FUNCTION__;
    TimerExerciseTime = new QTimer(this);
    TimerExerciseTime->setInterval(1000);
    connect(TimerExerciseTime, &QTimer::timeout, [=]() { ++exerciseTime; });
    Init();
}

Helicopter_Instructor::~Helicopter_Instructor()
{
    qDebug() << __FUNCTION__;
}

void Helicopter_Instructor::Init()
{
    qDebug() << __FUNCTION__;
    exerciseReInit = false;
    exerciseStarted = false;
    exerciseStop = false;
    exerciseTime = 0;
    exerciseTimeOld = 0;
}

void Helicopter_Instructor::UnloadAmmo()
{
    AmmunitionInPack.PacketReady = false;
    AmmunitionInPack.AmmunitionList.clear();
    AmmunitionInPack.packageCount = 0;
}

void Helicopter_Instructor::ExerciseReStart()
{
    qDebug() << __FUNCTION__;
    exerciseReInit = true;
    exerciseStarted = true;
    exerciseStop = false;
    TimerExerciseTime->start();
}

void Helicopter_Instructor::ExerciseStop()
{
    qDebug() << __FUNCTION__;
    exerciseReInit = false;
    exerciseStarted = false;
    exerciseStop = true;
    TimerExerciseTime->stop();
    exerciseTimeOld = exerciseTime;
    exerciseTime = 0;
}

/*
{
    "d":24, "m" : 2, "y" : 2022, "h" : 12, "mi" : 13,
        "t" : 0.0,   // температура?
        "cm" : 3, //cloud grade
        "cb" : 500.0,  // нижняя граница облачности
        "ct" : 600.0,  // верхнаяя граница облачности
        "s" : 0,       // бальность дождя и снега?
        "ws" : 0.0,    // скорость ветра
        "wd" : 0.0,    // направление ветра
        "mv" : 50000.0,  // дальность видимости как туман
        "vws" : 0.0,
        "wb" : 0.0,
        "p" : 0.0,    // давление ?
        "sn" : 9,     // заснеженность
        "cam" : 0     // camera id
}
*/

QString Weather::json()
{
    std::stringstream ss;
    ss << std::fixed << /*std::setw(11) <<*/ std::setprecision(1) << std::setfill('0') <<
        "{" <<
        "\"d\":" << day << ","
        "\"m\":" << month << ","
        "\"y\":" << year << ","
        "\"h\":" << hour << ","
        "\"mi\":" << min << ","
        "\"t\":" << temp << ","
        "\"cm\":" << cloud << ","
        "\"cb\":" << cloud_bot << ","
        "\"ct\":" << cloud_top << ","
        "\"s\":" << rain << ","
        "\"ws\":" << wind_speed << ","
        "\"wd\":" << wind_dir << ","
        "\"mv\":" << visibility << ","
        "\"vws\":" << vws << ","
        "\"wb\":" << wb << ","
        "\"p\":" << pressure << ","
        "\"sn\":" << snow << ","
        "\"cam\":" << cam <<
        "}";
    return QString::fromStdString(ss.str());
}

Weather::Weather(int i)
{
    //лето день облачно 5
    if (i == 1)
    {
        day = 16;
        month = 7;
        year = 2022;
        hour = 12;
        min = 0;
        temp = 25.f;
        cloud = 5;
        cloud_bot = 2000.f;
        cloud_top = 2500.f;
        rain = 0;
        wind_speed = 7;
        wind_dir = 90;
        visibility = 50000.f;
        vws = 0.f;
        wb = 0.f;
        pressure = 710.f;
        snow = 0;
        cam = 0;
    }
    //лето ночь ясно
    else if (i == 2)
    {
        day = 16;
        month = 7;
        year = 2022;
        hour = 2;
        min = 0;
        temp = 25.f;
        cloud = 0;
        cloud_bot = 100;
        cloud_top = 100;
        rain = 0;
        wind_speed = 0;
        wind_dir = 0;
        visibility = 50000.f;
        vws = 0.f;
        wb = 0.f;
        pressure = 745.f;
        snow = 0;
        cam = 0;
    }
    //зима день облачно 10
    else if (i == 3)
    {
        day = 16;
        month = 1;
        year = 2022;
        hour = 12;
        min = 0;
        temp = -10.f;
        cloud = 10;
        cloud_bot = 300;
        cloud_top = 500;
        rain = 0;
        wind_speed = 0;
        wind_dir = 0;
        visibility = 5000.f;
        vws = 0.f;
        wb = 0.f;
        pressure = 690.f;
        snow = 10;
        cam = 0;
    }
    //лето день ясно
    else
    {
        day = 16;
        month = 7;
        year = 2022;
        hour = 12;
        min = 0;
        temp = 25.f;
        cloud = 0;
        cloud_bot = 100;
        cloud_top = 100;
        rain = 0;
        wind_speed = 0;
        wind_dir = 0;
        visibility = 50000.f;
        vws = 0.f;
        wb = 0.f;
        pressure = 745.f;
        snow = 0;
        cam = 0;
    }

}

//QDataStream& operator>>( QDataStream& stream, Ammunition& ammunition )
//{
//    stream >> ammunition.gunType;
//    stream >> ammunition.ammoType;
//    stream >> ammunition.amount;
//    return stream;
//}

QDebug operator<<(QDebug out, const ExerciseParametersPacket& ep)
{
    out << "ReStart:" << ep.ReStart
        << ", Pause:" << ep.Pause
        << ", Stop:" << ep.Stop
        << ", ExerciseNum:" << ep.ExerciseNum
        << ", MapType:" << ep.MapType
        << ", night:" << ep.night
        << ", season:" << ep.season
        << ", Joystick:" << ep.Joystick
        << ", AutoloadingSystem:" << ep.AutoloadingSystem     // Автомат заряжания
        << ", Uuid:" << ep.Uuid                    // GUID карты и упражнения
        << ", directionsNumber:" << ep.directionsNumber            // Количество направлений
        << ", backfire:" << ep.backfire              // Ответный огонь
        << ", selectedKit:" << ep.selectedKit
        << ", selectedMode:" << ep.selectedMode
        << ", distance2screen:" << ep.distance2screen
        << ", run:" << ep.run
        << ", lang:" << ep.lang;                       // Язык интерфейса
    return out;
};

QDebug operator<<(QDebug out, const EnvironmentDataPacket& ep)
{
    out << "unixtime:" << ep.unixtime
        << ", temperature:" << ep.temperature
        << ", cloudsGrade:" << ep.cloudsGrade
        << ", bottomCloudsLevel:" << ep.bottomCloudsLevel
        << ", topCloudsLevel:" << ep.topCloudsLevel
        << ", moistureLevel:" << ep.moistureLevel
        << ", windSpeed:" << ep.windSpeed
        << ", windDirection:" << ep.windDirection
        << ", visibility:" << ep.visibility
        << ", pressure:" << ep.pressure
        << ", snowLevel:" << ep.snowLevel
        << ", camNumber:" << ep.camNumber
        << ", fog:" << ep.fog
        << ", amount_precipitation:" << ep.amount_precipitation
        << ", intensity_precipitation:" << ep.intensity_precipitation
        << ", intensity_vpp:" << ep.intensity_vpp
        ;
    return out;
}

QDebug operator<<(QDebug out, const MapTargets& ep)
{
    out << "TargetType:" << ep.TargetType
        << ", TargetColor:" << ep.TargetColor
        << ", azimuth:" << ep.azimuth
        << ", latitude:" << ep.latitude
        << ", longitude:" << ep.longitude
        << ", speed:" << ep.speed;
        return out;
}

QDebug operator<<(QDebug out, const TargetsDataPacket& ep)
{
    out << "mapTargetsCount:" << ep.mapTargetsCount
        << "mapTargetsList:" << ep.mapTargetsList;
    return out;
}

QDebug operator<<(QDebug out, const AmmunitionDataPacket& ep)
{
    out << "packageCount:" << ep.packageCount
        << ", AmmunitionList:" << ep.AmmunitionList;
    return out;
}

QDebug operator<<(QDebug out, const Ammunition& ep)
{
    out << "gunType:" << ep.gunType
        << ", ammoType:" << ep.ammoType
        << ", amount:" << ep.amount;
    return out;
}

QDebug operator<<(QDebug out, const MessagesDataPacket& ep)
{
    out << "MessageType:" << ep.MessageType
//        << ", MessageSize" << ep.MessageSize
        << ", MessageText" << ep.MessageText;
    return out;
}

QDebug operator<<(QDebug out, const MapObjects& ep)
{
    out << "type:" << ep.type
        << ", subType:" << ep.subType
        << ", azimuth:" << ep.azimuth
        << ", latitude:" << ep.latitude
        << ", longitude:" << ep.longitude;
    return out;
}
QDebug operator<<(QDebug out, const ObjectsDataPacket& ep)
{
    out << "mapObjCount:" << ep.mapObjCount
        << ", mapObjList:" << ep.mapObjList;
    return out;
}

QDebug operator<<(QDebug out, const ScriptDataPacket& ep)
{
    out << "type_load" << ep.type_load
        << ", bombs_weight" << ep.bombs_weight
        << ", aso2b" << ep.aso2b
        << ", consum_tank" << ep.consum_tank
        << ", left_tank" << ep.left_tank
        << ", right_tank" << ep.right_tank
        << ", refill" << ep.refill
        << ", empty_weight" << ep.empty_weight
        << ", crew_weight" << ep.crew_weight
        << ", load_weight" << ep.load_weight
        << ", full_weight" << ep.full_weight
        << ",\n"
        << "external_cargo" << ep.external_cargo
        << ", cable_length" << ep.cable_length
        << ", cargo_weight" << ep.cargo_weight
        << ", form_cargo" << ep.form_cargo
        << ", latitude" << ep.latitude
        << ", longitude" << ep.longitude
        << ", height" << ep.height
        << ", curs" << ep.curs
        << ",\n"
        << "air_radius" << ep.air_radius
        << ", air_density" << ep.air_density
        << ", auto_lat" << ep.auto_lat
        << ", auto_long" << ep.auto_long
        << ", auto_radius" << ep.auto_radius
        << ", auto_density" << ep.auto_density;
    return out;
}

QDebug operator<<(QDebug out, const FailuresDataPacket& ep)
{
    out << "failureType" << ep.failureType
        << ", failureActive" << ep.failureActive;
    return out;
}

QDebug operator<<(QDebug out, const CommandsDataPacket& ep)
{
    out << "MoveForward" << ep.MoveForward
        ;
    return out;
}

QDebug operator<<(QDebug out, const ControlDataPacket& ep)
{
    out << "PacketReady" << ep.PacketReady
//        << ", CommandSize" << ep.CommandSize
        << ", CommandText" << ep.CommandText
        ;
    return out;
}

QDataStream& operator<<( QDataStream& out, const TargetsShow& ep )
{
    out << ep.startTime;
    out << ep.dayTime;
    out << ep.nightTime;
    return out;
}

QDataStream& operator>>( QDataStream& in, TargetsShow& ep )
{
    in >> ep.startTime;
    in >> ep.dayTime;
    in >> ep.nightTime;
    return in;
}

QDataStream& operator<<( QDataStream& out, const MapObjects& ep )
{
    out << ep.objUID;
    out << ep.type;
    out << ep.subType;
    out << ep.azimuth;
    out << ep.latitude;
    out << ep.longitude;
    return out;
}

QDataStream& operator>>( QDataStream& in, MapObjects& ep )
{
    in >> ep.objUID;
    in >> ep.type;
    in >> ep.subType;
    in >> ep.azimuth;
    in >> ep.latitude;
    in >> ep.longitude;
    return in;
}

QDataStream& operator<<( QDataStream& out, const EnvironmentDataPacket& ep )
{
    out << ep.unixtime;
    out << ep.temperature;
    out << ep.cloudsGrade;
    out << ep.bottomCloudsLevel;
    out << ep.topCloudsLevel;
    out << ep.moistureLevel;
    out << ep.windSpeed;
    out << ep.windDirection;
    out << ep.visibility;
    out << ep.verticalWindSpeed;
    out << ep.windBlows;
    out << ep.pressure;
    out << ep.snowLevel;
    out << ep.camNumber;
    out << ep.fog;
    out << ep.amount_precipitation;
    out << ep.intensity_precipitation;
    out << ep.intensity_vpp;
    return out;
}

QDataStream& operator<<( QDataStream& out, const TargetsDataPacket& ep )
{
    out << ep.mapTargetsList;
    return out;
}

QDataStream& operator<<( QDataStream& out, const MapTargets& ep )
{
    out << ep.ObjectiveUID;
    out << ep.TargetUID;
    out << ep.TargetType;
    out << ep.TargetColor;
    out << ep.azimuth;
    out << ep.latitude;
    out << ep.longitude;
    out << ep.running;
    out << ep.speed;
    out << ep.moveAzimuth;
    out << ep.moveDistance;
    out << ep.onlyNight;
    out << ep.onlyDay;
    out << ep.dropTarget;
    out << ep.blockTarget;
    out << ep.targetShowNumber;
    out << ep.targetsShowList;
    return out;
}

QDataStream& operator<<( QDataStream& out, const Ammunition& ammo )
{
    out.setByteOrder( QDataStream::LittleEndian );
    out << static_cast<quint8>( ammo.gunType );
    out << static_cast<quint8>( ammo.ammoType );
    out << static_cast<quint32>( ammo.amount );
    return out;
}

QDataStream& operator<<( QDataStream& out, const ScriptDataPacket& ep )
{
    out << ep.type_load;
    out << ep.bombs_weight;
    out << ep.aso2b;
    out << ep.consum_tank;
    out << ep.left_tank;
    out << ep.right_tank;
    out << ep.refill;
    out << ep.empty_weight;
    out << ep.crew_weight;
    out << ep.load_weight;
    out << ep.full_weight;
    out << ep.external_cargo;
    out << ep.cable_length;
    out << ep.cargo_weight;
    out << ep.form_cargo;
    out << ep.latitude;
    out << ep.longitude;
    out << ep.height;
    out << ep.curs;
    out << ep.start_delay_time;
    out << ep.dir_wind;
    out << ep.speed_wind;
    out << ep.vspeed_wind;
    out << ep.gusts_wind;
    out << ep.air_temp;
    out << ep.ait_press;
    out << ep.visibility;
    out << ep.cloud;
    out << ep.low_cloud;
    out << ep.hi_cloud;
    out << ep.fallout;
    out << ep.snow_cover;
    out << ep.air_radius;
    out << ep.air_density;
    out << ep.auto_lat;
    out << ep.auto_long;
    out << ep.auto_radius;
    out << ep.auto_density;

    return out;
}

QDataStream& operator<<( QDataStream& out, const ObjectsDataPacket& ep )
{
    out << ep.mapObjList;
    return out;
}

QDataStream& operator>>( QDataStream& in, Ammunition& ammo )
{
    in >> ammo.gunType >> ammo.ammoType >> ammo.amount;
    return in;
}

QDataStream& operator>>( QDataStream& in, ScriptDataPacket& ep )
{
    in >> ep.type_load;
    in >> ep.bombs_weight;
    in >> ep.aso2b;
    in >> ep.consum_tank;
    in >> ep.left_tank;
    in >> ep.right_tank;
    in >> ep.refill;
    in >> ep.empty_weight;
    in >> ep.crew_weight;
    in >> ep.load_weight;
    in >> ep.full_weight;
    in >> ep.external_cargo;
    in >> ep.cable_length;
    in >> ep.cargo_weight;
    in >> ep.form_cargo;
    in >> ep.latitude;
    in >> ep.longitude;
    in >> ep.height;
    in >> ep.curs;
    in >> ep.start_delay_time;
    in >> ep.dir_wind;
    in >> ep.speed_wind;
    in >> ep.vspeed_wind;
    in >> ep.gusts_wind;
    in >> ep.air_temp;
    in >> ep.ait_press;
    in >> ep.visibility;
    in >> ep.cloud;
    in >> ep.low_cloud;
    in >> ep.hi_cloud;
    in >> ep.fallout;
    in >> ep.snow_cover;
    in >> ep.air_radius;
    in >> ep.air_density;
    in >> ep.auto_lat;
    in >> ep.auto_long;
    in >> ep.auto_radius;
    in >> ep.auto_density;
    //ep.PacketReady = true;
    return in;
}

QDataStream& operator>>( QDataStream& in, ObjectsDataPacket& ep )
{
    in >> ep.mapObjList;
    ep.mapObjCount = ep.mapObjList.count( );
    //ep.PacketReady = true;
    return in;
}

QDataStream& operator<<( QDataStream& out, const AmmunitionDataPacket& packet )
{
    //out << packet.AmmunitionList.count( );
    out << packet.AmmunitionList;
    return out;
}
QDataStream& operator<<( QDataStream& out, const ExerciseParametersPacket& ep )
{
    out << ep.ReStart;
    out << ep.Pause;
    out << ep.Stop;
    out << ep.ExerciseNum;
    out << ep.MapType;
    out << ep.night;
    out << ep.season;
    out << ep.Joystick;
    out << ep.AutoloadingSystem;
    out << ep.Uuid;
    out << ep.directionsNumber;
    out << ep.backfire;
    out << ep.selectedKit;
    out << ep.selectedMode;
    out << ep.distance2screen;
    out << ep.run;
    out << ep.lang;
    return out;
}

QDataStream& operator>>( QDataStream& in, ExerciseParametersPacket& ep )
{
    in >> ep.ReStart;
    in >> ep.Pause;
    in >> ep.Stop;
    in >> ep.ExerciseNum;
    in >> ep.MapType;
    in >> ep.night;
    in >> ep.season;
    in >> ep.Joystick;
    in >> ep.AutoloadingSystem;
    in >> ep.Uuid;
    in >> ep.directionsNumber;
    in >> ep.backfire;
    in >> ep.selectedKit;
    in >> ep.selectedMode;
    in >> ep.distance2screen;
    in >> ep.run;
    in >> ep.lang;
    //ep.PacketReady = true;
    return in;
}

QDataStream& operator>>( QDataStream& in, EnvironmentDataPacket& ep )
{
    in >> ep.unixtime;
    in >> ep.temperature;
    in >> ep.cloudsGrade;
    in >> ep.bottomCloudsLevel;
    in >> ep.topCloudsLevel;
    in >> ep.moistureLevel;
    in >> ep.windSpeed;
    in >> ep.windDirection;
    in >> ep.visibility;
    in >> ep.verticalWindSpeed;
    in >> ep.windBlows;
    in >> ep.pressure;
    in >> ep.snowLevel;
    in >> ep.camNumber;
    in >> ep.fog;
    in >> ep.amount_precipitation;
    in >> ep.intensity_precipitation;
    in >> ep.intensity_vpp;
    //ep.PacketReady = true;
    return in;
}

QDataStream& operator>>( QDataStream& in, TargetsDataPacket& ep )
{
    in >> ep.mapTargetsList;
    ep.mapTargetsCount = ep.mapTargetsList.count( );
    //ep.PacketReady = true;
    return in;
}

QDataStream& operator>>( QDataStream& in, MapTargets& ep )
{
    in >> ep.ObjectiveUID;
    in >> ep.TargetUID;
    in >> ep.TargetType;
    in >> ep.TargetColor;
    in >> ep.azimuth;
    in >> ep.latitude;
    in >> ep.longitude;
    in >> ep.running;
    in >> ep.speed;
    in >> ep.moveAzimuth;
    in >> ep.moveDistance;
    in >> ep.onlyNight;
    in >> ep.onlyDay;
    in >> ep.dropTarget;
    in >> ep.blockTarget;
    in >> ep.targetShowNumber;
    in >> ep.targetsShowList;
    return in;
}

QDataStream& operator>>( QDataStream& in, AmmunitionDataPacket& packet )
{
    in >> packet.AmmunitionList;
    packet.packageCount = packet.AmmunitionList.size( );
    //packet.PacketReady = true;
    return in;
}
bool operator==( const ExerciseParametersPacket& lhs, const ExerciseParametersPacket& rhs )
{
    return lhs.ReStart == rhs.ReStart
        && lhs.Pause == rhs.Pause
        && lhs.Stop == rhs.Stop
        && lhs.ExerciseNum == rhs.ExerciseNum
        && lhs.MapType == rhs.MapType
        && lhs.night == rhs.night
        && lhs.season == rhs.season
        && lhs.Joystick == rhs.Joystick
        && lhs.AutoloadingSystem == rhs.AutoloadingSystem
        && lhs.Uuid == rhs.Uuid
        && lhs.directionsNumber == rhs.directionsNumber
        && lhs.backfire == rhs.backfire
        && lhs.selectedKit == rhs.selectedKit
        && lhs.selectedMode == rhs.selectedMode
        && lhs.distance2screen == rhs.distance2screen
        && lhs.run == rhs.run
        && lhs.lang == rhs.lang
        ;
}

bool operator==( const EnvironmentDataPacket& lhs, const EnvironmentDataPacket& rhs )
{
    return lhs.unixtime == rhs.unixtime
        && lhs.temperature == rhs.temperature
        && lhs.cloudsGrade == rhs.cloudsGrade
        && lhs.bottomCloudsLevel == rhs.bottomCloudsLevel
        && lhs.topCloudsLevel == rhs.topCloudsLevel
        && lhs.moistureLevel == rhs.moistureLevel
        && lhs.windSpeed == rhs.windSpeed
        && lhs.windDirection == rhs.windDirection
        && lhs.visibility == rhs.visibility
        && lhs.verticalWindSpeed == rhs.verticalWindSpeed
        && lhs.windBlows == rhs.windBlows
        && lhs.pressure == rhs.pressure
        && lhs.snowLevel == rhs.snowLevel
        && lhs.camNumber == rhs.camNumber
        && lhs.fog == rhs.fog
        && lhs.amount_precipitation == rhs.amount_precipitation
        && lhs.intensity_precipitation == rhs.intensity_precipitation
        && lhs.intensity_vpp == rhs.intensity_vpp
        ;
}

bool operator==( const AmmunitionDataPacket& lhs, const AmmunitionDataPacket& rhs )
{
    return lhs.packageCount == rhs.packageCount
        //&& lhs.AmmunitionList == rhs.AmmunitionList
        ;
}

bool operator==( const TargetsDataPacket& lhs, const TargetsDataPacket& rhs )
{
    return lhs.mapTargetsCount == rhs.mapTargetsCount
        //&& lhs.mapTargetsList == rhs.mapTargetsList
        ;
}
bool operator==( const ScriptDataPacket& lhs, const ScriptDataPacket& rhs )
{
    return lhs.type_load == rhs.type_load
        && lhs.bombs_weight == rhs.bombs_weight
        && lhs.aso2b == rhs.aso2b
        && lhs.consum_tank == rhs.consum_tank
        && lhs.left_tank == rhs.left_tank
        && lhs.right_tank == rhs.right_tank
        && lhs.refill == rhs.refill
        && lhs.empty_weight == rhs.empty_weight
        && lhs.crew_weight == rhs.crew_weight
        && lhs.load_weight == rhs.load_weight
        && lhs.full_weight == rhs.full_weight
        && lhs.external_cargo == rhs.external_cargo
        && lhs.cable_length == rhs.cable_length
        && lhs.cargo_weight == rhs.cargo_weight
        && lhs.form_cargo == rhs.form_cargo
        && lhs.latitude == rhs.latitude
        && lhs.longitude == rhs.longitude
        && lhs.height == rhs.height
        && lhs.curs == rhs.curs
        && lhs.start_delay_time == rhs.start_delay_time
        && lhs.dir_wind == rhs.dir_wind
        && lhs.speed_wind == rhs.speed_wind
        && lhs.vspeed_wind == rhs.vspeed_wind
        && lhs.gusts_wind == rhs.gusts_wind
        && lhs.air_temp == rhs.air_temp
        && lhs.ait_press == rhs.ait_press
        && lhs.visibility == rhs.visibility
        && lhs.cloud == rhs.cloud
        && lhs.low_cloud == rhs.low_cloud
        && lhs.hi_cloud == rhs.hi_cloud
        && lhs.fallout == rhs.fallout
        && lhs.snow_cover == rhs.snow_cover
        && lhs.air_radius == rhs.air_radius
        && lhs.air_density == rhs.air_density
        && lhs.auto_lat == rhs.auto_lat
        && lhs.auto_long == rhs.auto_long
        && lhs.auto_radius == rhs.auto_radius
        && lhs.auto_density == rhs.auto_density
        ;
}
bool operator==( const ObjectsDataPacket& lhs, const ObjectsDataPacket& rhs )
{
    return lhs.mapObjCount == rhs.mapObjCount
        //&& lhs.mapObjList == rhs.mapObjList
        ;
}
