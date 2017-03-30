#ifndef GAMMA_CORRECTION_FILTER_WIDGET_H
#define GAMMA_CORRECTION_FILTER_WIDGET_H

#include <QDoubleSpinBox>

#include "filter_widget.h"
#include "filters/gamma_correction_filter.h"

class GammaCorrectionFilterWidget : public FilterWidget
{
public:
    GammaCorrectionFilterWidget(GammaCorrectionFilter *f, QWidget *parent = 0);
    void settingsUpdate() override;

private:
    GammaCorrectionFilter *filter;
    GammaCorrectionFilterSettings *settings;
    QDoubleSpinBox *box;
};

#endif // GAMMA_CORRECTION_FILTER_WIDGET_H
