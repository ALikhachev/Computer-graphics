#ifndef BLUR_FILTER_WIDGET_H
#define BLUR_FILTER_WIDGET_H

#include "filter_widget.h"
#include "filters/blur_filter.h"

class BlurFilterWidget : public FilterWidget
{
public:
    BlurFilterWidget(BlurFilter *f, QWidget *parent = 0);

private:
    BlurFilter *filter;
    BlurFilterSettings *settings;
};

#endif // BLUR_FILTER_WIDGET_H
