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

std::vector<Line3D> GeneratrixObject::getObjectSegments() const
{
    std::vector<Line3D> segments;
    auto segments_2d = this->getSegments2D();
    for (auto &segment_2d : segments_2d) {

    }
}

std::vector<std::pair<QPointF, QPointF>> GeneratrixObject::getSegments2D() const
{
    uint count = 100;
    std::vector<std::pair<QPointF, QPointF>> segments;
    for (uint i = 1; i < this->_knots.size() - 2; ++i) {
        QPointF from = this->_spline.solve(i, 0);
        for (uint j = 1; j < count; ++j) {
            QPointF to = this->_spline.solve(i, (double) j / count);
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
