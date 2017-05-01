#include "configuration.h"

const static double PI = 3.141592653589793238463;

Configuration::Configuration() :
    _grid_width(10),
    _grid_height(10),
    _segments_count(5),
    _domain_t_min(0),
    _domain_t_max(1),
    _domain_phi_min(0),
    _domain_phi_max(2 * PI),
    _clipping_near(5),
    _clipping_far(20),
    _clipping_rect_width(25),
    _clipping_rect_height(30),
    _background_color(qRgb(255, 255, 255)),
    _objects({
             QSharedPointer<GeneratrixObject>(new GeneratrixObject(qRgb(255, 0, 0), {QPoint(5, 35),
                                                                   QPoint(25, 10),
                                                                   QPoint(20, 0),
                                                                   QPoint(25, 50),
                                                                   QPoint(35, 50),
                                                                   QPoint(35, 25)}))
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

int Configuration::clippingNearDistance() const
{
    return this->_clipping_near;
}

int Configuration::clippingFarDistance() const
{
    return this->_clipping_far;
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
