#include <QFormLayout>
#include <QSlider>

#include "widgets/edge_filter_widget.h"
#include "filter_registry.h"
#include "qsliderbox.h"

EdgeFilterWidget::EdgeFilterWidget(EdgeFilter *f, QWidget *parent) : FilterWidget(parent),
    filter(f),
    settings(f->getSettings()),
    threshold_slider(new QSliderBox(0, 600, this))
{
    QFormLayout *layout = new QFormLayout(this);
    this->threshold_slider->setValue(this->settings->threshold);
    layout->addRow(tr("Edge threshold"), this->threshold_slider);
    connect(this->threshold_slider, &QSliderBox::valueChanged, [this] (int i) {
        this->settings->threshold = i;
        this->filter->request();
    });
}

void EdgeFilterWidget::settingsUpdate() {
    this->settings = this->filter->getSettings();
    this->threshold_slider->setValue(this->settings->threshold);
}

namespace {
    bool reg = FilterRegistry::getInstance().registerWidgetBuilder(QObject::tr("Edge"), [] (Filter *f, QWidget *parent) {
        EdgeFilter *Edge_filter = dynamic_cast<EdgeFilter *>(f);
        return new EdgeFilterWidget(Edge_filter, parent);
    });
}
