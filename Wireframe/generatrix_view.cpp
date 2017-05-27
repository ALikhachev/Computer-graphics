#include "generatrix_view.h"

#include <QPainter>
#include "drawing.h"

GeneratrixView::GeneratrixView(QSharedPointer<Configuration> config, QWidget *parent) : QWidget(parent),
    canvas(this->size(), QImage::Format_RGB32),
    config(config)
{
    this->setObject(config->objects()[0]);
}

void GeneratrixView::setObject(QSharedPointer<GeneratrixObject> object)
{
    this->_object = object;
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
    Drawing::fill(this->canvas, qRgb(0, 0, 0));
    auto knots = this->_object->knots();
    float scale = this->detectScale(knots);
    this->plotAxes();
    this->plotAxeSegmentation(scale);
    int x_offset = qRound((float) this->width() / 2);
    int y_offset = qRound((float) this->height() / 2);
    scale *= ScaleConst;
    if (knots.size() > 0) {
        QPointF prev = knots[0];
        Drawing::drawCircle(this->canvas, QPoint(scale * prev.x() + x_offset, scale * prev.y() + y_offset), 3, qRgb(255, 255, 0));
        for (auto it = knots.begin() + 1; it < knots.end(); ++it) {
            Drawing::drawCircle(this->canvas, QPoint(scale * it->x() + x_offset, scale * it->y() + y_offset), 3, qRgb(255, 255, 0));
            Drawing::drawLine(this->canvas,
                              QPoint(scale * prev.x() + x_offset, scale * prev.y() + y_offset),
                              QPoint(scale * it->x() + x_offset, scale * it->y() + y_offset),
                              qRgb(255, 255, 0));
            prev = *it;
        }
    }
    auto segments = this->_object->getSegments2D();
    for (auto it = segments.begin(); it < segments.end(); ++it) {
        Drawing::drawLine(this->canvas,
                          QPoint(scale * it->first.x() + x_offset, scale * it->first.y() + y_offset),
                          QPoint(scale * it->second.x() + x_offset, scale * it->second.y() + y_offset), this->_object->color());
    }
}

void GeneratrixView::plotAxes()
{
    int x_offset = qRound((float) this->width() / 2);
    int y_offset = qRound((float) this->height() / 2);
    Drawing::drawLine(this->canvas, QPoint(x_offset, 0), QPoint(x_offset, this->height() - 1), qRgb(127, 127, 127));
    Drawing::drawLine(this->canvas, QPoint(0, y_offset), QPoint(this->width() - 1, y_offset), qRgb(127, 127, 127));
}

void GeneratrixView::plotAxeSegmentation(float scale)
{
    int x_offset = qRound((float) this->width() / 2);
    int y_offset = qRound((float) this->height() / 2);
    int x_count = x_offset / scale;
    int y_count = y_offset / scale;
    for (int i = 0; i < x_count; ++i) {
        Drawing::drawLine(this->canvas,
                          QPoint(x_offset + i * scale, y_offset - AxeSegmentWidth),
                          QPoint(x_offset + i * scale, y_offset + AxeSegmentWidth),
                          qRgb(127, 127, 127));
        Drawing::drawLine(this->canvas,
                          QPoint(x_offset - i * scale, y_offset - AxeSegmentWidth),
                          QPoint(x_offset - i * scale, y_offset + AxeSegmentWidth),
                          qRgb(127, 127, 127));
    }
    for (int i = 0; i < y_count; ++i) {
        Drawing::drawLine(this->canvas,
                          QPoint(x_offset - AxeSegmentWidth, y_offset + i * scale),
                          QPoint(x_offset + AxeSegmentWidth, y_offset + i * scale),
                          qRgb(127, 127, 127));
        Drawing::drawLine(this->canvas,
                          QPoint(x_offset - AxeSegmentWidth, y_offset - i * scale),
                          QPoint(x_offset + AxeSegmentWidth, y_offset - i * scale),
                          qRgb(127, 127, 127));
    }
}

float GeneratrixView::detectScale(std::vector<QPointF> &knots)
{
    float x_max = std::max_element(knots.begin(), knots.end(), [] (const QPointF &v1, const QPointF &v2) {
        return std::fabs(v1.x()) < std::fabs(v2.x());
    })->x();
    float y_max = std::max_element(knots.begin(), knots.end(), [] (const QPointF &v1, const QPointF &v2) {
        return std::fabs(v1.y()) < std::fabs(v2.y());
    })->y();
    if (x_max == 0) x_max = 1;
    if (y_max == 0) y_max = 1;
    float scale_divider_x = (float) this->width() / (x_max * 2);
    float scale_divider_y = (float) this->height() / (y_max * 2);
    return std::min(scale_divider_x, scale_divider_y);
}
