#ifndef FILTER_REGISTRY_H
#define FILTER_REGISTRY_H

#include <QSharedPointer>
#include <QString>

#include "filter.h"

class FilterRegistry
{
private:
    std::vector<QSharedPointer<Filter>> filters;
    std::map<QString, QSharedPointer<Filter>> name_to_filter;
    std::map<QString, QSharedPointer<QWidget>> filter_to_widget;

    FilterRegistry() {}
public:
    FilterRegistry(FilterRegistry const&) = delete;
    void operator=(FilterRegistry const&) = delete;
    static FilterRegistry &getInstance();
    bool registerFilter(Filter *);
    bool bindWidget(QString, QWidget *);
    std::vector<QSharedPointer<Filter>> &getFilters();
    QSharedPointer<Filter> getFilter(QString);
    QSharedPointer<QWidget> getWidget(Filter *);
};

#endif // FILTER_REGISTRY_H
