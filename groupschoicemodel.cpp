#include "groupschoicemodel.h"

GroupsChoiceModel::GroupsChoiceModel(const QVector<QPair<QString, QString>>& vec_data, QObject* parent)
    : QAbstractListModel (parent)
{
    elems.reserve(vec_data.size());
    std::copy(std::begin(vec_data), std::end(vec_data), std::back_inserter(elems));
}

int GroupsChoiceModel::rowCount(const QModelIndex& parent) const
{
    return elems.size();
}

QVariant GroupsChoiceModel::data(const QModelIndex& index, int role) const
{
    if(index.row() < 0 || index.row() >= elems.size() || index.column() != 0)
    {
        return QVariant();
    }

    switch(role)
    {
    case Qt::DisplayRole:
        return (elems.at(index.row())).second;
    case Qt::CheckStateRole:
        return (checklist.contains(index.row())) ? Qt::Checked : Qt::Unchecked;
    default:
        return QVariant();
    }
}

bool GroupsChoiceModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    switch (role)
    {
    case Qt::CheckStateRole:
        if(value == Qt::Checked)
        {
            checklist.insert(index.row());
        }
        else
        {
            checklist.remove(index.row());
        }
        return true;
    }
    return false;
}

Qt::ItemFlags GroupsChoiceModel::flags(const QModelIndex& index) const
{
    if(!index.isValid())
    {
        return Qt::NoItemFlags;
    }

    return Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;
}

const QSet<int>& GroupsChoiceModel::chosenGroupsSet()
{
    return checklist;
}

const QVector<QPair<QString, QString>>& GroupsChoiceModel::getGroups()
{
    return elems;
}
