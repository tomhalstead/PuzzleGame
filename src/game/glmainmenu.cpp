#include "glmainmenu.h"

const int tileSize = 16;

GLMainMenu::GLMainMenu(GLWindow *parent): GLInterface(parent),topMenu(NULL) {
    menuFont = Font;
    Pen.setColor(QColor::fromRgb(255,255,255));
    QDir check;
    if(!check.exists("saves"))
        if(!check.mkdir("saves"))
            Parent->Push(new GLWindowInfo(new GLSplash(Parent,"Error creating save directory.  Ensure that the folder you are using is not read-only.")));
    if(!check.exists("campaigns"))
        if(!check.mkdir("campaigns"))
            Parent->Push(new GLWindowInfo(new GLSplash(Parent,"Error creating campaign directory.  Ensure that the folder you are using is not read-only.")));
}

GLMainMenu::~GLMainMenu()
{
    if(topMenu)
        delete topMenu;
    if(subMenu)
        delete subMenu;
}

bool GLMainMenu::resizeEvent(QResizeEvent* event)
{
    if(GLInterface::resizeEvent(NULL)) {
        setDimensions();
        return true;
    }
    return false;
}

void GLMainMenu::paint(QPainter& painter) {
    if(topMenu && isOpen()) {
        QDir check("saves");
        topMenu->at(1).Enabled = check.exists("default.dat");
        painter.setPen(Pen);
        painter.setFont(Font);
        painter.fillRect(0,0,Width(),Height(),Qt::black);
        painter.drawText(0,titleLine[0],Width(),titleHeight,Qt::AlignCenter,"PUZZLE");
        painter.drawText(0,titleLine[1],Width(),titleHeight,Qt::AlignCenter,"RPG");
        painter.drawText(0,titleLine[2],Width(),titleHeight,Qt::AlignCenter,"ENGINE");
        painter.setFont(menuFont);
        topMenu->Draw(painter,Pen,QColor::fromRgb(255,255,0));
    }
}

bool GLMainMenu::processKey(int key) {
    switch(key) {
    case Qt::Key_Escape:
        Close();
        return false;
    case Qt::Key_Return:
    case Qt::Key_Enter:
    case Qt::Key_Space:
        processMenuItem();
        break;
    case Qt::Key_S:
    case Qt::Key_Down:
    case Qt::Key_F:
        if(topMenu)
            ++(*topMenu);
        break;
    case Qt::Key_A:
    case Qt::Key_Up:
    case Qt::Key_W:
        if(topMenu)
            --(*topMenu);
        break;
    }
    return true;
}

void GLMainMenu::Close()
{
    qDebug() << "Close MainMenu";
    GLInterface::Close();
}

void GLMainMenu::Open()
{
    qDebug() << "Open MainMenu";
    GLInterface::Open();
}

void GLMainMenu::setDimensions() {
    tileWidth = Width() / tileSize;
    tileHeight = Height() / tileSize;
    titleHeight = (Height() - 80) /8;
    menuHeight = titleHeight / 4;
    setFontHeight(Font,titleHeight);
    setFontHeight(menuFont,menuHeight);
    for(int i = 0; i < 3; i++)
        titleLine[i] = (16*(i+1)) + (titleHeight * (i+1));
    int menuTop = titleLine[2] + (titleHeight*2) + menuHeight + 16;
    QRect r(0,menuTop,Width(),Height()-menuTop-16);
    if(!topMenu) {
        topMenu = new GLMenu(menuHeight,r);
        populateMenu();
    }
    else
        topMenu->Resize(r);
}

void GLMainMenu::populateMenu()
{
    if(!topMenu)
        return;
    QDir check("saves");
    topMenu->Add(MenuItem("New",true,0));
    topMenu->Add(MenuItem("Continue",check.exists("default.dat"),1));
    topMenu->Add(MenuItem("Puzzle Demo",check.exists("default.dat"),5));
    topMenu->Add(MenuItem("Custom",false,2));
    topMenu->Add(MenuItem("Multiplayer",false,3));
    topMenu->Add(MenuItem("Quit",true,4));
}

void GLMainMenu::processMenuItem()
{
    QString saveFile = "";
    switch(topMenu->CurrentItem().ID) {
    case 1:
        qDebug() << "Continue";
        saveFile = "saves/default.dat";
    case 0:
        try {
            qDebug() << "loading world";
            world = new GLWorld(Parent);
            world->loadWorld(":/campaign/default.dat",0,saveFile);
            Parent->Push(new GLWindowInfo(world));
        }
        catch(WORLD_ERROR e) {
            switch(e) {
            case WE_IO_ERROR:
                Parent->Push(new GLWindowInfo(new GLSplash(Parent,"ERROR READING CAMPAIGN FILE")));
                qDebug() << "IO ERROR";
                break;
            case WE_INVALID_INDEX:
                qDebug() << "INVALID INDEX";
                Parent->Push(new GLWindowInfo(new GLSplash(Parent,"INVALID CAMPAIGN FILE")));
                break;
            case WE_INVALID_FILE:
                qDebug() << "INVALID FILE";
                Parent->Push(new GLWindowInfo(new GLSplash(Parent,"INVALID CAMPAIGN FILE")));
                break;
            }
        }
        break;
    case 4:
        qDebug() << "Quit";
        Close();
        break;
    case 5:
        try {
            qDebug() << "Loading demo";
            world = new GLWorld(Parent);
            world->loadWorld(":/campaign/demo.dat",0,saveFile);
            Parent->Push(new GLWindowInfo(world));
        }
        catch(WORLD_ERROR e) {
            switch(e) {
            case WE_IO_ERROR:
                Parent->Push(new GLWindowInfo(new GLSplash(Parent,"ERROR READING CAMPAIGN FILE")));
                qDebug() << "IO ERROR";
                break;
            case WE_INVALID_INDEX:
                qDebug() << "INVALID INDEX";
                Parent->Push(new GLWindowInfo(new GLSplash(Parent,"INVALID CAMPAIGN FILE")));
                break;
            case WE_INVALID_FILE:
                qDebug() << "INVALID FILE";
                Parent->Push(new GLWindowInfo(new GLSplash(Parent,"INVALID CAMPAIGN FILE")));
                break;
            }
        }
        break;
    default:
        qDebug() << "OTHER";
        break;
    }
}
