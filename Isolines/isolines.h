#ifndef ISOLINES_H
#define ISOLINES_H

#include <QWidget>
#include <QSharedPointer>
#include <QImage>
#include <QResizeEvent>

#include "configuration.h"

struct IsolinesMousePosition {
    double x;
    double y;
    double value;
    bool out_of_screen;
};

class Isolines : public QWidget
{
    Q_OBJECT
public:
    explicit Isolines(QSharedPointer<Configuration> config, QWidget *parent = 0);

protected:
    void resizeEvent(QResizeEvent *) override;
    void paintEvent(QPaintEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    bool event(QEvent *) override;

signals:
    void pointerFunctionValueUpdated(IsolinesMousePosition position);

private:
    void plot();
    void drawGrid();
    static inline double f(double x, double y) {
        return std::sin(x) + std::cos(y);
    }

    QSharedPointer<Configuration> config;
    QImage image;
    std::vector<double> isolines;
    double scale_factor_x;
    double scale_factor_y;
    IsolinesMousePosition position;
};

#endif // ISOLINES_H
