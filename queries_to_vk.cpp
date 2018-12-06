#include "queries_to_vk.h"

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
#include <QScopedPointer>

#include <cmath>
#include <memory>
#include <exception>

QJsonDocument VkQuery::getRequest(const QNetworkRequest& request, int remaining_time)
{
    if(!(request.url()).isValid())
    {
        throw std::runtime_error{"in function VkQuery::getRequest(): invalid url"};
    }

    auto manager = std::make_unique<QNetworkAccessManager>();
    auto loop = std::make_unique<QEventLoop>();
    auto timer = std::make_unique<QTimer>();

    QObject::connect(manager.get(), &QNetworkAccessManager::finished, loop.get(), &QEventLoop::quit);
    QObject::connect(timer.get(), &QTimer::timeout, loop.get(), &QEventLoop::quit);

    timer->start(abs(remaining_time));
    std::unique_ptr<QNetworkReply> reply(manager->get(request));
    loop->exec();

    if(reply->error() != QNetworkReply::NoError)
    {
        throw std::runtime_error{"in function VkQuery::getRequest(): network error\n"};
    }

    auto result = QJsonDocument::fromJson(reply->readAll());
    checkForErrors(result, "VkQuery::getRequest()");
    return result;
}

QJsonDocument VkQuery::postRequest(const QNetworkRequest& request, QHttpMultiPart* mul_part, int remaining_time)
{
    if(!(request.url()).isValid())
    {
        throw std::runtime_error{"in function VkQuery::postRequest(): invalid url"};
    }

    auto manager = std::make_unique<QNetworkAccessManager>();
    auto loop = std::make_unique<QEventLoop>();
    auto timer = std::make_unique<QTimer>();

    QObject::connect(manager.get(), &QNetworkAccessManager::finished, loop.get(), &QEventLoop::quit);
    QObject::connect(timer.get(), &QTimer::timeout, loop.get(), &QEventLoop::quit);

    timer->start(abs(remaining_time));
    std::unique_ptr<QNetworkReply> reply(manager->post(request, mul_part));
    loop->exec();

    if(reply->error() != QNetworkReply::NoError)
    {
        throw std::runtime_error{"in function VkQuery::postRequest(): network error"};
    }

    auto result = QJsonDocument::fromJson(reply->readAll());
    checkForErrors(result, "VkQuery::getRequest()");
    return result;
}

QJsonDocument VkQuery::groupsGet(const QString& access_token, const QString& user_id)
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
    return getRequest(QNetworkRequest(request));
}

QJsonDocument VkQuery::getUserName(const QString& user_id, const QString& access_token)
{
    QUrl request("https://api.vk.com/method/users.get");

    QUrlQuery query(request);
    query.addQueryItem("user_id", user_id);
    query.addQueryItem("access_token", access_token);
    query.addQueryItem("v", "5.52");

    request.setQuery(query);

#ifdef DEBUG
    qDebug() << "Final request: " << request.toString();
#endif

    return getRequest(QNetworkRequest(request));
}

QJsonDocument VkQuery::wallPostToGroup(const QString& access_token, const QString& group_id, const QString& message)
{
#ifdef DEBUG
    qDebug() << "\n=============Sending post to Group with ID: " << group_id << " =====================";
#endif

    QUrl request("https://api.vk.com/method/wall.post");

    QUrlQuery query(request);
    query.addQueryItem("owner_id", QString("-").append(group_id));
    query.addQueryItem("friends_only", "1");
    query.addQueryItem("message", message);
    query.addQueryItem("access_token", access_token);
    query.addQueryItem("test_mode", "1");
    query.addQueryItem("v", "5.52");

#ifdef DEBUG
    qDebug() << "Query: " << "https://api.vk.com/method/wall.post?" << query.toString();
#endif

    request.setQuery(query);

    return getRequest(QNetworkRequest(request));
}

QJsonDocument VkQuery::wallPostToGroup(const QString& access_token, const QString& group_id, const QString& message, const QString& attachments)
{
#ifdef DEBUG
    qDebug() << "\n=============Sending post to Group with ID: " << group_id << " =====================";
#endif

    QUrl request("https://api.vk.com/method/wall.post");

    QUrlQuery query(request);
    query.addQueryItem("owner_id", QString("-").append(group_id));
    query.addQueryItem("friends_only", "1");
    query.addQueryItem("message", message);
    query.addQueryItem("attachments", attachments);
    query.addQueryItem("access_token", access_token);
    query.addQueryItem("test_mode", "1");
    query.addQueryItem("v", "5.52");

#ifdef DEBUG
    qDebug() << "Query: " << "https://api.vk.com/method/wall.post?" << query.toString();
#endif

    request.setQuery(query);

    return getRequest(QNetworkRequest(request));
}

QJsonDocument VkQuery::photosGetWallUploadServer(const QString& access_token, const QString& group_id)
{
    QUrl request("https://api.vk.com/method/photos.getWallUploadServer");

    QUrlQuery query(request);
    query.addQueryItem("access_token", access_token);
    query.addQueryItem("group_id", group_id);
    query.addQueryItem("v", "5.52");

    request.setQuery(query);

#ifdef DEBUG
    qDebug() << "Final request: " << request.toString();
#endif

    return getRequest(QNetworkRequest(request));
}

QJsonDocument VkQuery::photosSaveWallPhoto(const QString& access_token, const QString& group_id, const QString& server, const QString& photo, const QString& hash)
{
    QUrl request("https://api.vk.com/method/photos.saveWallPhoto");

    QUrlQuery query(request);

    query.addQueryItem("access_token", access_token);
    query.addQueryItem("group_id", group_id);
    query.addQueryItem("photo", photo);
    query.addQueryItem("server", server);
    query.addQueryItem("hash", hash);
    query.addQueryItem("v", "5.52");

    request.setQuery(query);

#ifdef DEBUG
    qDebug() << "Final request: " << request.toString();
#endif

    return getRequest(QNetworkRequest(request));
}

void VkQuery::checkForErrors(const QJsonDocument& doc, const QString& name_func)
{
    QString error_str("in function ");
    error_str.append(name_func);
    error_str.append(": ");

    if(doc.isEmpty())
    {
        error_str.append("no response from server");
        throw std::runtime_error(error_str.toStdString());
    }

    QJsonObject error = doc["error"].toObject();
    if(error.isEmpty())
    {
        return;
    }

    error_str.append("\n\terror_code from VK.Api: ");
    error_str.append(QString::number(error["error_code"].toInt()));
    error_str.append("\n\terror_msg from VK.Api: ");
    error_str.append(error["error_msg"].toString());
    error_str.append("\n");

    throw std::runtime_error(error_str.toStdString());
}
