#include "generatrix_object.h"

GeneratrixObject::GeneratrixObject(QRgb color, std::vector<QPoint> knots) :
    _color(color),
    _knots(knots),
    _spline(knots)
{

}

std::vector<QPoint> GeneratrixObject::knots() const
{
    return this->_knots;
}

std::vector<std::pair<QPointF, QPointF>> GeneratrixObject::getSegments() const
{
    std::vector<std::pair<QPointF, QPointF>> segments;
    for (uint i = 1; i < this->_knots.size() - 2; ++i) {
        QPointF from = this->_spline.solve(i, 0);
        for (int j = 1; j <= 100; ++j) {
            QPointF to = this->_spline.solve(i, (double) j / 100);
            segments.push_back(std::make_pair(from, to));
            from = to;
        }
    }
    return segments;
}

QRgb GeneratrixObject::color() const
{
    return this->_color;
}
