#include "configuration.h"

const static double PI = 3.141592653589793238463;

Configuration::Configuration() :
    _grid_width(5),
    _grid_height(7),
    _segments_count(3),
    _domain_t_min(0.5),
    _domain_t_max(1),
    _domain_phi_min(0),
    _domain_phi_max(1 * PI),
    _clipping_near(0),
    _clipping_far(15),
    _clipping_rect_width(10),
    _clipping_rect_height(10),
    _background_color(qRgb(255, 255, 255)),
    _objects({
             QSharedPointer<GeneratrixObject>(new GeneratrixObject(
                                                  this->_grid_height,
                                                  this->_grid_width,
                                                  this->_segments_count,
                                                  this->_domain_t_min,
                                                  this->_domain_t_max,
                                                  this->_domain_phi_min,
                                                  this->_domain_phi_max,
                                                  qRgb(255, 0, 0), {QPointF(0.059085, 0.169869),
                                                                    QPointF(0.000000, 0.155098),
                                                                    QPointF(0.310196, -1.129998),
                                                                    QPointF(1.94980, -0.745947)
                                                                    }))
             })
{

}

bool Configuration::load(QTextStream &stream)
{
    return false;
}

int Configuration::gridWidth() const
{
    return this->_grid_width;
}

int Configuration::gridHeight() const
{
    return this->_grid_height;
}

int Configuration::segmentsCount() const
{
    return this->_segments_count;
}

double Configuration::domainTMin() const
{
    return this->_domain_t_min;
}

double Configuration::domainTMax() const
{
    return this->_domain_t_max;
}

double Configuration::domainPhiMin() const
{
    return this->_domain_phi_min;
}

double Configuration::domainPhiMax() const
{
    return this->_domain_phi_max;
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
