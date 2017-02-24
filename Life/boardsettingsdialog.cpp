#include "boardsettingsdialog.h"

BoardSettingsDialog::BoardSettingsDialog(BoardSettings *, QWidget * parent) : QDialog(parent),
    mainLayout(this),
    okButton(tr("Apply"), this),
    cancelButton(tr("Cancel"), this),
    sizeBox(tr("Field size"), this),
    cellBox(tr("Cell properties"), this),
    rulesBox(tr("Game rules"), this)
{
    mainLayout.addWidget(&sizeBox, 0, 0, 2, 2);
    mainLayout.addWidget(&cellBox, 0, 2, 2, 5);
    mainLayout.addWidget(&rulesBox, 2, 0, 2, 6);
    mainLayout.addWidget(&okButton, 2, 6, 1, 1, Qt::AlignRight);
    mainLayout.addWidget(&cancelButton, 3, 6, 1, 1, Qt::AlignRight);
    setLayout(&mainLayout);
}
