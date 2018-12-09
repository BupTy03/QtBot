#pragma once
#ifndef ADDTASKWINDOW_H
#define ADDTASKWINDOW_H

#include <QDialog>
#include <QString>
#include <QStandardItemModel>
#include <QJsonDocument>
#include <QJsonArray>

#include <memory>
#include <vector>
#include <utility>

namespace Ui {
class AddTaskWindow;
}

class AddTaskWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AddTaskWindow(const QString& access_token, const QString& user_id, QWidget* parent = nullptr);
    virtual ~AddTaskWindow() override;

    int getInterval() const;
    int getPeriod() const;
    QString getMessage() const;
    bool hasImage() const;
    QString getImgPath() const;
    std::vector<std::pair<QString, QString>> getGroups() const;

private slots:
    void on_radioBtnList_toggled(bool checked);
    void on_radioBtnFile_toggled(bool checked);
    void on_loadImageBtn_clicked();
    void updateLoadImg();

private:
    void updateItems();
    bool areAllItemsUnchecked();
    std::shared_ptr<QJsonArray> getGroupsFromJson(const QJsonDocument& document) const;
    std::shared_ptr<QJsonArray> readGroupsFromFile(const QString& filename);

public slots:
    virtual int exec() override;

private:
    Ui::AddTaskWindow* ui{};
    QStandardItemModel* groupsModel_{};
    std::shared_ptr<QJsonArray> userGroups_;
    std::shared_ptr<QJsonArray> currentList_;
    QString imgPath_;
    QPixmap loadImg_;

    // QWidget interface
protected:
    virtual void resizeEvent(QResizeEvent* event) override;
};

#endif // ADDTASKWINDOW_H
