#include "configuration.h"

Configuration::Configuration() :
    _sw(5),
    _sh(5),
    _a(0),
    _b(0.75),
    _c(0),
    _d(2 * PI),
    _n(10),
    _m(10),
    _k(10),
    _current_object(0),
    _clipping_near(5),
    _clipping_far(20),
    _background_color(qRgb(255, 255, 255)),
    _rotation_transform(new IdentityTransform),
    _objects({
             QSharedPointer<GeneratrixObject>(new GeneratrixObject(
                                                  _n,
                                                  _m,
                                                  _k,
                                                  _a,
                                                  _b,
                                                  _c,
                                                  _d,
                                                  qRgb(255, 0, 0), QVector3D(1, 0, 0),
                                                                   {QPointF(-2.775 , -1.3),
                                                                    QPointF(-1.7375, -0.175),
                                                                    QPointF(0.9875 , -0.2625),
                                                                    QPointF(1.6    , -1.575),
                                                                    QPointF(0.5    , -1.7125)
                                                                    })),
             QSharedPointer<GeneratrixObject>(new GeneratrixObject(
                                                  _n,
                                                  _m,
                                                  _k,
                                                  _a,
                                                  _b,
                                                  _c,
                                                  _d,
                                                  qRgb(0, 0, 255), QVector3D(-5, 0, 0),
                                                                   {QPointF(-2.775 , -1.3),
                                                                    QPointF(-5.7375, 10.175),
                                                                    QPointF(0.9875 , -0.2625),
                                                                    QPointF(1.6    , -1.575),
                                                                    QPointF(0.5    , -1.7125)
                                                                    }))
             })
{

}

bool Configuration::load(QTextStream &stream)
{
    int n, m, k;
    float a, b, c, d;
    stream >> n >> m >> k >> a >> b >> c >> d;
    if (n <= 0 || m <= 0 || k <= 0 || a >= b || c >= d) {
        return false;
    }
    stream.readLine();
    float zn, zf, sw, sh;
    stream >> zn >> zf >> sw >> sh;
    if (zn >= zf || zn <= 0 || sw <= 0 || sh <= 0) {
        return false;
    }
    stream.readLine();
    float a1, a2, a3, a4, a5, a6, a7, a8, a9;
    stream >> a1 >> a2 >> a3;
    stream.readLine();
    stream >> a4 >> a5 >> a6;
    stream.readLine();
    stream >> a7 >> a8 >> a9;
    stream.readLine();
    int red, green, blue;
    stream >> red >> green >> blue;
    if (red < 0 || red > 255 || green < 0 || green > 255 || blue < 0 || blue > 255) {
        return false;
    }
    stream.readLine();
    int count;
    stream >> count;
    stream.readLine();
    if (count <= 0) {
        return false;
    }
    std::vector<QSharedPointer<GeneratrixObject>> objects;
    while (count-- > 0) {
        QSharedPointer<GeneratrixObject> object = parseObject(n, m, k, a, b, c, d, stream);
        if (object.data() == nullptr) {
            return false;
        }
        objects.push_back(object);
    }
    this->_n = n;
    this->_m = m;
    this->_k = k;
    this->_a = a;
    this->_b = b;
    this->_c = c;
    this->_d = d;
    this->_clipping_near = zn;
    this->_clipping_far = zf;
    this->_sw = sw;
    this->_sh = sh;
    this->_background_color = qRgb(red, green, blue);
    this->setRotationTransform(QSharedPointer<Transform>(new MatrixTransform(a1, a2, a3, a4, a5, a6, a7, a8, a9)));
    this->_objects = objects;
    emit updated();
    setCurrentObject(0);
    emit lengthChanged();
    return true;
}

QSharedPointer<GeneratrixObject> Configuration::parseObject(int n, int m, int k, float a, float b, float c, float d, QTextStream &stream)
{
    int red, green, blue;
    stream >> red >> green >> blue;
    if (red < 0 || red > 255 || green < 0 || green > 255 || blue < 0 || blue > 255) {
        return QSharedPointer<GeneratrixObject>(nullptr);
    }
    stream.readLine();
    float cx, cy, cz;
    stream >> cx >> cy >> cz;
    stream.readLine();
    float a1, a2, a3, a4, a5, a6, a7, a8, a9;
    stream >> a1 >> a2 >> a3;
    stream.readLine();
    stream >> a4 >> a5 >> a6;
    stream.readLine();
    stream >> a7 >> a8 >> a9;
    stream.readLine();
    int count;
    stream >> count;
    stream.readLine();
    if (count <= 0) {
       return QSharedPointer<GeneratrixObject>(nullptr);
    }
    std::vector<QPointF> knots;
    while (count-- > 0) {
        float x, y;
        stream >> x >> y;
        stream.readLine();
        knots.push_back(QPointF(x, y));
    }
    return QSharedPointer<GeneratrixObject>(new GeneratrixObject(n, m, k, a, b, c, d, qRgb(red, green, blue), knots));
}

int Configuration::currentObject() const
{
    return _current_object;
}

void Configuration::setCurrentObject(int value)
{
    this->_current_object = value;
    emit objectSelected(value);
}

float Configuration::clippingNearDistance() const
{
    return this->_clipping_near;
}

void Configuration::setClippingNearDistance(float val)
{
    this->_clipping_near = val;
}

float Configuration::clippingFarDistance() const
{
    return this->_clipping_far;
}

void Configuration::setClippingFarDistance(float val)
{
    this->_clipping_far = val;
}

QRgb Configuration::backgroundColor() const
{
    return this->_background_color;
}

const std::vector<QSharedPointer<GeneratrixObject> > Configuration::objects() const
{
    return this->_objects;
}

void Configuration::update()
{
    emit updated();
}

void Configuration::setCurrentX(float x)
{
    QVector3D center = this->_objects[this->_current_object]->getCenter();
    center.setX(x);
    this->_objects[this->_current_object]->setCenter(center);
}

float Configuration::currentX() const
{
    return this->_objects[this->_current_object]->getCenter().x();
}

void Configuration::setCurrentY(float x)
{
    QVector3D center = this->_objects[this->_current_object]->getCenter();
    center.setY(x);
    this->_objects[this->_current_object]->setCenter(center);
}

float Configuration::currentY() const
{
    return this->_objects[this->_current_object]->getCenter().y();
}

void Configuration::setCurrentZ(float x)
{
    QVector3D center = this->_objects[this->_current_object]->getCenter();
    center.setZ(x);
    this->_objects[this->_current_object]->setCenter(center);
}

float Configuration::currentZ() const
{
    return this->_objects[this->_current_object]->getCenter().z();
}

void Configuration::rotateCurrentObject(QSharedPointer<Transform> transform)
{
    QSharedPointer<Transform> rotation = this->_objects[this->_current_object]->getRotation();
    this->_objects[this->_current_object]->setRotation(rotation->compose(transform.data()));
}

int Configuration::n() const
{
    return _n;
}

void Configuration::setN(int value)
{
    _n = value;
    for (auto &object : _objects) {
        object->setN(value);
    }
}

int Configuration::m() const
{
    return _n;
}

void Configuration::setM(int value)
{
    _m = value;
    for (auto &object : _objects) {
        object->setM(value);
    }
}

int Configuration::k() const
{
    return _k;
}

void Configuration::setK(int value)
{
    _k = value;
    for (auto &object : _objects) {
        object->setK(value);
    }
}

float Configuration::a() const
{
    return _a;
}

void Configuration::setA(float value)
{
    _a = value;
    for (auto &object : _objects) {
        object->setA(value);
    }
    emit lengthChanged();
}

float Configuration::b() const
{
    return _b;
}

void Configuration::setB(float value)
{
    _b = value;
    for (auto &object : _objects) {
        object->setB(value);
    }
    emit lengthChanged();
}


float Configuration::c() const
{
    return _c;
}

void Configuration::setC(float value)
{
    _c = value;
    for (auto &object : _objects) {
        object->setC(value);
    }
}


float Configuration::d() const
{
    return _d;
}

void Configuration::setD(float value)
{
    _d = value;
    for (auto &object : _objects) {
        object->setD(value);
    }
}

float Configuration::sw() const
{
    return _sw;
}

void Configuration::setSw(float value)
{
    _sw = value;
}

float Configuration::sh() const
{
    return _sh;
}

void Configuration::setSh(float value)
{
    _sh = value;
}

QSharedPointer<Transform> Configuration::rotationTransform() const
{
    return this->_rotation_transform;
}

void Configuration::setRotationTransform(QSharedPointer<Transform> transform)
{
    this->_rotation_transform = transform;
}
