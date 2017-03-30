#ifndef ROTATE_FILTER_H
#define ROTATE_FILTER_H

#include "filter.h"

class RotateFilterSettings {
public:
    int angle = 0;
};

class RotateFilter : public Filter
{
public:
    RotateFilter();
    QImage applyFilter(QImage image, std::function<void(int)> updateProgress);
    QIcon getIcon();
    QString getName();
    RotateFilterSettings *getSettings();

private:
    RotateFilterSettings settings;
};

#endif // ROTATE_FILTER_H
