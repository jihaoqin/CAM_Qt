#include "LeftRibbonLayout.h"
#include <QWidget>

LeftRibbonLayout::LeftRibbonLayout()
{
    list.append(nullptr);
    list.append(nullptr);
    list.append(nullptr);
}

void LeftRibbonLayout::addItem(QLayoutItem* item){
   //do nothing
}
int LeftRibbonLayout::count() const{
    //do nothing
    int count = 0;
    for(auto i:list){
        if (i){
            count++;
        }
    }
    return count;
}

QLayoutItem* LeftRibbonLayout::itemAt(int index) const{
    return list.value(index);
}


QLayoutItem* LeftRibbonLayout::takeAt(int idx){
    return idx >= 0 && idx < list.size() ? list.takeAt(idx) : 0;
}

QSize LeftRibbonLayout::sizeHint() const {
    //return QSize(800, 600) + QSize(150,0);
    if(list.size() == 0){
        return QSize(0,0);
    }
    else if(list.size() > 0){
        QLayoutItem* o = list.at(0);
        QWidget* wid = o->widget();
        return QSize(wid->width(), wid->height());
    }
}

void LeftRibbonLayout::setGeometry(const QRect &r){
    QLayout::setGeometry(r);
    if(count() == 0){
        return;
    }
    int w = r.width();
    int h = r.height();
    list.at(0)->setGeometry(QRect(r.x(),r.y(),r.width(),r.height()));
    int num = count();
    if(count() > 1){
        QLayoutItem* o = list.at(1);
        QWidget* wid = o->widget();
        o->setGeometry(QRect(r.x(), r.y(), wid->width(), r.height()));
    }
    if(count()>2){
        QLayoutItem* o_1 = list.at(1);
        QWidget* wid_1 = o_1->widget();

        QLayoutItem* o = list.at(2);
        QWidget* wid = o->widget();
        int left_x = r.x()+wid_1->x()+wid_1->width();
        int left_y = r.y()+wid_1->y()+wid_1->height()/2.0;
        int w = wid->width();
        int h = wid->height();
        o->setGeometry(QRect(left_x, left_y, w, h));
        //o->setGeometry(QRect(400,400, 50,50));
    }

}

QSize LeftRibbonLayout::minimumSize() const{
    return sizeHint();
}

LeftRibbonLayout::~LeftRibbonLayout(){
    QLayoutItem* item;
    while((item = takeAt(0))){
        delete item;
    }
}

void LeftRibbonLayout::addBottomWidget(QWidget *w){
    if(list.size() == 0){
        list.append(new QWidgetItem(w));
    }
    else{
        list[0] = new QWidgetItem(w);
    }
}

void LeftRibbonLayout::addLeftWidget(QWidget *w){
    if(list.size() == 0){
        //do nothing
    }
    else if(list.size() == 1){
        list.append(new QWidgetItem(w));
    }
    else{
        list[1] = new QWidgetItem(w);
    }
}

void LeftRibbonLayout::addStrechLabel(QWidget *label){
    list[2] = new QWidgetItem(label);
    int num = count();
}
