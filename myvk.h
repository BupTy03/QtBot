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
    MyVK(const QString& user_id, const QString& access_token, QObject* parent = nullptr);

    QString getUserID();
    QString getAccessToken();

    void sendMsgToUserQuery(const QString& userId, const QString& text);
    void sendMsgToGroupQuery(const QString& groupId, const QString& text);
    void getGroupsQuery(const QString& userId);

    const std::vector<std::pair<QString, QString>>& getGroupsResponse();

private:
    QString user_id;
    QString access_token;
    std::vector<std::pair<QString, QString>> groupsResponse;

signals:
    void signalGroupsLoaded();

private slots:
    void slotMessageToUserSent(QNetworkReply *reply);
    void slotMessageToGroupSent(QNetworkReply *reply);
    void slotGotGroups(QNetworkReply *reply);
};

#endif // MYVK_H
