#include "groupschoicemodel.h"

GroupsChoiceModel::GroupsChoiceModel(const QVector<QPair<QString, QString>>& vec_data, QObject* parent)
    : QAbstractListModel (parent)
{
    elems = vec_data;
    //elems.reserve(vec_data.size());
    //std::copy(std::begin(vec_data), std::end(vec_data), std::back_inserter(elems));
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
        if(readOnly)
        {
            return QVariant();
        }
        return (checklist.contains(index.row())) ? Qt::Checked : Qt::Unchecked;
    default:
        return QVariant();
    }
}

bool GroupsChoiceModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if(readOnly)
    {
        return false;
    }

    switch (role)
    {
    case Qt::CheckStateRole:
        if(value == Qt::Checked)
        {
            checklist.push_back(index.row());
        }
        else
        {
            checklist.removeAll(index.row());
        }
        emit dataChanged(index.row());
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

    if(readOnly)
    {
        return Qt::ItemIsEnabled;
    }

    return Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;
}

void GroupsChoiceModel::setChecklist(const QLinkedList<int>& chkLst)
{
    this->checklist = chkLst;
    QVector<QPair<QString, QString>> new_gr;
    for(auto indx : chkLst)
    {
        new_gr.append(this->elems.at(indx));
    }
    this->elems = std::move(new_gr);
}

const QLinkedList<int>& GroupsChoiceModel::chosenGroups() const
{
    return checklist;
}

const QVector<QPair<QString, QString>>& GroupsChoiceModel::getGroups() const
{
    return elems;
}

void GroupsChoiceModel::setReadOnly(bool flag)
{
    readOnly = flag;
}
