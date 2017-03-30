#ifndef CONVOLUTION_FILTER_H
#define CONVOLUTION_FILTER_H

#include "filter.h"

#include <QSharedPointer>

class ConvolutionFilter : public Filter
{
public:
    QImage applyFilter(QImage, std::function<void(int)>);
    virtual std::vector<float> &getMatrix() = 0;
    virtual int getMatrixSize() = 0;
};

#endif // CONVOLUTION_FILTER_H
