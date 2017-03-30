#ifndef FILTER_WIDGET_H
#define FILTER_WIDGET_H

#include <QWidget>

class FilterWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FilterWidget(QWidget *parent = 0);
    virtual ~FilterWidget() {}
    virtual void settingsUpdate() = 0;
};

#endif // FILTER_WIDGET_H
