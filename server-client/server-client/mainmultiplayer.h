#ifndef MAINMULTIPLAYER_H
#define MAINMULTIPLAYER_H

#include <QMainWindow>
#include "connection_window.h"
#include "host_window.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainMultiplayer; }
QT_END_NAMESPACE

class MainMultiplayer : public Host_window
{
    Q_OBJECT

public:
    MainMultiplayer(QWidget *parent = nullptr);
    ~MainMultiplayer();

private slots:

    void on_create_server_button_clicked();

    void on_connect_button_clicked();

private:
    Ui::MainMultiplayer *ui;
    Connection_window *connection_window;
    Host_window *host_window;
};
#endif // MAINMULTIPLAYER_H
