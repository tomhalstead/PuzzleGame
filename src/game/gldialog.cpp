#include "gldialog.h"

GLDialog::GLDialog(GLWindow *parent, GLInterface *owner, const QStringList &menuItems,
                   DIALOGFUNC callBack, const QRect &pos, int fontSize,
                   const QColor &deselected, const QColor &selected):
                        GLInterface(parent),caller(owner),selColor(selected),
    deselColor(deselected), call(callBack),menu(NULL),position(pos)
{
    qDebug() << "GLDialog init: " << fontSize << "px font, window size:" << pos.left() << "," << pos.top() << pos.width() << "x" << pos.height() ;
    Pen.setColor(deselColor);
    Font.setPixelSize(fontSize);
    setFontHeight(Font,fontSize);
    qDebug() << "create menu";
    menu = new GLMenu(fontSize,QRect(position.left()+10,position.top()+10,position.width()-20,position.height()-20));
    qDebug() << "add menu items";
    for(int i = 0; i < menuItems.size(); i++)
        menu->Add(MenuItem(menuItems[i],true,i));
}

GLDialog::~GLDialog()
{
    if(menu)
        delete menu;
}

void GLDialog::paint(QPainter& painter)
{
    painter.setPen(Pen);
    painter.setFont(Font);
    painter.fillRect(position,Qt::black);
    menu->Draw(painter,Pen,selColor);
}

bool GLDialog::processKey(int key)
{
    switch(key) {
    case Qt::Key_Escape:
        Close();
        (*call)(caller,-1);
        break;
    case Qt::Key_Return:
    case Qt::Key_Enter:
    case Qt::Key_Space:
        Close();
        (*call)(caller,menu->CurrentItem().ID);
        break;
    case Qt::Key_S:
    case Qt::Key_D:
    case Qt::Key_Down:
    case Qt::Key_Right:
        if(menu)
            ++(*menu);
        break;
    case Qt::Key_A:
    case Qt::Key_Up:
    case Qt::Key_W:
    case Qt::Key_Left:
        if(menu)
            --(*menu);
        break;
    }
    return true;
}
