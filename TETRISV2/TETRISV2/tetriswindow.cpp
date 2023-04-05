#include "tetriswindow.h"
#include "tetrisboard.h"
#include <QPainter>
#include <QtWidgets>
#include <QCoreApplication>
#include <QGridLayout>
#include <QLabel>
#include <QLCDNumber>
#include <QPushButton>
TetrisWindow::TetrisWindow(QWidget *parent) : QWidget(parent), board(new TetrisBoard) //le constructeur crée la grille et l'endroit où l'on indiquera la prochaine pièce
{
   /*La classe QLabel est une classe de widget qui peut afficher du texte ou une image. Dans ce cas, elle est utilisée pour afficher du texte, et les méthodes setFrameStyle et setAlignment sont utilisées pour configurer l'apparence du widget. La méthode setFont est utilisée pour spécifier la police utilisée pour afficher le texte.
    La méthode setStyleSheet est utilisée pour configurer la feuille de style du widget QLabel,
    spécifiant les couleurs et les propriétés d'affichage, en utilisant une syntaxe similaire aux feuilles de style CSS.
    Enfin, les widgets nextPieceLabel et heldPieceLabel sont transmis à un objet board,
    probablement un objet de jeu, en utilisant les méthodes setNextPieceLabel et setHeldPieceLabel,
    pour indiquer à cet objet où afficher ces widgets sur l'interface graphique.
    */
    nextPieceLabel = new QLabel;
    nextPieceLabel->setFrameStyle(QFrame::Box | QFrame::Raised);
    nextPieceLabel->setAlignment(Qt::AlignCenter);
    nextPieceLabel->setFont(QFont("8514OEM", 20, QFont::Bold));
    nextPieceLabel->setStyleSheet("color: #fac898;");
    board->setNextPieceLabel(nextPieceLabel);

 /*  levelLabel = new QLabel(this);
    levelLabel->setGeometry(0, 0, width(), 20);
    levelLabel->setAlignment(Qt::AlignCenter);
    levelLabel->setStyleSheet("QLabel { background-color : rgba(255, 255, 255, 0%); }");

*/
    heldPieceLabel = new QLabel;
    heldPieceLabel->setFrameStyle(QFrame::Box | QFrame::Raised);
    heldPieceLabel->setAlignment(Qt::AlignCenter);
    heldPieceLabel->setFont(QFont("8514OEM", 20, QFont::Bold));
    heldPieceLabel->setStyleSheet("color: #fac898;");
    board->setHeldPieceLabel(heldPieceLabel);

    score = new QLCDNumber(5);// cela correspond à l'affichage du score en jeu
    score->setSegmentStyle(QLCDNumber::Filled);
    lignes = new QLCDNumber(5);//
    lignes ->setSegmentStyle(QLCDNumber::Filled);



    //ici on crée des boutons quit, start, pause aux fonctionalités respectives pour faciliter l'experience de l'utilisateur.
    start = new QPushButton(tr("&Commencer"));
    start->setFocusPolicy(Qt::NoFocus); //Ici on a mis NoFocus car on ne veut pas que ces boutons prennent le dessus sur le jeu au niveau des entrées clavier.
    start->setStyleSheet("font-size: 20px; color: #fac898; font-family: 8514OEM;");
    quit = new QPushButton(tr("&Quitter"));
    quit->setFocusPolicy(Qt::NoFocus);
    quit->setStyleSheet("font-size: 20px; color: #fac898; font-family: 8514OEM;");
    pause = new QPushButton(tr("&Pause"));
    pause->setFocusPolicy(Qt::NoFocus);
    pause->setStyleSheet("font-size: 20px; color: #fac898; font-family: 8514OEM;");

    //Ici on associe les boutons créés au dessus avec les fonctionalités respectives.
    connect(start, &QPushButton::clicked, board, &TetrisBoard::startGame);
    connect(quit , &QPushButton::clicked, qApp, &QCoreApplication::quit);
    connect(pause, &QPushButton::clicked, board, &TetrisBoard::pauseGame);
    connect(board, &TetrisBoard::scoreChange, score, qOverload<int>(&QLCDNumber::display));
    connect(board, &TetrisBoard::lignesChange, lignes, qOverload<int>(&QLCDNumber::display));

    QGridLayout *layout = new QGridLayout; //need to change some values here.

    layout->addWidget(createLabel(tr("PROCHAINE PIECE")), 0, 0);
    layout->addWidget(nextPieceLabel, 1, 0);
    layout->addWidget(createLabel(tr("PIECE STOCKEE")), 2, 0);
    layout->addWidget(heldPieceLabel, 3, 0);
    layout->addWidget(start, 4, 0);
    layout->addWidget(board, 0, 1, 6, 1);
    layout->addWidget(createLabel(tr("SCORE")), 0, 2);
    layout->addWidget(score, 1, 2);
    layout->addWidget(createLabel(tr("LIGNES")), 2, 2);
    layout->addWidget(lignes, 3, 2);
    layout->addWidget(quit, 4, 2);
    layout->addWidget(pause, 5, 2);
    setLayout(layout);

    setWindowTitle(tr("TETRIS"));
    resize(600, 350);
    setStyleSheet("background-color: #2aa198;");


}


QLabel *TetrisWindow::createLabel(const QString &text) // cette fonction sert essentiellement à ce que toutes les fenêtres auxiliaires soient terminées lorsque l'on quitte le jeu.
{
    QLabel *label = new QLabel(text);
    label->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    label->setStyleSheet("font-size: 20px; color: #fac898; font-family: 8514OEM;");

    return label;
}
