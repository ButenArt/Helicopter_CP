#include "SystemTray.h"
#include <QMenu>
#include <QSystemTrayIcon>

SystemTray::SystemTray(QObject *parent) : QObject(parent)
{
    QMenu *trayIconMenu = new QMenu();

    QAction *viewWindow = new QAction(trUtf8("Развернуть окно"), this);
    QAction *quitAction = new QAction(trUtf8("Выход"), this);

    connect(viewWindow, &QAction::triggered, this, &SystemTray::signalShow);
    connect(quitAction, &QAction::triggered, this, &SystemTray::signalQuit);

    trayIconMenu->addAction(viewWindow);
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon();
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setIcon(QIcon(":/resource/logo-aes.png"));
    trayIcon->show();
    trayIcon->setToolTip("Server");

    // Также подключаем сигнал нажатия на иконку к обработчику данного нажатия
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
}

// Метод, который обрабатывает нажатие на иконку приложения в трее
void SystemTray::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
        case QSystemTrayIcon::Trigger:
            // В случае сигнала нажатия на иконку трея
            emit signalIconActivated();
        break;
        default:
        break;
    }
}

void SystemTray::hideIconTray()
{
    trayIcon->hide();
}

void SystemTray::showIconTray()
{
    trayIcon->show();
}

void SystemTray::setToolTip(QString tooltip)
{
    trayIcon->setToolTip(tooltip);
}
