#include "logindialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(const QString& app_id, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    login2vk = new LoginToVk(app_id, this); // "6687169"

    connect(login2vk, SIGNAL(userIsLogged(QPair<QString, QString>)), this, SLOT(loginEnd(QPair<QString, QString>)));

    ui->gridLayout->addWidget(login2vk);

    login2vk->loadLogin();
    login2vk->show();
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

QPair<QString, QString> LoginDialog::getInfo()
{
    return info;
}

void LoginDialog::loginEnd(QPair<QString, QString> inf)
{
    info = std::move(inf);
    close();
}
