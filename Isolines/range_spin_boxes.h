#ifndef RANGE_SPIN_BOXES_H
#define RANGE_SPIN_BOXES_H

#include <QWidget>
#include <QDoubleSpinBox>

class RangeDoubleSpinBoxes : public QWidget
{
    Q_OBJECT
public:
    explicit RangeDoubleSpinBoxes(QWidget *parent = 0);
    /** Sets the value of spinbox for minimum */
    void setMinValue(double);
    double minValue() const;
    /** Sets the value of spinbox for maximum */
    void setMaxValue(double);
    double maxValue() const;
    void setMin(double);
    void setMax(double);
    void setRange(double, double);

signals:
    void minChanged(double);
    void maxChanged(double);

private slots:
    void minSpinBoxChanged(double);
    void maxSpinBoxChanged(double);

private:
    QDoubleSpinBox *spin_box_min;
    QDoubleSpinBox *spin_box_max;
};

#endif // RANGE_SPIN_BOXES_H
