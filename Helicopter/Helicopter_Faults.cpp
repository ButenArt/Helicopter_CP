#include "Helicopter_Faults.h"
#include "ui_Helicopter_Faults.h"
#include "Helicopter_core.h"
#include "Helicopter_instructor.h"
#include "Helicopter_ServiceErrors.h"
#include <QDebug>

Helicopter_Faults::Helicopter_Faults(Helicopter_core* Helicopter, QWidget *parent)
	: QWidget(parent)
	, Helicopter(Helicopter)
	, ui(new Ui::Helicopter_Faults)
{
	ui->setupUi(this);
	this->setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint);
	this->setFixedSize(this->width(), this->height());
	this->setWindowTitle("Faults");
}

Helicopter_Faults::~Helicopter_Faults()
{
	delete ui;
}

void Helicopter_Faults::on_Ok_clicked()
{
	Helicopter->instructor->fault.Fire = ui->Fault_Fire->isChecked();
	Helicopter->instructor->fault.Engine1 = ui->Fault_Engine1->isChecked();
	Helicopter->instructor->fault.Engine2 = ui->Fault_Engine2->isChecked();
	Helicopter->instructor->fault.Gear = ui->Fault_Gear->isChecked();
	Helicopter->instructor->fault.Gen1 = ui->Fault_Gen1->isChecked();
	Helicopter->instructor->fault.Gen2 = ui->Fault_Gen2->isChecked();
	Helicopter->instructor->fault.VU1 = ui->Fault_VU1->isChecked();
	Helicopter->instructor->fault.VU2 = ui->Fault_VU2->isChecked();
	Helicopter->instructor->fault.RV_contrl = ui->Fault_RV_contrl->isChecked();
	Helicopter->instructor->fault.RV_gear = ui->Fault_RV_gear->isChecked();
	Helicopter->instructor->fault.HidroMain = ui->Fault_HidroMain->isChecked();
	Helicopter->instructor->fault.HidroSecond = ui->Fault_HidroSecond->isChecked();
	Helicopter->instructor->fault.HidroDiafrag = ui->Fault_HidroDiafrag->isChecked();
	Helicopter->instructor->fault.Vibration_Max_left = ui->Fault_Vibration_Max_left->isChecked();
	Helicopter->instructor->fault.Vibration_left = ui->Fault_Vibration_left->isChecked();
	Helicopter->instructor->fault.Vibration_Max_right = ui->Fault_Vibration_Max_right->isChecked();
	Helicopter->instructor->fault.Vibration_right = ui->Fault_Vibration_right->isChecked();
	Helicopter->instructor->fault.Autopilot = ui->Fault_Autopilot->isChecked();
	Helicopter->instructor->fault.AviaHorizont = ui->Fault_AviaHorizont->isChecked();
	Helicopter->instructor->fault.Curs = ui->Fault_Curs->isChecked();
	Helicopter->instructor->fault.Barometer = ui->Fault_Barometer->isChecked();
	Helicopter->instructor->fault.Speed = ui->Fault_Speed->isChecked();
	Helicopter->instructor->fault.ARK = ui->Fault_ARK->isChecked();
	Helicopter->instructor->fault.BKK18 = ui->Fault_BKK18->isChecked();
}

void Helicopter_Faults::on_Cancel_clicked()
{
	ui->Fault_Fire->setChecked(false);
	ui->Fault_Engine1->setChecked(false);
	ui->Fault_Engine2->setChecked(false);
	ui->Fault_Gear->setChecked(false);
	ui->Fault_Gen1->setChecked(false);
	ui->Fault_Gen2->setChecked(false);
	ui->Fault_VU1->setChecked(false);
	ui->Fault_VU2->setChecked(false);
	ui->Fault_RV_contrl->setChecked(false);
	ui->Fault_RV_gear->setChecked(false);
	ui->Fault_HidroMain->setChecked(false);
	ui->Fault_HidroSecond->setChecked(false);
	ui->Fault_HidroDiafrag->setChecked(false);
	ui->Fault_Vibration_left->setChecked(false);
	ui->Fault_Vibration_right->setChecked(false);
	ui->Fault_Vibration_Max_left->setChecked(false);
	ui->Fault_Vibration_Max_right->setChecked(false);
	ui->Fault_Autopilot->setChecked(false);
	ui->Fault_AviaHorizont->setChecked(false);
	ui->Fault_Curs->setChecked(false);
	ui->Fault_Barometer->setChecked(false);
	ui->Fault_Speed->setChecked(false);
	ui->Fault_ARK->setChecked(false);
	ui->Fault_BKK18->setChecked(false);
}
