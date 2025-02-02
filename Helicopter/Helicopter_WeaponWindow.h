#pragma once

#include <QWidget>
#include <QCloseEvent>
#include <QTimer>

class Helicopter_core;

namespace Ui 
{ 
	class Helicopter_WeaponWindow; 
};

class Helicopter_WeaponWindow : public QWidget
{
	Q_OBJECT

public:
	explicit Helicopter_WeaponWindow(Helicopter_core* Helicopter, QWidget *parent = Q_NULLPTR);
	~Helicopter_WeaponWindow();
	QTimer* UpdateTimer;
	void SetState(bool flag, QWidget* widget, const QString stype_true, const QString stype_false);

private:
	Helicopter_core* Helicopter;
	void closeEvent(QCloseEvent* event);

private slots:
	void on_ParamsUpdate();

private:
	Ui::Helicopter_WeaponWindow *ui;
};
