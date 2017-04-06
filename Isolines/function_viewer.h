#ifndef FUNCTION_VIEWER_H
#define FUNCTION_VIEWER_H

#include <QWidget>
#include <QSharedPointer>

#include "legend.h"
#include "isolines.h"
#include "configuration.h"

class FunctionViewer : public QWidget
{
    Q_OBJECT
public:
    explicit FunctionViewer(QSharedPointer<Configuration> config, QWidget *parent = 0);

signals:
    void pointerFunctionValueUpdated(IsolinesMousePosition);

private:
    QSharedPointer<Configuration> config;
    Legend *legend;
    Isolines *isolines;
};

#endif // FUNCTION_VIEWER_H
