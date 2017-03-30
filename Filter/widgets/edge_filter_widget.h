#ifndef EDGE_FILTER_WIDGET_H
#define EDGE_FILTER_WIDGET_H

#include "qsliderbox.h"
#include "filter_widget.h"
#include "filters/edge_filter.h"

class EdgeFilterWidget : public FilterWidget
{
public:
    EdgeFilterWidget(EdgeFilter *f, QWidget *parent = 0);
    void settingsUpdate() override;

private:
    EdgeFilter *filter;
    EdgeFilterSettings *settings;
    QSliderBox *threshold_slider;
};

#endif // EDGE_FILTER_WIDGET_H
