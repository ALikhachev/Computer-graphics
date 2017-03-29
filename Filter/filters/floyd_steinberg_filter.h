#ifndef FLOYD_STEINBERG_FILTER_H
#define FLOYD_STEINBERG_FILTER_H

#include "filter.h"

class FloydSteinbergFilterSettings {
public:
    int red = 2;
    int green = 2;
    int blue = 2;
    int matrix_size = 4;
};

class FloydSteinbergFilter : public Filter
{
public:
    FloydSteinbergFilter();
    QImage applyFilter(QImage image, std::function<void(int)> updateProgress);
    QIcon getIcon();
    QString getName();
    FloydSteinbergFilterSettings *getSettings();

private:
    FloydSteinbergFilterSettings settings;
};

#endif // FLOYD_STEINBERG_FILTER_H
