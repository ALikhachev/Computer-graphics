#include "save_view.h"

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>

SaveView::SaveView(QWidget * parent) : QDialog(parent)
{
    QGridLayout *layout = new QGridLayout(this);
    QLabel *label = new QLabel(tr("Do you want save changes in model?"), this);
    layout->addWidget(label, 0, 0, 1, 2);
    QPushButton *save_button = new QPushButton(tr("Save"), this);
    layout->addWidget(save_button, 1, 0);
    QPushButton *cancel_button = new QPushButton(tr("No"), this);
    layout->addWidget(cancel_button, 1, 1);
    connect(cancel_button, SIGNAL(released()), this, SLOT(close()));
    connect(save_button, SIGNAL(released()), this, SLOT(forwardSave()));
}

void SaveView::forwardSave() {
    emit saveRequested();
    close();
}
