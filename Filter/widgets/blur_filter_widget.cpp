#include <QFormLayout>
#include <QComboBox>

#include "widgets/blur_filter_widget.h"
#include "filter_registry.h"
#include "qsliderbox.h"


BlurFilterWidget::BlurFilterWidget(BlurFilter *f, QWidget *parent) : FilterWidget(parent),
    filter(f),
    settings(f->getSettings())
{
    QFormLayout *layout = new QFormLayout(this);
    QComboBox *box = new QComboBox(this);
    box->addItem(tr("Blur"));
    box->addItem(tr("Box blur"));
    box->addItem(tr("Gauss blur (3x3)"));
    box->addItem(tr("Gauss blur (5x5)"));
    layout->addRow(tr("Type"), box);
    connect(box, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [this] (int i) {
        this->settings->matrix_index = i;
        this->filter->request();
    });
}

namespace {
    bool reg = FilterRegistry::getInstance().registerWidgetBuilder(QObject::tr("Blur"), [] (Filter *f, QWidget *parent) {
        BlurFilter *blur_filter = dynamic_cast<BlurFilter *>(f);
        return new BlurFilterWidget(blur_filter, parent);
    });
}
