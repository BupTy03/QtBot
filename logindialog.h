#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QPair>
#include <QVariant>

#include "logintovk.h"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(const QString& app_id, QWidget *parent = nullptr);
    ~LoginDialog();
    QPair<QString, QString> getInfo();

private:
    Ui::LoginDialog *ui;
    LoginToVk* login2vk;
    QPair<QString, QString> info;

public slots:
    void loginEnd(QPair<QString, QString> inf);

};

#endif // LOGINDIALOG_H
