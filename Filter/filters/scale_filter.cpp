#include "filters/scale_filter.h"
#include "filter_registry.h"
#include "utils.h"

FilterSettings *ScaleFilterSettings::clone() {
    return new ScaleFilterSettings(*this);
}

ScaleFilter::ScaleFilter()
{
}

QImage ScaleFilter::applyFilter(QImage image, std::function<void(int)> updateProgress) {
    return FilterUtils::scaleImage(image, this->settings.invert ? 1.0 / settings.scale_factor : settings.scale_factor, 350, 350, &updateProgress);
}

QIcon ScaleFilter::getIcon() {
    return QIcon(":/icons/scale.png");
}

QString ScaleFilter::getName() {
    return tr("Scale");
}

ScaleFilterSettings *ScaleFilter::getSettings() {
    return &this->settings;
}

void ScaleFilter::setSettings(FilterSettings *settings) {
    ScaleFilterSettings *w_settings = dynamic_cast<ScaleFilterSettings *>(settings);
    if (w_settings) {
        this->settings = *w_settings;
    }
}

namespace {
    bool reg = FilterRegistry::getInstance().registerFilter(new ScaleFilter);
}
