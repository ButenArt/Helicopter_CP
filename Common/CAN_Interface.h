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
	virtual ~ICAN_Board( ) = default; // ���������� ����������

public:
	static ICAN_Board* CreateInstance( QString name );	// �������-�������
	virtual void Delete( ) = 0;					// ������� ������

	virtual int ReadBuffer( int port ) = 0; // ���� �� ������ �� �����
	virtual int SendPacket( DWORD addr, BYTE FrameType, BYTE Length, BYTE frame[ ], int port ) = 0; // ������� ������
	virtual int ReadPacket( DWORD& addr, BYTE& FrameType, BYTE& Length, BYTE frame[ ], int port ) = 0; // ����� ������
	
	ICAN_Board& operator=( const ICAN_Board& ) = delete; // ������ ������������
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
	int ReadBuffer( int port ); // ���� �� ������ �� �����
	int SendPacket( DWORD addr, BYTE FrameType, BYTE Length, BYTE frame[ ], int port ) override; // ������� ������
	int ReadPacket( DWORD& addr, BYTE& FrameType, BYTE& Length, BYTE frame[ ], int port ) override; // ����� ������
};

#endif // ICAN