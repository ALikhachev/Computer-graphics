#ifndef FILTER_REGISTRY_H
#define FILTER_REGISTRY_H

#include <QSharedPointer>
#include <QString>
#include <functional>

#include "filter.h"
#include "filter_widget.h"

class FilterRegistry
{
private:
    std::vector<QSharedPointer<Filter>> filters;
    std::map<QString, QSharedPointer<Filter>> name_to_filter;
    std::map<QString, QSharedPointer<FilterWidget>> filter_to_widget;
    std::vector<std::function<void(void)>> widget_register_queue;

    FilterRegistry() {}
public:
    FilterRegistry(FilterRegistry const&) = delete;
    void operator=(FilterRegistry const&) = delete;
    static FilterRegistry &getInstance();
    bool registerFilter(Filter *);
    bool queueBindWidget(std::function<void(void)>);
    void unqueueBindWidgets();
    bool bindWidget(QString, FilterWidget *);
    std::vector<QSharedPointer<Filter>> &getFilters();
    QSharedPointer<Filter> getFilter(QString);
    QSharedPointer<FilterWidget> getWidget(Filter *);
};

#endif // FILTER_REGISTRY_H
