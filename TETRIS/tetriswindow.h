#include <QWidget>
#include <QLabel>
#include <QLCDNumber>
#include <QPushButton>
#include "tetrisboard.h"
#ifndef TETRISWINDOW_H
#define TETRISWINDOW_H


class TetrisWindow : public QWidget /*Définition de la classe TetrisWindow*/

{
Q_OBJECT

public:
    TetrisWindow(QWidget *parent = nullptr);

private: //ici on définit tout les éléments visuels que l'on veut voir apparaitre
    QLabel *createLabel(const QString &text);

    TetrisBoard *board; //notre grille de jeu

    QLabel *nextPiece; //l'affichage de la pièce suivante

    QLCDNumber *score;
    QLCDNumber *lignes;

    QPushButton *start;
    QPushButton *quit;
    QPushButton *pause;
};

#endif // TETRISWINDOW_H
