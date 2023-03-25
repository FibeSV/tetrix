#ifndef TETRISBOARD_H
#define TETRISBOARD_H
#include <QPointer>
#include <QFrame>
#include <QWidget>
#include <QLabel>
#include <QBasicTimer>
#include "tetrispiece.h"

class TetrisBoard : public QFrame
{
    Q_OBJECT

public:
    TetrisBoard(QWidget *parent = nullptr);

    void LabelPieceSuivante(QLabel *label);
    QSize sizeHint() const override; //donne la taille recommandée pour un widget QT
    QSize minimumSizeHint() const override;

public slots:
    void start();
    void pause();

signals:
    void ScoreChange(int score);
    void lignesChange(int numLines);

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void timerEvent(QTimerEvent *event) override; //permet de modifier la vitesse de chute des pièces.

private:
    enum { BoardWidth = 10, BoardHeight = 22 };

    TetrisShape &shapeAt(int x, int y) { return board[(y * BoardWidth) + x]; }
    int squareWidth() { return contentsRect().width() / BoardWidth; }
    int squareHeight() { return contentsRect().height() / BoardHeight; }
    void clearBoard();
    void Chute();
    void MoveDown();
    void Chute_hauteur(int hauteur);
    void effacer_ligne();
    void newPiece();
    void showNextPiece();
    bool tryMove(const TetrisPiece &newPiece, int newX, int newY);
    void drawSquare(QPainter &painter, int x, int y, TetrisShape shape);

    QBasicTimer timer;
    QPointer<QLabel> nextPieceLabel;
    bool isStarted;
    bool isPaused;
    bool isWaitingAfterLine;
    TetrisPiece curPiece;
    TetrisPiece nextPiece;
    int curX;
    int curY;
    int numLignesRmv;
    int numPiecesDrop;
    int score;
    TetrisShape board[BoardWidth * BoardHeight];
};

#endif // TETRISBOARD_H
