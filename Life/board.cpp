#include "board.h"

Board::Board(const BoardSettings *rules,
             const quint32 width, const quint32 height, std::vector<Cell> initial_state) :
    name(name),
    state(initial_state),
    prev_state(width * height),
    rules(rules),
    width(width),
    height(height),
    ticks_passed(0)
{
    // count initial impacts
}

void Board::updateImpacts(quint32 x, quint32 y, bool born) {
    quint16 sign = born ? 1 : -1;
    if (y % 2 == 0) {
        // first order neighbors
        if (x < width - 1) {
            state[width * (y - 1) + x].external_impact += sign * rules->nearImpact;
        }
        if (y + 1 < height) {
            if (x < width - 1) {
                state[width * (y + 1) + x].external_impact += sign * rules->nearImpact;
            }
            state[width * (y + 1) + x - 1].external_impact += sign * rules->nearImpact;
        }
        if (x + 1 < width) {
            state[width * y + x + 1].external_impact += sign * rules->nearImpact;
        }
        state[width * y + x - 1].external_impact += sign * rules->nearImpact;
        state[width * (y - 1) + x - 1].external_impact += sign * rules->nearImpact;
        // second order neighbors
        state[width * (y - 2) + x].external_impact += sign * rules->futherImpact;
        if (y + 2 < height) {
            state[width * (y + 2) + x].external_impact += sign * rules->futherImpact;
        }
        if (x + 1 < width - 1) {
            state[width * (y - 1) + x + 1].external_impact += sign * rules->futherImpact;
            if (y + 1 < height) {
                state[width * (y + 1) + x + 1].external_impact += sign * rules->futherImpact;
            }
        }
        if (y + 1 < height) {
            state[width * (y + 1) + x - 2].external_impact += sign * rules->futherImpact;
        }
        state[width * (y - 1) + x - 2].external_impact += sign * rules->futherImpact;
    } else {
        // first order neighbors
        state[width * y + x - 1].external_impact += sign * rules->nearImpact;
        if (x + 1 < width - 1) {
            state[width * y + x + 1].external_impact += sign * rules->nearImpact;
        }
        state[width * (y - 1) + x].external_impact += sign * rules->nearImpact;
        if (y + 1 < height) {
            state[width * (y + 1) + x].external_impact += sign * rules->nearImpact;
            state[width * (y + 1) + x + 1].external_impact += sign * rules->nearImpact;
        }
        state[width * (y - 1) + x + 1].external_impact += sign * rules->nearImpact;
        // second order neighbors
        state[width * (y - 2) + x].external_impact += sign * rules->futherImpact;
        if (y + 2 < height) {
            state[width * (y + 2) + x].external_impact += sign * rules->futherImpact;
        }
        state[width * (y - 1) + x - 1].external_impact += sign * rules->futherImpact;
        if (y + 1 < height) {
            state[width * (y + 1) + x - 1].external_impact += sign * rules->futherImpact;
            if (x + 2 < width) {
                state[width * (y + 1) + x + 2].external_impact += sign * rules->futherImpact;
            }
        }
        if (x + 2 < width) {
            state[width * (y - 1) + x + 2].external_impact += sign * rules->futherImpact;
        }
    }
}

void Board::setCell(quint32 x, quint32 y, bool alive) {
    Cell &cell = state[width * y + x];
    if ((!cell.alive && alive) || (cell.alive && !alive)) {
        // only if value is changed
        updateImpacts(x, y, alive);
        cell.alive = alive;
    }
}

void Board::invertCell(quint32 x, quint32 y) {
    Cell &cell = state[width * y + x];
    cell.alive = !cell.alive;
    updateImpacts(x, y, cell.alive);
}

void Board::tick() {
    prev_state = state;
    for (quint32 j = 0; j < height; ++j) {
        for (quint32 i = 0; i < width; ++i) {
            Cell &prev_cell = prev_state[j * width + i];
            Cell &new_cell = state[j * width + i];
            if (prev_cell.alive) {
                if (prev_cell.external_impact < rules->liveImpactRange.first || prev_cell.external_impact > rules->liveImpactRange.second) {
                    new_cell.alive = false;
                    updateImpacts(i, j, false);
                }
            } else {
                if (prev_cell.external_impact >= rules->birthdayImpactRange.first && prev_cell.external_impact <= rules->birthdayImpactRange.second) {
                    new_cell.alive = true;
                    updateImpacts(i, j, true);
                }
            }
            if (i == width - 2 && j % 2 == 1) {
                break;
            }
        }
    }
    ++ticks_passed;
}

std::vector<Cell> & Board::getState() {
    return state;
}

quint32 Board::getWidth() const {
    return width;
}

quint32 Board::getHeight() const {
    return height;
}

const BoardSettings * Board::getSettings() const {
    return rules;
}
