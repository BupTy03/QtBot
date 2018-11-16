#pragma once
#ifndef VKAUTH_H
#define VKAUTH_H

#include <QObject>
#include <QString>
#include <QtWebEngineWidgets>

#define DEBUG

class VKAuth : public QObject
{
    Q_OBJECT
public:
    explicit VKAuth(QString appID, QObject* parent = nullptr);

    void auth(QString scope);
    bool isValid();

    QString get_app_id() const {return appID_;}
    QString get_user_id() const {return userID_;}
    QString get_access_token() const {return accessToken_;}

signals:
    void done(bool);

private slots:
    void checkAuth(const QUrl& url);

private:
    QWebEngineView browser_;
    QString appID_;
    QString accessToken_;
    QString userID_;
};

#endif // VKAUTH_H
