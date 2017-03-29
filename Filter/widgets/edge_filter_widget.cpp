#include <QFormLayout>
#include <QSlider>

#include "widgets/edge_filter_widget.h"
#include "filter_registry.h"
#include "qsliderbox.h"

EdgeFilterWidget::EdgeFilterWidget(EdgeFilter *f, QWidget *parent) : FilterWidget(parent),
    filter(f),
    settings(f->getSettings())
{
    QFormLayout *layout = new QFormLayout(this);
    QSliderBox *threshold_slider = new QSliderBox(0, 600, this);
    threshold_slider->setValue(this->settings->threshold);
    layout->addRow(tr("Edge threshold"), threshold_slider);
    connect(threshold_slider, &QSliderBox::valueChanged, [this] (int i) {
        this->settings->threshold = i;
        this->filter->request();
    });
}

namespace {
    bool reg = FilterRegistry::getInstance().registerWidgetBuilder(QObject::tr("Edge"), [] (Filter *f, QWidget *parent) {
        EdgeFilter *Edge_filter = dynamic_cast<EdgeFilter *>(f);
        return new EdgeFilterWidget(Edge_filter, parent);
    });
}
