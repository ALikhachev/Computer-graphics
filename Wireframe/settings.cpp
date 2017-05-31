#include "settings.h"

#include <QLabel>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QDoubleSpinBox>
#include <QSpinBox>

Settings::Settings(QSharedPointer<Configuration> configuration, QWidget *parent) : QWidget(parent),
    _config(configuration)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    QFormLayout *form_layout = new QFormLayout();
    QFormLayout *form_layout2 = new QFormLayout();
    QDoubleSpinBox *spin_znear = new QDoubleSpinBox(this);
    QDoubleSpinBox *spin_zfar = new QDoubleSpinBox(this);
    spin_znear->setValue(this->_config->clippingNearDistance());
    form_layout->addRow(tr("Z near"), spin_znear);
    connect(spin_znear, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, [this, spin_zfar] (double value) {
        this->_config->setClippingNearDistance(value);
        this->_config->update();
        spin_zfar->setMinimum(value + 0.1f);
    });


    spin_zfar->setValue(this->_config->clippingFarDistance());
    form_layout->addRow(tr("Z far"), spin_zfar);
    connect(spin_zfar, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, [this, spin_znear] (double value) {
        this->_config->setClippingFarDistance(value);
        this->_config->update();
        spin_znear->setMaximum(value - 0.1f);
    });

    QSpinBox *spin_current_object = new QSpinBox(this);
    spin_current_object->setMaximum(this->_config->objects().size() - 1);

    form_layout2->addRow(tr("Object #"), spin_current_object);

    connect(spin_current_object, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, [this] (int value) {
        this->_config->setCurrentObject(value);
    });

    layout->addLayout(form_layout);
    layout->addLayout(form_layout2);
}
