#include "widgets/scale_filter_widget.h"
#include "filter_registry.h"

ScaleFilterWidget::ScaleFilterWidget(QSharedPointer<ScaleFilter> f, QWidget *parent) : FilterWidget(parent),
    filter(f)
{

}

namespace {
    bool reg = FilterRegistry::getInstance().queueBindWidget([] {
        FilterRegistry::getInstance().bindWidget(QObject::tr("Scale"), new ScaleFilterWidget(FilterRegistry::getInstance().getFilter("Scale").dynamicCast<ScaleFilter>()));
    });
}
