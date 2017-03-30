#ifndef QSLIDERBOX_H
#define QSLIDERBOX_H

#include <QWidget>
#include <QSlider>

class QSliderBox : public QWidget
{
    Q_OBJECT
public:
    explicit QSliderBox(int min, int max, QWidget *parent = 0);
    void setValue(int);
    int getValue();

signals:
    void valueChanged(int);

private:
    QSlider *slider;
};

#endif // QSLIDERBOX_H
