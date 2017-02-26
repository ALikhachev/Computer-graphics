#ifndef BOARD_H
#define BOARD_H

#include <QSharedPointer>

#include "board_settings.h"

struct Cell {
    quint16 external_impact;
    bool alive;
};

class Board
{
private:
    std::vector<Cell> state;
    std::vector<Cell> prev_state;
    const BoardSettings *rules;
    quint32 width;
    quint32 height;
    quint32 ticks_passed;

    // only call this method when cell state is changed
    void updateImpacts(quint32 x, quint32 y, bool born);
public:
    Board(const BoardSettings *rules, const quint32 width, const quint32 height, std::vector<Cell> initial_state);

    void setCell(quint32 x, quint32 y, bool alive);
    void invertCell(quint32 x, quint32 y);
    void tick();
    std::vector<Cell> & getState();
    quint32 getWidth() const;
    quint32 getHeight() const;
    const BoardSettings * getSettings() const;
};

#endif // BOARD_H
