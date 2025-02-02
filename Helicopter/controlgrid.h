#ifndef CONTROLGRID_H
#define CONTROLGRID_H

#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QTimer>


class ControlGrid: public QWidget
{
    Q_OBJECT

public:
    ControlGrid(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event) override;

    void setFrame(int);                     // размер грида
    void setJoystick(int, int, int, int);   // координаты ручки
    void setPedals(int, int);               // координаты педалей
    void setGas(int, int);                  // координаты шага газа

private:
    void draw(QPainter*);

    QRect frame;
    QPointF joystick;
    QPointF joystickA;
    QPointF pedals;
    QPointF pedalsA;
    QPointF gas_pitch;
    QPointF gas_pitchA;

    double scale;
    int w;  // ширина виджета
    int c;  // середина виджета
    int r;  // радиус точек и смещение от краев виджета
    qreal l;  // лимит работы автопилота радиус вокруг ручки
private slots:

};

//  Usage

//  ControlGrid cg1;
//  cg1.setFrame(c);
//  cg1.show();

//  cg1.setJoystick(127, 127, 135, 140);
//  cg1.setPedals(127, 140);
//  cg1.setGas(127, 140);

#endif // CONTROLGRID_H
