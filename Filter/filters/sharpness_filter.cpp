#include "filters/sharpness_filter.h"
#include "filter_registry.h"

SharpnessFilter::SharpnessFilter() :
    matrix({ 0.0, -1.0,  0.0,
            -1.0,  5.0, -1.0,
             0.0, -1.0,  0.0})
{
}

std::vector<float> &SharpnessFilter::getMatrix() {
    return matrix;
}

int SharpnessFilter::getMatrixSize() {
    return 3;
}

QIcon SharpnessFilter::getIcon() {
    return QIcon(":/icons/sharpness.png");
}

QString SharpnessFilter::getName() {
    return tr("Sharpness");
}

namespace {
    bool reg = FilterRegistry::getInstance().registerFilter(new SharpnessFilter);
}
