#include <QString>
#include <QPixmap>
#include <QHBoxLayout>
#include <QLabel>

#include "about_view.h"

AboutView::AboutView(QWidget *parent) : QDialog(parent)
{
    setMinimumSize(350, 200);
    setMaximumSize(350, 200);
    QHBoxLayout *layout = new QHBoxLayout(this);
    QLabel *photo = new QLabel(this);
    QPixmap pixmap = QPixmap(":/photos/author.jpg");
    photo->setPixmap(pixmap);
    layout->addWidget(photo);
    QLabel *description = new QLabel(tr("Conway's Game of Life\n\nАвтор: Александр Лихачев\nФИТ НГУ\nГруппа 14202"), this);
    layout->addWidget(description);
    setLayout(layout);
}
