#include "filter_registry.h"

bool FilterRegistry::registerFilter(Filter *f) {
    QSharedPointer<Filter> p = QSharedPointer<Filter>(f);
    this->filters.push_back(p);
    this->name_to_filter[f->getName()] = p;
    return true;
}

bool FilterRegistry::bindWidget(QString name, QWidget *widget) {
    this->filter_to_widget[name] = QSharedPointer<QWidget>(widget);
    return true;
}

std::vector<QSharedPointer<Filter>> &FilterRegistry::getFilters() {
    return this->filters;
}

QSharedPointer<Filter> FilterRegistry::getFilter(QString name) {
    return this->name_to_filter[name];
}

QSharedPointer<QWidget> FilterRegistry::getWidget(Filter *f) {
    return this->filter_to_widget[f->getName()];
}

FilterRegistry &FilterRegistry::getInstance() {
    static FilterRegistry instance;
    return instance;
}
