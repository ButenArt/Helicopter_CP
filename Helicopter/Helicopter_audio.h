#ifndef Helicopter_AUDIO_H
#define Helicopter_AUDIO_H

#include <Windows.h>
#include <QObject>
#include <QCoreApplication>
#include <QDir>
#include "Common\Audio.h"


class Helicopter_audio : public T_Audio
{
    Q_OBJECT

public:
    enum audio {
        FPUMP_WORK = 0, PO500_WORK, ENG_TA_DUB, ENG, FIRE_KRAN, TA_SCROLL, TA_START, TA_WORK, TA_STOP,
        ENG_START, ENG_TA, ENG_75, ENG_80, ENG_90, ENG_MAX, ENG_2_DOWN, ENG_1_DOWN, PPS, BREAK_P, BREAK_R, POMPASH
    };

    Helicopter_audio( QObject* parent = nullptr );
    ~Helicopter_audio();
public slots:
    void run( );
signals:
    void ready( );
};


#endif // Helicopter_AUDIO_H
