#ifndef ORDERED_DITHERING_FILTER_WIDGET_H
#define ORDERED_DITHERING_FILTER_WIDGET_H

#include "filter_widget.h"
#include "filters/ordered_dithering_filter.h"

class OrderedDitheringFilterWidget : public FilterWidget
{
public:
    OrderedDitheringFilterWidget(OrderedDitheringFilter *f, QWidget *parent = 0);

private:
    OrderedDitheringFilter *filter;
    OrderedDitheringFilterSettings *settings;
};

#endif // ORDERED_DITHERING_FILTER_WIDGET_H
