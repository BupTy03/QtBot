#pragma once
#ifndef ADDTASKWINDOW_H
#define ADDTASKWINDOW_H

#include <QDialog>
#include <QString>
#include <QVector>
#include <QStringList>
#include <QSharedPointer>
#include <QStandardItemModel>
#include <QJsonDocument>

namespace Ui {
class AddTaskWindow;
}

struct Group
{
    Group(){}

    Group(QString id, QString name)
        : id(std::move(id)), name(std::move(name)){}

    QString id;
    QString name;
};

class AddTaskWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AddTaskWindow(const QString& access_token, const QString& user_id, QWidget* parent = nullptr);
    virtual ~AddTaskWindow() override;

    int getInterval() const;
    int getPeriod() const;
    QVector<int> getGroupsIndexes() const;
    QStringList getGroupsIds() const;
    QStringList getGroupsNames() const;
    QString getMessage() const;
    bool hasImage() const;
    QString getImgPath() const;

private slots:
    void on_radioBtnList_toggled(bool checked);

    void on_radioBtnFile_toggled(bool checked);

    void on_loadImageBtn_clicked();

    void updateLoadImg();
private:
    void updateItems();
    bool areAllItemsUnchecked();
    QSharedPointer<QVector<Group>> getGroupsFromJson(const QJsonDocument& document) const;
    QSharedPointer<QVector<Group>> readGroupsFromFile(const QString& filename);

public slots:
    virtual int exec() override;

private:
    Ui::AddTaskWindow *ui;
    QStandardItemModel* groupsModel_;
    QSharedPointer<QVector<Group>> userGroups_;
    QSharedPointer<QVector<Group>> currentList_;
    QString imgPath_;
    QPixmap loadImg_;

    // QWidget interface
protected:
    virtual void resizeEvent(QResizeEvent* event) override;
};

#endif // ADDTASKWINDOW_H
