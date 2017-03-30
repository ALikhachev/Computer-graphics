#ifndef EDGES_FILTER_H
#define EDGES_FILTER_H

#include "convolution_filter.h"

class EdgeFilterSettings  : public FilterSettings
{
public:
    FilterSettings *clone() override;

    int threshold = 17;
};

class EdgeFilter : public ConvolutionFilter
{
public:
    EdgeFilter();
    QImage applyFilter(QImage image, std::function<void(int)> updateProgress);
    std::vector<float> &getMatrix();
    int getMatrixSize();
    QIcon getIcon();
    QString getName();
    EdgeFilterSettings *getSettings();
    void setSettings(FilterSettings *) override;

private:
    EdgeFilterSettings settings;
    std::vector<float> matrix;
};

#endif // EDGES_FILTER_H
