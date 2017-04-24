#ifndef GENERATRIX_VIEW_H
#define GENERATRIX_VIEW_H

#include <QWidget>

#include <QMouseEvent>
#include "bspline.h"

class GeneratrixView : public QWidget
{
    Q_OBJECT
public:
    explicit GeneratrixView(QWidget *parent = 0);

protected:
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    void plot();

    std::vector<QPoint> knots;
    QImage canvas;
    BSpline spline;
};

#endif // GENERATRIX_VIEW_H
