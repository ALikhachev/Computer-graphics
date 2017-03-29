#ifndef BLUR_FILTER_H
#define BLUR_FILTER_H

#include "filter.h"

class BlurFilter : public Filter
{
public:
    BlurFilter();
    QImage applyFilter(QImage, std::function<void(int)>);
    QIcon getIcon();
    QString getName();
};

#endif // BLUR_FILTER_H
