#ifndef TETRISBOARD_H
#define TETRISBOARD_H

#include <QBasicTimer>
#include <QFrame>
#include <QPointer>
#include <QPainter>
#include "qlabel.h"
#include "tetromino.h"

QT_BEGIN_NAMESPACE
class QLabel;
QT_END_NAMESPACE

class TetrisBoard : public QFrame
{
Q_OBJECT

public:
TetrisBoard(QWidget *parent = nullptr);
QSize sizeHint() const override;
QSize minimumSizeHint() const override;
void setHeldPieceLabel(QLabel *label);
void setNextPieceLabel(QLabel *label);

public slots:
void startGame();
void pauseGame();

signals:
void scoreChange(int score);
/*void levelChange(int level);*/
void showMessage(QString message);
void lignesChange(int numLines);

void nextPieceChanged(const Tetromino& nextPiece);

protected:
void paintEvent(QPaintEvent *event) override;
void keyPressEvent(QKeyEvent *event) override;
void timerEvent(QTimerEvent *event) override;

private:
enum { BoardWidth = 10, BoardHeight = 22 };
TetrisShape &shapeAt(int x, int y) { return board[(y * BoardWidth) + x]; }
int squareHeight() { return contentsRect().height() / BoardHeight; }
int squareWidth() { return contentsRect().width() / BoardWidth; }
void createNewPiece();
void removeFullLines();
void pieceDropped();
void paintPiece(QPainter &painter, int x, int y, TetrisShape shape);
void dropCurrentPiece();
void clearBoard();
bool tryMove(const Tetromino &newPiece, int newX, int newY);
void flashGameOverMessage(QPainter &painter);
void updateSlot();
void hold();
void updateHoldPieceLabel();
int timeoutTime() { return 1000 / (1 + level); }

QBasicTimer timer;
QPointer<QLabel> nextPieceLabel;
QPointer<QLabel> heldPieceLabel;
QPointer<QLabel> levelLabel;
Tetromino curPiece;
Tetromino nextPiece;
Tetromino heldPiece;
TetrisShape board[BoardWidth * BoardHeight];
int curX;
int curY;
int score;
int level;
int numPiecesDrop;
int numLignesRmv;
bool isGameOver;
bool isStarted;
bool isPaused;
bool isPieceHeld;
bool isWaitingAfterLine;

void movePieceOneLineDown();
void showNextPiece();
};

#endif
