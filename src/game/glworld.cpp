#include "glworld.h"
#include <QDebug>

GLWorld::GLWorld(GLWindow* parent, GLInterface *MainMenu): GLInterface(parent), world(NULL), room(NULL), intro(NULL),victory(NULL), mainMenu(MainMenu)
{
    QPixmap temp;
    temp.load("tileset.bmp");
    tileSets.push_back(temp);
    players.push_back(Player());
    tileSize = 32;
    setTileDimensions();

    Pen.setColor(QColor::fromRgb(255,255,255));
}

QPixmap *GLWorld::resizeEvent(QResizeEvent *event)
{
    GLInterface::resizeEvent(event);
    setTileDimensions();
    return paint();
}

QPixmap *GLWorld::paint()
{
    Painter.begin(Display);
    Painter.setRenderHint(QPainter::Antialiasing);
    Painter.setPen(Pen);
    Painter.setFont(Font);
    if(world && isOpen()) {
        Painter.fillRect(0,0,Display->width(),Display->height(),Qt::SolidPattern);
        Painter.drawText(QRect(0,0,Display->width(),titleBorder),room->Name,QTextOption(Qt::AlignCenter));
        for(size_t i = 0; i < room->Rows(); i++)
            for(size_t j = 0; j < room->Cols(); j++) {
                int draw = room->getTile(i,j).Draw;
                if(players[0].X == i && players[0].Y == j)
                    draw = 6;
                else {
                    PuzzleItem* p;

                    for(unsigned int k = 0; k < room->Puzzles.size(); k++) {
                        p = room->Puzzles.at(k)->getItem(i,j);
                        if(p) {
                            draw = (p->Status()) ? room->Puzzles.at(k)->TileSet->at(p->SetTile).Draw
                                                 : room->Puzzles.at(k)->TileSet->at(p->ClearTile).Draw;
                        }
                    }
                }
                Painter.drawPixmap(roomX + j*tileSize,roomY + i*tileSize,tileSize,tileSize,tileSets[room->SetIndex()],0,tileSize*draw,tileSize,tileSize);
            }

    }
    Painter.end();
    return Display;
}

QPixmap* GLWorld::processKey(int key)
{
    int check=-1, amount = 0;
    bool X = false;
    switch(key) {
    case Qt::Key_Escape:
        Close();
        break;
    case Qt::Key_W:
        if( players[0].X) {
            check = room->ItemIndex(players[0].X-1,players[0].Y);
            X = true;
            amount = -1;
        }
        break;
    case Qt::Key_A:
        if(players[0].Y) {
            check = room->ItemIndex(players[0].X,players[0].Y-1);
            amount = -1;
        }
        break;
    case Qt::Key_S:
        if(players[0].X <room->Rows()-1) {
            check = room->ItemIndex(players[0].X+1,players[0].Y);
            X = true;
            amount = 1;
        }
        break;
    case Qt::Key_D:
        if(players[0].Y < room->Cols()-1) {
            check = room->ItemIndex(players[0].X,players[0].Y+1);
            amount = 1;
        }
        break;
    default:
        break;
    }
    if(amount)
        if(move(check,0,X,amount))
            return paint();
    return NULL;
}

GLInterface* GLWorld::loadWorld(const QString &fileName, size_t player)
{
    if(world)
        delete world;
    world = new World;

    QFile f(fileName);
    QDomDocument d;
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
        throw WE_IO_ERROR;
    if(!d.setContent(&f)) {
        f.close();
        throw WE_INVALID_FILE;
    }
        loadExtraXMLInfo(d.firstChildElement());
        world->Load(d.firstChildElement());
        intro = new GLSplash(Parent,world->Info.IntroText,world->Info.IntroImage);
        victory = new GLSplash(Parent,world->Info.VictoryText,world->Info.VictoryImage);
        StartInfo si = world->Info.Start[player];
        players[player].X = si.Row;
        players[player].Y = si.Col;
        players[player].Room = si.RoomIndex;
        room = &world->getRoom(players[player].Room);
        curPlayer = player;
        setRoomPosition();
        return intro;
    }

    void GLWorld::Close()
    {
        qDebug() << "Close World";
        GLInterface::Close();
    }

    void GLWorld::Open()
    {
        qDebug() << "Open World";
        GLInterface::Open();
        intro->Open();
        Parent->Push(GLWindowInfo(this,true));
        Parent->Push(GLWindowInfo(intro));
    }

    void GLWorld::setTileDimensions() {
        titleBorder = Height() / 20;
        tilesWide = Width() / tileSize;
        tilesHigh = (Height()-titleBorder) / tileSize;
        startX = (Width() % tileSize)/2;
        startY = ((Height()-titleBorder) % tileSize)/2;
        setFontHeight(Font,titleBorder-10);
        if(room)
            setRoomPosition();
    }

    void GLWorld::setRoomPosition()
    {
        if(int(room->Cols()) < tilesWide)
            roomX = (tilesWide - room->Cols()) / 2;
    if(int(room->Rows()) < tilesHigh)
        roomY = (tilesHigh - room->Rows()) / 2;
    roomX = startX + (tileSize * roomX);
    roomY = startY + (tileSize * roomY);
}

void GLWorld::loadExtraXMLInfo(const QDomElement &root)
{

}

bool GLWorld::move(int location, int index, bool moveX, int amount)
{
    int connection = room->Connection(room->ItemIndex(players[index].X,players[index].Y),location);
    if(!connection)
        amount = 0;
    else if((connection & Tile::CONNECTION_PUZZLE) &&
            !room->Puzzles.at(room->getTile(location).RoomLink.Puzzle)->Solved())
        amount = 0;
    if(amount) {
        if(moveX)
            players[index].X += amount;
        else
            players[index].Y += amount;
        Tile t = room->getTile(players[index].X,players[index].Y);
        Link link;
        for(unsigned int i = 0; i < room->Puzzles.size(); i++)
            room->Puzzles.at(i)->Activate(players[index].X,players[index].Y);
        switch(t.Data) {
        case Tile::DATA_ROOMEDGE:
            for(size_t i = 0; i < room->Puzzles.size(); i++)
                if(!room->Puzzles.at(i)->Solved())
                    room->Puzzles.at(i)->Reset();
            link = t.RoomLink;
            players[index].X = link.Row;
            players[index].Y = link.Col;
            players[index].Room = link.Room;
            room = &world->getRoom(link.Room);
            setRoomPosition();
            break;
        case Tile::DATA_VICTORY:
            Parent->Push(victory);
            victory->Open();
            Close();

            break;
        default:
            break;
        }
        return true;
    }
    return false;
}
