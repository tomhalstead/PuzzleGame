#ifndef WORLDINFO_H
#define WORLDINFO_H

#include <QtGui>

struct StartInfo {
    size_t RoomIndex;
    size_t Row;
    size_t Col;
};

class WorldInfo {
public:
    WorldInfo();
    ~WorldInfo();
    QString IntroText;
    QPixmap* IntroImage;
    QString VictoryText;
    QPixmap* VictoryImage;
    std::vector<StartInfo> Start;
};


#endif // WORLDINFO_H
