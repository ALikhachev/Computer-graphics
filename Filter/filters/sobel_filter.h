#ifndef SOBEL_FILTER_H
#define SOBEL_FILTER_H

#include "filter.h"

class SobelFilterSettings  : public FilterSettings
{
public:
    FilterSettings *clone() override;

    int threshold = 127;
};

class SobelFilter : public Filter
{
public:
    SobelFilter();
    QImage applyFilter(QImage image, std::function<void(int)> updateProgress);
    QIcon getIcon();
    QString getName();
    SobelFilterSettings *getSettings();
    void setSettings(FilterSettings *) override;

private:
    SobelFilterSettings settings;
};

#endif // SOBEL_FILTER_H
