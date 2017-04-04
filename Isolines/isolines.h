#ifndef ISOLINES_H
#define ISOLINES_H

#include <QWidget>
#include <QSharedPointer>

#include "configuration.h"

class Isolines : public QWidget
{
    Q_OBJECT
public:
    explicit Isolines(QSharedPointer<Configuration> config, QWidget *parent = 0);

private:
    QSharedPointer<Configuration> config;
};

#endif // ISOLINES_H
