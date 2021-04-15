#ifndef QTABLEVIEWMODEL_H
#define QTABLEVIEWMODEL_H


#include <registerflag.h>
#include <QModelIndex>

class QTableViewModel : public QAbstractListModel
{
public:
    QTableViewModel(QObject *parent=nullptr);
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

private:
    QList<RegisterFlag> *values;
};

#endif // QTABLEVIEWMODEL_H

