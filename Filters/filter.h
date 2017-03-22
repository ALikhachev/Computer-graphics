#ifndef FILTER_H
#define FILTER_H

#include <QWidget>

#include "filter_parameters_widget.h"

class Filter
{
public:
    virtual QImage applyFilter(QImage) = 0;
    virtual FilterParametersWidget *getParametersWidget(QWidget *parent = 0) = 0;
};

#endif // FILTER_H
