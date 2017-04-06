#include "function_viewer.h"

#include <QVBoxLayout>

FunctionViewer::FunctionViewer(QSharedPointer<Configuration> config, QWidget *parent) : QWidget(parent),
    config(config),
    legend(new Legend(this->config, this)),
    isolines(new Isolines(this->config, this))
{
    this->setMinimumSize(400, 400);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(this->isolines);
    layout->addWidget(this->legend);

    connect(this->isolines, &Isolines::pointerFunctionValueUpdated, this, [this] (IsolinesMousePosition position) {
        emit pointerFunctionValueUpdated(position);
    });
}
