#include "board.h"

Board::Board(const BoardSettings *rules,
             const int width, const int height, std::vector<Cell> initial_state) :
    state(initial_state),
    prev_state(width * height),
    rules(rules),
    width(width),
    height(height),
    ticks_passed(0)
{
    recountImpacts();
}

void Board::updateImpacts(int x, int y, bool born) {
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
            if (x > 0) {
                state[width * (y + 1) + x - 1].external_impact += sign * rules->nearImpact;
            }
        }
        if (x + 1 < width) {
            state[width * y + x + 1].external_impact += sign * rules->nearImpact;
        }
        if (x > 0) {
            state[width * y + x - 1].external_impact += sign * rules->nearImpact;
            state[width * (y - 1) + x - 1].external_impact += sign * rules->nearImpact;
        }
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
        if (x > 1) {
            if (y + 1 < height) {
                state[width * (y + 1) + x - 2].external_impact += sign * rules->futherImpact;
            }
            state[width * (y - 1) + x - 2].external_impact += sign * rules->futherImpact;
        }
    } else {
        // first order neighbors
        if (x > 0) {
            state[width * y + x - 1].external_impact += sign * rules->nearImpact;
        }
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
        if (x > 0) {
            state[width * (y - 1) + x - 1].external_impact += sign * rules->futherImpact;
        }
        if (y + 1 < height) {
            if (x > 0) {
                state[width * (y + 1) + x - 1].external_impact += sign * rules->futherImpact;
            }
            if (x + 2 < width) {
                state[width * (y + 1) + x + 2].external_impact += sign * rules->futherImpact;
            }
        }
        if (x + 2 < width) {
            state[width * (y - 1) + x + 2].external_impact += sign * rules->futherImpact;
        }
    }
}

void Board::setCell(int x, int y, bool alive) {
    Cell &cell = state[width * y + x];
    if ((!cell.alive && alive) || (cell.alive && !alive)) {
        // only if value is changed
        updateImpacts(x, y, alive);
        cell.alive = alive;
    }
}

void Board::invertCell(int x, int y) {
    Cell &cell = state[width * y + x];
    cell.alive = !cell.alive;
    updateImpacts(x, y, cell.alive);
}

void Board::tick() {
    prev_state = state;
    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
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

int Board::getWidth() const {
    return width;
}

int Board::getHeight() const {
    return height;
}

void Board::resize(int width, int height) {
    std::vector<Cell> state(width * height);
    for (int j = 0; j < std::min(height, this->height); ++j) {
        memcpy(state.data() + j * width, this->state.data() + j * this->width, sizeof(Cell) * (std::min(width, this->width) - (1 & (j % 2 == 1))));
    }
    this->state = state;
    this->width = width;
    this->height = height;
    recountImpacts();
}

const BoardSettings * Board::getSettings() const {
    return rules;
}

void Board::clear() {
    memset(state.data(), 0, state.size() * sizeof(Cell));
}

void Board::recountImpacts() {
    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            state[j * width + i].external_impact = 0;
        }
    }
    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            if (state[j * width + i].alive) {
                updateImpacts(i, j, true);
            }
        }
    }
}
