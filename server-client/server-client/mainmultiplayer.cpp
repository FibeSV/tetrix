#include "mainmultiplayer.h"
#include "./ui_mainmultiplayer.h"

MainMultiplayer::MainMultiplayer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainMultiplayer)
{
    ui->setupUi(this);
}

MainMultiplayer::~MainMultiplayer()
{
    delete ui;
}

