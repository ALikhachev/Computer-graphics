#include "configuration.h"

const static double PI = 3.141592653589793238463;

Configuration::Configuration() :
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
                                                  qRgb(255, 0, 0), QVector3D(10, 0, 0),
                                                                   {QPointF(-2.775 , -1.3),
                                                                    QPointF(-1.7375, -0.175),
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
