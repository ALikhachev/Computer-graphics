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

private:
    QImage image;
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
    QImage image;
};

#endif // FILTER_ZONE_H
