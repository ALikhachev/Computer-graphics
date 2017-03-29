#include "floyd_steinberg_filter.h"

FloydSteinbergFilter::FloydSteinbergFilter()
{

}

QImage FloydSteinbergFilter::applyFilter(QImage image, std::function<void(int)> updateProgress) {
    return image;
}

QIcon FloydSteinbergFilter::getIcon() {
    return QIcon(":/icons/floyd_steinberg.png");
}

QString FloydSteinbergFilter::getName() {
    return "FloydSteinberg";
}

FloydSteinbergFilterSettings *FloydSteinbergFilter::getSettings() {
    return &this->settings;
}

//namespace {
//    bool reg = FilterRegistry::getInstance().registerFilter(new FloydSteinbergFilter);
//}
