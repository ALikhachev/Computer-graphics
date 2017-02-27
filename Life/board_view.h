#ifndef QCANVAS_H
#define QCANVAS_H

#include <QTimer>
#include <QImage>
#include <QPainter>
#include <QWidget>
#include <QPaintEvent>

#include "board.h"

struct SpanLine {
    int x0;
    int x1;
    int y;
};

class BoardView : public QWidget
{
    Q_OBJECT
public:
    explicit BoardView(Board *board, QWidget *parent = 0);

    void toggleEditing(bool allow);

protected:
    void paintEvent(QPaintEvent *) override;
    void resizeEvent(QResizeEvent *) override;
    void mousePressEvent(QMouseEvent * event) override;
    void mouseReleaseEvent(QMouseEvent * event) override;
    void mouseMoveEvent(QMouseEvent * event) override;

signals:

public slots:

private:
    void paint(QPainter &painter);
    void drawLine(QPoint from, QPoint to, QRgb color);
    void drawHexagon(QPoint start);
    bool tryDrawSimpleLine(QPoint &from, QPoint &to, QRgb color);
    void clipLine(QPoint &from, QPoint &to);
    void drawLineBresenham(QPoint &from, QPoint &to, QRgb color);
    void drawLineBresenhamY(QPoint &from, QPoint &to, QRgb color);
    void drawHorizontalLine(QPoint &from, QPoint &to, QRgb color);
    void drawVerticalLine(QPoint &from, QPoint &to, QRgb color);
    void fill(QRgb color);
    void spanFill(QPoint start, QRgb color);
    void getSpans(int min_x, int max_x, int y, QRgb *pixels, QRgb oldValue, std::vector<SpanLine> &spans);

    static const QRgb RedColor = qRgb(255, 0, 0);
    static const QRgb GreenColor = qRgb(0, 255, 0);
    static const QRgb BlueColor = qRgb(0, 0, 255);
    static const QRgb WhiteColor = qRgb(255, 255, 255);
    static const QRgb BlackColor = qRgb(0, 0, 0);
    static const QRgb GrayBackground = qRgb(240, 240, 240);
    static const QRgb AliveCellColor = qRgb(16, 202, 90);
    static const QRgb DeadCellColor = WhiteColor;

    QImage image;
    QTimer timer;
    Board *board;

    int hex_qrheight;
    int hex_semiwidth;
    double top_coeff;
    std::pair<int, int> lastChangedCell;
    bool editIsAllowed;
};

#endif // QCANVAS_H
