#ifndef BOARDSETTINGS_H
#define BOARDSETTINGS_H

#include <QtGlobal>

struct BoardSettings {
    quint32 width = 30;
    quint32 height = 30;
    quint8 cellSize = 50;
    quint8 nearImpact = 10;
    quint8 futherImpact = 3;
    std::pair<quint8, quint8> liveImpactRange = std::make_pair(20, 33);
    std::pair<quint8, quint8> birthdayImpactRange = std::make_pair(23, 29);
};

#endif // BOARDSETTINGS_H
