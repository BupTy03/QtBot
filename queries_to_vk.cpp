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

#include <cmath>

QJsonDocument vk_query::get_request(const QNetworkRequest& request, int remaining_time)
{
    QNetworkAccessManager manager;
    QEventLoop loop;
    QTimer timer;

    QObject::connect(&manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
    QObject::connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);

    timer.start(abs(remaining_time));
    QNetworkReply* reply = manager.get(request);
    loop.exec();
    return QJsonDocument::fromJson(reply->readAll());
}

QJsonDocument vk_query::groups_get(const QString& access_token, const QString& user_id, int wait_time)
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
    return get_request(QNetworkRequest(request), wait_time);
}

QJsonDocument vk_query::messages_send_to_user(const QString& access_token, const QString& user_id, const QString& message, int wait_time)
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

    return get_request(QNetworkRequest(request), wait_time);
}

QJsonDocument vk_query::wall_post_to_group(const QString& access_token, const QString& group_id, const QString& message, int wait_time)
{
#ifdef DEBUG
    qDebug() << "\n=============Sending message to Group with ID: " << group_id << " =====================";
#endif

    //QUrl request("https://api.vk.com/method/wall.post");

    QUrl request("localhost//test");

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

    return get_request(QNetworkRequest(request), wait_time);
}

QJsonDocument vk_query::get_user_name(const QString& user_id, const QString& access_token, int wait_time)
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

    return get_request(QNetworkRequest(request), wait_time);
}


QJsonDocument vk_query::photos_get_wall_upload_server(const QString& access_token, const QString& group_id, int wait_time)
{
    QUrl request("https://api.vk.com/method/photos.saveWallPhoto");

    QUrlQuery query(request);
    query.addQueryItem("access_token", access_token);
    query.addQueryItem("group_id", group_id);
    query.addQueryItem("v", "5.52");

    request.setQuery(query);

#ifdef DEBUG
    qDebug() << "Final request: " << request.toString();
#endif

    return get_request(QNetworkRequest(request), wait_time);
}

QJsonDocument vk_query::photos_save_wall_photo(const QString& access_token, const QString& group_id, const QString& photo, const QString& server, const QString& hash, int wait_time)
{
    QUrl request("https://api.vk.com/method/photos.getWallUploadServer");

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

    return get_request(QNetworkRequest(request), wait_time);
}

QJsonDocument vk_query::custom_get(QUrl url, const QMap<QString, QString>& params, int wait_time)
{
    QUrlQuery query(url);

    for(auto it = params.constBegin(); it != params.constEnd(); ++it)
    {
        query.addQueryItem(it.key(), it.value());
    }

    url.setQuery(query);

#ifdef DEBUG
    qDebug() << "Final request: " << url.toString();
#endif

    return get_request(QNetworkRequest(url), wait_time);
}
