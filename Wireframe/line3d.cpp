#include "line3d.h"

Line3D::Line3D(HomogeneousPoint3D from, HomogeneousPoint3D to) :
    _from(from),
    _to(to)
{

}

HomogeneousPoint3D Line3D::from() const
{
    return this->_from;
}

HomogeneousPoint3D Line3D::to() const
{
    return this->_to;
}

QVector3D Line3D::from3D() const
{
    return this->_from.to3D();
}

QVector3D Line3D::to3D() const
{
    return this->_to.to3D();
}

bool Line3D::clip()
{
    this->_from.normalize();
    this->_to.normalize();
    return clipCoordinate(0, -1, 1) && clipCoordinate(1, -1, 1) && clipCoordinate(2, 0, 1);
}

bool Line3D::clipCoordinate(int pos, float min, float max)
{
    float from = _from.to4D()[pos];
    float to = _to.to4D()[pos];
    if ((from < min && to < min)
            || (from > max && to > max)) {
        return false;
    }
    if (from > to) {
        std::swap(_from, _to);
        std::swap(from, to);
    }
    if (from < min) {
        QVector3D diff(_to.to4D().x() - _from.to4D().x(),
                       _to.to4D().y() - _from.to4D().y(),
                       _to.to4D().z() - _from.to4D().z());
        diff *= ((to - min) / (to - from));
        for (int i = 0; i < 3; ++i) {
            _from.to4D()[i] = _to.to4D()[i] - diff[i];
        }
        _from.to4D()[3] = 1;
    }
    from = _from.to4D()[pos];
    if (to > max) {
        QVector3D diff(_to.to4D().x() - _from.to4D().x(),
                       _to.to4D().y() - _from.to4D().y(),
                       _to.to4D().z() - _from.to4D().z());
        diff *= ((max - from) / (to - from));
        for (int i = 0; i < 3; ++i) {
            _to.to4D()[i] = diff[i] + _from.to4D()[i];
        }
        _to.to4D()[3] = 1;
    }
    return true;
}
