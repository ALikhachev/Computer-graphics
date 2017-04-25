#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QObject>
#include <QRgb>
#include <QMatrix3x3>
#include <QSharedPointer>

#include "generatrix_object.h"

class Configuration
{
public:
    Configuration();

    bool load(QTextStream &stream);

    int gridWidth() const;
    int gridHeight() const;
    int segmentsCount() const;
    double domainTMin() const;
    double domainTMax() const;
    double domainPhiMin() const;
    double domainPhiMax() const;
    int clippingNearDistance() const;
    int clippingFarDistance() const;
    int clippingRectWidth() const;
    int clippingRectHeight() const;
    QRgb backgroundColor() const;
    const QMatrix3x3 &rotationComposedMatrix() const;
    const std::vector<QSharedPointer<GeneratrixObject>> objects() const;

private:
    int _grid_width;
    int _grid_height;
    int _segments_count;
    double _domain_t_min;
    double _domain_t_max;
    double _domain_phi_min;
    double _domain_phi_max;
    int _clipping_near;
    int _clipping_far;
    int _clipping_rect_width;
    int _clipping_rect_height;
    QRgb _background_color;
    QMatrix3x3 _rotation_composed_matrix;
    std::vector<QSharedPointer<GeneratrixObject>> _objects;
};

#endif // CONFIGURATION_H
