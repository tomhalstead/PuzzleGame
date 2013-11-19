#include "glworld.h"
#include <QDebug>

GLWorld::GLWorld(GLWindow* parent,QString resourcePrefix, int NetworkMode):
    GLInterface(parent), world(NULL), room(NULL), intro(NULL),victory(NULL),
    netMode(NetworkMode), roomDisplay(NULL), prefix(resourcePrefix), state(STATE_INIT)
{
    Pen.setColor(QColor::fromRgb(255,255,255));
}

GLWorld::~GLWorld()
{
    if(world)
        delete world;
    if(intro)
        delete intro;
    if(victory)
        delete victory;
    if(roomDisplay)
        delete roomDisplay;
}

bool GLWorld::resizeEvent(QResizeEvent *event)
{
    if(GLInterface::resizeEvent(event)) {
        setTileDimensions();
        return true;
    }
    return false;
}

void GLWorld::paint(QPainter& painter)
{
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Pen);
    painter.setFont(Font);
    if(state == STATE_NORMAL) {
        painter.fillRect(0,0,Width(),Height(),Qt::black);
        painter.drawText(QRect(0,0,Width(),titleBorder),room->Name,QTextOption(Qt::AlignCenter));
        painter.drawText(QRect(0,Height()-titleBorder,Width(),titleBorder),message,QTextOption(Qt::AlignVCenter | Qt::AlignLeft));
        if(!--msgCount)
            message = "";
        int startRow, startCol, lastDraw=-1, curDraw, puzzleDraw;
        getStartPosition(startRow,startCol);
        QRect destRect(roomX,roomY,tileSize,tileSize), sourceRect;
        QPixmap* sourcePixmap;
        for(size_t j = 0;(int(j) < tilesHigh) && (unsigned(startRow) + j < room->Rows()); j++) {
            for(size_t i = 0; (int(i) < tilesWide) && (unsigned(startCol) + i < room->Cols()); i++) {
                curDraw = room->getTile(j+startRow,i+startCol).Draw;
                if(curDraw != lastDraw) {
                    sourcePixmap = tileSets[curTileSet].TileImages();
                    sourceRect = tileSets[curTileSet].Tile(curDraw);
                    lastDraw = curDraw;
                }
                painter.drawPixmap(destRect,*sourcePixmap,sourceRect);
                for(unsigned int k = 0; k < room->Puzzles.size(); k++) {
                    PuzzleItem* p;
                    GLTileSet* t;
                    p = room->Puzzles[k]->getItem(j+startRow,i+startCol);
                    if(p) {
                        t = &tileSets[room->Puzzles[k]->getTileSet().Index];
                        std::vector<TileInfo>* info = &room->Puzzles[k]->getTileSet().Tiles;
                        puzzleDraw = (p->Status()) ? info->at(p->SetTile).Draw
                                             : info->at(p->ClearTile).Draw;
                        painter.drawPixmap(destRect,*t->TileImages(),t->Tile(puzzleDraw));
                    }
                }
                destRect.translate(tileSize,0);
            }
            destRect.setLeft(roomX);
            destRect.setWidth(tileSize);
            destRect.translate(0,tileSize);
        }
        for(unsigned int i = 0; i < players.size(); i++) {
            if( players[i].Room == players[curPlayer].Room &&
                    (startRow <= int(players[i].Y)) &&
                    (int(players[i].Y) <= startRow + tilesHigh ) &&
                    (startCol <= int(players[i].X)) &&
                    (int(players[i].X) <= (startCol + tilesWide) ))
            {
                GLSprite* player = &sprites[players[i].Sprite];
                player->setTile(players[i].Facing);
                int playerDrawRow = (players[i].Y - startRow);
                int playerDrawCol = (players[i].X - startCol);
                destRect.setLeft(roomX + playerDrawCol * tileSize + ((tileSize-player->Width())/2));
                destRect.setTop(roomY + playerDrawRow * tileSize  + (tileSize * .8) - player->Height());
                destRect.setWidth(player->Width());
                destRect.setHeight(player->Height());
                player->Draw(painter,destRect);
            }
        }
    }
}

bool GLWorld::processKey(int key)
{
    qDebug() << "GLWorld::processKey:" << key;
    if(state != STATE_NORMAL)
        return false;
    int check=-1, amount = 0;
    bool X = false;
    QStringList menuItems;
    QRect menuRect;
    GLDialog* dialog;
    GLWindowInfo* wi;
    int menuSize;
    switch(key) {
    case Qt::Key_Escape:
        qDebug() <<"Key_Escape";
        menuItems << "SAVE" << "CANCEL" << "MAIN MENU";
        menuSize = QFontMetrics(Font).width("MAIN MENU")+20;
        menuRect.setRect((Width()-menuSize)/2,(Height()-menuSize)/2,menuSize,menuSize);
        state = STATE_DIALOG;
        qDebug() << "Create Dialog";
        try {
            dialog = new GLDialog(Parent,(this),menuItems,&GLWorld::processDialog,menuRect,titleBorder);
        }
        catch(std::bad_alloc e) {
            qDebug() << "bad alloc";
        }
        catch(...) {
            qDebug() << "other";
        }

        qDebug() << "Create WindowInfo";
        wi = new GLWindowInfo(dialog);
        qDebug() << " Push window";
        Parent->Push(wi);
        break;
    case Qt::Key_W:
    case Qt::Key_Up:
        qDebug() <<"Up";
        players[curPlayer].Facing = 0;
        sprites[players[curPlayer].Sprite].Advance();
        if( players[curPlayer].Y) {
            check = room->ItemIndex(players[curPlayer].Y-1,players[curPlayer].X);
            amount = -1;
        }
        break;
    case Qt::Key_A:
    case Qt::Key_Left:
        qDebug() <<"Left";
        players[curPlayer].Facing = 3;
        sprites[players[curPlayer].Sprite].Advance();
        if(players[curPlayer].X) {
            check = room->ItemIndex(players[curPlayer].Y,players[curPlayer].X-1);
            X = true;
            amount = -1;
        }
        break;
    case Qt::Key_S:
    case Qt::Key_Down:
        qDebug() <<"Down";
        players[curPlayer].Facing = 2;
        sprites[players[curPlayer].Sprite].Advance();
        if(players[curPlayer].Y <room->Rows()-1) {
            check = room->ItemIndex(players[curPlayer].Y+1,players[curPlayer].X);
            amount = 1;
        }
        break;
    case Qt::Key_D:
    case Qt::Key_Right:
        qDebug() <<"Right";
        players[curPlayer].Facing = 1;
        sprites[players[curPlayer].Sprite].Advance();
        if(players[curPlayer].X < room->Cols()-1) {
            check = room->ItemIndex(players[curPlayer].Y,players[curPlayer].X+1);
            X = true;
            amount = 1;
        }
        break;
    default:
        qDebug() <<"Other";
        break;
    }
    if(amount)
        if(move(check,0,X,amount))
            return activateTile(0);
    qDebug() << "Finished GLWorld::processKey";
    return true;
}

void GLWorld::loadWorld(const QString &fileName, size_t player, const QString& saveFile)
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
        qDebug() << "GLWorld XML load error";
        throw WE_INVALID_FILE;
    }
    world->Load(d.firstChildElement());
    qDebug() << "World loaded, initializing graphics.";
    tileSets.resize(world->getNumTileSets(),GLTileSet());
    qDebug() << "Load " << tileSets.size() << " tileset images";
    for(unsigned int i = 0; i < tileSets.size(); i++)
        tileSets[i] = GLTileSet(prefix + world->getTileSet(i).FileName,world->getTileSet(i).TileSize);;
    qDebug() << "Load intro/outro screens";
    intro = new GLSplash(Parent,world->Info.IntroText,world->Info.IntroImage);
    victory = new GLSplash(Parent,world->Info.VictoryText,world->Info.VictoryImage);
    qDebug() << "Load player start info";
    for(unsigned int i = 0; i < world->Info.Start.size(); i++)
        players.push_back(Player(world->Info.Start[i]));
    loadExtraXMLInfo(d.firstChildElement());
    room = &world->getRoom(players[player].Room);
    curTileSet = room->getTileSet().Index;
    curPlayer = player;
    tileSize = tileSets[curTileSet].getTileSize();
    setTileDimensions();
    if(saveFile == "")
        state = STATE_NORMAL;
    else
        loadState(saveFile);
}

void GLWorld::processDialog(GLInterface *caller, int result)
{
    if(result == 0)
        ((GLWorld*)caller)->SaveState();
    else if(result == 2)
        caller->Close();
    ((GLWorld*)caller)->state = STATE_NORMAL;
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
    Parent->Push(new GLWindowInfo(intro,false));
}

void GLWorld::setTileDimensions() {
    titleBorder = Height() / 40;
    tilesWide = Width() / tileSize;
    tilesHigh = (Height()-titleBorder*2) / tileSize;
    startX = (Width() % tileSize)/2;
    startY = titleBorder;
    setFontHeight(Font,titleBorder);
    if(room)
        setRoomPosition();
}

void GLWorld::setRoomPosition()
{
    roomX = (int(room->Cols()) < tilesWide) ? (tilesWide - room->Cols()) / 2 : 0;
    roomY = (int(room->Rows()) < tilesHigh) ? (tilesHigh - room->Rows()) / 2 : 0;
    roomX = startX + (tileSize * roomX);
    roomY = startY + (tileSize * roomY);
}

void GLWorld::loadExtraXMLInfo(const QDomElement &root)
{
    qDebug() << "Load extra XML";
    QDomElement child = root.firstChildElement("sprites");
    sprites.resize(child.childNodes().count(),GLSprite());
    for(child = child.firstChildElement(); !child.isNull(); child = child.nextSiblingElement()) {
        sprites[child.attribute("id").toUInt()] = GLSprite(prefix + child.attribute("fileName"),
                                   child.attribute("width").toInt(),
                                   child.attribute("height").toInt(),
                                   child.attribute("default").toInt());
    }
    for(child = root.firstChildElement("player");
        !child.isNull();
        child = child.nextSiblingElement("player"))
    {
        players[child.attribute("id").toUInt()].Sprite = child.attribute("sprite").toInt();
    }
    child = root.firstChildElement("items");
    itemList.resize(child.childNodes().size(),Item());
    for(child = child.firstChildElement("item");
        !child.isNull(); child = child.nextSiblingElement("item")) {
        Item* curItem = &itemList[child.attribute("id").toUInt()];
        curItem->Data = child.attribute("data").toInt();
        curItem->Type = child.attribute("type").toInt();
        curItem->Name = child.attribute("name");
        curItem->Description = child.attribute("description");
    }
}

bool GLWorld::move(int location, int index, bool moveX, int amount)
{
    qDebug() << "GLWorld::move()";
    int connection = room->Connection(room->ItemIndex(players[index].Y,players[index].X),location);
    qDebug() << "Connection: " << connection;
    if(!connection)
        amount = 0;
    else if((connection & Tile::CONNECTION_PUZZLE) &&
            !room->Puzzles.at(room->getTile(location).RoomLink.Puzzle)->Solved()) {
        amount = 0;
        setMessage("There must be something around here...");
    }
    else if((connection & Tile::CONNECTION_ITEM) &&
            !players[index].getItemQuantity(itemList[room->getTile(location).RoomLink.Item])) {

        setMessage("Requires " + itemList[room->getTile(location).RoomLink.Item].Name);
        amount = 0;
    }
    qDebug() << "Amount:" << amount;
    if(amount) {
        if(moveX)
            players[index].X += amount;
        else
            players[index].Y += amount;
        qDebug() << "move";
        return true;
    }
    qDebug() << "No move";
    return false;
}

bool GLWorld::activateTile(int playerIndex) {
    qDebug() << "Activate tile (Player " << playerIndex << ")";
    Tile *t = &room->getTile(players[playerIndex].Y,players[playerIndex].X);
    Link link;
    qDebug() << "Check Puzzles";
    for(unsigned int i = 0; i < room->Puzzles.size(); i++)
        room->Puzzles.at(i)->Activate(players[playerIndex].Y,players[playerIndex].X);
    qDebug() << "Check data";
    switch(t->Data) {
    case Tile::DATA_ROOMEDGE:
        qDebug() << "Room edge";
        for(size_t i = 0; i < room->Puzzles.size(); i++)
            if(!room->Puzzles.at(i)->Solved())
                room->Puzzles.at(i)->Reset();
        link = t->RoomLink;
        players[playerIndex].X = link.Col;
        players[playerIndex].Y = link.Row;
        players[playerIndex].Room = link.Room;
        room = &world->getRoom(link.Room);
        curTileSet = room->getTileSet().Index;
        tileSize = tileSets[curTileSet].getTileSize();
        setRoomPosition();
        break;
    case Tile::DATA_VICTORY:
        qDebug() << "Victory";
        Parent->Push(new GLWindowInfo(victory,false));
        Close();
        return false;
    case Tile::DATA_NONE:
        break;
    default:
        qDebug() << "Item: " << t->Data << " of " << itemList.size()-1;
        Item add = itemList[t->Data];
        add.Count = 1;
        qDebug() << "Add to inventory";
        players[playerIndex].addItem(add);
        t->Data = Tile::DATA_NONE;
        qDebug() << "Set message";
        setMessage(QString("Received %1!").arg(add.Name));
        break;
    }
    return true;
}

void GLWorld::getStartPosition(int &startRow, int &startCol)
{
    if(room->Rows() <= unsigned(tilesHigh))
        startRow = 0;
    else {
        startRow = players[curPlayer].Y - (tilesHigh / 2);
        if(startRow < 0)
            startRow = 0;
        else
            while(int(room->Rows())-startRow < tilesHigh)
                startRow--;
    }
    if(room->Cols() <= unsigned(tilesWide))
        startCol = 0;
    else {
        startCol = players[curPlayer].X - (tilesWide / 2);
        if(startCol < 0)
            startCol = 0;
        else
            while(int(room->Cols())-startCol < tilesWide)
                startCol--;
    }
}

void GLWorld::SaveState()
{
    QString fileName = "saves/" + world->Name + ".dat";
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        throw WE_IO_ERROR;
    QDomDocument d;
    QDomElement root, curChild, newChild;
    root = d.createElement("status");
    root.setAttribute("world",world->Name);
    d.appendChild(root);
    for(unsigned int i = 0; i < players.size(); i++) {
        newChild = d.createElement("player");
        newChild.setAttribute("room",players[i].Room);
        newChild.setAttribute("row",players[i].Y);
        newChild.setAttribute("col",players[i].X);
        for(unsigned int j = 0; j < players[i].numItems(); j++) {
            QDomElement itemData;
            itemData.setTagName("item");
            Item* item = &players[i].getItem(j);
            itemData.setAttribute("id",item->ID);
            itemData.setAttribute("count",item->Count);
        }
        root.appendChild(newChild);
    }
    for(unsigned int i = 0; i < world->Size(); i++) {
        Room* curRoom = &world->getRoom(i);
        for(unsigned int j = 0; j < curRoom->Puzzles.size(); j++) {
            Puzzle* p = curRoom->Puzzles[j];
            curChild = d.createElement("puzzle");
            curChild.setAttribute("room",i);
            curChild.setAttribute("id",j);
            root.appendChild(curChild);
            for(unsigned int r = 0; r < curRoom->Rows(); r++)
                for(unsigned int c = 0; c < curRoom->Cols(); c++) {
                    PuzzleItem* i = p->getItem(r,c);
                    if(i) {
                        newChild = d.createElement("item");
                        newChild.setAttribute("row",r);
                        newChild.setAttribute("col",c);
                        newChild.setAttribute("value",i->Status());
                        curChild.appendChild(newChild);
                    }
                }
        }
    }
    QTextStream out(&file);
    d.save(out,4);
    file.close();
    setMessage("Saved Game");
}

void GLWorld::loadState(const QString &fileName)
{
    QFile f(fileName);
    QDomDocument d;
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
        throw WE_IO_ERROR;
    if(!d.setContent(&f)) {
        f.close();
        qDebug() << "GLWorld XML load error";
        throw WE_INVALID_FILE;
    }
    QDomElement root = d.firstChildElement();
    if(root.attribute("world") != world->Name) {
        Parent->Push(new GLWindowInfo(new GLSplash(Parent,"Invalid save file")));
        Close();
    }
    for(QDomElement child = root.firstChildElement("player"); !child.isNull(); child = child.nextSiblingElement("player")) {
        size_t id = child.attribute("id").toUInt();
        players[id].Room = child.attribute("room").toUInt();
        players[id].Y = child.attribute("row").toUInt();
        players[id].X = child.attribute("col").toUInt();
        for(QDomElement item = child.firstChildElement("item"); !item.isNull(); item = item.nextSiblingElement("item")) {
            Item newItem = itemList[item.attribute("id").toUInt()];
            newItem.Count = item.attribute("count").toInt();
            players[id].addItem(newItem);
        }
    }
    for(QDomElement puzzle = root.firstChildElement("puzzle"); !puzzle.isNull(); puzzle = puzzle.nextSiblingElement("puzzle")) {
        Puzzle* p = world->getRoom(puzzle.attribute("room").toUInt()).Puzzles[puzzle.attribute("id").toUInt()];
        for(QDomElement item = puzzle.firstChildElement(); !item.isNull(); item = item.nextSiblingElement()) {
            PuzzleItem* i = p->getItem(item.attribute("row").toUInt(),item.attribute("col").toUInt());
            if(item.attribute("value").toInt())
                i->Set();
            else
                i->Clear();
        }
    }
    f.close();
    state = STATE_NORMAL;
}

void GLWorld::setMessage(const QString &msg)
{
    message = msg;
    msgCount = 50;
}
