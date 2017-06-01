#ifndef WIREFRAME_WIDGET_H
#define WIREFRAME_WIDGET_H

#include <QWidget>

#include "configuration.h"

class WireframeWidget : public QWidget
{
public:
    WireframeWidget(QSharedPointer<Configuration> configuration, QWidget *parent = 0);
};

#endif // WIREFRAME_WIDGET_H
