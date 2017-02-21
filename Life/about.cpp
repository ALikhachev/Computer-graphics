#include <QString>

#include "about.h"

About::About(QWidget *parent) : label(new QLabel(QString("FIT_14202_Likhachev_Life"), this)), layout(new QVBoxLayout)
{
    setMinimumSize(300, 200);
    setMaximumSize(300, 200);
    layout->addWidget(label.data());
    setLayout(layout.data());
}
