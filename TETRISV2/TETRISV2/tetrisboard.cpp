#include "tetrisboard.h"
#include "qevent.h"
#include "qpainter.h"
#include "qtimer.h"
#include <QKeyEvent>
#include <QLabel>
#include <QPainter>
#include <QMessageBox>

TetrisBoard::TetrisBoard(QWidget *parent)
    : QFrame(parent), isStarted(false), isPaused(false)
{
    isPieceHeld = false;
    isGameOver = false;
    setFrameStyle(QFrame::Panel | QFrame::Sunken);
    setFocusPolicy(Qt::StrongFocus); //on s'assure que tous les évènements claviers sont "coincés" dans le jeu.
    clearBoard();
    nextPiece.setRandomShape();
    setStyleSheet("background-color: #fdf6e3;");
}
void TetrisBoard::setNextPieceLabel(QLabel *label)
{
    nextPieceLabel = label;
}

void TetrisBoard::setHeldPieceLabel(QLabel *label)
{
    heldPieceLabel = label;
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

void TetrisBoard::startGame() //Une partie est lancée lorsque start est appellée.
{
    if (isPaused)
        return;

    isGameOver = false;
    isStarted = true;
    heldPiece.setShape(NoShape)  ;
    updateHoldPieceLabel();
    nextPieceLabel -> clear();
    isWaitingAfterLine = false;
    score = 0;
    level = 0;
    numLignesRmv = 0;// tous les indicateurs visuels de progression sont remis à zero
    numPiecesDrop = 0;
    clearBoard();

    emit lignesChange(numLignesRmv);
    emit scoreChange(score);
    //emit levelChange(level);

    createNewPiece();
    timer.start(timeoutTime(), this);//le timer sert à savoir quand on va faire descendre la pièce d'une ligne.
}

void TetrisBoard::pauseGame() //Permet de pauser le jeu.
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

        case Qt::Key_Up:
            tryMove(curPiece.rotatedLeft(), curX, curY); //essaie de faire pivoter la pièce vers la gauche
            break;
        case Qt::Key_Space:
            dropCurrentPiece(); //fait tomber la pièce directement en bas
            break;
        case Qt::Key_Down:
            movePieceOneLineDown(); //fait descendre la pièce d'une case
            break;
        case Qt::Key_C:
            hold();//on stocke une pièce dans la boîte hold

        default:
            QFrame::keyPressEvent(event); //Par défaut on attend un événemenet clavier
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
                paintPiece(painter, rect.left() + j * squareWidth(),
                           boardTop + i * squareHeight(), shape); //dessin de la pièce à la position j,i
        }
    }
    if (curPiece.shape() != NoShape) {
            for (int i = 0; i < 4; ++i) {
                int x = curX + curPiece.x(i);
                int y = curY - curPiece.y(i);
                paintPiece(painter, rect.left() + x * squareWidth(),
                           boardTop + (BoardHeight - y - 1) * squareHeight(),
                           curPiece.shape());
            }
    }

    if (isGameOver) {
        flashGameOverMessage(painter);
    }
}
void TetrisBoard::flashGameOverMessage(QPainter &painter)
{
    QString message = tr("Game Over");
    QFont font("Arial", 20, QFont::Bold, true);
    QFontMetrics fm(font);
    int textWidth = fm.horizontalAdvance(message);
    int textHeight = fm.height();

    // Flash colors
    QColor colors[] = {Qt::red, Qt::green, Qt::white, Qt::blue, Qt::magenta};
    static int colorIndex = 0;

    painter.setFont(font);
    painter.setPen(colors[colorIndex]);
    painter.drawText(width() / 2 - textWidth / 2,
                     height() / 2 - textHeight / 2, message);

    ++colorIndex;
    if (colorIndex >= 5) {
        colorIndex = 0;
    }
    QTimer::singleShot(100,this, &TetrisBoard::updateSlot);
    //QTimer::singleShot(100, this, &QWidget::update());
}
void TetrisBoard::updateSlot()
{
    update();
}

void TetrisBoard::timerEvent(QTimerEvent *event)
{
    // Vérifie si l'événement correspond à l'identifiant de temporisation du timer de TetrisBoard
    if (event->timerId() != timer.timerId()) {
        // Si ce n'est pas le cas, appelle la méthode timerEvent de la classe QFrame
        QFrame::timerEvent(event);
        return;
    }

    // Si le jeu est en attente après avoir complété une ligne
    if (isWaitingAfterLine) {
        // Réinitialise l'indicateur d'attente
        isWaitingAfterLine = false;
        // Génère une nouvelle pièce
        createNewPiece();
        // Relance le timer avec le temps d'attente défini par la méthode timeoutTime de TetrisBoard
        timer.start(timeoutTime(), this);
    } else {
        // Si le jeu n'est pas en attente, fait descendre la pièce d'une case
        movePieceOneLineDown();
    }
}
void TetrisBoard::pieceDropped()
{
    // Parcourt les 4 blocs de la pièce courante et les place dans la grille
    for (int i = 0; i < 4; i++) {
        int x = curX + curPiece.x(i);
        int y = curY - curPiece.y(i);
        shapeAt(x, y) = curPiece.shape();
    }


    // Si on n'est pas en attente après la suppression d'une ligne, génère une nouvelle pièce
    if (!isWaitingAfterLine) {
        createNewPiece(); // Génère une nouvelle pièce
    }

    // Supprime toutes les lignes complètes de la grille, si il y en a
    removeFullLines();
}

void TetrisBoard::clearBoard()
{
    for (int i = 0; i < BoardHeight * BoardWidth; i++)
        board[i] = NoShape; //initialisation de chaque case de la grille à "NoShape"
}
void TetrisBoard::dropCurrentPiece()
{
    //int drop = 0; //hauteur cible de la chute
    int newY = curY; // coordonnée en Y de la pièce
    while (newY > 0 && tryMove(curPiece, curX, newY - 1)) { // tant que la pièce peut être déplacée vers le bas
        newY--; // déplacer la pièce vers le bas
        //drop++;
    }
    pieceDropped(); // finaliser la position de la pièce
}
void TetrisBoard::movePieceOneLineDown()
{
    if (!tryMove(curPiece, curX, curY - 1))// on essaie de bouger la pièce vers le bas
        pieceDropped();// si ce n'est pas possible la position de la pièce est finalisée.
}
void TetrisBoard::removeFullLines()
{
    int numFullLines = 0; // Le nombre de lignes pleines qui ont été supprimées

    for (int i = BoardHeight - 1; i >= 0; --i) { // Parcours du tableau de bas en haut
        bool lineIsFull = true; // Variable pour vérifier si la ligne est pleine

        for (int j = 0; j < BoardWidth; ++j) { // Parcours de chaque case de la ligne
            if (shapeAt(j, i) == NoShape) { // Si la case est vide, la ligne n'est pas pleine
                lineIsFull = false;
                break;
            }
        }

        if (lineIsFull) { // Si la ligne est pleine, on la supprime
            ++numFullLines; // On incrémente le nombre de lignes pleines trouvées
            for (int k = i; k < BoardHeight - 1; ++k) { // Décale toutes les lignes au-dessus de la ligne pleine vers le bas
                for (int j = 0; j < BoardWidth; ++j) {
                    shapeAt(j, k) = shapeAt(j, k + 1);
                }
            }
            for (int j = 0; j < BoardWidth; ++j) { // La ligne du haut devient vide
                shapeAt(j, BoardHeight - 1) = NoShape;
            }
        }
    }
    if (numFullLines > 0) { // Si au moins une ligne a été supprimée
        numLignesRmv += numFullLines; // On incrémente le nombre total de lignes supprimées
        // Calcul du score en fonction du nombre de lignes supprimées
        if (numFullLines == 1) {
            score += 40*(level +1 );
        } else if (numFullLines == 2) {
            score += 100*(level +1);
        } else if (numFullLines == 3) {
            score += 300*(level +1);
        } else if (numFullLines >= 4) {
            score += 1200*(level +1);
        }
        emit lignesChange(numLignesRmv);
        emit scoreChange(score);
        isWaitingAfterLine = true; // On attend avant de faire apparaître la prochaine pièce

        // Vérifie si le niveau doit être incrémenté
        if (numLignesRmv % 5 == 0) { // Tous les 5 lignes supprimées, le niveau augmente
            ++level;
            //emit levelChange(level); // Émet un signal pour mettre à jour l'affichage du niveau
        }

        curPiece.setShape(NoShape); // On supprime la pièce courante
        update(); // Met à jour l'affichage
    }

}

void TetrisBoard::hold()
{
    if (!isStarted || isPaused || curPiece.shape() == NoShape)
        return;

    if (heldPiece.shape() == NoShape) {
            // Si aucune pièce n'a été stockée auparavant, on la stocke
            heldPiece = curPiece;
            createNewPiece();
        }
    else {
            // Si une pièce a déjà été stockée, on échange la pièce actuelle avec celle stockée
            Tetromino tmpPiece = heldPiece;
            heldPiece = curPiece;
            curPiece = tmpPiece;
            curX = BoardWidth / 2 + curPiece.minX();
            curY = BoardHeight - 1 + curPiece.minY();
        }

        // On marque la pièce stockée comme étant utilisée
        isPieceHeld = true;

    // Met à jour l'affichage de la pièce stockée et du score
    updateHoldPieceLabel();
}

void TetrisBoard::updateHoldPieceLabel()
{
    if (!heldPieceLabel) {
        return;
    }

    // Calcul des dimensions de la pièce
    int dx = heldPiece.maxX() - heldPiece.minX() + 1;
    int dy = heldPiece.maxY() - heldPiece.minY() + 1;

    // Création d'une QImage pour dessiner la pièce
    QImage image(dx * squareWidth(), dy * squareHeight(), QImage::Format_RGB32);

    // Remplissage de l'image avec la couleur de fond du QLabel
    QColor backgroundColor = heldPieceLabel->palette().color(QPalette::Window);
    image.fill(backgroundColor);

    // Création d'un QPainter pour dessiner sur l'image
    QPainter painter(&image);

    // Dessin de chaque carré de la pièce sur l'image
    for (int i = 0; i < 4; ++i) {
        int x = heldPiece.x(i) - heldPiece.minX();
        int y = heldPiece.y(i) - heldPiece.minY();
        paintPiece(painter, x * squareWidth(), y * squareHeight(),
                   heldPiece.shape());
    }

    // Affichage de l'image sur le QLabel pour la pièce stockée
    heldPieceLabel->setPixmap(QPixmap::fromImage(image));
}



void TetrisBoard::showNextPiece()
{
   if (!nextPieceLabel) {
      return;
    }

    // Calcul des dimensions de la pièce
    int dx = nextPiece.maxX() - nextPiece.minX() + 1;
    int dy = nextPiece.maxY() - nextPiece.minY() + 1;

    // Création d'une QImage pour dessiner la pièce
    QImage image(dx * squareWidth(), dy * squareHeight(), QImage::Format_RGB32);

    // Remplissage de l'image avec la couleur de fond du QLabel
    QColor backgroundColor = nextPieceLabel->palette().color(QPalette::Window);
    image.fill(backgroundColor);

    // Création d'un QPainter pour dessiner sur l'image
    QPainter painter(&image);

    // Dessin de chaque carré de la pièce sur l'image
    for (int i = 0; i < 4; ++i) {
        int x = nextPiece.x(i) - nextPiece.minX();
        int y = nextPiece.y(i) - nextPiece.minY();
        paintPiece(painter, x * squareWidth(), y * squareHeight(),
                   nextPiece.shape());
    }

    // Affichage de l'image sur le QLabel pour la prochaine pièce
    nextPieceLabel->setPixmap(QPixmap::fromImage(image));

}


bool TetrisBoard::tryMove(const Tetromino &newPiece, int newX, int newY)
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

void TetrisBoard::createNewPiece()
{
    curPiece = nextPiece;
    nextPiece.setRandomShape();
    showNextPiece();

    curX = BoardWidth / 2 + 1;
    curY = BoardHeight - 1 + curPiece.minY();

    if (!tryMove(curPiece, curX, curY)) {
        curPiece.setShape(NoShape);
        isGameOver = true;
    }

    if (isGameOver) {
        timer.stop();
        QMessageBox::information(this, tr("Game Over"),
           tr("Vous avez Perdu"));
    }
}


void TetrisBoard::paintPiece(QPainter &painter, int x, int y, TetrisShape shape)
{// Cette fonction est purement esthetique.
    QColor color;
    //I,O,T,L,J,Z,S, NoShape
    switch (shape) {
        case NoShape:

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
            color = Qt::gray; ;
            break;
    }

    painter.fillRect(x + 1, y + 1, squareWidth() - 2, squareHeight() - 2, color);

    painter.setPen(QPen(color.lighter(), 2));
    painter.drawLine(x, y + squareHeight() - 1, x, y);
    painter.drawLine(x, y, x + squareWidth() - 1, y);

    painter.setPen(QPen(color.darker(), 2));
    painter.drawLine(x + 1, y + squareHeight() - 1,
                     x + squareWidth() - 1, y + squareHeight() - 1);
    painter.drawLine(x + squareWidth() - 1, y + squareHeight() - 1,
                     x + squareWidth() - 1, y + 1);

}

