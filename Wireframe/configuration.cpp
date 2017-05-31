#include "configuration.h"

const static double PI = 3.141592653589793238463;

Configuration::Configuration() :
    _current_object(0),
    _clipping_near(5),
    _clipping_far(20),
    _clipping_rect_width(10),
    _clipping_rect_height(10),
    _background_color(qRgb(255, 255, 255)),
    _objects({
             QSharedPointer<GeneratrixObject>(new GeneratrixObject(
                                                  10,
                                                  10,
                                                  10,
                                                  0,
                                                  1,
                                                  0,
                                                  2 * PI,
                                                  qRgb(255, 0, 0), QVector3D(1, 0, 0),
                                                                   {QPointF(-2.775 , -1.3),
                                                                    QPointF(-1.7375, -0.175),
                                                                    QPointF(0.9875 , -0.2625),
                                                                    QPointF(1.6    , -1.575),
                                                                    QPointF(0.5    , -1.7125)
                                                                    })),
             QSharedPointer<GeneratrixObject>(new GeneratrixObject(
                                                  10,
                                                  10,
                                                  10,
                                                  0.3,
                                                  0.7,
                                                  0,
                                                  1 * PI,
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
    return false;
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

int Configuration::clippingRectWidth() const
{
    return this->_clipping_rect_width;
}

int Configuration::clippingRectHeight() const
{
    return this->_clipping_rect_height;
}

QRgb Configuration::backgroundColor() const
{
    return this->_background_color;
}

const QMatrix3x3 &Configuration::rotationComposedMatrix() const
{
    return this->_rotation_composed_matrix;
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
