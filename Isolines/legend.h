#ifndef LEGEND_H
#define LEGEND_H

#include <QWidget>
#include <QSharedPointer>

#include "configuration.h"

class Legend : public QWidget
{
    Q_OBJECT
public:
    explicit Legend(QSharedPointer<Configuration> config, QWidget *parent = 0);

private:
    QSharedPointer<Configuration> config;
};

#endif // LEGEND_H
