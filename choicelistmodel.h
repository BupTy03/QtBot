#pragma once
#ifndef CHOICELISTMODEL_H
#define CHOICELISTMODEL_H

#include <QAbstractListModel>
#include <QVector>

class ChoiceListModel : public QAbstractListModel
{
public:
    Q_OBJECT
public:
    explicit ChoiceListModel(const QStringList& str_list, QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    void setReadOnly(bool flag = true);

    QVector<int> getChosenElems();

private:
    QStringList elems_;
    QVector<bool> checklist_;
    bool readOnly_;
};

#endif // CHOICELISTMODEL_H
