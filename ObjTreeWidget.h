#ifndef OBJTREEWIDGET_H
#define OBJTREEWIDGET_H
#include <QTreeView>
#include "Node.h"
#include "GuiConnector.h"
#include "ObjModel.h"

class QPushButton;

class ObjTreeWidget:public QWidget
{
    Q_OBJECT
public:
    ObjTreeWidget(QWidget* parent = 0);
    void setConnector(GuiConnector*);
    void updateModel();
    void clearSelection();
    void deleteSelected();
    void displayInFront();
private:
    QTreeView* view;
    ObjModel* model;
    QPushButton* button;
    QPushButton* delButton;
    GuiConnector* connector;
};

#endif // OBJTREEWIDGET_H
