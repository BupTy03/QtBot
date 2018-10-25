#ifndef MYVK_H
#define MYVK_H

#include <QString>
#include <QUrl>
#include <QUrlQuery>
#include <QDebug>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QDomDocument>

#include <utility>
#include <vector>

class MyVK : public QObject
{
    Q_OBJECT
public:
    explicit MyVK(const QString& user_id, const QString& access_token, QObject* parent = nullptr);

    const QString& getUserID() const;
    const QString& getAccessToken() const;

    void sendMsgToUserQuery(const QString& userId, const QString& text);
    void sendMsgToGroupQuery(const QString& groupId, const QString& text);
    void getGroupsQuery(const QString& userId);

    const QVector<QPair<QString, QString>>& getGroupsResponse() const;

private:
    QString user_id;
    QString access_token;
    QVector<QPair<QString, QString>> groupsResponse;

signals:
    void signalGroupsLoaded();

private slots:
    void slotMessageToUserSent(QNetworkReply *reply);
    void slotMessageToGroupSent(QNetworkReply *reply);
    void slotGotGroups(QNetworkReply *reply);
};

#endif // MYVK_H
