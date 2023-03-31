#include "tetrisboard.h"
#include "qevent.h"
#include "qpainter.h"

TetrisBoard::TetrisBoard(QWidget *parent)
    : QFrame(parent), isStarted(false), isPaused(false)
{
    setFrameStyle(QFrame::Panel | QFrame::Sunken);
    setFocusPolicy(Qt::StrongFocus); //on s'assure que tous les évènements claviers sont "coincés" dans le jeu.
    clearBoard();

    nextPiece.setRandomShape();
}
void TetrisBoard::setLabelPieceSuivante(QLabel *label)
{
    nextPieceLabel = label;
}
QSize TetrisBoard::sizeHint() const //on limite les changements de taille des widgets pour que le jeu reste jouable
{
    return QSize(BoardWidth * 20 + frameWidth() * 4,
                 BoardHeight * 20+ frameWidth() * 4);
}

QSize TetrisBoard::minimumSizeHint() const //on utilise ici const, les tailles limites des fenêtres ne doivent pas êtres modifiées par le joueur.
{
    return QSize(BoardWidth * 2 + frameWidth() * 2,
                 BoardHeight * 2 + frameWidth() * 2);
}

void TetrisBoard::start() //Une partie est lancée lorsque start est appellée.
{
    if (isPaused)
        return;

    isStarted = true;
    isWaitingAfterLine = false;
    numLignesRmv = 0;// tous les indicateurs visuels de progression sont remis à zero
    numPiecesDrop = 0;
    score = 0;
    level = 1;
    clearBoard();

    emit lignesChange(numLignesRmv);
    emit scoreChange(score);
    emit levelChange(level);

    newPiece();
    timer.start(timeoutTime(), this);//le timer sert à savoir quand on va faire descendre la pièce d'une ligne.
}

void TetrisBoard::pause() //Permet de pauser le jeu.
{
    if(!isStarted) return;

    isPaused = !isPaused; //on change l'état du jeu
    if(isPaused) timer.stop();
    else timer.start(timeoutTime(), this);

    update();
}

void TetrisBoard::keyPressEvent(QKeyEvent *event)
{
    if(!isStarted || isPaused ||curPiece.shape() == NoShape) {
        QFrame::keyPressEvent(event);
        return;
    }
    switch (event->key()) {
        case Qt::Key_Left:
            tryMove(curPiece, curX - 1, curY); //essaie de déplacer la pièce à gauche
            break;
        case Qt::Key_Right:
            tryMove(curPiece, curX + 1, curY); //essaie de déplacer la pièce à droite
            break;
        case Qt::Key_Shift:
            tryMove(curPiece.rotatedRight(), curX, curY); //essaie de faire pivoter la pièce vers la droite
            break;
        case Qt::Key_Up:
            tryMove(curPiece.rotatedLeft(), curX, curY); //essaie de faire pivoter la pièce vers la gauche
            break;
        case Qt::Key_Space:
            dropDown(); //fait tomber la pièce directement en bas
            break;
        case Qt::Key_Down:
            oneLineDown(); //fait descendre la pièce d'une case
            break;
        default:
            QFrame::keyPressEvent(event); //le cas par défaut est de ne rien faire
        }
}

void TetrisBoard::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);
    QPainter painter(this);
    QRect rect = contentsRect();

    int boardTop = rect.bottom() - BoardHeight*squareHeight(); //calcul de la position du haut de la grille de jeu
    for (int i = 0; i < BoardHeight; ++i) {
        for (int j = 0; j < BoardWidth; ++j) {
            TetrisShape shape = shapeAt(j, BoardHeight - i - 1); //récupération de la forme de la pièce à la position j,i
            if (shape != NoShape)
                drawSquare(painter, rect.left() + j * squareWidth(),
                           boardTop + i * squareHeight(), shape); //dessin de la pièce à la position j,i
        }
    }
    if (curPiece.shape() != NoShape) {
          for (int i = 0; i < 4; ++i) {
              int x = curX + curPiece.x(i);
              int y = curY - curPiece.y(i); //calcul de la position de chaque carré de la pièce
              drawSquare(painter, rect.left() + x * squareWidth(),
                         boardTop + (BoardHeight - y - 1) * squareHeight(),
                         curPiece.shape()); //dessin de chaque carré de la pièce
          }
    }
}

void TetrisBoard::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == timer.timerId()) {
        if (isWaitingAfterLine) {
            isWaitingAfterLine = false;
            newPiece(); //génération d'une nouvelle pièce
            timer.start(timeoutTime(), this); //relance du timer
        } else {
            oneLineDown(); //fait descendre la pièce d'une case
        }
    } else {
        QFrame::timerEvent(event);
    }
}
void TetrisBoard::pieceDropped(int dropHeight) // Fonction appelée lorsque la pièce courante atteint le bas de la grille ou touche une autre pièce
{
    for (int i = 0; i < 4; ++i) {
        int x = curX + curPiece.x(i);
        int y = curY - curPiece.y(i);
        shapeAt(x, y) = curPiece.shape();
    }
    // Augmentation du compteur de pièces déposées et mise à jour du niveau tous les 25 pièces
    ++numPiecesDrop;
    if (numPiecesDrop % 25 == 0) {
        ++level;
        timer.start(timeoutTime(), this);
        emit levelChange(level);
    }

    score += dropHeight + 7;
    emit scoreChange(score);
    removeFullLines();

    if (!isWaitingAfterLine)
        newPiece();
}
void TetrisBoard::clearBoard()
{
    for (int i = 0; i < BoardHeight * BoardWidth; ++i)
        board[i] = NoShape; //initialisation de chaque case de la grille à "NoShape"
}
void TetrisBoard::dropDown()
{
    int dropHeight = 0;
    int newY = curY;
    while (newY > 0) {
        if (!tryMove(curPiece, curX, newY - 1))
            break;
        --newY;
        ++dropHeight;
    }
    pieceDropped(dropHeight);
}
void TetrisBoard::oneLineDown()
{
    if (!tryMove(curPiece, curX, curY - 1))
        pieceDropped(0);
}
void TetrisBoard::removeFullLines()
{
    int numFullLines = 0;

    for (int i = BoardHeight - 1; i >= 0; --i) {
        bool lineIsFull = true;

        for (int j = 0; j < BoardWidth; ++j) {
            if (shapeAt(j, i) == NoShape) {
                lineIsFull = false;
                break;
            }
        }

        if (lineIsFull) {
            ++numFullLines;
            for (int k = i; k < BoardHeight - 1; ++k) {
                for (int j = 0; j < BoardWidth; ++j)
                    shapeAt(j, k) = shapeAt(j, k + 1);
            }
            for (int j = 0; j < BoardWidth; ++j)
                shapeAt(j, BoardHeight - 1) = NoShape;
        }
    }
    if (numFullLines > 0) {
            numLignesRmv += numFullLines;
            score += 10 * numFullLines;
            emit lignesChange(numLignesRmv);
            emit scoreChange(score);

            timer.start(500, this);
            isWaitingAfterLine = true;
            curPiece.setShape(NoShape);
            update();
        }
    }
