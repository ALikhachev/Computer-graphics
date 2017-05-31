#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QObject>
#include <QRgb>
#include <QMatrix3x3>
#include <QSharedPointer>

#include "generatrix_object.h"

const static double PI = 3.141592653589793238463;

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
    int n() const;
    void setN(int value);
    int m() const;
    void setM(int value);
    int k() const;
    void setK(int value);
    float a() const;
    void setA(float value);
    float b() const;
    void setB(float value);
    float c() const;
    void setC(float value);
    float d() const;
    void setD(float value);
    float sw() const;
    void setSw(float value);
    float sh() const;
    void setSh(float value);

signals:
    void updated();
    void objectSelected(int index);

private:
    float _sw;
    float _sh;
    float _a;
    float _b;
    float _c;
    float _d;
    int _n;
    int _m;
    int _k;
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
