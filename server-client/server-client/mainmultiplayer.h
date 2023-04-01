#ifndef MAINMULTIPLAYER_H
#define MAINMULTIPLAYER_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainMultiplayer; }
QT_END_NAMESPACE

class MainMultiplayer : public QMainWindow
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
};
#endif // MAINMULTIPLAYER_H
