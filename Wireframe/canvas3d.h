#ifndef CANVAS3D_H
#define CANVAS3D_H

#include <QWidget>
#include <QSharedPointer>
#include <QImage>

#include "configuration.h"

class Canvas3D : public QWidget
{
    Q_OBJECT
public:
    explicit Canvas3D(QSharedPointer<Configuration> config, QWidget *parent = 0);

protected:
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *) override;

private:
    QSharedPointer<Configuration> _config;
    QImage _image;
};

#endif // CANVAS3D_H
