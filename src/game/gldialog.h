#ifndef GLDIALOG_H
#define GLDIALOG_H

#include "glinterface.h"
#include "glmenu.h"

typedef void (*DIALOGFUNC)(GLInterface*,int);

class GLDialog: public GLInterface
{
public:
    GLDialog(GLWindow* parent, GLInterface* owner, const QStringList& menuItems, DIALOGFUNC callBack,
             const QRect& pos,int fontSize=40,const QColor& deselected = Qt::white, const QColor &selected = Qt::yellow);
    ~GLDialog();
    virtual void paint(QPainter& painter);
    virtual bool processKey(int key);
private:
    GLInterface* caller;
    QColor selColor, deselColor;
    DIALOGFUNC call;
    GLMenu* menu;
    QRect position;
};

#endif // GLDIALOG_H
