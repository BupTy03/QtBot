#ifndef LOGINTOVK_H
#define LOGINTOVK_H

#include <QWebEngineView>
#include <QUrlQuery>
#include <QPair>

class LoginToVk : public QWebEngineView
{
	Q_OBJECT
private:
    QString app_id;

public:
    LoginToVk(const QString& app_id, QWidget* parent = nullptr);
    void loadLogin();

private slots:
    void userLogged(const QUrl& url);

signals:
    void userIsLogged(QPair<QString, QString>);

};

#endif // LOGINTOVK_H
