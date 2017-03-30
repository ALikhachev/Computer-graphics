#include <QFormLayout>
#include <QSlider>

#include "widgets/ordered_dithering_filter_widget.h"
#include "filter_registry.h"
#include "qsliderbox.h"

OrderedDitheringFilterWidget::OrderedDitheringFilterWidget(OrderedDitheringFilter *f, QWidget *parent) : FilterWidget(parent),
    filter(f),
    settings(f->getSettings()),
    log_2_slider(new QSliderBox(1, 7, this))
{
    QFormLayout *layout = new QFormLayout(this);
    this->log_2_slider->setValue(std::log2(this->settings->matrix_size));
    layout->addRow(tr("Matrix size (2<sup>k</sup>)"), this->log_2_slider);
    connect(this->log_2_slider, &QSliderBox::valueChanged, [this] (int i) {
        this->settings->matrix_size = std::pow(2, i);
        this->filter->request();
    });
}

void OrderedDitheringFilterWidget::settingsUpdate() {
    this->settings = this->filter->getSettings();
    this->log_2_slider->setValue(std::log2(this->settings->matrix_size));
}

namespace {
    bool reg = FilterRegistry::getInstance().registerWidgetBuilder(QObject::tr("Ordered dithering"), [] (Filter *f, QWidget *parent) {
        OrderedDitheringFilter *OrderedDithering_filter = dynamic_cast<OrderedDitheringFilter *>(f);
        return new OrderedDitheringFilterWidget(OrderedDithering_filter, parent);
    });
}
