#include <QFormLayout>
#include <QSlider>

#include "widgets/floyd_steinberg_filter_widget.h"
#include "filter_registry.h"
#include "qsliderbox.h"

FloydSteinbergFilterWidget::FloydSteinbergFilterWidget(FloydSteinbergFilter *f, QWidget *parent) : FilterWidget(parent),
    filter(f),
    settings(f->getSettings()),
    red_slider(new QSliderBox(2, 255, this)),
    green_slider(new QSliderBox(2, 255, this)),
    blue_slider(new QSliderBox(2, 255, this))
{
    QFormLayout *layout = new QFormLayout(this);
    this->red_slider->setValue(this->settings->red);
    this->green_slider->setValue(this->settings->green);
    this->blue_slider->setValue(this->settings->blue);
    layout->addRow(tr("Red"), this->red_slider);
    layout->addRow(tr("Green"), this->green_slider);
    layout->addRow(tr("Blue"), this->blue_slider);
    connect(this->red_slider, &QSliderBox::valueChanged, [this] (int i) {
        this->settings->red = i;
        this->filter->request();
    });
    connect(this->green_slider, &QSliderBox::valueChanged, [this] (int i) {
        this->settings->green = i;
        this->filter->request();
    });
    connect(this->blue_slider, &QSliderBox::valueChanged, [this] (int i) {
        this->settings->blue = i;
        this->filter->request();
    });
}

void FloydSteinbergFilterWidget::settingsUpdate() {
    this->settings = this->filter->getSettings();
    this->red_slider->setValue(this->settings->red);
    this->green_slider->setValue(this->settings->green);
    this->blue_slider->setValue(this->settings->blue);
}

namespace {
    bool reg = FilterRegistry::getInstance().registerWidgetBuilder(QObject::tr("Floyd-steinberg"), [] (Filter *f, QWidget *parent) {
        FloydSteinbergFilter *FloydSteinberg_filter = dynamic_cast<FloydSteinbergFilter *>(f);
        return new FloydSteinbergFilterWidget(FloydSteinberg_filter, parent);
    });
}
