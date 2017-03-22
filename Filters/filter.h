#ifndef FILTER_H
#define FILTER_H

#include <QWidget>
#include <QImage>

#include "filter_parameters_widget.h"

class Filter
{
public:
    virtual QImage applyFilter(QImage &) = 0;
    virtual FilterParametersWidget *getParametersWidget(QWidget *parent = 0) = 0;
};

class GrayscaleFilter : public Filter
{
public:
    GrayscaleFilter();
    QImage applyFilter(QImage &);
    FilterParametersWidget *getParametersWidget(QWidget *parent = 0);
};

#endif // FILTER_H
