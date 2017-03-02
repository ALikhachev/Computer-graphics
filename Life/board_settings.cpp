#include "board_settings.h"

void BoardSettings::updateView() {
    emit viewSettingsChanged();
}

void BoardSettings::updateModel() {
    emit modelSettingsChanged();
}

void BoardSettings::resetToDefault() {
    width = 24;
    height = 24;
    cellSize = 17;
    nearImpact = 10;
    futherImpact = 3;
    liveImpactRange = std::make_pair(20, 33);
    birthdayImpactRange = std::make_pair(23, 29);
    isXorMode = false;
    show_impacts = false;
}
