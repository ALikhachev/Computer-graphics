#ifndef CANVAS3D_H
#define CANVAS3D_H

#include <QWidget>
#include <QSharedPointer>

#include "configuration.h"

class Canvas3D : public QWidget
{
    Q_OBJECT
public:
    explicit Canvas3D(QSharedPointer<Configuration> config, QWidget *parent = 0);

private:
    QSharedPointer<Configuration> config;
};

#endif // CANVAS3D_H
