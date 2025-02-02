#ifndef UDP_OU_H
#define UDP_OU_H

#include <QObject>
#include <QTimer>
#include <QUdpSocket>
#include <Windows.h>

class UDP_OU : public QObject
{
    Q_OBJECT
  public:
    UDP_OU();
    ~UDP_OU();
  protected:
    void sendUDP(QByteArray udp_packet, QHostAddress hostaddr, quint16 udp_port);
    BYTE SetByte(bool bit1, bool bit2, bool bit3, bool bit4, bool bit5, bool bit6, bool bit7, bool bit8);
    DWORD TxInterval;             // интервал в мс между UDP посылками
    QString IP_Broadcast;         // UDP broadcast IP
    WORD REMOTE_PORT_OU_MV_UPR;   // UDP порт передачи данных UPR для приложения OU механика-водителя
    WORD REMOTE_PORT_OU_MV_IND;   // UDP порт передачи данных IND для приложения OU механика-водителя
    WORD REMOTE_PORT_OU_BO_UPR;   // UDP порт передачи данных UPR для приложения OU боевого отделения
    WORD REMOTE_PORT_OU_BO_IND;   // UDP порт передачи данных IND для приложения OU боевого отделения
    QTimer *UDP2OU_SendTimer;
  private:
    QUdpSocket *UdpSocket;

};

#endif // UDP_OU_H
