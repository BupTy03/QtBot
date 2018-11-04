#pragma once

#ifndef VKQUERY_H
#define VKQUERY_H

#define DEBUG

#include <QObject>
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QEventLoop>
#include <QUrlQuery>
#include <QUrl>
#include <QTimer>

#ifdef DEBUG
#include <QDebug>
#endif

#include <memory>

#include "vkauth.h"

class VKQuery : public QObject
{
    Q_OBJECT
public:
    explicit VKQuery(QObject* parent = nullptr);

    void get_groups(const VKAuth& vk);

signals:
    void done(std::shared_ptr<QJsonDocument>);
};


#endif // VKQUERY_H
