#ifndef OBJMODEL_H
#define OBJMODEL_H
#include <QAbstractItemModel>
#include "TreeItem.h"
#include "Node.h"

class TreeItem;


class ObjModel:public QAbstractItemModel
{
    Q_OBJECT
public:
    ObjModel(QObject* parent = 0);
    QModelIndex index(int row, int column
                      , const QModelIndex& parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    void bindData(NodePtr);
private:
    void setupData();
    void initialTreeItem();
    TreeItem* rootItem;
    NodePtr root;
};

#endif // OBJMODEL_H
