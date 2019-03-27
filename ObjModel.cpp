#include "ObjModel.h"
#include <memory>

using namespace std;

ObjModel::ObjModel(QObject* parent):QAbstractItemModel(parent), root(nullptr)
{
    initialTreeItem();
}

QModelIndex ObjModel::index(int row, int column, const QModelIndex& parent) const{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeItem* parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    TreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex ObjModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem = childItem->parentItem();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int ObjModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

int ObjModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();
}

QVariant ObjModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

    return item->data(index.column());
}

void ObjModel::setupData(){
    beginResetModel();
    if(nullptr == root){
        return ;
    }
    initialTreeItem();
    auto children = root->childrenPtrVec();
    std::shared_ptr<QString> strs;
    QList<QVariant> bandIds;
    for(auto &c:children){
        QString id = c->Id();
        if(id.contains("band")){
            bandIds<< id;
            QList<QVariant> name;
            name<<id;
            TreeItem* item = new TreeItem(name, rootItem);
            rootItem->appendChild(item);
        }
    }
    endResetModel();
}

void ObjModel::bindData(NodePtr n){
    root = n;
    initialTreeItem();
    setupData();
}

void ObjModel::initialTreeItem(){
    if(!rootItem){
        delete rootItem;
    }
    QList<QVariant> name;
    name<<"tee";
    rootItem = new TreeItem(name);
}
