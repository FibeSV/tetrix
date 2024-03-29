#ifndef TETRISPIECE_H
#define TETRISPIECE_H

enum TetrisShape { NoShape,I,O,T,L,J,Z,S };
class TetrisPiece
{

public:
    TetrisPiece() { setShape(NoShape); }//lors de sa création la pièce n'a pas de forme, cela facilite leur manipulation.
    void setRandomShape();
    void setShape(TetrisShape shape);

    TetrisShape shape() const { return pieceShape; }
    int x(int index) const { return coords[index][0]; } //largeur de la piece
    int y(int index) const { return coords[index][1]; } //hauteur de la piece

    int minX() const; //définitions des bornes des pièces (cela permet de limiter le nombre de pièces existantes
    int maxX() const;
    int minY() const;
    int maxY() const;

    TetrisPiece rotatedLeft() const; //les deux fonctions pour faire tourner les pièces.
    TetrisPiece rotatedRight() const;
    //il reste à définir HOLD et DROP functions

private:// ici on définit les 4 blocs qui constituent une pièce
    void setX(int index, int x) { coords[index][0] = x; }
    void setY(int index, int y) { coords[index][1] = y; }

    TetrisShape pieceShape;
    int coords[4][2];
};
#endif
