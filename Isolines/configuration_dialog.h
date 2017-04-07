#ifndef CONFIGURATION_DIALOG_H
#define CONFIGURATION_DIALOG_H

#include <QDialog>
#include <QWidget>
#include <QSharedPointer>
#include <QSpinBox>

#include "configuration.h"
#include "range_spin_boxes.h"

class ConfigurationDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ConfigurationDialog(QSharedPointer<Configuration> config, QWidget *parent = 0);

private slots:
    void save();

private:
    QSharedPointer<Configuration> config;

    QSpinBox *grid_cells_x;
    QSpinBox *grid_cells_y;
    RangeDoubleSpinBoxes *domain_x;
    RangeDoubleSpinBoxes *domain_y;
};

#endif // CONFIGURATION_DIALOG_H
