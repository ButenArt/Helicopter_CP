#ifndef MAINWINDOW
#define MAINWINDOW
#pragma once
//#pragma warning( disable: 26812 )
#include <QMainWindow>
#include <QCloseEvent>
#include <QTimer>
#include "Common/SystemTray.h"
#include "Helicopter_core.h"
#include "Helicopter_CanWindow.h"
//#include "Helicopter_errors_window.h"
#include "Helicopter_PanelWindow.h"
#include "Helicopter_WeaponWindow.h"
#include "Helicopter_Faults.h"

namespace Ui
{
    class Helicopter_MainWindow;
}

class FlightRecorder;
class FR_Controls;

class Helicopter_MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit Helicopter_MainWindow(QWidget *parent = 0);
    ~Helicopter_MainWindow();
private:
    Ui::Helicopter_MainWindow *ui;
    SystemTray *SystemTray_;
    QTimer *NetStatTimer_;
    Helicopter_core* Helicopter;
    Helicopter_CANWindow* CanWindow_;//  CanWindow;
    Helicopter_PanelWindow* PilotWindow_;
    Helicopter_WeaponWindow* WeaponWindow_;
    Helicopter_Faults* FaultWindow_;
    FlightRecorder* FR_;
    FR_Controls* FR_C;
    void closeEvent(QCloseEvent *event);
    void ExStart( quint8 maptype );
    void ExStop();
private slots:
    void keyPressEvent(QKeyEvent *event);
    void slotUpdateNetStat();
    void slotPlaceToTray();
    void slotShowMainWindow();
    void slotApplicationQuit();
    void on_ExStartButton_1_clicked();
    void on_ExStartButton_2_clicked();
    void on_ExStartButton_3_clicked();
    void on_ExStartButton_4_clicked();
    void on_ExStopButton_clicked();
    void on_Save_clicked();
    void on_weather_activated(int i);
    void on_Projector_ON_clicked();
    void on_Projector_OFF_clicked();
    void on_rb8001_clicked(bool checked);
    void on_rb10000_clicked(bool checked);

signals:
    void signalPlaceToTray();
};
#endif // MAINWINDOW
