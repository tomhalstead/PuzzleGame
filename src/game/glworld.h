#ifndef GLWORLD_H
#define GLWORLD_H

#include "glinterface.h"
#include "world.h"
#include "player.h"
#include "glsplash.h"

class GLWorld: public GLInterface
{
public:
    GLWorld(GLWindow *parent, GLInterface *MainMenu = 0);
    virtual QPixmap* resizeEvent(QResizeEvent *);
    virtual QPixmap* paint();
    virtual QPixmap* processKey(int key);
    virtual void Close();
    virtual void Open();
    GLInterface* loadWorld(const QString &fileName, size_t player = 0);
private:
    void setTileDimensions();
    void setRoomPosition();
    void loadExtraXMLInfo(const QDomElement& root);
    bool move(int location, int index, bool moveX, int amount);
    World* world;
    Room* room;
    std::vector<QPixmap> tileSets;
    std::vector<Player> players;
    int titleBorder, tileSize, tilesWide, tilesHigh, startX, startY, roomX, roomY;
    size_t curPlayer;
    GLSplash *intro, *victory;
    GLInterface* mainMenu;
};

#endif // GLWORLD_H
