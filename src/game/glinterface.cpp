#include "glinterface.h"

GLInterface::GLInterface(GLWindow *parent): Parent(parent),active(false){
    int id = QFontDatabase::addApplicationFont(":/font/16bit.ttf");
    Font = QFont(QFontDatabase::applicationFontFamilies(id).at(0));
}

GLInterface::~GLInterface () {}

bool GLInterface::resizeEvent(QResizeEvent *event)
{
    bool changed = false;
    if(active) {
        if(height != Parent->height()) {
            height = Parent->height();
            changed = true;
        }
        if(width != Parent->width()) {
            width = Parent->width();
            changed = true;
        }
    }
    return changed;
}

void GLInterface::Close()
{
    if(active) {
        active = false;
        Parent->repaint();
    }
}

void GLInterface::Open()
{
    if(!active) {
        active = true;
        resizeEvent(NULL);
    }
}


int GLInterface::Height() const
{
    return height;
}

int GLInterface::Width() const
{
    return width;
}

bool GLInterface::isOpen() const {
    return active;
}

void GLInterface::setFontHeight(QFont &font, int target)
{
    if(target < 1)
        font.setPixelSize(1);
    else {
        font.setPixelSize(target);
        QFontMetrics* m = new QFontMetrics(font);
        while(m->height() > target && font.pixelSize() > 1) {
            font.setPixelSize(font.pixelSize()-1);
            delete m;
            m = new QFontMetrics(font);
        }
    }
}
