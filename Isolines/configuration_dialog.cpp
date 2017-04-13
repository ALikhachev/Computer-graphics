#include "configuration_dialog.h"

#include <QFormLayout>
#include <QColorDialog>
#include <QLabel>
#include <QKeyEvent>

ConfigurationDialog::ConfigurationDialog(QSharedPointer<Configuration> config, QWidget *parent) : QDialog(parent),
    config(config),
    grid_cells_x(new QSpinBox(this)),
    grid_cells_y(new QSpinBox(this)),
    domain_x(new RangeDoubleSpinBoxes(this)),
    domain_y(new RangeDoubleSpinBoxes(this)),
    colors_list(new QListWidget(this)),
    remove_selected_color_button(new QPushButton(tr("Remove selected color"), this)),
    isolines_color_button(new QPushButton(this))
{
    this->setModal(true);
    QGridLayout *layout = new QGridLayout(this);
    QFormLayout *form_layout = new QFormLayout();
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
    QPushButton *cancel_button = new QPushButton(tr("Cancel"), this);;
    connect(ok_button, &QPushButton::clicked, this, &ConfigurationDialog::save);
    connect(cancel_button, &QPushButton::clicked, this, &QDialog::close);
    this->setupColorList(layout);
    QPalette pal = this->isolines_color_button->palette();
    this->isolines_color = QColor(this->config->isolinesColor());
    pal.setColor(QPalette::Button, this->isolines_color);
    this->isolines_color_button->setPalette(pal);
    this->isolines_color_button->setAutoFillBackground(true);
    this->isolines_color_button->setFlat(true);
    form_layout->addRow(tr("Isolines color:"), this->isolines_color_button);
    connect(this->isolines_color_button, &QPushButton::clicked, this, &ConfigurationDialog::editIsolinesColor);

    layout->addLayout(form_layout, 0, 0, 1, 3);
    layout->addWidget(ok_button, 4, 1);
    layout->addWidget(cancel_button, 4, 2);

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
    layout->addWidget(new QLabel(tr("Colors of ranges list:"), this), 1, 0, 1, 3);
    layout->addWidget(this->colors_list, 2, 0, 1, 3);
    layout->addWidget(add_color_button, 3, 0);
    layout->addWidget(this->remove_selected_color_button, 3, 1, 1, 2);
    this->remove_selected_color_button->setDisabled(true);
    connect(this->colors_list, &QListWidget::itemSelectionChanged, this, [this] {
        this->remove_selected_color_button->setDisabled(this->colors_list->count() == 1);
    });
    connect(add_color_button, &QPushButton::clicked, this, &ConfigurationDialog::addNewColor);
    connect(remove_selected_color_button, &QPushButton::clicked, this, [this] {
        if (this->colors_list->count() > 1) {
            qDeleteAll(this->colors_list->selectedItems());
        }
        this->remove_selected_color_button->setDisabled(this->colors_list->count() == 1);
    });
}

void ConfigurationDialog::save() {
    if (this->config->horizontalCellCount() != this->grid_cells_x->value()) {
        this->config->setHorizontalCellCount(this->grid_cells_x->value());
    }
    if (this->config->verticalCellCount() != this->grid_cells_y->value()) {
        this->config->setVerticalCellCount(this->grid_cells_y->value());
    }
    if (std::fabs(this->domain_x->minValue() - this->config->startX()) > 1e-15) {
        this->config->setStartX(this->domain_x->minValue());
    }
    if (std::fabs(this->domain_x->maxValue() - this->domain_x->minValue() - this->config->width()) > 1e-15) {
        this->config->setWidth(this->domain_x->maxValue() - this->domain_x->minValue());
    }
    if (std::fabs(this->domain_y->minValue() - this->config->startY()) > 1e-15) {
        this->config->setStartY(this->domain_y->minValue());
    }
    if (std::fabs(this->domain_y->maxValue() - this->domain_y->minValue() - this->config->height()) > 1e-15) {
        this->config->setHeight(this->domain_y->maxValue() - this->domain_y->minValue());
    }
    if (this->config->isolinesColor() != this->isolines_color.rgb()) {
        this->config->setIsolinesColor(this->isolines_color.rgb());
    }
    std::vector<QRgb> levels;
    for (int i = 0; i < this->colors_list->count(); ++i) {
        QColor color = this->colors_list->item(i)->backgroundColor();
        levels.push_back(color.rgb());
    }
    auto old_levels = this->config->levels();

    if (old_levels.size() != levels.size()) {
        this->config->setLevels(levels);
    } else {
        for (auto it1 = old_levels.begin(), it2 = levels.begin();
             it1 < old_levels.end() && it2 < levels.end();
             ++it1, ++it2) {
            if (*it1 != *it2) {
                this->config->setLevels(levels);
                break;
            }
        }
    }
    QDialog::close();
}

void ConfigurationDialog::addNewColor() {
    QColorDialog dialog;
    dialog.setModal(true);
    connect(&dialog, &QColorDialog::colorSelected, this, [this] (const QColor &color) {
        QListWidgetItem *item = new QListWidgetItem(this->colors_list);
        item->setBackgroundColor(color);
        this->colors_list->addItem(item);
        this->remove_selected_color_button->setDisabled(this->colors_list->count() == 1);
    });
    dialog.exec();
}

void ConfigurationDialog::editIsolinesColor() {
    QColorDialog dialog;
    dialog.setModal(true);
    dialog.setCurrentColor(this->isolines_color);
    connect(&dialog, &QColorDialog::colorSelected, this, [this] (const QColor &color) {
        QPalette pal = this->isolines_color_button->palette();
        pal.setColor(QPalette::Button, color);
        this->isolines_color = color;
        this->isolines_color_button->setPalette(pal);
    });
    dialog.exec();
}

void ConfigurationDialog::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
    case Qt::Key_Enter:
    case Qt::Key_Return:
        this->save();
        break;
    case Qt::Key_Escape:
        this->close();
        break;
    }
}
