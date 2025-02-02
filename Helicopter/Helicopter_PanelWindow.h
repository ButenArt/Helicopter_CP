#ifndef PILOTWINDOW
#define PILOTWINDOW
#pragma once

#include <QWidget>
#include <QCloseEvent>
#include <QTimer>
#include <QSlider>
#include <initializer_list>
#include "Helicopter_PanelRadio.h"

class Helicopter_core;
class ControlGrid;

namespace Ui { class Helicopter_PanelWindow; }

class Helicopter_PanelWindow : public QWidget
{
    Q_OBJECT

public:
    explicit Helicopter_PanelWindow(Helicopter_core* Helicopter, QWidget *parent = nullptr);
    ~Helicopter_PanelWindow();
    QTimer* UpdateTimer;
    void SetState(bool flag, QWidget* widget, const QString stype_true, const QString stype_false);
    void keyPressEvent(QKeyEvent* event);
private:
    Helicopter_core* Helicopter;
    ControlGrid* _ControlGrid;
    Helicopter_PanelRadio* PanelRadio_;
    void closeEvent(QCloseEvent* event);
    void showEvent(QShowEvent* event);
    void SetML2Window();
    template<typename... Args>
    void SetSlider( QSlider* slider, Args... args );
    template<typename... Args>
    void CheckSlider( QSlider* slider, Args... args );
private slots:
    void on_azs1_clicked();
    void on_azs2_clicked();
    void on_azs3_clicked();
    void on_azs4_clicked();
    void on_azs5_clicked();
    void on_azs6_clicked();
    void on_bFire_pressed( );
    void on_bFire_released( );
    void on_ParamsUpdate();
    void PitchSliderReleased();
    void RollSliderReleased();
private:
    Ui::Helicopter_PanelWindow* ui;
};

#endif 