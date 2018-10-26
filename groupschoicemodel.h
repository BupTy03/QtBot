#ifndef GROUPSCHOICEMODEL_H
#define GROUPSCHOICEMODEL_H

#include <QAbstractListModel>
#include <QLinkedList>

class GroupsChoiceModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit GroupsChoiceModel(const QVector<QPair<QString, QString>>& vec_data, QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    void setChecklist(const QLinkedList<int>& chkLst);

    const QLinkedList<int>& chosenGroups() const;
    const QVector<QPair<QString, QString>>& getGroups() const;
    void setReadOnly(bool flag = true);

signals:
    void dataChanged(int);

private:
    QVector<QPair<QString, QString>> elems;
    QLinkedList<int> checklist;
    bool readOnly{false};
};

#endif // GROUPSCHOICEMODEL_H
