#include <QModelIndex>
#include <QDebug>
#include <QPixmap>
#include <QMessageBox>

#include <phyer/qtableviewmodel.h>

QTableViewModel::QTableViewModel(Hardware *_hrdw)
    :QAbstractListModel()
{
    values = new QList<RegisterFlag>();
    hrdw = _hrdw;
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

        case Qt::BackgroundRole:
            if (this->values->at(index.row()).getEvalue() == "rw")
            {
                value = QBrush(QColor(117, 255, 184));
            }
            break;

        default:
            break;
    }

    return value;
}

bool QTableViewModel::setData(
    const QModelIndex &index
  , const QVariant &value
  , int role
){
    if (role != Qt::EditRole)
    {
        return false;
    }

    if (index.column() == 1)
    {
        (*this->values)[index.row()].setName(
            value.toString()
        );
    }
    if (index.column() == 2)
    {
        unsigned short temp_value = 0;
        bool temp_ok = false;

        temp_value = value.toString().toInt(&temp_ok, 16);

        if (!temp_ok)
        {
            QMessageBox msgBox;
            msgBox.setText("You should use hex-decimal value");
            msgBox.exec();
            return false;
        }

        if (hrdw)
        {
            hrdw->setRegisterValue(this->values->at(index.row()).getId(), temp_value);
            temp_value = hrdw->getRegisterValue(
                this->values->at(index.row()).getId()
            );
            (*this->values)[index.row()].setValue(
                QString::asprintf("0x%04hX", temp_value)
            );
        }
    }
    if (index.column() == 3)
    {
        if (value.toString() == "rw" || value.toString() == "ro")
        {
            (*this->values)[index.row()].setEvalue(
                value.toString()
            );
        }
    }
    return true;
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
            return QString("access");
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

Qt::ItemFlags QTableViewModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    if (index.row() >= Hardware::getStdRegCount() || index.column() == 2)
    {
        return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
    }
    if (index.row() >= Hardware::getStdRegCount() && index.column() == 3)
    {
        return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
    }
    return QAbstractItemModel::flags(index);
}

