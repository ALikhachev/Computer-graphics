#ifndef FLOYD_STEINBERG_FILTER_H
#define FLOYD_STEINBERG_FILTER_H

#include "filter.h"

class FloydSteinbergFilterSettings  : public FilterSettings
{
public:
    FilterSettings *clone() override;

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
    void setSettings(FilterSettings *) override;

private:
    static uchar getClosestColor(uchar color, int shades);

    FloydSteinbergFilterSettings settings;
};

#endif // FLOYD_STEINBERG_FILTER_H
