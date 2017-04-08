#include "configuration_dialog.h"

#include <QFormLayout>
#include <QPushButton>
#include <QColor>
#include <QColorDialog>
#include <QLabel>

ConfigurationDialog::ConfigurationDialog(QSharedPointer<Configuration> config, QWidget *parent) : QDialog(parent),
    config(config),
    grid_cells_x(new QSpinBox(this)),
    grid_cells_y(new QSpinBox(this)),
    domain_x(new RangeDoubleSpinBoxes(this)),
    domain_y(new RangeDoubleSpinBoxes(this)),
    colors_list(new QListWidget(this))
{
    this->setModal(true);
    QGridLayout *layout = new QGridLayout(this);
    QFormLayout *form_layout = new QFormLayout();
    layout->addLayout(form_layout, 0, 0, 1, 4);
    this->grid_cells_x->setRange(1, 100);
    this->grid_cells_x->setValue(this->config->horizontalCellCount());
    this->grid_cells_y->setRange(1, 100);
    this->grid_cells_y->setValue(this->config->verticalCellCount());
    this->domain_x->setRange(-10000.0, 10000);
    this->domain_x->setMinValue(this->config->startX());
    this->domain_x->setMaxValue(this->config->startX() + this->config->width());
    this->domain_y->setRange(-10000.0, 10000);
    this->domain_y->setMinValue(this->config->startY());
    this->domain_y->setMaxValue(this->config->startY() + this->config->height());
    form_layout->addRow(tr("Grid cells (X):"), this->grid_cells_x);
    form_layout->addRow(tr("Grid cells (Y):"), this->grid_cells_y);
    form_layout->addRow(tr("Domain (X):"), this->domain_x);
    form_layout->addRow(tr("Domain (Y):"), this->domain_y);
    QPushButton *ok_button = new QPushButton(tr("Save"), this);
    QPushButton *cancel_button = new QPushButton(tr("Cancel"), this);
    layout->addWidget(ok_button, 4, 2);
    layout->addWidget(cancel_button, 4, 3);
    connect(ok_button, &QPushButton::clicked, this, &ConfigurationDialog::save);
    connect(cancel_button, &QPushButton::clicked, this, &QDialog::close);
    this->setupColorList(layout);
}

void ConfigurationDialog::setupColorList(QGridLayout *layout) {
    this->colors_list->setDragDropMode(QAbstractItemView::DragDrop);
    this->colors_list->setDefaultDropAction(Qt::MoveAction);
    this->colors_list->setSelectionMode(QAbstractItemView::SingleSelection);
    auto levels = this->config->levels();
    for (auto it = levels.begin(); it < levels.end(); ++it) {
        QListWidgetItem *item = new QListWidgetItem(this->colors_list);
        item->setBackgroundColor(QColor(*it));
        this->colors_list->addItem(item);
    };
    QPushButton *add_color_button = new QPushButton(tr("Add color"), this);
    QPushButton *remove_selected_color = new QPushButton(tr("Remove selected color"), this);

    layout->addWidget(new QLabel(tr("Colors of ranges list:"), this), 1, 0, 1, 4);
    layout->addWidget(this->colors_list, 2, 0, 1, 4);
    layout->addWidget(add_color_button, 3, 0, 1, 2);
    layout->addWidget(remove_selected_color, 3, 2, 1, 2);
    remove_selected_color->setDisabled(true);
    connect(this->colors_list, &QListWidget::itemSelectionChanged, this, [this, remove_selected_color] {
        remove_selected_color->setDisabled(this->colors_list->count() == 1);
    });
    connect(add_color_button, &QPushButton::clicked, this, &ConfigurationDialog::addNewColor);
    connect(remove_selected_color, &QPushButton::clicked, this, [remove_selected_color, this] {
        if (this->colors_list->count() > 1) {
            qDeleteAll(this->colors_list->selectedItems());
        }
        remove_selected_color->setDisabled(this->colors_list->count() == 1);
    });
}

void ConfigurationDialog::save() {
    this->config->setHorizontalCellCount(this->grid_cells_x->value());
    this->config->setVerticalCellCount(this->grid_cells_y->value());
    this->config->setStartX(this->domain_x->minValue());
    this->config->setWidth(this->domain_x->maxValue() - this->domain_x->minValue());
    this->config->setStartY(this->domain_y->minValue());
    this->config->setHeight(this->domain_y->maxValue() - this->domain_y->minValue());
    this->config->update();
    std::vector<QRgb> levels;
    for (int i = 0; i < this->colors_list->count(); ++i) {
        QColor color = this->colors_list->item(i)->backgroundColor();
        levels.push_back(color.rgb());
    }
    this->config->setLevels(levels);
    QDialog::close();
}

void ConfigurationDialog::addNewColor() {
    QColorDialog dialog;
    dialog.setModal(true);
    connect(&dialog, &QColorDialog::colorSelected, this, [this] (const QColor &color) {
        QListWidgetItem *item = new QListWidgetItem(this->colors_list);
        item->setBackgroundColor(color);
        this->colors_list->addItem(item);
    });
    dialog.exec();
}
