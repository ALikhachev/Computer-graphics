#include "board_settings_view.h"

BoardSettingsView::BoardSettingsView(BoardSettings *settings, QWidget * parent) : QDialog(parent),
    settings(settings),
    mainLayout(this),
    okButton(tr("Apply"), this),
    cancelButton(tr("Cancel"), this),
    sizeBox(tr("Field size"), this),
    width_label(tr("Field width")),
    width_slider(Qt::Horizontal),
    height_label(tr("Field height")),
    height_slider(Qt::Horizontal),
    cellBox(tr("Cell properties"), this),
    cell_size_label(tr("Cell size")),
    cell_size_slider(Qt::Horizontal),
    rulesBox(tr("Game rules"), this),
    first_impact_label(tr("FST_IMPACT")),
    first_impact_edit(),
    second_impact_label(tr("SND_IMPACT")),
    live_impact_start_label(tr("LIVE_BEGIN")),
    live_impact_end_label(tr("LIVE_END")),
    birth_impact_start_label(tr("BIRTH_START")),
    birth_impact_end_label(tr("BIRTH_END")),
    error_message(this)
{
    error_message.setModal(true);
    sizeLayout.addWidget(&width_label, 0, 0);
    sizeLayout.addWidget(&width_edit, 0, 1);
    width_edit.setInputMask(QString("000"));
    connect(&width_edit, SIGNAL(editingFinished()), this, SLOT(widthEditChanged()));
    sizeLayout.addWidget(&width_slider, 1, 0, 1, 2);
    width_slider.setRange(WidthMin, WidthMax);
    width_slider.setSingleStep(1);
    width_slider.setTracking(true);
    connect(&width_slider, SIGNAL(valueChanged(int)), this, SLOT(widthSliderChanged(int)));
    sizeLayout.addWidget(&height_label, 2, 0);
    sizeLayout.addWidget(&height_edit, 2, 1);
    height_edit.setInputMask(QString("000"));
    connect(&height_edit, SIGNAL(editingFinished()), this, SLOT(heightEditChanged()));
    height_slider.setRange(HeightMin, HeightMax);
    height_slider.setSingleStep(1);
    height_slider.setTracking(true);
    connect(&height_slider, SIGNAL(valueChanged(int)), this, SLOT(heightSliderChanged(int)));

    sizeLayout.addWidget(&height_slider, 3, 0, 1, 2);
    sizeBox.setLayout(&sizeLayout);

    rules_layout.addWidget(&first_impact_label);
    rules_layout.addWidget(&first_impact_edit);
    first_impact_edit.setInputMask(QString("9.9"));
    rules_layout.addWidget(&second_impact_label);
    rules_layout.addWidget(&second_impact_edit);
    second_impact_edit.setInputMask(QString("9.9"));
    rules_layout.addWidget(&live_impact_start_label);
    rules_layout.addWidget(&live_impact_start_edit);
    live_impact_start_edit.setInputMask(QString("9.9"));
    rules_layout.addWidget(&live_impact_end_label);
    rules_layout.addWidget(&live_impact_end_edit);
    live_impact_end_edit.setInputMask(QString("9.9"));
    rules_layout.addWidget(&birth_impact_start_label);
    rules_layout.addWidget(&birth_impact_start_edit);
    birth_impact_start_edit.setInputMask(QString("9.9"));
    rules_layout.addWidget(&birth_impact_end_label);
    rules_layout.addWidget(&birth_impact_end_edit);
    birth_impact_end_edit.setInputMask(QString("9.9"));
    rulesBox.setLayout(&rules_layout);

    cell_layout.addWidget(&cell_size_label, 0, 0);
    cell_layout.addWidget(&cell_size_edit, 0, 1);
    cell_layout.addWidget(&cell_size_slider, 1, 0, 1, 2);
    cell_size_slider.setRange(CellSizeMin, CellSizeMax);
    cell_size_slider.setSingleStep(1);
    cell_size_slider.setTracking(true);
    connect(&cell_size_edit, SIGNAL(editingFinished()), this, SLOT(cellSizeEditChanged()));
    connect(&cell_size_slider, SIGNAL(valueChanged(int)), this, SLOT(cellSizeSliderChanged(int)));
    cellBox.setLayout(&cell_layout);

    mainLayout.addWidget(&sizeBox, 0, 1, 1, 2);
    mainLayout.addWidget(&cellBox, 1, 1, 1, 2);
    mainLayout.addWidget(&rulesBox, 0, 0, 3, 1);
    mainLayout.addWidget(&okButton, 2, 1, Qt::AlignRight);
    mainLayout.addWidget(&cancelButton, 2, 2, Qt::AlignRight);
    setLayout(&mainLayout);
    connect(&okButton, SIGNAL(released()), this, SLOT(save()));
    connect(&cancelButton, SIGNAL(released()), this, SLOT(hide()));
}

void BoardSettingsView::widthSliderChanged(int value) {
    width_edit.setText(QString::number(value));
}

void BoardSettingsView::widthEditChanged() {
    int width = width_edit.text().toInt();
    if (width < WidthMin) {
        width = WidthMin;
    } else if (width > WidthMax) {
        width = WidthMax;
    }
    width_slider.setValue(width);
}

void BoardSettingsView::heightSliderChanged(int value) {
    height_edit.setText(QString::number(value));
}

void BoardSettingsView::heightEditChanged() {
    int height = height_edit.text().toInt();
    if (height < HeightMin) {
        height = HeightMin;
    } else if (height > HeightMax) {
        height = HeightMax;
    }
    height_slider.setValue(height);
}

void BoardSettingsView::showEvent(QShowEvent *) {
    width_slider.setValue(settings->width);
    height_slider.setValue(settings->height);
    cell_size_slider.setValue(settings->cellSize);
    first_impact_edit.setText(QString("%1.%2").arg(settings->nearImpact / 10).arg(settings->nearImpact % 10));
    second_impact_edit.setText(QString("%1.%2").arg(settings->futherImpact / 10).arg(settings->futherImpact % 10));
    live_impact_start_edit.setText(QString("%1.%2").arg(settings->liveImpactRange.first / 10).arg(settings->liveImpactRange.first % 10));
    live_impact_end_edit.setText(QString("%1.%2").arg(settings->liveImpactRange.second / 10).arg(settings->liveImpactRange.second % 10));
    birth_impact_start_edit.setText(QString("%1.%2").arg(settings->birthdayImpactRange.first / 10).arg(settings->birthdayImpactRange.first % 10));
    birth_impact_end_edit.setText(QString("%1.%2").arg(settings->birthdayImpactRange.second / 10).arg(settings->birthdayImpactRange.second % 10));
}

void BoardSettingsView::cellSizeSliderChanged(int value) {
    cell_size_edit.setText(QString::number(value));
}

void BoardSettingsView::cellSizeEditChanged() {
    int cell_size = cell_size_edit.text().toInt();
    if (cell_size < CellSizeMin) {
        cell_size = CellSizeMin;
    }
    if (cell_size > CellSizeMax) {
        cell_size = CellSizeMax;
    }
    cell_size_slider.setValue(cell_size);
}

void BoardSettingsView::save() {
    int near_impact = first_impact_edit.text().remove(1, 1).toInt();
    int further_impact = second_impact_edit.text().remove(1, 1).toInt();
    int live_impact_start = live_impact_start_edit.text().remove(1, 1).toInt();
    int live_impact_end = live_impact_end_edit.text().remove(1, 1).toInt();
    int birth_impact_start = birth_impact_start_edit.text().remove(1, 1).toInt();
    int birth_impact_end = birth_impact_end_edit.text().remove(1, 1).toInt();

    // validate
    if (near_impact * 6 + further_impact * 6 >= 100) {
        error_message.showMessage("FST_IMPACT * 6 + SND_IMPACT * 6 must be lower than 10.0");
        return;
    }
    if (live_impact_start > birth_impact_start) {
        error_message.showMessage("LIVE_BEGIN must be lesser or equal to BIRTH_BEGIN");
        return;
    }
    if (birth_impact_start > birth_impact_end) {
        error_message.showMessage("BIRTH_BEGIN must be lesser or equal to BIRTH_END");
        return;
    }
    if (birth_impact_end > live_impact_end) {
        error_message.showMessage("BIRTH_END must be lesser or equal to LIVE_END");
        return;
    }

    settings->cellSize = cell_size_slider.value();
    settings->height = height_slider.value();
    settings->width = width_slider.value();
    settings->nearImpact = near_impact;
    settings->futherImpact = further_impact;
    settings->liveImpactRange = std::make_pair(live_impact_start, live_impact_end);
    settings->birthdayImpactRange = std::make_pair(birth_impact_start, birth_impact_end);
    settings->update();
    hide();
}
