#pragma once
#ifndef QUERIES_TO_VK_H
#define QUERIES_TO_VK_H

#include <QString>
#include <QNetworkRequest>
#include <QHttpMultiPart>
#include <QJsonDocument>

//#define DEBUG

namespace VkQuery
{
    QJsonDocument getRequest(const QNetworkRequest& request, int remaining_time = 10000);
    QJsonDocument postRequest(const QNetworkRequest& request, QHttpMultiPart* mul_part, int remaining_time = 10000);

    QJsonDocument getUserName(const QString& user_id, const QString& access_token);
    QJsonDocument groupsGet(const QString& access_token, const QString& user_id);

    QJsonDocument wallPostToGroup(const QString& access_token,
                                  const QString& group_id,
                                  const QString& message);

    QJsonDocument wallPostToGroup(const QString& access_token,
                                  const QString& group_id,
                                  const QString& message,
                                  const QString& attachments);

    QJsonDocument photosGetWallUploadServer(const QString& access_token, const QString& group_id);

    QJsonDocument photosSaveWallPhoto(const QString& access_token,
                                      const QString& group_id,
                                      const QString& server,
                                      const QString& photo,
                                      const QString& hash);

    void checkForErrors(const QJsonDocument& doc, const QString& name_func);
}

#endif // QUERIES_TO_VK_H
