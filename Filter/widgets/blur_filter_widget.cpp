#include <QFormLayout>
#include <QComboBox>

#include "widgets/blur_filter_widget.h"
#include "filter_registry.h"
#include "qsliderbox.h"


BlurFilterWidget::BlurFilterWidget(BlurFilter *f, QWidget *parent) : FilterWidget(parent),
    filter(f),
    settings(f->getSettings()),
    box(new QComboBox(this))
{
    QFormLayout *layout = new QFormLayout(this);
    this->box->addItem(tr("Blur"));
    this->box->addItem(tr("Box blur"));
    this->box->addItem(tr("Gauss blur (3x3)"));
    this->box->addItem(tr("Gauss blur (5x5)"));
    layout->addRow(tr("Type"), this->box);
    connect(this->box, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [this] (int i) {
        this->settings->matrix_index = i;
        this->filter->request();
    });
}

void BlurFilterWidget::settingsUpdate() {
    this->settings = this->filter->getSettings();
    this->box->setCurrentIndex(this->settings->matrix_index);
}

namespace {
    bool reg = FilterRegistry::getInstance().registerWidgetBuilder(QObject::tr("Blur"), [] (Filter *f, QWidget *parent) {
        BlurFilter *blur_filter = dynamic_cast<BlurFilter *>(f);
        return new BlurFilterWidget(blur_filter, parent);
    });
}
