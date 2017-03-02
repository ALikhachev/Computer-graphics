#ifndef SAVEVIEW_H
#define SAVEVIEW_H

#include <QDialog>

class SaveView : public QDialog
{
    Q_OBJECT

public:
    SaveView(QWidget * parent = 0);

private slots:
    void forwardSave();

signals:
    void saveRequested();
};

#endif // SAVEVIEW_H
