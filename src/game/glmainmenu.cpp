#include "glmainmenu.h"

const int tileSize = 16;

GLMainMenu::GLMainMenu(GLWindow *parent): GLInterface(parent),menu(NULL) {
    menuFont = Font;
    Pen.setColor(QColor::fromRgb(255,255,255));
}

GLMainMenu::~GLMainMenu()
{
    qDebug() << "~GLMainMenu";
    if(world)
        delete world;
    if(menu)
        delete menu;
}

QPixmap* GLMainMenu::resizeEvent(QResizeEvent* event)
{
    qDebug() << "Main menu resize";
    if(GLInterface::resizeEvent(NULL))
        setDimensions();
    qDebug() << "Resize complete, Paint.";
    return paint();
}

QPixmap* GLMainMenu::paint() {
    if(menu && isOpen()) {
        Painter.begin(Display);
        Painter.setPen(Pen);
        Painter.setFont(Font);
        Painter.fillRect(0,0,Width(),Height(),Qt::black);
        Painter.drawText(0,titleLine[0],Width(),titleHeight,Qt::AlignCenter,"PUZZLE");
        Painter.drawText(0,titleLine[1],Width(),titleHeight,Qt::AlignCenter,"RPG");
        Painter.drawText(0,titleLine[2],Width(),titleHeight,Qt::AlignCenter,"ENGINE");
        Painter.setFont(menuFont);
        menu->Draw(Painter,Pen,QColor::fromRgb(255,255,0));
        Painter.end();
    }
    return Display;
}

QPixmap* GLMainMenu::processKey(int key) {
    bool redraw = false;
    switch(key) {
    case Qt::Key_Escape:
        Close();
        break;
    case Qt::Key_Return:
    case Qt::Key_Enter:
        switch(menu->CurrentItem()) {
        case 0:
            try {
                if(world)
                    delete world;
                world = new GLWorld(Parent,this);
                world->loadWorld("default.map");
                world->Open();
            }
            catch(WORLD_ERROR e) {
                switch(e) {
                case WE_IO_ERROR:
                    qDebug() << "IO ERROR";
                    break;
                case WE_INVALID_INDEX:
                    qDebug() << "INVALID INDEX";
                    break;
                case WE_INVALID_FILE:
                    qDebug() << "INVALID FILE";
                    break;
                }
                exit(1);
            }
        default:
            break;
        }
        break;
    case Qt::Key_S:
    case Qt::Key_Down:
    case Qt::Key_F:
        if(menu) {
            ++(*menu);
            redraw = true;
        }
        break;
    case Qt::Key_A:
    case Qt::Key_Up:
    case Qt::Key_W:
        if(menu) {
            --(*menu);
            redraw = true;
        }
        break;
    }
    if(redraw)
        return paint();
    else
        return NULL;
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
    if(!menu) {
        menu = new GLMenu(menuHeight,r);
        populateMenu();
    }
    else
        menu->Resize(r);
}

void GLMainMenu::populateMenu()
{
    if(!menu)
        return;
    menu->Add(MenuItem("New",true));
    menu->Add(MenuItem("Load",false));
    menu->Add(MenuItem("Custom",true));
    menu->Add(MenuItem("Multiplayer",false));

}
