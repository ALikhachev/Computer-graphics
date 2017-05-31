#ifndef CANVAS3D_H
#define CANVAS3D_H

#include <QWidget>
#include <QSharedPointer>
#include <QImage>
#include <QMouseEvent>
#include <QVector3D>
#include <QPoint>

#include "configuration.h"
#include "wire_object.h"

class Canvas3D : public QWidget
{
    Q_OBJECT
public:
    explicit Canvas3D(QSharedPointer<Configuration> config, QWidget *parent = 0);

protected:
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
//    void wheelEvent(QWheelEvent *event) override;

private:
    void drawObject(WireObject *object, Transform *scale_transform);
    void drawBoundingBox();
    float findAbsMax();
    void plot();

    QSharedPointer<Configuration> _config;
    QImage _image;
    QSharedPointer<Transform> _rotation;
    QSharedPointer<Transform> _perspective;
    QSharedPointer<Transform> _camera;
    QPoint _rotation_tracking;
    constexpr double static const Pi = std::acos(-1);
};

#endif // CANVAS3D_H
