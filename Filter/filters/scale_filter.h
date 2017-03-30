#ifndef SCALE_FILTER_H
#define SCALE_FILTER_H

#include "filter.h"

class ScaleFilterSettings  : public FilterSettings
{
public:
    FilterSettings *clone() override;

    int scale_factor = 2;
    bool invert = false;
};

class ScaleFilter : public Filter
{
public:
    ScaleFilter();
    QImage applyFilter(QImage, std::function<void(int)>);
    QIcon getIcon();
    QString getName();
    ScaleFilterSettings *getSettings();
    void setSettings(FilterSettings *) override;

private:
    ScaleFilterSettings settings;
};

#endif // SCALE_FILTER_H
