#include "board_settings_view.h"

BoardSettingsView::BoardSettingsView(BoardSettings *settings, QWidget * parent) : QDialog(parent),
    settings(settings),
    width_edit(new QLineEdit(this)),
    width_slider(new QSlider(Qt::Horizontal, this)),
    height_edit(new QLineEdit(this)),
    height_slider(new QSlider(Qt::Horizontal, this)),
    cell_size_edit(new QLineEdit(this)),
    cell_size_slider(new QSlider(Qt::Horizontal, this)),
    first_impact_edit(new QLineEdit(this)),
    second_impact_edit(new QLineEdit(this)),
    live_impact_start_edit(new QLineEdit(this)),
    live_impact_end_edit(new QLineEdit(this)),
    birth_impact_start_edit(new QLineEdit(this)),
    birth_impact_end_edit(new QLineEdit(this))
{
    QGridLayout *main_layout = new QGridLayout(this);
    QLabel *width_label = new QLabel(tr("Field width"), this);

    QGroupBox *size_box = new QGroupBox(tr("Field size"), this);
    QGridLayout *size_layout = new QGridLayout(size_box);
    size_box->setLayout(size_layout);
    size_layout->addWidget(width_label, 0, 0);
    size_layout->addWidget(width_edit, 0, 1);
    width_edit->setInputMask(QString("000"));
    connect(width_edit, SIGNAL(editingFinished()), this, SLOT(widthEditChanged()));
    size_layout->addWidget(width_slider, 1, 0, 1, 2);
    width_slider->setRange(WidthMin, WidthMax);
    width_slider->setSingleStep(1);
    width_slider->setTracking(true);
    connect(width_slider, SIGNAL(valueChanged(int)), this, SLOT(widthSliderChanged(int)));

    QLabel *height_label = new QLabel(tr("Field height"), this);
    size_layout->addWidget(height_label, 2, 0);
    size_layout->addWidget(height_edit, 2, 1);
    height_edit->setInputMask(QString("000"));
    connect(height_edit, SIGNAL(editingFinished()), this, SLOT(heightEditChanged()));
    height_slider->setRange(HeightMin, HeightMax);
    height_slider->setSingleStep(1);
    height_slider->setTracking(true);
    connect(height_slider, SIGNAL(valueChanged(int)), this, SLOT(heightSliderChanged(int)));

    size_layout->addWidget(height_slider, 3, 0, 1, 2);

    QGroupBox *rules_box = new QGroupBox(tr("Game rules"), this);
    QVBoxLayout *rules_layout = new QVBoxLayout(rules_box);
    rules_box->setLayout(rules_layout);
    QLabel *first_impact_label = new QLabel(tr("FST_IMPACT"), this);
    rules_layout->addWidget(first_impact_label);
    rules_layout->addWidget(first_impact_edit);
    first_impact_edit->setInputMask(QString("9.9"));
    QLabel *second_impact_label = new QLabel(tr("SND_IMPACT"), this);
    rules_layout->addWidget(second_impact_label);
    rules_layout->addWidget(second_impact_edit);
    second_impact_edit->setInputMask(QString("9.9"));
    QLabel *live_impact_start_label = new QLabel(tr("LIVE_BEGIN"), this);
    rules_layout->addWidget(live_impact_start_label);
    rules_layout->addWidget(live_impact_start_edit);
    live_impact_start_edit->setInputMask(QString("9.9"));
    QLabel *live_impact_end_label = new QLabel(tr("LIVE_END"), this);
    rules_layout->addWidget(live_impact_end_label);
    rules_layout->addWidget(live_impact_end_edit);
    live_impact_end_edit->setInputMask(QString("9.9"));
    QLabel *birth_impact_start_label = new QLabel(tr("BIRTH_BEGIN"), this);
    rules_layout->addWidget(birth_impact_start_label);
    rules_layout->addWidget(birth_impact_start_edit);
    birth_impact_start_edit->setInputMask(QString("9.9"));
    QLabel *birth_impact_end_label = new QLabel(tr("BIRTH_END"), this);
    rules_layout->addWidget(birth_impact_end_label);
    rules_layout->addWidget(birth_impact_end_edit);
    birth_impact_end_edit->setInputMask(QString("9.9"));

    QGroupBox *cell_box = new QGroupBox(tr("Cell properties"), this);
    QGridLayout *cell_layout = new QGridLayout(cell_box);
    cell_box->setLayout(cell_layout);
    QLabel *cell_size_label = new QLabel(tr("Cell size"), this);
    cell_layout->addWidget(cell_size_label, 0, 0);
    cell_layout->addWidget(cell_size_edit, 0, 1);
    cell_layout->addWidget(cell_size_slider, 1, 0, 1, 2);
    cell_size_slider->setRange(CellSizeMin, CellSizeMax);
    cell_size_slider->setSingleStep(1);
    cell_size_slider->setTracking(true);
    connect(cell_size_edit, SIGNAL(editingFinished()), this, SLOT(cellSizeEditChanged()));
    connect(cell_size_slider, SIGNAL(valueChanged(int)), this, SLOT(cellSizeSliderChanged(int)));

    main_layout->addWidget(size_box, 0, 1, 1, 2);
    main_layout->addWidget(cell_box, 1, 1, 1, 2);
    main_layout->addWidget(rules_box, 0, 0, 3, 1);
    QPushButton *ok_button = new QPushButton(tr("Apply"), this);
    main_layout->addWidget(ok_button, 2, 1, Qt::AlignRight);
    QPushButton *cancel_button = new QPushButton(tr("Cancel"), this);
    main_layout->addWidget(cancel_button, 2, 2, Qt::AlignRight);
    setLayout(main_layout);
    connect(ok_button, SIGNAL(released()), this, SLOT(save()));
    connect(cancel_button, SIGNAL(released()), this, SLOT(close()));
}

void BoardSettingsView::widthSliderChanged(int value) {
    width_edit->setText(QString::number(value));
}

void BoardSettingsView::widthEditChanged() {
    int width = width_edit->text().toInt();
    if (width < WidthMin) {
        width = WidthMin;
    } else if (width > WidthMax) {
        width = WidthMax;
    }
    width_slider->setValue(width);
}

void BoardSettingsView::heightSliderChanged(int value) {
    height_edit->setText(QString::number(value));
}

void BoardSettingsView::heightEditChanged() {
    int height = height_edit->text().toInt();
    if (height < HeightMin) {
        height = HeightMin;
    } else if (height > HeightMax) {
        height = HeightMax;
    }
    height_slider->setValue(height);
}

void BoardSettingsView::showEvent(QShowEvent *) {
    width_slider->setValue(settings->width);
    height_slider->setValue(settings->height);
    cell_size_slider->setValue(settings->cellSize);
    first_impact_edit->setText(QString("%1.%2").arg(settings->nearImpact / 10).arg(settings->nearImpact % 10));
    second_impact_edit->setText(QString("%1.%2").arg(settings->futherImpact / 10).arg(settings->futherImpact % 10));
    live_impact_start_edit->setText(QString("%1.%2").arg(settings->liveImpactRange.first / 10).arg(settings->liveImpactRange.first % 10));
    live_impact_end_edit->setText(QString("%1.%2").arg(settings->liveImpactRange.second / 10).arg(settings->liveImpactRange.second % 10));
    birth_impact_start_edit->setText(QString("%1.%2").arg(settings->birthdayImpactRange.first / 10).arg(settings->birthdayImpactRange.first % 10));
    birth_impact_end_edit->setText(QString("%1.%2").arg(settings->birthdayImpactRange.second / 10).arg(settings->birthdayImpactRange.second % 10));
}

void BoardSettingsView::cellSizeSliderChanged(int value) {
    cell_size_edit->setText(QString::number(value));
}

void BoardSettingsView::cellSizeEditChanged() {
    int cell_size = cell_size_edit->text().toInt();
    if (cell_size < CellSizeMin) {
        cell_size = CellSizeMin;
    }
    if (cell_size > CellSizeMax) {
        cell_size = CellSizeMax;
    }
    cell_size_slider->setValue(cell_size);
}

void BoardSettingsView::showError(QString text) {
    QErrorMessage error_message;
    error_message.setModal(true);
    error_message.showMessage(text);
    error_message.exec();
}

void BoardSettingsView::save() {
    int near_impact = first_impact_edit->text().remove(1, 1).toInt();
    int further_impact = second_impact_edit->text().remove(1, 1).toInt();
    int live_impact_start = live_impact_start_edit->text().remove(1, 1).toInt();
    int live_impact_end = live_impact_end_edit->text().remove(1, 1).toInt();
    int birth_impact_start = birth_impact_start_edit->text().remove(1, 1).toInt();
    int birth_impact_end = birth_impact_end_edit->text().remove(1, 1).toInt();

    // validate
    if (near_impact * 6 + further_impact * 6 >= 100) {
        showError(QString("FST_IMPACT * 6 + SND_IMPACT * 6 must be lower than 10.0"));
        return;
    }
    if (live_impact_start > birth_impact_start) {
        showError(QString("LIVE_BEGIN must be lesser or equal to BIRTH_BEGIN"));
        return;
    }
    if (birth_impact_start > birth_impact_end) {
        showError(QString("BIRTH_BEGIN must be lesser or equal to BIRTH_END"));
        return;
    }
    if (birth_impact_end > live_impact_end) {
        showError(QString("BIRTH_END must be lesser or equal to LIVE_END"));
        return;
    }
    bool modelChanged = false;
    bool viewChanged = false;
    if (settings->height != height_slider->value() || settings->width != width_slider->value()) {
        modelChanged = true;
        viewChanged = true;
    }
    if (settings->cellSize != cell_size_slider->value()) {
        viewChanged = true;
    }
    if (settings->nearImpact != near_impact || settings->futherImpact != further_impact) {
        modelChanged = true;
    }
    settings->cellSize = cell_size_slider->value();
    settings->height = height_slider->value();
    settings->width = width_slider->value();
    settings->nearImpact = near_impact;
    settings->futherImpact = further_impact;
    settings->liveImpactRange = std::make_pair(live_impact_start, live_impact_end);
    settings->birthdayImpactRange = std::make_pair(birth_impact_start, birth_impact_end);
    if (modelChanged) {
        settings->updateModel();
    }
    if (viewChanged) {
        settings->updateView();
    }
    close();
}
