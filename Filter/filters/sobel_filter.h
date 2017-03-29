#ifndef SOBEL_FILTER_H
#define SOBEL_FILTER_H

#include "filter.h"

class SobelFilterSettings {
public:
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

private:
    SobelFilterSettings settings;
};

#endif // SOBEL_FILTER_H
