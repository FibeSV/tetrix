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
        timer.start(timeoutTime(), this); // réinitialisation timer -> temps de chute plus court
        emit levelChange(level);//  signal pour mettre à jour l'affichage du niveau
    }
    // Calcul et ajout du score pour le déplacement de la pièce à sa position finale
    score += dropHeight + 7;
    emit scoreChange(score); // signal pour mettre à jour l'affichage du score
    removeFullLines(); // suppression des lignes complètes, si il y en a

    if (!isWaitingAfterLine) // si on n'attend pas après une suppression de ligne, on génère une nouvelle pièce
        newPiece();
}
void TetrisBoard::clearBoard()
{
    for (int i = 0; i < BoardHeight * BoardWidth; i++)
        board[i] = NoShape; //initialisation de chaque case de la grille à "NoShape"
}
void TetrisBoard::dropDown()
{
    int dropHeight = 0; //hauteur cible de la chute
    int newY = curY; // coordonnée en Y de la pièce
    while (newY > 0) {
        if (!tryMove(curPiece, curX, newY - 1)) //si la pièce rencontre un obstacle.
            break;
        --newY; // déplacer la pièce vers le bas
        dropHeight++;
    }
    pieceDropped(dropHeight);//on finalise la position de la pièce.
}
void TetrisBoard::oneLineDown()
{
    if (!tryMove(curPiece, curX, curY - 1))// on essaie de bouger la pièce vers le bas
        pieceDropped(0);// si ce n'est pas possible la position de la pièce est finalisée.
}
void TetrisBoard::removeFullLines()
{
    int numFullLines = 0;

    for (int i = BoardHeight - 1; i >= 0; --i) { //on part du bas et on remonte.
        bool lineIsFull = true;

        for (int j = 0; j < BoardWidth; ++j) {    // Parcours des colonnes pour vérifier si la ligne est pleine
            if (shapeAt(j, i) == NoShape) {
                lineIsFull = false;
                break;
            }
        }

        if (lineIsFull) {
            ++numFullLines;
            for (int k = i; k < BoardHeight - 1; ++k) {// On doit décaler toutes les lignes situées au-dessus de la ligne supprimée
                for (int j = 0; j < BoardWidth; ++j)
                    shapeAt(j, k) = shapeAt(j, k + 1);
            }
            for (int j = 0; j < BoardWidth; ++j) // on supprime la dernière ligne.
                shapeAt(j, BoardHeight - 1) = NoShape;
        }
    }
    if (numFullLines > 0) { //on transmet les infos sur les lignes supprimées
            numLignesRmv += numFullLines;
            score += 10 * numFullLines;
            emit lignesChange(numLignesRmv);
            emit scoreChange(score);

            timer.start(500, this);// on patiente un peu avant de faire appraitre de nouvelles lignes.
            isWaitingAfterLine = true;
            curPiece.setShape(NoShape);
            update();
        }
    }

void TetrisBoard::showNextPiece()
{
    if (!nextPieceLabel) // Vérifie si le QLabel pour la prochaine pièce est valide
        return;

    // Calcul des dimensions de la pièce
    int dx = nextPiece.maxX() - nextPiece.minX() + 1;
    int dy = nextPiece.maxY() - nextPiece.minY() + 1;

    // Création d'une QPixmap pour dessiner la pièce
    QPixmap pixmap(dx * squareWidth(), dy * squareHeight());

    // Création d'un QPainter pour dessiner sur la QPixmap
    QPainter painter(&pixmap);

    // Remplissage de la QPixmap avec la couleur de fond du QLabel
    painter.fillRect(pixmap.rect(), nextPieceLabel->palette().window());

    // Dessin de chaque carré de la pièce sur la QPixmap
    for (int i = 0; i < 4; ++i) {
        int x = nextPiece.x(i) - nextPiece.minX();
        int y = nextPiece.y(i) - nextPiece.minY();
        drawSquare(painter, x * squareWidth(), y * squareHeight(),
                   nextPiece.shape());
    }

    // Affichage de la QPixmap sur le QLabel pour la prochaine pièce
    nextPieceLabel->setPixmap(pixmap);
}

bool TetrisBoard::tryMove(const TetrisPiece &newPiece, int newX, int newY)
{// Vérifie si le déplacement d'une pièce à une nouvelle position est possible
    int pieceX, pieceY;
    for (int i = 0; i < 4; ++i) {        // Récupère les nouvelles coordonnées pour le carré
        pieceX = newX + newPiece.x(i);
        pieceY = newY - newPiece.y(i);
        // Vérifie si le carré est en dehors des limites du plateau de jeu
        if (pieceX < 0 || pieceX >= BoardWidth || pieceY < 0 || pieceY >= BoardHeight || shapeAt(pieceX, pieceY) != NoShape)         // Vérifie si le carré est déjà occupé par une autre pièce
        return false;
    }
    // Si tous les carrés peuvent être déplacés, met à jour la pièce courante et ses coordonnées
    curPiece = newPiece;
    curX = newX;
    curY = newY;
    // Actualise le plateau de jeu pour afficher les changements
    update();
    return true;
}

void TetrisBoard::newPiece()
{
    curPiece = nextPiece;
    nextPiece.setRandomShape();
    showNextPiece();

    // Position initiale de la pièce
    curX = BoardWidth / 2 + 1;
    curY = BoardHeight - 1 + curPiece.minY();


    if (tryMove(curPiece, curX, curY)) {
        return;
    }
    //si on ne peut pas mettre une nouvelle pièce c'est game ove.
    curPiece.setShape(NoShape);
    timer.stop();
    isStarted = false;
}

void TetrisBoard::drawSquare(QPainter &painter, int x, int y, TetrisShape shape)
{// Cette fonction est purement esthetique.
    QColor color;
    //I,O,T,L,J,Z,S, NoShape
    switch (shape) {
        case NoShape:
            color = Qt::gray;
            break;
        case Z:
            color = Qt::red;
            break;
        case S:
            color = Qt::green;
            break;
        case L:
            color = Qt::blue;
            break;
        case T:
            color = Qt::magenta;
            break;
        case O:
            color = Qt::yellow;
            break;
        case J:
            color = Qt::cyan;
            break;
        case I:
            color = Qt::darkCyan;
            break;
    }

    painter.fillRect(x + 1, y + 1, squareWidth() - 2, squareHeight() - 2, color);

    painter.setPen(color.lighter());
    painter.drawLine(x, y + squareHeight() - 1, x, y);
    painter.drawLine(x, y, x + squareWidth() - 1, y);

    painter.setPen(color.darker());
    painter.drawLine(x + 1, y + squareHeight() - 1,
                     x + squareWidth() - 1, y + squareHeight() - 1);
    painter.drawLine(x + squareWidth() - 1, y + squareHeight() - 1,
                     x + squareWidth() - 1, y + 1);
}
