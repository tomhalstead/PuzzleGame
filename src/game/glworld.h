#ifndef GLWORLD_H
#define GLWORLD_H

#include "gltileset.h"
#include "glinterface.h"
#include "world.h"
#include "player.h"
#include "glsplash.h"
#include "gldialog.h"

class GLWorld: public GLInterface
{
public:
    GLWorld(GLWindow *parent, QString resourcePrefix = ":/campaign/",int NetworkMode = 0);
    ~GLWorld();
    virtual bool resizeEvent(QResizeEvent *);
    virtual void paint(QPainter& painter);
    virtual bool processKey(int key);
    virtual void Close();
    virtual void Open();
    void loadWorld(const QString &fileName, size_t player = 0, const QString &saveFile = "");
    static void processDialog(GLInterface* caller, int result);
    void SaveState();
    static const int NETWORK_NONE   = 0;
    static const int NETOWRK_HOST   = 1;
    static const int NETOWRK_CLIENT = 2;
private:
    void setTileDimensions();
    void setRoomPosition();
    void loadExtraXMLInfo(const QDomElement& root);
    bool move(int location, int index, bool moveX, int amount);
    bool activateTile(int playerIndex);
    void getStartPosition(int& startRow, int& startCol);
    void drawPlayer(int top, int left);
    void loadState(const QString& fileName);
    void setMessage(const QString& msg);
    World* world;
    Room* room;
    int tileSize, curTileSet;
    std::vector<GLTileSet> tileSets;
    std::vector<Player> players;
    int titleBorder, tilesWide, tilesHigh, startX, startY, roomX, roomY;
    size_t curPlayer;
    GLSplash *intro, *victory;
    int netMode;
    QPixmap *roomDisplay;
    QString prefix, message;
    int msgCount;
    std::vector<GLSprite> sprites;
    InterfaceState state;
    std::vector<Item> itemList;
    int itemTileSet;
};

#endif // GLWORLD_H
