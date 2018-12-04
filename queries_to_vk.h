#pragma once
#ifndef QUERIES_TO_VK_H
#define QUERIES_TO_VK_H

#include <QString>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QMap>

#define DEBUG

namespace vk_query
{
    QJsonDocument get_request(const QNetworkRequest& request, int remaining_time = 10000);
    QJsonDocument custom_get(QUrl url, const QMap<QString, QString>& params, int wait_time = 10000);
    QJsonDocument get_user_name(const QString& user_id, const QString& access_token, int wait_time = 10000);
    QJsonDocument groups_get(const QString& access_token, const QString& user_id, int wait_time = 10000);
    QJsonDocument messages_send_to_user(const QString& access_token, const QString& user_id, const QString& message, int wait_time = 10000);
    QJsonDocument wall_post_to_group(const QString& access_token, const QString& group_id, const QString& message, int wait_time = 10000);
    QJsonDocument photos_get_wall_upload_server(const QString& access_token, const QString& group_id, int wait_time = 10000);
    QJsonDocument photos_save_wall_photo(const QString& access_token, const QString& group_id, const QString& photo, const QString& server, const QString& hash, int wait_time = 10000);
}

#endif // QUERIES_TO_VK_H
