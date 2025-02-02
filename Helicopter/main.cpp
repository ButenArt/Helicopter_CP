#include "Helicopter_MainWindow.h"
#include <QApplication>
#include <QString>
#include <QLockFile>
#include <QDir>
#include <QMessageBox>
#include "main.h"
#include "Common/SettingsFile.h"

#include <qwidget.h>
#include <qplaintextedit.h>
#include <qboxlayout.h>
#include <qdebug.h>
#include <qmetaobject.h>
#include <qthread.h>
#include <qdatetime.h>

//#ifndef _DEBUG
//QWidget* DEBUG_MESSAGE_DISPLAY_WIDGET = NULL;
//QPlainTextEdit* DEBUG_MESSAGE_DISPLAY_TEXTEDIT = NULL;
//void setupDebugDisplay();
//void debugMessageDisplayFunc(QtMsgType type, const QMessageLogContext& context, const QString& msg);
//#endif

void myMessageHandler( QtMsgType type, const QMessageLogContext& context, const QString& message )
{
    static QMutex mutex;
    QMutexLocker lock( &mutex );

    static QFile logFile( "Helicopter.log" );
    if ( logFile.size( ) > 10000000 )
    {
        logFile.resize( 0 );
    }
    static bool logFileIsOpen = logFile.open( QIODevice::Append | QIODevice::Text );

    std::cerr << qPrintable( qFormatLogMessage( type, context, message ) ) << std::endl;

    if ( logFileIsOpen )
    {
        logFile.write( qFormatLogMessage( type, context, message ).toUtf8( ) + '\n' );
        logFile.flush( );
    }
}


int main(int argc, char *argv[])
{
    QStringList paths = QCoreApplication::libraryPaths();
    paths.append(".");
    paths.append("platforms");
    QCoreApplication::setLibraryPaths(paths);
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);
    QLockFile lockFile(QDir::temp().absoluteFilePath("Server.lock"));
    if(!lockFile.tryLock(100))
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText("Приложение уже запущено !!!");
        msgBox.exec();
        // TODO: тут бы сделать активацию уже запущенного приложения
        return 1;
    }
//#ifndef _DEBUG
//    setupDebugDisplay();
//#endif
#ifndef _DEBUG
    qInstallMessageHandler( myMessageHandler ); // в релизе пишем лог в файл t72.log
#endif
    TSettingsFile *settings = new TSettingsFile();
    Helicopter_MainWindow *window = new Helicopter_MainWindow;
    if(settings->WindowMinimize()) 
        window->hide(); 
    else 
        window->show();
    auto ret = a.exec();
 //   delete window;
//#ifndef _DEBUG
//    delete DEBUG_MESSAGE_DISPLAY_WIDGET;
//#endif
    return ret;
}
//#ifndef _DEBUG
//void setupDebugDisplay()
//{
//    QWidget* widget = new QWidget();
//    widget->setWindowTitle("Debug Log");
//    widget->setAttribute(Qt::WA_QuitOnClose, false); //quit only when mainwindow is closed
//    QBoxLayout* layout = new QVBoxLayout();
//    widget->setLayout(layout);
//    QPlainTextEdit* textEdit = new QPlainTextEdit(widget);
//    QFont font = QFont("Monospace");
//    font.setStyleHint(QFont::TypeWriter);
//    textEdit->setFont(font);
//    textEdit->setReadOnly(true);
//    layout->addWidget(textEdit);
//    widget->resize( 1200, 300);
//    widget->show();
//    DEBUG_MESSAGE_DISPLAY_WIDGET = widget;
//    DEBUG_MESSAGE_DISPLAY_TEXTEDIT = textEdit;
//    qInstallMessageHandler(debugMessageDisplayFunc);
//}
//void debugMessageDisplayFunc(QtMsgType type, const QMessageLogContext&, const QString& msg)
//{
//    bool do_abort = false;
//    const char* msgTypeStr = NULL;
//    switch (type) {
//    case QtDebugMsg:
//        msgTypeStr = "Debug";
//        break;
//    case QtWarningMsg:
//        msgTypeStr = "Warning";
//        break;
//    case QtCriticalMsg:
//        msgTypeStr = "Critical";
//        break;
//    case QtFatalMsg:
//        msgTypeStr = "Fatal";
//        do_abort = true;
//    default:
//        assert(0);
//        return;
//    }
//    QTime now = QTime::currentTime();
//    QString formattedMessage =
//        QString::fromLatin1("%1 %2 %3")
//        .arg(now.toString("hh:mm:ss:zzz"))
//        .arg(msgTypeStr).arg(msg);
//    // print on console:
//    fprintf(stderr, "%s\n", formattedMessage.toLocal8Bit().constData());
//    // print in debug log window
//    {
//        bool isMainThread = QThread::currentThread() == QApplication::instance()->thread();
//        if (DEBUG_MESSAGE_DISPLAY_TEXTEDIT)
//        {
//            if (isMainThread)
//                DEBUG_MESSAGE_DISPLAY_TEXTEDIT->appendPlainText(formattedMessage);
//            else // additional code, so that qDebug calls in threads will work aswell
//                QMetaObject::invokeMethod(DEBUG_MESSAGE_DISPLAY_TEXTEDIT, "appendPlainText", Qt::QueuedConnection, Q_ARG(QString, formattedMessage));
//        }
//    }
//}
//#endif
//
