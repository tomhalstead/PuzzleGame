#ifndef GLSPLASH_H
#define GLSPLASH_H

#include "glinterface.h"

class GLSplash : public GLInterface
{
public:
    GLSplash(GLWindow* parent, const QString& text, QPixmap* bg = 0, const QColor &fg = Qt::white);
    virtual ~GLSplash();
    virtual QPixmap* resizeEvent(QResizeEvent *event);
    virtual QPixmap* paint();
    virtual QPixmap* processKey(int key);
    virtual void Close();
    virtual void Open();
private:
    QString Text;
    QPixmap* background;
};

#endif // GLSPLASH_H
