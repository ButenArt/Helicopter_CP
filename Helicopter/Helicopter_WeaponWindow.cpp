#include "Helicopter_WeaponWindow.h"
#include "ui_Helicopter_WeaponWindow.h"
#include "Helicopter_core.h"
#include "Helicopter_BoardSystem.h"

Helicopter_WeaponWindow::Helicopter_WeaponWindow(Helicopter_core* Helicopter, QWidget *parent)
	: QWidget(parent)
	, Helicopter(Helicopter)
	, ui(new Ui::Helicopter_WeaponWindow)
{
	ui->setupUi(this);
	this->setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint);
	this->setFixedSize(this->width(), this->height());
	this->setWindowTitle("Weapon");
	UpdateTimer = new QTimer(this);
	UpdateTimer->setInterval(500);
	connect(UpdateTimer, SIGNAL(timeout()), this, SLOT(on_ParamsUpdate()));

	connect( ui->PKT_vert, QOverload<int>::of( &QSlider::valueChanged ),
			 [=] ( int i ) { Helicopter->PC.pkt_y = i; } );
	connect( ui->PKT_horizont, QOverload<int>::of( &QSlider::valueChanged ),
			 [=] ( int i ) { Helicopter->PC.pkt_x = i; } );
	connect( ui->pkt_fire, &QPushButton::pressed,
			 [=] ( ) { Helicopter->PC.pkt_fire = true; } );
	connect( ui->pkt_fire, &QPushButton::released ,
			 [=] ( ) { Helicopter->PC.pkt_fire = false; } );

	connect( ui->t_safety_lock, &QCheckBox::stateChanged,
			 [=] ( int i ) { Helicopter->PC.t_safety_lock = i; } );
	connect( ui->t_pus_load, &QCheckBox::stateChanged,
			 [=] ( int i ) { Helicopter->WeaponPult.t_pus_load = i; } );
	connect( ui->t_main_switch, &QCheckBox::stateChanged,
			 [=] ( int i ) { Helicopter->WeaponPult.t_main_switch = i; } );
	connect( ui->t_vzriv, &QCheckBox::stateChanged,
			 [=] ( int i ) { Helicopter->WeaponPult.t_vzriv = i; } );
	connect( ui->t_sbros, &QCheckBox::stateChanged,
			 [=] ( int i ) { Helicopter->WeaponPult.t_sbros = i; } );

	connect( ui->sw_weapon, QOverload<int>::of( &QSlider::valueChanged ),
			 [=] ( int i ) {
				 Helicopter->WeaponPult.t_FAB = false;     // бомбы
				 Helicopter->WeaponPult.t_NAR = false;     // неуправляемые ракеты
				 Helicopter->WeaponPult.t_UPK23 = false;   // пушки 23х2
				 Helicopter->WeaponPult.t_PKT = false;     // РПК

				 Helicopter->WeaponPult.sw_weapon_type_ = i;
				 switch ( i )
				 {
				 case 0:
					 Helicopter->WeaponPult.t_FAB = true;
					 break;
				 case 1:
					 Helicopter->WeaponPult.t_NAR = true;
					 break;
				 case 2:
					 Helicopter->WeaponPult.t_UPK23 = true;
					 break;
				 case 3:
					 Helicopter->WeaponPult.t_PKT = true;
					 break;
				 default:
					 break;
				 }
			 } );
	connect( ui->sw_burst, QOverload<int>::of( &QSlider::valueChanged ),
			 [=] ( int i ) {
				 Helicopter->WeaponPult.t_fire_short = false;
				 Helicopter->WeaponPult.t_fire_long  = false;
				 Helicopter->WeaponPult.t_fire_midle = false;
				 switch ( i )
				 {
				 case 1:
					 Helicopter->WeaponPult.t_fire_short = true;
					 break;
				 case 2:
					 Helicopter->WeaponPult.t_fire_long = true;
					 break;
				 case 3:
					 Helicopter->WeaponPult.t_fire_midle = true;
					 break;
				 default:
					 break;
				 }
			 } );

	connect( ui->sw_pos, QOverload<int>::of( &QSlider::valueChanged ),
			 [=] ( int i ) {
				 Helicopter->WeaponPult.t_pod_ext   = false;
				 Helicopter->WeaponPult.t_pod_all   = false;
				 Helicopter->WeaponPult.t_pod_inter = false;
				 switch ( i )
				 {
				 case 0:
					 Helicopter->WeaponPult.t_pod_ext = true;
					 break;
				 case 1:
					 Helicopter->WeaponPult.t_pod_all = true;
					 break;
				 case 2:
					 break;
				 default:
					 break;
				 }
			 } );

}

Helicopter_WeaponWindow::~Helicopter_WeaponWindow()
{
	delete ui;
}

void Helicopter_WeaponWindow::SetState(bool flag, QWidget* widget, const QString style_true, const QString style_false) 
{
	flag ? widget->setStyleSheet(style_true) : widget->setStyleSheet(style_false);
}

void Helicopter_WeaponWindow::on_ParamsUpdate()
{
	SetState( Helicopter->WeaponPult.i_bd_1_load, ui->bd1, "background:yellow", "background:white" );
	SetState( Helicopter->WeaponPult.i_bd_2_load, ui->bd2, "background:yellow", "background:white" );
	SetState( Helicopter->WeaponPult.i_bd_3_load, ui->bd3, "background:yellow", "background:white" );
	SetState( Helicopter->WeaponPult.i_bd_4_load, ui->bd4, "background:yellow", "background:white" );
	SetState( Helicopter->WeaponPult.i_bd_5_load, ui->bd5, "background:yellow", "background:white" );
	SetState( Helicopter->WeaponPult.i_bd_6_load, ui->bd6, "background:yellow", "background:white" );
	SetState( Helicopter->WeaponPult.i_pus_bd_1_6, ui->pus16, "background:yellow", "background:white" );
	SetState( Helicopter->WeaponPult.i_pus_bd_2_5, ui->pus25, "background:yellow", "background:white" );
	SetState( Helicopter->PC.i_weapon_warning, ui->i_weapon_warning, "background:yellow", "background:white" );
}

void Helicopter_WeaponWindow::closeEvent(QCloseEvent* event)
{
	event->ignore();
	UpdateTimer->stop();
	this->hide();
}
