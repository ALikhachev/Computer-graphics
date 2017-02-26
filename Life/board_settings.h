#ifndef BOARDSETTINGS_H
#define BOARDSETTINGS_H

#include <QtGlobal>

class BoardSettings
{
public:
    BoardSettings();

private:
    quint32 width;
    quint32 height;
    quint8 lineWeight;
    quint8 cellSize;
    quint8 nearImpact;
    quint8 futherImpact;
    std::pair<quint8, quint8> liveImpactRange;
    std::pair<quint8, quint8> birthdayImpactRange;
};

#endif // BOARDSETTINGS_H
