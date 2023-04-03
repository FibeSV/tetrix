#ifndef HOST_WINDOW_H
#define HOST_WINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QMetaType>
#include <QSet>
#include <QStandardPaths>
#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>

namespace Ui {
class Host_window;
}

class Host_window : public QMainWindow
{
    Q_OBJECT

public:
    explicit Host_window(QWidget *parent = nullptr);
    ~Host_window();

signals:
    void main_multiplayer_Window();

    //void newMessage(QString);




private slots:
    void on_pushButton_clicked();

    /*void newConnection();
    void appendToSocketList(QTcpSocket* socket);

    void readSocket();
    void discardSocket();
    void displayError(QAbstractSocket::SocketError socketError);
    void displayMessage(const QString& str);
    void sendMessage(QTcpSocket* socket);
    void sendAttachment(QTcpSocket* socket, QString filePath);

    void on_pushButton_sendMessage_clicked();
    void on_pushButton_sendAttachment_clicked();

    void refreshComboBox();*/

private:
    Ui::Host_window *ui;
    QTcpServer* server;
    QVector<QTcpSocket*> sockets;
    QByteArray Data;

};

#endif // HOST_WINDOW_H
