#include <QFormLayout>
#include <QSlider>

#include "widgets/floyd_steinberg_filter_widget.h"
#include "filter_registry.h"
#include "qsliderbox.h"

FloydSteinbergFilterWidget::FloydSteinbergFilterWidget(FloydSteinbergFilter *f, QWidget *parent) : FilterWidget(parent),
    filter(f),
    settings(f->getSettings())
{
    QFormLayout *layout = new QFormLayout(this);
    QSliderBox *red_slider = new QSliderBox(2, 255, this);
    QSliderBox *green_slider = new QSliderBox(2, 255, this);
    QSliderBox *blue_slider = new QSliderBox(2, 255, this);
    red_slider->setValue(this->settings->red);
    green_slider->setValue(this->settings->green);
    blue_slider->setValue(this->settings->blue);
    layout->addRow(tr("Red"), red_slider);
    layout->addRow(tr("Green"), green_slider);
    layout->addRow(tr("Blue"), blue_slider);
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
}

//namespace {
//    bool reg = FilterRegistry::getInstance().registerWidgetBuilder(QObject::tr("FloydSteinberg"), [] (Filter *f, QWidget *parent) {
//        FloydSteinbergFilter *FloydSteinberg_filter = dynamic_cast<FloydSteinbergFilter *>(f);
//        return new FloydSteinbergFilterWidget(FloydSteinberg_filter, parent);
//    });
//}
