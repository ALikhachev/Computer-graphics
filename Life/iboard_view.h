#ifndef IBOARDVIEW_H
#define IBOARDVIEW_H

#include <QtGlobal>

struct Cell;

class IBoardView {
public:
    virtual void onCellStateChanged(quint32 x, quint32 y, const Cell &cell) = 0;
};

#endif // IBOARDVIEW_H
