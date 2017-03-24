#ifndef FILTER_ZONE_H
#define FILTER_ZONE_H

#include <QWidget>
#include <QMouseEvent>

class FilterZone : public QWidget
{
    Q_OBJECT
public:
    explicit FilterZone(QWidget *parent = 0);

    virtual QImage getImage();
    virtual void setImage(QImage);
    virtual void clear();

protected:
    void paintEvent(QPaintEvent *) override;

    QImage image;
    QImage canvas;
    bool has_image;
private:
    void drawBorder();
};

struct Selection {
    int x;
    int y;
    int width;
    int height;
    bool empty;
};

class SourceZone : public FilterZone
{
    Q_OBJECT
public:
    explicit SourceZone(QWidget *parent = 0);
    void setImage(QImage) override;
    void clear() override;

signals:
    void zoneSelected(QImage);

protected:
    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;

private:
    void saveCanvas();
    void restoreCanvas();

    Selection selection;
    int scaled_width;
    int scaled_height;
    QImage canvas_without_selection;
};

#endif // FILTER_ZONE_H
