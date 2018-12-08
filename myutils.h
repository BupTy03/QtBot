#pragma once
#ifndef MYUTILS_H
#define MYUTILS_H

#include <QFile>
#include <QTextStream>

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
