#ifndef FILTER_H
#define FILTER_H

#include <QWidget>
#include <QImage>
#include <QString>
#include <QIcon>
#include <QThread>

#include "filter_parameters_widget.h"

class Filter;

class FilterWorker : public QObject
{
    Q_OBJECT
public slots:
    void doFilter(Filter *, QImage);
signals:
    void resultReady(QImage);
};

class Filter : public QObject
{
    Q_OBJECT
public:
    virtual QImage applyFilter(QImage) = 0;
    virtual FilterParametersWidget *getParametersWidget(QWidget *parent = 0) = 0;
    virtual QIcon getIcon() = 0;
    virtual QString getName() = 0;

public slots:
    void request();

signals:
    void requested(Filter *f);
};

class GrayscaleFilter : public Filter
{
public:
    GrayscaleFilter();
    QImage applyFilter(QImage);
    FilterParametersWidget *getParametersWidget(QWidget *parent = 0);
    QIcon getIcon();
    QString getName();
};

#endif // FILTER_H
