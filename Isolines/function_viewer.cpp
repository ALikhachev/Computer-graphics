#include "function_viewer.h"

#include <QVBoxLayout>

FunctionViewer::FunctionViewer(QWidget *parent) : QWidget(parent),
    config(new Configuration),
    legend(new Legend(this->config, this)),
    isolines(new Isolines(this->config, this))
{
    this->setMinimumSize(400, 400);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(this->isolines);
    layout->addWidget(this->legend);
}

bool FunctionViewer::readConfiguration(QString path) {
    // ...
}
