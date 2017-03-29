#include "ordered_dithering_filter.h"

OrderedDitheringFilter::OrderedDitheringFilter()
{

}

QImage OrderedDitheringFilter::applyFilter(QImage image, std::function<void(int)> updateProgress) {
    return image;
}

QIcon OrderedDitheringFilter::getIcon() {
    return QIcon(":/icons/ordered_dithering.png");
}

QString OrderedDitheringFilter::getName() {
    return "OrderedDithering";
}

OrderedDitheringFilterSettings *OrderedDitheringFilter::getSettings() {
    return &this->settings;
}

//namespace {
//    bool reg = FilterRegistry::getInstance().registerFilter(new OrderedDitheringFilter);
//}
