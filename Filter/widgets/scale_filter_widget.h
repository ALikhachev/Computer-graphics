#ifndef BLUR_FILTER_WIDGET_H
#define BLUR_FILTER_WIDGET_H

#include <QCheckBox>

#include "qsliderbox.h"
#include "filter_widget.h"
#include "filters/scale_filter.h"

class ScaleFilterWidget : public FilterWidget
{
public:
    ScaleFilterWidget(ScaleFilter *f, QWidget *parent = 0);
    void settingsUpdate() override;

private:
    ScaleFilter *filter;
    ScaleFilterSettings *settings;
    QSliderBox *scale_slider;
    QCheckBox *invert_scale_checkbox;
};

#endif // BLUR_FILTER_WIDGET_H
