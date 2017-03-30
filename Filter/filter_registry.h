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
    std::map<QString, std::function<FilterWidget *(Filter *, QWidget *)>> filter_to_widget;

    FilterRegistry() {}
public:
    FilterRegistry(FilterRegistry const&) = delete;
    void operator=(FilterRegistry const&) = delete;
    static FilterRegistry &getInstance();
    bool registerFilter(Filter *);
    bool registerWidgetBuilder(QString, std::function<FilterWidget *(Filter *, QWidget *)>);
    std::vector<QSharedPointer<Filter>> &getFilters();
    QSharedPointer<Filter> getFilter(QString);
    FilterWidget *getWidget(Filter *, QWidget *parent = 0);
};

#endif // FILTER_REGISTRY_H
