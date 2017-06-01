#include "generatrix_view.h"

#include <QPainter>
#include "drawing.h"

GeneratrixView::GeneratrixView(QSharedPointer<Configuration> config, QWidget *parent) : QWidget(parent),
    canvas(this->size(), QImage::Format_RGB32),
    config(config)
{
    this->setObject(config->objects()[0]);
    connect(this->config.data(), &Configuration::objectSelected, this, [this] (int index) {
        this->setObject(this->config->objects()[index]);
        this->plot();
        this->update();
    });
    connect(this->config.data(), &Configuration::lengthChanged, this, [this] () {
        this->plot();
        this->update();
    });
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
    const BSpline &spline = this->_object->getBSpline();
    float a = this->config->a();
    float b = this->config->b();
    int n = this->config->n();
    int k = this->config->k();
    float step = 1.0 / (n * k);
    QPointF from(spline.solve(0));
    for (float l = 0 + step; l < a; l += step) {
        QPointF to(spline.solve(l));
        Drawing::drawLine(this->canvas,
                          QPoint(scale * from.x() + x_offset, scale * from.y() + y_offset),
                          QPoint(scale * to.x() + x_offset, scale * to.y() + y_offset),
                          qRgb(100, 100, 100));
        from = to;
    }
    for (float l = a + step; l < b; l += step) {
        QPointF to(spline.solve(l));
        Drawing::drawLine(this->canvas,
                          QPoint(scale * from.x() + x_offset, scale * from.y() + y_offset),
                          QPoint(scale * to.x() + x_offset, scale * to.y() + y_offset),
                          qRgb(255, 0, 0));
        from = to;
    }
    for (float l = b + step; l <= 1; l += step) {
        QPointF to(spline.solve(l));
        Drawing::drawLine(this->canvas,
                          QPoint(scale * from.x() + x_offset, scale * from.y() + y_offset),
                          QPoint(scale * to.x() + x_offset, scale * to.y() + y_offset),
                          qRgb(100, 100, 100));
        from = to;
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
    return std::max(scale_divider_x, scale_divider_y);
}
