#include "UDP_OU.h"

UDP_OU::UDP_OU()
{
    TxInterval = 0;
    IP_Broadcast = "";
    REMOTE_PORT_OU_MV_UPR = 0;
    REMOTE_PORT_OU_MV_IND = 0;
    REMOTE_PORT_OU_BO_UPR = 0;
    REMOTE_PORT_OU_BO_IND = 0;
    UdpSocket = new QUdpSocket(this);
    UDP2OU_SendTimer = new QTimer(this);
}

UDP_OU::~UDP_OU()
{
    delete UDP2OU_SendTimer;
    delete UdpSocket;
}

void UDP_OU::sendUDP(QByteArray udp_packet, QHostAddress hostaddr, quint16 udp_port)
{
    UdpSocket->writeDatagram(udp_packet.data(), udp_packet.size(), hostaddr, udp_port);
}

BYTE UDP_OU::SetByte(bool bit1, bool bit2, bool bit3, bool bit4, bool bit5, bool bit6, bool bit7, bool bit8)
{
    BYTE tmp = 0x00;
    if(bit1) tmp = tmp | 0x01;
    if(bit2) tmp = tmp | 0x02;
    if(bit3) tmp = tmp | 0x04;
    if(bit4) tmp = tmp | 0x08;
    if(bit5) tmp = tmp | 0x10;
    if(bit6) tmp = tmp | 0x20;
    if(bit7) tmp = tmp | 0x40;
    if(bit8) tmp = tmp | 0x80;
    return tmp;
}
