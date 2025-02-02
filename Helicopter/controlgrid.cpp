#include "controlgrid.h"
#include <QDebug>

ControlGrid::ControlGrid( QWidget* parent ) : QWidget( parent )
{
	setFrame( 255 ); // вместо init

	QTimer* timer = new QTimer( this );
	timer->start( 20 );
	connect( timer, SIGNAL( timeout( ) ), this, SLOT( update( ) ) );
}


void ControlGrid::paintEvent( QPaintEvent* event )
{
	Q_UNUSED( event );

	QPainter qp( this );
	draw( &qp );
}

void ControlGrid::draw( QPainter* qp )
{
	QPen frame_pen( Qt::black, r / 2 );
	QPen line_pen( Qt::black, 1, Qt::DashLine );
	QPen joy_pen( Qt::red, r / 4 );
	QPen pedal_pen( Qt::darkGreen, r / 4 );
	QPen gas_pen( Qt::blue, r / 4 );

	//    qp->setRenderHint(QPainter::Antialiasing);

	qp->save( );
	{
		// оси
		qp->setPen( line_pen );
		qp->drawLine( c, 0, c, w );
		qp->drawLine( 0, c, w, c );

		// рамка
		qp->setPen( frame_pen );
		qp->drawRect( r / 2, r / 2, w - r, w - r );

		// рукоять, педали и газ без автопилота
		qp->setBrush( QBrush( Qt::red, Qt::SolidPattern ) );
		qp->setPen( joy_pen );
		qp->drawEllipse( joystick, r, r );

		qp->setBrush( QBrush( Qt::darkGreen, Qt::SolidPattern ) );
		qp->setPen( pedal_pen );
		qp->drawEllipse( pedals, r, r );

		qp->setBrush( QBrush( Qt::blue, Qt::SolidPattern ) );
		qp->setPen( gas_pen );
		qp->drawEllipse( gas_pitch, r, r );
		// лимит работы автопилота
		qp->setBrush( QBrush( Qt::red, Qt::NoBrush ) );
		qp->setPen( joy_pen );
		qp->drawRect( joystick.x() - l, joystick.y() - l, l * 2, l * 2 );

		// рукоять, педали и газ с автопилотом
		qp->setBrush( QBrush( Qt::NoBrush ) );
		qp->setPen( joy_pen );
		qp->drawEllipse( joystickA, r, r );

		qp->setPen( pedal_pen );
		qp->drawEllipse( pedalsA, r, r );

		qp->setPen( gas_pen );
		qp->drawEllipse( gas_pitchA, r, r );
	}
	qp->restore( );
}

void ControlGrid::setFrame( int width )
{
	frame = QRect( 0, 0, width, width );
	scale = double( width ) / 255;
	w = frame.width( );
	c = w / 2;
	r = w / 50;
	l = w / 10;
	setFixedSize( w, w );
}

void ControlGrid::setJoystick( int x, int y, int xa, int ya )
{
	joystick.setX( x * scale );
	joystick.setY( y * scale );
	joystickA.setX( xa * scale );
	joystickA.setY( ya * scale );
}

void ControlGrid::setPedals( int x, int xa )
{
	pedals.setX( x * scale );
	pedals.setY( r / 2 );
	pedalsA.setX( xa * scale );
	pedalsA.setY( r / 2 );
}
void ControlGrid::setGas( int y, int ya )
{
	gas_pitch.setX( r / 2 );
	gas_pitch.setY( y * scale );
	gas_pitchA.setX( r / 2 );
	gas_pitchA.setY( ya * scale );
}
