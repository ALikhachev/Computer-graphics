#ifndef EDGE_FILTER_WIDGET_H
#define EDGE_FILTER_WIDGET_H

#include "filter_widget.h"
#include "filters/edge_filter.h"

class EdgeFilterWidget : public FilterWidget
{
public:
    EdgeFilterWidget(EdgeFilter *f, QWidget *parent = 0);

private:
    EdgeFilter *filter;
   EdgeFilterSettings *settings;
};

#endif // EDGE_FILTER_WIDGET_H
