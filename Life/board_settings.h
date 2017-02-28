#ifndef BOARDSETTINGS_H
#define BOARDSETTINGS_H

#include <QtGlobal>
#include <QObject>

class BoardSettings : public QObject
{
    Q_OBJECT

public:
    quint32 width = 24;
    quint32 height = 24;
    quint8 cellSize = 17;
    quint8 nearImpact = 10;
    quint8 futherImpact = 3;
    std::pair<quint8, quint8> liveImpactRange = std::make_pair(20, 33);
    std::pair<quint8, quint8> birthdayImpactRange = std::make_pair(23, 29);
    bool isXorMode = false;
    bool show_impacts = false;

    void update();

};

#endif // BOARDSETTINGS_H
