#ifndef BOARD_H
#define BOARD_H

#include <QSharedPointer>
#include <QTextStream>

#include "board_settings.h"

struct Cell {
    quint16 external_impact;
    bool alive;
};

class Board : public QObject
{
    Q_OBJECT
private:
    std::vector<Cell> state;
    std::vector<Cell> prev_state;
    BoardSettings *rules;
    int width;
    int height;
    int ticks_passed;

    // only call this method when cell state is changed
    void updateImpacts(qint32 x, qint32 y, bool born);
    void recountImpacts();

public slots:
    void tick();

public:
    Board(BoardSettings *rules, const int width, const int height, std::vector<Cell> initial_state);

    void save(QTextStream &out);
    void load(QTextStream &out);

    void setCell(qint32 x, qint32 y, bool alive);
    void invertCell(qint32 x, qint32 y);
    std::vector<Cell> & getState();
    int getWidth() const;
    int getHeight() const;
    const BoardSettings * getSettings() const;
    void resize(int width, int height);
    void clear();
};

#endif // BOARD_H
