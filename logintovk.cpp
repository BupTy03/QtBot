#include "logintovk.h"

#include <iostream>
#include <QRegExp>

LoginToVk::LoginToVk(const QString& appId, QWidget* parent) : QWebEngineView (parent), app_id(appId)
{
    QObject::connect(this, SIGNAL(urlChanged(const QUrl&)), SLOT(userLogged(const QUrl&)));
}

void LoginToVk::loadLogin()
{
    QUrl url("https://oauth.vk.com/authorize");

    QUrlQuery query(url);
    query.addQueryItem("client_id", app_id);
    query.addQueryItem("display", "popup");
    query.addQueryItem("redirect_uri", "https://oauth.vk.com/blank.html");
    query.addQueryItem("scope", "266240"); //331776   266240
    query.addQueryItem("response_type", "token");
    query.addQueryItem("v", "5.52");


    std::cout << "Query:" << (query.toString()).toStdString() << std::endl;
    url.setQuery(query);
    std::cout << "Url:" << (url.toString()).toStdString() << std::endl;
    load(url);
}

void LoginToVk::userLogged(const QUrl& url)
{
    std::cout << "Response URL: " << (url.toString()).toStdString() << std::endl;
    std::cout << "Fragment: " << (url.fragment()).toStdString() << std::endl;

    QRegExp regExp("^access_token=([^&]+)&expires_in=\\d+&user_id=(\\d+)$");
    if(regExp.exactMatch((url.fragment())))
    {
        std::cout << (regExp.cap(1)).toStdString() << std::endl;
        std::cout << (regExp.cap(2)).toStdString() << std::endl;
        emit userIsLogged(qMakePair(regExp.cap(1), regExp.cap(2)));
    }
}
