#ifndef SOBEL_FILTER_WIDGET_H
#define SOBEL_FILTER_WIDGET_H

#include "qsliderbox.h"
#include "filter_widget.h"
#include "filters/sobel_filter.h"

class SobelFilterWidget : public FilterWidget
{
public:
    SobelFilterWidget(SobelFilter *f, QWidget *parent = 0);
    void settingsUpdate() override;

private:
    SobelFilter *filter;
    SobelFilterSettings *settings;
    QSliderBox *threshold_slider;
};

#endif // SOBEL_FILTER_WIDGET_H
