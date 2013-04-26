#include "glsplash.h"

GLSplash::GLSplash(GLWindow *parent, const QString &text, QPixmap *bg, const QColor& fg): GLInterface(parent), Text(text), background(bg) {
    Pen.setColor(fg);
    Font.setPixelSize(32);
    if(parent && parent->width() && parent->height()) {
        resizeEvent(NULL);
        parent->repaint();
    }

}

QPixmap *GLSplash::resizeEvent(QResizeEvent *event)
{
    GLInterface::resizeEvent(event);
    return paint();
}

GLSplash::~GLSplash()
{
    if(background)
        delete background;
}

QPixmap *GLSplash::paint()
{
    if(Display && isOpen()) {
        Painter.begin(Display);
        Painter.setPen(Pen);
        Painter.setFont(Font);
        Painter.fillRect(Parent->rect(),Qt::black);
        if(background)
            Painter.drawPixmap(0,0,*background);
        Painter.drawText(Parent->rect(),Text,QTextOption(Qt::AlignCenter));
        Painter.end();
    }
    return Display;
}

QPixmap *GLSplash::processKey(int key)
{
    if(key == Qt::Key_Space)
        Close();
    return NULL;
}

void GLSplash::Close()
{
    qDebug() << "Close Splash";
    GLInterface::Close();
}

void GLSplash::Open()
{
    qDebug() << "Open Splash";
    GLInterface::Open();
}
