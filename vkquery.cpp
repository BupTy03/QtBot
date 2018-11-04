#include "vkquery.h"

VKQuery::VKQuery(QObject *parent) : QObject(parent){}

void VKQuery::get_groups(const VKAuth& vk)
{
    qDebug() << "==================Getting IDs of Groups of User with ID: " << vk.get_user_id() << " =====================";
    QUrl request("https://api.vk.com/method/groups.get");

    QUrlQuery query(request);
    query.addQueryItem("user_id", vk.get_user_id());
    query.addQueryItem("count", "1000");
    query.addQueryItem("extended", "1");
    query.addQueryItem("fields", "name");
    query.addQueryItem("access_token", vk.get_access_token());
    query.addQueryItem("test_mode", "1");
    query.addQueryItem("v", "5.52");

    qDebug() << "Query:" << query.toString();
    request.setQuery(query);
    qDebug() << "Final request: " << request.toString();

    QNetworkAccessManager* netMgr = new QNetworkAccessManager(this);

    connect(netMgr, &QNetworkAccessManager::finished, [this](QNetworkReply* reply)
    {
        QJsonDocument document(QJsonDocument::fromJson(reply->readAll()));

        emit done(std::make_shared<QJsonDocument>(document));

//        for(const auto& group : get_groups_from_json(document))
//        {
//            qDebug() << "group_id = " << group.first;
//            qDebug() << "group_name = " << group.second;
//        }

        qDebug() << "=================We got Groups===================";
    });

    netMgr->get(QNetworkRequest(request));
}
