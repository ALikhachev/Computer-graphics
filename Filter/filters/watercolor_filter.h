#ifndef WATERCOLOR_FILTER_H
#define WATERCOLOR_FILTER_H

#include "convolution_filter.h"

class WatercolorFilter : public ConvolutionFilter
{
public:
    WatercolorFilter();
    QImage applyFilter(QImage image, std::function<void(int)> updateProgress);
    std::vector<float> &getMatrix();
    int getMatrixSize();
    QIcon getIcon();
    QString getName();

private:
    std::vector<float> matrix;
};

#endif // WATERCOLOR_FILTER_H
