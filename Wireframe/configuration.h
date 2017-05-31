#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QObject>
#include <QRgb>
#include <QMatrix3x3>
#include <QSharedPointer>

#include "generatrix_object.h"

class Configuration : public QObject
{
    Q_OBJECT
public:
    Configuration();

    bool load(QTextStream &stream);

    int currentObject() const;
    void setCurrentObject(int value);
    float clippingNearDistance() const;
    void setClippingNearDistance(float val);
    float clippingFarDistance() const;
    void setClippingFarDistance(float val);
    int clippingRectWidth() const;
    int clippingRectHeight() const;
    QRgb backgroundColor() const;
    const QMatrix3x3 &rotationComposedMatrix() const;
    const std::vector<QSharedPointer<GeneratrixObject>> objects() const;
    void update();
    void setCurrentX(float x);
    float currentX() const;
    void setCurrentY(float y);
    float currentY() const;
    void setCurrentZ(float z);
    float currentZ() const;
    void rotateCurrentObject(QSharedPointer<Transform> transform);

signals:
    void updated();
    void objectSelected(int index);

private:
    int _current_object;
    float _clipping_near;
    float _clipping_far;
    int _clipping_rect_width;
    int _clipping_rect_height;
    QRgb _background_color;
    QMatrix3x3 _rotation_composed_matrix;
    std::vector<QSharedPointer<GeneratrixObject>> _objects;
};

#endif // CONFIGURATION_H
