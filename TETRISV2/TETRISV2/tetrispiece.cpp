#include "tetrispiece.h"
#include <QRandomGenerator>

void TetrisPiece::setRandomShape()
{
    setShape(TetrisShape(QRandomGenerator::global()->bounded(7) + 1));
}
void TetrisPiece::setShape(TetrisShape shape)
{
    static constexpr int coordsTable[8][4][2] = {
        { { 0, 0 },   { 0, 0 },   { 0, 0 },   { 0, 0 } },
        { { 0, -1 },  { 0, 0 },   { -1, 0 },  { -1, 1 } },
        { { 0, -1 },  { 0, 0 },   { 1, 0 },   { 1, 1 } },
        { { 0, -1 },  { 0, 0 },   { 0, 1 },   { 0, 2 } },
        { { -1, 0 },  { 0, 0 },   { 1, 0 },   { 0, 1 } },
        { { 0, 0 },   { 1, 0 },   { 0, 1 },   { 1, 1 } },
        { { -1, -1 }, { 0, -1 },  { 0, 0 },   { 0, 1 } },
        { { 1, -1 },  { 0, -1 },  { 0, 0 },   { 0, 1 } }
    };

    for (int i = 0; i < 4 ; i++) {
        for (int j = 0; j < 2; ++j)
            coords[i][j] = coordsTable[shape][i][j];
    }
    pieceShape = shape;
}

int TetrisPiece::minX() const
{
    int min = coords[0][0];
    for (int i = 1; i < 4; ++i)
        min = qMin(min, coords[i][0]);
    return min;
}

int TetrisPiece::maxX() const
{
    int max = coords[0][0];
    for (int i = 1; i < 4; ++i)
        max = qMax(max, coords[i][0]);
    return max;
}

int TetrisPiece::minY() const
{
    int min = coords[0][1];
    for (int i = 1; i < 4; ++i)
        min = qMin(min, coords[i][1]);
    return min;
}

int TetrisPiece::maxY() const
{
    int max = coords[0][1];
    for (int i = 1; i < 4; ++i)
        max = qMax(max, coords[i][1]);
    return max;
}


TetrisPiece TetrisPiece::rotatedLeft() const
{
    if (pieceShape == O)
        return *this;

    TetrisPiece res;
    res.pieceShape = pieceShape;
    for (int i = 0; i < 4; ++i) {
        res.setX(i, y(i));
        res.setY(i, -x(i));
    }
    return res;
}

TetrisPiece TetrisPiece::rotatedRight() const
{
    if (pieceShape == O)
        return *this;

    TetrisPiece res;
    res.pieceShape = pieceShape;
    for (int i = 0; i < 4; ++i) {
        res.setX(i, -y(i));
        res.setY(i, x(i));
    }
    return res;
}
