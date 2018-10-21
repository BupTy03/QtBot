#include "myvk.h"

MyVK::MyVK(const QString& user_id, const QString& access_token, QObject* parent)
    : QObject(parent), user_id(user_id), access_token(access_token){}

QString MyVK::getUserID()
{
    return user_id;
}

QString MyVK::getAccessToken()
{
    return access_token;
}

void MyVK::sendMsgToUserQuery(const QString& userId, const QString& text)
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

    connect(netMgr, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotMessageToUserSent(QNetworkReply*)));

    netMgr->get(QNetworkRequest(request));
}

void MyVK::sendMsgToGroupQuery(const QString& groupId, const QString& text)
{
    qDebug() << "=============Sending message to Group with ID: " << groupId << " =====================";
    QUrl request("https://api.vk.com/method/messages.send");

    QUrlQuery query(request);
    query.addQueryItem("group_id", groupId);
    query.addQueryItem("message", text);
    query.addQueryItem("access_token", access_token);
    query.addQueryItem("test_mode", "1");
    query.addQueryItem("v", "5.52");


    qDebug() << "Query:" << query.toString();
    request.setQuery(query);
    qDebug() << "Final request: " << request.toString();

    QNetworkAccessManager* netMgr = new QNetworkAccessManager(this);

    connect(netMgr, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotMessageToGroupSent(QNetworkReply*)));

    netMgr->get(QNetworkRequest(request));
}

void MyVK::getGroupsQuery(const QString& userId)
{
    qDebug() << "==================Getting IDs of Groups of User with ID: " << userId << " =====================";
    QUrl request("https://api.vk.com/method/groups.get.xml");

    QUrlQuery query(request);
    query.addQueryItem("user_id", userId);
    query.addQueryItem("count", "1000");
    query.addQueryItem("extended", "1");
    query.addQueryItem("fields", "name");
    query.addQueryItem("access_token", access_token);
    query.addQueryItem("test_mode", "1");
    query.addQueryItem("v", "5.52");

    qDebug() << "Query:" << query.toString();
    request.setQuery(query);
    qDebug() << "Final request: " << request.toString();

    QNetworkAccessManager* netMgr = new QNetworkAccessManager(this);

    connect(netMgr, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotGotGroups(QNetworkReply*)));

    netMgr->get(QNetworkRequest(request));
}

const std::vector<std::pair<QString, QString>>& MyVK::getGroupsResponse()
{
    return groupsResponse;
}

void MyVK::slotMessageToUserSent(QNetworkReply* reply)
{
    if(!reply)
    {
        return;
    }
    //qDebug() << QString(reply->readAll());
    qDebug() << "=================Message was sent===================";
}

void MyVK::slotMessageToGroupSent(QNetworkReply* reply)
{
    if(!reply)
    {
        return;
    }
    //qDebug() << QString(reply->readAll());
    qDebug() << "=================Message was sent===================";
}

void MyVK::slotGotGroups(QNetworkReply* reply)
{
    if(!reply)
    {
        return;
    }

    QDomDocument xml_doc;
    xml_doc.setContent(reply->readAll());

    QDomElement response = xml_doc.firstChildElement();
    QDomNode group = response.firstChildElement("count");

    if(group.isNull())
    {
        return;
    }

    //qDebug() << "Count groups: " << (group.toElement().text()).toInt();

    unsigned int count = (group.toElement().text()).toUInt();
    groupsResponse.reserve(count);

    group = response.firstChildElement("items");
    group = group.firstChildElement("group");

    while(!group.isNull())
    {
        //qDebug() << "Id: " << ((group.firstChildElement("id")).toElement()).text();
        //qDebug() << "Name: " << ((group.firstChildElement("name")).toElement()).text();
        groupsResponse.emplace_back(((group.firstChildElement("id")).toElement()).text(),
                                    ((group.firstChildElement("name")).toElement()).text());
        group = group.nextSibling();
    }

    qDebug() << "=================We got Groups===================";
    emit signalGroupsLoaded();
}
