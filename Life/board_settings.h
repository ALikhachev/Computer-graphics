#ifndef BOARDSETTINGS_H
#define BOARDSETTINGS_H

#include <QtGlobal>
#include <QObject>

class BoardSettings : public QObject
{
    Q_OBJECT

public:
    int width = 24;
    int height = 24;
    int cellSize = 17;
    int nearImpact = 10;
    int futherImpact = 3;
    std::pair<int, int> liveImpactRange = std::make_pair(20, 33);
    std::pair<int, int> birthdayImpactRange = std::make_pair(23, 29);
    bool isXorMode = false;
    bool show_impacts = false;

    void updateView();
    void updateModel();
    void resetToDefault();

signals:
    void viewSettingsChanged();
    void modelSettingsChanged();
};

#endif // BOARDSETTINGS_H
