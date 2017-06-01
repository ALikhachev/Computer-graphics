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
    QFormLayout *form_layout3 = new QFormLayout();
    QFormLayout *form_layout4 = new QFormLayout();
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

    QDoubleSpinBox *spin_sw = new QDoubleSpinBox(this);
    spin_sw->setRange(1, 20);
    spin_sw->setSingleStep(0.1);
    spin_sw->setValue(this->_config->sw());
    form_layout->addRow(tr("sw"), spin_sw);

    connect(spin_sw, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, [this] (double value) {
        this->_config->setSw(value);
        this->_config->update();
    });

    QDoubleSpinBox *spin_sh = new QDoubleSpinBox(this);
    spin_sh->setRange(1, 20);
    spin_sh->setSingleStep(0.1);
    spin_sh->setValue(this->_config->sh());
    form_layout->addRow(tr("sh"), spin_sh);

    connect(spin_sh, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, [this] (double value) {
        this->_config->setSh(value);
        this->_config->update();
    });

    QSpinBox *spin_current_object = new QSpinBox(this);
    spin_current_object->setMaximum(this->_config->objects().size() - 1);

    form_layout2->addRow(tr("Object #"), spin_current_object);

    QDoubleSpinBox *spin_curx = new QDoubleSpinBox(this);
    spin_curx->setRange(-100, 100);
    spin_curx->setValue(this->_config->currentX());
    form_layout2->addRow(tr("x"), spin_curx);

    connect(spin_curx, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, [this] (double value) {
        this->_config->setCurrentX(value);
        this->_config->update();
    });

    QDoubleSpinBox *spin_cury = new QDoubleSpinBox(this);
    spin_cury->setRange(-100, 100);
    spin_cury->setValue(this->_config->currentY());
    form_layout2->addRow(tr("y"), spin_cury);

    connect(spin_cury, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, [this] (double value) {
        this->_config->setCurrentY(value);
        this->_config->update();
    });

    QDoubleSpinBox *spin_curz = new QDoubleSpinBox(this);
    spin_curz->setRange(-100, 100);
    spin_curz->setValue(this->_config->currentZ());
    form_layout2->addRow(tr("z"), spin_curz);

    connect(spin_curz, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, [this] (double value) {
        this->_config->setCurrentZ(value);
        this->_config->update();
    });

    QSpinBox *spin_n = new QSpinBox(this);
    spin_n->setValue(this->_config->n());
    spin_n->setRange(1, 50);
    form_layout3->addRow(tr("n"), spin_n);
    connect(spin_n, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, [this] (int value) {
        this->_config->setN(value);
        this->_config->update();
    });

    QSpinBox *spin_m = new QSpinBox(this);
    spin_m->setValue(this->_config->m());
    spin_m->setRange(1, 50);
    form_layout3->addRow(tr("m"), spin_m);
    connect(spin_m, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, [this] (int value) {
        this->_config->setM(value);
        this->_config->update();
    });

    QSpinBox *spin_k = new QSpinBox(this);
    spin_k->setValue(this->_config->k());
    spin_k->setRange(1, 50);
    form_layout3->addRow(tr("k"), spin_k);
    connect(spin_k, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, [this] (int value) {
        this->_config->setK(value);
        this->_config->update();
    });

    QDoubleSpinBox *spin_a= new QDoubleSpinBox(this);
    spin_a->setRange(0, 1);
    spin_a->setValue(this->_config->a());
    form_layout4->addRow(tr("a"), spin_a);

    connect(spin_a, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, [this] (double value) {
        this->_config->setA(value);
        this->_config->update();
    });

    QDoubleSpinBox *spin_b= new QDoubleSpinBox(this);
    spin_b->setRange(0, 1);
    spin_b->setValue(this->_config->b());
    form_layout4->addRow(tr("b"), spin_b);

    connect(spin_b, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, [this] (double value) {
        this->_config->setB(value);
        this->_config->update();
    });

    QDoubleSpinBox *spin_c = new QDoubleSpinBox(this);
    spin_c->setDecimals(5);
    spin_c->setRange(0, 2 * PI);
    spin_c->setValue(this->_config->c());
    form_layout4->addRow(tr("c"), spin_c);

    connect(spin_c, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, [this] (double value) {
        this->_config->setC(value);
        this->_config->update();
    });

    QDoubleSpinBox *spin_d = new QDoubleSpinBox(this);
    spin_d->setDecimals(5);
    spin_d->setRange(0, 2 * PI);
    spin_d->setValue(this->_config->d());
    form_layout4->addRow(tr("d"), spin_d);

    connect(spin_d, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, [this] (double value) {
        this->_config->setD(value);
        this->_config->update();
    });

    layout->addLayout(form_layout);
    layout->addLayout(form_layout2);
    layout->addLayout(form_layout3);
    layout->addLayout(form_layout4);

    connect(spin_current_object, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, [this, spin_curx, spin_cury, spin_curz] (int value) {
        this->_config->setCurrentObject(value);
        spin_curx->setValue(this->_config->currentX());
        spin_cury->setValue(this->_config->currentY());
        spin_curz->setValue(this->_config->currentZ());
    });

    connect(configuration.data(), &Configuration::updated, this, [this, spin_znear, spin_zfar, spin_sw, spin_sh,
            spin_current_object, spin_curx, spin_cury, spin_curz, spin_n, spin_m, spin_k, spin_a, spin_b, spin_c, spin_d] {
        spin_znear->setValue(this->_config->clippingNearDistance());
        spin_zfar->setValue(this->_config->clippingFarDistance());
        spin_sw->setValue(this->_config->sw());
        spin_sh->setValue(this->_config->sh());
        spin_current_object->setValue(0);
        spin_current_object->setMaximum(this->_config->objects().size() - 1);
        spin_curx->setValue(this->_config->currentX());
        spin_cury->setValue(this->_config->currentY());
        spin_curz->setValue(this->_config->currentZ());
        spin_n->setValue(this->_config->n());
        spin_m->setValue(this->_config->m());
        spin_k->setValue(this->_config->k());
        spin_a->setValue(this->_config->a());
        spin_b->setValue(this->_config->b());
        spin_c->setValue(this->_config->c());
        spin_d->setValue(this->_config->d());
    });
}
