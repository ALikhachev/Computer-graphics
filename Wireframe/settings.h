#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include <QSharedPointer>

#include "configuration.h"

class Settings : public QWidget
{
    Q_OBJECT
public:
    explicit Settings(QSharedPointer<Configuration> configuration, QWidget *parent = 0);

private:
    QSharedPointer<Configuration> _config;
};

#endif // SETTINGS_H
