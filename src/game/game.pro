TEMPLATE = app
CONFIG += qt
QT += xml
QT += opengl

SOURCES += \
    ../world/link.cpp \
    ../world/room.cpp \
    ../world/world.cpp \
    ../world/puzzle.cpp \
    ../world/puzzleitem.cpp \
    ../world/worldinfo.cpp \
    main.cpp \
    glinterface.cpp \
    glwindow.cpp \
    glworld.cpp \
    glmainmenu.cpp \
    glmenu.cpp \
    glsplash.cpp

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
    player.h \
    glinterface.h \
    glwindow.h \
    glworld.h \
    worldinfo.h \
    glmainmenu.h \
    glmenu.h \
    glsplash.h

RESOURCES += \
    resources.qrc
