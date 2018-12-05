#pragma once
#ifndef QUERIES_TO_VK_H
#define QUERIES_TO_VK_H

#include <QString>
#include <QNetworkRequest>
#include <QHttpMultiPart>
#include <QJsonDocument>
#include <QMap>

#define DEBUG

namespace VkQuery
{
    QJsonDocument getRequest(const QNetworkRequest& request, int remaining_time = 10000);
    QJsonDocument postRequest(const QNetworkRequest& request, QHttpMultiPart *mul_part, int remaining_time = 10000);

    QJsonDocument makeGetRequest(const QUrl& url, const QMap<QString, QString>& params, int wait_time = 10000);
    QJsonDocument getUserName(const QString& user_id, const QString& access_token, int wait_time = 10000);
    QJsonDocument groupsGet(const QString& access_token, const QString& user_id, int wait_time = 10000);
    QJsonDocument messagesSendToUser(const QString& access_token, const QString& user_id, const QString& message, int wait_time = 10000);
    QJsonDocument wallPostToGroup(const QString& access_token, const QString& group_id, const QString& message, int wait_time = 10000);
    QJsonDocument photosGetWallUploadServer(const QString& access_token, const QString& group_id, int wait_time = 10000);
    QJsonDocument photosSaveWallPhoto(const QString& access_token, const QString& group_id, const QString& photo, const QString& server, const QString& hash, int wait_time = 10000);
}

#endif // QUERIES_TO_VK_H
