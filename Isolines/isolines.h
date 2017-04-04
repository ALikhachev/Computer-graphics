#ifndef ISOLINES_H
#define ISOLINES_H

#include <QWidget>
#include <QSharedPointer>
#include <QImage>
#include <QResizeEvent>

#include "configuration.h"

class Isolines : public QWidget
{
    Q_OBJECT
public:
    explicit Isolines(QSharedPointer<Configuration> config, QWidget *parent = 0);

protected:
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    void plot();
    static inline double f(double x, double y) {
        return std::sin(x) + std::cos(y);
    }

    QSharedPointer<Configuration> config;
    QImage image;
    double min;
    double max;
    double step;
    std::vector<double> isolines;
    double mul;
};

#endif // ISOLINES_H
