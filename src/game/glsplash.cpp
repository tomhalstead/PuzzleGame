#include "glsplash.h"

GLSplash::GLSplash(GLWindow *parent, const QString &text, QPixmap *bg, const QColor& fg): GLInterface(parent), Text(text), background(bg) {
    Pen.setColor(fg);
    Font.setPixelSize(32);
    if(parent && parent->width() && parent->height()) {
        resizeEvent(NULL);
        parent->repaint();
    }

}

bool GLSplash::resizeEvent(QResizeEvent *event)
{
    return GLInterface::resizeEvent(event);
}

GLSplash::~GLSplash()
{
    if(background)
        delete background;
}

void GLSplash::paint(QPainter& painter)
{
    painter.setPen(Pen);
    painter.setFont(Font);
    painter.fillRect(Parent->rect(),Qt::black);
    if(background)
        painter.drawPixmap((Width()-background->width())/2,(Height()-background->height())/2,*background);
    painter.drawText(Parent->rect(),Text,QTextOption(Qt::AlignCenter));
}

bool GLSplash::processKey(int key)
{
    if(key == Qt::Key_Space ||
            key == Qt::Key_Escape ||
            key == Qt::Key_Enter  ||
            key == Qt::Key_Return) {
        Close();
        return false;
    }
    return true;
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
