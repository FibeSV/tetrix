#include "tetrisboard.h"

TetrisBoard::TetrisBoard(QWidget *parent)
    : QFrame(parent), isStarted(false), isPaused(false)
{
    setFrameStyle(QFrame::Panel | QFrame::Sunken);
    setFocusPolicy(Qt::StrongFocus); //on s'assure que tous les évènements claviers sont "coincés" dans le jeu.
    clearBoard();

    nextPiece.setRandomShape();
}
void TetrisBoard::LabelPieceSuivante(QLabel *label)
{
    nextPieceLabel = label;
}
QSize TetrisBoard::sizeHint() const //on limite les changements de taille des widgets pour que le jeu reste jouable
{
    return QSize(BoardWidth * 20 + frameWidth() * 4,
                 BoardHeight * 20+ frameWidth() * 4);
}

QSize TetrisBoard::minimumSizeHint() const
{
    return QSize(BoardWidth * 2 + frameWidth() * 2,
                 BoardHeight * 2 + frameWidth() * 2);
}

void TetrisBoard::start()
{
    if (isPaused)
        return;

    isStarted = true;
    isWaitingAfterLine = false;
    numLignesRmv = 0;
    numPiecesDrop = 0;
    score = 0;
    clearBoard();

    emit lignesChange(numLignesRmv);
    emit ScoreChange(score);

    newPiece();
    timer.start(timeoutTime(), this);
}
