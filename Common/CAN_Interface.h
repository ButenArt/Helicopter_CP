//#pragma once
#ifndef ICAN_H
#define ICAN_H

#include <windows.h>
#include <QDebug>
#include <Common/MOXA/MXCANFUNC.h>
#include <queue>

class ICAN_Board
{
protected:
	virtual ~ICAN_Board( ) = default; // защищенный деструктор

public:
	static ICAN_Board* CreateInstance( QString name );	// функция-фабрика
	virtual void Delete( ) = 0;					// удаляет объект

	virtual int ReadBuffer( int port ) = 0; // есть ли данные на прием
	virtual int SendPacket( DWORD addr, BYTE FrameType, BYTE Length, BYTE frame[ ], int port ) = 0; // отсылка пакета
	virtual int ReadPacket( DWORD& addr, BYTE& FrameType, BYTE& Length, BYTE frame[ ], int port ) = 0; // прием пакета
	
	ICAN_Board& operator=( const ICAN_Board& ) = delete; // запрет присваивания
};

//class CAN_Board : public ICAN_Board
//{
//
//};

class CAN_VIRT final :	public ICAN_Board
{
	std::queue<std::pair<DWORD, int>>  message;
private:
	CAN_VIRT( ){ qDebug( ) << __FUNCTION__; };
protected:
	~CAN_VIRT( ){ qDebug( ) << __FUNCTION__; };
public:
	friend ICAN_Board* ICAN_Board::CreateInstance( QString name );
	void Delete( ) override { delete this; };
	int ReadBuffer( int port ); // есть ли данные на прием
	int SendPacket( DWORD addr, BYTE FrameType, BYTE Length, BYTE frame[ ], int port ) override; // отсылка пакета
	int ReadPacket( DWORD& addr, BYTE& FrameType, BYTE& Length, BYTE frame[ ], int port ) override; // прием пакета
};

#endif // ICAN