#ifndef NAVIGATIONWINDOW
#define NAVIGATIONWINDOW
#pragma once

#include <QWidget>
#include <QCloseEvent>
#include <QTimer>
#include <QSlider>
#include <initializer_list>

class Helicopter_core;

namespace Ui { class Helicopter_PanelRadio; }

class Helicopter_PanelRadio : public QWidget
{
	Q_OBJECT

public:
    explicit Helicopter_PanelRadio(Helicopter_core* Helicopter, QWidget* parent = nullptr);
	~Helicopter_PanelRadio();
    QTimer* UpdateTimer;
    void SetState(bool flag, QWidget* widget, const QString stype_true, const QString stype_false);
private:
    Helicopter_core* Helicopter;
    void closeEvent(QCloseEvent* event);
    void showEvent(QShowEvent* event);
    void on_t_contrButton_pressed();
    void SetNavWindow();
    template<typename... Args>
    void SetSlider(QSlider* slider, Args... args);
    template<typename... Args>
    void CheckSlider(QSlider* slider, Args... args);
private slots:
    void on_ParamsUpdate();
private:
	Ui::Helicopter_PanelRadio* ui;
};

#endif 