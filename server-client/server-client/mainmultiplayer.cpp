#include "mainmultiplayer.h"
#include "./ui_mainmultiplayer.h"

MainMultiplayer::MainMultiplayer(QWidget *parent)
    : Host_window(parent)
    , ui(new Ui::MainMultiplayer)
{
    ui->setupUi(this);

    // Инициализируем второе окно
    connection_window = new Connection_window();
    // подключаем к слоту запуска главного окна по кнопке во втором окне
    connect(connection_window, &Connection_window::main_multiplayer_Window, this, &MainMultiplayer::show);

    // Инициализируем третье окно
    host_window = new Host_window();
    // подключаем к слоту запуска главного окна по кнопке в третьем окне
    connect(host_window, &Host_window::main_multiplayer_Window, this, &MainMultiplayer::show);

}

MainMultiplayer::~MainMultiplayer()
{
    delete ui;
}


void MainMultiplayer::on_connect_button_clicked()
{
//  this->close();
    connection_window->show();
}


void MainMultiplayer::on_create_server_button_clicked()
{
    //this->close();
    host_window->show();
}

