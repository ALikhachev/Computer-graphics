#include "line3d.h"

Line3D::Line3D(QVector4D from, QVector4D to) :
    _from(from),
    _to(to)
{

}

QVector4D Line3D::from() const
{
    return this->_from;
}

QVector4D Line3D::to() const
{
    return this->_to;
}

QVector3D Line3D::from3D() const
{
    return this->_from.toVector3D();
}

QVector3D Line3D::to3D() const
{
    return this->_to.toVector3D();
}

bool Line3D::clip()
{
    this->_from = QVector4D(this->_from.x() / this->_from.w(),
                            this->_from.y() / this->_from.w(),
                            this->_from.z() / this->_from.w(),
                            1);
    this->_to = QVector4D(this->_to.x() / this->_to.w(),
                          this->_to.y() / this->_to.w(),
                          this->_to.z() / this->_to.w(),
                          1);
    return clipCoordinate(0, -1, 1) && clipCoordinate(1, -1, 1) && clipCoordinate(2, 0, 1);
}

bool Line3D::clipCoordinate(int pos, float min, float max)
{
    float from = _from[pos];
    float to = _to[pos];
    if ((from < min && to < min)
            || (from > max && to > max)) {
        return false;
    }
    if (from > to) {
        std::swap(_from, _to);
        std::swap(from, to);
    }
    if (from < min) {
        QVector3D diff(_to.x() - _from.x(),
                       _to.y() - _from.y(),
                       _to.z() - _from.z());
        diff *= ((to - min) / (to - from));
        for (int i = 0; i < 3; ++i) {
            _from[i] = _to[i] - diff[i];
        }
        _from[3] = 1;
    }
    from = _from[pos];
    if (to > max) {
        QVector3D diff(_to.x() - _from.x(),
                       _to.y() - _from.y(),
                       _to.z() - _from.z());
        diff *= ((max - from) / (to - from));
        for (int i = 0; i < 3; ++i) {
            _to[i] = diff[i] + _from[i];
        }
        _to[3] = 1;
    }
    return true;
}
