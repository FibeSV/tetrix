#ifndef CONNECTION_WINDOW_H
#define CONNECTION_WINDOW_H

#include <QMainWindow>

namespace Ui {
class Connection_window;
}

class Connection_window : public QMainWindow
{
    Q_OBJECT

public:
    explicit Connection_window(QWidget *parent = nullptr);
    ~Connection_window();
signals:
    void main_multiplayer_Window();
private slots:
    void on_goback_Button_clicked();

private:
    Ui::Connection_window *ui;
};

#endif // CONNECTION_WINDOW_H
