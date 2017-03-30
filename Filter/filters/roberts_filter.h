#ifndef ROBERTS_FILTER_H
#define ROBERTS_FILTER_H

#include "filter.h"

class RobertsFilterSettings  : public FilterSettings
{
public:
    FilterSettings *clone() override;

    int threshold = 42;
};

class RobertsFilter : public Filter
{
public:
    RobertsFilter();
    QImage applyFilter(QImage image, std::function<void(int)> updateProgress);
    QIcon getIcon();
    QString getName();
    RobertsFilterSettings *getSettings();
    void setSettings(FilterSettings *) override;

private:
    RobertsFilterSettings settings;
};

#endif // ROBERTS_FILTER_H
