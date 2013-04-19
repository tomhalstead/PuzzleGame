TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += main.cpp \
    ../map/room.cpp \
    ../map/map.cpp \
    ../map/link.cpp

INCLUDEPATH += ../../include

HEADERS += \
    matrix.h \
    graph.h \
    room.h \
    map.h \
    link.h \
    tile.h

