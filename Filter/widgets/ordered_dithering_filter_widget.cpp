#include <QFormLayout>
#include <QSlider>

#include "widgets/ordered_dithering_filter_widget.h"
#include "filter_registry.h"
#include "qsliderbox.h"

OrderedDitheringFilterWidget::OrderedDitheringFilterWidget(OrderedDitheringFilter *f, QWidget *parent) : FilterWidget(parent),
    filter(f),
    settings(f->getSettings())
{
    QFormLayout *layout = new QFormLayout(this);
    QSliderBox *red_slider = new QSliderBox(2, 255, this);
    QSliderBox *green_slider = new QSliderBox(2, 255, this);
    QSliderBox *blue_slider = new QSliderBox(2, 255, this);
    QSliderBox *log_2_slider = new QSliderBox(1, 7, this);
    red_slider->setValue(this->settings->red);
    green_slider->setValue(this->settings->green);
    blue_slider->setValue(this->settings->blue);
    log_2_slider->setValue(std::log2(this->settings->matrix_size));
    layout->addRow(tr("Red"), red_slider);
    layout->addRow(tr("Green"), green_slider);
    layout->addRow(tr("Blue"), blue_slider);
    layout->addRow(tr("Matrix size (2<sup>k</sup>)"), log_2_slider);
    connect(red_slider, &QSliderBox::valueChanged, [this] (int i) {
        this->settings->red = i;
        this->filter->request();
    });
    connect(green_slider, &QSliderBox::valueChanged, [this] (int i) {
        this->settings->green = i;
        this->filter->request();
    });
    connect(blue_slider, &QSliderBox::valueChanged, [this] (int i) {
        this->settings->blue = i;
        this->filter->request();
    });
    connect(log_2_slider, &QSliderBox::valueChanged, [this] (int i) {
        this->settings->matrix_size = std::pow(2, i);
        this->filter->request();
    });
}

//namespace {
//    bool reg = FilterRegistry::getInstance().registerWidgetBuilder(QObject::tr("Ordered dithering"), [] (Filter *f, QWidget *parent) {
//        OrderedDitheringFilter *OrderedDithering_filter = dynamic_cast<OrderedDitheringFilter *>(f);
//        return new OrderedDitheringFilterWidget(OrderedDithering_filter, parent);
//    });
//}
