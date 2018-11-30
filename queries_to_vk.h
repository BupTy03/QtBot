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
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QTimer>

#define DEBUG

namespace vk_query
{
    QJsonDocument get_request(const QNetworkRequest& request, int remaining_time = 10000);
    QJsonDocument get_user_name(const QString& user_id, const QString& access_token, int wait_time = 10000);
    QJsonDocument groups_get(const QString& access_token, const QString& user_id, int wait_time = 10000);
    QJsonDocument messages_send_to_user(const QString& access_token, const QString& user_id, const QString& message, int wait_time = 10000);
    QJsonDocument wall_post_to_group(const QString& access_token, const QString& group_id, const QString& message, int wait_time = 10000);
}

#endif // QUERIES_TO_VK_H
