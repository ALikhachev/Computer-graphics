#ifndef EMBOSSING_FILTER_H
#define EMBOSSING_FILTER_H

#include "convolution_filter.h"

class EmbossingFilter : public ConvolutionFilter
{
public:
    EmbossingFilter();
    QImage applyFilter(QImage, std::function<void(int)>);
    std::vector<float> &getMatrix();
    int getMatrixSize();
    QIcon getIcon();
    QString getName();

private:
    std::vector<float> matrix;
};

#endif // EMBOSSING_FILTER_H
