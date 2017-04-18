#include <QString>
#include <QPixmap>
#include <QGridLayout>
#include <QLabel>

#include "about_view.h"

AboutView::AboutView(QWidget *parent) : QDialog(parent)
{
    setMinimumSize(350, 200);
    setMaximumSize(350, 200);
    QGridLayout *layout = new QGridLayout(this);
    QLabel *photo = new QLabel(this);
    QPixmap pixmap = QPixmap(":/photos/author.jpg");
    photo->setPixmap(pixmap);
    QLabel *about = new QLabel(QString("Isolines\n\nAuthor: Alexander Likhachev\nFIT NSU\nGroup 14202"), this);
    QLabel *desc = new QLabel(tr("Use <b>left mouse button</b> to define extra isoline<br/>"
                                 "Use <b>right mouse button</b> to delete it<br/>"
                                 "(in isolines showing mode)"), this);
    desc->setWordWrap(true);
    layout->addWidget(photo, 0, 0, 3, 2);
    layout->addWidget(about, 0, 2);
    layout->addWidget(desc, 2, 2);
}
