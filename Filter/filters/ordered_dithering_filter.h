#ifndef ORDERED_DITHERING_FILTER_H
#define ORDERED_DITHERING_FILTER_H

#include "filter.h"

class OrderedDitheringFilterSettings  : public FilterSettings
{
public:
    FilterSettings *clone() override;

    int matrix_size = 16;
};

class OrderedDitheringFilter : public Filter
{
public:
    OrderedDitheringFilter();
    QImage applyFilter(QImage image, std::function<void(int)> updateProgress);
    QIcon getIcon();
    QString getName();
    OrderedDitheringFilterSettings *getSettings();
    void setSettings(FilterSettings *) override;

private:
    static std::vector<float> buildMatrix(int size);

    OrderedDitheringFilterSettings settings;
};

#endif // ORDERED_DITHERING_FILTER_H
