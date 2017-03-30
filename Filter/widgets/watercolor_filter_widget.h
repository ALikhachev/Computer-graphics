#ifndef WATERCOLOR_FILTER_WIDGET_H
#define WATERCOLOR_FILTER_WIDGET_H

#include "qsliderbox.h"
#include "filter_widget.h"
#include "filters/watercolor_filter.h"

class WatercolorFilterWidget : public FilterWidget
{
public:
    WatercolorFilterWidget(WatercolorFilter *f, QWidget *parent = 0);
    void settingsUpdate() override;

private:
    WatercolorFilter *filter;
    WatercolorFilterSettings *settings;
    QSliderBox *radius_slider;
};

#endif // WATERCOLOR_FILTER_WIDGET_H
