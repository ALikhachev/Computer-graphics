#ifndef GAMMA_CORRECTION_FILTER_H
#define GAMMA_CORRECTION_FILTER_H

#include "filter.h"

class GammaCorrectionFilterSettings {
public:
    float gamma = 1;
};

class GammaCorrectionFilter : public Filter
{
public:
    GammaCorrectionFilter();
    QImage applyFilter(QImage, std::function<void(int)>);
    QIcon getIcon();
    QString getName();
    GammaCorrectionFilterSettings *getSettings();

private:
    GammaCorrectionFilterSettings settings;
};

#endif // GAMMA_CORRECTION_FILTER_H
