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
    Drawing::fill(this->canvas, qRgb(255, 255, 255));
    auto knots = this->_object->knots();
    for (auto it = knots.begin(); it < knots.end(); ++it) {
        Drawing::drawCircle(this->canvas, *it, 3, qRgb(127, 127, 0));
    }
    auto segments = this->_object->getSegments();
    for (auto it = segments.begin(); it < segments.end(); ++it) {
        Drawing::drawLine(this->canvas, it->first, it->second, this->_object->color());
    }
}
