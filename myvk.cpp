#include "myvk.h"

MyVK::MyVK(const QString& user_id, const QString& access_token, QObject* parent)
    : QObject(parent), user_id(user_id), access_token(access_token){}

void MyVK::sendMsgToUser(const QString& userId, const QString& text)
{
    qDebug() << "=============Sending message to User with ID: " << userId << " =====================";

    QUrl request("https://api.vk.com/method/messages.send");

    QUrlQuery query(request);
    query.addQueryItem("user_id", userId);
    query.addQueryItem("peer_id", userId);
    query.addQueryItem("message", text);
    query.addQueryItem("access_token", access_token);
    query.addQueryItem("test_mode", "1");
    query.addQueryItem("v", "5.52");


    qDebug() << "Query:" << query.toString();
    request.setQuery(query);
    qDebug() << "Final request: " << request.toString();

    QNetworkAccessManager* netMgr = new QNetworkAccessManager(this);

    connect(netMgr, SIGNAL(finished(QNetworkReply*)), this, SLOT(MessageSent(QNetworkReply*)));

    netMgr->get(QNetworkRequest(request));

    qDebug() << QSslSocket::sslLibraryVersionString();

    qDebug() << "=================Message was sent===================";
}

void MyVK::MessageSent(QNetworkReply* reply)
{
    qDebug() << QString(reply->readAll());
}
