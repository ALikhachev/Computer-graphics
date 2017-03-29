#ifndef BLUR_FILTER_WIDGET_H
#define BLUR_FILTER_WIDGET_H

#include <QSharedPointer>

#include "filter_widget.h"
#include "filters/scale_filter.h"

class ScaleFilterWidget : public FilterWidget
{
public:
    ScaleFilterWidget(QSharedPointer<ScaleFilter> f, QWidget *parent = 0);

private:
    QSharedPointer<ScaleFilter> filter;
};

#endif // BLUR_FILTER_WIDGET_H
