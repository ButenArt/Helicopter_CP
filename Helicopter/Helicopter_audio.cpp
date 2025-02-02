#include "Helicopter_audio.h"
#include <QDebug>

Helicopter_audio::Helicopter_audio( QObject* parent ) : T_Audio( parent )
{
	qDebug( ) << __FUNCTION__;
}

Helicopter_audio::~Helicopter_audio( )
{
	qDebug( ) << __FUNCTION__;
}

void Helicopter_audio::run( )
{
	qDebug( ) << __FUNCTION__;
	if ( QDir( ).exists( QCoreApplication::applicationDirPath( ).append( "/wav/" ) ) )
	{
		sounds.resize( 21 );
		// можно было бы и через push_back но так нагляднее под каким индексом находится звук, не забывает сделать ресайз под количество    
		sounds[FPUMP_WORK] = new TSound( FPUMP_WORK, "/wav/fpump_work.wav" );;
		sounds[PO500_WORK] = new TSound( PO500_WORK, "/wav/po500_work.wav" );
		sounds[ENG_TA_DUB] = new TSound( ENG_TA_DUB, "/wav/eng_ta.wav" );  // TODO дубляж
		sounds[ENG] = new TSound( ENG, "/wav/eng.wav" );
		sounds[FIRE_KRAN] = new TSound( FIRE_KRAN, "/wav/fire_kran.wav" );
		sounds[TA_SCROLL] = new TSound( TA_SCROLL, "/wav/TA_scroll.wav" );
		sounds[TA_START] = new TSound( TA_START, "/wav/TA_start.wav" );
		sounds[TA_WORK] = new TSound( TA_WORK, "/wav/TA_work.wav" );
		sounds[TA_STOP] = new TSound( TA_STOP, "/wav/TA_stop.wav" );
		sounds[ENG_START] = new TSound( ENG_START, "/wav/eng_start.wav" );
		sounds[ENG_TA] = new TSound( ENG_TA, "/wav/eng_ta.wav" );
		sounds[ENG_75] = new TSound( ENG_75, "/wav/eng_75.wav" );
		sounds[ENG_80] = new TSound( ENG_80, "/wav/eng_80.wav" );
		sounds[ENG_90] = new TSound( ENG_90, "/wav/eng_90.wav" );
		sounds[ENG_MAX] = new TSound( ENG_MAX, "/wav/eng_max.wav" );
		sounds[ENG_2_DOWN] = new TSound( ENG_2_DOWN, "/wav/eng_2_down.wav" );
		sounds[ENG_1_DOWN] = new TSound( ENG_1_DOWN, "/wav/eng_1_down.wav" );
		sounds[PPS] = new TSound( PPS, "/wav/PPS.wav" );
		sounds[BREAK_P] = new TSound( BREAK_P, "/wav/break_p.wav" );
		sounds[BREAK_R] = new TSound( BREAK_R, "/wav/break_r.wav" );
		sounds[POMPASH] = new TSound(POMPASH, "/wav/pompashEng.wav");
	}
	Init( );
	SoundsEnabled = true;
	emit ready( );
}
