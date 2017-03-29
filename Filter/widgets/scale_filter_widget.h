#ifndef BLUR_FILTER_WIDGET_H
#define BLUR_FILTER_WIDGET_H

#include <QSharedPointer>

#include "filter_widget.h"
#include "filters/scale_filter.h"

class ScaleFilterWidget : public FilterWidget
{
public:
    ScaleFilterWidget(ScaleFilter *f, QWidget *parent = 0);

private:
    ScaleFilter *filter;
};

#endif // BLUR_FILTER_WIDGET_H
