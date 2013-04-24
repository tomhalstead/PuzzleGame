TEMPLATE = app
CONFIG += console
CONFIG += qt
QT += xml

SOURCES += main.cpp \
    ../map/link.cpp \
    ../map/room.cpp \
    ../map/world.cpp \
    ../map/puzzle.cpp \
    ../map/puzzleitem.cpp \

INCLUDEPATH += ../../include

HEADERS += \
    matrix.h \
    graph.h \
    room.h \
    link.h \
    tile.h \
    tileinfo.h \
    world.h \
    puzzle.h \
    puzzleitem.h \

