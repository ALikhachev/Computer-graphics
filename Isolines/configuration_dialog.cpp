#include "configuration_dialog.h"

#include <QFormLayout>
#include <QGridLayout>
#include <QPushButton>

ConfigurationDialog::ConfigurationDialog(QSharedPointer<Configuration> config, QWidget *parent) : QDialog(parent),
    config(config),
    grid_cells_x(new QSpinBox(this)),
    grid_cells_y(new QSpinBox(this)),
    domain_x(new RangeDoubleSpinBoxes(this)),
    domain_y(new RangeDoubleSpinBoxes(this))
{
    this->setModal(true);
    QGridLayout *layout = new QGridLayout(this);
    QFormLayout *form_layout = new QFormLayout();
    layout->addLayout(form_layout, 0, 0, 1, 3);
    this->grid_cells_x->setRange(0, 200);
    this->grid_cells_x->setValue(this->config->horizontalCellCount());
    this->grid_cells_y->setRange(0, 200);
    this->grid_cells_y->setValue(this->config->verticalCellCount());
    this->domain_x->setRange(-10000.0, 10000);
    this->domain_x->setMinValue(this->config->startX());
    this->domain_x->setMaxValue(this->config->startX() + this->config->width());
    this->domain_y->setRange(-10000.0, 10000);
    this->domain_y->setMinValue(this->config->startY());
    this->domain_y->setMaxValue(this->config->startY() + this->config->height());
    form_layout->addRow(tr("Grid cells (X):"), this->grid_cells_x);
    form_layout->addRow(tr("Grid cells (Y):"), this->grid_cells_y);
    form_layout->addRow(tr("Domain (X):"), this->domain_x);
    form_layout->addRow(tr("Domain (Y):"), this->domain_y);
    QPushButton *ok_button = new QPushButton(tr("Save"), this);
    QPushButton *cancel_button = new QPushButton(tr("Cancel"), this);
    layout->addWidget(ok_button, 1, 1);
    layout->addWidget(cancel_button, 1, 2);
    connect(ok_button, &QPushButton::clicked, this, &ConfigurationDialog::save);
    connect(cancel_button, &QPushButton::clicked, this, &QDialog::close);
}

void ConfigurationDialog::save() {
    this->config->setHorizontalCellCount(this->grid_cells_x->value());
    this->config->setVerticalCellCount(this->grid_cells_y->value());
    this->config->setStartX(this->domain_x->minValue());
    this->config->setWidth(this->domain_x->maxValue() - this->domain_x->minValue());
    this->config->setStartY(this->domain_y->minValue());
    this->config->setHeight(this->domain_y->maxValue() - this->domain_y->minValue());
    this->config->update();
    QDialog::close();
}
