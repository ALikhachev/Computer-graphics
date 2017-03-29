#ifndef ORDERED_DITHERING_FILTER_H
#define ORDERED_DITHERING_FILTER_H

#include "filter.h"

class OrderedDitheringFilterSettings {
public:
    int red = 2;
    int green = 2;
    int blue = 2;
    int matrix_size = 4;
};

class OrderedDitheringFilter : public Filter
{
public:
    OrderedDitheringFilter();
    QImage applyFilter(QImage image, std::function<void(int)> updateProgress);
    QIcon getIcon();
    QString getName();
    OrderedDitheringFilterSettings *getSettings();

private:
    OrderedDitheringFilterSettings settings;
};

#endif // ORDERED_DITHERING_FILTER_H
