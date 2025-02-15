#ifndef FLIGHTRECORDER_H
#define FLIGHTRECORDER_H

#include <QWidget>
#include <QBoxLayout>
#include <QPainter>
#include <QTimer>
#include <QDebug>
#include <QSettings>
#include <QMessageBox>
#include <QCloseEvent>

#include <ctime>
#include <cstdlib>

#include "../Helicopter/Helicopter_Model.h"

class CheckBox;
class Scales;
class Plots;
class FR_Controls;

// Общий виджет
class FlightRecorder : public QWidget
{
	Q_OBJECT

private:
	// Структура для атрибутов каждого параметра
	struct Param
	{
		QString name;
		QString abbreviation;
		float* param_ptr;
		quint16 section;
		QColor color;
		float min;
		float max;

		float divisions[11];
		bool rad_to_deg;
	};
	void closeEvent( QCloseEvent* event );
public:
	FlightRecorder( FParam* fapr, IParam* ipar, QWidget* parent = nullptr, quint16 width = 0, quint16 height = 0 );
	~FlightRecorder( );

	void setMaxPoints( const unsigned int n ) { max_points = n; }
	void setSubPlots( const unsigned int sp );
	void setChecked( ) { checked = true; }

	quint16 getWidth( )      const { return w; }
	quint16 getHeight( )     const { return h; }
	quint16 getSpacing( )    const { return spacing; }
	float   getDx( )         const { return dx; }
	quint16 getSubPlots( )   const { return sub_plots; }
	quint16 getMaxPoints( )  const { return max_points; }
	quint16 getCount( )      const { return NParams; }
	bool    getChecked( )    const { return checked; }
	bool    getCheckBox( int id ) const;

	int* getMoveControls( ) const;
	const Param* getParam( int id );

	void saveParamId( const int id ) { config_list.push_back( id ); }

	FParam* fpar;
	IParam* ipar;

	void addParam(
		const QString& name,            // Название параметра
		const QString& abbreviation,    // Аббревиатура/сокращение
		float* param_ptr,               // Указатель на параметр
		const quint16 section,          // Секция, графика (0 - sub_plots)
		const float min = 0,            // 0 - 100 для
		const float max = 100,          // процентных параметров
		const bool rad_to_deg = false,  // Перевод градусов в радианы
		QColor color = Qt::white        // Цвет графика и шкалы
	);
	void deleteParam( int id = 0 );

public slots:
	void start( const int t = 33 ) { timer->start( t ); }
	void stop( ) { timer->stop( ); }

private:
	void read_config( );
	QSettings* ini;
	QList<int> config_list;

	quint16 w, h;
	int start_x, start_y;
	int size_x, size_y;
	int c_move_x, c_move_y;

	QHBoxLayout* layout;
	QTimer* timer;

	quint16 NParams = 0;        // Количество параметров
	quint16 max_points = 100;   // Максимальное количество точек графика
	quint16 sub_plots = 1;      // Количество секций с графиками
	float dx = 0;               // Расстояние между точками по Х
	const quint16 spacing = 1;  // Отступ

	// Флаги
	bool dark_theme = false;
	bool checked = false;

	// Виджеты
	CheckBox* checkbox;
	Scales* scales;
	Plots* plots;

	QVector<Param*> params_vector;
};


#endif // FLIGHTRECORDER_H

/* Параметры
 * Высота геометрическая
 * Скорость приборная
 * Гиромагнитный курс
 * Крен
 * Тангаж
 * Вертикальная перегрузка
 * Положение РУ общего шага
 * Частота вращения НВ
 * Частота вращения ТК левого двигателя
 * Частота вращения ТК правого двигателя
 *
 */
