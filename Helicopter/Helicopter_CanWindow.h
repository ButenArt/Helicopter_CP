#pragma once

#include <QWidget>
#include <QCloseEvent>
#include <QTimer>
#include "Helicopter_core.h"

class Helicopter_controls;

namespace Ui
{
    class Helicopter_CANWindow;
}

class Helicopter_CANWindow : public QWidget
{
    Q_OBJECT

public:
    explicit Helicopter_CANWindow(QWidget *parent = 0, QSharedPointer<Helicopter_controls> controls = 0);
    ~Helicopter_CANWindow();
    QTimer *UpdateTimer;
private:
    QSharedPointer<Helicopter_controls> _controls;
    void closeEvent(QCloseEvent *event);
private slots:
    void slotParamsUpdate();
private:
    Ui::Helicopter_CANWindow* ui;
};
