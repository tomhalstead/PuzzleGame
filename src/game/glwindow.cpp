#include "glwindow.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QtGui/QMouseEvent>
#include <GL/glu.h>


GLWindowInfo::GLWindowInfo(GLInterface *i, bool destroy, TransitionType t): Interface(i), TransitionOnPop(t),DestroyOnPop(destroy){}


GLWindow::GLWindow(QWidget *parent, bool useMouse, QPixmap *Cursor)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent),prevWindow(NULL), inTransition(false), mouse(useMouse), topActivated(false),started(false), cursor(Cursor), curKey(0), timer(NULL)
{
    if(!useMouse)
        this->setCursor(QCursor(Qt::BlankCursor));
    setAutoFillBackground(false);
}

GLWindowInfo* GLWindow::Top()
{
    if(windows.size())
        return &windows.top();
    else
        return NULL;
}

void GLWindow::resizeEvent(QResizeEvent *event) {
    if(started && Top())
        Top()->Interface->resizeEvent(event);
}

void GLWindow::paintEvent(QPaintEvent *event)
{
    /*
    if(started) {
        clearInactive();
        GLInterface* curInterface = Top()->Interface;
        if(curInterface) {
            curImage = curInterface->paintEvent(event);
            if(curImage)
                paint();
        }
    }
    */
}

void GLWindow::initializeGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0,0,0,1);
    setAutoBufferSwap(true);
    glLoadIdentity();
}

void GLWindow::mousePressEvent(QMouseEvent *event) {
    /*
    if(started) {
        QPixmap* result;
        GLInterface* curInterface = Top()->Interface;
        clearInactive();
        if( curInterface && (result = curInterface->mousePressEvent(event)) ) {
            curImage = result;
            paint();
        }
    }
    */
}
void GLWindow::mouseMoveEvent(QMouseEvent *event) {
    /*
    if(started) {
        clearInactive();
        QPixmap* result;
        GLInterface* curInterface = Top()->Interface;
        if(curInterface && (result = curInterface->mouseMoveEvent(event)) ) {
            curImage = result;
            paint();
        }
    }
    */
}

void GLWindow::keyPressEvent(QKeyEvent* event) {
    curKey = event->key();
    qDebug() << "Set key: " << curKey;
}

void GLWindow::keyReleaseEvent(QKeyEvent *event)
{
    qDebug() << "Clear Key";
    curKey = 0;
}

void GLWindow::paint()
{
    qDebug() << "Call paint: " << Top()->Interface;
    GLInterface* top = Top()->Interface;
    top->processKey(curKey);
    curKey = 0;
    clearInactive();
    curImage = top->paint();
    if(curImage) {
        painter.begin(this);
        painter.drawPixmap(0,0,width(),height(),*curImage);
        painter.end();
    }
}

void GLWindow::Push(GLWindowInfo window, bool activate)
{
    topActivated = false;
    windows.push(window);
    if(activate) {
        topActivated = true;
        Top()->Interface->Open();
    }

}

void GLWindow::Start() {
    if(!timer) {
        timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(paint()));
        timer->start();
        started = true;
    }
}

bool GLWindow::isStarted() const
{
    return started;
}

void GLWindow::setInterface(TransitionType type)
{
    switch(type) {
    case TRANSITION_NONE:
        if(prevWindow) {
            if(prevWindow->DestroyOnPop)
                delete prevWindow;
            prevWindow = NULL;
        }
        break;
    }
}

void GLWindow::Pop()
{
    qDebug() << "Pop";
    prevWindow = Top();
    windows.pop();
    setInterface(prevWindow->TransitionOnPop);
    if(started && windows.empty()) {
        qDebug() << "Close program";
        timer->stop();
        close();
    }
    qDebug() << "End pop";
}

void GLWindow::clearInactive()
{
    qDebug() << "clearInactive: " << windows.size() << " items on stack.";
    if(topActivated) {
        while(Top() && !Top()->Interface->isOpen()) {
            Pop();
        }
        qDebug() << "\tRemoved items: " << windows.size() << " items on stack.";
    }
    else {
        topActivated = true;
        Top()->Interface->Open();
    }
    qDebug() << "Exit inactive check";
}
