#ifndef GLWIDGET_H
#define GLWIDGET_H

enum TransitionType {TRANSITION_NONE};
#include <QGLWidget>
#include <QtGui>
#include <vector>
#include <stack>
#include "glinterface.h"


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
    GLWindow(QWidget *parent = NULL);
    ~GLWindow();
    GLWindowInfo* Top();
    void Pop();
    void Push(GLWindowInfo *window, bool activate = true, TransitionType t = TRANSITION_NONE);
    void Start();
    void Update(QPixmap* p);
    bool isStarted() const;
    void Clear();
protected:
    void initializeGL();
    void resizeEvent(QResizeEvent *);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

signals:
    
private slots:
    void process();

private:
    void paint();
    void beginTransition(TransitionType type = TRANSITION_NONE);
    QPainter painter;
    QPixmap* curImage;
    QPixmap* prevImage;
    GLWindowInfo* prevWindow;
    bool inTransition, topActivated, started;
    std::stack<GLWindowInfo*> windows;
    int curKey;
    QTimer* timer;
};

#endif // GLWIDGET_H
