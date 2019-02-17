#ifndef LEFTRIBBONLAYOUT_H
#define LEFTRIBBONLAYOUT_H
#include <QLayout>

class StrechLabel;
class LeftRibbonLayout : public QLayout
{
    Q_OBJECT

public:
    LeftRibbonLayout();
    ~LeftRibbonLayout();
    virtual void addItem(QLayoutItem* item) override;
    virtual int count() const override;
    virtual QLayoutItem* itemAt(int index) const override;
    virtual QLayoutItem* takeAt(int index) override;
    virtual QSize sizeHint() const override;
    virtual void setGeometry(const QRect &r) override;
    virtual QSize minimumSize() const override;
    void addBottomWidget(QWidget* w);
    void addLeftWidget(QWidget* w);
    void addStrechLabel(QWidget* label);
private:
    QList<QLayoutItem*> list;
};

#endif // LEFTRIBBONLAYOUT_H
