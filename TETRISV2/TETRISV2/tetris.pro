QT += widgets

HEADERS       = tetrisboard.h \
                tetrispiece.h \
                tetriswindow.h
SOURCES       = main.cpp \
                tetrisboard.cpp \
                tetrispiece.cpp \
                tetriswindow.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/widgets/tetrix
INSTALLS += target
