#ifndef FLOYD_STEINBERG_FILTER_WIDGET_H
#define FLOYD_STEINBERG_FILTER_WIDGET_H

#include "filter_widget.h"
#include "filters/floyd_steinberg_filter.h"

class FloydSteinbergFilterWidget : public FilterWidget
{
public:
    FloydSteinbergFilterWidget(FloydSteinbergFilter *f, QWidget *parent = 0);

private:
    FloydSteinbergFilter *filter;
    FloydSteinbergFilterSettings *settings;
};

#endif // FLOYD_STEINBERG_FILTER_WIDGET_H
