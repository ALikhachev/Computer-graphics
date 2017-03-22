#ifndef FILTERZONE_H
#define FILTERZONE_H

#include <QWidget>

class FilterZone : public QWidget
{
    Q_OBJECT
public:
    explicit FilterZone(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *) override;
};

#endif // FILTERZONE_H
