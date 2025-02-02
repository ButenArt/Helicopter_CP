#include "Helicopter_imager.h"

Helicopter_Imager::Helicopter_Imager( QObject* parent ) : QObject( parent )
{
	qDebug() << __FUNCTION__;
}

Helicopter_Imager::~Helicopter_Imager()
{
	qDebug() << __FUNCTION__;
}

void Helicopter_Imager::Init()
{
}

void Helicopter_Imager::ImagerStatePacket(HelicopterState /*packet*/)
{
}

QDebug operator<<(QDebug out, const BallisticDataPacket& ep)
{
	out << "PacketReady=" << ep.PacketReady
		<< ", ObjectiveUID=" << ep.ObjectiveUID
		<< ", TargetUID=" << ep.TargetUID
		<< ", ObjectiveType=" << ep.ObjectiveType
		<< ", Hit=" << ep.Hit
		<< ", Radius=" << ep.Radius
		<< ", GunType=" << ep.GunType
		<< ", TargetState=" << ep.TargetState
		<< ", TriggerID=" << ep.TriggerID
		<< ", Direction=" << ep.Direction
		<< ", Hit_X=" << ep.Hit_X
		<< ", Hit_Y=" << ep.Hit_Y
		;
	return out;
}
QDebug operator<<(QDebug out, const Elements& ep)
{
	out << "ElementType=" << ep.ElementType
		<< ", ElementState=" << ep.ElementState
		<< ", ElementNameSize=" << ep.ElementNameSize
		<< ", ElementName=" << ep.ElementName
		<< ", CommentSize=" << ep.CommentSize
		<< ", Comment=" << ep.Comment
		;
	return out;
}
QDebug operator<<(QDebug out, const ImagerElementsStateDataPacket& ep)
{
	out << "PacketReady=" << ep.PacketReady
		<< ", ElementsCount=" << ep.ElementsCount
		<< ", ElementsStateList=" << ep.ElementsStateList
		<< ", OfflineCounter=" << ep.OfflineCounter
		;
	return out;
}
QDebug operator<<(QDebug out, const ImagerReadyDataPacket& ep)
{
	out << "PacketReady=" << ep.PacketReady
		<< ", simulatorType=" << static_cast<int>(ep.simulatorType)
		<< ", ImagerReady=" << ep.ImagerReady
		;
	return out;
}
QDebug operator<<(QDebug out, const ImagerStateDataPacket& ep)
{
	out << "PacketReady=" << ep.PacketReady
		<< ", X=" << ep.X
		<< ", Y=" << ep.Y
		<< ", Z=" << ep.Z
		<< ", Roll=" << ep.Roll
		<< ", Pitch=" << ep.Pitch
		<< ", Course=" << ep.Course
		<< ", Sail=" << ep.Sail
		<< ", Surface=" << ep.Surface
		<< ", Speed=" << ep.Speed
		<< ", WheelsCount=" << ep.WheelsCount
		<< ", WheelsList=" << ep.WheelsList
		<< ", EngineTorque=" << ep.EngineTorque
		<< ", RPM=" << ep.RPM
		<< ", AmmoLoad=" << ep.AmmoLoad
		;
	return out;
}
QDebug operator<<(QDebug out, const MissedDataPacket& ep)
{
	out << "PacketReady=" << ep.PacketReady
		<< ", X=" << ep.X
		<< ", Y=" << ep.Y
		<< ", Missed=" << ep.Missed
		;
	return out;
}
QDebug operator<<(QDebug out, const CargoDataPacket& ep)
{
	out << "PacketReady=" << ep.PacketReady
		<< ", Load=" << ep.Load
		;
	return out;
}
QDebug operator<<(QDebug out, const ProjectileDataPacket& ep)
{
	out << "PacketReady=" << ep.PacketReady
		<< ", TrajectoriesCount=" << ep.TrajectoriesCount
		<< ", TrajectoriesList=" << ep.TrajectoriesList
		;
	return out;
}
QDebug operator<<(QDebug out, const TargetControl& ep)
{
	out << "ObjectiveUID=" << ep.ObjectiveUID
		<< ", TargetUID=" << ep.TargetUID
		<< ", TargetState=" << ep.TargetState
		;
	return out;
}
QDebug operator<<(QDebug out, const TargetsControlDataPacket& ep)
{
	out << "PacketReady=" << ep.PacketReady
		<< ", mapTargetsCount=" << ep.mapTargetsCount
		<< ", targetControlList=" << ep.targetControlList
		;
	return out;
}
QDebug operator<<(QDebug out, const Trajectories& ep)
{
	out << "WeaponID=" << ep.WeaponID
		<< ", WeaponType=" << ep.WeaponType
		<< ", ProjectileType=" << ep.ProjectileType
		<< ", ProjectileSubType=" << ep.ProjectileSubType
		<< ", InitialVelocity=" << ep.InitialVelocity
		<< ", ShotID=" << ep.ShotID
		<< ", time=" << ep.time
		<< ", pos_offset=" << ep.pos_offset
		<< ", pos_height=" << ep.pos_height
		<< ", pos_distance=" << ep.pos_distance
		<< ", rise_angle=" << ep.rise_angle
		;
	return out;
}
QDebug operator<<(QDebug out, const TriggerDataPacket& ep)
{
	out << "PacketReady=" << ep.PacketReady
		<< ", ObstacleUID=" << ep.ObstacleUID
		<< ", ObstacleType=" << ep.ObstacleType
		<< ", ObstacleIn=" << ep.ObstacleIn
		<< ", ObstacleOut=" << ep.ObstacleOut
		<< ", Trigger=" << ep.Trigger
		;
	return out;
}

QDebug operator<<( QDebug out, const ExitstateDataPacked& ep )
{
	out << "simulatorType = " << static_cast<int>( ep.Cause )
		;
	return out;
}

bool operator==( const ImagerReadyDataPacket& lhs, const ImagerReadyDataPacket& rhs )
{
	return lhs.ImagerReady == rhs.ImagerReady
		&& lhs.simulatorType == rhs.simulatorType;
}

bool operator!=( const ImagerReadyDataPacket& lhs, const ImagerReadyDataPacket& rhs )
{
	return !( lhs == rhs );
}

QDebug operator<<(QDebug out, const Wheels& ep)
{
	out << "PacketReady=" << ep.wheelSpeed
		;
	return out;
}

ImagerReadyDataPacket& ImagerReadyDataPacket::operator=( const ImagerReadyDataPacket& rhs )
{
	if ( this != &rhs )
	{
		PacketReady = rhs.PacketReady;
		simulatorType = rhs.simulatorType;
		ImagerReady = rhs.ImagerReady;
	}
	return *this;
}
