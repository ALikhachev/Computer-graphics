#ifndef GAMMA_CORRECTION_FILTER_H
#define GAMMA_CORRECTION_FILTER_H

#include "filter.h"

class GammaCorrectionFilter : public Filter
{
public:
    GammaCorrectionFilter();
    QImage applyFilter(QImage, std::function<void(int)>);
    QIcon getIcon();
    QString getName();
};

#endif // GAMMA_CORRECTION_FILTER_H
