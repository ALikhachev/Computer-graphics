#include <QString>

#include "about_view.h"

AboutView::AboutView(QWidget *parent) : QDialog(parent)
{
    setMinimumSize(300, 200);
    setMaximumSize(300, 200);
    QVBoxLayout *layout = new QVBoxLayout(this);
    QLabel *label = new QLabel(tr("FIT_14202_Likhachev_Life"), this);
    layout->addWidget(label);
    setLayout(layout);
}
