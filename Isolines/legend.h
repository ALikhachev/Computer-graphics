#ifndef LEGEND_H
#define LEGEND_H

#include <QWidget>
#include <QSharedPointer>
#include <QImage>
#include <QResizeEvent>

#include "configuration.h"
#include "isolines_mouse_position.h"

class Legend : public QWidget
{
    Q_OBJECT
public:
    explicit Legend(QSharedPointer<Configuration> config, QWidget *parent = 0);

public slots:
    void pointerFunctionValueUpdated(IsolinesMousePosition);

protected:
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    void plot();
    void drawBorders();

    QSharedPointer<Configuration> config;
    QImage image;
    double step;
    double current_value;
    bool has_current_value;
    const static int LegendHeight = 30;
    const static int CharWidth = 10;
    const static int HPadding = CharWidth * 3;
    const static int VTopPadding = 20;
    const static int VBottomPadding = 6;
};

#endif // LEGEND_H
