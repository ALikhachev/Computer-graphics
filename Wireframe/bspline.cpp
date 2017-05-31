#include "bspline.h"

#include <QVector4D>

BSpline::BSpline(std::vector<QPointF> values)
    : values(values),
      spline_matrix(-1.0/6.0,  3.0/6.0, -3.0/6.0, 1.0/6.0,
                     3.0/6.0, -6.0/6.0,  3.0/6.0, 0.0/6.0,
                    -3.0/6.0,  0.0/6.0,  3.0/6.0, 0.0/6.0,
                     1.0/6.0,  4.0/6.0,  1.0/6.0, 0.0/6.0),
      vectors_x(values.size()),
      vectors_y(values.size())
{
    this->calculateCoefficientsVectors();
}

QPointF BSpline::solve(double t) const
{
    auto pair = this->getKnotByLength(t);
    return solve(pair.first, pair.second);
}

QPointF BSpline::solve(int knot, double t) const
{
    QPointF point;
    QVector4D t_vect(std::pow(t, 3), std::pow(t, 2), t, 1);
    point.setX(QVector4D::dotProduct(t_vect, this->vectors_x[knot]));
    point.setY(QVector4D::dotProduct(t_vect, this->vectors_y[knot]));
    return point;
}

std::pair<int, float> BSpline::getKnotByLength(float length_percent) const
{
    float length = this->length() * length_percent;

    float len = 0;
    for (uint i = 1; i < this->values.size() - 2; ++i) {
        QPointF prev = solve(i, 0);
        for (float t = 0.001f; t < 1.0f; t += 0.001f) {
            QPointF curr = solve(i, t);
            len += std::sqrt(std::pow(curr.x() - prev.x(), 2) + std::pow(curr.y() - prev.y(), 2));
            if (len > length) {
                return std::make_pair(i, t);
            }
        }
    }
    return std::make_pair(this->values.size() - 2, 0);
}

void BSpline::calculateCoefficientsVectors()
{
    for (uint i = 1; i < this->values.size(); ++i) {
        QVector4D g_x(this->values[i - 1].x(),
                      this->values[i    ].x(),
                      this->values[i + 1].x(),
                      this->values[i + 2].x());
        QVector4D g_y(this->values[i - 1].y(),
                      this->values[i    ].y(),
                      this->values[i + 1].y(),
                      this->values[i + 2].y());
        this->vectors_x[i] = this->spline_matrix * g_x;
        this->vectors_y[i] = this->spline_matrix * g_y;
    }
}

float BSpline::length() const
{
    float len = 0;
    for (uint i = 1; i < this->values.size() - 2; ++i) {
        QPointF prev = solve(i, 0);
        for (float t = 0.001f; t < 1.0f; t += 0.001f) {
            QPointF curr = solve(i, t);
            len += std::sqrt(std::pow(curr.x() - prev.x(), 2) + std::pow(curr.y() - prev.y(), 2));
        }
    }
    return len;
}
