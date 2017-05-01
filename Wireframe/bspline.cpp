#include "bspline.h"

#include <QVector4D>

BSpline::BSpline(std::vector<QPoint> values)
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

QPointF BSpline::solve(int knot, double t) const
{
    QPointF point;
    QVector4D t_vect(std::pow(t, 3), std::pow(t, 2), t, 1);
    point.setX(QVector4D::dotProduct(t_vect, this->vectors_x[knot]));
    point.setY(QVector4D::dotProduct(t_vect, this->vectors_y[knot]));
    return point;
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
