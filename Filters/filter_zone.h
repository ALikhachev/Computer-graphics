#ifndef FILTERZONE_H
#define FILTERZONE_H

#include <QWidget>

class FilterZone : public QWidget
{
    Q_OBJECT
public:
    explicit FilterZone(QWidget *parent = 0);

    void setImage(QImage &);

protected:
    void paintEvent(QPaintEvent *) override;

private:
    void drawLine(QPoint, QPoint, QRgb);
    void offsetImage();

    QImage image;
};

#endif // FILTERZONE_H
