#include "vkauth.h"

VKAuth::VKAuth(QString appID, QObject* parent) : QObject(parent), appID_(std::move(appID)){}

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
    browser_.page()->profile()->setHttpCacheType(QWebEngineProfile::NoCache);
    browser_.page()->profile()->setPersistentCookiesPolicy(QWebEngineProfile::NoPersistentCookies);
    browser_.load(url);

    QObject::connect(&browser_, &QWebEngineView::urlChanged, [this](const QUrl& url)
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
            browser_.close();
            emit done(isValid());
            return;
        }

        browser_.close();
        QMessageBox::information(&browser_, tr("Ошибка"), tr("Не удалось войти ¯\\_(ツ)_/¯"));
    });

    browser_.show();
}

bool VKAuth::isValid()
{
    return !accessToken_.isEmpty() && !userID_.isEmpty();
}

