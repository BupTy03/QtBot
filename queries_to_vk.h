#pragma once
#ifndef QUERIES_TO_VK_H
#define QUERIES_TO_VK_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QEventLoop>
#include <QNetworkReply>
#include <QUrl>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QTimer>

#define DEBUG

class vk_query
{
public:
    static QJsonDocument get_request(const QNetworkRequest& request, int remaining_time)
    {
        QNetworkAccessManager manager;
        QEventLoop loop;
        QTimer timer;

        QMetaObject::Connection mo_conn = QObject::connect(&manager, &QNetworkAccessManager::finished, [&loop](QNetworkReply* reply)
        {
            loop.exit();
        });
        QMetaObject::Connection mo_timer_conn = QObject::connect(&timer, &QTimer::timeout, [&loop]()
        {
            loop.exit();
        });

        timer.start(remaining_time);
        QNetworkReply* reply = manager.get(request);
        loop.exec();
        QObject::disconnect(mo_conn);
        QObject::disconnect(mo_timer_conn);
        return QJsonDocument::fromJson(reply->readAll());
    }
    static QJsonDocument groups_get(const QString& access_token, const QString& user_id)
    {
#ifdef DEBUG
        qDebug() << "\n==================Getting IDs of Groups of User with ID: " << user_id << " =====================";
#endif
        QUrl request("https://api.vk.com/method/groups.get");

        QUrlQuery query(request);
        query.addQueryItem("user_id", user_id);
        query.addQueryItem("count", "1000");
        query.addQueryItem("extended", "1");
        query.addQueryItem("fields", "name");
        query.addQueryItem("access_token", access_token);
        query.addQueryItem("test_mode", "1");
        query.addQueryItem("v", "5.52");
#ifdef DEBUG
        qDebug() << "Query:" << query.toString();
#endif
        request.setQuery(query);
#ifdef DEBUG
        qDebug() << "Final request: " << request.toString();
#endif
        return get_request(QNetworkRequest(request), 10000);
    }
    static QJsonDocument messages_send_to_user(const QString& access_token, const QString& user_id, const QString& message)
    {
#ifdef DEBUG
        qDebug() << "\n=============Sending message to User with ID: " << user_id << " =====================";
#endif
        QUrl request("https://api.vk.com/method/messages.send");

        QUrlQuery query(request);
        query.addQueryItem("user_id", user_id);
        query.addQueryItem("peer_id", user_id);
        query.addQueryItem("message", message);
        query.addQueryItem("access_token", access_token);
        query.addQueryItem("test_mode", "1");
        query.addQueryItem("v", "5.52");
#ifdef DEBUG
        qDebug() << "Query:" << query.toString();
#endif
        request.setQuery(query);
#ifdef DEBUG
        qDebug() << "Final request: " << request.toString();
#endif

        return get_request(QNetworkRequest(request), 10000);
    }
    static QJsonDocument messages_send_to_group(const QString& access_token, const QString& group_id, const QString& message)
    {
#ifdef DEBUG
        qDebug() << "\n=============Sending message to Group with ID: " << group_id << " =====================";
#endif

        //QUrl request("https://api.vk.com/method/messages.send");

        QUrl request("localhost//test");

        QUrlQuery query(request);
        query.addQueryItem("group_id", group_id);
        query.addQueryItem("message", message);
        query.addQueryItem("access_token", access_token);
        query.addQueryItem("test_mode", "1");
        query.addQueryItem("v", "5.52");

#ifdef DEBUG
        qDebug() << "Query:" << query.toString();
#endif

        request.setQuery(query);

#ifdef DEBUG
        qDebug() << "Final request: " << request.toString();
#endif

        return get_request(QNetworkRequest(request), 10000);
    }
};

#endif // QUERIES_TO_VK_H
