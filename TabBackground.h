#ifndef TABBACKGROUND_H
#define TABBACKGROUND_H
#include <QWidget>
class QVBoxLayout;
class GuiConnector;

class TabBackground: public QWidget
{
    Q_OBJECT

public:
    TabBackground(QWidget* parent);
    void setWidget(QWidget* w);
    bool isSet();
    void setConnector(GuiConnector*);
    friend class GuiConnector;
protected:
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
private:
    QWidget* center;
    QVBoxLayout* layout;
    GuiConnector* connector;
};

#endif // TABBACKGROUND_H
