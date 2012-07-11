#include "adjmatrixmodel.h"

#include <QtDebug>

AdjMatrixModel::AdjMatrixModel(int nn, QObject *parent) :
    QAbstractTableModel(parent)
{
    n = nn;
}

Qt::ItemFlags AdjMatrixModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractTableModel::flags(index);
    return index.isValid() ? (flags | Qt::ItemIsUserCheckable) : flags;
}

QVariant AdjMatrixModel::data(const QModelIndex &index, int role) const
{
    //checking if the index is valid
    if (!index.isValid())
        return QVariant();
    //adj. matrix can't have loops in our program, so we will prevent the user from entering such
   /* if (index.row() == index.column())
        return Qt::Unchecked;*/
    if (role == Qt::CheckStateRole) {
        return els[index] ? Qt::Checked : Qt::Unchecked;
    }
    return QVariant();
}

bool AdjMatrixModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && (role == Qt::CheckStateRole)) {
        els[index] = value.toBool();
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

int AdjMatrixModel::rowCount(const QModelIndex &) const
{
    return n;
}

int AdjMatrixModel::columnCount(const QModelIndex &parent) const
{
    return n;
}

QVariant AdjMatrixModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();
    return QString::number(section + 1);
}

void AdjMatrixModel::toBoolMatrix(bool **matrix)
{
    for(uint i = 0; i < n; ++i)
        for(uint j = 0; j < n; ++j)
        {
            QModelIndex qm = index(i, j);
            if(data(qm, Qt::CheckStateRole).toBool() == true)
                matrix[i][j] = true;
            else
                matrix[i][j] = false;
        }
}

void AdjMatrixModel::fromBoolMatrix(bool **matrix, uint nn)
{
   n = nn;
   for(uint i = 0; i < n; ++i)
       for(uint j = 0; j < n; ++j)
       {
           QModelIndex qm = index(i, j);
           if(matrix[i][j])
               els[qm] = true;
           else
               els[qm] = false;
       }
}


void AdjMatrixModel::incrementN()
{
    ++n;
    beginInsertRows(QModelIndex(), n, n);
    for(uint i = 0; i < n; ++i)
        els[index(n-1, i)] = false;
    endInsertRows();
    beginInsertColumns(QModelIndex(), n, n);
    for(uint i = 0; i < n; ++i)
        els[index(i, n-1)] = false;
    endInsertColumns();
}

void AdjMatrixModel::decrementN()
{
    //do we need matrices of 1x1?
    if (n < 2)
        return;

    --n;
    beginRemoveRows(QModelIndex(), n, n);
    for(uint i = 0; i < n; ++i)
        els.remove(index(n-1, i));
    endRemoveRows();
    beginRemoveColumns(QModelIndex(), n, n);
    for(uint i = 0; i < n-1; ++i)
        els.remove(index(i, n-1));
    endRemoveColumns();
}

int AdjMatrixModel::dimension() const
{
    if(rowCount() != columnCount())
        return -1;
    return rowCount();
}
