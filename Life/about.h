#ifndef ABOUT_H
#define ABOUT_H

#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QScopedPointer>

class About : public QDialog
{
public:
    About(QWidget * parent = 0);

private:
    QScopedPointer<QVBoxLayout> layout;
    QScopedPointer<QLabel> label;
};

#endif // ABOUT_H
