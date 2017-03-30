#ifndef NEGATIVE_FILTER_H
#define NEGATIVE_FILTER_H

#include "filter.h"

class NegativeFilter : public Filter
{
public:
    NegativeFilter();
    QImage applyFilter(QImage, std::function<void(int)>);
    QIcon getIcon();
    QString getName();
};

#endif // NEGATIVE_FILTER_H
