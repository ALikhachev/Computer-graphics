#ifndef FILTER_ZONE_H
#define FILTER_ZONE_H

#include <QWidget>

class FilterZone : public QWidget
{
    Q_OBJECT
public:
    explicit FilterZone(QWidget *parent = 0);

    QImage getImage();
    void setImage(QImage);
    void clear();

protected:
    void paintEvent(QPaintEvent *) override;

private:
    static void emptyImage(QImage &);

    QImage image;
    int depth_bytes;

    const int DashPeriods = 176;
};

#endif // FILTER_ZONE_H
