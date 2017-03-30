#ifndef GRAYSCALE_FILTER_H
#define GRAYSCALE_FILTER_H

#include "filter.h"

class GrayscaleFilter : public Filter
{
public:
    GrayscaleFilter();
    QImage applyFilter(QImage, std::function<void(int)>);
    QIcon getIcon();
    QString getName();
};

#endif // GRAYSCALE_FILTER_H
