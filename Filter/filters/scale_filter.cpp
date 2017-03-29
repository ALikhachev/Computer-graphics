#include "filters/scale_filter.h"
#include "filter_registry.h"
#include "utils.h"

ScaleFilter::ScaleFilter() : Filter(new ScaleFilterSettings)
{
}

QImage ScaleFilter::applyFilter(QImage image, std::function<void(int)> updateProgress) {
    ScaleFilterSettings *settings = dynamic_cast<ScaleFilterSettings *>(this->settings);
    return FilterUtils::scaleImage(image, settings->scale_factor);
}

QIcon ScaleFilter::getIcon() {
    return QIcon(":/icons/scale.png");
}

QString ScaleFilter::getName() {
    return tr("Scale");
}

namespace {
    bool reg = FilterRegistry::getInstance().registerFilter(new ScaleFilter);
}
