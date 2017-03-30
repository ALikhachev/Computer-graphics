#ifndef ROBERTS_FILTER_WIDGET_H
#define ROBERTS_FILTER_WIDGET_H

#include "qsliderbox.h"
#include "filter_widget.h"
#include "filters/roberts_filter.h"

class RobertsFilterWidget : public FilterWidget
{
public:
    RobertsFilterWidget(RobertsFilter *f, QWidget *parent = 0);
    void settingsUpdate() override;

private:
    RobertsFilter *filter;
    RobertsFilterSettings *settings;
    QSliderBox *threshold_slider;
};

#endif // ROBERTS_FILTER_WIDGET_H
