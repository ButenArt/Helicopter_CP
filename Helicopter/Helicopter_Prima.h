#ifndef PRIMA_H
#define PRIMA_H

#include <QObject>
#include <QTimer>
#include <QVector>
#include <QMap>
#include "Helicopter_core.h"

class Helicopter_Prima : public QObject
{
    Q_OBJECT
public:
    Helicopter_Prima(Helicopter_core* Helicopter_);
    ~Helicopter_Prima();

private:
    Helicopter_core* Helicopter;
    QTimer* PrimaTimer;

    bool str_smen_freq_flag = false;
    quint8 str_freq = 0;
    quint8 chanel_ZU = 1; //канал при ЗУ от 1-40
    QMap<qint8, QVector<qint32>> chanels;
    QMap<qint8, QVector<qint32>> chanels_reserv;
    const QMap<qint8, QVector<BYTE>> modul = {
        {1, {0x52, 0x0a, 0x15, 0x42}},    //  АМэ  // 0x52 это пустота
        {2, {0x52, 0x0a, 0x15, 0x3a}},    //  АМу
        {3, {0x52, 0x17, 0x15, 0x2a}},    //  ОМв
        {4, {0x52, 0x17, 0x15, 0x34}},    //  ОМн
        {5, {0x52, 0x17, 0x15, 0x3a}},    //  ОМу
        {6, {0x17, 0x15, 0x01, 0x2a}},    // ОМ1в
        {7, {0x17, 0x15, 0x01, 0x34}}     // ОМ1н
    };

 public slots:
    void Algorithm();
    void PrimaSlot();
};

#endif // PRIMA_H

/*
| КЛЮЧ |   hex   | Алфавит |
 --------------------------
|  0  |   0x00  |     0   |
|  1  |   0x01  |     1   |
|  2  |   0x02  |     2   |
|  3  |   0x03  |     3   |
|  4  |   0x04  |     4   |
|  5  |   0x05  |     5   |
|  6  |   0x06  |     6   |
|  7  |   0x07  |     7   |
|  8  |   0x08  |     8   |
|  9  |   0x09  |     9   |
| 10  |   0x0a  |     А   |
| 11  |   0x0b  |     Б   |
| 12  |   0x0c  |     В   |
| 13  |   0x0d  |     Г   |
| 14  |   0x0e  |     Д   |
| 15  |   0x0f  |     Е   |
| 16  |   0x10  |     Ж   |
| 17  |   0x11  |     З   |
| 18  |   0x12  |     И   |
| 19  |   0x13  |     К   |
| 20  |   0x14  |     Л   |
| 21  |   0x15  |     М   |
| 22  |   0x16  |     Н   |
| 23  |   0x17  |     О   |
| 24  |   0x18  |     П   |
| 25  |   0x19  |     Р   |
| 26  |   0x1a  |     С   |
| 27  |   0x1b  |     Т   |
| 28  |   0x1c  |     У   |
| 29  |   0x1d  |     Ф   |
| 30  |   0x1e  |     Х   |
| 31  |   0x1f  |     Ц   |
| 32  |   0x20  |     Ч   |
| 33  |   0x21  |     Ш   |
| 34  |   0x22  |     Щ   |
| 35  |   0x23  |     Э   |
| 36  |   0x24  |     Ю   |
| 37  |   0x25  |     Я   |
| 38  |   0x26  |         |
| 39  |   0x27  |         |
| 40  |   0x28  |     а   |
| 41  |   0x29  |     б   |
| 42  |   0x2a  |     в   |
| 43  |   0x2b  |     г   |
| 44  |   0x2c  |     д   |
| 45  |   0x2d  |     е   |
| 46  |   0x2e  |     ж   |
| 47  |   0x2f  |     з   |
| 48  |   0x30  |     и   |
| 49  |   0x31  |     к   |
| 50  |   0x32  |     л   |
| 51  |   0x33  |     м   |
| 52  |   0x34  |     н   |
| 53  |   0x35  |     о   |
| 54  |   0x36  |     п   |
| 55  |   0x37  |     р   |
| 56  |   0x38  |     с   |
| 57  |   0x39  |     т   |
| 58  |   0x3a  |     у   |
| 59  |   0x3b  |     ф   |
| 60  |   0x3c  |     х   |
| 61  |   0x3d  |     ц   |
| 62  |   0x3e  |     ч   |
| 63  |   0x3f  |     ш   |
| 64  |   0x40  |     щ   |
| 65  |   0x41  |     ь   |
| 66  |   0x42  |     э   |
| 67  |   0x43  |     ю   |
| 68  |   0x44  |     я   |
| 69  |   0x45  |         |
| 70  |   0x46  |     %   |
| 71  |   0x47  |     ,   |
| 72  |   0x48  |     ↑   |
| 73  |   0x49  |     /   |
| 74  |   0x4a  |     +   |
| 75  |   0x4b  |     _   |
| 76  |   0x4c  |     "   |//это две полоски для мощности
| 77  |   0x4d  |     #   |//три полоски для мощности
| 78  |   0x4e  |     $   |//четыре полоски для мощности
| 79  |   0x4f  |     &   |//квадратик для громкости
| 80  |   0x50  |         |
| 81  |   0x51  |         |
| 82  |   0x52  |         |//Пустота - выключен
--------------------------
*/