#include "tetromino.h"
#include <QRandomGenerator>
#include <QtCore>
void Tetromino::setRandomShape()
{
    setShape(TetrisShape(QRandomGenerator::global()->bounded(7) + 1));
}
void Tetromino::setShape(TetrisShape shape)
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

int Tetromino::minX() const
{
    int min = coords[0][0];
    for (int i = 1; i < 4; ++i)
        min = qMin(min, coords[i][0]);
    return min;
}

int Tetromino::maxX() const
{
    int max = coords[0][0];
    for (int i = 1; i < 4; ++i)
        max = qMax(max, coords[i][0]);
    return max;
}

int Tetromino::minY() const
{
    int min = coords[0][1];
    for (int i = 1; i < 4; ++i)
        min = qMin(min, coords[i][1]);
    return min;
}

int Tetromino::maxY() const
{
    int max = coords[0][1];
    for (int i = 1; i < 4; ++i)
        max = qMax(max, coords[i][1]);
    return max;
}


Tetromino Tetromino::rotatedLeft() const
{
    // If the piece is shaped like an O, it cannot be rotated
    if (pieceShape == O)
        return *this;

    Tetromino res; // Create a new TetrisPiece object to hold the rotated piece
    res.pieceShape = pieceShape; // Copy over the current shape

    // Apply the rotation transformation to each block of the piece
    for (int i = 0; i < 4; ++i) {
        // The x-coordinate becomes the y-coordinate of the current block
        res.setX(i, y(i));
        // The y-coordinate becomes the negative of the x-coordinate of the current block
        res.setY(i, -x(i));
    }
    // Apply additional rotation by swapping x and y coordinates and negating the new y

    return res; // Return the rotated piece
}

