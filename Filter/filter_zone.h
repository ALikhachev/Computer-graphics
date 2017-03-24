#ifndef FILTER_ZONE_H
#define FILTER_ZONE_H

#include <QWidget>
#include <QMouseEvent>

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

    static const int RgbaDepth = 4;
private:
    void drawBorder();

    QImage image;
    QImage canvas;

    static const int BorderDashLength = 3;
};

struct Selection {
    int x;
    int y;
    int width;
    int height;
    bool empty;
};

class SourceZone : public QWidget
{
    Q_OBJECT
public:
    explicit SourceZone(QWidget *parent = 0);
    void setSourceImage(QImage);
    void clear();
signals:
    void zoneSelected(QImage);
protected:
    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
private:
    Selection selection;
    QImage source_image;
    QImage canvas;
    bool clean;
    int scaled_width;
    int scaled_height;
};

#endif // FILTER_ZONE_H
