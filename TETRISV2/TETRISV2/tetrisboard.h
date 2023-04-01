
#ifndef TETRISBOARD_H
#define TETRISBOARD_H

#include <QBasicTimer>
#include <QFrame>
#include <QPointer>
#include <QPainter>

#include "qlabel.h"
#include "tetrispiece.h"

QT_BEGIN_NAMESPACE
class QLabel;
QT_END_NAMESPACE


class TetrisBoard : public QFrame
{
    Q_OBJECT

public:
    TetrisBoard(QWidget *parent = nullptr);

    void setNextPieceLabel(QLabel *label);
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

public slots:
    void start();
    void pause();

signals:
    void scoreChange(int score);
    void levelChange(int level);
    void lignesChange(int numLines);
    void nextPieceChanged(const TetrisPiece& nextPiece);

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void timerEvent(QTimerEvent *event) override;

private:
    enum { BoardWidth = 10, BoardHeight = 22 };

    TetrisShape &shapeAt(int x, int y) { return board[(y * BoardWidth) + x]; }
    int timeoutTime() { return 1000 / (1 + level); }
    int squareWidth() { return contentsRect().width() / BoardWidth; }
    int squareHeight() { return contentsRect().height() / BoardHeight; }
    void clearBoard();
    void dropDown();
    void oneLineDown();
    void pieceDropped(int dropHeight);
    void removeFullLines();
    void newPiece();
    void showNextPiece();
    bool tryMove(const TetrisPiece &newPiece, int newX, int newY);
    void drawSquare(QPainter &painter, int x, int y, TetrisShape shape);
    void GameOver();

    QBasicTimer timer;
    QPointer <QLabel> nextPieceLabel;
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
    int level;
    TetrisShape board[BoardWidth * BoardHeight];
};

#endif
