#include "logintovk.h"

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
    query.addQueryItem("scope", "331776"); //331776   266240
    query.addQueryItem("response_type", "token");
    query.addQueryItem("v", "5.52");


    qDebug() << "Query:" << query.toString();
    url.setQuery(query);
    qDebug() << "Url:" << url.toString();
    load(url);
}

void LoginToVk::userLogged(const QUrl& url)
{
    qDebug() << "Response URL: " << url.toString();
    qDebug() << "Fragment: " << url.fragment();

    QRegExp regExp("^access_token=([^&]+)&expires_in=\\d+&user_id=(\\d+)$");
    if(regExp.exactMatch((url.fragment())))
    {
        qDebug() << regExp.cap(1);
        qDebug() << regExp.cap(2);
        emit userIsLogged(qMakePair(regExp.cap(2), regExp.cap(1)));
    }
}
