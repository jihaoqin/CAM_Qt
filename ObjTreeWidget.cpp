#include "ObjTreeWidget.h"
#include <QVBoxLayout>
#include "Controller.h"
#include "Data.h"
#include <QPushButton>
#include "Color.h"

ObjTreeWidget::ObjTreeWidget(QWidget* parent):QWidget(parent), button(nullptr), lastPicked()
{
    view = new QTreeView();
    model = new ObjModel();
    view->setModel(model);
    view->setHeaderHidden(true);
    QVBoxLayout* layout = new QVBoxLayout(this);
    button = new QPushButton(this);
    button->setText("clear selection");
    button->setFixedHeight(40);
    delButton = new QPushButton(this);
    delButton->setText("delete");
    button->setFixedHeight(40);
    layout->addWidget(button);
    layout->addWidget(delButton);
    layout->addWidget(view);
    setLayout(layout);
    view->show();

    connect(button, &QPushButton::pressed, this, &ObjTreeWidget::clearSelection);
    connect(delButton, &QPushButton::pressed, this, &ObjTreeWidget::deleteSelected);
    connect(view, &QTreeView::pressed, this, &ObjTreeWidget::displayInFront);
}

void ObjTreeWidget::setConnector(GuiConnector* c){
    connector = c;
    updateModel();
}

void ObjTreeWidget::updateModel(){
    model->bindData(connector->getData()->getNodeRoot());
}

void ObjTreeWidget::clearSelection(){
    auto selection = view->currentIndex();
    if(selection.row() == -1){
        return;
    }
    auto itemData = view->model()->itemData(selection);
    QStringVec bandId{itemData.values().at(0).toString()};
    connector->getCtrl()->setColor(bandId, Color::YELLOW);
    view->setCurrentIndex(QModelIndex());
}

void ObjTreeWidget::deleteSelected(){
    auto selection = view->currentIndex();
    if(selection.row() == -1){
        return;
    }
    auto itemData = view->model()->itemData(selection);
    QString bandId = itemData.values().at(0).toString();
    connector->getCtrl()->deleteBand(bandId);
}

void ObjTreeWidget::displayInFront(){
    connector->getCtrl()->setColor(QStringVec{lastPicked}, Color::YELLOW);
    auto selection = view->currentIndex();
    if(selection.row() == -1){
        return;
    }
    auto itemData = view->model()->itemData(selection);
    QStringVec bandId{itemData.values().at(0).toString()};
    connector->getCtrl()->setColor(bandId, Color::GREEN);
    lastPicked = bandId.at(0);
}
