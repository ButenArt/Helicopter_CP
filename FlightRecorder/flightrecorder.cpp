#include "flightrecorder.h"
#include "checkbox.h"
#include "scales.h"
#include "plots.h"

FlightRecorder::FlightRecorder( FParam* Fpar, IParam* Ipar, QWidget* parent, quint16 /*width*/, quint16 /*height*/ )
	: QWidget( parent )
{
	ini = new QSettings( "fr_config.ini", QSettings::IniFormat );
	read_config( );
	this->setWindowFlags( Qt::Window | Qt::WindowCloseButtonHint );
	this->move( start_x, start_y );
	this->setFixedSize( size_x, size_y );
	// переменные size_x size_y читаеются из конфига, если прочитаны 0 то открываем на полный экран
	this->resize( size_x, size_y );
	fpar = Fpar;
	ipar = Ipar;
	w = size_x;
	h = size_y;

	layout = new QHBoxLayout( this );
	layout->setSpacing( spacing );
	layout->setContentsMargins( spacing, spacing, 0, 0 );
	setLayout( layout );

	checkbox = new CheckBox( this );
	checkbox->setFixedSize( w * 0.1, h );
	layout->addWidget( checkbox );

	plots = new Plots( this );
	plots->setFixedSize( w * 0.75, h );
	layout->addWidget( plots );

	scales = new Scales( this );
	scales->setFixedSize( w * 0.15, h );
	layout->addWidget( scales );

	timer = new QTimer( this );
	connect( timer, SIGNAL( timeout( ) ), this, SLOT( update( ) ) );

	checkbox->initCheckboxGeometry( );
	scales->initScalesGeometry( );
	plots->initPlotsGeometry( );

	this->show( );
	this->setChecked( );
	checkbox->show( );
	plots->show( );
	scales->show( );

}

FlightRecorder::~FlightRecorder( )
{
	stop( );

	for ( auto p : params_vector )
		delete p;
	params_vector.clear( );

	delete ini;
	delete plots;
	delete scales;
	delete checkbox;
	delete layout;
}

bool FlightRecorder::getCheckBox( int id ) const
{
	return checkbox->checkbox_vector[id]->isChecked( );
}

int* FlightRecorder::getMoveControls( ) const
{
	static int arr[] = { c_move_x, c_move_y };
	return arr;
}

const FlightRecorder::Param* FlightRecorder::getParam( int id )
{
	return params_vector[id];
}

void FlightRecorder::setSubPlots( const unsigned int sp )
{
	sub_plots = sp;

	scales->sections.resize( sp );
	plots->sections.resize( sp );

	checkbox->initCheckboxGeometry( );
	scales->initScalesGeometry( );
	plots->initPlotsGeometry( );
}

void FlightRecorder::read_config( )
{
	size_x = 0;
	size_y = 0;

	ini->beginGroup( "POSITION" );
	{
		start_x = ini->value( "move_x" ).toInt( );
		start_y = ini->value( "move_y" ).toInt( );

		size_x = ini->value( "size_x", 1900 ).toInt( );
		size_y = ini->value( "size_y", 1000 ).toInt( );

		c_move_x = ini->value( "controls_move_x" ).toInt( );
		c_move_y = ini->value( "controls_move_y" ).toInt( );
	}
	ini->endGroup( );

	ini->beginGroup( "GEOMETRY" );
	{
		dx = ini->value( "dx" ).toFloat( );
	}
	ini->endGroup( );
}


void FlightRecorder::addParam(
	const QString& /*name*/,
	const QString& abbreviation,
	float* param_ptr,
	const quint16 section,
	const float min,
	const float max,
	const bool rad_to_deg,
	QColor color
)
{
	int sections_check = 0;
	for ( auto param : params_vector )
		if ( param->section == section ) { sections_check++; }

	if ( sections_check < 3 )
	{
		NParams++;

		if ( color == Qt::white )
		{
			srand( time( NULL ) + rand( ) );
			int r = rand( ) / 255, g = rand( ) / 255, b = rand( ) / 255;
			color = QColor( r, g, b );
		}

		Param* newParam = new Param;
		newParam->name = abbreviation; // name; // в окне слева пишет краткое название
		newParam->abbreviation = abbreviation;
		newParam->param_ptr = param_ptr;
		newParam->section = section;
		newParam->color = color;
		newParam->min = min;
		newParam->max = max;
		newParam->rad_to_deg = rad_to_deg;

		float d = static_cast<float>( max - min ) / 10;
		for ( int i = 0; i < 11; i++ )
			newParam->divisions[i] = max - d * i;

		params_vector.push_back( newParam );
		checkbox->addParam( );
		plots->addNew( );
	} else
	{
		QMessageBox::warning( this, "Warning", "Максимум 3 графика в одной секции" );
	}
}

void FlightRecorder::deleteParam( int id )
{
	NParams--;
	params_vector.remove( id );
	checkbox->deleteParam( id );
}

void FlightRecorder::closeEvent( QCloseEvent* event )
{
	event->ignore( );
	this->stop( );
	this->hide( );
}
