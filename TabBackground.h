#ifndef TABBACKGROUND_H
#define TABBACKGROUND_H
#include <QWidget>
class QVBoxLayout;

class TabBackground: public QWidget
{
    Q_OBJECT

public:
    TabBackground(QWidget* parent);
    void setWidget(QWidget* w);
    bool isSet();
protected:
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
private:
    QWidget* center;
    QVBoxLayout* layout;
};

#endif // TABBACKGROUND_H
