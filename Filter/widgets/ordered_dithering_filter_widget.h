#ifndef ORDERED_DITHERING_FILTER_WIDGET_H
#define ORDERED_DITHERING_FILTER_WIDGET_H

#include "qsliderbox.h"
#include "filter_widget.h"
#include "filters/ordered_dithering_filter.h"

class OrderedDitheringFilterWidget : public FilterWidget
{
public:
    OrderedDitheringFilterWidget(OrderedDitheringFilter *f, QWidget *parent = 0);
    void settingsUpdate() override;

private:
    OrderedDitheringFilter *filter;
    OrderedDitheringFilterSettings *settings;
    QSliderBox *log_2_slider;
};

#endif // ORDERED_DITHERING_FILTER_WIDGET_H
