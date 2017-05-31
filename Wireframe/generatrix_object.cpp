#include "generatrix_object.h"

GeneratrixObject::GeneratrixObject(int n, int m, int k,
                                   float a, float b, float c, float d,
        QRgb color, std::vector<QPointF> knots) : WireObject(),
    _color(color),
    _knots(knots),
    _spline(knots),
    _a(a),
    _b(b),
    _c(c),
    _d(d),
    _n(n),
    _m(m),
    _k(k)
{
    this->buildSegments();
}

std::vector<QPointF> GeneratrixObject::knots() const
{
    return this->_knots;
}

const std::vector<Line3D> &GeneratrixObject::getSegments() const
{
    return _segments;
}

void GeneratrixObject::buildSegments()
{
    float long_step = (_b - _a) / (_n);
    float rotate_step = (_d - _c) / (_m);
    for (float l = _a; l < _b; l += long_step) {
        for (float i = _c; i <= _d; i += rotate_step) {
            QPointF point = this->_spline.solve(l);
            if (i + rotate_step <= _d + 0.001f) {
                HomogeneousPoint3D start_rotate(
                                point.y() * std::cos(i),
                                point.y() * std::sin(i),
                                point.x()
                            );
                for (int j = 1; j <= _k; ++j) {
                    HomogeneousPoint3D end_rotate(
                                    point.y() * std::cos(i + j * rotate_step / _k),
                                    point.y() * std::sin(i + j * rotate_step / _k),
                                    point.x()
                                );
                    this->_segments.push_back(Line3D(start_rotate, end_rotate));
                    start_rotate = end_rotate;
                }
            }
            if (l + long_step <= _b) {
                HomogeneousPoint3D start_long(
                                point.y() * std::cos(i),
                                point.y() * std::sin(i),
                                point.x()
                            );

                for (int j = 1; j <= _k; ++j) {
                    point = this->_spline.solve(l + j * long_step / _k);
                    HomogeneousPoint3D end_long(
                                    point.y() * std::cos(i),
                                    point.y() * std::sin(i),
                                    point.x()
                                );
                    this->_segments.push_back(Line3D(start_long, end_long));
                    start_long = end_long;
                }
            }
        }
    }
}

std::vector<std::pair<QPointF, QPointF>> GeneratrixObject::getSegments2D() const
{
    std::vector<std::pair<QPointF, QPointF>> segments;
    uint count = _n * _k;
    QPointF from = this->_spline.solve(0);
    for (uint j = 1; j <= count; ++j) {
        QPointF to = this->_spline.solve((float) j / count);
        segments.push_back(std::make_pair(from, to));
        from = to;
    }
    return segments;
}

QRgb GeneratrixObject::color() const
{
    return this->_color;
}
