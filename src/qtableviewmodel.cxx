#include <QModelIndex>
#include <QDebug>
#include <QPixmap>

#include <phyer/qtableviewmodel.h>

QTableViewModel::QTableViewModel(QObject *parent)
    :QAbstractListModel(parent)
{
    values = new QList<RegisterFlag>();
}

int QTableViewModel::rowCount(const QModelIndex &) const
{
    return values->count();
}

int QTableViewModel::columnCount(const QModelIndex &) const
{
    return 4;
}

QVariant QTableViewModel::data( const QModelIndex &index, int role ) const
{
    QVariant value;

    switch ( role )
    {
        case Qt::DisplayRole: //string
        {
            switch (index.column())
            {
                case 0:
                    value = QVariant::fromValue(this->values->at(index.row()).getId());
                    break;
                case 1:
                    value = this->values->at(index.row()).getName();
                    break;
                case 2:
                    value = this->values->at(index.row()).getValue();
                    break;
                case 3:
                    value = this->values->at(index.row()).getEvalue();
                    break;
            }
        }
        break;

        case Qt::UserRole: //data
            value = QVariant::fromValue(this->values->at(index.row()).getId());
            break;

        case Qt::ToolTipRole:
            value = this->values->at(index.row()).getDesc();
            break;

        default:
            break;
    }

    return value;
}

QString QTableViewModel::dataReg(const QModelIndex &index) const
{
    return this->values->at(index.row()).getValue();
}

unsigned long QTableViewModel::dataAddr(const QModelIndex &index) const
{
    return this->values->at(index.row()).getId();
}

QVariant QTableViewModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return QString("addr");
        case 1:
            return QString("name");
        case 2:
            return QString("val");
        case 3:
            return QString("type");
        }
    }
    return QVariant();
}

void QTableViewModel::populate(QList<RegisterFlag> newValues)
{
    this->beginInsertRows(QModelIndex(), 1, newValues.count());
        *this->values = newValues;
    endInsertRows();
}

void QTableViewModel::append(RegisterFlag value)
{
    int newRow = this->values->count()+1;

    this->beginInsertRows(QModelIndex(), newRow, newRow);
        values->append(value);
    endInsertRows();
}

void QTableViewModel::update(int idx, RegisterFlag value)
{
    (*this->values)[idx] = value;

    QModelIndex item_idx_s = this->index(idx,0);
    QModelIndex item_idx_e = this->index(idx,this->columnCount(QModelIndex()));

    emit this->dataChanged(item_idx_s ,item_idx_e );
}

void QTableViewModel::deleteRow(int idx)
{
    this->beginRemoveRows(QModelIndex(), idx,idx);

        (*this->values).removeAt(idx);

    this->endRemoveRows();
}

void QTableViewModel::insertAt(int idx, RegisterFlag value)
{
    int newRow = idx;

    this->beginInsertRows(QModelIndex(), newRow, newRow);

        values->insert(newRow,value);

    endInsertRows();
}

