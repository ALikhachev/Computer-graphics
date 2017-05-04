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
    QLabel *about = new QLabel(QString("Wireframe\n\nAuthor: Alexander Likhachev\nFIT NSU\nGroup 14202"), this);
    layout->addWidget(photo, 0, 0, 3, 2);
    layout->addWidget(about, 0, 2, 3, 1);
}
