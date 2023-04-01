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


void MainMultiplayer::on_create_server_button_clicked()
{

}


void MainMultiplayer::on_connect_button_clicked()
{

}

