#ifndef FLOYD_STEINBERG_FILTER_WIDGET_H
#define FLOYD_STEINBERG_FILTER_WIDGET_H

#include "qsliderbox.h"
#include "filter_widget.h"
#include "filters/floyd_steinberg_filter.h"

class FloydSteinbergFilterWidget : public FilterWidget
{
public:
    FloydSteinbergFilterWidget(FloydSteinbergFilter *f, QWidget *parent = 0);
    void settingsUpdate() override;

private:
    FloydSteinbergFilter *filter;
    FloydSteinbergFilterSettings *settings;
    QSliderBox *red_slider;
    QSliderBox *green_slider;
    QSliderBox *blue_slider;
};

#endif // FLOYD_STEINBERG_FILTER_WIDGET_H
