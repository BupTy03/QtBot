#include "vkauth.h"

#include <QUrlQuery>
#include <QRegExp>

VKAuth::VKAuth(QString appID, QObject* parent) : QObject(parent), appID_(std::move(appID))
{
    QObject::connect(&browser_, &QWebEngineView::urlChanged, this, &VKAuth::checkAuth);
    blockSignals(true);
}

void VKAuth::auth(QString scope)
{
    QUrl url("https://oauth.vk.com/authorize");

    QUrlQuery query(url);
    query.addQueryItem("client_id", appID_);
    query.addQueryItem("display", "popup");
    query.addQueryItem("redirect_uri", "https://oauth.vk.com/blank.html");
    query.addQueryItem("scope", scope); //   331776   266240
    query.addQueryItem("response_type", "token");
    query.addQueryItem("v", "5.52");
#ifdef DEBUG
    qDebug() << "Query:" << query.toString();
#endif
    url.setQuery(query);
#ifdef DEBUG
    qDebug() << "Url:" << url.toString();
#endif
    browser_.load(url);
    blockSignals(false);
    browser_.show();
}

void VKAuth::reauth(QString scope)
{
    (((browser_.page())->profile())->cookieStore())->deleteAllCookies();
    ((browser_.page())->profile())->clearHttpCache();
    auth(scope);
}

bool VKAuth::isValid()
{
    return !accessToken_.isEmpty() && !userID_.isEmpty();
}

void VKAuth::checkAuth(const QUrl& url)
{
#ifdef DEBUG
    qDebug() << "Response URL: " << url.toString();
    qDebug() << "Fragment: " << url.fragment();
#endif
    QRegExp regExp("^access_token=([^&]+)&expires_in=\\d+&user_id=(\\d+)$");
    if(regExp.exactMatch((url.fragment())))
    {
        accessToken_ = regExp.cap(1);
        userID_      = regExp.cap(2);
#ifdef DEBUG
        qDebug() << "user_id: " << userID_;
        qDebug() << "access_token = " << accessToken_;
#endif
    }
    else return;

    browser_.close();
    emit done(this->isValid());
    //blockSignals(true);
}

