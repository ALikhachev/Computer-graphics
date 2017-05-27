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
    auto segments_2d = this->getSegments2D();
    float rotate_step = (_d - _c) / (_m);
    int t = 0;
    int len = segments_2d.size();
    for (auto &segment_2d : segments_2d) {
        for (float i = _c; i <= _d; i += rotate_step) {
            if (i + rotate_step <= _d && t % _k == 0) {
                HomogeneousPoint3D start_rotate(
                                segment_2d.first.y() * std::cos(i),
                                segment_2d.first.y() * std::sin(i),
                                segment_2d.first.x()
                            );
                for (int j = 1; j <= _k; ++j) {
                    HomogeneousPoint3D end_rotate(
                                    segment_2d.first.y() * std::cos(i + j * rotate_step / _k),
                                    segment_2d.first.y() * std::sin(i + j * rotate_step / _k),
                                    segment_2d.first.x()
                                );
                    this->_segments.push_back(Line3D(start_rotate, end_rotate));
                    start_rotate = end_rotate;
                }
            }
            HomogeneousPoint3D start(
                            segment_2d.first.y() * std::cos(i),
                            segment_2d.first.y() * std::sin(i),
                            segment_2d.first.x()
                        );
            HomogeneousPoint3D end_long(
                            segment_2d.second.y() * std::cos(i),
                            segment_2d.second.y() * std::sin(i),
                            segment_2d.second.x()
                        );
            this->_segments.push_back(Line3D(start, end_long));
            if (i + rotate_step <= _d && t == len - 1) {
                HomogeneousPoint3D start_rotate(
                                segment_2d.second.y() * std::cos(i),
                                segment_2d.second.y() * std::sin(i),
                                segment_2d.second.x()
                            );
                for (int j = 1; j <= _k; ++j) {
                    HomogeneousPoint3D end_rotate(
                                    segment_2d.second.y() * std::cos(i + j * rotate_step / _k),
                                    segment_2d.second.y() * std::sin(i + j * rotate_step / _k),
                                    segment_2d.second.x()
                                );
                    this->_segments.push_back(Line3D(start_rotate, end_rotate));
                    start_rotate = end_rotate;
                }
            }
        }
        t++;
    }
}

std::vector<std::pair<QPointF, QPointF>> GeneratrixObject::getSegments2D() const
{
    std::vector<std::pair<QPointF, QPointF>> segments;
    uint count = _n * _k;
    for (uint i = 1; i < this->_knots.size() - 2; ++i) {
        QPointF from = this->_spline.solve(i, 0);
        for (uint j = 1; j <= count; ++j) {
            QPointF to = this->_spline.solve(i, (float) j / count);
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
