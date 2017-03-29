#ifndef ROBERTS_FILTER_WIDGET_H
#define ROBERTS_FILTER_WIDGET_H

#include "filter_widget.h"
#include "filters/roberts_filter.h"

class RobertsFilterWidget : public FilterWidget
{
public:
    RobertsFilterWidget(RobertsFilter *f, QWidget *parent = 0);

private:
    RobertsFilter *filter;
    RobertsFilterSettings *settings;
};

#endif // ROBERTS_FILTER_WIDGET_H
