#ifndef ROTATE_FILTER_WIDGET_H
#define ROTATE_FILTER_WIDGET_H

#include "qsliderbox.h"
#include "filter_widget.h"
#include "filters/rotate_filter.h"

class RotateFilterWidget : public FilterWidget
{
public:
    RotateFilterWidget(RotateFilter *f, QWidget *parent = 0);
    void settingsUpdate() override;

private:
    RotateFilter *filter;
    RotateFilterSettings *settings;
    QSliderBox *angle_slider;
};

#endif // ROTATE_FILTER_WIDGET_H
