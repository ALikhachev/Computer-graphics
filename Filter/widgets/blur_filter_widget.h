#ifndef BLUR_FILTER_WIDGET_H
#define BLUR_FILTER_WIDGET_H

#include <QComboBox>

#include "filter_widget.h"
#include "filters/blur_filter.h"

class BlurFilterWidget : public FilterWidget
{
public:
    BlurFilterWidget(BlurFilter *f, QWidget *parent = 0);
    void settingsUpdate() override;

private:
    BlurFilter *filter;
    BlurFilterSettings *settings;
    QComboBox *box;
};

#endif // BLUR_FILTER_WIDGET_H
