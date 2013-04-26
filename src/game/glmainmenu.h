#ifndef GLMAINMENU_H
#define GLMAINMENU_H

#include "glinterface.h"
#include "glworld.h"
#include "glmenu.h"

class GLMainMenu : public GLInterface
{
public:
    GLMainMenu(GLWindow* parent);
    ~GLMainMenu();
    virtual QPixmap* resizeEvent(QResizeEvent* event);
    virtual QPixmap* paint();
    virtual QPixmap* processKey(int key);
    virtual void Close();
    virtual void Open();
    void setDimensions();
private:
    void populateMenu();
    GLWorld* world;
    GLMenu* menu;
    int tileHeight, tileWidth;
    int titleHeight,titleLine[3];
    int menuHeight;
    QFont menuFont;
};

#endif // GLMAINMENU_H
