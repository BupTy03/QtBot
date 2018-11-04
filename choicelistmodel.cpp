#include "choicelistmodel.h"

#include <algorithm>

ChoiceListModel::ChoiceListModel(const QStringList& str_list, QObject* parent)
    : QAbstractListModel (parent), elems_(str_list), checklist_(str_list.size(), false), readOnly_(false)
{
}

int ChoiceListModel::rowCount(const QModelIndex& parent) const
{
    return elems_.size();
}

QVariant ChoiceListModel::data(const QModelIndex& index, int role) const
{
    if(index.row() < 0 || index.row() >= elems_.size() || index.column() != 0)
    {
        return QVariant();
    }

    switch(role)
    {
    case Qt::DisplayRole:
        return (elems_.at(index.row()));
    case Qt::CheckStateRole:
        if(readOnly_)
        {
            return QVariant();
        }

        return checklist_.at(index.row()) ? Qt::Checked : Qt::Unchecked;
    }
    return QVariant();
}

bool ChoiceListModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if(readOnly_)
    {
        return false;
    }

    switch (role)
    {
    case Qt::CheckStateRole:
        checklist_[index.row()] = (value == Qt::Checked);
        return true;
    }
    return false;
}

Qt::ItemFlags ChoiceListModel::flags(const QModelIndex& index) const
{
    if(!index.isValid())
    {
        return Qt::NoItemFlags;
    }

    if(readOnly_)
    {
        return Qt::ItemIsEnabled;
    }

    return Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;
}

void ChoiceListModel::setReadOnly(bool flag)
{
    readOnly_ = flag;
}

QVector<int> ChoiceListModel::getChosenElems()
{
    QVector<int> result;
    const auto vec_size = checklist_.size();
    for(int i = 0; i < vec_size; ++i)
    {
        if(checklist_.at(i))
        {
            result.push_back(i);
        }
    }
    return result;
}
