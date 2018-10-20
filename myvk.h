#ifndef MYVK_H
#define MYVK_H

#include <QString>
#include <QUrl>
#include <QUrlQuery>
#include <QDebug>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QNetworkRequest>

class MyVK : public QObject
{
    Q_OBJECT
public:
    MyVK(const QString& user_id, const QString& access_token, QObject* parent = nullptr);
    void sendMsgToUser(const QString& userId, const QString& text);

private:
    QString user_id;
    QString access_token;
    QNetworkReply* net_reply;

private slots:
    void MessageSent(QNetworkReply *reply);

};

#endif // MYVK_H
