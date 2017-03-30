#ifndef ROTATE_FILTER_WIDGET_H
#define ROTATE_FILTER_WIDGET_H

#include "filter_widget.h"
#include "filters/rotate_filter.h"

class RotateFilterWidget : public FilterWidget
{
public:
    RotateFilterWidget(RotateFilter *f, QWidget *parent = 0);

private:
    RotateFilter *filter;
    RotateFilterSettings *settings;
};

#endif // ROTATE_FILTER_WIDGET_H
