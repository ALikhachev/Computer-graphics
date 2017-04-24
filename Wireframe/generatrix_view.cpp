#include "generatrix_view.h"

#include <QPainter>
#include "drawing.h"

GeneratrixView::GeneratrixView(QWidget *parent) : QWidget(parent),
    knots(std::vector<QPoint>{QPoint(5, 350),
                              QPoint(150, 100),
                              QPoint(250, 50),
                              QPoint(550, 50),
                              QPoint(305, 250)}),
    canvas(this->size(), QImage::Format_RGB32),
    spline(knots)
{
}

void GeneratrixView::resizeEvent(QResizeEvent *)
{
    this->canvas = QImage(this->size(), QImage::Format_RGB32);
    this->plot();
}

void GeneratrixView::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawImage(0, 0, this->canvas);
}

void GeneratrixView::plot()
{
    Drawing::fill(this->canvas, qRgb(255, 255, 255));
    for (uint i = 0; i < this->knots.size(); ++i) {
        Drawing::drawCircle(this->canvas, this->knots[i], 3, qRgb(255, 0, 0));
    }
    for (uint i = 1; i < this->knots.size() - 2; ++i) {
        QPoint from = this->spline.solve(i, 0);
        for (int j = 1; j <= 100; ++j) {
            QPoint to = this->spline.solve(i, (double) j / 100);
            Drawing::drawLine(this->canvas, from, to, qRgb(0, 0, 0));
            from = to;
        }
    }
}
