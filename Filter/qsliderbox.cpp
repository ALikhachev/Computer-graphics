#include <QHBoxLayout>
#include <QSpinBox>
#include <QDebug>

#include "qsliderbox.h"

QSliderBox::QSliderBox(int min, int max, QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setMargin(0);
    QSpinBox *spin_box = new QSpinBox(this);
    spin_box->setRange(min, max);
    this->slider = new QSlider(Qt::Horizontal, this);
    this->slider->setRange(min, max);
    layout->addWidget(this->slider);
    layout->addWidget(spin_box);

    connect(slider, &QSlider::valueChanged, this, [this] (int value) {
        emit QSliderBox::valueChanged(value);
    });
    connect(spin_box, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this->slider, &QSlider::setValue);
    connect(this->slider, &QSlider::valueChanged, spin_box, &QSpinBox::setValue);
}

void QSliderBox::setValue(int i) {
    this->slider->setValue(i);
}

int QSliderBox::getValue() {
    return this->slider->value();
}
