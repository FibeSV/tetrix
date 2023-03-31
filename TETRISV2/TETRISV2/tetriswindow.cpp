#include "tetriswindow.h"
#include <QtWidgets>
TetrisWindow::TetrisWindow(QWidget *parent) : QWidget(parent), board(new TetrisBoard) //le constructeur crée la grille et l'endroit où l'on indiquera la prochaine pièce
{
    score = new QLCDNumber(5);// cela correspond à l'affichage du score en jeu
    score->setSegmentStyle(QLCDNumber::Filled);
    lignes = new QLCDNumber(5);//
    lignes ->setSegmentStyle(QLCDNumber::Filled);

    //ici on crée des boutons quit, start, pause aux fonctionalités respectives pour faciliter l'experience de l'utilisateur.
    start = new QPushButton(tr("&Commencer"));
    start->setFocusPolicy(Qt::NoFocus); //Ici on a mis NoFocus car on ne veut pas que ces boutons prennent le dessus sur le jeu au niveau des entrées clavier.
    quit = new QPushButton(tr("&Quitter"));
    quit->setFocusPolicy(Qt::NoFocus);
    pause = new QPushButton(tr("&Pause"));
    pause->setFocusPolicy(Qt::NoFocus);

    //Ici on associe les boutons créés au dessus avec les fonctionalités respectives.
    connect(start, &QPushButton::clicked, board, &TetrisBoard::start);
    connect(quit , &QPushButton::clicked, qApp, &QCoreApplication::quit);
    connect(pause, &QPushButton::clicked, board, &TetrisBoard::pause);
    connect(board, &TetrisBoard::scoreChange, score, qOverload<int>(&QLCDNumber::display));
    connect(board, &TetrisBoard::lignesChange, lignes, qOverload<int>(&QLCDNumber::display));

    QGridLayout *layout = new QGridLayout; //need to change some values here.
    layout->addWidget(createLabel(tr("Suivant")), 0, 0);
    layout->addWidget(nextPiece, 1, 0);
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
}


QLabel *TetrisWindow::createLabel(const QString &text) // cette fonction sert essentiellement à ce que toutes les fenêtres auxiliaires soient terminées lorsque l'on quitte le jeu.
{
    QLabel *label = new QLabel(text);
    label->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    return label;
}
