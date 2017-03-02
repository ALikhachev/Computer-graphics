#ifndef BOARDSETTINGSDIALOG_H
#define BOARDSETTINGSDIALOG_H

#include <QErrorMessage>
#include <QGridLayout>
#include <QGroupBox>
#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QSlider>
#include <QLineEdit>

#include "board_settings.h"

class BoardSettingsView : public QDialog
{
    Q_OBJECT

public:
    BoardSettingsView(BoardSettings *settings, QWidget * parent = 0);

protected:
    void showEvent(QShowEvent * event) override;

private slots:
    void widthSliderChanged(int value);
    void widthEditChanged();
    void heightSliderChanged(int value);
    void heightEditChanged();
    void cellSizeSliderChanged(int value);
    void cellSizeEditChanged();
    void save();

private:
    void showError(QString text);

    BoardSettings *settings;

    QLineEdit *width_edit;
    QSlider *width_slider;
    QLineEdit *height_edit;
    QSlider *height_slider;

    QLineEdit *cell_size_edit;
    QSlider *cell_size_slider;

    QLineEdit *first_impact_edit;
    QLineEdit *second_impact_edit;
    QLineEdit *live_impact_start_edit;
    QLineEdit *live_impact_end_edit;
    QLineEdit *birth_impact_start_edit;
    QLineEdit *birth_impact_end_edit;

    static const int CellSizeMin = 2;
    static const int CellSizeMax = 60;
    static const int HeightMin = 1;
    static const int HeightMax = 100;
    static const int WidthMin = 1;
    static const int WidthMax = 100;
};

#endif // BOARDSETTINGSDIALOG_H
