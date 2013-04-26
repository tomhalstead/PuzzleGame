#ifndef GLWIDGET_H
#define GLWIDGET_H

enum TransitionType {TRANSITION_NONE};
#include <QGLWidget>
#include <QtGui>
#include <vector>
#include <stack>
#include "glinterface.h"
#include <QKeyEvent>

class GLInterface;

struct GLWindowInfo {
    GLWindowInfo(GLInterface* i, bool destroy = true, TransitionType t = TRANSITION_NONE );
    GLInterface* Interface;
    TransitionType TransitionOnPop;
    bool DestroyOnPop;
};

class GLWindow : public QGLWidget
{
    Q_OBJECT
public:
    GLWindow(QWidget *parent = NULL, bool useMouse = false, QPixmap* Cursor = 0);
    //void Transition(GLInterface* glInterface, TransitionType type);
    GLWindowInfo* Top();
    void Pop();
    void Push(GLWindowInfo window, bool activate = false);
    void Start();
    void Update(QPixmap* p);
    bool isStarted() const;
protected:
    void initializeGL();
    void resizeEvent(QResizeEvent *);
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

signals:
    
public slots:
    void paint();

private:
    void clearInactive();
    void setInterface(TransitionType type = TRANSITION_NONE);
    QPainter painter;
    QPixmap* curImage;
    GLWindowInfo* prevWindow;
    bool inTransition, mouse, topActivated,started;
    QPixmap* cursor;
    std::stack<GLWindowInfo> windows;
    int curKey;
    QTimer* timer;
};

#endif // GLWIDGET_H
