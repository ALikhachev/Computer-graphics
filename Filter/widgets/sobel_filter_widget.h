#ifndef SOBEL_FILTER_WIDGET_H
#define SOBEL_FILTER_WIDGET_H

#include "filter_widget.h"
#include "filters/sobel_filter.h"

class SobelFilterWidget : public FilterWidget
{
public:
    SobelFilterWidget(SobelFilter *f, QWidget *parent = 0);

private:
    SobelFilter *filter;
   SobelFilterSettings *settings;
};

#endif // SOBEL_FILTER_WIDGET_H
