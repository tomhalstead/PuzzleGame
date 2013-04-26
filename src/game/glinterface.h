#ifndef GLINTERFACE_H
#define GLINTERFACE_H

#include <QtGui>
#include "glwindow.h"

class GLWindow;

class GLInterface
{
public:
    GLInterface(GLWindow* parent);
    virtual ~GLInterface();
    virtual QPixmap* resizeEvent(QResizeEvent *event);
    virtual QPixmap* paint()=0;
    virtual QPixmap* processKey(int key)=0;
    virtual void Close();
    virtual void Open();
    bool isOpen() const;
    int Height() const;
    int Width() const;
protected:
    void setFontHeight(QFont& font, int target);
    void destroyDisplay();
    GLWindow* Parent;
    QPixmap* Display;
    QPainter Painter;
    QPen Pen;
    QFont Font;
private:
    bool active;
    int height, width;
};

#endif // GLINTERFACE_H
