#ifndef WATERCOLOR_FILTER_H
#define WATERCOLOR_FILTER_H

#include "convolution_filter.h"

class WatercolorFilterSettings {
public:
    int radius = 1;
};

class WatercolorFilter : public ConvolutionFilter
{
public:
    WatercolorFilter();
    QImage applyFilter(QImage image, std::function<void(int)> updateProgress);
    std::vector<float> &getMatrix();
    int getMatrixSize();
    QIcon getIcon();
    QString getName();
    WatercolorFilterSettings *getSettings();

private:
    WatercolorFilterSettings settings;
    std::vector<float> matrix;
};

#endif // WATERCOLOR_FILTER_H
