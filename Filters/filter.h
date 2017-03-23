#ifndef FILTER_H
#define FILTER_H

#include <QWidget>
#include <QImage>
#include <QString>
#include <QIcon>

#include "filter_parameters_widget.h"

class Filter : public QObject
{
    Q_OBJECT
public:
    virtual QImage applyFilter(QImage &) = 0;
    virtual FilterParametersWidget *getParametersWidget(QWidget *parent = 0) = 0;
    virtual QIcon getIcon() = 0;
    virtual QString getName() = 0;
};

class GrayscaleFilter : public Filter
{
public:
    GrayscaleFilter();
    QImage applyFilter(QImage &);
    FilterParametersWidget *getParametersWidget(QWidget *parent = 0);
    QIcon getIcon();
    QString getName();
};

#endif // FILTER_H
