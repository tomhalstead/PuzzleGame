#ifndef GLMAINMENU_H
#define GLMAINMENU_H

#include <QDir>
#include "glinterface.h"
#include "glworld.h"
#include "glmenu.h"

class GLMainMenu : public GLInterface
{
public:
    GLMainMenu(GLWindow* parent);
    ~GLMainMenu();
    virtual bool resizeEvent(QResizeEvent* event);
    void paint(QPainter& painter);
    virtual bool processKey(int key);
    virtual void Close();
    virtual void Open();
    void setDimensions();
private:
    void populateMenu();
    void processMenuItem();
    GLWorld* world;
    GLMenu *topMenu, *subMenu;
    int tileHeight, tileWidth;
    int titleHeight,titleLine[3];
    int menuHeight;
    QFont menuFont;
};

#endif // GLMAINMENU_H
