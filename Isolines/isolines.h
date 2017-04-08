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
    void mousePressEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    bool event(QEvent *) override;

signals:
    void pointerFunctionValueUpdated(IsolinesMousePosition position);

private slots:
    void resizeImage();
    void repaint();
    void replot();

private:
    void paint();
    void plot();
    void drawGrid();
    void drawIsolines();
    static inline double f(double x, double y) {
        return std::sin(x) + std::cos(y);
    }

    QSharedPointer<Configuration> config;
    QImage image;
    QImage image_plot;
    double user_isoline;
    bool has_user_isoline;
    double scale_factor_x;
    double scale_factor_y;
    IsolinesMousePosition position;
    bool continious_isoline_mode;
    const static int CircleRadius = 3;
    bool need_replot;
};

#endif // ISOLINES_H
