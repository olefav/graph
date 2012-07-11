#ifndef ADJMATRIXMODEL_H
#define ADJMATRIXMODEL_H

#include <QAbstractTableModel>

class AdjMatrixModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    void toBoolMatrix(bool ** matrix);
    void fromBoolMatrix(bool ** matrix, uint n);
    explicit AdjMatrixModel(int nn = 2,QObject *parent = 0);
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    int dimension() const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    void incrementN();
    void decrementN();
private:
    uint n;
    QHash<QModelIndex, bool> els;
        
};

#endif // ADJMATRIXMODEL_H
