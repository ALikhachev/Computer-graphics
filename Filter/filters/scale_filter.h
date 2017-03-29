#ifndef SCALE_FILTER_H
#define SCALE_FILTER_H

#include "filter.h"

class ScaleFilterSettings : public FilterSettings {
public:
    float scale_factor = 2;
};

class ScaleFilter : public Filter
{
public:
    ScaleFilter();
    QImage applyFilter(QImage, std::function<void(int)>);
    QIcon getIcon();
    QString getName();
};

#endif // SCALE_FILTER_H
