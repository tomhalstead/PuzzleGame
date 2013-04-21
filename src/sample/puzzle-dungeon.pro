TEMPLATE = app
CONFIG += console
CONFIG += qt
QT += xml

SOURCES += main.cpp \
    ../map/link.cpp \
    ../map/map.cpp \
    ../map/room.cpp \
    ../map/tileinfo.cpp \
    ../map/world.cpp \

INCLUDEPATH += ../../include

HEADERS += \
    matrix.h \
    graph.h \
    room.h \
    map.h \
    link.h \
    tile.h \
    tileinfo.h \
    world.h \

