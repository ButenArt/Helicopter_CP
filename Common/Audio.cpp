#include "Audio.h"
#include <QDebug>
#include <QCoreApplication>
#include <QAudioFormat>


// функции Play и PlayOnce вызываются из главного потока, что бы проигывание звука выполнялось в своем потоке
// эти функции эмитят соответствующий сигнал через очередь, который выполняется слотов в аудио потоке

// TODO реализовать плавное изменение частоты работы двигателя
// TODO побороть пропадание звука при вызове suspend. Сейчас звук играет постоянно, просто приглушается громкость до 0.

T_Audio::T_Audio( QObject* parent ) : QObject( parent )
{
	//SoundsEnabled = true;
	SoundsEnabled = false;
}

T_Audio::~T_Audio( )
{

}

void T_Audio::Init( )
{
	//audioFormat.setSampleRate(defaultSamleRate);
	for ( auto& sound : sounds )
	{
		sound->MIX = false;
		//        connect( sound->audioOutput, SIGNAL( stateChanged( QAudio::State )), sound, SLOT( On_StateChanget( QAudio::State )), Qt::QueuedConnection );
	}
	connect( this, SIGNAL( play_once( int ) ), this, SLOT( On_PlayOnce( int ) ), Qt::QueuedConnection );
	connect( this, SIGNAL( play( int ) ), this, SLOT( On_Play( int ) ), Qt::QueuedConnection );
	connect( this, SIGNAL( stop( int ) ), this, SLOT( On_Stop( int ) ), Qt::QueuedConnection );
}

void T_Audio::Play( int num, bool enable )
{
	//	qDebug( ) << __FUNCTION__ << "( " << num << " )";
	if ( !SoundsEnabled ) return;
	if ( sounds.size( ) > num && sounds[num]->audioOutput )
	{
		sounds[num]->MIX = enable;
		sounds[num]->REP = true;
		if ( sounds[num]->File.atEnd( ) )
			sounds[num]->File.seek( sizeof( WAVFILEHEADER ) );
		if ( enable )
			emit play( num );
		else
			emit stop( num );
	}
}

void T_Audio::Stop( int num )
{
	//	qDebug( ) << __FUNCTION__ << "( " << num << " )";
	if ( !SoundsEnabled ) return;
	if ( sounds.size( ) > num && sounds[num]->audioOutput )
	{
		sounds[num]->MIX = false;
		sounds[num]->REP = false;
		sounds[num]->File.seek( sizeof( WAVFILEHEADER ) );
		//sounds[num]->audioOutput->suspend();
		emit stop( num );
	}
}

// TODO попробую остановку сделать внутри основного потока
void T_Audio::StopAll( )
{
	//qDebug( ) << __FUNCTION__ << "( )";
	for ( auto& sound : sounds )
	{
		if ( sound->audioOutput ) sound->audioOutput->suspend( );
	}
}

void T_Audio::PlayOnce( int num )
{
	//	qDebug( ) << __FUNCTION__ << "( " << num << " )";
	if ( !SoundsEnabled ) return;
	if ( sounds.size( ) > num && sounds[num]->audioOutput )
	{
		sounds[num]->MIX = true;
		sounds[num]->REP = false;
		sounds[num]->File.seek( sizeof( WAVFILEHEADER ) ); // Устанавливаем на начало 
		//    qDebug() << "запуск проигрывания" << sounds[num]->URL;
		emit play_once( num );
	}
}

void T_Audio::On_PlayOnce( int num )
{
	//	qDebug( ) << __FUNCTION__ << "( " << num << " )";
	if ( sounds.size( ) > num && sounds[num]->audioOutput )
	{
		sounds[num]->audioOutput->start( &( sounds[num]->File ) );
		sounds[num]->audioOutput->setVolume( 1.f );
	}
}

// pushmode
void T_Audio::On_Play( int num )
{
	//qDebug( ) << __FUNCTION__ << "( " << num << " )";
	if ( sounds.size( ) > num && sounds[num]->audioOutput )
	{
		sounds[num]->audioOutput->resume( );
		sounds[num]->audioOutput->setVolume( 1.f );
	}
}

void T_Audio::On_Stop( int num )
{
	//qDebug( ) << __FUNCTION__ << "( " << num << " )";
	if ( sounds.size( ) > num && sounds[num]->audioOutput )
	{
		//sounds[num]->audioOutput->suspend( );
		sounds[num]->audioOutput->setVolume( 0.f );
	}
}

//enum State { ActiveState, SuspendedState, StoppedState, IdleState, InterruptedState };
void TSound::On_StateChanget( QAudio::State state )
{
	qDebug( ) << __FUNCTION__ << "(" << state << ")";
	switch ( state )
	{
	case QAudio::ActiveState:
		qDebug( ) << "запуск проигрывания" << URL;
		// Active for other reasons
		break;
	case QAudio::SuspendedState:
		// Suspend for other reasons
		qDebug( ) << "приостоновка проигрывания" << URL;
		break;
	case QAudio::IdleState:
		// Finished playing доиграли до конца файла, если надо - повторяем его
		qDebug( ) << "окончание проигрывания" << URL;
		emit stoping( index );
		break;
	case QAudio::StoppedState:
		qDebug( ) << "остановка проигрывания" << URL;
		// Stopped for other reasons
		break;

	default:
		break;
	}
}

TSound::TSound( int index, QString name ) :index( index ), VOL( 1.f ), MIX( false ), REP( true ), fading( 0 ), audioOutput( nullptr )
{
	URL = QCoreApplication::applicationDirPath( ).append( name );
	this->File.setFileName( this->URL );
	WAVFILEHEADER WavFileHeader;
	if ( this->File.open( QIODevice::ReadOnly ) &&
		this->File.read( (char*)&WavFileHeader, sizeof( WavFileHeader ) ) == sizeof( WavFileHeader ) )
	{
		//qDebug( ) << "файл:" << this->URL;
		qDebug( ) << __FUNCTION__ << "( " << this->URL << " )";
		// Распечатать информацию чтения
	//qDebug( ).nospace( ) << "RiffName:" << WavFileHeader.RiffName[0] << WavFileHeader.RiffName[1] << WavFileHeader.RiffName[2] << WavFileHeader.RiffName[3];
	//qDebug( ).nospace( ) << "WavName:" << WavFileHeader.WavName[0] << WavFileHeader.WavName[1] << WavFileHeader.WavName[2] << WavFileHeader.WavName[3];
	//qDebug( ).nospace( ) << "FmtName:" << WavFileHeader.FmtName[0] << WavFileHeader.FmtName[1] << WavFileHeader.FmtName[2] << WavFileHeader.FmtName[3];
	//qDebug( ).nospace( ) << "DATANAME:" << WavFileHeader.DATANAME[0] << WavFileHeader.DATANAME[1] << WavFileHeader.DATANAME[2] << WavFileHeader.DATANAME[3];
	//qDebug( ) << "Длина блока FMT:" << WavFileHeader.nFmtLength;
	//qDebug( ) << "Следуйте PCM:" << WavFileHeader.nAudioFormat;
	//qDebug( ) << "Количество каналов:" << WavFileHeader.nChannleNumber;
	//qDebug( ) << "Частота отбора проб:" << WavFileHeader.nSampleRate;
	//qDebug( ) << "Блок выравнивания блока данных:" << WavFileHeader.nBytesPerSample;
	//qDebug( ) << "Средние байты в секунду:" << WavFileHeader.nBytesPerSecond;
	//qDebug( ) << "Количество цифр данных, полученных каждой выборки :" << WavFileHeader.nBitsPerSample;
		QAudioFormat Format;

		// Установите формат записи
		Format.setSampleRate( WavFileHeader.nSampleRate ); // Установите скорость дискретизации в отборную проб Гц. Соберите, сколько звуковых данных собираются в секундах, собранные в секунду.
		Format.setChannelCount( WavFileHeader.nChannleNumber ); // установить количество каналов на канал.
		Format.setSampleSize( WavFileHeader.nBitsPerSample ); /*Установите размер образца к указанным образцам, обычно составляет 8 или 16, но некоторые системы могут поддерживать большее количество образцов.*/
		Format.setCodec( "audio/pcm" ); // Настройка формата кодирования
		Format.setByteOrder( QAudioFormat::LittleEndian ); // образец - небольшой конечный порядок байта
		Format.setSampleType( QAudioFormat::SignedInt ); // тип образца

		this->File.seek( sizeof( WAVFILEHEADER ) );
		this->audioOutput = new QAudioOutput( QAudioDeviceInfo::defaultOutputDevice( ), Format, this );
		this->audioOutput->setVolume( this->VOL );
		this->audioOutput->start( &( this->File ) );
		this->audioOutput->suspend( );
		// цепляем событие изменения статуса проигрывания на метод класса this
		connect( this->audioOutput, SIGNAL( stateChanged( QAudio::State ) ), this, SLOT( On_StateChanget( QAudio::State ) ), Qt::QueuedConnection );
	}
	else
	{
		qDebug( ) << "Проблема чтения файла:" << this->URL;
		this->audioOutput = nullptr;
	}
};
