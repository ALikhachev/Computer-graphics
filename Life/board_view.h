#ifndef QCANVAS_H
#define QCANVAS_H

#include <QTimer>
#include <QImage>
#include <QPainter>
#include <QWidget>
#include <QPaintEvent>

#include "board.h"
#include "iboard_view.h"

struct SpanLine {
    int x0;
    int x1;
    int y;
};

class BoardView : public QWidget, public IBoardView
{
    Q_OBJECT
public:
    explicit BoardView(Board *board, QWidget *parent = 0);
    void onCellStateChanged(quint32 x, quint32 y, const Cell &cell) override;

protected:
    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent * event) override;

signals:

public slots:

private:
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
    void drawHexagonOutlines();

    static const QRgb RedColor = qRgb(255, 0, 0);
    static const QRgb GreenColor = qRgb(0, 255, 0);
    static const QRgb BlueColor = qRgb(0, 0, 255);
    static const QRgb WhiteColor = qRgb(255, 255, 255);
    static const QRgb BlackColor = qRgb(0, 0, 0);

    QImage image;
    QTimer timer;
    Board *board;

    uint hex_qrheight;
    uint hex_semiwidth;
    double top_coeff;
};

#endif // QCANVAS_H
