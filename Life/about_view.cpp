#include <QString>

#include "about_view.h"

AboutView::AboutView(QWidget *parent) : QDialog(parent),
    label(tr("FIT_14202_Likhachev_Life"), this),
    layout(this)
{
    setMinimumSize(300, 200);
    setMaximumSize(300, 200);
    layout.addWidget(&label);
    setLayout(&layout);
}
