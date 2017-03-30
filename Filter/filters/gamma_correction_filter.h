#ifndef GAMMA_CORRECTION_FILTER_H
#define GAMMA_CORRECTION_FILTER_H

#include "filter.h"

class GammaCorrectionFilterSettings  : public FilterSettings
{
public:
    FilterSettings *clone() override;

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
    void setSettings(FilterSettings *) override;

private:
    GammaCorrectionFilterSettings settings;
};

#endif // GAMMA_CORRECTION_FILTER_H
