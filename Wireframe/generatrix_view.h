#ifndef GENERATRIX_VIEW_H
#define GENERATRIX_VIEW_H

#include <QWidget>

#include <QMouseEvent>
#include <QSharedPointer>

#include "configuration.h"
#include "bspline.h"

class GeneratrixView : public QWidget
{
    Q_OBJECT
public:
    explicit GeneratrixView(QSharedPointer<Configuration> config, QWidget *parent = 0);
    void setObject(QSharedPointer<GeneratrixObject> object);

protected:
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    void plot();

    QImage canvas;
    QSharedPointer<Configuration> config;
    QSharedPointer<GeneratrixObject> _object;
};

#endif // GENERATRIX_VIEW_H
