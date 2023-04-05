QT += widgets

HEADERS       = tetrisboard.h \
                tetriswindow.h \
                tetromino.h
SOURCES       = main.cpp \
                tetrisboard.cpp \
                tetriswindow.cpp \
                tetromino.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/widgets/tetrix
INSTALLS += target
