#ifndef FILTERZONE_H
#define FILTERZONE_H

#include <QWidget>

class FilterZone : public QWidget
{
    Q_OBJECT
public:
    explicit FilterZone(QWidget *parent = 0);

    void setImage(QImage &);
    void clear();

protected:
    void paintEvent(QPaintEvent *) override;

private:
    void drawLine(QPoint, QPoint, QRgb);
    void offsetImage();
    void drawBorder();
    static void emptyImage(QImage &);

    QImage image;
    int depth_bytes;

    const int DashPeriods = 176;
};

#endif // FILTERZONE_H
