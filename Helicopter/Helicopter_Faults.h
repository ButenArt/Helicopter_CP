#pragma once

#include <QWidget>

class Helicopter_core;

namespace Ui { class Helicopter_Faults; };

class Helicopter_Faults : public QWidget
{
	Q_OBJECT
private:
	Helicopter_core* Helicopter;
public:
	Helicopter_Faults(Helicopter_core* Helicopter, QWidget *parent = Q_NULLPTR);
	~Helicopter_Faults();
private slots:
	void on_Ok_clicked();
	void on_Cancel_clicked();

private:
	Ui::Helicopter_Faults *ui;
};
