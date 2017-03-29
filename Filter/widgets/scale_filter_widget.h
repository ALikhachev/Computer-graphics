#ifndef BLUR_FILTER_WIDGET_H
#define BLUR_FILTER_WIDGET_H

#include "filter_widget.h"
#include "filters/scale_filter.h"

class ScaleFilterWidget : public FilterWidget
{
public:
    ScaleFilterWidget(ScaleFilter *f, QWidget *parent = 0);

private:
    ScaleFilter *filter;
    ScaleFilterSettings settings;
};

#endif // BLUR_FILTER_WIDGET_H
