#ifndef GROUPSCHOICEMODEL_H
#define GROUPSCHOICEMODEL_H

#include <QAbstractListModel>
#include <QSet>

class GroupsChoiceModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit GroupsChoiceModel(const QVector<QPair<QString, QString>>& vec_data, QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    const QSet<int>& chosenGroupsSet();
    const QVector<QPair<QString, QString>>& getGroups();

private:
    QVector<QPair<QString, QString>> elems;
    QSet<int> checklist;
};

#endif // GROUPSCHOICEMODEL_H
