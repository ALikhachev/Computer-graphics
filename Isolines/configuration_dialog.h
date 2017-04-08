#ifndef CONFIGURATION_DIALOG_H
#define CONFIGURATION_DIALOG_H

#include <QDialog>
#include <QWidget>
#include <QSharedPointer>
#include <QSpinBox>
#include <QListWidgetItem>
#include <QListWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QColor>

#include "configuration.h"
#include "range_spin_boxes.h"

class ConfigurationDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ConfigurationDialog(QSharedPointer<Configuration> config, QWidget *parent = 0);

private slots:
    void save();
    void addNewColor();
    void editIsolinesColor();

private:
    void setupColorList(QGridLayout *layout);

    QSharedPointer<Configuration> config;
    QSpinBox *grid_cells_x;
    QSpinBox *grid_cells_y;
    RangeDoubleSpinBoxes *domain_x;
    RangeDoubleSpinBoxes *domain_y;
    QListWidget *colors_list;
    QPushButton *remove_selected_color_button;
    QPushButton *isolines_color_button;
    QColor isolines_color;
};

#endif // CONFIGURATION_DIALOG_H
