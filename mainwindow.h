#pragma once
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QThread>
#include <QJsonDocument>
#include <QJsonArray>
#include <QCloseEvent>

#include "addtaskwindow.h"
#include "task.h"
#include "taskwidget.h"
#include "vkauth.h"

namespace Ui {
class MainWindow;
}

struct User
{
    User(){}

    User(QString id, QString name, QString access_token)
        : id(std::move(id)), name(std::move(name)), access_token(std::move(access_token)){}

    QString id;
    QString name;
    QString access_token;
};


class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    virtual ~MainWindow() override;

private slots:
    void on_ExitAction_triggered();

    void on_NewTaskAction_triggered();

    void checkLogin(bool success);

    void on_AddUser_triggered();

    void on_ChangeUserCB_currentIndexChanged(int index);

private:
    QString userNameFromJson(const QJsonDocument& doc) const;
    void addNewUser(const QString& id, const QString& access_token);
    void updateUsersComboBox();

    virtual void closeEvent(QCloseEvent* event) override;

private:
    Ui::MainWindow *ui{nullptr};
    VKAuth* vkAuth_{nullptr};
    int currentUser{};
    QThread* secondThread_{nullptr};
    const QString app_id_{"6667132"};
    const QString scope_{"335876"}; // +4(photos)
                                    // +8192(wall)
                                    // +65536(offline)
                                    // +262144(groups) = 335876
    QJsonArray users_;
};

#endif // MAINWINDOW_H
