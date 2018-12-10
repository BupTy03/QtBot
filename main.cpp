#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QDateTime>
#include <QTextStream>
#include <QDebug>
#include <QMutex>
#include <QMutexLocker>

#include <memory>
#include <stdexcept>
#include <functional>

void messageHandler(QtMsgType type,
                    const QMessageLogContext& context,
                    const QString& msg);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qInstallMessageHandler(messageHandler);

    MainWindow w;
    w.show();

    return a.exec();
}

void messageHandler(QtMsgType type,
                    const QMessageLogContext& context,
                    const QString& msg)
{
    QMutex _mutex;
    QMutexLocker _locker(&_mutex);

    std::unique_ptr<QFile, std::function<void(QFile*)>> logfile(new QFile("log.txt"),
    [](QFile* ptr_f)
    {
        if(ptr_f->isOpen())
        {
            ptr_f->close();
        }
        delete ptr_f;
    });

    if(!logfile->open(QIODevice::Append | QIODevice::Text))
    {
        return;
    }

    QTextStream out(logfile.get());
    out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz ");
    switch (type)
    {
    case QtInfoMsg:     out << "INFO "; break;
    case QtDebugMsg:    out << "DEBUG "; break;
    case QtWarningMsg:  out << "WARNING "; break;
    case QtCriticalMsg: out << "CRITICAL "; break;
    case QtFatalMsg:    out << "FATAL "; break;
    }

    out << ": " << msg << endl;
}
