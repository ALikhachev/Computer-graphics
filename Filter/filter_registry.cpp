#include "filter_registry.h"

bool FilterRegistry::registerFilter(Filter *f) {
    QSharedPointer<Filter> p = QSharedPointer<Filter>(f);
    this->filters.push_back(p);
    this->name_to_filter[f->getName()] = p;
    return true;
}

bool FilterRegistry::registerWidgetBuilder(QString name, std::function<FilterWidget *(Filter *, QWidget *)> widgetConstructor) {
    this->filter_to_widget[name] = widgetConstructor;
    return true;
}

std::vector<QSharedPointer<Filter>> &FilterRegistry::getFilters() {
    return this->filters;
}

QSharedPointer<Filter> FilterRegistry::getFilter(QString name) {
    return this->name_to_filter[name];
}

FilterWidget *FilterRegistry::getWidget(Filter *f, QWidget *parent) {
    return this->filter_to_widget.find(f->getName()) != this->filter_to_widget.end() ? this->filter_to_widget[f->getName()](f, parent)
            : NULL;
}

FilterRegistry &FilterRegistry::getInstance() {
    static FilterRegistry instance;
    return instance;
}
