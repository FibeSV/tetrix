#include "tetriswindow.h"
#include <QApplication>
#include <QPixmap>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    TetrisWindow window;

    QPixmap logoPixmap("C:/Users/Pinata/Desktop/ENSTA/TETRIS/Tetris_Syoma_Q/tetrix/TETRISV2/TETRISV2/Tetris.jpg");
    window.setWindowIcon(QIcon(logoPixmap));

    window.show();
    return app.exec();
}
