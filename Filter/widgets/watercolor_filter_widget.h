#ifndef WATERCOLOR_FILTER_WIDGET_H
#define WATERCOLOR_FILTER_WIDGET_H

#include "filter_widget.h"
#include "filters/watercolor_filter.h"

class WatercolorFilterWidget : public FilterWidget
{
public:
    WatercolorFilterWidget(WatercolorFilter *f, QWidget *parent = 0);

private:
    WatercolorFilter *filter;
    WatercolorFilterSettings *settings;
};

#endif // WATERCOLOR_FILTER_WIDGET_H
