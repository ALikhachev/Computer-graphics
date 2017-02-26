#ifndef BOARDSETTINGSDIALOG_H
#define BOARDSETTINGSDIALOG_H

#include <QGridLayout>
#include <QGroupBox>
#include <QDialog>
#include <QPushButton>

#include "board_settings.h"

class BoardSettingsView : public QDialog
{
public:
    BoardSettingsView(BoardSettings *settings, QWidget * parent = 0);

private:
    BoardSettings *settings;

    QGridLayout mainLayout;
    QPushButton okButton;
    QPushButton cancelButton;
    QGroupBox sizeBox;
    QGroupBox cellBox;
    QGroupBox rulesBox;
};

#endif // BOARDSETTINGSDIALOG_H
