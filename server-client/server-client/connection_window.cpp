#include "connection_window.h"
#include "ui_connection_window.h"

Connection_window::Connection_window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Connection_window)
{
    ui->setupUi(this);
}

Connection_window::~Connection_window()
{
    delete ui;
}


void Connection_window::on_goback_Button_clicked()
{
    this->close();
    emit main_multiplayer_Window();
}

