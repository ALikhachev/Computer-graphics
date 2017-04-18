#include "range_spin_boxes.h"

#include <QHBoxLayout>
#include <QLabel>

RangeDoubleSpinBoxes::RangeDoubleSpinBoxes(QWidget *parent) : QWidget(parent),
    spin_box_min(new QDoubleSpinBox),
    spin_box_max(new QDoubleSpinBox)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setMargin(0);
    layout->addWidget(new QLabel(tr("from"), this));
    layout->addWidget(this->spin_box_min);
    layout->addWidget(new QLabel(tr("to"), this));
    layout->addWidget(this->spin_box_max);
    this->spin_box_min->setSingleStep(0.5);
    this->spin_box_max->setSingleStep(0.5);

    connect(this->spin_box_min, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &RangeDoubleSpinBoxes::minSpinBoxChanged);
    connect(this->spin_box_max, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &RangeDoubleSpinBoxes::maxSpinBoxChanged);
}

void RangeDoubleSpinBoxes::setMinValue(double val) {
    this->spin_box_min->setValue(val);
}

void RangeDoubleSpinBoxes::setMaxValue(double val) {
    this->spin_box_max->setValue(val);
}

double RangeDoubleSpinBoxes::minValue() const {
    return this->spin_box_min->value();
}

double RangeDoubleSpinBoxes::maxValue() const {
    return this->spin_box_max->value();
}

void RangeDoubleSpinBoxes::setMin(double min) {
    this->spin_box_min->setMinimum(min);
    this->spin_box_max->setMinimum(min);
}

void RangeDoubleSpinBoxes::setMax(double max) {
    this->spin_box_min->setMaximum(max);
    this->spin_box_max->setMaximum(max);
}

void RangeDoubleSpinBoxes::setRange(double min, double max) {
    this->spin_box_min->setRange(min, max);
    this->spin_box_max->setRange(min, max);
}

void RangeDoubleSpinBoxes::minSpinBoxChanged(double min) {
    this->spin_box_max->setMinimum(min);
    emit minChanged(min);
}

void RangeDoubleSpinBoxes::maxSpinBoxChanged(double max) {
    this->spin_box_min->setMaximum(max);
    emit maxChanged(max);
}
