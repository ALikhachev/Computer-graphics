#include "gamma_correction_filter_widget.h"

#include <QFormLayout>
#include <QDoubleSpinBox>

#include "filters/gamma_correction_filter.h"
#include "filter_registry.h"

GammaCorrectionFilterWidget::GammaCorrectionFilterWidget(GammaCorrectionFilter *f, QWidget *parent) : FilterWidget(parent),
    filter(f),
    settings(f->getSettings())
{
    QFormLayout *layout = new QFormLayout(this);
    QDoubleSpinBox *box = new QDoubleSpinBox(this);
    box->setRange(0.01, 10.0);
    box->setValue(this->settings->gamma);
    layout->addRow(tr("Gamma value (0-10]"), box);
    connect(box, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [this] (double val) {
        this->settings->gamma = val;
        this->filter->request();
    });
}

namespace {
    bool reg = FilterRegistry::getInstance().registerWidgetBuilder(QObject::tr("Gamma"), [] (Filter *f, QWidget *parent) {
        GammaCorrectionFilter *gamma_filter = dynamic_cast<GammaCorrectionFilter *>(f);
        return new GammaCorrectionFilterWidget(gamma_filter, parent);
    });
}
