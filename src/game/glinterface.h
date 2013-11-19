#ifndef GLINTERFACE_H
#define GLINTERFACE_H

#include <QtGui>
#include "glwindow.h"

enum InterfaceState {STATE_INIT, STATE_NORMAL, STATE_DIALOG, STATE_ANIMATE, STATE_TRANSTION};
class GLWindow;

class GLInterface
{
public:
    GLInterface(GLWindow* parent);
    virtual ~GLInterface();
    virtual bool resizeEvent(QResizeEvent *event);
    virtual void paint(QPainter& painter)=0;
    virtual bool processKey(int key)=0;
    virtual void Close();
    virtual void Open();
    bool isOpen() const;
    int Height() const;
    int Width() const;
protected:
    void setFontHeight(QFont& font, int target);
    GLWindow* Parent;
    QPen Pen;
    QFont Font;
private:
    bool active;
    int height, width;
};

#endif // GLINTERFACE_H
