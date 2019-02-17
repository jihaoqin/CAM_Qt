#ifndef TABBACKGROUND_H
#define TABBACKGROUND_H
#include <QWidget>

class TabBackground: public QWidget
{
    Q_OBJECT

public:
    TabBackground(QWidget* parent);
protected:
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
};

#endif // TABBACKGROUND_H
