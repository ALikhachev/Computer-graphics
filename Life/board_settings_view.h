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
    BoardSettings *settings;

    QGridLayout mainLayout;
    QPushButton okButton;
    QPushButton cancelButton;

    QGroupBox sizeBox;
    QGridLayout sizeLayout;
    QLabel width_label;
    QLineEdit width_edit;
    QSlider width_slider;
    QLabel height_label;
    QLineEdit height_edit;
    QSlider height_slider;

    QGroupBox cellBox;
    QGridLayout cell_layout;
    QLabel cell_size_label;
    QLineEdit cell_size_edit;
    QSlider cell_size_slider;

    QGroupBox rulesBox;
    QVBoxLayout rules_layout;
    QLabel first_impact_label;
    QLineEdit first_impact_edit;
    QLabel second_impact_label;
    QLineEdit second_impact_edit;
    QLabel live_impact_start_label;
    QLineEdit live_impact_start_edit;
    QLabel live_impact_end_label;
    QLineEdit live_impact_end_edit;
    QLabel birth_impact_start_label;
    QLineEdit birth_impact_start_edit;
    QLabel birth_impact_end_label;
    QLineEdit birth_impact_end_edit;

    QErrorMessage error_message;

    static const int CellSizeMin = 2;
    static const int CellSizeMax = 100;
    static const int HeightMin = 1;
    static const int HeightMax = 100;
    static const int WidthMin = 1;
    static const int WidthMax = 100;
};

#endif // BOARDSETTINGSDIALOG_H
