#ifndef QTABLEVIEWMODEL_H
#define QTABLEVIEWMODEL_H

#include <QModelIndex>

#include <phyer/hardware.h>


class QTableViewModel : public QAbstractListModel
{
    Hardware *hrdw;

public:
    QList<RegisterFlag> *values;
    QTableViewModel(Hardware *_hrdw = nullptr);
    int rowCount(const QModelIndex &) const;
    int columnCount(const QModelIndex &parent) const;

    QVariant data(const QModelIndex &index, int role) const;
    QString dataReg(const QModelIndex &index) const;
    unsigned long dataAddr(const QModelIndex &index) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    void populate(QList<RegisterFlag> newValues);

    void append(RegisterFlag value);
    void update(int idx, RegisterFlag value);
    void deleteRow(int idx);
    void insertAt(int idx, RegisterFlag value);
    Qt::ItemFlags flags(const QModelIndex &index) const;

    bool setData(
        const QModelIndex &index
      , const QVariant &value
      , int role = Qt::EditRole
    );
};

#endif // QTABLEVIEWMODEL_H

