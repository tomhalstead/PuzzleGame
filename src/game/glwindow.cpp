#include "glwindow.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QtGui/QMouseEvent>
#include <GL/glu.h>


GLWindowInfo::GLWindowInfo(GLInterface *i, bool destroy, TransitionType t): Interface(i), TransitionOnPop(t),DestroyOnPop(destroy){}


GLWindow::GLWindow(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent),prevWindow(NULL), inTransition(false), topActivated(false),started(false), curKey(0), timer(NULL)
{
    this->setCursor(QCursor(Qt::BlankCursor));
    setAutoFillBackground(false);
}

GLWindow::~GLWindow()
{
    if(prevWindow)
        delete prevWindow;
    while(!windows.empty()) {
        if(windows.top()->DestroyOnPop)
            delete windows.top();
        windows.pop();
    }
}

GLWindowInfo* GLWindow::Top()
{
    if(windows.empty())
        return NULL;
    else
        return windows.top();
}

void GLWindow::resizeEvent(QResizeEvent *event) {
    if(started && Top())
        Top()->Interface->resizeEvent(event);
}

void GLWindow::initializeGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0,0,0,1);
    setAutoBufferSwap(true);
    glLoadIdentity();
}

void GLWindow::keyPressEvent(QKeyEvent* event) {
    curKey = event->key();
}

void GLWindow::keyReleaseEvent(QKeyEvent *event)
{
    curKey = 0;
}

void GLWindow::paint()
{
    if(curImage) {
        painter.begin(this);
        Top()->Interface->paint(painter);
        painter.end();
    }
}

void GLWindow::process()
{
    if(windows.empty() || !topActivated)
        return;
    GLInterface* top = Top()->Interface;
    if(!top->isOpen())
        Pop();
    else {
        if(!curKey || top->processKey(curKey))
            paint();
        curKey = 0;
    }
}

void GLWindow::Push(GLWindowInfo* window, bool activate, TransitionType t)
{
    topActivated = activate;
    if(activate) {
        topActivated = true;
        if(!windows.empty() && Top()->Interface->isOpen())
            prevWindow = Top();
        windows.push(window);
        Top()->Interface->Open();
        beginTransition(t);
    }
    else
        windows.push(window);

}

void GLWindow::Start() {
    if(!timer) {
        timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(process()));
        timer->start();
        started = true;
    }
}

bool GLWindow::isStarted() const
{
    return started;
}

void GLWindow::Clear()
{
    painter.begin(this);
    painter.fillRect(0,0,this->width(),this->height(),Qt::black);
    painter.end();
}

void GLWindow::beginTransition(TransitionType type)
{
    switch(type) {
    case TRANSITION_NONE:
        if(prevWindow) {
            //if(prevWindow->DestroyOnPop)
                //delete prevWindow->Interface;
            //delete prevWindow;
        //prevWindow = NULL;
        }
        break;
    }
}

void GLWindow::Pop()
{
    prevWindow = Top();
    windows.pop();
    while(Top() && !Top()->Interface->isOpen()) {
        if(windows.top()->DestroyOnPop)
            delete windows.top()->Interface;
        delete windows.top();
        windows.pop();
    }
    qDebug() << "inactive items cleared: windows.size=" << windows.size();
    if(started && windows.empty()) {
        delete prevWindow->Interface;
        delete prevWindow;
        prevWindow = NULL;
        timer->stop();
        close();
    }
    else {
        if(prevWindow->TransitionOnPop == TRANSITION_NONE) {
            if(prevWindow->DestroyOnPop)
                delete prevWindow->Interface;
            delete prevWindow;
            prevWindow = NULL;
        }
        else
            beginTransition(prevWindow->TransitionOnPop);
    }
    qDebug() << "End pop";
}
