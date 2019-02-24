#ifndef STRECHLABEL_H
#define STRECHLABEL_H
#include <QLabel>

class GuiConnector;
class StrechLabel : public QLabel
{
    Q_OBJECT
public:
    StrechLabel(QWidget* parent = nullptr);
    void setConnector(GuiConnector*);
    friend class GuiConnector;

signals:
    void strech(int w);
protected:
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void enterEvent(QEvent* event) override;
    void leaveEvent(QEvent* event) override;
private:
    QPoint lastPos;
    GuiConnector* connector;
};

#endif // STRECHLABEL_H
