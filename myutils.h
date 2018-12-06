#pragma once
#ifndef MYUTILS_H
#define MYUTILS_H

#include <QSharedPointer>
#include <QTextStream>
#include <QFile>

template<typename T, typename... Ts>
QSharedPointer<T> makeQSharedPointer(Ts&&... params)
{
    return QSharedPointer<T>(new T(std::forward<Ts>(params)...));
}

class Log
{
public:
    static void toFile(const QString& message)
    {
        QFile file("log.txt");
        if (file.open(QIODevice::Append | QIODevice::Text))
        {
            QTextStream out(&file);
            out << "Error " << message;
            file.close();
        }
    }
};

#endif // MYUTILS_H
