#ifndef ABOUT_H
#define ABOUT_H

#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QScopedPointer>

class AboutView : public QDialog
{
public:
    AboutView(QWidget * parent = 0);

private:
    QLabel label;
    QVBoxLayout layout;
};

#endif // ABOUT_H
