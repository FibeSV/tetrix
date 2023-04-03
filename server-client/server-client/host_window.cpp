#include "host_window.h"
#include "./ui_host_window.h"

Host_window::Host_window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Host_window)
{
    ui->setupUi(this);

    server = new QTcpServer();

    if(server->listen(QHostAddress::Any, 2323))
    {
       //connect(this, &Host_window::newMessage, this, &Host_window::displayMessage);
       //connect(server, &QTcpServer::newConnection, this, &Host_window::newConnection);
       statusBar()->showMessage("Server is listening...");
    }
    else
    {
        QMessageBox::critical(this,"QTCPServer",QString("Unable to start the server: %1.").arg(server->errorString()));
        exit(EXIT_FAILURE);
    }
}

Host_window::~Host_window()
{
    delete ui;
}

void Host_window::on_pushButton_clicked()
{
    this->close();
    emit main_multiplayer_Window();
}

