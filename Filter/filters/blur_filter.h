#ifndef BLUR_FILTER_H
#define BLUR_FILTER_H

#include "convolution_filter.h"

#include <QSharedPointer>

class BlurFilterSettings  : public FilterSettings
{
public:
    FilterSettings *clone() override;

    int matrix_index = 0;
};

class BlurFilter : public ConvolutionFilter
{
public:
    BlurFilter();
    std::vector<float> &getMatrix();
    int getMatrixSize();
    QIcon getIcon();
    QString getName();
    BlurFilterSettings *getSettings();
    void setSettings(FilterSettings *) override;

private:
    BlurFilterSettings settings;
    std::vector<std::vector<float>> matrices;
};

#endif // BLUR_FILTER_H
