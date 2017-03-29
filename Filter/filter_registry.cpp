#include "filter_registry.h"

bool FilterRegistry::registerFilter(Filter *f) {
    QSharedPointer<Filter> p = QSharedPointer<Filter>(f);
    this->filters.push_back(p);
    this->name_to_filter[f->getName()] = p;
    return true;
}

bool FilterRegistry::queueBindWidget(std::function<void(void)> fun) {
    this->widget_register_queue.push_back(fun);
}

void FilterRegistry::unqueueBindWidgets() {
    for (auto it = this->widget_register_queue.begin(); it < this->widget_register_queue.end(); ++it) {
        (*it)();
        this->widget_register_queue.erase(it);
    }
}

bool FilterRegistry::bindWidget(QString name, FilterWidget *widget) {
    this->filter_to_widget[name] = QSharedPointer<FilterWidget>(widget);
    return true;
}

std::vector<QSharedPointer<Filter>> &FilterRegistry::getFilters() {
    return this->filters;
}

QSharedPointer<Filter> FilterRegistry::getFilter(QString name) {
    return this->name_to_filter[name];
}

QSharedPointer<FilterWidget> FilterRegistry::getWidget(Filter *f) {
    return this->filter_to_widget[f->getName()];
}

FilterRegistry &FilterRegistry::getInstance() {
    static FilterRegistry instance;
    return instance;
}
