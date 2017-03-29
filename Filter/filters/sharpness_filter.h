#ifndef SHARPNESS_FILTER_H
#define SHARPNESS_FILTER_H

#include "convolution_filter.h"

class SharpnessFilter : public ConvolutionFilter
{
public:
    SharpnessFilter();
    std::vector<float> &getMatrix();
    int getMatrixSize();
    QIcon getIcon();
    QString getName();

private:
    std::vector<float> matrix;
};

#endif // SHARPNESS_FILTER_H
